#pragma once

#include"SelNumPick.h"

// CSelOption dialog

class CSelOption : public CDialogEx
{
	DECLARE_DYNAMIC(CSelOption)

public:
	CSelOption(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelOption();
	void GetLocalSelectedIDs();
	void GetLocalSelectedIDsByNumPick();
	void GetLocalSelectedIDsByProp();
	void GetLocalSelectedIDsByAdja();
	void UpdateGlobalSelectedIDs();
	void Reset();
	CArray<int, int> m_aSelectedIDs; // locoal selected IDs

// Dialog Data
	enum { IDD = IDD_SEL_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_pSelObj;
	CComboBox m_pSelBy;
	int m_nRange;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeComboObject();
	afx_msg void OnSelChangeComboSelBy();
	afx_msg void OnClickedButtonReplot();
	afx_msg void OnClickedButtonApply();
	virtual void OnOK();
	CComboBox m_pSelProp;
//	CEdit m_strProp;
	CString m_strProp;
};
