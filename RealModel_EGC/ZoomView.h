#if !defined(AFX_ZOOMVIEW_H__1EE10FF4_48FE_4C39_A3D0_FF58234E36AC__INCLUDED_)
#define AFX_ZOOMVIEW_H__1EE10FF4_48FE_4C39_A3D0_FF58234E36AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomView.h : header file
//
typedef struct ScreenPara//用来记录历史屏幕参数的结构
{
	double blc;//比例尺
	double sx;//屏幕左小角的横坐标
	double sy;//屏幕坐下角的纵坐标
}ScreenStruct;

/////////////////////////////////////////////////////////////////////////////
// CZoomView view

class CZoomView : public CView
{
protected:
	CZoomView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CZoomView)

// Attributes
protected:
	long nHScrollMax,//横向滚动条的滚动范围
		 nVScrollMax,//纵向滚动条的滚动范围
		 nHScrollPos,//横向滚动条的所在位置
		 nVScrollPos;//纵向滚动条的所在位置
	long nXPage,//按中横向滚动条的中间区域时滚动的范围（滚动几个最小滚动距离）
		 nYPage,//按中纵向滚动条的中间区域时滚动的范围（滚动几个最小滚动距离）
		 nXLine,//按中横向滚动条按钮时滚动范围（滚动几个最小滚动距离，一般为1）
		 nYLine,//按中纵向滚动条按钮时滚动范围（滚动几个最小滚动距离，一般为1）
		 nScrollMin;//单位滚动范围滚动的像素数

public:
	int m_CurrentScreen;//当前屏幕的序号
	int m_MaxScreen;//最多能够有的屏幕数
	ScreenStruct * m_Screen;//记录屏幕参数的结构指针
	double m_xStart,m_yStart;//存放屏幕左下角实际坐标和比例尺
	int m_wScreen,m_hScreen;//存放客户区的实际宽度和高度（逻辑坐标）
	double blc, m_iblc;
	int m_MapMode, m_ixStart, m_iyStart, IXStart, IYStart;//影射模式
	double m_dTmppointx,m_dTmppointy,m_dPointOrignx,m_dPointOrigny,m_dPointOrign1x,m_dPointOrign1y;
	CPoint mPointOrign,mPointOrign1,mPointOld;
	long m_nInitDraw;
	//绘图时用到的参数
	short m_pColor;//当前画笔颜色的序号
	short m_bColor;//当前底色的序号
	short m_brColor;//当前画刷颜色的序号
	short m_LineWide;//当前直线宽度（像素）
	short m_LineType;//当前线型
	short m_Layer;//当前层
// Operations
public:
	BOOL RectCross(double &x1, double &y1, double &x2, double &y2, double xx1, double yy1, double xx2, double yy2);
	void ReDrawRect(double X1, double Y1, double X2, double Y2);
	double CalDisp(double x1, double y1, double x2, double y2);
	void SetPosition(double x,double y);//将鼠标点置于(x,y)处
	void InitVScroll();
	void InitHScroll();
	void AddScreen(double StartX, double StartY, double blc);
	void DrawPoint(CDC *pDC, double x,double y,short bushColor);
	double VLtoDL(long l);
	double VLtoDL(int l);
	long DLtoVL(double l);
	void VPtoDP(long x, long y, double &X, double &Y);
	void VPtoDP(int x, int y, double &X, double &Y);
	void DPtoVP(double x, double y, long &X, long &Y);
	void DPtoVP(double x, double y, int &X, int &Y);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomView)
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CZoomView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CZoomView)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnViewZoomin();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMVIEW_H__1EE10FF4_48FE_4C39_A3D0_FF58234E36AC__INCLUDED_)
