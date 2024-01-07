#pragma once
#include<afxtempl.h>

class CQuad
{
public:
	int ID; //id 
	int RGB[3]; // Color by RGB. 2010.12.5
	int nID[4];
	int UseTime;//使用次数
	bool flag;
	CArray<int, int>iRgnPassQd;//相邻单元号
	double x[4], y[4], z[4];
	CArray<int, int>iInsidNode;
	double avg[3];//4个顶点坐标平均值
	double Para[4];

// Operations
public:
	CQuad(void);
	CQuad(int n0, int n1, int n2, int n3);
    CQuad(int qID, int n0, int n1, int n2, int n3);
    CQuad(int* n, double* X, double* Y, double* Z);
    CQuad(int n0, double x0, double y0, double z0, int n1, double x1, double y1, double z1, 
		  int n2, double x2, double y2, double z2, int n3, double x3, double y3, double z3);

	void Clear(void);
	~CQuad(void);
};

