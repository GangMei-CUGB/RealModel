#include "StdAfx.h"
#include "Loop.h"


CLoop::CLoop(void)
{
}

CLoop::CLoop(int id, int flag, int n, double* x, double* y, double* z)
{
	ID = id;  flag = flag;
	for(int i = 0; i < n; i++)
		pNds.Add(new CNode(i, x[i], y[i], z[i]));
}

CLoop::~CLoop(void)
{
}

void CLoop::Clear(void)
{
	if(!iVPLn.IsEmpty()) {
		iVPLn.RemoveAll();
	}

	if(!iNds.IsEmpty()) {
		iNds.RemoveAll();
	}

	while(!pNds.IsEmpty()) {
		pNds.GetAt(0)->Clear();
		delete pNds.GetAt(0);  pNds.RemoveAt(0);
	}

	while(!pTrs.IsEmpty()) {
		pTrs.GetAt(0)->Clear();
		delete pTrs.GetAt(0);  pTrs.RemoveAt(0);
	}

	while(!pEgs.IsEmpty()) {
		pEgs.GetAt(0)->Clear();
		delete pEgs.GetAt(0);  pEgs.RemoveAt(0);
	}

	while(!pPLns.IsEmpty()) {
		pPLns.GetAt(0)->Clear();
		delete pPLns.GetAt(0);  pPLns.RemoveAt(0);
	}

}
