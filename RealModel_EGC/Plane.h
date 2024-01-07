#pragma once
#include<afxtempl.h>
#include"Node.h"

class CPlane
{
public:
	int ID;
	double Para[4]; // Parameters of plane
	
	// 平面表现为一个矩形，4个顶点坐标保存如下：
	double x[4], y[4], z[4];

	//Create a boundary box based on the rectangle( as a plane)
	CNode *pBoxLB; // left botton cornor of boundary box
	CNode *pBoxRT; // right top cornor of boundary box

	double minx, miny, minz; // left botton cornor of boundary box
	double maxx, maxy, maxz; // right top cornor of boundary box

	CPlane(void);
	~CPlane(void);
};

