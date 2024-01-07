// PropDef.cpp : implementation file
//

#include "stdafx.h"
#include "RealModel.h"
#include "PropDef.h"
#include "afxdialogex.h"


// CPropDef dialog

IMPLEMENT_DYNAMIC(CPropDef, CDialogEx)

CPropDef::CPropDef(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPropDef::IDD, pParent)
{

}

CPropDef::~CPropDef()
{
}

void CPropDef::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_pList);
}


BEGIN_MESSAGE_MAP(CPropDef, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CPropDef::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CPropDef::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CPropDef::OnBnClickedButtonRemove)
END_MESSAGE_MAP()


// CPropDef message handlers


BOOL CPropDef::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	// Set ListCtrl可以整行选择和网格条纹
    DWORD styles = m_pList.GetExtendedStyle();
    m_pList.SetExtendedStyle(styles|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    m_pList.SetBkColor(RGB(0xFF, 0xFF, 0xE0));
    m_pList.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));

	// Insert table head
	m_pList.InsertColumn( 0, _T("ID"), LVCFMT_LEFT, 40 ); 
    m_pList.InsertColumn( 1, _T("Name"), LVCFMT_LEFT, 75 ); 
	m_pList.InsertColumn( 2, _T("Age"), LVCFMT_LEFT, 60 ); 
    m_pList.InsertColumn( 3, _T("Material"), LVCFMT_LEFT, 90 ); 
	m_pList.InsertColumn( 4, _T("Color"), LVCFMT_LEFT, 120 ); 
	m_pList.InsertColumn( 5, _T("Statement"), LVCFMT_LEFT, 300 );  
	
	// Update from global
	CPropNew newProp;

	// Add it to global properties
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();

	int i, j;
	int NumOfProp = pView->m_aProps.GetSize();
	for(i = 0; i < NumOfProp; i++) {
		CProp* pProp = pView->m_aProps.GetAt(i);
	    newProp.m_nID = pProp->ID;
	    newProp.m_strName = pProp->Name;
	    newProp.m_nAge = pProp->Age;
	    newProp.m_strMaterial = pProp->Material;
	    for(j = 0; j < 3; j++) {
		    newProp.RGB[j] = pProp->RGB[j];
		}
		int nLen = pProp->Statement.GetLength();
		newProp.m_strStatement = pProp->Statement.Left(nLen - 5); // - _T(" //END"); Delete end 

		CString str;
	    int nPos = m_pList.GetItemCount();
	    str.Format(_T("%d"), newProp.m_nID);
	    m_pList.InsertItem(nPos, str); // 0
	    m_pList.SetItemText(nPos, 1, newProp.m_strName); // 1
	    str.Format(_T("%d"), newProp.m_nAge);
	    m_pList.SetItemText(nPos, 2, str); // 2
	    m_pList.SetItemText(nPos, 3, newProp.m_strMaterial); // 3
	    str.Format(_T("RGB(%d,%d,%d)"), newProp.RGB[0], newProp.RGB[1], newProp.RGB[2]);
	    m_pList.SetItemText(nPos, 4, str); // 4
	    m_pList.SetItemText(nPos, 5, newProp.m_strStatement); // 5   
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropDef::OnBnClickedButtonNew()
{
	// TODO: Add your control notification handler code here
	CPropNew newProp;
	newProp.m_nID = m_pList.GetItemCount();
	UpdateData(FALSE);
	newProp.DoModal();
	UpdateData(TRUE);
	CString str;
	int nPos = m_pList.GetItemCount();
	str.Format(_T("%d"), newProp.m_nID);
	m_pList.InsertItem(nPos, str); // 0
	m_pList.SetItemText(nPos, 1, newProp.m_strName); // 1
	str.Format(_T("%d"), newProp.m_nAge);
	m_pList.SetItemText(nPos, 2, str); // 2
	m_pList.SetItemText(nPos, 3, newProp.m_strMaterial); // 3
	str.Format(_T("RGB(%d,%d,%d)"), newProp.RGB[0], newProp.RGB[1], newProp.RGB[2]);
	m_pList.SetItemText(nPos, 4, str); // 4
	m_pList.SetItemText(nPos, 5, newProp.m_strStatement); // 5   

	// Add it to global properties
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	CProp* pProp = new CProp;
	pProp->ID = newProp.m_nID;
	pProp->Name = newProp.m_strName;
	pProp->Age = newProp.m_nAge;
	pProp->Material = newProp.m_strMaterial;
	for(int i = 0; i < 3; i++)
		pProp->RGB[i] = newProp.RGB[i];
	
	pProp->Statement = newProp.m_strStatement + _T(" //END"); // Set end 
	pView->m_aProps.Add(pProp);

}

void CPropDef::OnBnClickedButtonEdit()
{
	// TODO: Add your control notification handler code here
	int nMark = m_pList.GetSelectionMark();
	if(nMark == -1) {
		AfxMessageBox(_T("No selected item. Please select one !"));
	}
	else {

		CPropNew newProp;
		// newProp.SetWindowTextW(_T("Edit"));
	    CString str;
		int nPos = m_pList.GetSelectionMark();
		str = m_pList.GetItemText(nPos, 0);
		// newProp.m_nID = str;

		str = m_pList.GetItemText(nPos, 1);
		newProp.m_strName = str;

		str = m_pList.GetItemText(nPos, 2);
		// newProp.m_nAge = str;

		str = m_pList.GetItemText(nPos, 3);
	    newProp.m_strMaterial = str;

		str = m_pList.GetItemText(nPos, 4);
	    // newProp.m_nID = str;

		str = m_pList.GetItemText(nPos, 5);
	    newProp.m_strStatement = str;

		UpdateData(FALSE);

	    newProp.DoModal();
	    UpdateData(TRUE);
		/*
	    CString str;
		int nPos = m_pList.GetSelectionMark();
	    str.Format(_T("%d"), newProp.m_nID);
	    m_pList.InsertItem(nPos, str); // 0
	    m_pList.SetItemText(nPos, 1, newProp.m_strName); // 1
	    str.Format(_T("%d"), newProp.m_nAge);
	    m_pList.SetItemText(nPos, 2, str); // 2
	    m_pList.SetItemText(nPos, 3, newProp.m_strMaterial); // 3
	    str.Format(_T("RGB(%d,%d,%d)"), newProp.RGB[0], newProp.RGB[1], newProp.RGB[2]);
	    m_pList.SetItemText(nPos, 4, str); // 4
	    m_pList.SetItemText(nPos, 5, newProp.m_strStatement); // 5 

		m_pList.DeleteItem(nMark);

		// Add it to global properties
	    CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
        CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	    pView->m_aProps.RemoveAt(nMark);
		*/
		// Initiate 
		m_pList.SetSelectionMark(-1);
	}
}


void CPropDef::OnBnClickedButtonRemove()
{
	// TODO: Add your control notification handler code here
	int nMark = m_pList.GetSelectionMark();
	if(nMark == -1) {
		AfxMessageBox(_T("No selected item. Please select one !"));
	}
	else {
		m_pList.DeleteItem(nMark);

		// Add it to global properties
	    CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
        CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	    pView->m_aProps.RemoveAt(nMark);

		// Initiate 
		m_pList.SetSelectionMark(-1);
		
	}
}


void CPropDef::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}
