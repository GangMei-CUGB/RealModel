// SelOption.cpp : implementation file
//

#include "stdafx.h"
#include "RealModel.h"
#include "SelOption.h"
#include "afxdialogex.h"


// CSelOption dialog

IMPLEMENT_DYNAMIC(CSelOption, CDialogEx)

CSelOption::CSelOption(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelOption::IDD, pParent)
{

	m_nRange = 0;
	m_strProp = _T("");
}

CSelOption::~CSelOption()
{
}

void CSelOption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OBJECT, m_pSelObj);
	DDX_Control(pDX, IDC_COMBO_SEL_BY, m_pSelBy);
	DDX_Radio(pDX, IDC_RADIO_FROM_FULL, m_nRange);
	DDX_Control(pDX, IDC_COMBO_SEL_BY_PROP, m_pSelProp);
	//  DDX_Control(pDX, IDC_EDIT_PROP, m_strProp);
	DDX_Text(pDX, IDC_EDIT_PROP, m_strProp);
}


BEGIN_MESSAGE_MAP(CSelOption, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJECT, &CSelOption::OnSelChangeComboObject)
	ON_CBN_SELCHANGE(IDC_COMBO_SEL_BY, &CSelOption::OnSelChangeComboSelBy)
	ON_BN_CLICKED(IDC_BUTTON_REPLOT, &CSelOption::OnClickedButtonReplot)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSelOption::OnClickedButtonApply)
END_MESSAGE_MAP()


// CSelOption message handlers


BOOL CSelOption::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_pSelObj.AddString(_T("Points"));
	m_pSelObj.AddString(_T("Lines"));
	m_pSelObj.AddString(_T("Areas"));
	m_pSelObj.AddString(_T("Surfaces"));
	m_pSelObj.AddString(_T("Polyheds"));
	m_pSelObj.AddString(_T("Blocks"));
	m_pSelObj.SetCurSel(0);
	m_pSelObj.SetItemHeight(1, 18);

	m_pSelBy.AddString(_T("By Num/Pick"));
	m_pSelBy.AddString(_T("By Property"));
	m_pSelBy.AddString(_T("Adjacent to"));
	m_pSelBy.SetCurSel(0);
	m_pSelBy.SetItemHeight(1, 18);

	m_pSelProp.AddString(_T("By ID"));
	m_pSelProp.AddString(_T("By Name"));
	m_pSelProp.AddString(_T("By Age"));
	m_pSelProp.AddString(_T("By Material"));
	m_pSelProp.SetCurSel(0);
	m_pSelProp.SetItemHeight(1, 18);

	// Set Unable
	CComboBox* pComboBox;
	pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_SEL_BY_PROP);
	pComboBox->EnableWindow(FALSE);
	CEdit* pEdit;
	pEdit = (CEdit*) GetDlgItem(IDC_EDIT_PROP);
	pEdit->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSelOption::OnSelChangeComboObject()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	pView->m_nSelectObj = m_pSelObj.GetCurSel();
}

void CSelOption::OnSelChangeComboSelBy()
{
	// TODO: Add your control notification handler code here
	int nPos = m_pSelBy.GetCurSel();
	switch(nPos) {
	case -1: // Not Select
		{
		// Set Unable
	    CComboBox* pComboBox;
	    pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_SEL_BY_PROP);
	    pComboBox->EnableWindow(FALSE);
	    CEdit* pEdit;
	    pEdit = (CEdit*) GetDlgItem(IDC_EDIT_PROP);
	    pEdit->EnableWindow(FALSE);

		AfxMessageBox(_T("No Selection !"), IDOK);
		break;
		}
	case 0: // Select by Num/Pick
		{
		// Set Unable
	    CComboBox* pComboBox;
	    pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_SEL_BY_PROP);
	    pComboBox->EnableWindow(FALSE);
	    CEdit* pEdit;
	    pEdit = (CEdit*) GetDlgItem(IDC_EDIT_PROP);
	    pEdit->EnableWindow(FALSE);

		CSelNumPick pSelNum;
		pSelNum.DoModal();
		break;
		}
	case 1: // Select by Property
		{
			CComboBox* pComboBox;
			pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_SEL_BY_PROP);
			pComboBox->EnableWindow(TRUE);
			CEdit* pEdit;
			pEdit = (CEdit*) GetDlgItem(IDC_EDIT_PROP);
			pEdit->EnableWindow(TRUE);

		    break;
		}
	case 2: // Select by Adjacent to
		{
		// Set Unable
	    CComboBox* pComboBox;
	    pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_SEL_BY_PROP);
	    pComboBox->EnableWindow(FALSE);
	    CEdit* pEdit;
	    pEdit = (CEdit*) GetDlgItem(IDC_EDIT_PROP);
	    pEdit->EnableWindow(FALSE);

		CSelNumPick pSelNum;
		pSelNum.DoModal();
		break;
		}
	default:
		AfxMessageBox(_T("Nothing will be done !"), IDOK);
	}

}

void CSelOption::OnClickedButtonReplot()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	int nSelectObj = m_pSelObj.GetCurSel();
	pView->Replot(nSelectObj);
}

