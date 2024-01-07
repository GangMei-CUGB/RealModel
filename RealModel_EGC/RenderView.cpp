// RenderView.cpp : implementation file
//

#include "stdafx.h"
#include "RealModel.h"
#include "RenderView.h"
#include "math.h"

#define PI 3.1415926535

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenderView

IMPLEMENT_DYNCREATE(CRenderView, CZoomView)

CRenderView::CRenderView()
{
//	m_nProjectionType = ORTHOGRAPHIC; 
	m_nProjectionType = PERSPECTIVE;
	m_bGlobal = true;
	m_pDC = NULL;
	int i = 0;
	for(i = 0; i < 3; i++) m_fLocalAmbient[i] = 0.5;
	
	for(i = 0; i < 3; i++)
	{
		m_lightAmb[i] = 0.0;
		m_lightDif[i] = 0.7f;
		m_lightSpe[i] = 1.0;
	}
    m_lightAmb[3] = 1.0;
    m_lightDif[3] = 1.0;
	m_lightSpe[3] = 1.0;
    m_spotExponent = 10.0;
    m_fLocalAmbient[4] = 1.0;
	m_dXrange = 1.0;   m_dYrange = 1.0;  m_dZrange = 1.0;
//	x_angle = -60.0;
//	y_angle = -10.0;
	x_angle = 0.0;
	y_angle = 0.0;
	
}

CRenderView::~CRenderView()
{
	int i, n;
	n = m_aList.GetSize() - 1;
	for(i = n; i >= 0; i--){delete m_aList.GetAt(i); m_aList.RemoveAt(i);} 
	m_aList.RemoveAll();
}


BEGIN_MESSAGE_MAP(CRenderView, CZoomView)
	//{{AFX_MSG_MAP(CRenderView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
//	ON_COMMAND(ID_IMAGE_COLOR, OnImageColor)//meigang 20071224
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
//	ON_COMMAND(ID_IMAGE_CAPTURE, &CRenderView::OnImageCapture)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenderView drawing

void CRenderView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRenderView diagnostics

#ifdef _DEBUG
void CRenderView::AssertValid() const
{
	CZoomView::AssertValid();
}

void CRenderView::Dump(CDumpContext& dc) const
{
	CZoomView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRenderView message handlers

void CRenderView::OnSize(UINT nType, int cx, int cy) 
{
	CZoomView::OnSize(nType, cx, cy);
	
	if(cy > 0){
		if((m_oldRect.right>cx) || (m_oldRect.bottom > cy))  RedrawWindow();
		m_oldRect.right = cx;
		m_oldRect.bottom = cy;

		glViewport(0, 0, cx, cy);
		glPushMatrix();
		   glMatrixMode(GL_PROJECTION);
		      glLoadIdentity();
		
			  if(m_nProjectionType == PERSPECTIVE) { 
				  GLdouble fAspect;
				  fAspect = (GLdouble)cx / (GLdouble)cy;
			//	  gluPerspective(m_dFOV, fAspect, 1.0, m_dZrange*3);
			//	  gluLookAt(0.1,0.4,0.1,0,0,-1,0,1,0);
				  glFrustum(-1.5, 1.5, -1.5, 1.5, 5.0, 10.0); //
				  glTranslatef(0, 0, -8.5f);
			  } 
			  else {
				  if(cx <= cy)
					  glOrtho(-m_dXrange*1.2, m_dXrange*1.2, -m_dYrange*cy/cx*1.2, m_dYrange*cy/cx*1.2, m_dZrange*3, -3*m_dZrange);
				  else
					  glOrtho(-m_dXrange*cx/cy*1.2, m_dXrange*cx/cy*1.2, -m_dYrange*1.2, m_dYrange*1.2, 3.0*m_dZrange, -3*m_dZrange);
			  }

		   glMatrixMode(GL_MODELVIEW);
	    glPopMatrix();
	}	

}

void CRenderView::OnDestroy() 
{
	CZoomView::OnDestroy();
	
	HGLRC hrc;

	hrc = ::wglGetCurrentContext();

    ::wglMakeCurrent(NULL,  NULL);
	
    if(hrc)
        ::wglDeleteContext(hrc);

    if(m_pDC)
        delete m_pDC;	
}

BOOL CRenderView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |=WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	
	return CZoomView::PreCreateWindow(cs);
}

