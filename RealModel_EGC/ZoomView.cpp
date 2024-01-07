// ZoomView.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "RealModel.h"
#include "ZoomView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define pi acos(-1.0)
//CZoomView * g_pView;//ȫ�ֶ���

 double xMinScreen,yMinScreen,xMaxScreen,yMaxScreen;//ȫ�ֶ���
/*BOOL b_Draw;
void DPtoVP(double x,double y,long &X,long &Y)//ȫ�ֺ���
{
	g_pView->DPtoVP(x,y,X,Y);
}
void VPtoDP(long x,long y,double &X,double &Y)//ȫ�ֺ���
{
	g_pView->VPtoDP(x,y,X,Y);
}
long DLtoVL(double l)//ȫ�ֺ���
{
	return g_pView->DLtoVL(l);
}
double VLtoDL(long l)//ȫ�ֺ���
{
	return g_pView->VLtoDL(l);
}*/
/////////////////////////////////////////////////////////////////////////////
// CZoomView

IMPLEMENT_DYNCREATE(CZoomView, CView)

CZoomView::CZoomView()
{
	m_xStart = 0.0;
	m_yStart = 0.0;
	m_ixStart = 10;
	m_iyStart = 10;
	IXStart = m_ixStart;
	IYStart = m_iyStart;
	blc = 1.0;
	nScrollMin = 50; //����������С������Χ������Ļ�Ϲ���50������
	nXLine = 1;//�������һ��������Χ�����й������İ���ʱ�Ĺ�����
	nYLine = 1;//�������һ��������Χ�����й������İ���ʱ�Ĺ�����
	m_MaxScreen = 10000;
	m_Screen = new ScreenStruct[m_MaxScreen];
	m_CurrentScreen = 0;
	m_Screen[0].sx = m_xStart;
	m_Screen[0].sy = m_yStart;
	m_Screen[0].blc = blc;
	m_nInitDraw = 0;
	m_pColor = 0;//��ǰ������ɫ�����
	m_bColor = 0;//��ǰ��ɫ�����
	m_brColor = 0;//��ǰ��ˢ��ɫ�����
	m_LineWide = 1;//��ǰֱ�߿�ȣ����أ�
	m_LineType = 0;//��ǰ����
	m_Layer = 1;//��ǰ��
}

CZoomView::~CZoomView()
{
}


BEGIN_MESSAGE_MAP(CZoomView, CView)
	//{{AFX_MSG_MAP(CZoomView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
//	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomView drawing

void CZoomView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CZoomView diagnostics

#ifdef _DEBUG
void CZoomView::AssertValid() const
{
	CView::AssertValid();
}

void CZoomView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZoomView message handlers

BOOL CZoomView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
//	HBRUSH hbkbrush=CreateSolidBrush(RGB(192,192,192));//������ɫ����ˢ
 //   LPCSTR lpMyOwnClass=AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_OWNDC,0,hbkbrush);//ע������
 //   cs.lpszClass=lpMyOwnClass;//�޸�ȱʡ������
//	cs.style = cs.style | WS_HSCROLL | WS_VSCROLL;
	return CView::PreCreateWindow(cs);
//	return CMDIChildWnd::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CZoomView printing

BOOL CZoomView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CZoomView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CZoomView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/*CFiniteDoc* CZoomView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFiniteDoc)));
	return (CFiniteDoc*)m_pDocument;
}*/
/////////////////////////////////////////////////////////////////////////////
// CZoomView message handlers

void CZoomView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	m_wScreen = cx;//�ͻ����Ŀ�ȣ��߼����꣩
	m_hScreen = cy;//�ͻ����ĸ߶ȣ��߼����꣩
	nXPage = m_hScreen/nScrollMin;//�������һ����Ҫ�Ļ���������
	nYPage = m_wScreen/nScrollMin;//�������һ����Ҫ�Ļ���������
	InitHScroll();
	InitVScroll();
}

