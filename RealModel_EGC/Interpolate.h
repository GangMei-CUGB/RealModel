#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "Node.h"

struct Point {
    int ID;
	float x, y, z;
};

using namespace std;

class CInterpolate
{
public:
	// Basic
	int Dcinv(float *a, int n);

	// Kriging
	void OrdinaryKriging(float *dpx, float *dpy, float *dpz, int ndp, float *ipx, float *ipy, float *ipz, int nip);
	void UniversalKriging(float *dpx, float *dpy, float *dpz, int ndp, float *ipx, float *ipy, float *ipz, int nip);

	void UniversalKriging_STL(vector<Point> &dP, vector<Point> &iP);
	void IDW_STL(vector<Point> &dP, vector<Point> &iP, float p);
	void IDW_STL(vector<CNode*> &dP, vector<CNode*> &iP, float p);

	// DSI

	// IDW

	CInterpolate(void);
	~CInterpolate(void);
};

