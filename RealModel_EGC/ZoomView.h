#if !defined(AFX_ZOOMVIEW_H__1EE10FF4_48FE_4C39_A3D0_FF58234E36AC__INCLUDED_)
#define AFX_ZOOMVIEW_H__1EE10FF4_48FE_4C39_A3D0_FF58234E36AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomView.h : header file
//
typedef struct ScreenPara//������¼��ʷ��Ļ�����Ľṹ
{
	double blc;//������
	double sx;//��Ļ��С�ǵĺ�����
	double sy;//��Ļ���½ǵ�������
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
	long nHScrollMax,//����������Ĺ�����Χ
		 nVScrollMax,//����������Ĺ�����Χ
		 nHScrollPos,//���������������λ��
		 nVScrollPos;//���������������λ��
	long nXPage,//���к�����������м�����ʱ�����ķ�Χ������������С�������룩
		 nYPage,//����������������м�����ʱ�����ķ�Χ������������С�������룩
		 nXLine,//���к����������ťʱ������Χ������������С�������룬һ��Ϊ1��
		 nYLine,//���������������ťʱ������Χ������������С�������룬һ��Ϊ1��
		 nScrollMin;//��λ������Χ������������

public:
	int m_CurrentScreen;//��ǰ��Ļ�����
	int m_MaxScreen;//����ܹ��е���Ļ��
	ScreenStruct * m_Screen;//��¼��Ļ�����Ľṹָ��
	double m_xStart,m_yStart;//�����Ļ���½�ʵ������ͱ�����
	int m_wScreen,m_hScreen;//��ſͻ�����ʵ�ʿ�Ⱥ͸߶ȣ��߼����꣩
	double blc, m_iblc;
	int m_MapMode, m_ixStart, m_iyStart, IXStart, IYStart;//Ӱ��ģʽ
	double m_dTmppointx,m_dTmppointy,m_dPointOrignx,m_dPointOrigny,m_dPointOrign1x,m_dPointOrign1y;
	CPoint mPointOrign,mPointOrign1,mPointOld;
	long m_nInitDraw;
	//��ͼʱ�õ��Ĳ���
	short m_pColor;//��ǰ������ɫ�����
	short m_bColor;//��ǰ��ɫ�����
	short m_brColor;//��ǰ��ˢ��ɫ�����
	short m_LineWide;//��ǰֱ�߿�ȣ����أ�
	short m_LineType;//��ǰ����
	short m_Layer;//��ǰ��
// Operations
public:
	BOOL RectCross(double &x1, double &y1, double &x2, double &y2, double xx1, double yy1, double xx2, double yy2);
	void ReDrawRect(double X1, double Y1, double X2, double Y2);
	double CalDisp(double x1, double y1, double x2, double y2);
	void SetPosition(double x,double y);//����������(x,y)��
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
