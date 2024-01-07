// PropSet.cpp : implementation file
//

#include "stdafx.h"
#include "RealModel.h"
#include "PropSet.h"
#include "afxdialogex.h"


// CPropSet dialog

IMPLEMENT_DYNAMIC(CPropSet, CDialogEx)

CPropSet::CPropSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPropSet::IDD, pParent)
{

	m_strItems = _T("");
	m_nAge = 0;
	m_nID = 0;
	m_strMaterial = _T("");
	m_strName = _T("");
	m_strStatement = _T("");
}

CPropSet::~CPropSet()
{
}

void CPropSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PROPERTY, m_pProperty);
	DDX_CBString(pDX, IDC_COMBO_ITEMS, m_strItems);
	DDX_Text(pDX, IDC_EDIT_AGE, m_nAge);
	DDX_Text(pDX, IDC_EDIT_ID, m_nID);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_strMaterial);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_STATEMENT, m_strStatement);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_COLOR, m_pMFCColor);
}


BEGIN_MESSAGE_MAP(CPropSet, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PROPERTY, &CPropSet::OnCbnSelchangeComboProperty)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CPropSet::OnBnClickedButtonSelect)
END_MESSAGE_MAP()


// CPropSet message handlers


BOOL CPropSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	int nProp = pView->m_aProps.GetSize();
	if(nProp == 0) {
		AfxMessageBox(_T("Error: No Existed Propterty !"));
	}
	else {
		CString str;
		for(int i = 0; i < nProp; i++) {
			str = pView->m_aProps.GetAt(i)->Name;
			m_pProperty.AddString(str);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropSet::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	int i, ID;
	for(i = 0; i < m_nSelectedIDs.GetSize(); i++) {
		ID = m_nSelectedIDs.GetAt(i); // IDs of Selected Items
		pView->m_aBlks.GetAt(ID)->iProperty = iProperty;
	}

	CDialogEx::OnOK();
}

void CPropSet::OnCbnSelchangeComboProperty()
{
	// TODO: Add your control notification handler code here
	int nSel = m_pProperty.GetCurSel();
	iProperty = nSel; // Index , not ID
	if(nSel > -1) {
		CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
        CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
		CProp* pProp = pView->m_aProps.GetAt(nSel);
		m_nID = pProp->ID;
		m_strName = pProp->Name;
		m_nAge = pProp->Age;
		m_strMaterial = pProp->Material;
		m_pMFCColor.SetColor(COLORREF RGB(pProp->RGB[0], pProp->RGB[1], pProp->RGB[2]));
		m_strStatement = pProp->Statement;
		UpdateData(FALSE);
	}
}

void CPropSet::OnBnClickedButtonSelect()
{
	// TODO: Add your control notification handler code here
	CSelNumPick pSelNum;
	pSelNum.DoModal();
	UpdateData(TRUE);

	int i, id;
	for(i = 0; i < pSelNum.m_aSelectedIDs.GetSize(); i++) {
		id = pSelNum.m_aSelectedIDs.GetAt(i);
		m_nSelectedIDs.Add(id);
	}
	id = m_nSelectedIDs.GetAt(0);
	m_strItems.Format(_T("%d"), id);
	CString str;
	for(i = 1; i < m_nSelectedIDs.GetSize(); i++) {
		id = m_nSelectedIDs.GetAt(i);
		str.Format(_T(",%d"), id);
		m_strItems += str;
	}

	UpdateData(FALSE);

}
