#pragma once


// CSelNumPick dialog
#include"RealModelDoc.h"
#include"RealModelView.h"

#include<afxtempl.h>

class CSelNumPick : public CDialogEx
{
	DECLARE_DYNAMIC(CSelNumPick)

public:
	CSelNumPick(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelNumPick();
	int CStringToInt(CString str, int* Number);
	void GetLocalSelectedIDs();
	void Reset();
	CArray<int, int> m_aSelectedIDs; // locoal selected IDs

// Dialog Data
	enum { IDD = IDD_SEL_NUM_PICK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nPick;
	int m_nSingle;
	int m_nCount;
	int m_nMax;
	int m_nMin;
	int m_nNum;
	CString m_strInput;
	int m_nItems;
	virtual void OnOK();
	afx_msg void OnBnClickedButtonApply();
};
