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
//CZoomView * g_pView;//全局对象

 double xMinScreen,yMinScreen,xMaxScreen,yMaxScreen;//全局对象
/*BOOL b_Draw;
void DPtoVP(double x,double y,long &X,long &Y)//全局函数
{
	g_pView->DPtoVP(x,y,X,Y);
}
void VPtoDP(long x,long y,double &X,double &Y)//全局函数
{
	g_pView->VPtoDP(x,y,X,Y);
}
long DLtoVL(double l)//全局函数
{
	return g_pView->DLtoVL(l);
}
double VLtoDL(long l)//全局函数
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
	nScrollMin = 50; //滚动条的最小滚动范围是在屏幕上滚动50个像素
	nXLine = 1;//横向滚动一个滚动范围（按中滚动条的按键时的滚动）
	nYLine = 1;//纵向滚动一个滚动范围（按中滚动条的按键时的滚动）
	m_MaxScreen = 10000;
	m_Screen = new ScreenStruct[m_MaxScreen];
	m_CurrentScreen = 0;
	m_Screen[0].sx = m_xStart;
	m_Screen[0].sy = m_yStart;
	m_Screen[0].blc = blc;
	m_nInitDraw = 0;
	m_pColor = 0;//当前画笔颜色的序号
	m_bColor = 0;//当前底色的序号
	m_brColor = 0;//当前画刷颜色的序号
	m_LineWide = 1;//当前直线宽度（像素）
	m_LineType = 0;//当前线型
	m_Layer = 1;//当前层
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
//	HBRUSH hbkbrush=CreateSolidBrush(RGB(192,192,192));//创建灰色背景刷
 //   LPCSTR lpMyOwnClass=AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_OWNDC,0,hbkbrush);//注册新类
 //   cs.lpszClass=lpMyOwnClass;//修改缺省的类风格
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
	m_wScreen = cx;//客户区的宽度（逻辑坐标）
	m_hScreen = cy;//客户区的高度（逻辑坐标）
	nXPage = m_hScreen/nScrollMin;//横向滚动一屏需要的基本滚动数
	nYPage = m_wScreen/nScrollMin;//纵向滚动一屏需要的基本滚动数
	InitHScroll();
	InitVScroll();
}

void CZoomView::DPtoVP(double x, double y, int &X, int &Y)
{
	X = int((x - m_xStart)/blc) + m_ixStart;               //得到点的逻辑坐标横坐标
	if(m_MapMode == 1)						   //如果是MM_TEXT影象模式
		Y = m_hScreen - int((y - m_yStart)/blc) - m_iyStart;    //得到点的逻辑坐标纵坐标
	else                                       //如果是其它影象模式
		Y = int((y - m_yStart)/blc) - m_hScreen + m_iyStart;    //得到点的逻辑坐标纵坐标
}
void CZoomView::DPtoVP(double x, double y, long &X, long &Y)
{
	X = long((x - m_xStart)/blc) + m_ixStart;               //得到点的逻辑坐标横坐标
	if(m_MapMode == 1)						   //如果是MM_TEXT影象模式
		Y = m_hScreen - long((y - m_yStart)/blc) - m_iyStart;    //得到点的逻辑坐标纵坐标
	else                                       //如果是其它影象模式
		Y = long((y - m_yStart)/blc) - m_hScreen + m_iyStart;    //得到点的逻辑坐标纵坐标
}

void CZoomView::VPtoDP(long x, long y, double &X, double &Y)
{
	X = m_xStart + (x - m_ixStart) * blc;          //得到点的实际坐标横坐标
	if(m_MapMode == 1)						   //如果是MM_TEXT影象模式
		Y = m_yStart + blc*(m_hScreen - y - m_iyStart);    //得到点的实际坐标纵坐标
	else                                       //如果是其它影象模式
		Y = m_yStart + blc*(y + m_hScreen - m_iyStart);    //得到点的实际坐标纵坐标
}
void CZoomView::VPtoDP(int x, int y, double &X, double &Y)
{
	X = m_xStart + (x - m_ixStart) * blc;                      //得到点的实际坐标横坐标
	if(m_MapMode == 1)						   //如果是MM_TEXT影象模式
		Y = m_yStart + blc*(m_hScreen - y - m_iyStart);    //得到点的实际坐标纵坐标
	else                                       //如果是其它影象模式
		Y = m_yStart + blc*(y + m_hScreen - m_iyStart);    //得到点的实际坐标纵坐标
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
	switch(nSBCode)//判断按键的位置
	{
		case SB_TOP://如果将滚动条滚动到顶部
			nScrollInc = -nVScrollPos;//得到相对滚动范围
			break;
		case SB_BOTTOM://如果将滚动条滚动到底部
			nScrollInc = nVScrollMax - nVScrollPos;//得到相对滚动范围
			break;
		case SB_LINEUP://如果按中了滚动条中向上的按键
			nScrollInc = -nYLine;//得到相对滚动范围
			break;
		case SB_LINEDOWN://如果按中了滚动条中向下的按键
			nScrollInc = nYLine;//得到相对滚动范围
			break;
		case SB_PAGEUP://如果按中了中间活动按钮的上部区域
			nScrollInc = -nYPage;//得到相对滚动范围
			break;
		case SB_PAGEDOWN://如果按中了中间活动按钮的下部区域
			nScrollInc = nYPage;//得到相对滚动范围
			break;
		case SB_THUMBPOSITION://如果用鼠标拖动中间活动按钮到一个位置
			nScrollInc = nPos - nVScrollPos;//得到相对移动的最小滚动范围数
			break;
		default:
			nScrollInc = 0;//得到相对滚动范围
	}
	//进行滚动边界检查，得到实际的滚动位置（不能超出滚动条的滚动范围）
	nNewPos = max(0,min(nVScrollPos + nScrollInc,nVScrollMax));
	//得到实际的相对滚动范围
	nScrollInc = nNewPos - nVScrollPos;
	if(nScrollInc)//如果产生了滚动
	{
		nVScrollPos = nNewPos;//设定新的滚动位置
		SetScrollPos(SB_VERT,nVScrollPos);
		UpdateWindow();//更新滚动条
		//调整纵坐标使图形产生滚动
		m_yStart = m_yStart - blc*nScrollInc*nScrollMin;
		GetClientRect(&rr);//得到客户区的矩形边界
		if(abs(nScrollInc) * nScrollMin < rr.bottom)
		//滚动后的屏幕与滚动前有重叠
		{
			if(nScrollInc > 0)//如果是图形向上滚动，则滚动屏幕上重叠区域的矩形边界
				rr.top = nScrollInc*nScrollMin;
			else//如果图形向下滚动
				rr.bottom = rr.bottom + nScrollInc*nScrollMin;//得到重叠区域
			ScrollWindow(0,-nScrollInc*nScrollMin,rr);//滚动重叠的区域
			if(nScrollInc > 0)//如果是向上滚动
				rr.top = rr.bottom - nScrollInc*nScrollMin;//得到需要重画区域
			else//如果是向下滚动
				rr.bottom = -nScrollInc*nScrollMin;//得到需要重画的区域
			InvalidateRect(rr,1);//对图形进行局部重画
		}
		else//如果滚动后的区域与滚动前的区域没有重叠，则全屏重画
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
	if(nScrollInc)//如果产生了滚动
	{
		nHScrollPos = nNewPos;//设定新的滚动位置
		SetScrollPos(SB_HORZ,nHScrollPos);
		UpdateWindow();//改变滚动条的位置
		m_xStart = m_xStart + blc*nScrollInc*nScrollMin;//调整横坐标使图形产生滚动
		GetClientRect(&r1);//得到客户区的矩形边界
		if(abs(nScrollInc)*nScrollMin < r1.right)//如果滚动后的屏幕与滚动前的有重叠
		{
			if(nScrollInc > 0)//如果使图形向上滚动
				r1.left = nScrollInc*nScrollMin;//得到滚动屏幕上重叠区域的矩形
			else//如果图形向下滚动
				r1.right = r1.right - nScrollInc*nScrollMin;//得到重叠区域的矩形
			ScrollWindow(-nScrollInc*nScrollMin,0,r1);//滚动重叠的区域
			if(nScrollInc > 0)//如果是向上滚动
				r1.left = r1.right - nScrollInc*nScrollMin;//得到需要重画的区域
			else//如果是向下滚动
				r1.right = -nScrollInc*nScrollMin;//得到需要重画的区域
			InvalidateRect(r1,0);//对图形进行局部重画
		}
		else//如果滚动后的区域与滚动前的区域没有重叠，则全屏重画
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
	//如果当前屏幕记录在数组的最后一项，即数组没有空间再存信息
	{
		//将数组作堆式滚动，去掉第二屏（[0]中存的式首屏信息，操作时不改变）
		for(int i = 1; i < m_MaxScreen -1; i++)
			m_Screen[i] = m_Screen[i+1];
	}
	else //如果不是在数组的最后一项，屏幕记录号增加1
		m_CurrentScreen++;
	//记录下本屏幕的参数
	m_Screen[m_CurrentScreen].sx = StartX;
	m_Screen[m_CurrentScreen].sy = StartY;
	m_Screen[m_CurrentScreen].blc = blc;
}

void CZoomView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
		case VK_HOME://按Home下键时
			OnHScroll(SB_PAGEUP,0,NULL);
			break;
		case VK_END://按下End键时
			OnHScroll(SB_PAGEDOWN,0,NULL);
			break;
		case VK_PRIOR://按下PgUp键时
			OnVScroll(SB_PAGEUP,0,NULL);
			break;
		case VK_NEXT://按下PgDn键时
			OnVScroll(SB_PAGEDOWN,0,NULL);
			break;
		case VK_UP://按下上箭头
			OnVScroll(SB_LINEUP,0,NULL);
			break;
		case VK_DOWN://按下下箭头
			OnVScroll(SB_LINEDOWN,0,NULL);
			break;
		case VK_LEFT://按下左箭头
			OnHScroll(SB_LINEUP,0,NULL);
			break;
		case VK_RIGHT://按下右箭头
			OnHScroll(SB_LINEDOWN,0,NULL);
			break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CZoomView::InitHScroll()//设定横向滚动条
{
	//得到滚动条的最大范围
	nVScrollMax = int((double(m_hScreen)*m_Screen[0].blc)/(blc*nScrollMin));
	//得到当前的滚动条位置
	nVScrollPos = nVScrollMax - 
		int((m_yStart - m_Screen[0].sy)/(blc*nScrollMin));
	//设置滚动条的最大范围和当前位置
	SetScrollRange(SB_VERT,0,nVScrollMax,0);
	SetScrollPos(SB_VERT,nVScrollPos);///rrr.top = 0;
	UpdateWindow();
}

void CZoomView::InitVScroll()//设定纵向滚动条
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
	//得到区域与视图区域相交的矩形
	BOOL IsCross = RectCross(xx1,yy1,xx2,yy2,X1,Y1,X2,Y2);
	if(IsCross)//如果相交，则重画这一区域
	{
		//以下得到这个区域的逻辑坐标
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
		return FALSE;//如不相交，函数返回0
	else//两个矩形相交，得到相交矩形的坐标
	{
		x1 = max(x1,xx1);
		y1 = max(y1,yy1);
		x2 = min(x2,xx2);
		y2 = min(y2,yy2);
		return TRUE;
	}
}
