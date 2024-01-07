#pragma once

#include "PropNew.h"
#include "Prop.h"
#include"RealModelDoc.h"
#include"RealModelView.h"
// CPropDef dialog

class CPropDef : public CDialogEx
{
	DECLARE_DYNAMIC(CPropDef)

public:
	CPropDef(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropDef();

// Dialog Data
	enum { IDD = IDD_PROP_DEF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_pList;
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonRemove();
	virtual void OnOK();
};
