#pragma once
#include<afxtempl.h>
#include"Node.h"
#include"Loop.h"

class CPohd
{
public:
	int ID; // id 
	int flag; // indefication, -1: need to remove. 0 : have not indentify. 1 : valid, nedd to keep
	int Color; // real color
	int iProperty; // ID of the group, property
	int iColorIndex; // color index
	int RGB[3]; // Color by RGB. 2010.12.5
	int iTopSurf; // ID of top surface;
	int iButSurf; // ID of buttom surface;
	
	CArray<int, int>iNds; // IDs of nodes of polyhedron
	CArray<int, int>iLps; // IDs of loops of polyhedron
	CArray<CNode*, CNode*> pNds; // nodes of polyhedron
	CArray<CLoop*, CLoop*> pLps; // loops / areas of polyhedron

	CArray<int, int>iNbs; // IDs of Neighbor polyhedrons
	CArray<CPohd*, CPohd*> pNbs; // Points of Neighbor polyhedron

	CArray<int, int>iRgnIntrTr;//相交三角形单元号 2013.4.30
	CPohd(void);
	void Clear(void);
	~CPohd(void);
};

