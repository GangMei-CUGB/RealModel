#pragma once


// CPropNew dialog

class CPropNew : public CDialogEx
{
	DECLARE_DYNAMIC(CPropNew)

public:
	CPropNew(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropNew();
	int RGB[3];

// Dialog Data
	enum { IDD = IDD_PROP_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nAge;
	int m_nID;
	CString m_strMaterial;
	CString m_strName;
	CString m_strStatement;
	CMFCColorButton m_pMFCColor;
	afx_msg void OnBnClickedMFCColor();
	virtual BOOL OnInitDialog();
};
