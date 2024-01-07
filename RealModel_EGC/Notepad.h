#pragma once


// CNotepad dialog

class CNotepad : public CDialogEx
{
	DECLARE_DYNAMIC(CNotepad)

public:
	CNotepad(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNotepad();

// Dialog Data
	enum { IDD = IDD_NOTEPAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
