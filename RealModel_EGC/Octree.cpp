#include "StdAfx.h"
#include "Octree.h"


COctree::COctree(void)
{
}


COctree::~COctree(void)
{
}

void COctree::Add(CNode* pNode)
{
	m_aNode.Add(pNode);
}

void COctree::BuildOctree_Node()
{
	CNode* pMin = new CNode(0, 0.0, 0.0, 0.0);
	CNode* pMax = new CNode(0, 0.0, 0.0, 0.0);

	int i, j, k;

	for(i = 0; i < m_aNode.GetSize(); i++) {
		CNode* pt = m_aNode.GetAt(i);
		if(pt->x < pMin->x)  pMin->x = pt->x;
		if(pt->y < pMin->y)  pMin->y = pt->y;
		if(pt->z < pMin->z)  pMin->z = pt->z;

		if(pt->x > pMax->x)  pMax->x = pt->x;
		if(pt->y > pMax->y)  pMax->y = pt->y;
		if(pt->z > pMax->z)  pMax->z = pt->z;
	}

	double cx, cy, cz, hx, hy, hz;
	cx = (pMax->x + pMin->x) * 0.5f;
	cy = (pMax->y + pMin->y) * 0.5f;
	cz = (pMax->z + pMin->z) * 0.5f;

	hx = (pMax->x - pMin->x) * 0.5f;
	hy = (pMax->y - pMin->y) * 0.5f;
	hz = (pMax->z - pMin->z) * 0.5f;

	CNode* pCenter = new CNode(0, cx, cy, cz);
	COctreeNode* pNode = new COctreeNode(pCenter, hx, hy, hz);
	pAllTree.Add(pNode);
	
	// Create subtree
	int start, end, num;
	start = 0;  end = pAllTree.GetSize();
	while (start != end) { // Stop when no new node
		for(i = start; i < end; i++) {
			BuildChild_Node(pAllTree.GetAt(i));
		}

		// update
		start = end;
		end = pAllTree.GetSize();
	}

}

void COctree::BuildChild_Node(COctreeNode* pOctree)
{
	if(pOctree->bNeedDivide == true) {
		// Create 8 child
		double half_x, half_y, half_z;
		double x, y, z;
		int i, j, id;

		half_x = pOctree->halfWidth_x * 0.5;
		half_y = pOctree->halfWidth_y * 0.5;
		half_z = pOctree->halfWidth_z * 0.5;

		// construct the eight children of the subtree
		CNode offset;
		for(i = 0; i < 8; i++) {
			offset.x = ((i & 1) ? half_x : -half_x);
			offset.y = ((i & 2) ? half_y : -half_y);
			offset.z = ((i & 4) ? half_z : -half_z);

			CNode* center = pOctree->center;
			x = center->x + offset.x;
			y = center->y + offset.y;
			z = center->z + offset.z;

			CNode* pCenter = new CNode(i, x, y, z);
			COctreeNode* pNode = new COctreeNode(center, half_x, half_y, half_z);

			// Set
			pNode->Depth = pOctree->Depth + 1;

			// Check
			if(pNode->Depth < stopDepth) {
				pNode->bNeedDivide = true;
			}
			else pNode->bNeedDivide = false;

			// Save
			pOctree->pChild[i] = pNode;
			pAllTree.Add(pNode);

		}

		// Test
		for(i = 0; i < 8; i++) {	
			COctreeNode* pNode = pOctree->pChild[i];
			for(j = 0; j < pOctree->pObjList.GetSize(); j++) {
				id = pOctree->pObjList.GetAt(j);
				if(IsInside_Node(m_aNode.GetAt(id), pNode) == true) {
					pNode->pObjList.Add(id);
					break;
				}
			}

			// Re-check
			if(pNode->pObjList.GetSize() < 2) {
				pNode->bNeedDivide = false;
			}
		}

	}

}

bool COctree::IsInside_Node(CNode* pt, COctreeNode* pNode)
{
	if(pt->x < pNode->center->x - pNode->halfWidth_x)  return false;
	if(pt->y < pNode->center->y - pNode->halfWidth_y)  return false;
	if(pt->z < pNode->center->z - pNode->halfWidth_z)  return false;

	if(pt->x > pNode->center->x + pNode->halfWidth_x)  return false;
	if(pt->x > pNode->center->x + pNode->halfWidth_x)  return false;
	if(pt->x > pNode->center->x + pNode->halfWidth_x)  return false;

	return true;

}