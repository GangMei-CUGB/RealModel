#pragma once


// CDefProperty dialog

class CDefProperty : public CDialogEx
{
	DECLARE_DYNAMIC(CDefProperty)

public:
	CDefProperty(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDefProperty();

// Dialog Data
	enum { IDD = IDD_DEF_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_pList;
};
