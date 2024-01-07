#pragma once

#include "RealModelDoc.h"
#include "RealModelView.h"
#include "SelNumPick.h"
#include <afxtempl.h>

// CPropSet dialog

class CPropSet : public CDialogEx
{
	DECLARE_DYNAMIC(CPropSet)

public:
	CPropSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropSet();
	CArray<int, int> m_nSelectedIDs;
	int iProperty;

// Dialog Data
	enum { IDD = IDD_PROP_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_pProperty;
	CString m_strItems;
	int m_nAge;
	int m_nID;
	CString m_strMaterial;
	CString m_strName;
	CString m_strStatement;
	CMFCColorButton m_pMFCColor;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCbnSelchangeComboProperty();
	afx_msg void OnBnClickedButtonSelect();
};
