#pragma once
class CPair
{
public:
	int ID; // ID of a pair;
	int Flag;
	int FstID, SedID; // IDs of a pair of elements;
	bool bCheck; // indetify whether a pair has been checked or not;
	bool bIntr2D, bIntr3D;

	CPair(void);
	CPair(int fid, int sid);
	CPair(int id, int fid, int sid);

	~CPair(void);
};