void CRenderView::Init()
{
	PIXELFORMATDESCRIPTOR pfd;
    int         n;
	HGLRC		hrc;
//	GLfloat     fMaxObjSize, fAspect;   //new
//	GLfloat     fNearPlane, fFarPlane;  //new

    m_pDC = new CClientDC(this);

    ASSERT(m_pDC != NULL);

    if (!bSetupPixelFormat())
        return;

    n =::GetPixelFormat(m_pDC->GetSafeHdc());
    ::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

    hrc = wglCreateContext(m_pDC->GetSafeHdc());
    wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

    GetClientRect(&m_oldRect);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//    glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
}

void CRenderView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bOpenGL){
		if(GetCapture() == this) //检查是否捕捉
		{
			x_angle += double(point.y - MouseDownPoint.y)/3.6; //旋转对象一个步长
			y_angle += double(point.x - MouseDownPoint.x)/3.6;
			Invalidate(true);
			MouseDownPoint = point;
		}
	}
	CZoomView::OnMouseMove(nFlags, point);
}

void CRenderView::Render()
{
//	glTranslatef(0.0f, 0.0f, 2.5);
	glRotated(x_angle, 1.0, 0.0, 0.0);
	glRotated(y_angle, 0.0, 1.0, 0.0);
//	glRotated(y_angle+x_angle, 0.0, 0.0, 1.0);
	DrawAxis();
    if(m_bGlobal) glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_fLocalAmbient);	
    CreateRenderLight();
}

BOOL CRenderView::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
          PFD_SUPPORT_OPENGL|         // support OpenGL
		PFD_DOUBLEBUFFER|              //double buffered   
		PFD_STEREO_DONTCARE,
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        32,                             // 32-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    int pixelformat;

    if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
    {
        MessageBox(_T("ChoosePixelFormat failed"));
        return FALSE;
    }

    if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
    {
        MessageBox(_T("SetPixelFormat failed"));
        return FALSE;
    }
    if(pfd.dwFlags & PFD_NEED_PALETTE)
		SetLogicalPalette();	//设置逻辑调色板
    return TRUE;
}

int CRenderView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CZoomView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	Init();
	
	return 0;
}

void CRenderView::CreateSun(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
//	GLfloat light_specular[] = {1.0, 1.0, 0.2, 1.0};
	GLfloat light_position[] = {x, y, z, 0.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
}

void CRenderView::CreateSpotlight(GLfloat *position, GLfloat *direction, GLfloat cutoff)
{
	glLightfv(GL_LIGHT1, GL_AMBIENT, m_lightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, m_lightDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, m_lightSpe);
	glLightfv(GL_LIGHT1, GL_POSITION, position);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, m_spotExponent);

    glEnable(GL_LIGHT1);
}

void CRenderView::AccumlateScene(int j, int num)
{
	GLdouble dOffsetX[16], dOffsetY[16];
	GLdouble dTransRef[16][2]=
	{
		{0.375, 0.4375}, {0.625, 0.0625}, {0.875, 0.1875}, {0.125, 0.0625},
		{0.375, 0.6875}, {0.875, 0.4375}, {0.625, 0.5625}, {0.375, 0.9375},
		{0.625, 0.3125}, {0.125, 0.5625}, {0.125, 0.8125}, {0.375, 0.1875},
		{0.875, 0.9375}, {0.875, 0.6875}, {0.125, 0.3125}, {0.625, 0.8125}
	};
	GLint i, viewport[4];
	GLfloat lightPos[] = {0.0f, 3.0f, 2.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glGetIntegerv(GL_VIEWPORT, viewport);

	for(i = 0; i < 16; i++)
	{
		dOffsetX[i] = dTransRef[i][0] * 10/viewport[2];
		dOffsetY[i] = dTransRef[i][1] * 10/viewport[3];
	}
	glShadeModel(GL_FLAT);
	glClearAccum(0.0, 0.0, 0.0, 0.0);
	glClear(GL_ACCUM_BUFFER_BIT);
	for(i = 0; i < 8; i++)
	{
		glPushMatrix();
		     glTranslated(dOffsetX[i], dOffsetY[i], 0.0);
			 glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			 glCallList(2); ///???
		glPopMatrix();
		glAccum(GL_ACCUM, 1.0/8.0);
		glFlush();
	//	SwapBuffers(wglGetCurrentDC());
	}
    glAccum(GL_RETURN, 1.0);
	glShadeModel(GL_SMOOTH);
}

void CRenderView::CreateFixedSpotlight()
{
	GLfloat Amb[4], DifSpe[4], position[4], direction[3];
	for(int i = 0; i < 4; i++) Amb[i] = 0.5;     //Amb[3] = 1.0;
	DifSpe[0] = 0.9f;   DifSpe[1] = 0.9f;  DifSpe[2] = 0.5f;    DifSpe[3] = 1.0f;//淡黄
	position[0] = 0;   position[1] = 0.0;  position[2] = 1.0;  position[3] = 1.0;
    direction[0] = 0;  direction[1] = 0.0; direction[2] = -1.0;

	glLightfv(GL_LIGHT2, GL_AMBIENT, Amb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, DifSpe);
	glLightfv(GL_LIGHT2, GL_SPECULAR, DifSpe);
	glLightfv(GL_LIGHT2, GL_POSITION, position);

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90.0);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1);
    glEnable(GL_LIGHT2);
}

