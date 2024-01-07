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
	CArray<int, int>sPsdTrgl;//�����õ��������
	CArray<int, int>sPsdQuad;//�����õ���ı���
	CArray<int, int>sPsdPhds; // passed polyhedrons
	CArray<int, int>sNeiOnMesh;//�õ������㼯

	CNode(void);
	CNode(int nID);
	CNode(double X, double Y);
	CNode(int nID, double X, double Y);
	CNode(int nID, double X, double Y, double Z); 
	void Clear(void);
	~CNode(void);
};