void CSelOption::OnClickedButtonApply()
{
	// TODO: Add your control notification handler code here
	GetLocalSelectedIDs();
	UpdateGlobalSelectedIDs();
	Reset();
}

void CSelOption::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	GetLocalSelectedIDs();
	UpdateGlobalSelectedIDs();

	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	int nSelectObj = m_pSelObj.GetCurSel();
	pView->Replot(nSelectObj);

	CDialogEx::OnOK();
}

// Get local selected IDs from the dialog Num/Pick
void CSelOption::GetLocalSelectedIDs()
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	int nSelectBy = m_pSelBy.GetCurSel();

	switch(nSelectBy) {
	case SEL_BY_NUM_PICK:
        GetLocalSelectedIDsByNumPick();
		break;
	case SEL_BY_PROPERTY:
		GetLocalSelectedIDsByProp();
		break;
	case SEL_BY_ADJACENT:
		GetLocalSelectedIDsByAdja();
		break;
	default:
		AfxMessageBox(_T("Error: No Selection !"));
	}

}

// Get local selected IDs by Num/Pick
void CSelOption::GetLocalSelectedIDsByNumPick()
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	for(int i = 0; i < pView->m_nSelectedIDs.GetSize(); i++) {
		m_aSelectedIDs.Add(pView->m_nSelectedIDs.GetAt(i));
	}

}

// Get local selected IDs by Property
void CSelOption::GetLocalSelectedIDsByProp()
{
	// #define PROP_BY_ID           0  // By ID
    // #define PROP_BY_NAME         1  // By Name
    // #define PROP_BY_AGE          2  // By Age
    // #define PROP_BY_MATERIAL     3  // By Material
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	int i, id, nSelectProp;

	// Selected Property
	UpdateData(TRUE);
	CProp* pSelProp;
	CString input;
	int* number = new int[1000], n;
	nSelectProp = m_pSelProp.GetCurSel();

	switch (nSelectProp) {
	case PROP_BY_ID:
	    input = m_strProp;
	    n = pView->CStringToInt(input, number);
		if(n != 1) {
			AfxMessageBox(_T("Please select only 1 property !"));  
			delete number;  break;
		}
		else {
			for(i = 0; i < pView->m_aProps.GetSize(); i++) {
				if(pView->m_aProps.GetAt(i)->ID == number[0]) {
					pSelProp = pView->m_aProps.GetAt(i);
					break;
				}
			}
		}
		break;
	case PROP_BY_NAME:
		input = m_strProp;
		if(m_strProp == _T("")) {
			AfxMessageBox(_T("Please select only 1 property !"));  
			break;
		}
		else {
			for(i = 0; i < pView->m_aProps.GetSize(); i++) {
				if(pView->m_aProps.GetAt(i)->Name.Compare(input) == 0) {
					pSelProp = pView->m_aProps.GetAt(i);
					break;
				}
			}
		}
		break;
	case PROP_BY_AGE:
	    input = m_strProp;
	    n = pView->CStringToInt(input, number);
		if(n != 1) {
			AfxMessageBox(_T("Please select only 1 property !"));  
			delete number;  break;
		}
		else {
			for(i = 0; i < pView->m_aProps.GetSize(); i++) {
				if(pView->m_aProps.GetAt(i)->Age == number[0]) {
					pSelProp = pView->m_aProps.GetAt(i);
					break;
				}
			}
		}
		break;
	case PROP_BY_MATERIAL:
		input = m_strProp;
		if(m_strProp == _T("")) {
			AfxMessageBox(_T("Please select only 1 property !"));  
			break;
		}
		else {
			for(i = 0; i < pView->m_aProps.GetSize(); i++) {
				if(pView->m_aProps.GetAt(i)->Material.Compare(input) == 0) {
					pSelProp = pView->m_aProps.GetAt(i);
					break;
				}
			}
		}
		break;
	default:
		AfxMessageBox(_T("Error: No Selection !"));
	}
	delete number;

	// Get items by property
	int iProperty = pSelProp->ID; // ID
	int nSelectObj = m_pSelObj.GetCurSel();
	switch(nSelectObj) {
	case SELECT_POINT:
		for(i = 0; i < pView->m_aNds.GetSize(); i++) {
			if(pView->m_aNds.GetAt(i)->iProperty == iProperty) {
			   id = pView->m_aNds.GetAt(i)->ID;
			   m_aSelectedIDs.Add(id);
			}
		}
		break;
	case SELECT_LINE:
        // Not Done !
		break;
	case SELECT_AREA:
		for(i = 0; i < pView->m_aLps.GetSize(); i++) {
			if(pView->m_aLps.GetAt(i)->iProperty == iProperty) {
			   id = pView->m_aLps.GetAt(i)->ID;
			   m_aSelectedIDs.Add(id);
			}
		}
		break;
	case SELECT_SURFACE:
        // Not Done !
		break;
	case SELECT_POLYHEDRA:
		for(i = 0; i < pView->m_aPhds.GetSize(); i++) {
			if(pView->m_aPhds.GetAt(i)->iProperty == iProperty) {
			   id = pView->m_aPhds.GetAt(i)->ID;
			   m_aSelectedIDs.Add(id);
			}
		}
		break;
	case SELECT_BLOCK:
		for(i = 0; i < pView->m_aBlks.GetSize(); i++) {
			if(pView->m_aBlks.GetAt(i)->iProperty == iProperty) {
			// id = pView->m_aBlks.GetAt(i)->ID;
				id = i;
			   m_aSelectedIDs.Add(id);
			}
		}
		break;
	default:
		AfxMessageBox(_T("Error: No Selection !"));
	}

}

