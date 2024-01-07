#include "StdAfx.h"
#include "Block.h"


CBlock::CBlock(void)
{
}


CBlock::~CBlock(void)
{
}

void CBlock::Clear(void)
{
	if(!iPhds.IsEmpty()) {
		iPhds.RemoveAll();
	}

	while(!pNds.IsEmpty()) {
		pNds.GetAt(0)->Clear();
		delete pNds.GetAt(0);  pNds.RemoveAt(0);
	}

	while(!pTrs.IsEmpty()) {
		pTrs.GetAt(0)->Clear();
		delete pTrs.GetAt(0);  pTrs.RemoveAt(0);
	}

	while(!pQds.IsEmpty()) {
		pQds.GetAt(0)->Clear();
		delete pQds.GetAt(0);  pQds.RemoveAt(0);
	}
	
	while(!pLps.IsEmpty()) {
		pLps.GetAt(0)->Clear();
		delete pLps.GetAt(0);  pLps.RemoveAt(0);
	}

	while(!pSfs.IsEmpty()) {
		pSfs.GetAt(0)->Clear();
		delete pSfs.GetAt(0);  pSfs.RemoveAt(0);
	}

	while(!pPhs.IsEmpty()) {
		pPhs.GetAt(0)->Clear();
		delete pPhs.GetAt(0);  pPhs.RemoveAt(0);
	}

	while(!UpLpNds.IsEmpty()) {
		UpLpNds.GetAt(0)->Clear();
		delete UpLpNds.GetAt(0);  UpLpNds.RemoveAt(0);
	}

	while(!DnLpNds.IsEmpty()) {
		DnLpNds.GetAt(0)->Clear();
		delete DnLpNds.GetAt(0);  DnLpNds.RemoveAt(0);
	}

}