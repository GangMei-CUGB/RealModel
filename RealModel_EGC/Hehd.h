#pragma once
#include<afxtempl.h>
#include"Node.h"
#include"Loop.h"

class CHehd
{
public:
	int ID; // id 
	int flag; // indefication, -1: need to remove. 0 : have not indentify. 1 : valid, nedd to keep
	int Color; // real color
	int iProperty; // ID of the group, property
	int iColorIndex; // color index
	int RGB[3]; // Color by RGB. 2010.12.5
	int iTopSurf; // ID of top surface;
	int iButSurf; // ID of buttom surface;
	
	CArray<int, int>iNds; // IDs of nodes of polyhedron
	CArray<int, int>iLps; // IDs of loops of polyhedron
	CArray<CNode*, CNode*> pNds; // nodes of polyhedron
	CArray<CLoop*, CLoop*> pLps; // loops / areas of polyhedron

    int nID[8];
	double x[8], y[8], z[8];
	CArray<int, int>iBlkPassHx;
	CArray<int, int>iIntrTriRgn0;
	CArray<int, int>iIntrTriRgn1;

	CHehd(void);
	CHehd(int n0, int n1, int n2, int n3, int n4, int n5, int n6, int n7);
	CHehd(int m, int n0, int n1, int n2, int n3, int n4, int n5, int n6, int n7);
    CHehd(int pID, int n0, int n1, int n2, int n3, int n4, int n5, int n6, int n7, int Mat);
    CHehd(int* n, double* X, double* Y, double* Z);
	CHehd(double* nx, double* ny, double* nz, double* tx, double* ty, double* tz );

	~CHehd(void);
};

