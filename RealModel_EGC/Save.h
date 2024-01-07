#pragma once

#include <fstream>
#include <vector>
#include "Node.h"

using namespace std;

class CSave
{
public:
	void SaveOnlyPoint(vector<CNode*> &pts);

	CSave(void);
	~CSave(void);
};

