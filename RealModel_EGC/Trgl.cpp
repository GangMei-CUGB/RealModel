#include "StdAfx.h"
#include "Trgl.h"


CTrgl::CTrgl(void)
{
}

CTrgl::CTrgl(int n0, int n1, int n2)
{
	nID[0] = n0; nID[1] = n1; nID[2] = n2;
}

CTrgl::CTrgl(int tID, int n0, int n1, int n2)
{
	ID = tID; nID[0] = n0; nID[1] = n1; nID[2] = n2;
}

CTrgl::CTrgl(int tID, int n0, int n1, int n2, int RID, int UT)
{
	ID = tID; nID[0] = n0; nID[1] = n1; nID[2] = n2;  sID = RID; UseTime = UT;
}

CTrgl::CTrgl(int* n, double* X, double* Y, double* Z)
{
	for(int i = 0; i < 3; i++) {
		nID[i] = n[i]; x[i] = X[i]; y[i] = Y[i]; z[i] = Z[i];
	}
}

CTrgl::CTrgl(int n0, double x0, double y0, double z0, int n1, double x1, double y1, double z1, int n2, double x2, double y2, double z2)
{
	nID[0] = n0;  nID[1] = n1;  nID[2] = n2;
	x[0] = x0;  x[1] = x1;   x[2] = x2;
	y[0] = y0;  y[1] = y1;   y[2] = y2;
	z[0] = z0;  z[1] = z1;   z[2] = z2;
}

CTrgl::~CTrgl(void)
{
}

void CTrgl::Clear(void)
{
	if(!iRgnPassTr.IsEmpty()) {
		iRgnPassTr.RemoveAll();
	}

	if(!iInsidNode.IsEmpty()) {
		iInsidNode.RemoveAll();
	}

	if(!iPassNode.IsEmpty()) {
		iPassNode.RemoveAll();
	}

	if(!iRgnIntrTr.IsEmpty()) {
		iRgnIntrTr.RemoveAll();
	}

	if(!iRgnIntrLps.IsEmpty()) {
		iRgnIntrLps.RemoveAll();
	}

	if(!iRgnIntrLns.IsEmpty()) {
		iRgnIntrLns.RemoveAll();
	}
}