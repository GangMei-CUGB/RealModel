#pragma once
#include"Node.h"

class CPLine
{
public:
	int ID;
	int flag, nb;
	CArray<CNode*, CNode*> pNds; 

	CPLine(void);
	void Clear(void);
	~CPLine(void);
};

