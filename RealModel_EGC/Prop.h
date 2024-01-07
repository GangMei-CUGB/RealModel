#pragma once
class CProp
{
public:
	int ID;
	CString Name;
	int Age;
	CString Material;
	CString Statement;
	int RGB[3]; // Color by RGB. 2010.12.5

	CProp(void);
	~CProp(void);
};

