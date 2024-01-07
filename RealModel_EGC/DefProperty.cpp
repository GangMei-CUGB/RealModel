// DefProperty.cpp : implementation file
//

#include "stdafx.h"
#include "RealModel.h"
#include "DefProperty.h"
#include "afxdialogex.h"


// CDefProperty dialog

IMPLEMENT_DYNAMIC(CDefProperty, CDialogEx)

CDefProperty::CDefProperty(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDefProperty::IDD, pParent)
{
	
}

CDefProperty::~CDefProperty()
{
}

void CDefProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_pList);
}


BEGIN_MESSAGE_MAP(CDefProperty, CDialogEx)
END_MESSAGE_MAP()


// CDefProperty message handlers


BOOL CDefProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	/*
	TCHAR rgtsz[2][10] = {_T("列头1"), _T("列头2")};
    LV_COLUMN lvcolumn;
    CRect rect;
    m_pList.GetWindowRect(&rect);
	int i;
    for(i=0; i<2; i++)
    {
	   lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	   lvcolumn.fmt = LVCFMT_LEFT;
	   lvcolumn.pszText = rgtsz[i];
	   lvcolumn.iSubItem = i;
	   lvcolumn.iOrder = i;
	   if(i == 0)
           lvcolumn.cx = rect.Width()*3/5 ; 
	   else
           lvcolumn.cx = rect.Width()*2/5;

       m_pList.InsertColumn(i, &lvcolumn);
    }
	*/
	// Insert table head
	m_pList.InsertColumn( 0, _T("ID"), LVCFMT_LEFT, 40 ); 
    m_pList.InsertColumn( 1, _T("Name"), LVCFMT_LEFT, 60 ); 
	m_pList.InsertColumn( 2, _T("Age"), LVCFMT_LEFT, 60 ); 
    m_pList.InsertColumn( 3, _T("Material"), LVCFMT_LEFT, 80 ); 
	m_pList.InsertColumn( 4, _T("Color"), LVCFMT_LEFT, 80 ); 
	m_pList.InsertColumn( 5, _T("Statement"), LVCFMT_LEFT, 100 );  
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
