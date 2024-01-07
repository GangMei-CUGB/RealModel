// Data Structure
// Octree Node 2012.3.30
#pragma once
#include "Node.h"

class COctreeNode
{
public:
	// Octree node data structure
    CNode* center; // Center point of octree node (not strictly needed)
    float halfWidth; // Half the width of the node volume (not strictly needed)
	double halfWidth_x, halfWidth_y, halfWidth_z;
	COctreeNode *pChild[8]; // Pointers to the eight children nodes
    CArray<int, int> pObjList; // Linked list of objects contained at this node

	int Depth;
	bool bNeedDivide;

	COctreeNode(void);
	COctreeNode(CNode* pCenter, double half_x, double half_y, double half_z);
	~COctreeNode(void);
};

