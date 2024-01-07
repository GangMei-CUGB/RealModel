#pragma once
#include<afxtempl.h>

/* Close by MeiGang 2013.02.25
// For Exact Geometric Computation
#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif
#include "CORE/CORE.h"  // this must appear after the standard headers
//*/

using namespace std;

class CTrgl
{
public:
	int ID; //id of trgl
	int RGB[3]; // Color by RGB. 2010.12.5
	int sID; //id of surface
	int UseTime;//ԭ�����е�
	bool bUsed; // used or not
	bool bValid;
	bool bNeedCheck; // need to check or not
	int nID[3], lID[3], eID[3];
	double x[3], y[3], z[3];
	CArray<int, int>iRgnPassTr;//���ڵ�Ԫ��
	
	CArray<int, int>iInsidNode;
	CArray<int, int>iPassNode; //iPassNode��ʾ�������ι�ԭʼ�����ţ�
	CArray<int, int>iRgnIntrTr;//�ཻ�����ε�Ԫ�� 2013.4.30
    CArray<int, int>iRgnIntrLps; // �������ཻ���γɵ��������� 2D
	CArray<int, int>iRgnIntrLns; // �������ཻ���γɵ������߶� 3D  2009.12.6

	CTrgl(void);
	CTrgl(int n0, int n1, int n2);
    CTrgl(int tID, int n0, int n1, int n2);
    CTrgl(int tID, int n0, int n1, int n2, int RID, int UT);
	CTrgl(int* n, double* x, double* y, double* z);
	CTrgl(int n0, double x0, double y0, double z0, int n1, double x1, double y1, double z1, int n2, double x2, double y2, double z2);

	void Clear(void);
	~CTrgl(void);
};

