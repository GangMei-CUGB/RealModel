#pragma once

// Close by MeiGang 2013.02.25
/*
#include "CORE/CORE.h"  // this must appear after the standard headers
//*/

class CMollerEGC
{
public:
	//triangle mesh intersection in 3D
	int tri_tri_intersect_with_isectline(double V0[3],double V1[3],double V2[3],double U0[3],double U1[3],double U2[3],int *coplanar,double isectpt1[3],double isectpt2[3]);
	int compute_intervals_isectline(double VERT0[3],double VERT1[3],double VERT2[3],double VV0,double VV1,double VV2,double D0,double D1,double D2,
		double D0D1,double D0D2,double *isect0,double *isect1,double isectpoint0[3],double isectpoint1[3]);
	int coplanar_tri_tri(double N[3],double V0[3],double V1[3],double V2[3],double U0[3],double U1[3],double U2[3]);
    int tri_tri_intersect(double V0[3],double V1[3],double V2[3],double U0[3],double U1[3],double U2[3]);
	int NoDivTriTriIsect(double V0[3],double V1[3],double V2[3],double U0[3],double U1[3],double U2[3]);
	void isect2(double VTX0[3],double VTX1[3],double VTX2[3],double VV0,double VV1,double VV2,double D0,double D1,double D2,double *isect0,double *isect1,double isectpoint0[3],double isectpoint1[3]);
    
	CMollerEGC(void);
	~CMollerEGC(void);
};

