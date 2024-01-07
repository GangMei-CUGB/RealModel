#pragma once
#include<afxtempl.h>
#include"Node.h"
#include"Trgl.h"
#include"Edge.h"
#include"PLine.h"

class CLoop
{
public:
	int ID;
	int flag; //outer(0) or inner(1) or line(2)
	int RGB[3]; // Color by RGB. 2010.12.5
	int UseTime;
	int iDirection; // Direction: Positive 1 Negative -1
	bool bUsed;
	bool bValid;
	CArray<CNode*, CNode*> pNds;
	CArray<CTrgl*, CTrgl*> pTrs;
	CArray<CEdge*, CEdge*> pEgs;
	CArray<CPLine*, CPLine*> pPLns;
	CArray<int, int>iVPLn;
	CArray<int, int>iNds;
	CArray<int, int>iRgnIntrTr;//相交三角形单元号 2013.4.30
	int iProperty; // ID of the group, property
    int iCurParent; // current owned sub-surface

	CTrgl* pParent; // Parent triangle

	CArray<int, int> iCorrLps; // Corresponding loops

	CLoop(void); 
    CLoop(int id, int flag, int n, double* x, double* y, double* z);

	void Clear(void);
	~CLoop(void);
};

