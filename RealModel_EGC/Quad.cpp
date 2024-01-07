#include "StdAfx.h"
#include "Quad.h"


CQuad::CQuad(void)
{
}

CQuad::CQuad(int n0, int n1, int n2, int n3)
{
	nID[0] = n0; nID[1] = n1; nID[2] = n2;  nID[3] = n3;flag = false;
}

CQuad::CQuad(int qID, int n0, int n1, int n2, int n3)
{
	ID = qID; nID[0] = n0; nID[1] = n1; nID[2] = n2;  nID[3] = n3;  flag = false;
}

CQuad::CQuad(int* n, double* X, double* Y, double* Z)
{
	for(int i = 0; i < 4; i++){nID[i] = n[i]; x[i] = X[i]; y[i] = Y[i]; z[i] = Z[i];}
}

CQuad::CQuad(int n0, double x0, double y0, double z0, int n1, double x1, double y1, double z1, 
			   int n2, double x2, double y2, double z2, int n3, double x3, double y3, double z3)
{
	nID[0] = n0;  nID[1] = n1;  nID[2] = n2;  nID[3] = n3;
	x[0] = x0;  x[1] = x1;   x[2] = x2;   x[3] = x3;
	y[0] = y0;  y[1] = y1;   y[2] = y2;   y[3] = y3;
	z[0] = z0;  z[1] = z1;   z[2] = z2;   z[3] = z3;
}

CQuad::~CQuad(void)
{
}

void CQuad::Clear(void)
{
	if(!iRgnPassQd.IsEmpty()) {
		iRgnPassQd.RemoveAll();
	}

	if(!iInsidNode.IsEmpty()) {
		iInsidNode.RemoveAll();
	}
}