void CZoomView::DPtoVP(double x, double y, int &X, int &Y)
{
	X = int((x - m_xStart)/blc) + m_ixStart;               //�õ�����߼����������
	if(m_MapMode == 1)						   //�����MM_TEXTӰ��ģʽ
		Y = m_hScreen - int((y - m_yStart)/blc) - m_iyStart;    //�õ�����߼�����������
	else                                       //���������Ӱ��ģʽ
		Y = int((y - m_yStart)/blc) - m_hScreen + m_iyStart;    //�õ�����߼�����������
}
void CZoomView::DPtoVP(double x, double y, long &X, long &Y)
{
	X = long((x - m_xStart)/blc) + m_ixStart;               //�õ�����߼����������
	if(m_MapMode == 1)						   //�����MM_TEXTӰ��ģʽ
		Y = m_hScreen - long((y - m_yStart)/blc) - m_iyStart;    //�õ�����߼�����������
	else                                       //���������Ӱ��ģʽ
		Y = long((y - m_yStart)/blc) - m_hScreen + m_iyStart;    //�õ�����߼�����������
}

void CZoomView::VPtoDP(long x, long y, double &X, double &Y)
{
	X = m_xStart + (x - m_ixStart) * blc;          //�õ����ʵ�����������
	if(m_MapMode == 1)						   //�����MM_TEXTӰ��ģʽ
		Y = m_yStart + blc*(m_hScreen - y - m_iyStart);    //�õ����ʵ������������
	else                                       //���������Ӱ��ģʽ
		Y = m_yStart + blc*(y + m_hScreen - m_iyStart);    //�õ����ʵ������������
}
void CZoomView::VPtoDP(int x, int y, double &X, double &Y)
{
	X = m_xStart + (x - m_ixStart) * blc;                      //�õ����ʵ�����������
	if(m_MapMode == 1)						   //�����MM_TEXTӰ��ģʽ
		Y = m_yStart + blc*(m_hScreen - y - m_iyStart);    //�õ����ʵ������������
	else                                       //���������Ӱ��ģʽ
		Y = m_yStart + blc*(y + m_hScreen - m_iyStart);    //�õ����ʵ������������
}

double CZoomView::VLtoDL(int l)
{
	return double(blc*l);
}
double CZoomView::VLtoDL(long l)
{
	return double(blc*l);
}
long CZoomView::DLtoVL(double l)
{
	return long(l/blc);
}

void CZoomView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	m_MapMode = MM_TEXT;
}

