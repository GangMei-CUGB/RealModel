#include "StdAfx.h"
#include "Count.h"


CCount::CCount(void)
{
}

CCount::CCount(int CI, int CJ, int CK)
{
	ci = CI;
	cj = CJ;
	ck = CK;
}

CCount::CCount(int CI)
{
	ci = CI;
}

CCount::CCount(int CI, int CJ)
{
	ci = CI;
	cj = CJ;
}

CCount::~CCount(void)
{
}

void CCount::Clear(void)
{
	if(!m_aFstEle.IsEmpty()) {
		m_aFstEle.RemoveAll();
	}

	if(!m_aSndEle.IsEmpty()) {
		m_aSndEle.RemoveAll();
	}
}