// Get local selected IDs by Adjacent
void CSelOption::GetLocalSelectedIDsByAdja()
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	int i, j, id, nSelectObj = m_pSelObj.GetCurSel();
	CPohd* pPhd;
	CBlock* pBlk;

	switch(nSelectObj) {
	case SELECT_POINT:
		// Not Done !
		break;
	case SELECT_LINE:
        // Not Done !
		break;
	case SELECT_AREA:
        // Not Done !
		break;
	case SELECT_SURFACE:
        // Not Done !
		break;
	case SELECT_POLYHEDRA: // Neighbors of Polyhedron
	    for(i = 0; i < pView->m_nSelectedIDs.GetSize(); i++) {
			id = pView->m_nSelectedIDs.GetAt(i);
			pPhd = pView->m_aPhds.GetAt(id);
			for(j = 0; j < pPhd->iNbs.GetSize(); j++) {
				m_aSelectedIDs.Add(pPhd->iNbs.GetAt(j));
			}
	    }
		break;
	case SELECT_BLOCK: // Neighbors of Block
	    for(i = 0; i < pView->m_nSelectedIDs.GetSize(); i++) {
			id = pView->m_nSelectedIDs.GetAt(i);
			pBlk = pView->m_aBlks.GetAt(id);
			for(j = 0; j < pBlk->iNbs.GetSize(); j++) {
				m_aSelectedIDs.Add(pBlk->iNbs.GetAt(j));
			}
	    }
		break;
	default:
		AfxMessageBox(_T("Error: No Selection !"));
	}

}

// Update global selected IDs from its local ones
void CSelOption::UpdateGlobalSelectedIDs()
{
	// #define SELECT_POINT         0  // Point
    // #define SELECT_LINE          1  // Line
    // #define SELECT_AREA          2  // Area
    // #define SELECT_SURFACE       3  // Surface
    // #define SELECT_POLYHEDRA     4  // Polyhedra  
    // #define SELECT_BLOCK         5  // Block

	// #define SEL_BY_NUM_PICK      0  // By Num/Pick
    // #define SEL_BY_PROPERTY      1  // By Property
    // #define SEL_BY_ADJACENT      2  // By Adjacent to

    // #define PROP_BY_ID           0  // By ID
    // #define PROP_BY_NAME         1  // By Name
    // #define PROP_BY_AGE          2  // By Age
    // #define PROP_BY_MATERIAL     3  // By Material

	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	int i, id, nSelectObj, nSelectBy, nSelectProp;

	nSelectObj = m_pSelObj.GetCurSel();
	nSelectBy = m_pSelBy.GetCurSel();
	switch(nSelectBy) {
	case SEL_BY_NUM_PICK:
		break;
	case SEL_BY_PROPERTY:
		break;
	case SEL_BY_ADJACENT:
		break;
	default:
		AfxMessageBox(_T("Error: No Selection !"));
	}

	switch(nSelectObj) {
	case SELECT_POINT:
		for(i = 0; i < m_aSelectedIDs.GetSize(); i++) {
			id = m_aSelectedIDs.GetAt(i);
			pView->m_nSelectedPoints.Add(id);
		}
		break;
	case SELECT_LINE:
		for(i = 0; i < m_aSelectedIDs.GetSize(); i++) {
			id = m_aSelectedIDs.GetAt(i);
			pView->m_nSelectedLines.Add(id);
		}
		break;
	case SELECT_AREA:
		for(i = 0; i < m_aSelectedIDs.GetSize(); i++) {
			id = m_aSelectedIDs.GetAt(i);
			pView->m_nSelectedAreas.Add(id);
		}
		break;
	case SELECT_SURFACE:
		for(i = 0; i < m_aSelectedIDs.GetSize(); i++) {
			id = m_aSelectedIDs.GetAt(i);
			pView->m_nSelectedSurfaces.Add(id);
		}
		break;
	case SELECT_POLYHEDRA:
		for(i = 0; i < m_aSelectedIDs.GetSize(); i++) {
			id = m_aSelectedIDs.GetAt(i);
			pView->m_nSelectedPolyhedrons.Add(id);
		}
		break;
	case SELECT_BLOCK:
		for(i = 0; i < m_aSelectedIDs.GetSize(); i++) {
			id = m_aSelectedIDs.GetAt(i);
			pView->m_nSelectedBlocks.Add(id);
		}
		break;
	default:
		AfxMessageBox(_T("Error: No Selection !"));
	}

}

// Reset, not important at present, but be important later
void CSelOption::Reset()
{
	//m_pSelObj;
	//m_pSelBy;
	m_nRange = 0;
	UpdateData(FALSE);
}