#pragma once
#include <math.h>

#include "Loop.h"
#include "pair.h"
#include "Cordnt.h"

/* Close by MeiGang 2013.02.25
#ifndef EGC
   #define EGC // Switch for Exact Geometric Computation
#endif
//*/


// Close by MeiGang 2013.02.25
/* 
#include "CORE/CORE.h"  // this must appear after the standard headers
//*/

class CEarCut
{
public:
	void Dcinv(double *a, int n);
	void rsift(double* p, int* ID, int i, int n);
	void prhap(double* p, int* ID, int n);

	double AngleToASideInTrgl(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2);
	double AngleOfVecters(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2);
	
	void EquaOfTrgl(double *x, double *y, double *z, double *Para);
	double VolOfTetra(double *x, double *y, double *z);
	void NodeOnNormal(double *x, double *y, double *z, double eps);
	void GetNewCoordt(CTrgl *tr, CCordnt *NewCordt);
	bool IsCCwize(CNode* p, int vcount);
	bool IsSameNode(CNode *nd1, CNode *nd2, double eps);
	bool IsSameNode(double ax, double ay, double az, double bx, double by, double bz, double eps);
	bool IsPointOnSegment(double p0x, double p0y, double p1x, double p1y, double ptx, double pty, double eps);
	int  IsNdInTrgl(double xn, double yn, double x0, double y0, double x1, double y1, double x2, double y2);
	void PolygonPartion(CLoop * lp);
	void PolygonPartionExtended(CLoop * lp);
	void PolygonPartion3D(CLoop * lp);
	void ConvertPolygonHole(CLoop* pOrig, CLoop* pHole);
	int IntrSegment2Segment2D(CNode * p0, CNode * d0, CNode * p1, CNode * d1, CNode * i0, CNode * i1);
	int FindIntersection(double u0, double u1, double v0, double v1, double w[2]);

	void NodeOnNormal(float *x, float *y, float *z, float eps);
	float VolOfTetra(float *x, float *y, float *z);
	void EquaOfTrgl(float *x, float *y, float *z, float *Para);

	CEarCut(void);
	~CEarCut(void);
};

