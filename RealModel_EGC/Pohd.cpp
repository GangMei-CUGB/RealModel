#include "StdAfx.h"
#include "Pohd.h"


CPohd::CPohd(void)
{
}


CPohd::~CPohd(void)
{
}

void CPohd::Clear(void)
{
	if(!iNds.IsEmpty()) {
		iNds.RemoveAll();
	}

	if(!iLps.IsEmpty()) {
		iLps.RemoveAll();
	}

	if(!iNbs.IsEmpty()) {
		iNbs.RemoveAll();
	}
	
	while(!pNds.IsEmpty()) {
		pNds.GetAt(0)->Clear();
		delete pNds.GetAt(0);  pNds.RemoveAt(0);
	}

	while(!pLps.IsEmpty()) {
		pLps.GetAt(0)->Clear();
		delete pLps.GetAt(0);  pLps.RemoveAt(0);
	}

	while(!pNbs.IsEmpty()) {
		pNbs.GetAt(0)->Clear();
		delete pNbs.GetAt(0);  pNbs.RemoveAt(0);
	}
}