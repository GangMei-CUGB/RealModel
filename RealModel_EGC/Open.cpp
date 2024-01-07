#include "StdAfx.h"
#include "Open.h"


COpen::COpen(void)
{
}


COpen::~COpen(void)
{
}

void COpen::OpenOnlyPoint(vector<CNode*> &pts)
{
	CFileDialog Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		int i = 0, ID = 0;
		double x, y, z;
		CString filePath = Dlg.GetPathName(); 
		ifstream fin( filePath );
		if(!fin) { AfxMessageBox(_T("Cannot find file ! \n")); return; }
		while(!fin.eof()) {
			fin >> ID >> x >> y >> z;
			pts.push_back(new CNode(i++, x, y, z));
		}
		fin.close();
	}
}