void CZoomView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CZoomView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	long nScrollInc;
	int nNewPos;
	CRect rr;
	switch(nSBCode)//�жϰ�����λ��
	{
		case SB_TOP://���������������������
			nScrollInc = -nVScrollPos;//�õ���Թ�����Χ
			break;
		case SB_BOTTOM://������������������ײ�
			nScrollInc = nVScrollMax - nVScrollPos;//�õ���Թ�����Χ
			break;
		case SB_LINEUP://��������˹����������ϵİ���
			nScrollInc = -nYLine;//�õ���Թ�����Χ
			break;
		case SB_LINEDOWN://��������˹����������µİ���
			nScrollInc = nYLine;//�õ���Թ�����Χ
			break;
		case SB_PAGEUP://����������м���ť���ϲ�����
			nScrollInc = -nYPage;//�õ���Թ�����Χ
			break;
		case SB_PAGEDOWN://����������м���ť���²�����
			nScrollInc = nYPage;//�õ���Թ�����Χ
			break;
		case SB_THUMBPOSITION://���������϶��м���ť��һ��λ��
			nScrollInc = nPos - nVScrollPos;//�õ�����ƶ�����С������Χ��
			break;
		default:
			nScrollInc = 0;//�õ���Թ�����Χ
	}
	//���й����߽��飬�õ�ʵ�ʵĹ���λ�ã����ܳ����������Ĺ�����Χ��
	nNewPos = max(0,min(nVScrollPos + nScrollInc,nVScrollMax));
	//�õ�ʵ�ʵ���Թ�����Χ
	nScrollInc = nNewPos - nVScrollPos;
	if(nScrollInc)//��������˹���
	{
		nVScrollPos = nNewPos;//�趨�µĹ���λ��
		SetScrollPos(SB_VERT,nVScrollPos);
		UpdateWindow();//���¹�����
		//����������ʹͼ�β�������
		m_yStart = m_yStart - blc*nScrollInc*nScrollMin;
		GetClientRect(&rr);//�õ��ͻ����ľ��α߽�
		if(abs(nScrollInc) * nScrollMin < rr.bottom)
		//���������Ļ�����ǰ���ص�
		{
			if(nScrollInc > 0)//�����ͼ�����Ϲ������������Ļ���ص�����ľ��α߽�
				rr.top = nScrollInc*nScrollMin;
			else//���ͼ�����¹���
				rr.bottom = rr.bottom + nScrollInc*nScrollMin;//�õ��ص�����
			ScrollWindow(0,-nScrollInc*nScrollMin,rr);//�����ص�������
			if(nScrollInc > 0)//��������Ϲ���
				rr.top = rr.bottom - nScrollInc*nScrollMin;//�õ���Ҫ�ػ�����
			else//��������¹���
				rr.bottom = -nScrollInc*nScrollMin;//�õ���Ҫ�ػ�������
			InvalidateRect(rr,1);//��ͼ�ν��оֲ��ػ�
		}
		else//�������������������ǰ������û���ص�����ȫ���ػ�
			Invalidate();
	}
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CZoomView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	long nScrollInc;
	int nNewPos;
	CRect r1;
	switch(nSBCode)
	{
		case SB_LEFT:
			nScrollInc = -nHScrollPos;
			break;
		case SB_RIGHT:
			nScrollInc = nHScrollMax - nHScrollPos;
			break;
		case SB_LINELEFT:
			nScrollInc = -nXLine;
			break;
		case SB_LINERIGHT:
			nScrollInc = nXLine;
			break;
		case SB_PAGEUP:
			nScrollInc = -nXPage;
			break;
		case SB_PAGEDOWN:
			nScrollInc = nXPage;
			break;
		case SB_THUMBPOSITION:
			nScrollInc = nPos - nHScrollPos;
			break;
		default:
			nScrollInc = 0;
	}
	nNewPos = max(0,min(nHScrollPos + nScrollInc,nHScrollMax));
	nScrollInc = nNewPos - nHScrollPos;
	if(nScrollInc)//��������˹���
	{
		nHScrollPos = nNewPos;//�趨�µĹ���λ��
		SetScrollPos(SB_HORZ,nHScrollPos);
		UpdateWindow();//�ı��������λ��
		m_xStart = m_xStart + blc*nScrollInc*nScrollMin;//����������ʹͼ�β�������
		GetClientRect(&r1);//�õ��ͻ����ľ��α߽�
		if(abs(nScrollInc)*nScrollMin < r1.right)//������������Ļ�����ǰ�����ص�
		{
			if(nScrollInc > 0)//���ʹͼ�����Ϲ���
				r1.left = nScrollInc*nScrollMin;//�õ�������Ļ���ص�����ľ���
			else//���ͼ�����¹���
				r1.right = r1.right - nScrollInc*nScrollMin;//�õ��ص�����ľ���
			ScrollWindow(-nScrollInc*nScrollMin,0,r1);//�����ص�������
			if(nScrollInc > 0)//��������Ϲ���
				r1.left = r1.right - nScrollInc*nScrollMin;//�õ���Ҫ�ػ�������
			else//��������¹���
				r1.right = -nScrollInc*nScrollMin;//�õ���Ҫ�ػ�������
			InvalidateRect(r1,0);//��ͼ�ν��оֲ��ػ�
		}
		else//�������������������ǰ������û���ص�����ȫ���ػ�
		Invalidate();
	}
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CZoomView::DrawPoint(CDC *pDC, double x, double y, short brushColor)
{
	CBrush brushRectPoint;
	CPen penPoint;
//	brushRectPoint.CreateSolidBrush(p_GraphPara->GetColor(brushColor));
//	penPoint.CreatePen(PS_SOLID,0,p_GraphPara->GetColor(brushColor));
	CRect rectPoint;
	rectPoint.bottom = int(y + 4);
	rectPoint.right = int(x + 4);
	rectPoint.top = int(y - 4);
	rectPoint.left = int(x - 4);
	CBrush * pOldBrush = pDC->SelectObject(&brushRectPoint);
	CPen * pOldPen = pDC->SelectObject(&penPoint);
	pDC->LPtoDP(&rectPoint);
	pDC->Rectangle(rectPoint);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	brushRectPoint.DeleteObject();
	penPoint.DeleteObject();
}

/*void CFiniteView::OnViewZoomin() 
{
	// TODO: Add your command handler code here
}*/

