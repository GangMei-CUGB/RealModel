#pragma once
#include<afxtempl.h>

#include"Node.h"
#include"Trgl.h"
#include"Quad.h"
#include"Loop.h"

class CSurf
{
public:
	int ID;
	int flag;
	int iVec; //normal of region
	int PosOrNeg;//法线方向是指向正还是负的。正：1；负：-1
	int iProperty;
	int iParent; // parent mesh model
	int iOutIn; // outer or inner

	int Color; //set color
	int RGB[3]; // Color by RGB. 2010.12.5
	int iColorIndex; // color index
	bool bUsed; // used or not
	CString fName; //surface file name
	CArray<int, int>iBlkPassRgn;
	CArray<CNode*, CNode*> pNds; // nodes
	CArray<CTrgl*, CTrgl*> pTrs; // triangles
	CArray<CQuad*, CQuad*> pQds; // quadrilaterals
	CArray<CLoop*, CLoop*> pLps; // loops 
	CArray<CLoop*, CLoop*> m_alps; 
	CArray<CPLine*, CPLine*> pLns;
	CArray<CLoop*, CLoop*> pBds; // boundary for tri mesh
	CArray<int, int>iVTrgls;  //dirctiong of trgl in blk
    
	CSurf(void);
	void Clear(void);
	~CSurf(void);
};