void CRenderView::ChangeGlobalAmbient(GLfloat *amb, bool flag)
{
	for(int i = 0; i < 3; i++) m_fLocalAmbient[i] = amb[i];
	m_bGlobal = flag;
}

void CRenderView::CreateMaterial(long clr)
{
	int r, g, b;
	double fParaR, fParaG, fParaB;
	float AmbDif[4], Spe[4]; 
	
	r = int(clr / 1000000); 
	g = int((clr - 1000000 * r) / 1000);
	b = int(clr - 1000000 * r - 1000 * g);
    
	fParaR = r / 255.0;  fParaG = g / 255.0;  fParaB = b / 255.0;

	//R
	int k = 0;
	AmbDif[k] =  float(fParaR);
	Spe[k] =  float(fParaR + 0.05);

	//G
	k = 1;
	AmbDif[k] =  float(fParaG);
	Spe[k] =  float(fParaG + 0.05);

	//蓝
	k = 2;
	AmbDif[k] =  float(fParaB);
	Spe[k] =  float(fParaB + 0.05);

	AmbDif[3] = 1.0f;
	Spe[3] = 1.0f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, AmbDif);//散射光和环境光相同
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Spe);

	glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 100);
}

void CRenderView::ChangeSpotlightFactor(GLfloat *amb, GLfloat *dif, GLfloat *spe)
{
	for(int i = 0; i < 3; i++)
	{
		m_lightAmb[i] = amb[i];
		m_lightDif[i] = dif[i];
		m_lightSpe[i] = spe[i];
	}
}
/////////////////////////////////////////////////////////////////////
//	                  设置逻辑调色板
//////////////////////////////////////////////////////////////////////
void CRenderView::SetLogicalPalette()
{
	struct
    {
        WORD Version;
        WORD NumberOfEntries;
        PALETTEENTRY aEntries[256];
    } logicalPalette = { 0x300, 256 };

	BYTE reds[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE greens[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE blues[] = {0, 85, 170, 255};

    for (int colorNum=0; colorNum<256; ++colorNum)
    {
        logicalPalette.aEntries[colorNum].peRed =
            reds[colorNum & 0x07];
        logicalPalette.aEntries[colorNum].peGreen =
            greens[(colorNum >> 0x03) & 0x07];
        logicalPalette.aEntries[colorNum].peBlue =
            blues[(colorNum >> 0x06) & 0x03];
        logicalPalette.aEntries[colorNum].peFlags = 0;
    }

    m_hPalette = CreatePalette ((LOGPALETTE*)&logicalPalette);
}
//计算视野（用于透视里的fovy参数）size为将场景围住的立方体盒内接球半径
//distance为其球心到视点的距离
double CRenderView::CalculateFovy(double size, double distance)
{
	double radtheta, degtheta;
	radtheta = 2.0 * atan2(size, distance);
	degtheta = (180.0*radtheta)/PI;
	return degtheta;
}

void CRenderView::CalculateModelCenter( CCordnt* crt)
{
	m_pModelCenter[0] = float((crt->maxX - crt->minX)*crt->Scale / 2);
    m_pModelCenter[1] = float((crt->maxY - crt->minY)*crt->Scale / 2);
	m_pModelCenter[2] = float((crt->maxZ - crt->minZ)*crt->Scale / 2);
}

void CRenderView::CreateRenderLight()
{
	GLfloat direct[3];
	GLfloat psi[4];
	psi[0] = -1.0;  psi[1] = -1.0; psi[2] = 10.0; psi[3] =1.0;
    for(int i = 0; i < 3; i++) direct[i] = -psi[i];
	CreateSpotlight(psi, direct, 45.0);
	CreateFixedSpotlight();
}

void CRenderView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bOpenGL){
		MouseDownPoint = point;    //记录鼠标的位置
	    SetCapture();             //捕捉鼠标的位置
	}
	else{
	}
	CZoomView::OnLButtonDown(nFlags, point);
}

