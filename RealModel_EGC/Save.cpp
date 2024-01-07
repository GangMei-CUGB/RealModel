#include "StdAfx.h"
#include "Save.h"

CSave::CSave(void)
{
	
}


CSave::~CSave(void)
{
}

void CSave::SaveOnlyPoint(vector<CNode*> &pts)
{
	CFileDialog Dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		int i = 0, ID = 0;
		double x, y, z;
		CString filePath = Dlg.GetPathName(); 
		if(filePath.Right(4)!= _T(".txt") )   filePath += _T(".txt");
		ofstream fin(filePath);

		for(i = 0; i < pts.size() - 1; i++) {
			ID = pts.at(i)->ID;
			x = pts.at(i)->x;  y = pts.at(i)->y;  z = pts.at(i)->z;
			fin << ID <<"   " <<  x << "   " << y << "   " << z << endl;
		}
		ID = pts.at(i)->ID;
		x = pts.at(i)->x;  y = pts.at(i)->y;  z = pts.at(i)->z;
		fin << ID <<"   " <<  x << "   " << y << "   " << z ;
		fin.close(); 
	}
}