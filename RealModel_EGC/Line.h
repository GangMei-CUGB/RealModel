#pragma once
#include<afxtempl.h>

class CLine
{
public:
	int ID;
	int nID[2];
	int UseTime;
	bool bUsed;
	int iProperty; // ID of the group, property
	CArray<int, int> sPsdTrgl;

	//save the intersection segment of 2 triangles
	int FstTri, SecTri;
	double FstNd[3], SecNd[3];
	
	CLine(void);
	CLine(int id, int* nid, int ut); 
	CLine(int nid1, int nid2);
	void Clear(void);
	~CLine(void);
};

