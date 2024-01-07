#pragma once

#include<afxtempl.h>

class CCount
{
public:
	CArray<int,int>m_aFstEle;
	CArray<int,int>m_aSndEle;
	int ci, cj, ck;
	int ik;

	CCount(void);
	CCount(int CI, int CJ, int CK);
	CCount(int CI, int CJ);
	CCount(int CI);

	void Clear(void);
	~CCount(void);
};

