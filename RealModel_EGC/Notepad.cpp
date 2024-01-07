// Notepad.cpp : implementation file
//

#include "stdafx.h"
#include "RealModel.h"
#include "Notepad.h"
#include "afxdialogex.h"


// CNotepad dialog

IMPLEMENT_DYNAMIC(CNotepad, CDialogEx)

CNotepad::CNotepad(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNotepad::IDD, pParent)
{

}

CNotepad::~CNotepad()
{
}

void CNotepad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNotepad, CDialogEx)
END_MESSAGE_MAP()


// CNotepad message handlers
