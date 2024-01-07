#pragma once

#include<afxtempl.h>
#include"Node.h"
#include"Trgl.h"
#include"Quad.h"
#include"Loop.h"
#include"Surf.h"
#include"Pohd.h"

class CBlock
{
public:
	int ID;
	int flag;
	int Color; //set color
		
	int iProperty; // ID of the group, property
	int iColorIndex; // color index
	int RGB[3]; // Color by RGB. 2010.12.5
	int iBoolean; // which part of Boolean operation

	CArray<CNode*, CNode*> pNds; // nodes
    CArray<CTrgl*, CTrgl*> pTrs; // triangles
	CArray<CQuad*, CQuad*> pQds; // quadrilaterals
	CArray<CLoop*, CLoop*> pLps; // loops
	CArray<CSurf*, CSurf*> pSfs; // surfaces
	CArray<CPohd*, CPohd*> pPhs; // polyhedrons

	CArray<int, int> iPhds; // IDs of all polyhedrons of a group 

	CArray<int, int>iNbs; // IDs of Neighbor polyhedrons
	CArray<CBlock*, CBlock*> pNbs; // Points of Neighbor polyhedrons
	
	//2009.12.17
	//to save the block/prism formed by intersecting of triangle meshes
	//a date structure spcial for the prism/block
	CArray<CNode*, CNode*> UpLpNds; // 不规则块体上部环上所有节点，可能只有1个
	CArray<CNode*, CNode*> DnLpNds; // 不规则块体下部环上所有节点，可能只有1个
	int FstTri, SecTri;

	CBlock(void);
	void Clear(void);
	~CBlock(void);
};

