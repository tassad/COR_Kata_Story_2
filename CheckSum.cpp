#include "CheckSum.h"


CCheckSum::CCheckSum(void)
{
  short int cons[8] = { 9, 8, 7, 6, 5, 4, 3, 2};
	m_vCalc = _mm_setr_epi16(	cons[0],
								cons[1],
								cons[2],
								cons[3],
								cons[4],
								cons[5],
								cons[6],
								cons[7]);
}


CCheckSum::~CCheckSum(void)
{
}


char CCheckSum::ValidateAccount(Account Acc)
{
	char ret = 0;
	long int val = 0;
	// load vetors
	m_vAcc = _mm_setr_epi16(Acc.dSrc[0],
							Acc.dSrc[1],
							Acc.dSrc[2],
							Acc.dSrc[3],
							Acc.dSrc[4],
							Acc.dSrc[5],
							Acc.dSrc[6],
							Acc.dSrc[7]);

	// make the calc
	m_vRes = _mm_madd_epi16(m_vAcc, m_vCalc);

	// unrolled loop
	val = Acc.dSrc[8] + m_vRes.m128i_u32[0] + m_vRes.m128i_u32[1] + m_vRes.m128i_u32[2] + m_vRes.m128i_u32[3];

	if( (val % 11) != 0 )
	{
		ret = 1;		
	}

	return ret;
}
