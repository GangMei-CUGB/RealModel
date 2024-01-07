#include "StdAfx.h"
#include "Line.h"

CLine::CLine(void)
{
	UseTime = 0;
}

CLine::CLine(int id, int* nid, int ut)
{
	ID = id; nID[0] = nid[0]; nID[1] = nid[1]; UseTime = ut;
}
CLine::CLine(int nid1, int nid2)
{
	nID[0] = nid1; nID[1] = nid2; 
}

CLine::~CLine(void)
{
}

void CLine::Clear(void)
{
	if(!sPsdTrgl.IsEmpty()) {
		sPsdTrgl.RemoveAll();
	}

}