void CRenderView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bOpenGL){
		MouseDownPoint = CPoint(0, 0);  //鼠标按下位置置于原点
	    ReleaseCapture();               //解除鼠标的位置
	}
	CZoomView::OnLButtonUp(nFlags, point);
}

void CRenderView::DrawAxis()
{
//	glViewport(0, 0, 80,80);
//	glRotated(x_angle, 1.0, 0.0, 0.0);
//	glRotated(y_angle, 0.0, 1.0, 0.0);
	glPushMatrix();
	// 绘制三个坐标轴
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		// 绘制红色的x轴
		glColor3f(1.f,0.f,0.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.2f,0.0f,0.0f);
		glVertex3f(0.2f,0.0f,0.0f);
		glVertex3f(0.18f,0.01f,0.0f);
		glVertex3f(0.2f,0.0f,0.0f);
		glVertex3f(0.18f,-0.01f,0.0f);
		// 绘制兰色的y轴
		glColor3f(0.f,0.f,1.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.2f,0.0f);
		glVertex3f(0.0f,0.2f,0.0f);
		glVertex3f(0.01f,0.18f,0.0f);
		glVertex3f(0.0f,0.2f,0.0f);
		glVertex3f(-0.01f,0.18f,0.0f);
		// 绘制绿色的z轴
		glColor3f(0.f,1.f,0.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,0.2f);
		glVertex3f(0.0f,0.0f,0.2f);
		glVertex3f(0.0f,0.01f,0.18f);
		glVertex3f(0.0f,0.0f,0.2f);
		glVertex3f(0.0f,-0.01f,0.18f);
	glEnd();
	glEnable(GL_LIGHTING);
    glPopMatrix();
	
//	glPopMatrix();
}

void CRenderView::OnImageColor() 
{
	COLORREF col;
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK){ col = dlg.GetColor(); }
//	glClearColor(GetRValue(col)/255.0f, GetGValue(col)/255.0f, GetBValue(col)/255.0f, 1.0);
    //可以用来改变材质的颜色
	m_fColor[0] = GetRValue(col)/255.0f;  //red
	m_fColor[1] = GetGValue(col)/255.0f;  //green
	m_fColor[2] = GetBValue(col)/255.0f;  //blue	
}

BOOL CRenderView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bOpenGL) return true;
	else return CZoomView::OnEraseBkgnd(pDC);
}

