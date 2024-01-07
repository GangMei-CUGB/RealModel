// SelNumPick.cpp : implementation file
//

#include "stdafx.h"
#include "SelNumPick.h"
#include "RealModel.h"
#include "afxdialogex.h"


// CSelNumPick dialog

IMPLEMENT_DYNAMIC(CSelNumPick, CDialogEx)

CSelNumPick::CSelNumPick(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelNumPick::IDD, pParent)
{

	m_nPick = 0;
	m_nSingle = 0;
	m_nCount = 0;
	m_nMax = 0;
	m_nMin = 0;
	m_nNum = 0;
	m_strInput = _T("");
	m_nItems = 0;
}

CSelNumPick::~CSelNumPick()
{
}

void CSelNumPick::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_PICK, m_nPick);
	DDX_Radio(pDX, IDC_RADIO_SINGLE, m_nSingle);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
	DDX_Text(pDX, IDC_EDIT_MAX, m_nMax);
	DDX_Text(pDX, IDC_EDIT_MIN, m_nMin);
	DDX_Text(pDX, IDC_EDIT_NUM, m_nNum);
	DDX_Text(pDX, IDC_EDIT_INPUT_IDS, m_strInput);
	DDX_Radio(pDX, IDC_RADIO_LIST_ITEMS, m_nItems);
}


BEGIN_MESSAGE_MAP(CSelNumPick, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSelNumPick::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CSelNumPick message handlers
int  CSelNumPick::CStringToInt(CString str, int* Number) 
{ 
	int nLength = str.GetLength();
	int Position = 0; 
	int NumberOfArrary = 0; 
	CString tmp = str; // ��ʱ�ַ���
	CString Cur; //��ǰҪת��������   

	while(1) 
    { 
		Position = tmp.Find(',', 0);//Ѱ���ַ����м��   , 
		if(Position != -1)//�ж��Ƿ񻹴���   �� 
		{ 
			Cur = tmp.Left(Position);//�õ��ַ������� 
			//�ߵ����� 
			char *chr=new char[Cur.GetLength()];
            WideCharToMultiByte(CP_ACP,0,Cur.GetBuffer(),-1,chr,Cur.GetLength(),NULL,NULL);
	        Cur.ReleaseBuffer();

			Number[NumberOfArrary]   =   atoi(chr);//ת�� 
			nLength = tmp.GetLength();
			tmp = tmp.Right(nLength - Position - 1);//�õ�ʣ����ַ��� 
			NumberOfArrary++;  
		} 
		else 
		{ 
			Cur = tmp; 
			char *chr=new char[Cur.GetLength()];
            WideCharToMultiByte(CP_ACP,0,Cur.GetBuffer(),-1,chr,Cur.GetLength(),NULL,NULL);
	        Cur.ReleaseBuffer();

			Number[NumberOfArrary]   =   atoi(chr); 
			break; 
		}   
    }   

	return NumberOfArrary + 1;

} 


void CSelNumPick::OnOK()
{
	GetLocalSelectedIDs();
	CDialogEx::OnOK();
}

void CSelNumPick::OnBnClickedButtonApply()
{
	GetLocalSelectedIDs();
	Reset();
}

void CSelNumPick::GetLocalSelectedIDs()
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	UpdateData(TRUE);
	int* number = new int[1000];
	CString input = m_strInput;
	int n = CStringToInt(input, number);
	if(m_nItems == 0) {
	for(int i = 0; i < n; i++) {
		m_aSelectedIDs.Add(number[i]);
	    pView->m_nSelectedIDs.Add(number[i]); // Post to global
	}
	}
	else if(m_nItems == 1) {
		if(n != 2)  AfxMessageBox(_T("Too many numbers inputted !"));
		for(int i = number[0]; i < number[1] + 1; i++) {
		m_aSelectedIDs.Add(i);
	    pView->m_nSelectedIDs.Add(i); // Post to global
	}
	}
}

void CSelNumPick::Reset()
{
	m_nPick = 0;
	m_nSingle = 0;
	m_nCount = 0;
	m_nMax = 0;
	m_nMin = 0;
	m_nNum = 0;
	m_strInput = _T("");
	m_nItems = 0;
	UpdateData(FALSE);
}