#include "StdAfx.h"
#include "Edge.h"


CEdge::CEdge(void)
{
}

CEdge::CEdge(int head, int tail)
{
	Start = head;  End = tail;
}

CEdge::~CEdge(void)
{
}

void CEdge::Clear(void)
{
	if(!sPsdTrgl.IsEmpty()) {
		sPsdTrgl.RemoveAll();
	}

}
