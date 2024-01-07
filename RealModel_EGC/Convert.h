#pragma once
#include "Surf.h"
#include "Model.h"

class CConvert
{
public:
	void ConvertTrglSurfToANSYS(CSurf * surf);
	void ConvertQuadSurfToANSYS(CSurf * surf);
	void ConvertPolySurfToANSYS(CSurf * surf);
	void ConvertBlkGroupToANSYS(CModel * model);

	CConvert(void);
	~CConvert(void);
};

