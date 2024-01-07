#include "StdAfx.h"
#include "PLine.h"


CPLine::CPLine(void)
{
}


CPLine::~CPLine(void)
{
}

void CPLine::Clear(void)
{
	while(!pNds.IsEmpty()) {
		pNds.GetAt(0)->Clear();
		delete pNds.GetAt(0);  pNds.RemoveAt(0);
	}
}