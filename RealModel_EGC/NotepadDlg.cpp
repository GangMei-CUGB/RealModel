// NotepadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RealModel.h"
#include "NotepadDlg.h"
#include "afxdialogex.h"


// CNotepadDlg dialog

IMPLEMENT_DYNAMIC(CNotepadDlg, CDialogEx)

CNotepadDlg::CNotepadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNotepadDlg::IDD, pParent)
{

}

CNotepadDlg::~CNotepadDlg()
{
}

void CNotepadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_TEXT, m_pRichEdit);
}


BEGIN_MESSAGE_MAP(CNotepadDlg, CDialogEx)
END_MESSAGE_MAP()


// CNotepadDlg message handlers


BOOL CNotepadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
