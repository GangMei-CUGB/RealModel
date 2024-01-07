#include "StdAfx.h"
#include "Convert.h"
#include <fstream>

CConvert::CConvert(void)
{
}


CConvert::~CConvert(void)
{
}

void CConvert::ConvertTrglSurfToANSYS(CSurf * surf)
{
	if(AfxMessageBox(_T("Save to Ansys ?"), MB_YESNOCANCEL) != 6)  return;
	CFileDialog Dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) 
	{
		int i, j, nid[3];
		CString fn = Dlg.GetPathName(); 
		CString fname = fn + "_ForAnsys"; 
		if(fname.Right(4)!= ".txt")   fname += ".txt";
		ofstream fout(fname);
		fout << "/prep7" << endl;
		for(i = 0; i < surf->pNds.GetSize(); i++){
			fout  << "K, " << i+1 <<", " 
				  << surf->pNds.GetAt(i)->x <<", " 
				  << surf->pNds.GetAt(i)->y <<", " 
				  << surf->pNds.GetAt(i)->z <<endl;
		}
		for(i = 0; i < surf->pTrs.GetSize(); i++){
            for(j = 0; j < 3; j++)    
				nid[j] = surf->pTrs.GetAt(i)->nID[j] + 1; // add 1
			fout << "A, " << nid[0] << ", " << nid[1] << ", " << nid[2]  << endl;
		}
		fout.close();
	}

	AfxMessageBox(_T("Everything is Done ! "));
	surf->Clear();

}

void CConvert::ConvertQuadSurfToANSYS(CSurf * surf)
{
	if(AfxMessageBox(_T("Save to Ansys ?"), MB_YESNOCANCEL) != 6)  return;
	CFileDialog Dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) 
	{
		int i, j, nid[4];
		CString fn = Dlg.GetPathName(); 
		CString fname = fn + "_ForAnsys"; 
		if(fname.Right(4)!= ".txt")   fname += ".txt";
		ofstream fout(fname);
		fout << "/prep7" << endl;
		for(i = 0; i < surf->pNds.GetSize(); i++){
			fout  << "K, " << i+1 <<", " 
				  << surf->pNds.GetAt(i)->x <<", " 
				  << surf->pNds.GetAt(i)->y <<", " 
				  << surf->pNds.GetAt(i)->z <<endl;
		}
		for(i = 0; i < surf->pQds.GetSize(); i++){
            for(j = 0; j < 4; j++)    
				nid[j] = surf->pQds.GetAt(i)->nID[j] + 1; // add 1
			fout << "A, " << nid[0] << ", " << nid[1] << ", " << nid[2]  << ", " << nid[3] << endl;
		}
		fout.close();
	}

	AfxMessageBox(_T("Everything is Done ! "));
	surf->Clear();

}

void CConvert::ConvertPolySurfToANSYS(CSurf * surf)
{
	if(AfxMessageBox(_T("Save to Ansys ?"), MB_YESNOCANCEL) != 6)  return;
	CFileDialog Dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) 
	{
		int i, j, id;
		CString fn = Dlg.GetPathName(); 
		CString fname = fn + "_ForAnsys"; 
		if(fname.Right(4)!= ".txt")   fname += ".txt";
		ofstream fout(fname);
		fout << "/prep7" << endl;
		for(i = 0; i < surf->pNds.GetSize(); i++){
			fout  << "K, " << i+1 <<", " 
				  << surf->pNds.GetAt(i)->x <<", " 
				  << surf->pNds.GetAt(i)->y <<", " 
				  << surf->pNds.GetAt(i)->z <<endl;
		}
		for(i = 0; i < surf->pLps.GetSize(); i++){
			fout << "A, ";
			for(j = 0; j < surf->pLps.GetAt(i)->iNds.GetSize(); j++) {   
				id = surf->pLps.GetAt(i)->iNds.GetAt(j) + 1; // add 1
				fout << id << ", " ;
			}
			fout << endl;
		}
		fout.close();
	}

	AfxMessageBox(_T("Everything is Done ! "));
	surf->Clear();
	
} 

void CConvert::ConvertBlkGroupToANSYS(CModel * model)
{
	// To Ansys: Node, Area, Block
	CString fn, fname;
	int i, j, t0, t1, t2, t3, t4;
	t0 = 1;  t1 = 1; t2 = 1; t3 = 1; t4 = 0;
	if(AfxMessageBox(_T("Save to Ansys ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) 
	{
		fn = Dlg.GetPathName(); 
		CString str = _T("_ForAnsys");
		fname = fn + str;
		if(fname.Right(4)!=".txt")   fname += ".txt";
		ofstream fout(fname);
		fout << "/prep7" << endl;
		for(i = 0; i < model->m_aNds.GetSize(); i++){ //Nodes
			fout << "K," << i+1 <<", " 
				  << model->m_aNds.GetAt(i)->x << ", " 
				  << model->m_aNds.GetAt(i)->y << ", " 
				  << model->m_aNds.GetAt(i)->z << endl;
		}

		for(i = 0; i < model->m_aLps.GetSize(); i++){//Areas
			fout << "A, ";
			for(j = 0; j < model->m_aLps.GetAt(i)->iNds.GetSize() - 1; j++)
				fout << model->m_aLps.GetAt(i)->iNds.GetAt(j) + 1<< ", ";
			fout << model->m_aLps.GetAt(i)->iNds.GetAt(j) + 1;
			fout << endl;
		}
		
		//*
		for(i = 0; i < model->m_aPhds.GetSize(); i++){//volumes
			fout << "ASEL," << "S," << " ," << " ," << model->m_aPhds.GetAt(i)->iLps.GetAt(0) + 1 << endl;
			for(j = 1; j < model->m_aPhds.GetAt(i)->iLps.GetSize(); j++)
				fout << "ASEl," << "A," << " ," << " ," << model->m_aPhds.GetAt(i)->iLps.GetAt(j) + 1 << endl;
			fout << "VA," << "ALL"<<endl;
		}
		//*/
		fout.close();
	}

}