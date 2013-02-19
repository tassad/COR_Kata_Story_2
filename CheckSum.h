#pragma once

#include "defs.h"
#include "emmintrin.h"

class CCheckSum
{

public:

  /// construction

	// default constructor
	//
	CCheckSum(void);


	// default distructor
	//
	~CCheckSum(void);


	/// operations

	// validate account by calculate the check sum... SSE2 accelerated 
	//
	char ValidateAccount(Account Acc);

private:

	/// member variables

	// 128 bit: 8 "unsigned short" vector to process account's 8 digits
	//
	__m128i m_vAcc;


	// 128 bit: 8 "unsigned short" vector holds checksum 8 digits { 9, 8, 7, 6, 5, 4, 3, 2}
	//
	__m128i m_vCalc;


	// 128 bit: 8 "unsigned short" vector holds checksum process partial results
	//
	__m128i m_vRes;

};

