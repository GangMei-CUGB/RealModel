#if !defined(AFX_RENDERVIEW_H__7B1AB256_C4AE_45A0_A80D_A7DD259CC9E6__INCLUDED_)
#define AFX_RENDERVIEW_H__7B1AB256_C4AE_45A0_A80D_A7DD259CC9E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenderView.h : header file
//
#include "ZoomView.h"
#include "Cordnt.h"
#include "Count.h"
#include "Afxtempl.h"
#include"gl\gl.h"
#include"gl\glu.h"

/////////////////////////////////////////////////////////////////////////////
// CRenderView view

class CRenderView : public CZoomView
{
protected:
	CRenderView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRenderView)

// Attributes
public:
	//opengl para
    enum { ORTHOGRAPHIC, PERSPECTIVE };

    GLdouble m_dZrange;
	GLdouble m_dXrange;
	GLdouble m_dYrange;
	GLdouble m_dFOV;//y方向的视角
	CClientDC	*m_pDC;
	CRect		m_oldRect;
	bool m_bGlobal;
	bool m_bOpenGL;
	GLfloat m_fLocalAmbient[4];
	GLfloat m_lightAmb[4], m_lightDif[4], m_lightSpe[4];
	
	GLfloat m_materialAmb[4], m_materialDif[4], m_materialSpe[4], m_lightPos[4];
	GLfloat m_fRotate[3], m_fTranslate[3], m_fScale[4];
	GLfloat m_spotExponent;
	GLfloat m_fColor[3];
	HPALETTE	m_hPalette;			//调色板
    GLfloat m_pModelCenter[3];//模型中心坐标

	CPoint MouseDownPoint, ptCurrent, ptPrevious, ptOrigin;
	double x_angle;
	double y_angle;

public:
   // may have problems
   CTypedPtrArray<CObArray, CCount*>m_aList;
   // CTypedPtrArray<CObArray, CCordnt*>m_aList;

// Operations
public:
	long SetNewList();
	void DrawList();
	void DrawPoint(CDC *pDC, double x, double y, COLORREF color);
    void DrawPoint(CDC *pDC, CPoint Point, COLORREF color);
    void DrawLineTo(CDC *pDC, CPoint point, COLORREF color);
	void DrawAxis();
	void CreateRenderLight();
	void CalculateModelCenter( CCordnt* crt); //计算模型的中心
	double CalculateFovy(double size, double distance); //计算透视参数Fovy
	void SetLogicalPalette(void);
	void ChangeSpotlightFactor(GLfloat *amb, GLfloat *dif, GLfloat *spe);
	void CreateMaterial(long clr);
	void ChangeGlobalAmbient(GLfloat *amb, bool flag= true);
	void CreateFixedSpotlight();
	void AccumlateScene(int j, int num);
	void CreateSpotlight(GLfloat *position, GLfloat *direction, GLfloat cutoff);
	void CreateSun(GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 1.0);
	BOOL bSetupPixelFormat();
    void CalculateNormal(GLdouble *fVertex1, GLdouble *fVertex2, GLdouble *fVertex3, GLdouble *fNormal);
	void Render();
	void Init();

	void CalculateNormalEGC(GLfloat *fVertex1, GLfloat *fVertex2, GLfloat *fVertex3, GLfloat *fNormal);
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenderView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRenderView();
	int m_nProjectionType;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRenderView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnImageColor();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnImageCapture();
};
#ifndef _DEBUG  // debug version in RockModelView.cpp

#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENDERVIEW_H__7B1AB256_C4AE_45A0_A80D_A7DD259CC9E6__INCLUDED_)