void CZoomView::AddScreen(double StartX, double StartY, double blc)
{
	if(m_CurrentScreen == m_MaxScreen)
	//�����ǰ��Ļ��¼����������һ�������û�пռ��ٴ���Ϣ
	{
		//����������ʽ������ȥ���ڶ�����[0]�д��ʽ������Ϣ������ʱ���ı䣩
		for(int i = 1; i < m_MaxScreen -1; i++)
			m_Screen[i] = m_Screen[i+1];
	}
	else //�����������������һ���Ļ��¼������1
		m_CurrentScreen++;
	//��¼�±���Ļ�Ĳ���
	m_Screen[m_CurrentScreen].sx = StartX;
	m_Screen[m_CurrentScreen].sy = StartY;
	m_Screen[m_CurrentScreen].blc = blc;
}

void CZoomView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
		case VK_HOME://��Home�¼�ʱ
			OnHScroll(SB_PAGEUP,0,NULL);
			break;
		case VK_END://����End��ʱ
			OnHScroll(SB_PAGEDOWN,0,NULL);
			break;
		case VK_PRIOR://����PgUp��ʱ
			OnVScroll(SB_PAGEUP,0,NULL);
			break;
		case VK_NEXT://����PgDn��ʱ
			OnVScroll(SB_PAGEDOWN,0,NULL);
			break;
		case VK_UP://�����ϼ�ͷ
			OnVScroll(SB_LINEUP,0,NULL);
			break;
		case VK_DOWN://�����¼�ͷ
			OnVScroll(SB_LINEDOWN,0,NULL);
			break;
		case VK_LEFT://�������ͷ
			OnHScroll(SB_LINEUP,0,NULL);
			break;
		case VK_RIGHT://�����Ҽ�ͷ
			OnHScroll(SB_LINEDOWN,0,NULL);
			break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CZoomView::InitHScroll()//�趨���������
{
	//�õ������������Χ
	nVScrollMax = int((double(m_hScreen)*m_Screen[0].blc)/(blc*nScrollMin));
	//�õ���ǰ�Ĺ�����λ��
	nVScrollPos = nVScrollMax - 
		int((m_yStart - m_Screen[0].sy)/(blc*nScrollMin));
	//���ù����������Χ�͵�ǰλ��
	SetScrollRange(SB_VERT,0,nVScrollMax,0);
	SetScrollPos(SB_VERT,nVScrollPos);///rrr.top = 0;
	UpdateWindow();
}

void CZoomView::InitVScroll()//�趨���������
{
	nHScrollMax = int((double(m_hScreen)*m_Screen[0].blc)/(blc*nScrollMin));
	nHScrollPos = int((m_xStart - m_Screen[0].sx)/(blc*nScrollMin));
	SetScrollRange(SB_HORZ,0,nHScrollMax,0);
	SetScrollPos(SB_HORZ,nHScrollPos);//rrr.top = 0;
	UpdateWindow();
}

void CZoomView::SetPosition(double x, double y)
{
	CPoint point;
	DPtoVP(x,y,point.x,point.y);
	ClientToScreen(&point);
	SetCursorPos(point.x,point.y);
}

double CZoomView::CalDisp(double x1, double y1, double x2, double y2)
{
	return (double)sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void CZoomView::ReDrawRect(double X1, double Y1, double X2, double Y2)
{
	double xx1,xx2,yy1,yy2;
	CRect r1;
	xx1 = xMinScreen; xx2 = xMaxScreen;
	yy1 = yMinScreen; yy2 = yMaxScreen;
	//�õ���������ͼ�����ཻ�ľ���
	BOOL IsCross = RectCross(xx1,yy1,xx2,yy2,X1,Y1,X2,Y2);
	if(IsCross)//����ཻ�����ػ���һ����
	{
		//���µõ����������߼�����
		r1.left = int((xx1 - m_xStart)/blc) - 1;
		r1.right = int((xx2 - m_xStart)/blc) + 1;
		r1.top = m_hScreen - int((yy2 - m_yStart)/blc) - 1;
		r1.bottom = m_hScreen - int((yy1 - m_yStart)/blc) + 1;
		InvalidateRect(r1);
	}
}
BOOL CZoomView::RectCross(double &x1, double &y1, double &x2, double &y2, double xx1, double yy1, double xx2, double yy2)
{
	if(x1 > xx2 || x2 < xx1 || y1 > yy2 || y2 < yy1)
		return FALSE;//�粻�ཻ����������0
	else//���������ཻ���õ��ཻ���ε�����
	{
		x1 = max(x1,xx1);
		y1 = max(y1,yy1);
		x2 = min(x2,xx2);
		y2 = min(y2,yy2);
		return TRUE;
	}
}
