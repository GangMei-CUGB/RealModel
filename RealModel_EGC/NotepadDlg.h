#pragma once


// CNotepadDlg dialog

class CNotepadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNotepadDlg)

public:
	CNotepadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNotepadDlg();

// Dialog Data
	enum { IDD = IDD_NOTEPAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CRichEditCtrl m_pRichEdit;
};
