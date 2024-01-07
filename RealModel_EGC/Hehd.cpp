#include "StdAfx.h"
#include "Hehd.h"

CHehd::CHehd(void)
{
}

CHehd::CHehd(int n0, int n1, int n2, int n3, int n4, int n5, int n6, int n7)
{
	nID[0] = n0; nID[1] = n1; nID[2] = n2;  nID[3] = n3;  
	nID[4] = n4; nID[5] = n5; nID[6] = n6;  nID[7] = n7;
}

CHehd::CHehd(int m, int n0, int n1, int n2, int n3, int n4, int n5, int n6, int n7)
{
	nID[0] = n0; nID[1] = n1; nID[2] = n2;  nID[3] = n3;  
	nID[4] = n4; nID[5] = n5; nID[6] = n6;  nID[7] = n7;  iProperty = m;
}

CHehd::CHehd(int pID, int n0, int n1, int n2, int n3, int n4, int n5, int n6, int n7, int Mat)
{
	ID = pID;  iProperty = Mat;
	nID[0] = n0; nID[1] = n1; nID[2] = n2;  nID[3] = n3;  
	nID[4] = n4; nID[5] = n5; nID[6] = n6;  nID[7] = n7;  iProperty = Mat;
}

CHehd::CHehd(int* n, double* X, double* Y, double* Z)
{
	for(int i = 0; i < 8; i++){nID[i] = n[i]; x[i] = X[i]; y[i] = Y[i]; z[i] = Z[i];}
}
CHehd::CHehd(double* nx, double* ny, double* nz, double* tx, double* ty, double* tz )
{
	int i;
	for(i = 0; i < 4; i++){ x[i] = nx[i]; y[i] = ny[i]; z[i] = nz[i];}
	for(i = 0; i < 4; i++){ x[i+4] = tx[i]; y[i+4] = ty[i]; z[i+4] = tz[i];}
}

CHehd::~CHehd(void)
{
}

