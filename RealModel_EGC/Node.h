#pragma once
#include<afxtempl.h>

// Close by MeiGang 2013.02.25
/* 
#include "CORE/CORE.h"  // this must appear after the standard headers
//*/

class CNode
{
public:
	int ID;
	int CF;
	int flag;
	int UseTime;
	double x, y, z;
	int iProperty; // ID of the group, property

	bool bvalid;
	CArray<int, int>sPsdTrgl;//经过该点的三角形
	CArray<int, int>sPsdQuad;//经过该点的四边形
	CArray<int, int>sPsdPhds; // passed polyhedrons
	CArray<int, int>sNeiOnMesh;//该点的领域点集

	CNode(void);
	CNode(int nID);
	CNode(double X, double Y);
	CNode(int nID, double X, double Y);
	CNode(int nID, double X, double Y, double Z); 
	void Clear(void);
	~CNode(void);
};

