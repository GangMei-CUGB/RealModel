#include "StdAfx.h"
#include "Node.h"


CNode::CNode(void)
{
}

CNode::CNode(double X, double Y)
{
	x = X; y = Y; z = 0.00;
}
CNode::CNode(int nID, double X, double Y)
{
	ID = nID; x = X; y = Y;  z = 0.00;
}
CNode::CNode(int nID)
{
	ID = nID; 
}

CNode::CNode(int nID, double X, double Y, double Z)
{
	ID = nID; x = X; y = Y; z = Z; flag=1;
}

CNode::~CNode(void)
{
}

void CNode::Clear(void)
{
	if(!sPsdTrgl.IsEmpty()) {
		sPsdTrgl.RemoveAll();
	}

	if(!sPsdQuad.IsEmpty()) {
		sPsdQuad.RemoveAll();
	}

	if(!sPsdPhds.IsEmpty()) {
		sPsdPhds.RemoveAll();
	}

}