#include "StdAfx.h"
#include "Surf.h"


CSurf::CSurf(void)
{
}


CSurf::~CSurf(void)
{
}

void CSurf::Clear(void)
{
	if(!iBlkPassRgn.IsEmpty()) {
		iBlkPassRgn.RemoveAll();
	}

	if(!iVTrgls.IsEmpty()) {
		iVTrgls.RemoveAll();
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

	while(!m_alps.IsEmpty()) {
		m_alps.GetAt(0)->Clear();
		delete m_alps.GetAt(0);  m_alps.RemoveAt(0);
	}

	while(!pLns.IsEmpty()) {
		pLns.GetAt(0)->Clear();
		delete pLns.GetAt(0);  pLns.RemoveAt(0);
	}

	while(!pBds.IsEmpty()) {
		pBds.GetAt(0)->Clear();
		delete pBds.GetAt(0);  pBds.RemoveAt(0);
	}

}
