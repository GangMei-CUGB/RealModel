#include "StdAfx.h"
#include "Pair.h"


CPair::CPair(void)
{
}

CPair::CPair(int fid, int sid)
{
	FstID = fid;  SedID = sid;
}
CPair::CPair(int id, int fid, int sid)
{
	ID = id;  FstID = fid;  SedID = sid;
}

CPair::~CPair(void)
{
}
