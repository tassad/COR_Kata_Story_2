#include "FileManager.h"
#include "Digitize.h"
#include "CheckSum.h"


int main( int argc, char *argv[])
{
  // operation objects declaration
	//
	CFileManager	srcFile;	// handles source file 
	CDigitize		parser;		// handles optical chars to digital operations
	CCheckSum		validator;	// handles checksum operations


	// data buffers declaration
	//
	unsigned char	*optical_src_buf;		// scanned (source) data source buffer
	Account			*acc_bit_presentation;	// bit presentation of scanned account set
	Account			*acc_digital;			// didgital copy, reconized account set


	// local variables
	//
	int ret = 1;		// main() return value

	int srcFileSize;
	int lineLength, lineTerm;
	int numberOfAcc;


	// validate commandline
	//
	if(argc != 2)
	{
		printf("\nerror file name");
		return ret;
	}
	

	// get source file name and open it
	//
	if(srcFile.OpenSrc(argv[1]) == -1)
	{
		printf("\nerror open file");
		return ret;
	}

	
	// get source file size
	//
	srcFileSize = srcFile.GetSize();


	// make sure that file has at least 1 account to process
	//
	if(srcFileSize < (LINE_CHARS * ACC_OLINES) )
	{
		printf("\nerror file data");
		return ret;
	}


	/// calculate src buffer size

	//1- Find Line length: check for line termination chars 10-13, then use appropriete line size
	//2- Find number of accounts in file: fileSize/ (Line length * 4)

	lineTerm = srcFile.LineTermCount();
	lineLength = LINE_CHARS + lineTerm;

	numberOfAcc = srcFileSize / (lineLength * ACC_OLINES);	//fileSize/ (Line length * 4)
	

	/// buffers init

	optical_src_buf			= new unsigned char [(lineLength * ACC_OLINES * numberOfAcc)+1];

	acc_bit_presentation	= new Account[numberOfAcc];
	acc_digital				= new Account[numberOfAcc];


	// load source file data to buffer
	//
	srcFile.LoadScanData(optical_src_buf, (lineLength * ACC_OLINES * numberOfAcc));


	/// process optical data from file:
	
	// transform to bit presntaion form
	//
	parser.Transform(optical_src_buf, acc_bit_presentation, lineLength, numberOfAcc, lineTerm);
	
	// translate bit presentation chars into digital values (ascii)
	//
	parser.Translate(acc_bit_presentation, acc_digital, numberOfAcc);

	
	for(int res = 0; res < numberOfAcc; res++)
	{
		/// calc checksum for known digits only
		
		if(acc_digital[res].status > -1)	
		{
			acc_digital[res].status = validator.ValidateAccount(acc_digital[res]);	// 0= valid; 1= invalid
		}
				
		/// test preview results
		
		printf("%d: %c%c%c%c%c%c%c%c%c  %d\n", res+1, acc_digital[res].dSrc[0] +48, acc_digital[res].dSrc[1]+48, acc_digital[res].dSrc[2]+48, acc_digital[res].dSrc[3]+48, acc_digital[res].dSrc[4]+48, acc_digital[res].dSrc[5]+48, acc_digital[res].dSrc[6]+48, acc_digital[res].dSrc[7]+48, acc_digital[res].dSrc[8]+48, acc_digital[res].status);		
	}


	///cleanup
	
	ret = 0;

	delete[] acc_digital;
	delete[] acc_bit_presentation;	
	delete[] optical_src_buf;

	printf("\nOCR done\n");
	
	return ret;
}
