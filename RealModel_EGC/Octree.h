// Data Structure
// Octree 2012.3.30
#pragma once
#include "OctreeNode.h"
#include "Node.h"
#include "Trgl.h"

class COctree
{
public:
	CArray<COctreeNode*, COctreeNode*> pAllTree; // all trees 

	CArray<CNode*, CNode*> m_aNode;
	CArray<CTrgl*, CTrgl*> m_aTrgl;

	void Add(CNode* pNode);
	void Add(CTrgl* pTrgl);

	void BuildOctree_Node();
	void BuildOctree_Trgl();

	int stopDepth;
	void BuildChild_Node(COctreeNode* pOctree);
	void BuildChild_Trgl(COctreeNode* pOctree);
	bool IsInside_Node(CNode* pt, COctreeNode* pNode);
	bool IsInside_Trgl(CNode* pt, COctreeNode* pNode);

	COctree(void);
	~COctree(void);
};

