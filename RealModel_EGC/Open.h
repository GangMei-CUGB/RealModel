#pragma once

#include <fstream>
#include <vector>
#include "Node.h"

using namespace std;

class COpen
{
public:
	void OpenOnlyPoint( vector<CNode*> &pts );

	COpen(void);
	~COpen(void);
};

