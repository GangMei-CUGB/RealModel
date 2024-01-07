#pragma once
#include<afxtempl.h>
#include"Node.h"

class CEdge
{
public:
	int ID;
	int flag;
	int UseTime;
	bool bUsed;
	bool bValid;
	CNode * fstNode;
	CNode * lstNode;
	CArray<int, int> sPsdTrgl;

	int Start, End;

	//save the intersection segment of 2 triangles
	int FstTri, SecTri;
	double FstNd[3], SecNd[3];

	CEdge(void);
	CEdge(int head, int tail);
	void Clear(void);
	~CEdge(void);
};

