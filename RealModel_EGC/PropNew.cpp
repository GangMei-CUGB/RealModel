// PropNew.cpp : implementation file
//

#include "stdafx.h"
#include "RealModel.h"
#include "PropNew.h"
#include "afxdialogex.h"


// CPropNew dialog

IMPLEMENT_DYNAMIC(CPropNew, CDialogEx)

CPropNew::CPropNew(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPropNew::IDD, pParent)
{

	m_nAge = 0;
	m_nID = 0;
	m_strMaterial = _T("");
	m_strName = _T("");
	m_strStatement = _T("");
	RGB[0] = 0;  RGB[1] = 0;  RGB[2] = 0;
}

CPropNew::~CPropNew()
{
}

void CPropNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AGE, m_nAge);
	DDX_Text(pDX, IDC_EDIT_ID, m_nID);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_strMaterial);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_STATEMENT, m_strStatement);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_COLOR, m_pMFCColor);
}


BEGIN_MESSAGE_MAP(CPropNew, CDialogEx)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON_COLOR, &CPropNew::OnBnClickedMFCColor)
END_MESSAGE_MAP()


// CPropNew message handlers


void CPropNew::OnBnClickedMFCColor()
{
	// TODO: Add your control notification handler code here
	COLORREF color = m_pMFCColor.GetColor(); 
    if (color == -1) {
        color = m_pMFCColor.GetAutomaticColor(); 
    } 
	RGB[0] = GetRValue(color);
	RGB[1] = GetGValue(color);
	RGB[2] = GetBValue(color);
}


BOOL CPropNew::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