void CRenderView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default	
	CZoomView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRenderView::DrawPoint(CDC *pDC, CPoint Point, COLORREF color)
{
	CBrush brushRectPoint;
	CPen penPoint;
	brushRectPoint.CreateSolidBrush(color);
	penPoint.CreatePen(PS_SOLID,0,color);
	CRect rectPoint;
    
	long ix = Point.x;  long iy = Point.y;
	rectPoint.bottom = int(iy - 2);
	rectPoint.right = int(ix + 2);
	rectPoint.top = int(iy + 2);
	rectPoint.left = int(ix - 2);
	CBrush * pOldBrush = pDC->SelectObject(&brushRectPoint);
	CPen * pOldPen = pDC->SelectObject(&penPoint);
	pDC->Rectangle(rectPoint);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void CRenderView::DrawPoint(CDC *pDC, double x, double y, COLORREF color)
{
	CBrush brushRectPoint;
	CPen penPoint;
	brushRectPoint.CreateSolidBrush(color);
	penPoint.CreatePen(PS_SOLID,0,color);
	CRect rectPoint;

	int ix, iy;
    DPtoVP(x, y, ix, iy);

	rectPoint.bottom = int(iy - 2);
	rectPoint.right = int(ix + 2);
	rectPoint.top = int(iy + 2);
	rectPoint.left = int(ix - 2);
	CBrush * pOldBrush = pDC->SelectObject(&brushRectPoint);
	CPen * pOldPen = pDC->SelectObject(&penPoint);
	pDC->Rectangle(rectPoint);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void CRenderView::DrawLineTo(CDC *pDC, CPoint point, COLORREF color)
{
	CPen newpen(PS_SOLID,1,color);
	CPen* old = pDC->SelectObject(&newpen);
	pDC->SetROP2(R2_NOT); 
	pDC->MoveTo(ptOrigin);
	pDC->LineTo(ptPrevious);
	pDC->MoveTo(ptOrigin);
	ptCurrent = point;
	pDC->LineTo(point);
    ptPrevious = point;

	pDC->SelectObject(old);
}

void CRenderView::CalculateNormal(GLdouble *fVertex1, GLdouble *fVertex2, GLdouble *fVertex3, GLdouble *fNormal)
{
	GLdouble fVector1[3], fVector2[3];
	fVector1[0]=fVertex2[0]-fVertex1[0];
	fVector1[1]=fVertex2[1]-fVertex1[1];
	fVector1[2]=fVertex2[2]-fVertex1[2];
	fVector2[0]=fVertex3[0]-fVertex1[0];
	fVector2[1]=fVertex3[1]-fVertex1[1];
	fVector2[2]=fVertex3[2]-fVertex1[2];

	fNormal[0]=fVector1[1]*fVector2[2]-fVector1[2]*fVector2[1];
	fNormal[1]=fVector1[2]*fVector2[0]-fVector1[0]*fVector2[2];
	fNormal[2]=fVector1[0]*fVector2[1]-fVector1[1]*fVector2[0];

	double fNormalLength=sqrt(fNormal[0]*fNormal[0]+fNormal[1]*fNormal[1]+fNormal[2]*fNormal[2]);
	if(fNormalLength!=0.0)
	{
		fNormal[0]=fNormal[0]/fNormalLength;
		fNormal[1]=fNormal[1]/fNormalLength;
		fNormal[2]=fNormal[2]/fNormalLength;
	}
	else
	{
		fNormal[0]=0.0;
		fNormal[1]=0.0;
		fNormal[2]=1.0;
	}
}

void CRenderView::CalculateNormalEGC(GLfloat *fVertex1, GLfloat *fVertex2, GLfloat *fVertex3, GLfloat *fNormal)
{
	GLfloat fVector1[3], fVector2[3];
	fVector1[0]=fVertex2[0]-fVertex1[0];
	fVector1[1]=fVertex2[1]-fVertex1[1];
	fVector1[2]=fVertex2[2]-fVertex1[2];
	fVector2[0]=fVertex3[0]-fVertex1[0];
	fVector2[1]=fVertex3[1]-fVertex1[1];
	fVector2[2]=fVertex3[2]-fVertex1[2];

	fNormal[0]=fVector1[1]*fVector2[2]-fVector1[2]*fVector2[1];
	fNormal[1]=fVector1[2]*fVector2[0]-fVector1[0]*fVector2[2];
	fNormal[2]=fVector1[0]*fVector2[1]-fVector1[1]*fVector2[0];

	float fNormalLength=sqrt(fNormal[0]*fNormal[0]+fNormal[1]*fNormal[1]+fNormal[2]*fNormal[2]);
	if(fNormalLength!=0.0)
	{
		fNormal[0]=fNormal[0]/fNormalLength;
		fNormal[1]=fNormal[1]/fNormalLength;
		fNormal[2]=fNormal[2]/fNormalLength;
	}
	else
	{
		fNormal[0]=0.0;
		fNormal[1]=0.0;
		fNormal[2]=1.0;
	}
}


long CRenderView::SetNewList()
{
	
	if(m_aList.GetSize() > 0){
		int i = m_aList.GetSize() - 1; 		
  	    return m_aList.GetAt(i)->ik + 1;
	}
	else return 1000;
	
}
void CRenderView::DrawList()
{
//	CRect rt;
//	GetClientRect(&rt);
//	glViewport(0, 0, rt.Width(), rt.Height());

//	glRotated(x_angle, 1.0, 0.0, 0.0);
//	glRotated(y_angle, 0.0, 1.0, 0.0);
	for(int i = 0; i < m_aList.GetSize(); i++){
		CreateMaterial(m_aList.GetAt(i)->cj); 
		glCallList(m_aList.GetAt(i)->ik);
	}
}

