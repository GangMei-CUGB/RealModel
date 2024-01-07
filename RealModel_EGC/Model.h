#pragma once

#include "Node.h"
#include "Loop.h"
#include "Pohd.h"
#include "Block.h"

class CModel
{
public:
	// Array
	CArray<CSurf*, CSurf*> m_aSurfs; // surfaces
	CArray<CNode*, CNode*> m_aNds; // all nodes of all groups
	CArray<CLoop*, CLoop*> m_aLps; // all areas of all groups
    CArray<CPohd*, CPohd*> m_aPhds; // all polyhedrons of all groups
	CArray<CBlock*, CBlock*> m_aBlks; // all goups

	void Clear();

	CModel(void);
	~CModel(void);
};

