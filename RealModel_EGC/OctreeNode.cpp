#include "StdAfx.h"
#include "OctreeNode.h"


COctreeNode::COctreeNode(void)
{
}

COctreeNode::COctreeNode(CNode* pCenter, double half_x, double half_y, double half_z)
{
	center = pCenter;
	halfWidth_x = half_x;
	halfWidth_y = half_y;
	halfWidth_z = half_z;
}

COctreeNode::~COctreeNode(void)
{
}
