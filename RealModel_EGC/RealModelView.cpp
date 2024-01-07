
// RealModelView.cpp : implementation of the CRealModelView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RealModel.h"
#endif

// RAPID package 
#include "RAPID.h"
// #pragma comment(lib, "RAPID.lib")

#include <fstream>
#include "math.h"

#include "RealModelDoc.h"
#include "RealModelView.h"

using namespace std;   //加上这句就行了

#define PI 3.1415926
#define MAX 1000000000
#define MIN -1000000000
#define RATE 0.5

// #define CIGMA 0.000000000001
// #define fCIGMA 0.00001
// #define sqrEpsilon 0.000000000001
// #define Epsilon 0.000001
// #define myEps 0.000001
#define CIGMA 0.000000001
#define fCIGMA 0.00001
#define sqrEpsilon 0.000000001


//* Valid for Cutting
//#define Epsilon 0.00001
//#define myEps 0.00001

// Valid for Intersecting
#define Epsilon 0.0001
#define myEps 0.0001 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UP_BLOCK

// CRealModelView

IMPLEMENT_DYNCREATE(CRealModelView, CRenderView)

BEGIN_MESSAGE_MAP(CRealModelView, CRenderView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRealModelView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_COMMAND(ID_IMAGE_ZOOM, &CRealModelView::OnImageZoom)
	ON_COMMAND(ID_IMAGE_PAN, &CRealModelView::OnImagePan)
	ON_COMMAND(ID_IMAGE_ROTATE, &CRealModelView::OnImageRotate)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_IMAGE_CAPTURE, &CRealModelView::OnImageCapture)
	ON_COMMAND(ID_3D_MODEL_OPEN_SURFACE, &CRealModelView::On3DModelOpenSurface)
	ON_COMMAND(ID_3D_MODEL_INTERSECT, &CRealModelView::On3DModelIntersect)
	ON_COMMAND(ID_2D_MODEL_OPEN_SURFACE, &CRealModelView::On2DModelOpenSurface)
	ON_COMMAND(ID_2D_MODEL_INTERSECT, &CRealModelView::On2DModelIntersect)
	ON_COMMAND(ID_3D_MODEL_OPEN_BLOCK_GROUP, &CRealModelView::On3DModelOpenBlockGroup)
	ON_COMMAND(ID_3D_MODEL_ADD_SURFACE, &CRealModelView::On3DModelAddSurface)
	ON_COMMAND(ID_3D_MODEL_INTERSECT_AND_CUT, &CRealModelView::On3DModelIntersectAndCut)
	ON_COMMAND(ID_3D_MODEL_SAVE_BLOCK_GROUP, &CRealModelView::On3DModelSaveBlockGroup)
	ON_WM_TIMER()
	ON_COMMAND(ID_IMAGE_ZOOM_IN, &CRealModelView::OnImageZoomIn)
	ON_COMMAND(ID_IMAGE_ZOOM_OUT, &CRealModelView::OnImageZoomOut)
	ON_COMMAND(ID_IMAGE_ZOOM_FIT, &CRealModelView::OnImageZoomFit)
	ON_COMMAND(ID_IMAGE_ZOOM_ORIGINAL, &CRealModelView::OnImageZoomOriginal)
	ON_COMMAND(ID_IMAGE_INITIALIZE, &CRealModelView::OnImageInitialize)
	ON_COMMAND(ID_INTERSECT_CUT_ADD_SURFACE, &CRealModelView::OnAddSurface)
	ON_COMMAND(ID_INTERSECT_CUT_GET_COORDINATE_SYSTEM, &CRealModelView::OnGetCoordinateSystem)
	ON_COMMAND(ID_INTERSECT_CUT_INTERSECT_AND_CUT, &CRealModelView::OnIntersectAndCut)
	ON_COMMAND(ID_INTERSECT_CUT_OPEN_BLOCK_GROUP, &CRealModelView::OnOpenBlockGroup)
	ON_COMMAND(ID_INTERSECT_CUT_SAVE_BLOCK_GROUP, &CRealModelView::OnSaveBlockGroup)
	ON_COMMAND(ID_INTERSECT_CUT_SET_COORDINATE_SYSTEM, &CRealModelView::OnSetCoordinateSystem)
	ON_COMMAND(ID_POSITION_FRONTVIEW, &CRealModelView::OnPositionFrontView)
	ON_COMMAND(ID_POSITION_BACKVIEW, &CRealModelView::OnPositionBackView)
	ON_COMMAND(ID_POSITION_LEFTVIEW, &CRealModelView::OnPositionLeftView)
	ON_COMMAND(ID_POSITION_RIGHTVIEW, &CRealModelView::OnPositionRightView)
	ON_COMMAND(ID_POSITION_TOPVIEW, &CRealModelView::OnPositionTopView)
	ON_COMMAND(ID_POSITION_BOTTOMVIEW, &CRealModelView::OnPositionBottomView)
	ON_COMMAND(ID_AXIS_XLEFT, &CRealModelView::OnAxisXLeft)
	ON_COMMAND(ID_AXIS_XRIGHT, &CRealModelView::OnAxisXRight)
	ON_COMMAND(ID_AXIS_YLEFT, &CRealModelView::OnAxisYLeft)
	ON_COMMAND(ID_AXIS_YRIGHT, &CRealModelView::OnAxisYRight)
	ON_COMMAND(ID_AXIS_ZLEFT, &CRealModelView::OnAxisZLeft)
	ON_COMMAND(ID_AXIS_ZRIGHT, &CRealModelView::OnAxisZRight)
	ON_COMMAND(ID_GET_COORD_SYSTEM, &CRealModelView::OnGetCoordSystem)
	ON_COMMAND(ID_SET_COORD_SYSTEM, &CRealModelView::OnSetCoordSystem)
	ON_COMMAND(ID_IMAGE_PAN_LEFT, &CRealModelView::OnMoveLeft)
	ON_COMMAND(ID_IMAGE_PAN_UP, &CRealModelView::OnMoveUp)
	ON_COMMAND(ID_IMAGE_PAN_RIGHT, &CRealModelView::OnMoveRight)
	ON_COMMAND(ID_IMAGE_PAN_DOWN, &CRealModelView::OnMoveDown)
	ON_COMMAND(ID_2D_MODEL_INTERSECT_SLOW, &CRealModelView::On2DModelIntersectSlow)
	ON_COMMAND(ID_IMAGE_PREVIOUS_VIEW, &CRealModelView::OnImagePreviousView)
	ON_COMMAND(ID_IMAGE_NEXT_VIEW, &CRealModelView::OnImageNextView)
	ON_COMMAND(ID_DISPLAY_TRGL_SURFACE, &CRealModelView::OnDisplayTrglSurface)
	ON_COMMAND(ID_DISPLAY_QUAD_SURFACE, &CRealModelView::OnDisplayQuadSurface)
	ON_COMMAND(ID_DISPLAY_POLY_SURFACE, &CRealModelView::OnDisplayPolySurface)
	ON_COMMAND(ID_DISPLAY_POLYHEDRON, &CRealModelView::OnDisplayPolyhedron)
	ON_COMMAND(ID_DISPLAY_POINT_VIEW, &CRealModelView::OnDisplayPointView)
	ON_COMMAND(ID_DISPLAY_LINE_VIEW, &CRealModelView::OnDisplayLineView)
	ON_COMMAND(ID_DISPLAY_AREA_VIEW, &CRealModelView::OnDisplayAreaView)
	ON_COMMAND(ID_DISPLAY_SURFACE_VIEW, &CRealModelView::OnDisplaySurfaceView)
	ON_COMMAND(ID_DISPLAY_POLYHEDRA_VIEW, &CRealModelView::OnDisplayPolyhedraView)
	ON_COMMAND(ID_DISPLAY_BLOCK_VIEW, &CRealModelView::OnDisplayBlockView)
	ON_COMMAND(ID_DEBUG_CUT_UPDATE_SURFACE, &CRealModelView::OnDebugCutUpdateSurface)
	ON_COMMAND(ID_DEBUG_ONLY_MAPPING, &CRealModelView::OnDebugOnlyMapping)
	ON_COMMAND(ID_DEBUG_INTERSECT_FLOAT, &CRealModelView::OnDebugIntersectFloat)
	ON_COMMAND(ID_SELECT_ENTITIES, &CRealModelView::OnSelectEntities)
	ON_COMMAND(ID_SELECT_EVERYTHING, &CRealModelView::OnSelectEverything)
	ON_COMMAND(ID_SETTINGS_PROP_DEF, &CRealModelView::OnSettingsPropDef)
	ON_COMMAND(ID_SETTINGS_PROP_SET, &CRealModelView::OnSettingsPropSet)
	ON_COMMAND(ID_DEBUG_READ_DATA_PROP, &CRealModelView::OnDebugReadDataProp)
	ON_COMMAND(ID_SETTINGS_SAVE_PROP, &CRealModelView::OnSettingsSaveProp)
	ON_COMMAND(ID_SETTINGS_LOAD_PROP, &CRealModelView::OnSettingsLoadProp)
	ON_COMMAND(ID_3D_MODEL_OPEN_BLOCK_GROUP_PROP, &CRealModelView::On3DModelOpenBlockGroupProp)
	ON_COMMAND(ID_3D_MODEL_SAVE_BLOCK_GROUP_PROP, &CRealModelView::On3DModelSaveBlockGroupProp)
	ON_COMMAND(ID_3D_MODEL_OPEN_SURFACE_OLD, &CRealModelView::On3DModelOpenSurfaceOld)
	ON_COMMAND(ID_MESH_IMPORT_OFF, &CRealModelView::OnMeshImportOff)
	ON_COMMAND(ID_DEBUG_SURF_CUT_BLOCK, &CRealModelView::OnDebugSurfCutBlock)
	ON_COMMAND(ID_INTERPOLATE_KRIGING_ORDINARY, &CRealModelView::OnInterpolateKrigingOrdinary)
	ON_COMMAND(ID_INTERPOLATE_KRIGING_UNIVERSAL, &CRealModelView::OnInterpolateKrigingUniversal)
	ON_COMMAND(ID_INTERPOLATE_DSI, &CRealModelView::OnInterpolateDSI)
	ON_COMMAND(ID_INTERPOLATE_IDW, &CRealModelView::OnInterpolateIDW)
	ON_COMMAND(ID_TOOLS_BY_ROTATE_X, &CRealModelView::OnToolsByRotateX)
	ON_COMMAND(ID_TOOLS_BY_ROTATE_Y, &CRealModelView::OnToolsByRotateY)
	ON_COMMAND(ID_TOOLS_BY_ROTATE_Z, &CRealModelView::OnToolsByRotateZ)
	ON_COMMAND(ID_TOOLS_BY_MOVE, &CRealModelView::OnToolsByMove)
	ON_COMMAND(ID_TOOLS_BY_SCALE, &CRealModelView::OnToolsByScale)
	ON_COMMAND(ID_DEBUG_ANTI_ALISING, &CRealModelView::OnDebugAntiAlising)
	ON_COMMAND(ID_DEBUG_GET_TOP_SURFACE, &CRealModelView::OnDebugGetTopSurface)
	ON_COMMAND(ID_TOOLS_OFFSET_Z, &CRealModelView::OnToolsOffsetZ)
	ON_COMMAND(ID_3D_MODEL_GET_TOP_SURFACE, &CRealModelView::On3DModelGetTopSurface)
	ON_COMMAND(ID_3D_MODEL_COMBINE_BLOCK_GROUP, &CRealModelView::On3DModelCombineBlockGroup)
	ON_COMMAND(ID_3D_MODEL_BLOCK_GROUP_TO_ANSYS, &CRealModelView::On3DModelBlockGroupToAnsys)
	ON_COMMAND(ID_TOOLS_GET_BOTTOM_SURFACE, &CRealModelView::OnToolsGetBottomSurface)
	ON_COMMAND(ID_2D_MODEL_MAPPING, &CRealModelView::On2DModelMapping)
	ON_COMMAND(ID_2D_MODEL_OPEN_POLY_SURFACE, &CRealModelView::On2DModelOpenPolySurface)
	ON_COMMAND(ID_DEBUG_TRIANGULATE_POLYGON_SURFACE, &CRealModelView::OnDebugTriangulatePolygonSurface)
	ON_COMMAND(ID_2D_MODEL_MAPPING_COMPLEX, &CRealModelView::On2DModelMappingComplex)
	ON_COMMAND(ID_CONVERT_TRGL_TO_POLY, &CRealModelView::OnConvertTrglToPoly)
	ON_COMMAND(ID_TOOLS_CHECK_CCW, &CRealModelView::OnToolsCheckCCW)
	ON_COMMAND(ID_CONVERT_QUAD_TO_ANSYS, &CRealModelView::OnConvertQuadToAnsys)
	ON_COMMAND(ID_CONVERT_TRGL_TO_ANSYS, &CRealModelView::OnConvertTrglToAnsys)
	ON_COMMAND(ID_CONVERT_POLY_TO_ANSYS, &CRealModelView::OnConvertPolyToAnsys)
	ON_COMMAND(ID_CONVERT_BLOCK_TO_ANSYS, &CRealModelView::OnConvertBlockToAnsys)
	ON_COMMAND(ID_TOOLS_CHECK_SINGLE_VALUE_MESH, &CRealModelView::OnToolsCheckSingleValueMesh)
	ON_COMMAND(ID_TOOLS_SELECT_A_POLYHEDRON, &CRealModelView::OnToolsSelectAPolyhedron)
	ON_COMMAND(ID_TOOLS_COMBINE_POLY_SURF, &CRealModelView::OnToolsCombinePolySurf)
	ON_COMMAND(ID_TOOLS_COMBINE_TRGL_SURF, &CRealModelView::OnToolsCombineTrglSurf)
	ON_COMMAND(ID_INTERPOLATE_IDW_BY_SURFACE, &CRealModelView::OnInterpolateIdwBySurface)
	ON_COMMAND(ID_INTERPOLATE_IDW_BY_POINTS, &CRealModelView::OnInterpolateIdwByPoints)
	ON_COMMAND(ID_DISPLAY_BLOCK_GROUP, &CRealModelView::OnDisplayBlockGroup)
	END_MESSAGE_MAP()

// CRealModelView construction/destruction

CRealModelView::CRealModelView()
{
	// TODO: add construction code here
	pSurf = NULL; pCrt = NULL;  pBlk = NULL; 
	pFrame = NULL;


	int i;
	for(i = 0; i < 3; i++) {	
		m_fRotate[i] = 0.0; 
		m_fTranslate[i] = 0.0; 
	}
	for(i = 0; i < 4; i++){	
		m_fScale[i] = 1.0;	
		m_lightAmb[i] = 0.0; 
		m_lightDif[i] = 1.0;
		m_lightSpe[i] = 1.0;
		m_lightPos[i] = 0.5;
		m_materialAmb[i] = 0.0; 
		m_materialDif[i] = 0.0;
		m_materialSpe[i] = 0.0;		
	}
	m_lightAmb[3] = 1.0; 
	m_bOpenGL = true;

	m_iBound = -1;
	m_iPushnum = 0; 
	m_nDrawType = -1;
	m_iOBColor = 255000000;
	m_sWorkSpace = "D:\\xiaowan";
	m_iMeshScale = 1500;
	m_bMouseinput = false;
	m_iSelectMousePoint = -1;
	m_hvDraw = 0;
	m_nDrawState = 0;
	//m_iMesh = false;

	//几何工具
	m_operator = OPERATOR_VIEW_ROTATE;
	m_uShowMode = SHOW_MODE_LINE;

	ScenePos.x=0.0;
	ScenePos.y=0.0;
	ScenePos.z=0.0;
	
	SceneAngle.x=0.0;
	SceneAngle.y=0.0;
	SceneAngle.z=0.0;

	SceneZoom = 1.25;
	SceneDepth = 300.0;

	m_iView = 0;

	/*
	// for select
	glMode = GL_RENDER; // the render mode is GL_RENDER in default
	bPicked = false;
	nPicked = -1;
	nPrePicked = -1;
	nCurPicked = -1;
	*/
	
	// For Antialising
	m_nAntialising = DISPLAY_ANTIALISING_ON;

}

CRealModelView::~CRealModelView()
{
}

BOOL CRealModelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// return CView::PreCreateWindow(cs);
	return CRenderView::PreCreateWindow(cs);
}

// CRealModelView drawing

void CRealModelView::OnDraw(CDC* /*pDC*/)
{
	CRealModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
		
	if(m_bOpenGL){
	//	RenderScene();
		DrawScene();
	} 
}


// CRealModelView printing


void CRealModelView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRealModelView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRealModelView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRealModelView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}



void CRealModelView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRealModelView diagnostics

#ifdef _DEBUG
void CRealModelView::AssertValid() const
{
	CView::AssertValid();
}

void CRealModelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRealModelDoc* CRealModelView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRealModelDoc)));
	return (CRealModelDoc*)m_pDocument;
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////////////////////
// CRealModelView message handlers

void CRealModelView::DrawScene()
{
	// Clear
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ); // black backgroud
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //////////////////////////////////// SetViewMode();
	m_dAspectRatio = double ( m_ClientRect.right ) / double ( m_ClientRect.bottom );
	// setup viewport transformation
	glViewport( 0, 0, m_ClientRect.right, m_ClientRect.bottom);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	if( m_ClientRect.right <= m_ClientRect.bottom )
		glOrtho(-1.0*SceneZoom,SceneZoom,-1.0*SceneZoom*m_ClientRect.bottom/m_ClientRect.right,SceneZoom*m_ClientRect.bottom/m_ClientRect.right,-1.0*SceneDepth,SceneDepth);
	else
		glOrtho(-1.0*SceneZoom*m_ClientRect.right/m_ClientRect.bottom,SceneZoom*m_ClientRect.right/m_ClientRect.bottom,-1.0*SceneZoom,SceneZoom,-1.0*SceneDepth,SceneDepth);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-90,1.0f,0.0f,0.0f); // important!
	// rotate with a given angle
	glTranslatef(ScenePos.x,ScenePos.y,ScenePos.z);
	glRotated(SceneAngle.x,1.0,0.0,0.0);
	glRotated(SceneAngle.y,0.0,1.0,0.0);
	glRotated(SceneAngle.z,0.0,0.0,1.0);
	//////////////////////////////////// SetViewMode();

	glColorMaterial(GL_FRONT,GL_DIFFUSE); // can be deleted!
	glEnable(GL_COLOR_MATERIAL); // can be deleted!
   
//	glMatrixMode(GL_MODELVIEW); // Do not need to modifiy at this time!
//	glLoadIdentity();
	glPushMatrix();

    //////////////////////////////////// Render();
	glRotated(x_angle, 1.0, 0.0, 0.0);
	glRotated(y_angle, 0.0, 1.0, 0.0);

//	DrawAxis();

    if(m_bGlobal) glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_fLocalAmbient);	
    CreateRenderLight();
	//////////////////////////////////// Render();

    glEnable(GL_LIGHTING);
	int numOfList = 100;
	for(GLint i = 1; i < numOfList; i++) {
		if( glIsList(i) == GL_TRUE )  glCallList(i);
		else  break;
	}
	glPopMatrix();

	glFinish();
	::SwapBuffers(m_pDC->GetSafeHdc());
}

//Read a triangle surface/plane: grid and nodes
void CRealModelView::ReadDataSurfaceOld(CSurf* surf)
{
	int i, j, k, m, n0, n1, n2, ndata;
	float x, y, z;
	ndata = AfxMessageBox(_T("Read a Triangular Surface's Mesh ?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6){
		CFileDialog Dlg0(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"));
    	if(Dlg0.DoModal() == IDOK){
			ifstream fin0(Dlg0.GetPathName());
			while(!fin0.eof())
			{   
				fin0 >> i >> n0 >> n1 >> n2 ; 
				surf->pTrs.Add(new CTrgl(i, n0, n1, n2));  // Numbering begins at 0
			}
			fin0.close();
		}
	}
	else return;

	ndata = AfxMessageBox(_T("Read a Triangular Surface's Node ?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6){
		CFileDialog Dlg1(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"));
    	if(Dlg1.DoModal() == IDOK){
			ifstream fin1(Dlg1.GetPathName());
			while(!fin1.eof())
			{
				fin1 >> i >> x >> y >> z;
				surf->pNds.Add(new CNode(i, x, y, z));  // Numbering begins at 0
			}
			fin1.close();
		}		 
	}
	else return;

	//将每个三角形节点坐标保存到各自三角形中 2009.11.02
	int id;
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			surf->pTrs.GetAt(i)->x[j] = surf->pNds.GetAt(id)->x;
			surf->pTrs.GetAt(i)->y[j] = surf->pNds.GetAt(id)->y;
			surf->pTrs.GetAt(i)->z[j] = surf->pNds.GetAt(id)->z;
		}
	}

	//记录经过每个点的三角形ID。 CArray<int, int>sPassedTrgl;//经过该点的三角形  2009.11.11
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			surf->pNds.GetAt(id)->sPsdTrgl.Add(i);
		}
	}

	//记录每个三角形的邻域三角形，不包括自身   	CArray<int, int>iRgnPassTr;//相邻单元号  2009.11.11
	CArray<int, int>idset;
	bool bexist;
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		idset.RemoveAll();
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			for(k = 0; k < surf->pNds.GetAt(id)->sPsdTrgl.GetSize(); k++) {
				bexist = false;
				for(m = 0; m < idset.GetSize(); m++) {
					if( idset.GetAt(m) == surf->pNds.GetAt(id)->sPsdTrgl.GetAt(k)) { 
						bexist = true;  break; 
					}
				}
				if( !bexist && surf->pNds.GetAt(id)->sPsdTrgl.GetAt(k) != i)  
					idset.Add( surf->pNds.GetAt(id)->sPsdTrgl.GetAt(k) );
			}

		}
		for(j = 0; j < idset.GetSize(); j++) {
			surf->pTrs.GetAt(i)->iRgnPassTr.Add(idset.GetAt(j));
		}
	}

}

void CRealModelView::ReadPolygonSurface(CSurf * surf)
{
	int i, j, id, m, n0, ndata;
	double x, y, z;
	int nNds, nTrgls;
	ndata = AfxMessageBox(_T("Read a Polygon Surface ?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == IDYES){
		CFileDialog Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|All Files (*.*)|*.*"), NULL);
    	if(Dlg.DoModal() == IDOK){
			ifstream fin(Dlg.GetPathName());
			fin >> nNds >> nTrgls;
			for(i = 0; i < nNds; i++) {
		        fin >> i >> x >> y >> z;
				surf->pNds.Add(new CNode(i, x, y, z)); 
			}
			for(i = 0; i < nTrgls; i++) {
				CLoop * lp = new CLoop();
				fin >> id >> m;
				lp->ID = id;
				for(j = 0; j < m; j++) {
					fin >> n0;
					lp->iNds.Add(n0);
				    lp->pNds.Add(surf->pNds.GetAt(n0));
				}
				surf->pLps.Add(lp);
			}

			fin.close();
		}
	}
	else return;

}

//2010.4.20 by Mei Gang, University of Freiburg
//Read a triangle surface/plane: grid and nodes. New file format
void CRealModelView::ReadDataSurface(CSurf* surf)
{
	int i, j, k, m, n0, n1, n2, ndata;
	double x, y, z;
	int nNds, nTrgls;
	ndata = AfxMessageBox(_T("Read a Triangle Surface?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6){
	m_nObjectType = OBJECT_TYPE_SURFACE;

	// MSDN 上的例子
    // szFilters is a text string that includes two file name filters:
    // "*.txt" for "MyType Files" and "*.*' for "All Files."
    TCHAR szFilters[]= _T("MyType Files (*.txt)|*.txt|All Files (*.*)|*.*||");
 
   // Create an Open dialog; the default file name extension is ".my".
    CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
       OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
 
   // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
   // returns IDOK.
    if(fileDlg.DoModal() == IDOK)
    {
       CString pathName = fileDlg.GetPathName();
 
      // Implement opening and reading file in here.
 
      //Change the window's title to the opened file's title.
       CString fileName = fileDlg.GetFileTitle();
 
      // SetWindowText(fileName);

	  // For Output
	  if(pFrame == NULL) {
	     pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	  }
	  CString str;
	  str = _T("Open a Surface : ") + pathName;
	  pFrame->AddStrToOutputWnd(1, str);
   // }

	  std::locale loc1 = std::locale::global(std::locale(".936"));

	//	CFileDialog Dlg0(true,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Strata File(*.str)|*.str|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"), NULL);
    //	if(Dlg0.DoModal() == IDOK){
		//	ifstream fin0(fileDlg.GetPathName());

	        fstream fin0;
		//	fin0.open(fileDlg.GetPathName(),ios::in);
			fin0.open(pathName,ios::in);
			
			if(fin0.fail() == true) {
				AfxMessageBox(_T("Fail to open file!"));
				exit(0);
			}

			fin0 >> nNds >> nTrgls;
			for(i = 0; i < nNds; i++) {
		        fin0 >> i >> x >> y >> z;
				surf->pNds.Add(new CNode(i, x, y, z));  // Numbering begins at 0
			//	surf->pNds.Add(new CNode(--i, x, y, z));  // Numbering begins at 1
			}
			for(i = 0; i < nTrgls; i++) {
				fin0 >> i >> n0 >> n1 >> n2 ; 
				surf->pTrs.Add(new CTrgl(i, n0, n1, n2));  // Numbering begins at 0
			//	surf->pTrs.Add(new CTrgl(--i, --n0, --n1, --n2));  // Numbering begins at 1 
			}

			fin0.close();

		  std::locale::global(std::locale(loc1));

		}
	}
	else return;

	//将每个三角形节点坐标保存到各自三角形中 2009.11.02
	int id;
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			surf->pTrs.GetAt(i)->x[j] = surf->pNds.GetAt(id)->x;
			surf->pTrs.GetAt(i)->y[j] = surf->pNds.GetAt(id)->y;
			surf->pTrs.GetAt(i)->z[j] = surf->pNds.GetAt(id)->z;
		}
	}

	//记录经过每个点的三角形ID。 CArray<int, int>sPassedTrgl;//经过该点的三角形  2009.11.11
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			surf->pNds.GetAt(id)->sPsdTrgl.Add(i);
		}
	}

	//记录每个三角形的邻域三角形，不包括自身   	CArray<int, int>isurfPassTr;//相邻单元号  2009.11.11
	CArray<int, int>idset;
	bool bexist;
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		idset.RemoveAll();
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			for(k = 0; k < surf->pNds.GetAt(id)->sPsdTrgl.GetSize(); k++) {
				bexist = false;
				for(m = 0; m < idset.GetSize(); m++) {
					if( idset.GetAt(m) == surf->pNds.GetAt(id)->sPsdTrgl.GetAt(k)) { bexist = true;  break; }
				}
				if( !bexist && surf->pNds.GetAt(id)->sPsdTrgl.GetAt(k) != i)  
					idset.Add( surf->pNds.GetAt(id)->sPsdTrgl.GetAt(k) );
			}

		}
		for(j = 0; j < idset.GetSize(); j++) {
			surf->pTrs.GetAt(i)->iRgnPassTr.Add(idset.GetAt(j));
		}
	}

}

// Read a triangle surface/plane: grid and nodes
// New format while support EGC. Gang Mei 2012.12.25
void CRealModelView::ReadDataSurfaceEGC(CSurf* surf)
{
	int i, j, k, m, n0, n1, n2, ndata;
	float x, y, z;
	int nNds, nTrgls;

	ndata = AfxMessageBox(_T("Read a Triangular Surface ?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6){
		CFileDialog Dlg0(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"));
    	if(Dlg0.DoModal() == IDOK){
			ifstream fin0(Dlg0.GetPathName());

			fin0 >> nNds >> nTrgls;
			for(i = 0; i < nNds; i++) {
		        fin0 >> i >> x >> y >> z;
				surf->pNds.Add(new CNode(i, x, y, z));  // Numbering begins at 0
			}
			for(i = 0; i < nTrgls; i++) {
				fin0 >> i >> n0 >> n1 >> n2 ; 
				surf->pTrs.Add(new CTrgl(i, n0, n1, n2));  // Numbering begins at 0
			}

			fin0.close();
		}
	}
	else return;

	//将每个三角形节点坐标保存到各自三角形中 2009.11.02
	int id;
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			surf->pTrs.GetAt(i)->x[j] = surf->pNds.GetAt(id)->x;
			surf->pTrs.GetAt(i)->y[j] = surf->pNds.GetAt(id)->y;
			surf->pTrs.GetAt(i)->z[j] = surf->pNds.GetAt(id)->z;
		}
	}

	//记录经过每个点的三角形ID。 CArray<int, int>sPassedTrgl;//经过该点的三角形  2009.11.11
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			surf->pNds.GetAt(id)->sPsdTrgl.Add(i);
		}
	}

	//记录每个三角形的邻域三角形，不包括自身   	CArray<int, int>iRgnPassTr;//相邻单元号  2009.11.11
	CArray<int, int>idset;
	bool bexist;
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		idset.RemoveAll();
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			for(k = 0; k < surf->pNds.GetAt(id)->sPsdTrgl.GetSize(); k++) {
				bexist = false;
				for(m = 0; m < idset.GetSize(); m++) {
					if( idset.GetAt(m) == surf->pNds.GetAt(id)->sPsdTrgl.GetAt(k)) { 
						bexist = true;  break; 
					}
				}
				if( !bexist && surf->pNds.GetAt(id)->sPsdTrgl.GetAt(k) != i)  
					idset.Add( surf->pNds.GetAt(id)->sPsdTrgl.GetAt(k) );
			}

		}
		for(j = 0; j < idset.GetSize(); j++) {
			surf->pTrs.GetAt(i)->iRgnPassTr.Add(idset.GetAt(j));
		}
	}

}


CCordnt* CRealModelView::GetNewCordntSystem(CSurf *Surf)
{
#ifndef EGC // Not support EGC
	int i, k;
	double minx, miny, minz, maxx, maxy, maxz, sx, sy, sz, s;
	CCordnt* Crt = new CCordnt();
	//get the total number of nodes on tins
	double* cor = new double[Surf->pNds.GetSize()]; 
  
	k = Surf->pNds.GetSize();
	//record the coordinate-x
	for(i = 0; i < k; i++)cor[i] = Surf->pNds.GetAt(i)->x;

	//sort the cor
	prhap(cor, k);
	minx = cor[0];	maxx = cor[k-1];
	sx = cor[k-1] - cor[0];

	//record the coordinate-y
	for(i = 0; i < k; i++)cor[i] = Surf->pNds.GetAt(i)->y;

	//sort the cor
	prhap(cor, k);
	miny = cor[0];  maxy = cor[k-1];
	sy = cor[k-1] - cor[0];

	//record the coordinate-z
	for(i = 0; i < k; i++)cor[i] = Surf->pNds.GetAt(i)->z;
	//sort the cor
	prhap(cor, k);
	minz = cor[0]; maxz = cor[k-1];
	sz = cor[k-1] - cor[0];

    free(cor);
	Crt->u = (minx + maxx) / 2.0;  Crt->v = (miny + maxy) / 2.0;  Crt->w = (minz + maxz) / 2.0;
	Crt->minX = minx;  Crt->maxX = maxx;
	Crt->minY = miny;  Crt->maxY = maxy;
	Crt->minZ = minz;  Crt->maxZ = maxz;

	s = sx;
	if(s < sy) s = sy;
	if(s < sz) s = sz;
	Crt->Scale = 2.0 / s;

	return(Crt);

#else // Does support EGC
	int i, k;
	float minx, miny, minz, maxx, maxy, maxz, sx, sy, sz, s;
	CCordnt* Crt = new CCordnt();
	//get the total number of nodes on tins
	float * cor = new float[Surf->pNds.GetSize()]; 
  
	k = Surf->pNds.GetSize();
	//record the coordinate-x
	for(i = 0; i < k; i++)  cor[i] = Surf->pNds.GetAt(i)->x.floatValue();

	//sort the cor
	prhap(cor, k);
	minx = cor[0];	maxx = cor[k-1];
	sx = cor[k-1] - cor[0];

	//record the coordinate-y
	for(i = 0; i < k; i++) cor[i] = Surf->pNds.GetAt(i)->y.floatValue();

	//sort the cor
	prhap(cor, k);
	miny = cor[0];  maxy = cor[k-1];
	sy = cor[k-1] - cor[0];

	//record the coordinate-z
	for(i = 0; i < k; i++) cor[i] = Surf->pNds.GetAt(i)->z.floatValue();
	//sort the cor
	prhap(cor, k);
	minz = cor[0]; maxz = cor[k-1];
	sz = cor[k-1] - cor[0];

    free(cor);
	Crt->u = (minx + maxx) / 2.0;  Crt->v = (miny + maxy) / 2.0;  Crt->w = (minz + maxz) / 2.0;
	Crt->minX = minx;  Crt->maxX = maxx;
	Crt->minY = miny;  Crt->maxY = maxy;
	Crt->minZ = minz;  Crt->maxZ = maxz;

	s = sx;
	if(s < sy) s = sy;
	if(s < sz) s = sz;
	Crt->Scale = 2.0 / s;

	return(Crt);

#endif

}

double CRealModelView::VolOfTetra(double *x, double *y, double *z)
{
	double v;
    v = (x[3] - x[0]) * (y[3] - y[1]) * (z[3] - z[2]) + 
		(x[3] - x[1]) * (y[3] - y[2]) * (z[3] - z[0]) + 
		(x[3] - x[2]) * (y[3] - y[0]) * (z[3] - z[1]) -
        (x[3] - x[2]) * (y[3] - y[1]) * (z[3] - z[0]) -		
        (x[3] - x[1]) * (y[3] - y[0]) * (z[3] - z[2]) -
        (x[3] - x[0]) * (y[3] - y[2]) * (z[3] - z[1]);
	return(v);
}

float CRealModelView::VolOfTetra(float *x, float *y, float *z)
{
	float v;
    v = (x[3] - x[0]) * (y[3] - y[1]) * (z[3] - z[2]) + 
		(x[3] - x[1]) * (y[3] - y[2]) * (z[3] - z[0]) + 
		(x[3] - x[2]) * (y[3] - y[0]) * (z[3] - z[1]) -
        (x[3] - x[2]) * (y[3] - y[1]) * (z[3] - z[0]) -		
        (x[3] - x[1]) * (y[3] - y[0]) * (z[3] - z[2]) -
        (x[3] - x[0]) * (y[3] - y[2]) * (z[3] - z[1]);
	return(v);
}

void CRealModelView::NodeOnNormal(double *x, double *y, double *z, double eps)
{
	double *pa, A, B, C, D, t;
	double dt = eps;
	pa = new double[4];
    
	//根据三角形结点求出平面方程
	EquaOfTrgl(x, y, z, pa);
	A = pa[0]; B = pa[1]; C = pa[2], D = pa[3];

	//三角形法向的方向余弦
	//A != 0 && B == 0 && C == 0//基准面与Z0Y面平行
	if(fabs(A) > dt && fabs(B) < dt && fabs(C) < dt){
		x[3] = x[0] + 1; y[3] = y[0]; z[3] = z[0];
		if(VolOfTetra(x,y,z) < 0) x[3] = x[0] - 1; //保证新Z轴与三角形正法向一致
	}
	//A == 0 && B == 0 && C != 0//基准面与X0Y面平行
	else if(fabs(A) < dt && fabs(B) < dt && fabs(C) > dt){
		x[3] = x[0]; y[3] = y[0]; z[3] = z[0] + 1;
		if(VolOfTetra(x,y,z) < 0) z[3] = z[0] - 1; //保证新Z轴与三角形正法向一致
	}
	//A == 0 && B != 0 && C == 0//基准面与X0Z面平行
	else if(fabs(A) < dt && fabs(B) > dt && fabs(C) < dt){
		x[3] = x[0]; y[3] = y[0] + 1; z[3] = z[0];
		if(VolOfTetra(x,y,z) < 0) y[3] = y[0] - 1; //保证新Z轴与三角形正法向一致
	}
	//A == 0 && B != 0 && C != 0//基准面与X轴平行
	else if(fabs(A) < dt && fabs(B) > dt && fabs(C) > dt){
		t = 1 / sqrt(B * B + C * C);
		x[3] = x[0];  y[3] = B * t + y[0];  z[3] = C * t + z[0];
		if(VolOfTetra(x,y,z) < 0) {//保证新Z轴与三角形正法向一致
			t = -1 * t; y[3] = B * t + y[0];  z[3] = C * t + z[0];
		}
	}
	//A != 0 && B == 0 && C != 0{//基准面与Y轴平行
	else if(fabs(A) > dt && fabs(B) < dt && fabs(C) > dt){
		t = 1 / sqrt(A * A + C * C);
		x[3] =A * t + x[0];  y[3] = y[0];  z[3] = C * t + z[0];
		if(VolOfTetra(x,y,z) < 0) {//保证新Z轴与三角形正法向一致
			t = -1 * t; x[3] = A * t + x[0];  z[3] = C * t + z[0];
		}
	}
	//A != 0 && B != 0 && C == 0//基准面与Y轴平行
	else if(fabs(A) > dt && fabs(B) > dt && fabs(C) < dt){
		t = 1 / sqrt(A * A + B * B);
		x[3] =A * t + x[0];  y[3] = B * t + y[0];  z[3] = z[0];
		if(VolOfTetra(x,y,z) < 0) {//保证新Z轴与三角形正法向一致
			t = -1 * t; x[3] = A * t + x[0];  y[3] = B * t + y[0];
		}
	}
	//A != 0 && B != 0 && C != 0//其他一般情况
	else if(fabs(A) > dt && fabs(B) > dt && fabs(C) > dt){
		t = 1 / sqrt(A * A + B * B + C * C);
		x[3] = A * t + x[0]; y[3] = B * t + y[0]; z[3] = C * t + z[0];
		if(VolOfTetra(x,y,z) < 0) {//保证新Z轴与三角形正法向一致
			t = -1 * t; x[3] = A * t + x[0]; y[3] = B * t + y[0]; z[3] = C * t + z[0];
		}
	}
	delete[] pa;
}

void CRealModelView::NodeOnNormal(float *x, float *y, float *z, float eps)
{
	float *pa, A, B, C, D, t;
	float dt = eps;
	pa = new float[4];
    
	//根据三角形结点求出平面方程
	EquaOfTrgl(x, y, z, pa);
	A = pa[0]; B = pa[1]; C = pa[2], D = pa[3];

	//三角形法向的方向余弦
	//A != 0 && B == 0 && C == 0//基准面与Z0Y面平行
	if(fabs(A) > dt && fabs(B) < dt && fabs(C) < dt){
		x[3] = x[0] + 1; y[3] = y[0]; z[3] = z[0];
		if(VolOfTetra(x,y,z) < 0) x[3] = x[0] - 1; //保证新Z轴与三角形正法向一致
	}
	//A == 0 && B == 0 && C != 0//基准面与X0Y面平行
	else if(fabs(A) < dt && fabs(B) < dt && fabs(C) > dt){
		x[3] = x[0]; y[3] = y[0]; z[3] = z[0] + 1;
		if(VolOfTetra(x,y,z) < 0) z[3] = z[0] - 1; //保证新Z轴与三角形正法向一致
	}
	//A == 0 && B != 0 && C == 0//基准面与X0Z面平行
	else if(fabs(A) < dt && fabs(B) > dt && fabs(C) < dt){
		x[3] = x[0]; y[3] = y[0] + 1; z[3] = z[0];
		if(VolOfTetra(x,y,z) < 0) y[3] = y[0] - 1; //保证新Z轴与三角形正法向一致
	}
	//A == 0 && B != 0 && C != 0//基准面与X轴平行
	else if(fabs(A) < dt && fabs(B) > dt && fabs(C) > dt){
		t = 1 / sqrt(B * B + C * C);
		x[3] = x[0];  y[3] = B * t + y[0];  z[3] = C * t + z[0];
		if(VolOfTetra(x,y,z) < 0) {//保证新Z轴与三角形正法向一致
			t = -1 * t; y[3] = B * t + y[0];  z[3] = C * t + z[0];
		}
	}
	//A != 0 && B == 0 && C != 0{//基准面与Y轴平行
	else if(fabs(A) > dt && fabs(B) < dt && fabs(C) > dt){
		t = 1 / sqrt(A * A + C * C);
		x[3] =A * t + x[0];  y[3] = y[0];  z[3] = C * t + z[0];
		if(VolOfTetra(x,y,z) < 0) {//保证新Z轴与三角形正法向一致
			t = -1 * t; x[3] = A * t + x[0];  z[3] = C * t + z[0];
		}
	}
	//A != 0 && B != 0 && C == 0//基准面与Y轴平行
	else if(fabs(A) > dt && fabs(B) > dt && fabs(C) < dt){
		t = 1 / sqrt(A * A + B * B);
		x[3] =A * t + x[0];  y[3] = B * t + y[0];  z[3] = z[0];
		if(VolOfTetra(x,y,z) < 0) {//保证新Z轴与三角形正法向一致
			t = -1 * t; x[3] = A * t + x[0];  y[3] = B * t + y[0];
		}
	}
	//A != 0 && B != 0 && C != 0//其他一般情况
	else if(fabs(A) > dt && fabs(B) > dt && fabs(C) > dt){
		t = 1 / sqrt(A * A + B * B + C * C);
		x[3] = A * t + x[0]; y[3] = B * t + y[0]; z[3] = C * t + z[0];
		if(VolOfTetra(x,y,z) < 0) {//保证新Z轴与三角形正法向一致
			t = -1 * t; x[3] = A * t + x[0]; y[3] = B * t + y[0]; z[3] = C * t + z[0];
		}
	}
	delete[] pa;
}

void CRealModelView::GetNewCoordt(CTrgl *tr, CCordnt *NewCordt)
{
#ifndef EGC // Not support EGC
	double *x, *y, *z;
	x = new double[4]; y = new double[4]; z = new double[4]; 
    x[0] = tr->x[0];  x[1] = tr->x[1];  x[2] = tr->x[2];
    y[0] = tr->y[0];  y[1] = tr->y[1];  y[2] = tr->y[2];
    z[0] = tr->z[0];  z[1] = tr->z[1];  z[2] = tr->z[2];
     //坐标平移量
	NewCordt->u = x[0];  NewCordt->v = y[0];  NewCordt->w = z[0];  
 
	//0-1边方向余弦 //新坐标系中X轴
	double d = sqrt( (x[1]-x[0])*(x[1]-x[0]) + (y[1]-y[0])*(y[1]-y[0]) + (z[1]-z[0])*(z[1]-z[0]) );
	NewCordt->lx = (x[1] - x[0]) / d;	NewCordt->mx = (y[1] - y[0]) / d;	NewCordt->nx = (z[1] - z[0]) / d;
    
	//求法向的正方向余弦 //新坐标系中Z轴
	NodeOnNormal(x, y, z, myEps); //求法线上的x[3],y[3],z[3],该点与x[0],y[0],z[0]距离为1
	NewCordt->lz = x[3] - x[0]; NewCordt->mz = y[3] - y[0];  NewCordt->nz = z[3] - z[0];

	//求0-2边正方向的方向余弦 //新坐标系中Y轴
	x[2] = x[1]; y[2] = y[1]; z[2] = z[1]; x[1] = x[3]; y[1] = y[3]; z[1] = z[3];
	NodeOnNormal(x, y, z, myEps); //求法线上的x[3],y[3],z[3],该点与x[0],y[0],z[0]距离为1
	NewCordt->ly = x[3] - x[0]; NewCordt->my = y[3] - y[0]; NewCordt->ny = z[3] - z[0];
	delete[] x; delete[] y; delete[] z;

#else // Does support EGC
	float *x, *y, *z;
	x = new float[4]; y = new float[4]; z = new float[4]; 
	for(int i = 0; i < 3; i++) {
		x[i] = tr->x[i].floatValue();  
		y[i] = tr->y[i].floatValue();
		z[i] = tr->z[i].floatValue();
	}

     //坐标平移量
	NewCordt->u = x[0];  NewCordt->v = y[0];  NewCordt->w = z[0];  
 
	//0-1边方向余弦 //新坐标系中X轴
	float d = sqrt( (x[1]-x[0])*(x[1]-x[0]) + (y[1]-y[0])*(y[1]-y[0]) + (z[1]-z[0])*(z[1]-z[0]) );
	NewCordt->lx = (x[1] - x[0]) / d;	NewCordt->mx = (y[1] - y[0]) / d;	NewCordt->nx = (z[1] - z[0]) / d;
    
	//求法向的正方向余弦 //新坐标系中Z轴
	NodeOnNormal(x, y, z, myEps); //求法线上的x[3],y[3],z[3],该点与x[0],y[0],z[0]距离为1
	NewCordt->lz = x[3] - x[0]; NewCordt->mz = y[3] - y[0];  NewCordt->nz = z[3] - z[0];

	//求0-2边正方向的方向余弦 //新坐标系中Y轴
	x[2] = x[1]; y[2] = y[1]; z[2] = z[1]; x[1] = x[3]; y[1] = y[3]; z[1] = z[3];
	NodeOnNormal(x, y, z, myEps); //求法线上的x[3],y[3],z[3],该点与x[0],y[0],z[0]距离为1
	NewCordt->ly = x[3] - x[0]; NewCordt->my = y[3] - y[0]; NewCordt->ny = z[3] - z[0];
	delete[] x; delete[] y; delete[] z;

#endif

}

//compile tin lists for draw
void CRealModelView::CompileTinList(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
#ifndef EGC // Not support EGC
	int j, k, n;
	double x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];     

    //begin a list
	/////////////////////////////////////////////
	glNewList(list, GL_COMPILE);
	if(lf == 0)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else 	   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES); 
	{
		for(j = 0; j < Surf->pTrs.GetSize(); j++){
			//get the index and coordinates of nodes
			for(k = 0; k < 3; k++){
				n = Surf->pTrs.GetAt(j)->nID[k];  
				x[k] = Surf->pNds.GetAt(n)->x; 
				y[k] = Surf->pNds.GetAt(n)->y;  
				z[k] = Surf->pNds.GetAt(n)->z; 
	            x[k] = (x[k] - Crt->u) * Crt->Scale;	
                y[k] = (y[k] - Crt->v) * Crt->Scale;	
                z[k] = (z[k] - Crt->w) * Crt->Scale;
			}
			
			//calculate the normal
			fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
			fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
			fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
			CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
			glNormal3dv(fNormal);
			
			//draw trgl
			for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		}			
	}
	glEnd();	
	glEndList();

#else // Does support EGC
	int j, k, n;
	float x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];     

    //begin a list
	/////////////////////////////////////////////
	glNewList(list, GL_COMPILE);
	if(lf == 0)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else 	   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES); 
	{
		for(j = 0; j < Surf->pTrs.GetSize(); j++){
			//get the index and coordinates of nodes
			for(k = 0; k < 3; k++){
				n = Surf->pTrs.GetAt(j)->nID[k];  
				x[k] = Surf->pNds.GetAt(n)->x.floatValue(); 
				y[k] = Surf->pNds.GetAt(n)->y.floatValue();  
				z[k] = Surf->pNds.GetAt(n)->z.floatValue(); 
	            x[k] = (x[k] - Crt->u) * Crt->Scale;	
                y[k] = (y[k] - Crt->v) * Crt->Scale;	
                z[k] = (z[k] - Crt->w) * Crt->Scale;
			}
			
			//calculate the normal
			fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
			fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
			fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
			CalculateNormal(fPoint0, fPoint1, fPoint2, fNormal);
			glNormal3dv(fNormal);
			
			//draw trgl
			for(k = 0; k < 3; k++)  glVertex3f(x[k], y[k], z[k]);//
		}			
	}
	glEnd();	
	glEndList();

#endif

}

//To compute the intersecton of segment-segment
//Return value:
//0-Segments do not intersect;
//1-Segments intersect on a point;
//2-Segments intersect on a interval;
int CRealModelView::IntrSegment2Segment2D(CNode * p0, CNode * d0, CNode * p1, CNode * d1, CNode * i0, CNode * i1)
{
	CNode * E = new CNode(p1->x - p0->x, p1->y - p0->y);
	double kross = d0->x * d1->y - d0->y * d1->x;
	double sqrKross = kross * kross;
	double sqrLen0 = d0->x * d0->x + d0->y * d0->y;
	double sqrLen1 = d1->x * d1->x + d1->y * d1->y;
	if(sqrKross > sqrEpsilon * sqrLen0 * sqrLen1) {
		//lines of segments are not parallel
		double s = (E->x * d1->y - E->y * d1->x ) / kross;
		double t = (E->x * d0->y - E->y * d0->x ) / kross;
		if(-1*Epsilon < s && s < 1 + Epsilon && -1*Epsilon < t && t < 1 + Epsilon) {
			//intersection of lines is a point on each segment
			i0->x = p0->x + s * d0->x;    i0->y = p0->y + s * d0->y;    i0->z = 0.0;
			return 1;
		}
		else {
			//intersection of lines is not a point on segment p0 + s * d0
			//intersection of lines is not a point on segment p1 + t * d1
			return 0;
		}
	}
	
	else {
		//lines of the segments are parallel
		double sqrLenE = E->x * E->x + E->y * E->y;
		kross = E->x * d0->y - E->y * d0->x ;
		sqrKross = kross * kross;
		if(sqrKross > sqrEpsilon * sqrLen0 * sqrLenE) {
			// lines are different
			return 0;
		}
		else {
			//lines of the segments are the same. Need to test for overlap of segements
			double s0 = (d0->x * E->x + d0->y * E->y ) / sqrLen0;
			double s1 = s0 + (d0->x * d1->x + d0->y * d1->y ) / sqrLen0;
			double w[2], smin, smax;
			if(s0 > s1)  { smin = s1;  smax = s0; }
			else { smin = s0;  smax = s1; }
			int imax = FindIntersection(0.0, 1.0, smin, smax, w, myEps);
			
			if(imax == 1) { i0->x = p0->x + w[0] * d0->x;    i0->y = p0->y + w[0] * d0->y;    i0->z = 0.0;}
			if(imax == 2) { i0->x = p0->x + w[0] * d0->x;    i0->y = p0->y + w[0] * d0->y;    i0->z = 0.0;
			                i1->x = p0->x + w[1] * d0->x;    i1->y = p0->y + w[1] * d0->y;    i1->z = 0.0;}
			return imax;
		}
	}
	delete E;

}

//common function
/////////////////////////////////////////////////////////////////////////////
void CRealModelView::pihap(int* p, int n)
{
	int i,mm,t;
    mm = n/2;
    for(i = mm - 1; i >= 0; i--)
       isift(p,i,n-1);
    for(i = n-1; i >= 1; i--)
    { 
		t = p[0];   p[0] = p[i];  p[i] = t;
        isift(p,0,i-1);
     }
}

void CRealModelView::isift(int *p, int i, int n)
{
	int j,t;
    t = p[i];  j = 2*(i+1)-1;
    while(j <= n)
      { 
		if((j<n) && (p[j]<p[j+1])) j = j + 1;
        if(t < p[j])
        { 
			p[i] = p[j];   i = j;   j = 2*(i+1)-1;
		}
        else j = n + 1;
      }
    p[i]=t;
}

//double数的堆排序
void CRealModelView::prhap(double* p,int n)
{
	int i,mm;
    double t;
    mm=n/2;
    for (i=mm-1; i>=0; i--)
      rsift(p,i,n-1);
    for (i=n-1; i>=1; i--)
      { t=p[0]; p[0]=p[i]; p[i]=t;
        rsift(p,0,i-1);
      }
}

//double数的堆排序
void CRealModelView::prhap(float* p,int n)
{
	int i,mm;
    float t;
    mm=n/2;
    for (i=mm-1; i>=0; i--)
      rsift(p,i,n-1);
    for (i=n-1; i>=1; i--)
      { t=p[0]; p[0]=p[i]; p[i]=t;
        rsift(p,0,i-1);
      }
}

//组堆
void CRealModelView::rsift(double* p, int i, int n)
{
	int j;
    double t;
    t=p[i]; j=2*(i+1)-1;
    while (j<=n)
      { if ((j<n)&&(p[j]<p[j+1])) j=j+1;
        if (t<p[j])
          { p[i]=p[j]; i=j; j=2*(i+1)-1;}
        else j=n+1;
      }
    p[i]=t;
}

//组堆
void CRealModelView::rsift(float* p, int i, int n)
{
	int j;
    float t;
    t=p[i]; j=2*(i+1)-1;
    while (j<=n)
      { if ((j<n)&&(p[j]<p[j+1])) j=j+1;
        if (t<p[j])
          { p[i]=p[j]; i=j; j=2*(i+1)-1;}
        else j=n+1;
      }
    p[i]=t;
}

void CRealModelView::prhap(double* p, int* ID, int n)
{
	int i,mm,d;
    double t;
    mm=n/2;
    for (i=mm-1; i>=0; i--)
      rsift(p,ID,i,n-1);
    for (i=n-1; i>=1; i--)
      { t=p[0]; d=ID[0];
	    p[0]=p[i]; ID[0]=ID[i];
		p[i]=t; ID[i]=d;
        rsift(p,ID,0,i-1);
      }
}

void CRealModelView::prhap(float* p, int* ID, int n)
{
	int i,mm,d;
    float t;
    mm=n/2;
    for (i=mm-1; i>=0; i--)
      rsift(p,ID,i,n-1);
    for (i=n-1; i>=1; i--)
      { t=p[0]; d=ID[0];
	    p[0]=p[i]; ID[0]=ID[i];
		p[i]=t; ID[i]=d;
        rsift(p,ID,0,i-1);
      }
}

//组堆
void CRealModelView::rsift(double* p, int* ID, int i, int n)
{
	int j, d;
    double t;
    t=p[i]; d=ID[i]; j=2*(i+1)-1;
    while (j<=n)
      { if ((j<n)&&(p[j]<p[j+1])) j=j+1;
        if (t<p[j])
          { p[i]=p[j]; ID[i]=ID[j]; i=j; j=2*(i+1)-1;}
        else j=n+1;
      }
    p[i]=t; ID[i]=d;
    return;
}

//组堆
void CRealModelView::rsift(float* p, int* ID, int i, int n)
{
	int j, d;
    float t;
    t=p[i]; d=ID[i]; j=2*(i+1)-1;
    while (j<=n)
      { if ((j<n)&&(p[j]<p[j+1])) j=j+1;
        if (t<p[j])
          { p[i]=p[j]; ID[i]=ID[j]; i=j; j=2*(i+1)-1;}
        else j=n+1;
      }
    p[i]=t; ID[i]=d;
    return;
}

void CRealModelView::prhap(int* p, int* ID, int n)
{
	int i,mm,d,t;
    mm=n/2;
    for (i=mm-1; i>=0; i--)
      rsift(p,ID,i,n-1);
    for (i=n-1; i>=1; i--)
      { t=p[0]; d=ID[0];
	    p[0]=p[i]; ID[0]=ID[i];
		p[i]=t; ID[i]=d;
        rsift(p,ID,0,i-1);
      }
    return;
}

//组堆
void CRealModelView::rsift(int* p, int* ID, int i, int n)
{
	int j,d,t;
    t=p[i]; d=ID[i]; j=2*(i+1)-1;
    while (j<=n)
      { if ((j<n)&&(p[j]<p[j+1])) j=j+1;
        if (t<p[j])
          { p[i]=p[j]; ID[i]=ID[j]; i=j; j=2*(i+1)-1;}
        else j=n+1;
      }
    p[i]=t; ID[i]=d;
    return;
}

//Node数的堆排序  according to axis x
void CRealModelView::SortNode(CNode* p, int n)
{
	int i, j, k, l, mx, my, mz;
	double* x = new double[n];
	int* c = new int[n];
	//copy the data
	for(i = 0; i < n; i++){x[i] = p[i].x;  c[i] = p[i].ID;}
	//sort according to x
	prhap(x, c, n);
	i = 0;
	while(i < n){
		qrbsh(x, n, x[i], x[i], &mx);
		double* y = new double[mx];
		for(l = 0; l < mx; l++) y[l] = p[c[i + l]].y; 
		//sort y with same x
		prhap(y, c + i, mx);
		j = 0;
		while(j < mx){
			qrbsh(y, mx, y[j], y[j], &my);
		    double* z = new double[my];
            for(l = 0; l < my; l++) z[l] = p[c[i + j + l]].z;
			//sort z with same y
		    prhap(z, c + i + j, my);
			k = 0;
			while(k < my){
				qrbsh(z, my, z[k], z[k], &mz);
				//sort c with same z
				pihap(c + i + j + k, mz);
				k += mz;
			}
			delete[] z;
			j += my;
		}
		delete[] y;
		i += mx;
	}
	//copy the data for output
	for(i = 0; i < n; i++)p[i].ID = c[i];  
	for(i = 0; i < n; i++)x[i] = p[i].x;  
	for(i = 0; i < n; i++)p[i].x = x[c[i]];  
	for(i = 0; i < n; i++)x[i] = p[i].y;  
	for(i = 0; i < n; i++)p[i].y = x[c[i]];  
	for(i = 0; i < n; i++)x[i] = p[i].z;  
	for(i = 0; i < n; i++)p[i].z = x[c[i]]; 

	delete[] x;  delete[] c;
}

//search ID of suitable node
int CRealModelView::qrbsh(double* p, int n, double a, double b, int* m)
{
    int i,j,k;
    i=1; j=n;
    while (i<=j)
      { k=(i+j)/2;
        if ((p[k-1]>=a)&&(p[k-1]<=b))
          { i=k-1; j=0;
            while ((i>=0)&&(p[i]>=a)) 
                { i=i-1; j=j+1;}
            i=i+1;
            while ((k<=n-1)&&(p[k]<=b)) 
                { k=k+1; j=j+1;}
            *m=j; return(i);
          }
        if (p[k-1]>b) j=k-1;
        else i=k+1;
      }
    while ((i>=0)&&(b<p[i])) i=i-1;
    i=i+1; *m=0;
    return(i);
}

//search ID of suitable node
int CRealModelView::qrbsh(int* p, int n, int a, int b, int* m)
{
    int i,j,k;
    i=1; j=n;
    while (i<=j)
      { k=(i+j)/2;
        if ((p[k-1]>=a)&&(p[k-1]<=b))
          { i=k-1; j=0;
            while ((i>=0)&&(p[i]>=a)) 
                { i=i-1; j=j+1;}
            i=i+1;
            while ((k<=n-1)&&(p[k]<=b)) 
                { k=k+1; j=j+1;}
            *m=j; return(i);
          }
        if (p[k-1]>b) j=k-1;
        else i=k+1;
      }
    while ((i>=0)&&(b<p[i])) i=i-1;
    i=i+1; *m=0;
    return(i);
}

//矩阵求逆
void CRealModelView::Dcinv(double *a, int n)
{
	int *is,*js,i,j,k,l,u,v;
    double d,p;
    is = new int[n * sizeof(int)];
    js= new int[n * sizeof(int)];
    for (k=0; k<=n-1; k++)
      { d=0.0;
        for (i=k; i<=n-1; i++)
        for (j=k; j<=n-1; j++)
          { l=i*n+j; p=fabs(a[l]);
            if (p>d) { d=p; is[k]=i; js[k]=j;}
          }
        if (d+1.0==1.0)
          { free(is); free(js); //printf("err**not inv\n");
            //return(0);
          }
        if (is[k]!=k)
          for (j=0; j<=n-1; j++)
            { u=k*n+j; v=is[k]*n+j;
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
        if (js[k]!=k)
          for (i=0; i<=n-1; i++)
            { u=i*n+k; v=i*n+js[k];
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
        l=k*n+k;
        a[l]=1.0/a[l];
        for (j=0; j<=n-1; j++)
          if (j!=k)
            { u=k*n+j; a[u]=a[u]*a[l];}
        for (i=0; i<=n-1; i++)
          if (i!=k)
            for (j=0; j<=n-1; j++)
              if (j!=k)
                { u=i*n+j;
                  a[u]=a[u]-a[i*n+k]*a[k*n+j];
                }
        for (i=0; i<=n-1; i++)
          if (i!=k)
            { u=i*n+k; a[u]=-a[u]*a[l];}
      }
    for (k=n-1; k>=0; k--)
      { if (js[k]!=k)
          for (j=0; j<=n-1; j++)
            { u=k*n+j; v=js[k]*n+j;
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
        if (is[k]!=k)
          for (i=0; i<=n-1; i++)
            { u=i*n+k; v=i*n+is[k];
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
      }
    free(is); free(js);
    //return(1);
}

int CRealModelView::gaus(double *a, double *b, int n)
{ 
	int *js,l,k,i,j,is,p,q;
    double d,t;
	js = new int[n*sizeof(int)];
	l=1;
	for (k=0;k<=n-2;k++)
	{ d=0.0;
	for (i=k;i<=n-1;i++)
		for (j=k;j<=n-1;j++)
		{ t=fabs(a[i*n+j]);
		if (t>d) { d=t; js[k]=j; is=i;}
		}
		if (d+1.0==1.0) l=0;
		else
		{ if (js[k]!=k)
		for (i=0;i<=n-1;i++)
		{ p=i*n+k; q=i*n+js[k];
		t=a[p]; a[p]=a[q]; a[q]=t;
		}
		if (is!=k)
		{ for (j=k;j<=n-1;j++)
		{ p=k*n+j; q=is*n+j;
		t=a[p]; a[p]=a[q]; a[q]=t;
		}
		t=b[k]; b[k]=b[is]; b[is]=t;
		}
		}
		if (l==0)
		{ free(js); printf("fail\n");
		return(0);
		}
		d=a[k*n+k];
		for (j=k+1;j<=n-1;j++)
		{ p=k*n+j; a[p]=a[p]/d;}
		b[k]=b[k]/d;
		for (i=k+1;i<=n-1;i++)
		{ for (j=k+1;j<=n-1;j++)
		{ p=i*n+j;
		a[p]=a[p]-a[i*n+k]*a[k*n+j];
		}
		b[i]=b[i]-a[i*n+k]*b[k];
		}
	}
	d=a[(n-1)*n+n-1];
	if (fabs(d)+1.0==1.0)
	{ free(js); printf("fail\n");
	return(0);
	}
	b[n-1]=b[n-1]/d;
	for (i=n-2;i>=0;i--)
	{ t=0.0;
	for (j=i+1;j<=n-1;j++)
		t=t+a[i*n+j]*b[j];
	b[i]=b[i]-t;
	}
	js[n-1]=n-1;
	for (k=n-1;k>=0;k--)
		if (js[k]!=k)
		{ t=b[k]; b[k]=b[js[k]]; b[js[k]]=t;}
		free(js);
		return(1);
}

void CRealModelView::Regress(int nn, int nv, double* xx, double* yy, double* zz, double* aa)
{
    int i, j, k;
	double* B;
	double* A;
	double BB[6];
	double xi, yi, hi;
	B = new double[nn];
	A = new double[nn * nn];

	for(i = 0; i < nn; i++) B[i] = 0.0;

	for(i = 0; i < nn; i++)
		for(j = 0; j < nn; j++) A[i * nn + j] = 0.0;

    for(k = 0; k < nn; k++)
		for(j = 0; j < nn; j++)
			for(i = 0; i < nv; i++)
			{
				xi = 1.0 * xx[i];
				yi = 1.0 * yy[i];
				hi = 1.0 * zz[i];
				BB[0] = 1.0;
				BB[1] = xi;
				BB[2] = yi;
				BB[3] = xi * xi;
				BB[4] = yi * yi;
				BB[5] = xi * yi;
				A[k * nn + j] += BB[j] * BB[k];
			}
			 
	for(k = 0; k < nn; k++)
		for(i = 0; i < nv; i++)
		{
			xi = 1.0 * xx[i];
			yi = 1.0 * yy[i];
			hi = 1.0 * zz[i];
			BB[0] = 1.0;
			BB[1] = xi;
			BB[2] = yi;
			BB[3] = xi * xi;
			BB[4] = yi * yi;
			BB[5] = xi * yi;
			B[k] += hi * BB[k];
		}

	int temp = Gauss(A, B, nn, aa);

	delete A; delete B;
}

//全选主元高斯消去法，a为系数矩阵，b为方程组右端常数向量，x为解向量
int CRealModelView::Gauss(double *a, double *b, int n, double *x)
{ 
	int *js,l,k,i,j,is,p,q;
    double d,t;
    js = new int[n*sizeof(int)];
    l=1;
    for (k=0;k<=n-2;k++)
      { d=0.0;
        for (i=k;i<=n-1;i++)
          for (j=k;j<=n-1;j++)
            { t=fabs(a[i*n+j]);
              if (t>d) { d=t; js[k]=j; is=i;}
            }
        if (d+1.0==1.0) l=0;
        else
          { if (js[k]!=k)
              for (i=0;i<=n-1;i++)
                { p=i*n+k; q=i*n+js[k];
                  t=a[p]; a[p]=a[q]; a[q]=t;
                }
            if (is!=k)
              { for (j=k;j<=n-1;j++)
                  { p=k*n+j; q=is*n+j;
                    t=a[p]; a[p]=a[q]; a[q]=t;
                  }
                t=b[k]; b[k]=b[is]; b[is]=t;
              }
          }
        if (l==0)
          { free(js); printf("fail\n");
            return(0);
          }
        d=a[k*n+k];
        for (j=k+1;j<=n-1;j++)
          { p=k*n+j; a[p]=a[p]/d;}
        b[k]=b[k]/d;
        for (i=k+1;i<=n-1;i++)
          { for (j=k+1;j<=n-1;j++)
              { p=i*n+j;
                a[p]=a[p]-a[i*n+k]*a[k*n+j];
              }
            b[i]=b[i]-a[i*n+k]*b[k];
          }
      }
    d=a[(n-1)*n+n-1];
    if (fabs(d)+1.0==1.0)
      { free(js); printf("fail\n");
        return(0);
      }
    x[n-1]=b[n-1]/d;
    for (i=n-2;i>=0;i--)
      { t=0.0;
        for (j=i+1;j<=n-1;j++)
          t=t+a[i*n+j]*x[j];
        x[i]=b[i]-t;
      }
    js[n-1]=n-1;
    for (k=n-1;k>=0;k--)
      if (js[k]!=k)
        { t=x[k]; x[k]=x[js[k]]; x[js[k]]=t;}
    free(js);
    return(1);
}


void CRealModelView::OnSize(UINT nType, int cx, int cy)
{
	//CRenderView::OnSize(nType, cx, cy);
	CView::OnSize(nType, cx, cy);
	
	if ( 0 < cx && 0 < cy )
	{
		// 更新客户区，计算窗口的比例
		m_ClientRect.right = cx;
		m_ClientRect.bottom = cy;
	};
}


void CRealModelView::OnImageZoom()
{
	// TODO: Add your command handler code here
	m_operator = OPERATOR_VIEW_ZOOM;
}


void CRealModelView::OnImagePan()
{
	// TODO: Add your command handler code here
	m_operator = OPERATOR_VIEW_MOVE;
}


void CRealModelView::OnImageRotate()
{
	// TODO: Add your command handler code here
	m_operator = OPERATOR_VIEW_ROTATE;
}


void CRealModelView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRealModelDoc* pDoc=GetDocument();
	point2D p_Cur;
	
	if(m_operator==OPERATOR_VIEW_ROTATE)
	{
		MouseDownPoint=point;
		SetCapture();
	}
	else if(m_operator==OPERATOR_VIEW_MOVE)
	{
		p_Cur.x=point.x;
		p_Cur.y=point.y;
		p_Cur=GetGLValue(p_Cur);
		view_base_point.x=p_Cur.x;
		view_base_point.y=p_Cur.y;
		SetCapture();
	}
	else if(m_operator==OPERATOR_VIEW_ZOOM)
	{
		MouseDownPoint=point;
		SetCapture();
	}

	CView::OnLButtonDown(nFlags, point);
	
//	CRenderView::OnLButtonDown(nFlags, point);

}


void CRealModelView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	MouseDownPoint=CPoint(0,0);
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
	
//	CRenderView::OnLButtonUp(nFlags, point);
}

void CRealModelView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRealModelDoc* pDoc=GetDocument();
	point2D p_Cur;
	{
		p_Cur.x=point.x;
		p_Cur.y=point.y;
		p_Cur=GetGLValue(p_Cur);
		view_base_point.x=p_Cur.x;
		view_base_point.y=p_Cur.y;
		SetCapture();
	}
	
	CView::OnRButtonDown(nFlags, point);

//	CRenderView::OnRButtonDown(nFlags, point);

}

void CRealModelView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	/* Code produced by VS 10.0. 2011.10.5
	ClientToScreen(&point);
	OnContextMenu(this, point);
	*/

	// TODO: Add your message handler code here and/or call default
	MouseDownPoint=CPoint(0,0);
	ReleaseCapture();

	m_operator = OPERATOR_VIEW_ROTATE;// Initiate
//	CView::OnRButtonUp(nFlags, point);

}

void CRealModelView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	point2D p_Cur;
	
	// Left button down: 3 cases
	if(MK_LBUTTON == nFlags) {

	if(m_operator == OPERATOR_VIEW_ROTATE&&GetCapture()==this)
	{
		SceneAngle.x+= (point.y-MouseDownPoint.y)/3.6;
		SceneAngle.z+= (point.x-MouseDownPoint.x)/3.6;
		MouseDownPoint=point;
	}
	else if(m_operator==OPERATOR_VIEW_ZOOM&&GetCapture()==this)
	{
		SceneZoom+=((point.y-MouseDownPoint.y)/200)*SceneZoom;
		if(SceneZoom>=200.0)
			SceneZoom=200.0;
		else if(SceneZoom<=0.1)
			SceneZoom=0.1;
		MouseDownPoint=point;
	}
	else if(m_operator==OPERATOR_VIEW_MOVE && GetCapture()==this)
	{
		p_Cur.x=point.x;
		p_Cur.y=point.y;
		p_Cur=GetGLValue(p_Cur);
		ScenePos.x+=p_Cur.x-view_base_point.x;
		ScenePos.z+=p_Cur.y-view_base_point.y;
		view_base_point.x=p_Cur.x;
		view_base_point.y=p_Cur.y;
	}

	}

	// Right button down: pan
	else if(MK_RBUTTON == nFlags) {
		m_operator=OPERATOR_VIEW_MOVE;
		if(m_operator==OPERATOR_VIEW_MOVE && GetCapture()==this)
	    {
		p_Cur.x=point.x;
		p_Cur.y=point.y;
		p_Cur=GetGLValue(p_Cur);
		ScenePos.x+=p_Cur.x-view_base_point.x;
		ScenePos.z+=p_Cur.y-view_base_point.y;
		view_base_point.x=p_Cur.x;
		view_base_point.y=p_Cur.y;
	    }
	}

	Invalidate();

 	CView::OnMouseMove(nFlags, point);

	//CRenderView::OnMouseMove(nFlags, point);
}


BOOL CRealModelView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta < 0) {
		SceneZoom = SceneZoom + (-1.0 * zDelta / 500.0) * SceneZoom;
	}
	else {
		SceneZoom = SceneZoom - (zDelta/500.0) * SceneZoom;
	}
	if(SceneZoom>=500.0)  SceneZoom=500.0;
	else if(SceneZoom<=0.01)  SceneZoom=0.01;
	Invalidate();
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
	
//	return CRenderView::OnMouseWheel(nFlags, zDelta, pt);
}

point2D CRealModelView::GetGLValue(point2D pt)
{
	point2D p;
	int w = m_ClientRect.right, h = m_ClientRect.bottom;
	if(w < h) {
		p.x=(pt.x-w/2)*SceneZoom*2/w;
		p.y=(h/2-pt.y)*SceneZoom*2/w;
	}
	else {
		p.x=(pt.x-w/2)*SceneZoom*2/h;
		p.y=(h/2-pt.y)*SceneZoom*2/h;
	}
	return p;
}


void CRealModelView::OnImageCapture()
{
	// TODO: Add extra validation here
	CBitmap m_bitmap;
	CRect rect;

	// 截取工作区
	GetWindowRect(&rect);

	/* 截取全屏
	rect.left=0;
	rect.top=0;
	rect.right=::GetSystemMetrics(SM_CXSCREEN);
	rect.bottom=::GetSystemMetrics(SM_CYSCREEN);
	*/

	m_bitmap.m_hObject = CopyScreenToBitmap(rect);
	
//	CFileDialog dlg(false, _T("*.bmp"), _T("Capture.bmp"), NULL, _T("*.bmp|*.bmp||"));
	CFileDialog dlg(false, _T("*.bmp"), _T("Capture.bmp"), OFN_READONLY | OFN_OVERWRITEPROMPT, _T("*.bmp|*.bmp||"));

	if (dlg.DoModal()==IDOK)
	{
		SaveBitmapToFile(m_bitmap,dlg.GetPathName());
	}

}

HBITMAP CRealModelView::CopyScreenToBitmap(LPRECT lpRect) 
{ 
	
	// 屏幕和内存设备描述表 
	HDC hScrDC=0, hMemDC; 
	// 位图句柄 
	HBITMAP hBitmap, hOldBitmap; 
	// 选定区域坐标 
	int nX, nY, nX2, nY2; 
	// 位图宽度和高度 
	int nWidth, nHeight; 
	// 屏幕分辨率 
	int xScrn, yScrn; 	
	// 确保选定区域不为空矩形 
	// if (IsRectEmpty(lpRect)) 
	// return NULL; 
	//为屏幕创建设备描述表 
	hScrDC = CreateDC( _T("DISPLAY"), NULL, NULL, NULL); 
	//
	int ok=1; 
	if(hScrDC==NULL)  //如果创建失败
	{	
		ok=0;
	}
	//为屏幕设备描述表创建兼容的内存设备描述表 
	hMemDC = CreateCompatibleDC(hScrDC); 	
	// 获得选定区域坐标 
	nX = lpRect-> left; 
	nY = lpRect-> top; 
	nX2 = lpRect-> right; 
	nY2 = lpRect-> bottom; 
	// 获得屏幕分辨率 
	xScrn = GetDeviceCaps(hScrDC, HORZRES); 
	yScrn = GetDeviceCaps(hScrDC, VERTRES); 
	//确保选定区域是可见的 
	if (nX <0) 
		nX = 0; 
	if (nY < 0) 
		nY = 0; 
	if (nX2 > xScrn) 
		nX2 = xScrn; 
	if (nY2 > yScrn) 
		nY2 = yScrn; 
	nWidth = nX2 - nX; 
	nHeight = nY2 - nY; 
	
	// 创建一个与屏幕设备描述表兼容的位图 
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight); 
	
	// 把新位图选到内存设备描述表中 
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); 
	
	// 把屏幕设备描述表拷贝到内存设备描述表中 
	BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY); 
	
	//得到屏幕位图的句柄 
	hBitmap =(HBITMAP)SelectObject(hMemDC, hOldBitmap); 
	
	//清除 
	DeleteDC(hScrDC); 
	DeleteDC(hMemDC); 
	
	// 返回位图句柄 
	return hBitmap; 
	
}

BOOL CRealModelView::SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpFileName ) 
{ 
	HDC hDC; //设备描述表 
	int iBits; //当前显示分辨率下每个像素所占字节数 
	WORD wBitCount; //位图中每个像素所占字节数 
	DWORD dwPaletteSize=0, //定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数 
		dwBmBitsSize, 
		dwDIBSize, dwWritten; 
	BITMAP Bitmap; //位图属性结构 
	BITMAPFILEHEADER bmfHdr; //位图文件头结构 
	BITMAPINFOHEADER bi; //位图信息头结构 
	LPBITMAPINFOHEADER lpbi; //指向位图信息头结构 
	
	HANDLE fh, hDib, hPal,hOldPal=NULL; //定义文件，分配内存句柄，调色板句柄 
	
	//计算位图文件每个像素所占字节数 
	HDC hWndDC = CreateDC(_T("DISPLAY"), NULL,NULL,NULL); 
	hDC = ::CreateCompatibleDC( hWndDC ) ; 
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); 
	DeleteDC(hDC); 
	
	if (iBits <= 1) 
		wBitCount = 1; 
	else if (iBits <= 4) 
		wBitCount = 4; 
	else if (iBits <= 8) 
		wBitCount = 8; 
	else if (iBits <= 24) 
		wBitCount = 24; 
	else 
		wBitCount = 24 ; 
	
	//计算调色板大小 
	if (wBitCount <= 8) 
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD); 
	
	//设置位图信息头结构 
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap); 
	bi.biSize = sizeof(BITMAPINFOHEADER); 
	bi.biWidth = Bitmap.bmWidth; 
	bi.biHeight = Bitmap.bmHeight; 
	bi.biPlanes = 1; 
	bi.biBitCount = wBitCount; 
	bi.biCompression = BI_RGB; 
	bi.biSizeImage = 0; 
	bi.biXPelsPerMeter = 0; 
	bi.biYPelsPerMeter = 0; 
	bi.biClrUsed = 0; 
	bi.biClrImportant = 0; 
	
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31)/32) * 4 * Bitmap.bmHeight ; 
	
	//为位图内容分配内存 
	hDib = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER)); 
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
	*lpbi = bi; 
	
	// 处理调色板 
	hPal = GetStockObject(DEFAULT_PALETTE); 
	if (hPal) 
	{ 
		hDC = ::GetDC(NULL); 
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
		RealizePalette(hDC); 
	} 
	
	// 获取该调色板下新的像素值 
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, 
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
		+dwPaletteSize, 
		(LPBITMAPINFO ) 
		lpbi, DIB_RGB_COLORS); 
	
	//恢复调色板 
	if (hOldPal) 
	{ 
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
		RealizePalette(hDC); 
		::ReleaseDC(NULL, hDC); 
	} 
	
	//创建位图文件 
	fh = CreateFile(lpFileName, GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
	
	if (fh == INVALID_HANDLE_VALUE) 
		return FALSE; 
	
	// 设置位图文件头 
	bmfHdr.bfType = 0x4D42; // "BM" 
	dwDIBSize = sizeof(BITMAPFILEHEADER) 
		+ sizeof(BITMAPINFOHEADER) 
		+ dwPaletteSize + dwBmBitsSize; 
	bmfHdr.bfSize = dwDIBSize; 
	bmfHdr.bfReserved1 = 0; 
	bmfHdr.bfReserved2 = 0; 
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) 
		+ (DWORD)sizeof(BITMAPINFOHEADER) 
		+ dwPaletteSize; 
	
	// 写入位图文件头 
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
	
	// 写入位图文件其余内容 
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); 
	
	//清除 
	GlobalUnlock(hDib); 
	GlobalFree(hDib); 
	CloseHandle(fh); 
	
	return TRUE; 
	
}

//2009.11.11 By Mei Gang
//To indetify the intersected triangles for each triangle
void CRealModelView::SearchIntrTrgls(CSurf * pSf0, CSurf * pSf1, double eps) 
{
	int i, j, k, m, mark;
	CArray<CTrgl*, CTrgl*>pTrs;
	CArray<CPair*, CPair*>pTrPair;
	CTrgl * pTr0; CTrgl * pTr1;

	int * tid0 = new int[pSf0->pTrs.GetSize()];
	int * tid1 = new int[pSf1->pTrs.GetSize()];
	for(i = 0; i < pSf0->pTrs.GetSize(); i++)
	{
		tid0[i] = i;//统一编号
		pTrs.Add(pSf0->pTrs.GetAt(i));
	}
	for(i = 0; i < pSf1->pTrs.GetSize(); i++) {
		tid1[i] = i + pSf0->pTrs.GetSize();
		pTrs.Add(pSf1->pTrs.GetAt(i));
	}

	//1--找第一对相交三角形对
	for(i = 0; i < pSf0->pTrs.GetSize(); i++) {
		for(j = 0; j < pSf1->pTrs.GetSize(); j++) {
			mark = IsTwoTrglIntr(pSf0->pTrs.GetAt(i), pSf1->pTrs.GetAt(j), myEps) ;
			if(mark == 1) {
				CPair * pTP = new CPair();
                pTP->bCheck = false;  pTP->FstID = tid0[i];  pTP->SedID = tid1[j];  //保存合并之后的ID
				pTrPair.Add(pTP);     break;
			}
		}
	    if(mark == 1)  break;
	}
	

	//2--找每个三角形的相邻三角形单元号。读入data时已经搞定！

	//3--找每个三角形的相交三角形，保存到每个三角形的iSfIntrTr
	//将所有面的三角形的指针保存到
	int Add = pSf0->pTrs.GetSize();
	int OldNumPair, NewNumPair, tid[2], tempid[2], fid, sid;
	bool bexist; //原始队列里是否已经存在该三角形对
 
	//加入三角形自身
	for(i = 0; i < pSf0->pTrs.GetSize(); i++)  pSf0->pTrs.GetAt(i)->iRgnPassTr.Add(i);
	for(i = 0; i < pSf1->pTrs.GetSize(); i++)  pSf1->pTrs.GetAt(i)->iRgnPassTr.Add(i);

	int iStep = 0;
    OldNumPair = pTrPair.GetSize();  NewNumPair = -1; //初始值
	while( OldNumPair != NewNumPair) {
		OldNumPair = pTrPair.GetSize(); //上次三角形对数目
		for(i = 0; i < OldNumPair; i++) { 
								
			// For output window
			++iStep;
			CString str;
			str.Format( _T("Searching Intersected Triangles...   Step = %d"), iStep);
			pFrame->AddStrToOutputWnd(1, str);
					
			if ( pTrPair.GetAt(i)->bCheck )  continue; //已经检查过，跳过
			tid[0] = pTrPair.GetAt(i)->FstID;  tid[1] = pTrPair.GetAt(i)->SedID;
			for(j = 0; j < pTrs.GetAt(tid[0])->iRgnPassTr.GetSize(); j++) { // A 的所有相邻三角形
				tempid[0] = pTrs.GetAt(tid[0])->iRgnPassTr.GetAt(j);
				pTr0 = pTrs.GetAt(tempid[0]);
				for(k = 0; k < pTrs.GetAt(tid[1])->iRgnPassTr.GetSize(); k++) { // B 的所有相邻三角形
					/*
					// For output window
					CString str;
					str = _T("Searching Intersected Triangles...");
					pFrame->AddStrToOutputWnd(1, str);
					*/

					tempid[1] = pTrs.GetAt(tid[1])->iRgnPassTr.GetAt(k) + Add; //一定要 + Add
					pTr1 = pTrs.GetAt(tempid[1]);
					if(IsTwoTrglIntr(pTr0, pTr1, myEps) == 1) { // intersect
						bexist = false;
						for(m = 0; m < pTrPair.GetSize(); m++) { //判断是否已经存在三角形对
							if( (tempid[0] == pTrPair.GetAt(m)->FstID && tempid[1] == pTrPair.GetAt(m)->SedID) ||
								(tempid[0] == pTrPair.GetAt(m)->SedID && tempid[1] == pTrPair.GetAt(m)->FstID)  ) {
								bexist = true;  break; 
							}
						}
						if( !bexist ) { //不存在，可以将三角形对添加入队列
							CPair * pTP = new CPair();
							pTP->bCheck = false;  pTP->FstID = tempid[0];  pTP->SedID = tempid[1];
							pTrPair.Add(pTP);
						}
					}
				}
			}
            pTrPair.GetAt(i)->bCheck = true; //已经搜索过
		}
		NewNumPair = pTrPair.GetSize(); //最新三角形数目,更新
	}

	//4：根据三角形对，找出每个三角形的相交三角形
	for(i = 0; i < pTrPair.GetSize(); i++) {
		fid = pTrPair.GetAt(i)->FstID;  sid = pTrPair.GetAt(i)->SedID;
		pTrs.GetAt(fid)->iRgnIntrTr.Add(sid);
		pTrs.GetAt(sid)->iRgnIntrTr.Add(fid);
	}

	for(i = pTrPair.GetUpperBound(); i > -1; i--) {
		delete pTrPair.GetAt(i);  pTrPair.RemoveAt(i);
	}
	delete tid0;  delete tid1;

}

// 2013.4.22 RAPID Package http://gamma.cs.unc.edu/OBB/
// Search pairs of intersected triangles based on OBB Tree
void CRealModelView::SearchInsectPair2D_OBBTree(CSurf * pSf0, CSurf * pSf1, double eps) 
{
#ifndef EGC
	int i;
	CTrgl *ta;

	// Construct
    RAPID_model *b1 = new RAPID_model;
    RAPID_model *b2 = new RAPID_model;

	// Add triangles
	double p0[3], p1[3], p2[3];

	//// surface 0 : model-1 Projected onto plane !
	b1->BeginModel();
	for(i = 0; i < pSf0->pTrs.GetSize(); i++) {
		ta = pSf0->pTrs.GetAt(i);
		p0[0] = ta->x[0];  p0[1] = ta->y[0];  p0[2] = 0.0; // p0[2] = ta->z[0];
		p1[0] = ta->x[1];  p1[1] = ta->y[1];  p1[2] = 0.0; // p1[2] = ta->z[1];
		p2[0] = ta->x[2];  p2[1] = ta->y[2];  p2[2] = 0.0; // p2[2] = ta->z[2];
		b1->AddTri(p0, p1, p2, i);
	}
	b1->EndModel();

	//// surface 1 : model-2 Projected onto plane !
	b2->BeginModel();
	for(i = 0; i < pSf1->pTrs.GetSize(); i++) {
		ta = pSf1->pTrs.GetAt(i);
		p0[0] = ta->x[0];  p0[1] = ta->y[0];  p0[2] = 0.0; // p0[2] = ta->z[0];
		p1[0] = ta->x[1];  p1[1] = ta->y[1];  p1[2] = 0.0; // p1[2] = ta->z[1];
		p2[0] = ta->x[2];  p2[1] = ta->y[2];  p2[2] = 0.0; // p2[2] = ta->z[2];
		b2->AddTri(p0, p1, p2, i);
	}
	b2->EndModel();

	// this placement causes them to overlap a large amount.
    double R1[3][3], R2[3][3], T1[3], T2[3];
  
    R1[0][0] = R1[1][1] = R1[2][2] = 1.0; // No scale
    R1[0][1] = R1[1][0] = R1[2][0] = 0.0;
    R1[0][2] = R1[1][2] = R1[2][1] = 0.0;

    R2[0][0] = R2[1][1] = R2[2][2] = 1.0;
    R2[0][1] = R2[1][0] = R2[2][0] = 0.0;
    R2[0][2] = R2[1][2] = R2[2][1] = 0.0;
  
    T1[0] = 0.0;  T1[1] = 0.0; T1[2] = 0.0; // No transform
    T2[0] = 0.0;  T2[1] = 0.0; T2[2] = 0.0;

    // now we can perform a collision query:
    RAPID_Collide(R1, T1, b1, R2, T2, b2, RAPID_ALL_CONTACTS);

	// 根据三角形对，找出每个三角形的相交三角形
	// Save intersected triangle-pair
	int id1, id2;
	for(i = 0; i < RAPID_num_contacts; i++) {
		id1 = RAPID_contact[i].id1;
		id2 = RAPID_contact[i].id2;
		pSf0->pTrs.GetAt(id1)->iRgnIntrTr.Add(id2);
		pSf1->pTrs.GetAt(id2)->iRgnIntrTr.Add(id1);
	}

	delete b1;  delete b2;

	CString str;
	str.Format(_T("Intersect Pairs = %d \n"), RAPID_num_contacts );
	AfxMessageBox(str);
	// exit(0);

#else
	int i;
	CTrgl *ta;

	// Construct
    RAPID_model *b1 = new RAPID_model;
    RAPID_model *b2 = new RAPID_model;

	// Add triangles
	float p0[3], p1[3], p2[3];

	//// blk : model-1
	b1->BeginModel();
	for(i = 0; i < pSf0->pTrs.GetSize(); i++) {
		ta = pSf0->pTrs.GetAt(i);
		p0[0] = ta->x[0].floatValue();  p0[1] = ta->y[0].floatValue();  p0[2] = ta->z[0].floatValue();
		p1[0] = ta->x[1].floatValue();  p1[1] = ta->y[1].floatValue();  p1[2] = ta->z[1].floatValue();
		p2[0] = ta->x[2].floatValue();  p2[1] = ta->y[2].floatValue();  p2[2] = ta->z[2].floatValue();
		b1->AddTriEGC(p0, p1, p2, i);  // EGC
	}
	b1->EndModel();

	//// surf : model-2
	b2->BeginModel();
	for(i = 0; i < pSf1->pTrs.GetSize(); i++) {
		ta = pSf1->pTrs.GetAt(i);
		p0[0] = ta->x[0].floatValue();  p0[1] = ta->y[0].floatValue();  p0[2] = ta->z[0].floatValue();
		p1[0] = ta->x[1].floatValue();  p1[1] = ta->y[1].floatValue();  p1[2] = ta->z[1].floatValue();
		p2[0] = ta->x[2].floatValue();  p2[1] = ta->y[2].floatValue();  p2[2] = ta->z[2].floatValue();
		b2->AddTriEGC(p0, p1, p2, i);
	}
	b2->EndModel();

	// this placement causes them to overlap a large amount.
    float R1[3][3], R2[3][3], T1[3], T2[3];
  
    R1[0][0] = R1[1][1] = R1[2][2] = 1.0; // No scale
    R1[0][1] = R1[1][0] = R1[2][0] = 0.0;
    R1[0][2] = R1[1][2] = R1[2][1] = 0.0;

    R2[0][0] = R2[1][1] = R2[2][2] = 1.0;
    R2[0][1] = R2[1][0] = R2[2][0] = 0.0;
    R2[0][2] = R2[1][2] = R2[2][1] = 0.0;
  
    T1[0] = 0.0;  T1[1] = 0.0; T1[2] = 0.0; // No transform
    T2[0] = 0.0;  T2[1] = 0.0; T2[2] = 0.0;

    // now we can perform a collision query:
    RAPID_CollideEGC(R1, T1, b1, R2, T2, b2, RAPID_ALL_CONTACTS);

	// 根据三角形对，找出每个三角形的相交三角形
	// Save intersected triangle-pair
	int id1, id2;
	for(i = 0; i < RAPID_num_contacts; i++) {
		id1 = RAPID_contact[i].id1;
		id2 = RAPID_contact[i].id2;
		pSf0->pTrs.GetAt(id1)->iRgnIntrTr.Add(id2);
		pSf1->pTrs.GetAt(id2)->iRgnIntrTr.Add(id1);
	}

	delete b1;  delete b2;

	CString str;
	str.Format(_T("Intersect Pairs = %d \n"), RAPID_num_contacts );
	AfxMessageBox(str);
	// exit(0);
	
#endif

}

//2009.11.09 By Mei Gang
//To test whether two triangles intersect
//reurn value:
//0-- do not intersect;  1-- intersect
int CRealModelView::IsTwoTrglIntr(CTrgl * pTr0, CTrgl * pTr1, double eps) 
{
	int i, j;
	double sx, sy, ex, ey, xn, yn, sum;
	double delta = eps;
	int NumPosi = 0,  NumZero = 0,  NumNega = 0;
	
	//以pTr0为基础，判断pTr1
	for(i = 0; i < 3; i++) {
		if( i == 2) { sx = pTr0->x[2];  sy = pTr0->y[2];  ex = pTr0->x[0];  ey = pTr0->y[0];}
		else { sx = pTr0->x[i];  sy = pTr0->y[i];  ex = pTr0->x[i+1];  ey = pTr0->y[i+1];}
		NumPosi = 0;  NumZero = 0;  NumNega = 0;
		for(j = 0; j < 3; j++) {
			xn = pTr1->x[j];  yn = pTr1->y[j];
			sum = (xn - sx) * (ey - sy) -  (ex -sx) * (yn - sy);
			if(sum > delta)  NumPosi++;
			else if(sum < -1.0 * delta)  NumNega++;
			else  NumZero++;
		}
		if( NumPosi == 3 )  return 0;
	}
	
	//以pTr1为基础，判断pTr0
	for(i = 0; i < 3; i++) {
		if( i == 2) { sx = pTr1->x[2];  sy = pTr1->y[2];  ex = pTr1->x[0];  ey = pTr1->y[0];}
		else { sx = pTr1->x[i];  sy = pTr1->y[i];  ex = pTr1->x[i+1];  ey = pTr1->y[i+1];}
		NumPosi = 0;  NumZero = 0;  NumNega = 0;
		for(j = 0; j < 3; j++) {
			xn = pTr0->x[j];  yn = pTr0->y[j];
			sum = (xn - sx) * (ey - sy) -  (ex -sx) * (yn - sy);
			if(sum > delta)  NumPosi++;
			else if(sum < -1.0 * delta)  NumNega++;
			else  NumZero++;
		}
		if( NumPosi == 3 )  return 0;
	}
	
	return 1;

}

// last modified on 2011.11.22
// intersection of an arbitrary polygon with another one
// assume the first point and the last point of loop are the same one
// Important: assume there is only one intersection part. normally, there are more than 1
bool CRealModelView::IntrOfPolygonAndPolygon(CLoop * plp0, CLoop * plp1, CLoop * pIntr, double eps)
{
	int i, j, k;
	double tx, ty, tz;
	CNode * p0, * d0, * p1, * d1, * i0, * i1;
	bool * b0 = new bool[plp0->pNds.GetSize()]; // classify
	bool * b1 = new bool[plp1->pNds.GetSize()];
	int * n0 = new int [plp0->pNds.GetSize()]; // count
	int * n1 = new int [plp1->pNds.GetSize()];

	// very special case : two polygons are same
	bool bSame, bFind;
	bSame = true;
	for(i = 0; i < plp0->pNds.GetSize() - 1; i++) {
		bFind = false;
		for(j = 0; j < plp1->pNds.GetSize() - 1; j++) {
			if(IsSameNode(plp0->pNds.GetAt(i), plp1->pNds.GetAt(j), eps)) {
				bFind = true;  break;
			}
		}
		if(bFind == false) {
			bSame = false;  break;
		}
	}
	if(bSame == true && plp0->pNds.GetSize() == plp1->pNds.GetSize() ) { 
		pIntr->pNds.RemoveAll();
		for(i = 0; i < plp0->pNds.GetSize() - 1; i++) { // make sure that the first and last nodes are not the same
			tx = plp0->pNds.GetAt(i)->x;
			ty = plp0->pNds.GetAt(i)->y;
			tz = plp0->pNds.GetAt(i)->z;
			pIntr->pNds.Add(new CNode(i, tx, ty, tz));
		}
		return true;
	}

	// Other cases
	CArray<CEdge*, CEdge*>m_aEgs;
	CArray<CPair*, CPair*>pairs;
	for(i = 0; i < plp0->pNds.GetSize() - 1; i++) { // points of plp0 in plp1
		b0[i] = false;  n0[i] = 0;
		if(IsNdInPolygonNew(plp0->pNds.GetAt(i), plp1, eps) == 0 )  b0[i] = true;
	}
	for(i = 0; i < plp1->pNds.GetSize() - 1; i++) { // points of plp1 in plp0
		b1[i] = false;  n1[i] = 0;
		if(IsNdInPolygonNew(plp1->pNds.GetAt(i), plp0, eps) == 0 )  b1[i] = true;
	}

	// special case: one polygon is in the other 
	bool bInside = true;
	for(i = 0; i < plp0->pNds.GetSize(); i++) {
		bInside = bInside && b0[i];
	}
	if(bInside == true) { // loop plp0 is in loop plp1
		pIntr->pNds.RemoveAll();
		for(i = 0; i < plp0->pNds.GetSize() - 1; i++) { // make sure that the first and last nodes are not the same
			tx = plp0->pNds.GetAt(i)->x;
			ty = plp0->pNds.GetAt(i)->y;
			tz = plp0->pNds.GetAt(i)->z;
			pIntr->pNds.Add(new CNode(i, tx, ty, tz));
		}
		return true;
	}
	bInside = true;
	for(i = 0; i < plp1->pNds.GetSize(); i++) {
		bInside = bInside && b1[i];
	}
	if(bInside == true) { // loop plp1 is in loop plp0
		for(i = 0; i < plp1->pNds.GetSize() - 1; i++) { // make sure that the first and last nodes are not the same
			tx = plp1->pNds.GetAt(i)->x;
			ty = plp1->pNds.GetAt(i)->y;
			tz = plp1->pNds.GetAt(i)->z;
			pIntr->pNds.Add(new CNode(i, tx, ty, tz));
		}
		return true;
	}
	
	// common case
	for(i = 0; i < plp0->pNds.GetSize() - 1; i++) {
		if( b0[i] && b0[i+1] ) { // segment is in the other loop
			CEdge * pEg = new CEdge();
			pEg->fstNode = plp0->pNds.GetAt(i);
			pEg->lstNode = plp0->pNds.GetAt(i+1);
			m_aEgs.Add( pEg );
		} 
		if( b0[i] && !b0[i+1] || !b0[i] && b0[i+1] ) { // one point is in the other loop while the ohter point is not
			p0 = plp0->pNds.GetAt(i);
			tx = plp0->pNds.GetAt(i+1)->x - plp0->pNds.GetAt(i)->x;
			ty = plp0->pNds.GetAt(i+1)->y - plp0->pNds.GetAt(i)->y;
			tz = plp0->pNds.GetAt(i+1)->z - plp0->pNds.GetAt(i)->z; 
			
			d0 = new CNode(i+1, tx, ty, tz);
			for(j = 0; j < plp1->pNds.GetSize() - 1; j++) {
				p1 = plp1->pNds.GetAt(j);
				tx = plp1->pNds.GetAt(j+1)->x - plp1->pNds.GetAt(j)->x;
				ty = plp1->pNds.GetAt(j+1)->y - plp1->pNds.GetAt(j)->y;
				tz = plp1->pNds.GetAt(j+1)->z - plp1->pNds.GetAt(j)->z;
				 
				d1 = new CNode(j+1, tx, ty, tz);
                i0 = new CNode();  i1 = new CNode();
				if( IntrSegment2Segment2D(p0, d0, p1, d1, i0, i1, myEps) == 1) {
					n1[j] += 1;
					CPair * pair = new CPair();
					pair->FstID = i;  pair->SedID = j;
					pairs.Add(pair);

					if( b0[i] ) {
						CEdge * pEg = new CEdge();
						pEg->fstNode = plp0->pNds.GetAt(i);
						pEg->lstNode = i0;
						m_aEgs.Add( pEg );
					//	break;
					}
					else {
						CEdge * pEg = new CEdge();
						pEg->fstNode = i0;
						pEg->lstNode = plp0->pNds.GetAt(i+1);
						m_aEgs.Add( pEg );
					//	break;
					}
				}
				delete d1; 
			}
			delete d0; 
		}
	}
	for(i = 0; i < plp1->pNds.GetSize() - 1; i++) {
		if( b1[i] && b1[i+1] ) {
			CEdge * pEg = new CEdge();
			pEg->fstNode = plp1->pNds.GetAt(i);
			pEg->lstNode = plp1->pNds.GetAt(i+1);
			m_aEgs.Add( pEg );
		} 
		if( b1[i] && !b1[i+1] || !b1[i] && b1[i+1] ) {
			p0 = plp1->pNds.GetAt(i);
			tx = plp1->pNds.GetAt(i+1)->x - plp1->pNds.GetAt(i)->x;
			ty = plp1->pNds.GetAt(i+1)->y - plp1->pNds.GetAt(i)->y;
			tz = plp1->pNds.GetAt(i+1)->z - plp1->pNds.GetAt(i)->z;
			
			d0 = new CNode(i+1, tx, ty, tz);
			for(j = 0; j < plp0->pNds.GetSize() - 1; j++) {
				p1 = plp0->pNds.GetAt(j);
				tx = plp0->pNds.GetAt(j+1)->x - plp0->pNds.GetAt(j)->x;
				ty = plp0->pNds.GetAt(j+1)->y - plp0->pNds.GetAt(j)->y;
				tz = plp0->pNds.GetAt(j+1)->z - plp0->pNds.GetAt(j)->z;
				 
				d1 = new CNode(j+1, tx, ty, tz);
				i0 = new CNode();  i1 = new CNode();
				if( IntrSegment2Segment2D(p0, d0, p1, d1, i0, i1, myEps) == 1) {
					n0[j] += 1;
					CPair * pair = new CPair();
					pair->FstID = j;  pair->SedID = i;
					pairs.Add(pair);

					if( b1[i] ) {
						CEdge * pEg = new CEdge();
						pEg->fstNode = plp1->pNds.GetAt(i);
						pEg->lstNode = i0;
						m_aEgs.Add( pEg );
					//	break;
					}
					else {
						CEdge * pEg = new CEdge();
						pEg->fstNode = i0;
						pEg->lstNode = plp1->pNds.GetAt(i+1);
						m_aEgs.Add( pEg );
					//	break;
					}
				}
				delete d1; 
			}
			delete d0;  
		}

	}
	
	// intersection line by one segment with other two. 一条边同两条边相交
	CArray<CNode*, CNode*>seg0;
	CArray<CNode*, CNode*>seg1;
	for(i = 0; i < plp0->pNds.GetSize()-1; i++) {
		if(n0[i] == 2) {
			for(j = 0; j < pairs.GetSize(); j++) {
				if(pairs.GetAt(j)->FstID == i) {
					p0 = plp0->pNds.GetAt(i);
					tx = plp0->pNds.GetAt(i+1)->x - plp0->pNds.GetAt(i)->x;
					ty = plp0->pNds.GetAt(i+1)->y - plp0->pNds.GetAt(i)->y;
					tz = plp0->pNds.GetAt(i+1)->z - plp0->pNds.GetAt(i)->z; 
					d0 = new CNode(i+1, tx, ty, tz);

					k = pairs.GetAt(j)->SedID;
					p1 = plp1->pNds.GetAt(k);
					tx = plp1->pNds.GetAt(k+1)->x - plp1->pNds.GetAt(k)->x;
					ty = plp1->pNds.GetAt(k+1)->y - plp1->pNds.GetAt(k)->y;
					tz = plp1->pNds.GetAt(k+1)->z - plp1->pNds.GetAt(k)->z;
					d1 = new CNode(k+1, tx, ty, tz);

					i0 = new CNode();  i1 = new CNode();
					IntrSegment2Segment2D(p0, d0, p1, d1, i0, i1, myEps);
					seg0.Add(i0);

					delete d0;  delete d1;
				}
			}
			break;
		}
	}
		
	for(i = 0; i < plp1->pNds.GetSize()-1; i++) {
		if(n1[i] == 2) {
			for(j = 0; j < pairs.GetSize(); j++) {
				if(pairs.GetAt(j)->SedID == i) {
					p0 = plp1->pNds.GetAt(i);
					tx = plp1->pNds.GetAt(i+1)->x - plp1->pNds.GetAt(i)->x;
					ty = plp1->pNds.GetAt(i+1)->y - plp1->pNds.GetAt(i)->y;
					tz = plp1->pNds.GetAt(i+1)->z - plp1->pNds.GetAt(i)->z;
					d0 = new CNode(i+1, tx, ty, tz);
					
					k = pairs.GetAt(j)->FstID;
					p1 = plp0->pNds.GetAt(k);
					tx = plp0->pNds.GetAt(k+1)->x - plp0->pNds.GetAt(k)->x;
					ty = plp0->pNds.GetAt(k+1)->y - plp0->pNds.GetAt(k)->y;
					tz = plp0->pNds.GetAt(k+1)->z - plp0->pNds.GetAt(k)->z;
					d1 = new CNode(k+1, tx, ty, tz);

					i0 = new CNode();  i1 = new CNode();
					IntrSegment2Segment2D(p0, d0, p1, d1, i0, i1, myEps);
					seg0.Add(i0);

					delete d0;  delete d1; 
				}
			}
			break;
		}
	}

	if(seg0.GetSize() == 2) { // add it into the edge set
		CEdge * eg = new CEdge();
		eg->fstNode = seg0.GetAt(0);  eg->lstNode = seg0.GetAt(1);
		m_aEgs.Add(eg);
	}
	if(seg1.GetSize() == 2) {
		CEdge * eg = new CEdge();
		eg->fstNode = seg1.GetAt(0);  eg->lstNode = seg1.GetAt(1);
		m_aEgs.Add(eg);
	}
	seg0.RemoveAll();  seg1.RemoveAll();

	delete b0;  delete b1;  delete n0;  delete n1;

	//form intersection loop
	for(i = m_aEgs.GetUpperBound(); i > -1; i--) {
		if(IsSameNode(m_aEgs.GetAt(i)->fstNode, m_aEgs.GetAt(i)->lstNode, myEps)) {
			delete m_aEgs.GetAt(i);  m_aEgs.RemoveAt(i);
		}
	}

	/*
	for(i = 0; i < m_aEgs.GetSize()-1; i++) {
		for(j = i+1; j < m_aEgs.GetSize(); j++) {
			if(IsSameNode(m_aEgs.GetAt(i)->fstNode, m_aEgs.GetAt(j)->fstNode, myEps) && 
				IsSameNode(m_aEgs.GetAt(i)->lstNode, m_aEgs.GetAt(j)->lstNode, myEps) ){
				 m_aEgs.RemoveAt(j);
			}
			if(IsSameNode(m_aEgs.GetAt(i)->fstNode, m_aEgs.GetAt(j)->lstNode, myEps) && 
				IsSameNode(m_aEgs.GetAt(i)->lstNode, m_aEgs.GetAt(j)->fstNode, myEps) ){
				 m_aEgs.RemoveAt(j);
			}
		}
	}
	*/
	
	if(m_aEgs.GetSize() < 3) {
	//	AfxMessageBox("Can't form a loop because edges are less than 3. \n Function : IntrOfPolygonAndPolygon().", MB_OK|MB_ICONSTOP);
		return false; // can not form a loop
	}
    /*
	pIntr->pNds.Add(m_aEgs.GetAt(0)->fstNode);
	pIntr->pNds.Add(m_aEgs.GetAt(0)->lstNode);
	while( !IsSameNode(pIntr->pNds.GetAt(0), pIntr->pNds.GetAt(pIntr->pNds.GetUpperBound())) ) {
		for( i = 1; i < m_aEgs.GetSize(); i++) {
			if(IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetUpperBound()), m_aEgs.GetAt(i)->fstNode)) {
				pIntr->pNds.Add(m_aEgs.GetAt(i)->lstNode);
			}
		}
	}
	*/
	
	//Form intersection face(cross intersection), use m_aEgs
	pIntr->pNds.RemoveAll();
	pIntr->pNds.Add(m_aEgs.GetAt(0)->fstNode);
	pIntr->pNds.Add(m_aEgs.GetAt(0)->lstNode);
	while( !IsSameNode(pIntr->pNds.GetAt(0), pIntr->pNds.GetAt(pIntr->pNds.GetUpperBound()), myEps) ) {
		for( i = 1; i < m_aEgs.GetSize(); i++) {
			if(IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 1), m_aEgs.GetAt(i)->fstNode, myEps)
				&&
			   !IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 2), m_aEgs.GetAt(i)->lstNode, myEps)) {
				pIntr->pNds.Add(m_aEgs.GetAt(i)->lstNode);
				break;
			}
			
			// sometimes, this must be used, in other times, this is not neccessary ! 2010.11.30
			if(IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 1), m_aEgs.GetAt(i)->lstNode, myEps)
				&&
			   !IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 2), m_aEgs.GetAt(i)->fstNode, myEps)) {
				pIntr->pNds.Add(m_aEgs.GetAt(i)->fstNode);
				break;
			}
			
		}
	}
	//Delete last node, very important!
	pIntr->pNds.RemoveAt(pIntr->pNds.GetSize() - 1);
	// Begin : copy, very important, 2010.11.30
	CArray<CNode*, CNode*> tmpNode;
	for(i = 0; i < pIntr->pNds.GetSize(); i++) {
		tmpNode.Add(pIntr->pNds.GetAt(i));
	}
	pIntr->pNds.RemoveAll();
	for(i = 0; i < tmpNode.GetSize(); i++) {
		tx = tmpNode.GetAt(i)->x;
		ty = tmpNode.GetAt(i)->y;
		tz = tmpNode.GetAt(i)->z;
		pIntr->pNds.Add(new CNode(i, tx, ty, tz));
	}
	tmpNode.RemoveAll();
	// End

	return true;
}

// last modified on 2011.12.2
// intersection of an convex polygon with another one
// assume the first point and the last point of loop are the same one
// Important: assume there is only one intersection part. normally, there are more than 1
bool CRealModelView::IntrOfConvexPolygonV2(CLoop * plp0, CLoop * plp1, CLoop * pIntr, double eps)
{
	int i, j, k, t, N;
	double tx, ty, tz;
	CNode * p0, * d0, * p1, * d1, * i0, * i1;
	bool bfind, bexist;

	// Copy data
	CLoop* tplp0 = new CLoop();
	CLoop* tplp1 = new CLoop();
	for(i = 0; i < plp0->pNds.GetSize(); i++) { 
		tx = plp0->pNds.GetAt(i)->x;
		ty = plp0->pNds.GetAt(i)->y;
		tz = plp0->pNds.GetAt(i)->z;
		tplp0->pNds.Add(new CNode(i, tx, ty, tz)); 
	}
	N = tplp0->pNds.GetUpperBound();
	if(IsSameNode(tplp0->pNds.GetAt(0), tplp0->pNds.GetAt(N), eps) == false) {
		tx = plp0->pNds.GetAt(0)->x;
		ty = plp0->pNds.GetAt(0)->y;
		tz = plp0->pNds.GetAt(0)->z;
		tplp0->pNds.Add(new CNode(0, tx, ty, tz)); 
	}
	for(i = 0; i < plp1->pNds.GetSize(); i++) { 
		tx = plp1->pNds.GetAt(i)->x;
		ty = plp1->pNds.GetAt(i)->y;
		tz = plp1->pNds.GetAt(i)->z;
		tplp1->pNds.Add(new CNode(i, tx, ty, tz)); 
	}
	N = tplp1->pNds.GetUpperBound();
	if(IsSameNode(tplp1->pNds.GetAt(0), tplp1->pNds.GetAt(N), eps) == false) {
		tx = plp1->pNds.GetAt(0)->x;
		ty = plp1->pNds.GetAt(0)->y;
		tz = plp1->pNds.GetAt(0)->z;
		tplp1->pNds.Add(new CNode(0, tx, ty, tz)); 
	}

	// check whethr polygon is in 2D
	for(i = 0; i < tplp0->pNds.GetSize(); i++) {
		tz = tplp0->pNds.GetAt(i)->z;
		if(!(fabs(tz) < eps)) {
			tplp0->pNds.GetAt(i)->z = 0.00;
		//	AfxMessageBox(_T("Polygon is not in 2D !"));
		}
	}
	for(i = 0; i < tplp1->pNds.GetSize(); i++) {
		tz = tplp1->pNds.GetAt(i)->z;
		if(!(fabs(tz) < eps)) {
			tplp1->pNds.GetAt(i)->z = 0.00;
		//	AfxMessageBox(_T("Polygon is not in 2D !"));
		}
	}

	// Boundary box test for new loops
	// Only can be putted here
	CNode * aLB = new CNode();  CNode * aTR = new CNode();
	CNode * bLB = new CNode();  CNode * bTR = new CNode();
	CreatBoundaryBox(tplp0, aLB, aTR);
	CreatBoundaryBox(tplp1, bLB, bTR);
	bool bIntersect = BoundaryBoxIntrTest(aLB, aTR, bLB, bTR);
	delete aLB;  delete aTR;  delete bLB;  delete bTR;
	if(bIntersect == false) {
		tplp0->Clear();  tplp1->Clear();
		return false;
	}
	
	// all valid points
	CArray<CNode*, CNode*> pValid;

	// case 1 : find the non-outside points
	for(i = 0; i < tplp0->pNds.GetSize() - 1; i++) { // points of tplp0 in tplp1
		if(IsNdInPolygon(tplp0->pNds.GetAt(i), tplp1, eps) == true) {
			tx = tplp0->pNds.GetAt(i)->x;
			ty = tplp0->pNds.GetAt(i)->y;
			tz = tplp0->pNds.GetAt(i)->z;
			pValid.Add(new CNode(i, tx, ty, tz));
		}  
	}
	for(i = 0; i < tplp1->pNds.GetSize() - 1; i++) { // points of tplp1 in tplp0
		if(IsNdInPolygon(tplp1->pNds.GetAt(i), tplp0, eps) == true) {
			tx = tplp1->pNds.GetAt(i)->x;
			ty = tplp1->pNds.GetAt(i)->y;
			tz = tplp1->pNds.GetAt(i)->z;
			pValid.Add(new CNode(i, tx, ty, tz));
		}
	}

	// case 2 : find the intesection point
	for(i = 0; i < tplp0->pNds.GetSize() - 1; i++) {
			p0 = tplp0->pNds.GetAt(i);
			tx = tplp0->pNds.GetAt(i+1)->x - tplp0->pNds.GetAt(i)->x;
			ty = tplp0->pNds.GetAt(i+1)->y - tplp0->pNds.GetAt(i)->y;
			tz = tplp0->pNds.GetAt(i+1)->z - tplp0->pNds.GetAt(i)->z; 
			d0 = new CNode(i+1, tx, ty, tz);

			for(j = 0; j < tplp1->pNds.GetSize() - 1; j++) {
				p1 = tplp1->pNds.GetAt(j);
				tx = tplp1->pNds.GetAt(j+1)->x - tplp1->pNds.GetAt(j)->x;
				ty = tplp1->pNds.GetAt(j+1)->y - tplp1->pNds.GetAt(j)->y;
				tz = tplp1->pNds.GetAt(j+1)->z - tplp1->pNds.GetAt(j)->z;
				d1 = new CNode(j+1, tx, ty, tz);

                i0 = new CNode();  i1 = new CNode();
				if( IntrSegment2Segment2D(p0, d0, p1, d1, i0, i1, myEps) == 1) {
					pValid.Add(i0);
					delete d1;  delete i1;
				}
				else {
					delete d1;  delete i0;  delete i1;
				}
				
			}
			delete d0; 
	}

	// check first time
	if(pValid.GetSize() < 3) {
		for(t = pValid.GetUpperBound(); t > -1; t--) {
			delete pValid.GetAt(t);  pValid.RemoveAt(t);
		}
		tplp0->Clear();  tplp1->Clear();
		return false;
	}

	// delete same points
	CArray<CNode*, CNode*> pTmp;
	tx = pValid.GetAt(0)->x;
	ty = pValid.GetAt(0)->y;
	tz = pValid.GetAt(0)->z;
	pTmp.Add(new CNode(0, tx, ty, tz));
	for(i = 1; i < pValid.GetSize(); i++) {
		bfind = false;
		for(j = 0; j < pTmp.GetSize(); j++) {
			if(IsSameNode(pValid.GetAt(i), pTmp.GetAt(j), eps) == true) {
				bfind = true;  break;
			}
		}
		if(bfind == false) {
			tx = pValid.GetAt(i)->x;
			ty = pValid.GetAt(i)->y;
			tz = pValid.GetAt(i)->z;
			pTmp.Add(new CNode(i, tx, ty, tz));
		}
	}
	for(i = pValid.GetUpperBound(); i > -1; i--) {
		delete pValid.GetAt(i);  pValid.RemoveAt(i);
	}
	pValid.RemoveAll();
	for(i = 0; i < pTmp.GetSize(); i++) {
		pValid.Add(pTmp.GetAt(i));
	}
	pTmp.RemoveAll();

	/*
	// for output window
	for(i = 0; i < pValid.GetSize(); i++) {
		tx = pValid.GetAt(i)->x;
		ty = pValid.GetAt(i)->y;
		tz = pValid.GetAt(i)->z;
		CString str;
		str.Format(_T("X = %f  Y = %f  Z = %f"), tx, ty, tz);
		pFrame->AddStrToOutputWnd(1, str);
	}
	*/

	// check intersection
	//// no intersection, or a point, a segment, in short no area!
	if(pValid.GetSize() < 3) { // check second time
		for(t = pValid.GetUpperBound(); t > -1; t--) {
			delete pValid.GetAt(t);  pValid.RemoveAt(t);
		}
		tplp0->Clear();  tplp1->Clear();
		return false;
	}

	//// form area intersection
	CArray<CEdge*, CEdge*> m_aEgs;
	CArray<CEdge*, CEdge*> tEgs;
	for(i = 0; i < tplp0->pNds.GetSize() - 1; i++) {
		CEdge * pEg = new CEdge();
		pEg->fstNode = tplp0->pNds.GetAt(i);
		pEg->lstNode = tplp0->pNds.GetAt(i+1);
		tEgs.Add( pEg );
	} 
	for(i = 0; i < tplp1->pNds.GetSize() - 1; i++) {
		CEdge * pEg = new CEdge();
		pEg->fstNode = tplp1->pNds.GetAt(i);
		pEg->lstNode = tplp1->pNds.GetAt(i+1);
		tEgs.Add( pEg );
	} 

	for(i = 0; i < pValid.GetSize()-1; i++) {
		CNode* pa = pValid.GetAt(i);
		for(j = i+1; j < pValid.GetSize(); j++) {
			CNode* pb = pValid.GetAt(j);
			for(k = 0; k < tEgs.GetSize(); k++) {
				CEdge* eg = tEgs.GetAt(k);
				if(IsPointOnSegment(eg->fstNode, eg->lstNode, pa, myEps) && 
				   IsPointOnSegment(eg->fstNode, eg->lstNode, pb, myEps) ) {
					   CEdge * edge = new CEdge();
					   edge->fstNode = pa;  edge->lstNode = pb;
					   m_aEgs.Add(edge);
					   break;
				}
			}
		}
	}
	
	//form intersection loop
	if(m_aEgs.GetSize() < 3) {
		for(t = m_aEgs.GetUpperBound(); t > -1; t--) {
			m_aEgs.GetAt(t)->Clear();
			delete m_aEgs.GetAt(t);  m_aEgs.RemoveAt(t);
		}
		for(t = pValid.GetUpperBound(); t > -1; t--) {
			delete pValid.GetAt(t);  pValid.RemoveAt(t);
		}
		tplp0->Clear();  tplp1->Clear();
		return false; // can not form a loop
	}

	for(i = m_aEgs.GetUpperBound(); i > -1; i--) {
		if(IsSameNode(m_aEgs.GetAt(i)->fstNode, m_aEgs.GetAt(i)->lstNode, myEps)) {
			delete m_aEgs.GetAt(i);  m_aEgs.RemoveAt(i);
		}
	}

	if(m_aEgs.GetSize() < 3) { // can not form a loop
		for(t = m_aEgs.GetUpperBound(); t > -1; t--) {
			m_aEgs.GetAt(t)->Clear();
			delete m_aEgs.GetAt(t);  m_aEgs.RemoveAt(t);
		}
		for(t = pValid.GetUpperBound(); t > -1; t--) {
			delete pValid.GetAt(t);  pValid.RemoveAt(t);
		}
		tplp0->Clear();  tplp1->Clear();
		return false; 
	}

	//Form intersection face(cross intersection), use m_aEgs
	pIntr->pNds.RemoveAll();
	pIntr->pNds.Add(m_aEgs.GetAt(0)->fstNode);
	pIntr->pNds.Add(m_aEgs.GetAt(0)->lstNode);
	while( !IsSameNode(pIntr->pNds.GetAt(0), pIntr->pNds.GetAt(pIntr->pNds.GetUpperBound()), myEps) ) {
		for( i = 1; i < m_aEgs.GetSize(); i++) {
			if(IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 1), m_aEgs.GetAt(i)->fstNode, myEps)
				&&
			   !IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 2), m_aEgs.GetAt(i)->lstNode, myEps)) {
				pIntr->pNds.Add(m_aEgs.GetAt(i)->lstNode);
				break;
			}
			
			// sometimes, this must be used, in other times, this is not neccessary ! 2010.11.30
			if(IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 1), m_aEgs.GetAt(i)->lstNode, myEps)
				&&
			   !IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 2), m_aEgs.GetAt(i)->fstNode, myEps)) {
				pIntr->pNds.Add(m_aEgs.GetAt(i)->fstNode);
				break;
			}
			
		}
	}
	//Delete last node, very important!
	pIntr->pNds.RemoveAt(pIntr->pNds.GetSize() - 1);
	// Begin : copy, very important, 2010.11.30
	CArray<CNode*, CNode*> tmpNode;
	for(i = 0; i < pIntr->pNds.GetSize(); i++) {
		tmpNode.Add(pIntr->pNds.GetAt(i));
	}
	pIntr->pNds.RemoveAll();
	for(i = 0; i < tmpNode.GetSize(); i++) {
		tx = tmpNode.GetAt(i)->x;
		ty = tmpNode.GetAt(i)->y;
		tz = tmpNode.GetAt(i)->z;
		pIntr->pNds.Add(new CNode(i, tx, ty, tz));
	}
	tmpNode.RemoveAll();
	// End

	tplp0->Clear();  tplp1->Clear();

	return true;
}

bool CRealModelView::IsNdInPolygon(CNode *pNd, CLoop *pLp, double eps)
{
	int i;
	double sx, sy, ex, ey, sum;
	double delta = eps;
	double xn = pNd->x;
	double yn = pNd->y;

	for(i = 0; i < pLp->pNds.GetSize() - 1; i++) {
		sx = pLp->pNds.GetAt(i)->x;    sy = pLp->pNds.GetAt(i)->y; 
		ex = pLp->pNds.GetAt(i+1)->x;  ey = pLp->pNds.GetAt(i+1)->y;
		sum = (xn - sx) * (ey - sy) -  (ex -sx) * (yn - sy);
		if(sum > delta) {
			return false;  break; // outside
		}
	}

	return true; // inside, including on edge or on vertex
}

// 2011.5.30
// 0: inside;  1: on edge or vertex;  2: outside
int CRealModelView::IsNdInPolygonNew(CNode *pNd, CLoop *pLp, double eps)
{
	int i;
	double sx, sy, ex, ey, sum;
	double delta = eps;
	double xn = pNd->x;
	double yn = pNd->y;

	for(i = 0; i < pLp->pNds.GetSize() - 1; i++) {
		sx = pLp->pNds.GetAt(i)->x;  sy = pLp->pNds.GetAt(i)->y; 
		ex = pLp->pNds.GetAt(i+1)->x;  ey = pLp->pNds.GetAt(i+1)->y;
		sum = (xn - sx) * (ey - sy) -  (ex -sx) * (yn - sy);
		if(sum > delta) {
			return 2;  break; // outside
		}
	}

	for(i = 0; i < pLp->pNds.GetSize() - 1; i++) {
		sx = pLp->pNds.GetAt(i)->x;  sy = pLp->pNds.GetAt(i)->y; 
		ex = pLp->pNds.GetAt(i+1)->x;  ey = pLp->pNds.GetAt(i+1)->y;
		if( IsPointOnSegment(sx, sy, ex, ey, pNd->x, pNd->y, myEps) == true ) {
			return 1;  break; // on boundary, including on edge or on vertex
		}
	}

	return 0; // inside
}

//To compute the intersecton of segment-segment
//Return value:
//0-Segments do not intersect;
//1-Segments intersect on a point;
//2-Segments intersect on a interval;
int CRealModelView::IntrSegment2Segment2D(CNode * p0, CNode * d0, CNode * p1, CNode * d1, CNode * i0, CNode * i1, double eps)
{
	CNode * E = new CNode(p1->x - p0->x, p1->y - p0->y);
	double kross = d0->x * d1->y - d0->y * d1->x;
	double sqrKross = kross * kross;
	double sqrLen0 = d0->x * d0->x + d0->y * d0->y;
	double sqrLen1 = d1->x * d1->x + d1->y * d1->y;
	if(sqrKross > sqrEpsilon * sqrLen0 * sqrLen1) {
		//lines of segments are not parallel
		double s = (E->x * d1->y - E->y * d1->x ) / kross;
		double t = (E->x * d0->y - E->y * d0->x ) / kross;
		if(-1*Epsilon < s && s < 1 + Epsilon && -1*Epsilon < t && t < 1 + Epsilon) {
			//intersection of lines is a point on each segment
			i0->x = p0->x + s * d0->x;    i0->y = p0->y + s * d0->y;    i0->z = 0.0;
			return 1;
		}
		else {
			//intersection of lines is not a point on segment p0 + s * d0
			//intersection of lines is not a point on segment p1 + t * d1
			return 0;
		}
	}
	
	else {
		//lines of the segments are parallel
		double sqrLenE = E->x * E->x + E->y * E->y;
		kross = E->x * d0->y - E->y * d0->x ;
		sqrKross = kross * kross;
		if(sqrKross > sqrEpsilon * sqrLen0 * sqrLenE) {
			// lines are different
			return 0;
		}
		else {
			//lines of the segments are the same. Need to test for overlap of segements
			double s0 = (d0->x * E->x + d0->y * E->y ) / sqrLen0;
			double s1 = s0 + (d0->x * d1->x + d0->y * d1->y ) / sqrLen0;
			double w[2], smin, smax;
			if(s0 > s1)  { smin = s1;  smax = s0; }
			else { smin = s0;  smax = s1; }
			int imax = FindIntersection(0.0, 1.0, smin, smax, w , myEps);
			
			if(imax == 1) { i0->x = p0->x + w[0] * d0->x;    i0->y = p0->y + w[0] * d0->y;    i0->z = 0.0;}
			if(imax == 2) { i0->x = p0->x + w[0] * d0->x;    i0->y = p0->y + w[0] * d0->y;    i0->z = 0.0;
			                i1->x = p0->x + w[1] * d0->x;    i1->y = p0->y + w[1] * d0->y;    i1->z = 0.0;}
			return imax;
		}
	}
	delete E;

}

//To find the overlap of two interval [u0, u1] and [v0, v1]
//2009.10.14  By Mei Gang at University of Freiburg in Germany
//return value:
//0: the intervals do not overlap
//1: the overlap is a point, which will be saved in w[0]
//2: the overlap is a segment, which will be saved in w[0] and w[1]
int CRealModelView::FindIntersection(double u0, double u1, double v0, double v1, double w[2], double eps)
{
	if(u1 < v0 || u0 > v1)    return 0;

	if(u1 > v0) {
		if(u0 < v1) {
			if(u0 < v0)  w[0] = v0;  else  w[0] = u0;
			if(u1 > v1)  w[1] = v1;  else  w[1] = u1;
			return 2;
		} else {
			//u0 == v1
			w[0] = u0;
			return 1;
		}
	} else {
		//u1 == v0
		w[0] =  u1;
		return 1;
	}
}

// Last Modified 2011.11.23
//judge if two nodes are same
// return TRUE if same, otherwise FALSE
// spere is smaller than cube and in it
bool CRealModelView::IsSameNode(CNode *nd1, CNode *nd2, double eps)
{
	// Check error
	bool bCube = fabs(nd1->x - nd2->x) < eps && 
		         fabs(nd1->y - nd2->y) < eps && 
		         fabs(nd1->z - nd2->z) < eps ;
	bool bSphere = sqrt( (nd1->x - nd2->x) * (nd1->x - nd2->x) + 
		                 (nd1->y - nd2->y) * (nd1->y - nd2->y) + 
		                 (nd1->z - nd2->z) * (nd1->z - nd2->z) ) < eps;

	// Case 1 : in sphere
	if(bSphere == true)  return true;

	// Case 2 : in cube while not in spere
	if(bSphere == false && bCube == true) {
		CString str;
		str.Format(_T("Warning :  IsSameNode() \n"));
		pFrame->AddStrToOutputWnd(1, str);
	//	AfxMessageBox(_T("Error: IsSameNode(CNode *nd1, CNode *nd2, double eps)"));
		return true;
	}

	// Case 3 : not in cube
	if(bCube == false)  return false;
	
}

bool CRealModelView::IsSameNode(double ax, double ay, double az, double bx, double by, double bz, double eps)
{
	// Check error
	bool bCube = fabs(ax - bx) < eps && fabs(ay - by) < eps && fabs(az - bz) < eps ;
	bool bSphere = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by) + (az - bz) * (az - bz) ) < eps;
		                 
	// Case 1 : in sphere
	if(bSphere == true)  return true;

	// Case 2 : in cube while not in spere
	if(bSphere == false && bCube == true) {
		CString str;
		str.Format(_T("Warning :  IsSameNode() \n"));
		pFrame->AddStrToOutputWnd(1, str);
	//	AfxMessageBox(_T("Error: IsSameNode(CNode *nd1, CNode *nd2, double eps)"));
		return true;
	}

	// Case 3 : not in cube
	if(bCube == false)  return false;
	
}

// Read old file format
void CRealModelView::On3DModelOpenSurfaceOld()
{
    pSurf = new CSurf(); //  must be a global varible
//	CRegion * pRgn = new CRegion(); // a local varible
	ReadDataSurfaceOld(pSurf);  // old file format
	m_aSurfs.Add(pSurf);
	
	// Creat color
	for(int i = 0; i < m_aSurfs.GetSize(); i++) {
		m_aSurfs.GetAt(i)->ID = i;
		CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	}
	// pCrt = new CCordnt();
    // pCrt = GetNewCordntSystem(pSurf);

	if(pCrt == NULL) {
	    pCrt = new CCordnt();
        pCrt = GetNewCordntSystem(pSurf);
	}

	int nSurf = m_aSurfs.GetSize();
	int * list = new int[nSurf];
	for(int j = 0; j < nSurf; j++){
		list[j] = j+1;
		pSurf = m_aSurfs.GetAt(j);
	//	CompileTinList(pRgn, pCrt, list[j], 1);   // only edges, old
	//	glCompileTinList(pSurf, pCrt, list[j], 0); // edge and elements, new
		glCompileTinListAnti(pSurf, pCrt, list[j], 0); // edge and elements, new
	}

	m_bOpenGL = 1;
	Invalidate(TRUE);
	delete[] list;
}

// Read new file format 
void CRealModelView::On3DModelOpenSurface()
{
	// TODO: Add your command handler code here
	pSurf = new CSurf(); //  must be a global varible
//	CRegion * pRgn = new CRegion(); // a local varible

#ifndef EGC
	ReadDataSurface(pSurf); // new format
#else
	ReadDataSurfaceEGC(pSurf); // for EGC
#endif

	m_aSurfs.Add(pSurf);
	
	// Creat color
	for(int i = 0; i < m_aSurfs.GetSize(); i++) {
		m_aSurfs.GetAt(i)->ID = i;
		CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
		CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	}
	// pCrt = new CCordnt();
    // pCrt = GetNewCordntSystem(pSurf);

//	CreateRGBColor(m_aSurfs.GetAt(0)->RGB, 3); // The best color

	if(pCrt == NULL) {
	    pCrt = new CCordnt();
        pCrt = GetNewCordntSystem(pSurf);
	}

	int nSurf = m_aSurfs.GetSize();
	int * list = new int[nSurf];
	for(int j = 0; j < nSurf; j++){
		list[j] = j+1;
		pSurf = m_aSurfs.GetAt(j);
//		glCompileTinList(pSurf, pCrt, list[j], 1); // edge and elements, new
		glCompileSurfaceMeshListBest(pSurf, pCrt, list[j]);
	}

	m_bOpenGL = 1;
	Invalidate(TRUE);
	delete[] list;

}

bool CRealModelView::IsTwoLpsSame(CLoop * plp1, CLoop * plp2) 
{
	int i, j;
	bool bexist = false;
	for(i = 0; i < plp1->iNds.GetSize(); i++) {
		bexist = false;
		for(j = 0; j < plp2->iNds.GetSize(); j++) {
			if(plp1->iNds.GetAt(i) == plp2->iNds.GetAt(j)) { 
				bexist = true; break;
			}
		}
		if(!bexist) return false;
	}
	return true;
}

// 2010.1.25 by Mei Gang at University of Freiburg, Germany.
// emerge or rearrange all groups, polyhedron, loop and node
void CRealModelView::EmergeBlockGroups()
{
	int i, j, k, t;
	bool bBox, bIntr;
	CLoop * pLp;
	double x, y, z;
	int id_lp, id_nd;
	
	for(i = m_aNds.GetUpperBound(); i > -1; i--) { // clear up
		m_aNds.GetAt(i)->Clear();
		delete m_aNds.GetAt(i);   m_aNds.RemoveAt(i); 
	}
	for(i = m_aLps.GetUpperBound(); i > -1; i--) {
		m_aLps.GetAt(i)->Clear();
		delete m_aLps.GetAt(i);   m_aLps.RemoveAt(i);
	}
	m_aNds.RemoveAll();  m_aLps.RemoveAll();
	
	CArray<CNode*, CNode*>pNds; 
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = 0; j < m_aPhds.GetAt(i)->pLps.GetSize(); j++) {
			for(k = 0; k < m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetSize(); k++) {
				x = m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->x;
				y = m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->y;
				z = m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->z;
				pNds.Add(new CNode(k, x, y, z));
			}
		}
	}

	//sort the nodes
	int cnt_1, idx; // int count

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; p[idx].y = pNds.GetAt(j)->y; p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll();  
	//sort nodes
    SortNode(p, cnt_1);

	//re-arrange nodes
	int* iID = new int[cnt_1];	int* nID = new int[cnt_1];
	for(i = 0; i < cnt_1; i++)iID[i] = p[i].ID;

	nID[0] = 0; idx = 0;
	pNds.Add(new CNode(idx, p[0].x, p[0].y, p[0].z)); 

	//save each node for one time
	bool boolexist;
	for(i = 1; i < cnt_1; i++){
		boolexist = false;
		for(j = 0; j < pNds.GetSize(); j++) {
			if(IsSameNode(&p[i], pNds.GetAt(j), myEps)) { 
				boolexist = true;  break; 
			}
		}
		if(!boolexist) pNds.Add(new CNode(pNds.GetSize(), p[i].x, p[i].y, p[i].z));
	}
	delete [] p;

	delete[] iID;  delete[] nID;

	//New node set
	for(i = 0; i < pNds.GetSize(); i++) {
		x = pNds.GetAt(i)->x;  y = pNds.GetAt(i)->y;  z = pNds.GetAt(i)->z;
		m_aNds.Add(new CNode(i, x, y, z));
	}
    
	//New face/loop set
	CArray<CLoop*, CLoop*>pLps; 
	CLoop * plp;
	bool bexist;
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = 0; j < m_aPhds.GetAt(i)->pLps.GetSize(); j++) {
			for(k = 0; k < m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetSize(); k++) {
				x = m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->x;
				y = m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->y;
				z = m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->z;
				for(t = 0; t < pNds.GetSize(); t++) {
					if(IsSameNode(m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k),
						pNds.GetAt(t), myEps)) {
                        m_aPhds.GetAt(i)->pLps.GetAt(j)->iNds.Add(t);
					    break; 
					}
				}
			}
		}
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = 0; j < m_aPhds.GetAt(i)->pLps.GetSize(); j++) {
			plp = m_aPhds.GetAt(i)->pLps.GetAt(j);
			if(i == 0 && j == 0) { pLps.Add(plp); }
			else {
				bexist = false;
				for(k = 0; k < pLps.GetSize(); k++) {
					if( IsTwoLpsSame(plp, pLps.GetAt(k)) ) {  
						bexist = true;  break;
					}
				}
				if(!bexist) { pLps.Add(plp); }
			}
		}
	}
	for(i = 0; i < pLps.GetSize(); i++) {
		CLoop * lp = new CLoop();
		for(j = 0; j < pLps.GetAt(i)->iNds.GetSize(); j++) {
			if(j != 0 && pLps.GetAt(i)->iNds.GetAt(j) == lp->iNds.GetAt(0))  continue;  // delete the same last
			lp->iNds.Add(pLps.GetAt(i)->iNds.GetAt(j));
		}
		m_aLps.Add(lp);
	}

	//New polyhedron set
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		m_aPhds.GetAt(i)->iLps.RemoveAll();
		for(j = 0; j < m_aPhds.GetAt(i)->pLps.GetSize(); j++) {
			plp = m_aPhds.GetAt(i)->pLps.GetAt(j);
			for(k = 0; k < m_aLps.GetSize(); k++) {
				if( IsTwoLpsSame(plp, m_aLps.GetAt(k)) ) {  
					m_aPhds.GetAt(i)->iLps.Add(k);  break;
				}
			}
		}
	}
	/*
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = m_aPhds.GetAt(i)->pLps.GetSize() - 1; j > -1; j--) {
			delete m_aPhds.GetAt(i)->pLps.GetAt(j);  m_aPhds.GetAt(i)->pLps.RemoveAt(j);
		}
	}
    */
	//New block group set
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		m_aPhds.GetAt(i)->ID = i;
	}
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		m_aBlks.GetAt(i)->iPhds.RemoveAll();
		for(j = 0; j < m_aBlks.GetAt(i)->pPhs.GetSize(); j++) {
			m_aBlks.GetAt(i)->iPhds.Add(m_aBlks.GetAt(i)->pPhs.GetAt(j)->ID);
		}
	}

	//Remove the memory
	for(i = pNds.GetUpperBound(); i > -1; i--) {
		delete pNds.GetAt(i);   pNds.RemoveAt(i);
	}
	for(i = pLps.GetUpperBound(); i > -1; i--) {
		delete pLps.GetAt(i);   pLps.RemoveAt(i);
	}
	
}

// 2010.1.25 by Mei Gang at University of Freiburg, Germany.
// emerge or rearrange all groups, polyhedron, loop and node
void CRealModelView::EmergeBlockGroupsBest()
{
	int i, j, k, t;
	bool bBox, bIntr;
	CLoop * pLp;
	double x, y, z;
	int id = 0, id_lp, id_nd;
	
	for(i = m_aNds.GetUpperBound(); i > -1; i--) { // clear up
		m_aNds.GetAt(i)->Clear();
		delete m_aNds.GetAt(i);   m_aNds.RemoveAt(i); 
	}
	for(i = m_aLps.GetUpperBound(); i > -1; i--) {
		m_aLps.GetAt(i)->Clear();
		delete m_aLps.GetAt(i);   m_aLps.RemoveAt(i);
	}
	m_aNds.RemoveAll();  m_aLps.RemoveAll();
	
	CArray<CNode*, CNode*>pNds; 
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = 0; j < m_aPhds.GetAt(i)->pLps.GetSize(); j++) {
			for(k = 0; k < m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetSize(); k++) {
				x = m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->x;
				y = m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->y;
				z = m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->z;
				m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->ID = id;
				pNds.Add(new CNode(id++, x, y, z));
			}
		}
	}

	//sort the nodes
	int cnt_1, idx; // int count

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; 
		p[idx].y = pNds.GetAt(j)->y; 
		p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}


	//sort nodes
    SortNode(p, cnt_1);

	//save each node for one time
	int * newIDs = new int[cnt_1];

	id = 0;
	newIDs[0] = 0;

	for(i = 0; i < cnt_1 - 1; i++){
		j = i + 1;
		if( IsSameNode(p[i].x, p[i].y, p[i].z, p[j].x, p[j].y, p[j].z, myEps) == false ) { 
	//	if( IsSameNode(p[i].x, p[i].y, p[i].z, p[j].x, p[j].y, p[j].z, 0.01) == false ) { 
			id++;
		}
		newIDs[j] = id;
	}

	// Update : 1st step
	for(i = 0; i < cnt_1; i++) {
		id = p[i].ID;
		pNds.GetAt(id)->ID = newIDs[i];
	}

	// Update : 2nd step
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = 0; j < m_aPhds.GetAt(i)->pLps.GetSize(); j++) {
			m_aPhds.GetAt(i)->pLps.GetAt(j)->iNds.RemoveAll();
			for(k = 0; k < m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetSize(); k++) {
				id = m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->ID;
				m_aPhds.GetAt(i)->pLps.GetAt(j)->pNds.GetAt(k)->ID = pNds.GetAt(id)->ID;
				m_aPhds.GetAt(i)->pLps.GetAt(j)->iNds.Add(pNds.GetAt(id)->ID);
			}
		}
	}

	// Save : after updating , important 
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 

	i = 0;
	x = p[i].x;  y = p[i].y;  z = p[i].z;
	pNds.Add(new CNode(i, x, y, z));
	for(i = 1; i < cnt_1; i++) {
		if(newIDs[i] != newIDs[i-1]) {
			x = p[i].x;  y = p[i].y;  z = p[i].z;
	        pNds.Add(new CNode(i, x, y, z));
		}
	}

	//New node set
	for(i = 0; i < pNds.GetSize(); i++) {
		x = pNds.GetAt(i)->x;  y = pNds.GetAt(i)->y;  z = pNds.GetAt(i)->z;
		m_aNds.Add(new CNode(i, x, y, z));
	}
    
	//New face/loop set
	CArray<CLoop*, CLoop*>pLps; 
	CLoop * plp;
	bool bexist;

	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = 0; j < m_aPhds.GetAt(i)->pLps.GetSize(); j++) {
			plp = m_aPhds.GetAt(i)->pLps.GetAt(j);
			if(i == 0 && j == 0) { pLps.Add(plp); }
			else {
				bexist = false;
				for(k = 0; k < pLps.GetSize(); k++) {
					if( IsTwoLpsSame(plp, pLps.GetAt(k)) ) {  
						bexist = true;  break;
					}
				}
				if(!bexist) { pLps.Add(plp); }
			}
		}
	}
	for(i = 0; i < pLps.GetSize(); i++) {
		CLoop * lp = new CLoop();
		for(j = 0; j < pLps.GetAt(i)->iNds.GetSize(); j++) {
			if(j != 0 && pLps.GetAt(i)->iNds.GetAt(j) == lp->iNds.GetAt(0))  continue;  // delete the same last
			lp->iNds.Add(pLps.GetAt(i)->iNds.GetAt(j));
		}
		m_aLps.Add(lp);
	}

	//New polyhedron set
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		m_aPhds.GetAt(i)->iLps.RemoveAll();
		for(j = 0; j < m_aPhds.GetAt(i)->pLps.GetSize(); j++) {
			plp = m_aPhds.GetAt(i)->pLps.GetAt(j);
			for(k = 0; k < m_aLps.GetSize(); k++) {
				if( IsTwoLpsSame(plp, m_aLps.GetAt(k)) ) {  
					m_aPhds.GetAt(i)->iLps.Add(k);  break;
				}
			}
		}
	}
	/*
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = m_aPhds.GetAt(i)->pLps.GetSize() - 1; j > -1; j--) {
			delete m_aPhds.GetAt(i)->pLps.GetAt(j);  m_aPhds.GetAt(i)->pLps.RemoveAt(j);
		}
	}
    */
	//New block group set
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		m_aPhds.GetAt(i)->ID = i;
	}
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		m_aBlks.GetAt(i)->iPhds.RemoveAll();
		for(j = 0; j < m_aBlks.GetAt(i)->pPhs.GetSize(); j++) {
			m_aBlks.GetAt(i)->iPhds.Add(m_aBlks.GetAt(i)->pPhs.GetAt(j)->ID);
		}
	}

	//Remove the memory
	for(i = pNds.GetUpperBound(); i > -1; i--) {
		delete pNds.GetAt(i);   pNds.RemoveAt(i);
	}
	for(i = pLps.GetUpperBound(); i > -1; i--) {
		delete pLps.GetAt(i);   pLps.RemoveAt(i);
	}
	
}

// 2010.11.23 by Mei Gang at University of Freiburg, Germany.
void CRealModelView::EmergePolygonSurface(CSurf * surf)
{
	//sort the nodes
	int i, j, k, cnt_1, idx, count;

	//get number of nodes on surfs
	cnt_1 = 0; idx = 0;
	for(i = 0; i < surf->pLps.GetSize(); i++) {
		cnt_1 += surf->pLps.GetAt(i)->pNds.GetSize();
	}

	//copy nodes
	CNode* p = new CNode[cnt_1];
    for(i = 0; i < surf->pLps.GetSize(); i++){
		for(j = 0; j < surf->pLps.GetAt(i)->pNds.GetSize(); j++){
			p[idx].ID = idx;	
			p[idx].x = surf->pLps.GetAt(i)->pNds.GetAt(j)->x;	
			p[idx].y = surf->pLps.GetAt(i)->pNds.GetAt(j)->y;	
			p[idx].z = surf->pLps.GetAt(i)->pNds.GetAt(j)->z;	
			idx++;
		}
	}
	//sort nodes
    SortNode(p, cnt_1);

	//re-arrange nodes
	int* iID = new int[cnt_1];	int* nID = new int[cnt_1];
	for(i = 0; i < cnt_1; i++)  iID[i] = p[i].ID;

	for(i = surf->pNds.GetSize()-1; i > -1; i--) {
		delete surf->pNds.GetAt(i);  surf->pNds.RemoveAt(i);
	}
	nID[0] = 0; idx = 0;
	surf->pNds.Add(new CNode(idx, p[0].x, p[0].y, p[0].z)); 
	
	//re-create nodes
	for(i = 1; i < cnt_1; i++){
		if(!IsSameNode(&p[i], &p[i-1], myEps)) {
			idx++; surf->pNds.Add(new CNode(idx, p[i].x, p[i].y, p[i].z));
		}
		nID[i] = idx;
	}
	delete [] p;

	//sort id
	prhap(iID, nID, cnt_1); idx = 0;  count = 0;

	//copy new nodes for region
	double x, y, z;
    for(i = 0; i < surf->pLps.GetSize(); i++){
		for(j = 0; j < surf->pLps.GetAt(i)->pNds.GetSize(); j++){
			x = surf->pLps.GetAt(i)->pNds.GetAt(j)->x;
            y = surf->pLps.GetAt(i)->pNds.GetAt(j)->y;
			z = surf->pLps.GetAt(i)->pNds.GetAt(j)->z;
			for(k = 0; k < surf->pNds.GetSize(); k++) {
				if( IsSameNode(new CNode(i, x, y, z ), surf->pNds.GetAt(k), myEps) ) {
					surf->pLps.GetAt(i)->pNds.GetAt(j)->ID = k;
					surf->pLps.GetAt(i)->iNds.Add(k);
				}
			}
		}
	}

	delete[] iID;  delete[] nID;
	
}

// 2010.11.23 by Mei Gang at University of Freiburg, Germany.
// 未检验，可能有问题！
void CRealModelView::EmergePolygonSurfaceBest(CSurf * surf)
{
	int i, j, k, t;
	double x, y, z;
	int id = 0, id_lp, id_nd;
	
	CArray<CNode*, CNode*>pNds; 
	for(i = 0; i < surf->pLps.GetSize(); i++) {
		for(j = 0; j < surf->pLps.GetAt(i)->pNds.GetSize(); j++) {
				x = surf->pLps.GetAt(i)->pNds.GetAt(j)->x;
				y = surf->pLps.GetAt(i)->pNds.GetAt(j)->y;
				z = surf->pLps.GetAt(i)->pNds.GetAt(j)->z;
				surf->pLps.GetAt(i)->pNds.GetAt(j)->ID = id;
				pNds.Add(new CNode(id++, x, y, z));
		}
	}

	//sort the nodes
	int cnt_1, idx; // int count

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; 
		p[idx].y = pNds.GetAt(j)->y; 
		p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}


	//sort nodes
    SortNode(p, cnt_1);

	//save each node for one time
	int * newIDs = new int[cnt_1];

	id = 0;
	newIDs[0] = 0;

	for(i = 0; i < cnt_1 - 1; i++){
		j = i + 1;
		if( IsSameNode(p[i].x, p[i].y, p[i].z, p[j].x, p[j].y, p[j].z, myEps) == false ) { 
			id++;
		}
		newIDs[j] = id;
	}

	// Update : 1st step
	for(i = 0; i < cnt_1; i++) {
		id = p[i].ID;
		pNds.GetAt(id)->ID = newIDs[i];
	}

	// Update : 2nd step
	for(i = 0; i < surf->pLps.GetSize(); i++) {
		surf->pLps.GetAt(i)->iNds.RemoveAll();
		for(j = 0; j < surf->pLps.GetAt(i)->pNds.GetSize(); j++) {
				id = surf->pLps.GetAt(i)->pNds.GetAt(j)->ID;
				surf->pLps.GetAt(i)->pNds.GetAt(j)->ID = pNds.GetAt(id)->ID;
				surf->pLps.GetAt(i)->iNds.Add(pNds.GetAt(id)->ID);
		}
	}
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 


	// Save : after updating , important 
	int num = surf->pNds.GetSize();
	for(i = num - 1; i >= 0; i--){
		surf->pNds.GetAt(i)->Clear(); 
		delete surf->pNds.GetAt(i); 
		surf->pNds.RemoveAt(i);    
	}
	surf->pNds.RemoveAll(); 

	i = 0;
	x = p[i].x;  y = p[i].y;  z = p[i].z;
	surf->pNds.Add(new CNode(i, x, y, z));
	for(i = 1; i < cnt_1; i++) {
		if(newIDs[i] != newIDs[i-1]) {
			x = p[i].x;  y = p[i].y;  z = p[i].z;
	        surf->pNds.Add(new CNode(i, x, y, z));
		}
	}
	delete [] p;
}

// 2010.11.24 by Mei Gang at University of Freiburg, Germany.
void CRealModelView::EmergeTriangleSurface(CSurf * surf)
{
	//sort the nodes
	int i, j, k, cnt_1, idx, count;

	//get number of nodes on surfs
	cnt_1 = 0; idx = 0;
	cnt_1 = 3 * surf->pTrs.GetSize();

	//copy nodes
	CNode* p = new CNode[cnt_1];
    for(i = 0; i < surf->pTrs.GetSize(); i++){
		for(j = 0; j < 3; j++){
			p[idx].ID = idx;	
			p[idx].x = surf->pTrs.GetAt(i)->x[j];	
			p[idx].y = surf->pTrs.GetAt(i)->y[j];
			p[idx].z = surf->pTrs.GetAt(i)->z[j];
			idx++;
		}
	}
	//sort nodes
    SortNode(p, cnt_1);

	//re-arrange nodes
	int* iID = new int[cnt_1];	int* nID = new int[cnt_1];
	for(i = 0; i < cnt_1; i++)iID[i] = p[i].ID;

	for(i = surf->pNds.GetSize()-1; i > -1; i--) {
		delete surf->pNds.GetAt(i);  surf->pNds.RemoveAt(i);
	}
	nID[0] = 0; idx = 0;
	surf->pNds.Add(new CNode(idx, p[0].x, p[0].y, p[0].z)); 
	
	//re-create nodes
	for(i = 1; i < cnt_1; i++){
		if(!IsSameNode(&p[i], &p[i-1], myEps)) {
			idx++; surf->pNds.Add(new CNode(idx, p[i].x, p[i].y, p[i].z));
		}
		nID[i] = idx;
	}
	delete [] p;
	//sort id
	prhap(iID, nID, cnt_1); idx = 0;  count = 0;

	//copy new nodes for region
	double x, y, z;
    for(i = 0; i < surf->pTrs.GetSize(); i++){
		for(j = 0; j < 3; j++){
			x = surf->pTrs.GetAt(i)->x[j];
            y = surf->pTrs.GetAt(i)->y[j];
			z = surf->pTrs.GetAt(i)->z[j];
			for(k = 0; k < surf->pNds.GetSize(); k++) {
				if( IsSameNode(new CNode(i, x, y, z ), surf->pNds.GetAt(k), myEps) ) {
					surf->pTrs.GetAt(i)->nID[j] = k;
				}
			}
		}
	}

	delete[] iID;  delete[] nID;
	
}

// 2013.04.27 by Mei Gang at University of Freiburg, Germany.
void CRealModelView::EmergeTriangleSurfaceBest(CSurf * surf)
{
	int i, j, k, t;
	double x, y, z;
	int id = 0, id_lp, id_nd;
	
	CArray<CNode*, CNode*>pNds; 
	id = 0;
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			x = surf->pTrs.GetAt(i)->x[j];
			y = surf->pTrs.GetAt(i)->y[j];
			z = surf->pTrs.GetAt(i)->z[j];
			surf->pTrs.GetAt(i)->nID[j] = id;
			pNds.Add(new CNode(id++, x, y, z));
		}
	}

	//sort the nodes
	int cnt_1, idx; // int count

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; 
		p[idx].y = pNds.GetAt(j)->y; 
		p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}

	//sort nodes
    SortNode(p, cnt_1);

	//save each node for one time
	int * newIDs = new int[cnt_1];

	id = 0;
	newIDs[0] = 0;

	for(i = 0; i < cnt_1 - 1; i++){
		j = i + 1;
		if( IsSameNode(p[i].x, p[i].y, p[i].z, p[j].x, p[j].y, p[j].z, myEps) == false ) { 
			id++;
		}
		newIDs[j] = id;
	}

	// Update : 1st step
	for(i = 0; i < cnt_1; i++) {
		id = p[i].ID;
		pNds.GetAt(id)->ID = newIDs[i];
	}

	// Save
	surf->pNds.RemoveAll();
	i = 0;
	x = p[i].x;  y = p[i].y;  z = p[i].z;
	surf->pNds.Add(new CNode(i, x, y, z));
	for(i = 1; i < cnt_1; i++) {
		if(newIDs[i] != newIDs[i-1]) {
			x = p[i].x;  y = p[i].y;  z = p[i].z;
			surf->pNds.Add(new CNode(i, x, y, z));
		}
	}

	// Update : 2nd step
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			surf->pTrs.GetAt(i)->nID[j] = pNds.GetAt(id)->ID;
		}
	}

	delete [] p;
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 
}

void CRealModelView::SaveTrglMesh(CSurf* surf)
{
	// Save trgl surface
	double x, y, z;
	int i, j, n, ID, n0, n1, n2, NumOfNodes, NumOfElems;
	CString fn;
	NumOfNodes = surf->pNds.GetSize();   
	NumOfElems = surf->pTrs.GetSize();

	if(AfxMessageBox(_T("Saving Trgl Surface ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		fn = fn + _T("_TrglSurf");
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);
		fin << NumOfNodes << "   " << NumOfElems << endl;
		for(i = 0; i < NumOfNodes; i++) { // Node
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin << i <<"   "<<  x <<"   "<< y <<"   "<< z << endl;
		}
		for(i = 0; i < NumOfElems; i++) { // Mesh
			n0 = surf->pTrs.GetAt(i)->nID[0];
			n1 = surf->pTrs.GetAt(i)->nID[1];
			n2 = surf->pTrs.GetAt(i)->nID[2];
			fin << i <<"   "<< n0 <<"   "<< n1 <<"   "<< n2 << endl;
		}
		fin.close();
	}

}

// save quadrilateral mesh
void CRealModelView::SaveQuadMesh(CSurf* surf)
{
	// Save quad surface
	double x, y, z;
	int i, j, n, ID, n0, n1, n2, n3, NumOfNodes, NumOfElems;
	CString fn;
	NumOfNodes = surf->pNds.GetSize();   
	NumOfElems = surf->pTrs.GetSize();

	if(AfxMessageBox(_T("Saving Quad Surface ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		fn = fn + _T("_QuadSurf");
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);
		fin << NumOfNodes << "   " << NumOfElems << endl;
		for(i = 0; i < NumOfNodes; i++) { // Node
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin << i <<"   "<<  x <<"   "<< y <<"   "<< z << endl;
		}
		for(i = 0; i < NumOfElems; i++) { // Mesh
			n0 = surf->pQds.GetAt(i)->nID[0];
			n1 = surf->pQds.GetAt(i)->nID[1];
			n2 = surf->pQds.GetAt(i)->nID[2];
			n3 = surf->pQds.GetAt(i)->nID[3];
			fin << i <<"   "<< n0 <<"   "<< n1 <<"   "<< n2 <<"   "<< n3 << endl;
		}
		fin.close();
	}

}

// 2010.11.23 by Mei Gang at University of Freiburg, Germany.
void CRealModelView::SavePolygonSurface(CSurf * surf)
{
	// Save polygon surface
	double x, y, z;
	int i, j, n, ID, NumOfNodes, NumOfElems;
	CString fn;
	NumOfNodes = surf->pNds.GetSize();   
	NumOfElems = surf->pLps.GetSize();

	if(AfxMessageBox(_T("Saving Polygon Surface ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		fn = fn + _T("_PolySurf");
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);
		fin << NumOfNodes << "   " << NumOfElems << endl;
		for(i = 0; i < NumOfNodes; i++) { // Node
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin << i <<"   "<<  x <<"   "<< y <<"   "<< z <<endl;
		}
		for(i = 0; i < NumOfElems; i++) { // Mesh
			n = surf->pLps.GetAt(i)->iNds.GetSize();
			fin << i << "   "<< n << "   ";
			for(j = 0; j < n; j++) {
				ID = surf->pLps.GetAt(i)->iNds.GetAt(j);
				fin << ID << "   ";
			}
			fin << endl;
		}
		fin.close();
	}

}

// 2011.3.21
// display existed polyhedrons by colors
void CRealModelView::DisplayPolyhedraColor() 
{
	// delete existed display lists, very important!
	glDeleteLists(1, m_aSurfs.GetSize());

	// create a list of resulting polyhedrons
	pCrt = new CCordnt();
    pCrt = GetNewCordntSystemBlockGroup();
	GLuint allBlockGroups;
	allBlockGroups = glGenLists(1);
//	CompileBlockGroupList(pCrt, allBlockGroups, 0);
	glCompileBlockGroupList(pCrt, allBlockGroups, 1); // IF == 1

	m_nObjectType = OBJECT_TYPE_POLYHEDRA;
	m_bOpenGL = 1;
	Invalidate(TRUE);

}

//2010.4.17 by Mei Gang, University of Freiburg, Germany
//To get a new coordinate system for all block groups, reurn a pointor CCordnt* 
//All block groups are stored into m_aBlks.
//All polyhedrons are sttored into m_aPhds.
//All loops/areas are stored into m_aLps
//All nodes are stored into m_aNds.
CCordnt* CRealModelView::GetNewCordntSystemBlockGroup()
{
#ifndef EGC
	int i, k;
	double minx, miny, minz, maxx, maxy, maxz, sx, sy, sz, s;
	CCordnt* Crt = new CCordnt();
	//get the total number of nodes on tins
	double* cor = new double[m_aNds.GetSize()]; 
    
	k = m_aNds.GetSize();
	//record the coordinate-x
	for(i = 0; i < k; i++)  cor[i] = m_aNds.GetAt(i)->x;

	//sort the cor
	prhap(cor, k);
	minx = cor[0];	maxx = cor[k-1];
	sx = cor[k-1] - cor[0];

	//record the coordinate-y
	for(i = 0; i < k; i++)  cor[i] = m_aNds.GetAt(i)->y;

	//sort the cor
	prhap(cor, k);
	miny = cor[0];  maxy = cor[k-1];
	sy = cor[k-1] - cor[0];

	//record the coordinate-z
	for(i = 0; i < k; i++)cor[i] = m_aNds.GetAt(i)->z;

	//sort the cor
	prhap(cor, k);
	minz = cor[0]; maxz = cor[k-1];
	sz = cor[k-1] - cor[0];

    free(cor);
	Crt->u = (minx + maxx) / 2.0;  Crt->v = (miny + maxy) / 2.0;  Crt->w = (minz + maxz) / 2.0;
	Crt->minX = minx;  Crt->maxX = maxx;
	Crt->minY = miny;  Crt->maxY = maxy;
	Crt->minZ = minz;  Crt->maxZ = maxz;

	s = sx;
	if(s < sy) s = sy;
	if(s < sz) s = sz;
	Crt->Scale = 2.0 / s;

	return(Crt);

#else
	int i, k;
	float minx, miny, minz, maxx, maxy, maxz, sx, sy, sz, s;
	CCordnt* Crt = new CCordnt();
	//get the total number of nodes on tins
	float * cor = new float[m_aNds.GetSize()]; 
    
	k = m_aNds.GetSize();
	//record the coordinate-x
	for(i = 0; i < k; i++)  cor[i] = m_aNds.GetAt(i)->x.floatValue();

	//sort the cor
	prhap(cor, k);
	minx = cor[0];	maxx = cor[k-1];
	sx = cor[k-1] - cor[0];

	//record the coordinate-y
	for(i = 0; i < k; i++)  cor[i] = m_aNds.GetAt(i)->y.floatValue();

	//sort the cor
	prhap(cor, k);
	miny = cor[0];  maxy = cor[k-1];
	sy = cor[k-1] - cor[0];

	//record the coordinate-z
	for(i = 0; i < k; i++)cor[i] = m_aNds.GetAt(i)->z.floatValue();

	//sort the cor
	prhap(cor, k);
	minz = cor[0]; maxz = cor[k-1];
	sz = cor[k-1] - cor[0];

    free(cor);
	Crt->u = (minx + maxx) / 2.0;  
	Crt->v = (miny + maxy) / 2.0;  
	Crt->w = (minz + maxz) / 2.0;
	Crt->minX = minx;  Crt->maxX = maxx;
	Crt->minY = miny;  Crt->maxY = maxy;
	Crt->minZ = minz;  Crt->maxZ = maxz;

	s = sx;
	if(s < sy) s = sy;
	if(s < sz) s = sz;
	Crt->Scale = 2.0 / s;

	return(Crt);
#endif

}

void CRealModelView::SaveBlockGroups()
{
	//Save block groups : Node, Area, Polyhedron, Block group
	int i, j;
	CString fn, fnBlk, fnameBlk;
	if( AfxMessageBox(_T("Save Blocks files ?"), MB_YESNOCANCEL ) != 6 ) return;
	CFileDialog Dlg(false, NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Data File(*.txt)|*.txt"), NULL);

	if(Dlg.DoModal() == IDOK) 
	{
		fn = Dlg.GetPathName();
		fnameBlk = fn + _T("_BlockGroups");
		if(fnameBlk.Right(4)!="txt")   fnameBlk += ".txt";
		ofstream fout0(fnameBlk);
		fout0 << m_aNds.GetSize() << "      " <<m_aLps.GetSize() << "      " << m_aPhds.GetSize() << "      " << m_aBlks.GetSize() <<endl;

		// Node
		for(i = 0; i < m_aNds.GetSize(); i++){ 
			fout0 << i << "      " << m_aNds.GetAt(i)->x << "      " << m_aNds.GetAt(i)->y << "      " <<  m_aNds.GetAt(i)->z <<endl;
		}

		// Area
		for(i = 0; i < m_aLps.GetSize(); i++) {
			fout0 << m_aLps.GetAt(i)->iNds.GetSize() << "      ";
			if( (i + 1) % 10 == 0 )  fout0 << endl;
		}
		if(m_aLps.GetSize() % 10 != 0)  fout0 << endl;
		for(i = 0; i < m_aLps.GetSize(); i++){
			fout0 << i << "      " ;
			for(j = 0; j < m_aLps.GetAt(i)->iNds.GetSize(); j++) {
				fout0 << m_aLps.GetAt(i)->iNds.GetAt(j) << "      " ;
			}
			fout0 << endl;
		}
		
		//Polyhedron
		for(i = 0; i < m_aPhds.GetSize(); i++) {
			fout0 << m_aPhds.GetAt(i)->pLps.GetSize() << "      ";
			if( (i + 1) % 10 == 0 )  fout0 << endl;
		}
		if(m_aPhds.GetSize() % 10 != 0)  fout0 << endl;
		for(i = 0; i < m_aPhds.GetSize(); i++){
			fout0 << i << "      " ;
			for(j = 0; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++)
				fout0 << m_aPhds.GetAt(i)->iLps.GetAt(j) << "      ";
			fout0 << endl;
		}

		//Block group
		for(i = 0; i < m_aBlks.GetSize(); i++){
			fout0 << i << "      " << m_aBlks.GetAt(i)->iPhds.GetSize() << endl ;
			for(j = 0; j < m_aBlks.GetAt(i)->iPhds.GetSize(); j++){
				fout0 << m_aBlks.GetAt(i)->iPhds.GetAt(j) << "      " ;
				if( (j + 1) % 10 == 0)  fout0 << endl ;
			}
			fout0 << endl ;
		}

		fout0.close();
	}

}


void CRealModelView::SaveModel(CModel * model)
{
	//Save block groups : Node, Area, Polyhedron, Block group
	int i, j;
	CString fn, fnBlk, fnameBlk;
	if( AfxMessageBox(_T("Save Blocks files ?"), MB_YESNOCANCEL ) != 6 ) return;
	CFileDialog Dlg(false, NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Data File(*.txt)|*.txt"), NULL);

	if(Dlg.DoModal() == IDOK) 
	{
		fn = Dlg.GetPathName();
		fnameBlk = fn;
		fnameBlk = fn + _T("_BlockGroups");
		if(fnameBlk.Right(4)!="txt")   fnameBlk += ".txt";
		ofstream fout0(fnameBlk);
		fout0 << model->m_aNds.GetSize() << "      " 
			  << model->m_aLps.GetSize() << "      " 
			  << model->m_aPhds.GetSize() << "      " 
			  << model->m_aBlks.GetSize() << endl;

		// Node
		for(i = 0; i < model->m_aNds.GetSize(); i++){ 
			fout0 << i << "      " 
				  << model->m_aNds.GetAt(i)->x << "      " 
				  << model->m_aNds.GetAt(i)->y << "      " 
				  << model->m_aNds.GetAt(i)->z << endl;
		}

		// Area
		for(i = 0; i < model->m_aLps.GetSize(); i++) {
			fout0 << model->m_aLps.GetAt(i)->iNds.GetSize() << "      ";
			if( (i + 1) % 10 == 0 )  fout0 << endl;
		}
		if(model->m_aLps.GetSize() % 10 != 0)  fout0 << endl;
		for(i = 0; i < model->m_aLps.GetSize(); i++){
			fout0 << i << "      " ;
			for(j = 0; j < model->m_aLps.GetAt(i)->iNds.GetSize(); j++) {
				fout0 << model->m_aLps.GetAt(i)->iNds.GetAt(j) << "      " ;
			}
			fout0 << endl;
		}
		
		//Polyhedron
		for(i = 0; i < model->m_aPhds.GetSize(); i++) {
			fout0 << model->m_aPhds.GetAt(i)->pLps.GetSize() << "      ";
			if( (i + 1) % 10 == 0 )  fout0 << endl;
		}
		if(model->m_aPhds.GetSize() % 10 != 0)  fout0 << endl;
		for(i = 0; i < model->m_aPhds.GetSize(); i++){
			fout0 << i << "      " ;
			for(j = 0; j < model->m_aPhds.GetAt(i)->iLps.GetSize(); j++)
				fout0 << model->m_aPhds.GetAt(i)->iLps.GetAt(j) << "      ";
			fout0 << endl;
		}

		//Block group
		for(i = 0; i < model->m_aBlks.GetSize(); i++){
			fout0 << i << "      " << model->m_aBlks.GetAt(i)->iPhds.GetSize() << endl ;
			for(j = 0; j < model->m_aBlks.GetAt(i)->iPhds.GetSize(); j++){
				fout0 << model->m_aBlks.GetAt(i)->iPhds.GetAt(j) << "      " ;
				if( (j + 1) % 10 == 0)  fout0 << endl ;
			}
			fout0 << endl ;
		}

		fout0.close();
	}

}

// 2012.1.12 by Mei Gang 
// Save all Block groups including properties into a new format file
void CRealModelView::SaveBlockGroupsProperty()
{
	//Save block groups : Node, Area, Polyhedron, Block group
	int i, j;
	CString fn, fnBlk, fnameBlk;
	if( AfxMessageBox(_T("Save Blocks files ?"), MB_YESNOCANCEL ) != 6 ) return;
	CFileDialog Dlg(false, NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Data File(*.txt)|*.txt"), NULL);

	if(Dlg.DoModal() == IDOK) 
	{
		fn = Dlg.GetPathName();
		fnameBlk = fn;
		fnameBlk = fn + _T("_BlockGroups");
		if(fnameBlk.Right(4)!="txt")   fnameBlk += ".txt";
		ofstream fout0(fnameBlk);
		fout0 << m_aNds.GetSize() << "      " <<m_aLps.GetSize() << "      " << m_aPhds.GetSize() << "      " 
			  << m_aBlks.GetSize()<< "      " << m_aProps.GetSize() << endl;

		// Node
		for(i = 0; i < m_aNds.GetSize(); i++){ 
			fout0 << i << "      " << m_aNds.GetAt(i)->x << "      " << m_aNds.GetAt(i)->y << "      " <<  m_aNds.GetAt(i)->z <<endl;
		}

		// Area
		for(i = 0; i < m_aLps.GetSize(); i++) {
			fout0 << m_aLps.GetAt(i)->iNds.GetSize() << "      ";
			if( (i + 1) % 10 == 0 )  fout0 << endl;
		}
		if(m_aLps.GetSize() % 10 != 0)  fout0 << endl;
		for(i = 0; i < m_aLps.GetSize(); i++){
			fout0 << i << "      " ;
			for(j = 0; j < m_aLps.GetAt(i)->iNds.GetSize(); j++) {
				fout0 << m_aLps.GetAt(i)->iNds.GetAt(j) << "      " ;
			}
			fout0 << endl;
		}
		
		//Polyhedron
		for(i = 0; i < m_aPhds.GetSize(); i++) {
			fout0 << m_aPhds.GetAt(i)->pLps.GetSize() << "      ";
			if( (i + 1) % 10 == 0 )  fout0 << endl;
		}
		if(m_aPhds.GetSize() % 10 != 0)  fout0 << endl;
		for(i = 0; i < m_aPhds.GetSize(); i++){
			fout0 << i << "      " ;
			for(j = 0; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++)
				fout0 << m_aPhds.GetAt(i)->iLps.GetAt(j) << "      ";
			fout0 << endl;
		}

		//Block group
		for(i = 0; i < m_aBlks.GetSize(); i++){
			fout0 << i << "      " << m_aBlks.GetAt(i)->iPhds.GetSize() << endl ;
			for(j = 0; j < m_aBlks.GetAt(i)->iPhds.GetSize(); j++){
				fout0 << m_aBlks.GetAt(i)->iPhds.GetAt(j) << "      " ;
				if( (j + 1) % 10 == 0)  fout0 << endl ;
			}
			fout0 << endl ;
		}

		//List of Properties
	    int ID, Age, RGB[3];
	    CString Name, Material, Statement;
	    string sName, sMaterial, sStatement;
	    CStringW strw;
	    CStringA stra;
		for(i = 0; i < m_aProps.GetSize(); i++) { 
			ID = m_aProps.GetAt(i)->ID;
			Name = m_aProps.GetAt(i)->Name;
			Age = m_aProps.GetAt(i)->Age;
			Material = m_aProps.GetAt(i)->Material;
			Statement = m_aProps.GetAt(i)->Statement;
			for(j = 0; j < 3; j++) {
				RGB[j] = m_aProps.GetAt(i)->RGB[j];
			}

			strw = Name;
            stra = strw.GetBuffer(0);  strw.ReleaseBuffer();
            sName = stra.GetBuffer(0);  stra.ReleaseBuffer();

			strw = Material;
            stra = strw.GetBuffer(0);  strw.ReleaseBuffer();
            sMaterial = stra.GetBuffer(0);  stra.ReleaseBuffer();

			strw = Statement;
            stra = strw.GetBuffer(0);  strw.ReleaseBuffer();
            sStatement = stra.GetBuffer(0);  stra.ReleaseBuffer();
             
	    	fout0 << ID <<"   "<<  sName <<"   "<< Age <<"   "<< sMaterial << "   "
				<<RGB[0] <<" " << RGB[1]<<" "<< RGB[2]<<"  "<< sStatement << endl;
		}

		//Property of Blocks
		for(i = 0; i < m_aBlks.GetSize(); i++){
			fout0 << i << "      " << m_aBlks.GetAt(i)->iProperty << endl ;
		}

		fout0.close();
	}

}

// 2010.11.26 by Mei Gang //intersection in 3D
// two triangular surface intersect to form a group of polyhedrons, a block
void CRealModelView::On3DModelIntersect()
{
	// TODO: Add your command handler code here
	CBlock * pBlk = new CBlock();

//	InsectOfTrglSurf_2DAnd3D(m_aSurfs.GetAt(0), m_aSurfs.GetAt(1), pBlk);
	InsectOfTrglSurf_2DAnd3DBest(m_aSurfs.GetAt(0), m_aSurfs.GetAt(1), pBlk);
	// InsectOfTrglSurf_2DAnd3DV2(m_aSurfs.GetAt(0), m_aSurfs.GetAt(1), pBlk, myEps);
	// IntrTriMesh2DAnd3D_Float(m_aSurfs.GetAt(0), m_aSurfs.GetAt(1), pBlk);

	// add it into global set
	for(int i = 0; i < pBlk->pPhs.GetSize(); i++) {
		m_aPhds.Add(pBlk->pPhs.GetAt(i));
	}
	m_aBlks.Add(pBlk);

	// Emerge
//	EmergeBlockGroups();
	EmergeBlockGroupsBest();

	// display
	DisplayPolyhedraColor();

	// save
	SaveBlockGroups();

	m_aSurfs.GetAt(1)->Clear();  delete m_aSurfs.GetAt(1);  m_aSurfs.RemoveAt(1);
	m_aSurfs.GetAt(0)->Clear();  delete m_aSurfs.GetAt(0);  m_aSurfs.RemoveAt(0);
	
}

//2009.10.19. By Mei Gang, in Freiburg, Germany
//To intersect 2 triangles in 2D
//CTrgl * pT0, CTrgl * pT1------2 triangles;
//CLoop * pIntr-------- to save the intersection

//2009.10.19. By Mei Gang, in Freiburg, Germany
bool CRealModelView::IsTwoElesAreSame(double *x0, double *y0, double *z0, double *x1, double * y1, double *z1, int node)
{
	int i, j;
	bool same = true;   bool * tt = new bool[node];
	for(i = 0; i < node; i++){
		tt[i] = false;
		for(j = 0; j < node; j++){
			if(IsSameNode(x0[i], y0[i], z0[i], x1[j], y1[j], z1[j], myEps) == true) {
			   tt[i] = true;    break;
			}
		}
	}
    for(i = 0; i < node; i++){
		if (i == 0)   same = tt[i];
		else same = same && tt[i];
	}
	return same;
	delete[] tt;
	
}

//2009.10.20. By Mei GANG in Freiburg, Germany
//TO compute the equation of a line in 2D
//(x1, y1)  (x2, y2)
// para[3]: a, b, c  parameters of line equation (ax + by + c = 0)
// p, d :parameters of line equation( r(t) = p + t * d)
void CRealModelView::EquaLine2D( double x1, double y1, double x2, double y2, double para[3])
{
	para[0] = y1 - y2;  //a
	para[1] = x2 - x1;  //b
	para[2] = x1 * y2 - x2 * y1;  //c
}
void CRealModelView::EquaLine2D(double x1, double y1, double x2, double y2, CNode *p, CNode *d)
{
	/*
	double a, b, c;
	a = y1 - y2;
	b = x2 - x1;
	c = x1 * y2 - x2 * y1;
	p->x = -1 * a * c / (a * a + b * b);  p->y = -1 * b * c / (a * a + b * b);
	d->x = -1 * b;  d->y = a;
	*/
	p->x = x1;  p->y = y1;
	d->x = x2 - x1;  d->y = y2 - y1;

}

// Test whether a Node is in a triangle or not. By Mei Gang 2009.10.9
// (xn, yn)  the Node to be tested
// x0, y0, x1, y1, x2, y2 the vertexes of triangle, they are supposed to lay on clockwise
// return value: 
// 0-Node is in triangle;  1-Node is on vertex;  2-Node is on edge;  3-Node is outside of the triangle
int CRealModelView::IsNdInTrgl(double xn, double yn, double x0, double y0, double x1, double y1, double x2, double y2)
{
	double sx, sy, ex, ey, sum;
	double delta = 0.0001;
	int NumPosi = 0,  NumZero = 0,  NumNega = 0;

	// add 2011.5.30
	if(sqrt((x0 - xn)*(x0 - xn) + (y0 - yn)*(y0 - yn)) < delta)  return 1;
	if(sqrt((x1 - xn)*(x1 - xn) + (y1 - yn)*(y1 - yn)) < delta)  return 1;
	if(sqrt((x2 - xn)*(x2 - xn) + (y2 - yn)*(y2 - yn)) < delta)  return 1;

	if(IsPointOnSegment(x0, y0, x1, y1, xn, yn, myEps) == true)  return 2;
	if(IsPointOnSegment(x1, y1, x2, y2, xn, yn, myEps) == true)  return 2;
	if(IsPointOnSegment(x2, y2, x0, y0, xn, yn, myEps) == true)  return 2;
	// end 2011.5.30
	
	//line 01
	sx = x0;  sy = y0;  ex = x1;  ey = y1;
    sum = (xn - sx) * (ey - sy) -  (ex -sx) * (yn - sy);
	if(sum > delta)  NumPosi++;
	else if(sum < -1.0 * delta)  NumNega++;
	     else  NumZero++;

	//line 12
	sx = x1;  sy = y1;  ex = x2;  ey = y2;
	sum = (xn - sx) * (ey - sy) -  (ex -sx) * (yn - sy);
	if(sum > delta)  NumPosi++;
	else if(sum < -1.0 * delta)  NumNega++;
	     else  NumZero++;

	//line 20
	sx = x2;  sy = y2;  ex = x0;  ey = y0;
	sum = (xn - sx) * (ey - sy) -  (ex -sx) * (yn - sy);
	if(sum > delta)  NumPosi++;
	else if(sum < -1.0 * delta)  NumNega++;
	     else  NumZero++;

	if( NumPosi > 0)    return 3; // Node is outside of the triangle
	else {
		if( NumNega == 3 )    return 0; // Node is in triangle
		else {
			if( NumZero == 1)    return 2; //Node is on edge;
			// if( NumZero == 2)    return 1; // Node is on vertex;
			else return 1;  // Node is on vertex;
		}
	}

}

//2009.10.16  By Mei Gang in Freiburg
//2009.11.03  Modified
//To form the intersection part(pIntr) of two loops(p0 and p1)
//pIntr: save the nodes of the intersection. it is maybe Null, a point, a segment or a loop;
//return value: the number of the nodes on the intersection. it is maybe 0, 1, 2 and >2.
int CRealModelView::FormIntrLoop2Loop2D(CLoop * p0, CLoop * p1, CLoop * pIntr)
{
	int NumNdsIntr;
	CArray<CNode*, CNode*>pNds;
	int i, j, id, nsize;
	double x, y, z, xx, yy, zz;

	//合并两个环上节点
	id = 0;
	nsize = p0->pNds.GetSize();
	for(i = 0; i < nsize; i++) {
		x = p0->pNds.GetAt(i)->x;  y = p0->pNds.GetAt(i)->y;  z = p0->pNds.GetAt(i)->z;
		pNds.Add(new CNode(id++, x, y, z));
		pNds.GetAt( pNds.GetUpperBound() )->bvalid = p0->pNds.GetAt(i)->bvalid;
	}
	nsize = p1->pNds.GetSize();
	for(i = 0; i < nsize; i++) {
		x = p1->pNds.GetAt(i)->x;  y = p1->pNds.GetAt(i)->y;  z = p1->pNds.GetAt(i)->z;
		pNds.Add(new CNode(id++, x, y, z));
		pNds.GetAt( pNds.GetUpperBound() )->bvalid = p1->pNds.GetAt(i)->bvalid;
	}

	//删除重复点
	double r;
	for(i = 0; i < pNds.GetSize(); i++) {
		x = pNds.GetAt(i)->x;  y = pNds.GetAt(i)->y;  z = pNds.GetAt(i)->z;
		for(j = i + 1; j < pNds.GetSize(); ) {
			xx = pNds.GetAt(j)->x;  yy = pNds.GetAt(j)->y;  zz = pNds.GetAt(j)->z;
			r = (x - xx) * (x - xx) + (y - yy) * (y - yy) + (z - zz) * (z - zz);
			if(r < CIGMA) {
				delete pNds.GetAt(j);  pNds.RemoveAt(j);
			}  else {
				j++;
			}
		}
	}
	//重新命名点号
	nsize = pNds.GetSize();
	for(i = 0; i < nsize; i++)  pNds.GetAt(i)->ID = i;

	//更新环中节点编号，单独保存，为搜索一阶邻域点准备
	CArray<int, int> p0id, p1id, pIntrid;
	nsize = p0->pNds.GetSize();
	for(i = 0; i < nsize; i++) {
		x = p0->pNds.GetAt(i)->x;  y = p0->pNds.GetAt(i)->y;  z = p0->pNds.GetAt(i)->z;
		for(j = 0; j < pNds.GetSize(); j++) {
			xx = pNds.GetAt(j)->x;  yy = pNds.GetAt(j)->y;  zz = pNds.GetAt(j)->z;
			r = (x - xx) * (x - xx) + (y - yy) * (y - yy) + (z - zz) * (z - zz);
			if(r < CIGMA) { p0id.Add(j);  break; }
		}
	}
	nsize = p1->pNds.GetSize();
	for(i = 0; i < nsize; i++) {
		x = p1->pNds.GetAt(i)->x;  y = p1->pNds.GetAt(i)->y;  z = p1->pNds.GetAt(i)->z;
		for(j = 0; j < pNds.GetSize(); j++) {
			xx = pNds.GetAt(j)->x;  yy = pNds.GetAt(j)->y;  zz = pNds.GetAt(j)->z;
			r = (x - xx) * (x - xx) + (y - yy) * (y - yy) + (z - zz) * (z - zz);
			if(r < CIGMA) { p1id.Add(j);  break; }
		}
	}

	//一阶邻域点
	nsize = p0id.GetSize();
    for(i = 0; i < nsize - 1; i++) {
		pNds.GetAt( p0id.GetAt(i) )->sNeiOnMesh.Add( p0id.GetAt(i + 1));
		pNds.GetAt( p0id.GetAt(i + 1))->sNeiOnMesh.Add( p0id.GetAt(i) );
	}
	nsize = p1id.GetSize();
    for(i = 0; i < nsize - 1; i++) {
		pNds.GetAt( p1id.GetAt(i) )->sNeiOnMesh.Add( p1id.GetAt(i + 1));
		pNds.GetAt( p1id.GetAt(i + 1))->sNeiOnMesh.Add( p1id.GetAt(i) );
	}

	//相交部分（分为空、点、线段、环）
	CArray<int, int> idvalid;
	nsize = pNds.GetSize();
	for(i = 0; i < nsize; i++) { if(pNds.GetAt(i)->bvalid) idvalid.Add(i); } // 有效点的个数
	
	switch( idvalid.GetSize() )
	{
	case 0: { break; } //空（不相交）
	case 1: { //点（相交部分为一个点）
        pIntrid.Add( idvalid.GetAt(0) );
		break; 
			}
	case 2: { //多线段（相交部分为一条多线段，可能只能存在一条线段）
		pIntrid.Add( idvalid.GetAt(0) );
		pIntrid.Add( idvalid.GetAt(1) );
		break;
			}
	default: { //环（相交部分为一个闭合环）

		int start, mid, end;
		bool bvalid = false;
		nsize = pNds.GetSize();
		for(i = 0; i < nsize; i++) { //找起始点
			if(pNds.GetAt(i)->bvalid) {
				start = i;  break;
			}
		}
		mid = start;  end = -1; //初始化
		pIntrid.Add(mid); //保存起点
		while( end != start ) { //环的首、尾点相同
			for(i = 0; i < pNds.GetAt(mid)->sNeiOnMesh.GetSize(); i++) {
				id = pNds.GetAt(mid)->sNeiOnMesh.GetAt(i);    bvalid = false;
				bvalid = pNds.GetAt(id)->bvalid;
				if(bvalid) {
					if(pIntrid.GetSize() == 1) { //环中原来只有一个点
						end = id;  break;
					}
					else { 
						if( id != pIntrid.GetAt(pIntrid.GetSize() - 2)) { //环中原来至少有2个点
						    end = id;  break; }	
					}
				}
			}
			mid = end; //更新mid
			pIntrid.Add(mid);
		}

		break; 
		
	}
	}

/*
	//检查环上节点是否逆时针,否，调换
	CNode * p = new CNode[pIntrid.GetSize()];
	for(i = 0; i < pIntrid.GetSize(); i++){
		id = pIntrid.GetAt(i);
		p[i].x = pNds.GetAt(id)->x;  p[i].y = pNds.GetAt(id)->y;  p[i].z = pNds.GetAt(id)->z;
	}
	if( !Isccwize(p, pIntrid.GetSize()) ) { //如果是顺时针
		int * nid = new int[pIntrid.GetSize()];
	    for(i = 0; i < pIntrid.GetSize(); i++)  nid[i] = pIntrid.GetAt( pIntrid.GetUpperBound() - i);
        pIntrid.RemoveAll();
		for(i = 0; i < pIntrid.GetSize(); i++)  pIntrid.Add( nid[i] );
		delete [] nid;
	}
    delete [] p;
*/
    for(i = 0; i < pIntrid.GetSize(); i++){
		id = pIntrid.GetAt(i);
		x = pNds.GetAt(id)->x;  y = pNds.GetAt(id)->y;  z = pNds.GetAt(id)->z;
		pIntr->pNds.Add(new CNode(i, x, y, z));
	}
	NumNdsIntr = pIntr->pNds.GetSize();
	
    for(i = pNds.GetUpperBound(); i > -1; i--) { delete pNds.GetAt(i);  pNds.RemoveAt(i); }
	p0id.RemoveAll();  p1id.RemoveAll();  pIntrid.RemoveAll();  idvalid.RemoveAll();
	return NumNdsIntr;

}

void CRealModelView::IntrTriangle2Triangle2(CTrgl * pT0, CTrgl * pT1, CLoop * pIntr)
{
	////////特例
	int i, j, k;
	double x0[3], y0[3], z0[3], x1[3], y1[3], z1[3];
	for(i = 0; i < 3; i++) {
		x0[i] = pT0->x[i];  y0[i] = pT0->y[i];  z0[i] = pT0->z[i];
		x1[i] = pT1->x[i];  y1[i] = pT1->y[i];  z1[i] = pT1->z[i];
	}

	//两个三角形完全重合
	if( IsTwoElesAreSame(x0, y0, z0, x1, y1, z1, 3) ) {
		for(i = 0; i < 3; i++) {
			pIntr->pNds.Add(new CNode(i, pT0->x[i], pT0->y[i], pT0->z[i]) );
		}
		i = 0;
		pIntr->pNds.Add(new CNode(i, pT0->x[i], pT0->y[i], pT0->z[i]) );  // 重新保存第一个点，环的首尾相同
		return;
	}

	//相交部分为其中一个三角形
	//一个三角形的任意三个顶点都不在另外一个三角形外
	int outT0[3], outT1[3];
	for(i = 0; i < 3; i++) {
		outT0[i] = IsNdInTrgl(x0[i], y0[i], x1[0], y1[0], x1[1], y1[1], x1[2], y1[2]);	
	}
	if(outT0[0] != 3 && outT0[1] != 3 && outT0[2] != 3) { // 重合部分为三角形 pT0
		for(i = 0; i < 3; i++) {
			pIntr->pNds.Add(new CNode(i, pT0->x[i], pT0->y[i], pT0->z[i]) );
		}
		i = 0;
        pIntr->pNds.Add(new CNode(i, pT0->x[i], pT0->y[i], pT0->z[i]) );
		return;
	}
	for(i = 0; i < 3; i++) {
		outT1[i] = IsNdInTrgl(x1[i], y1[i], x0[0], y0[0], x0[1], y0[1], x0[2], y0[2]);	
	}
	if(outT1[0] != 3 && outT1[1] != 3 && outT1[2] != 3) { // 重合部分为三角形 pT1
		for(i = 0; i < 3; i++) {
			pIntr->pNds.Add(new CNode(i, pT1->x[i], pT1->y[i], pT1->z[i]) );
		}
		i = 0;
        pIntr->pNds.Add(new CNode(i, pT1->x[i], pT1->y[i], pT1->z[i]) );
		return;
	}

	// 2011.5.30 Begin
	// 特殊情况，相交部分为一点或线段
	
	CArray<int, int> nIn; // number of inside vertices
	CArray<int, int> nOn; // just on vertex or edge
	CArray<int, int> nOut; // out
	
	// 0-Node is in triangle;  1-Node is on vertex;  2-Node is on edge;  3-Node is outside of the triangle
	// for the first triangle
	nIn.RemoveAll();  nOn.RemoveAll();  nOut.RemoveAll();
	for(i = 0; i < 3; i++) { 
		if(outT0[i] == 0)  nIn.Add(i);
		if(outT0[i] == 1)  nOn.Add(i);
		if(outT0[i] == 2)  nOn.Add(i);
		if(outT0[i] == 3)  nOut.Add(i);
	}
	if( nOn.GetSize() == 1 && nOut.GetSize() == 2 ) { //相交部分为一个点
		i = nOn.GetAt(0);
        pIntr->pNds.Add(new CNode(i, pT0->x[i], pT0->y[i], pT0->z[i]) );
		return;
	}
	if( nOn.GetSize() == 2 && nOut.GetSize() == 1 ) { //相交部分为一线段
		i = nOn.GetAt(0);
        pIntr->pNds.Add(new CNode(i, pT0->x[i], pT0->y[i], pT0->z[i]) );
		i = nOn.GetAt(1);
        pIntr->pNds.Add(new CNode(i, pT0->x[i], pT0->y[i], pT0->z[i]) );
		return;
	}

	// for the second triangle
	nIn.RemoveAll();  nOn.RemoveAll();  nOut.RemoveAll();
	for(i = 0; i < 3; i++) { 
		if(outT1[i] == 0)  nIn.Add(i);
		if(outT1[i] == 1)  nOn.Add(i);
		if(outT1[i] == 2)  nOn.Add(i);
		if(outT1[i] == 3)  nOut.Add(i);
	}
	if( nOn.GetSize() == 1 && nOut.GetSize() == 2 ) { //相交部分为一个点
		i = nOn.GetAt(0);
        pIntr->pNds.Add(new CNode(i, pT1->x[i], pT1->y[i], pT1->z[i]) );
		return;
	}
	if( nOn.GetSize() == 2 && nOut.GetSize() == 1 ) { //相交部分为一线段
		i = nOn.GetAt(0);
        pIntr->pNds.Add(new CNode(i, pT1->x[i], pT1->y[i], pT1->z[i]) );
		i = nOn.GetAt(1);
        pIntr->pNds.Add(new CNode(i, pT1->x[i], pT1->y[i], pT1->z[i]) );
		return;
	}
	// 2011.5.30 end

	////////普通情况。
	////////至少有一个点在另外一个三角形外。
	CLoop * p0 = new CLoop();  CLoop * p1 = new CLoop();
	for(i = 0; i < 3; i++) {
		p0->pNds.Add( new CNode(i, pT0->x[i], pT0->y[i], pT0->z[i]) );
		p1->pNds.Add( new CNode(i, pT1->x[i], pT1->y[i], pT1->z[i]) );
	}

	//step 1: 判断一个三角形的顶点是否在另一个三角形内  已经计算过，直接用
	for(i = 0; i < 3; i++) {
		if(outT0[i] !=3 )  p0->pNds.GetAt(i)->bvalid = true;
		else p0->pNds.GetAt(i)->bvalid = false;
		if(outT1[i] !=3 )  p1->pNds.GetAt(i)->bvalid = true;
		else p1->pNds.GetAt(i)->bvalid = false;
	}
	//step 2: 两三角形的线段相交并分割，按次序保存到 CLoop 中
	bool bvectex[2], bexist;
	int nCase, l;
	double lx, ly, lz;
	CNode * pN0 = new CNode();  CNode * d0 = new CNode();  
	CNode * pN1 = new CNode();  CNode * d1 = new CNode();
	CNode * i0 = new CNode();  CNode * i1 = new CNode();
	
	p0->pPLns.RemoveAll();  p1->pPLns.RemoveAll();
	for(i = 0; i < 3; i++) { p0->pPLns.Add( new CPLine() );  p1->pPLns.Add( new CPLine() ); } // 交点在此另外保存一份
	for(i = 0; i < 3; i++) {
		if( i == 2) { EquaLine2D( pT0->x[i], pT0->y[i], pT0->x[0], pT0->y[0], pN0, d0); }
		else { EquaLine2D( pT0->x[i], pT0->y[i], pT0->x[i + 1], pT0->y[i + 1], pN0, d0); }
		for(j = 0; j < 3; j++) {
			if( j == 2) { EquaLine2D( pT1->x[j], pT1->y[j], pT1->x[0], pT1->y[0], pN1, d1); }
			else { EquaLine2D( pT1->x[j], pT1->y[j], pT1->x[j + 1], pT1->y[j + 1], pN1, d1);}
			nCase = IntrSegment2Segment2D(pN0, d0, pN1, d1, i0, i1, myEps); // 线段相交
			
			//分割线段，并按次序保存
			if( nCase == 1) { //线段交于一点, 在CLoop中单独保存一份
				bvectex[0] = false;  bvectex[1] = false;
				for(k = 0; k < 3; k++) { // 判断交点是否为线段所在三角形的端点，是-不保存；否-保存
					bvectex[0] = bvectex[0] || IsSameNode(i0->x, i0->y, i0->z, pT0->x[k], pT0->y[k], pT0->z[k], myEps);
					bvectex[1] = bvectex[1] || IsSameNode(i0->x, i0->y, i0->z, pT1->x[k], pT1->y[k], pT1->z[k], myEps);	
				}
			
				if( !bvectex[0] ) { // T0
					if( p0->pPLns.GetAt(i)->pNds.GetSize() < 1) { // 点集为Null，直接保存
						p0->pPLns.GetAt(i)->pNds.Add(new CNode(i, i0->x, i0->y, i0->z));  
						p0->pPLns.GetAt(i)->pNds.GetAt( p0->pPLns.GetAt(i)->pNds.GetUpperBound())->bvalid = true;
					}
					else {
						bexist = false;
						for(l = 0; l < p0->pPLns.GetAt(i)->pNds.GetSize(); l++) {
							lx = p0->pPLns.GetAt(i)->pNds.GetAt(l)->x;
							ly = p0->pPLns.GetAt(i)->pNds.GetAt(l)->y;
							lz = p0->pPLns.GetAt(i)->pNds.GetAt(l)->z;
							//bexist = bexist || SameNode(i0->x, i0->y, i0->z, lx, ly, lz);
							bexist = bexist || sqrt((i0->x - lx) * (i0->x - lx) + (i0->y - ly) * (i0->y - ly)) < 0.00001;
						}
						if( !bexist ) { //不存在，保存
							p0->pPLns.GetAt(i)->pNds.Add(new CNode(i, i0->x, i0->y, i0->z));  
							p0->pPLns.GetAt(i)->pNds.GetAt( p0->pPLns.GetAt(i)->pNds.GetUpperBound())->bvalid = true;
						}
					}
				}
				if( !bvectex[1] ) { // T1
					if( p1->pPLns.GetAt(j)->pNds.GetSize() < 1) {
						p1->pPLns.GetAt(j)->pNds.Add(new CNode(j, i0->x, i0->y, i0->z)); 
						p1->pPLns.GetAt(j)->pNds.GetAt( p1->pPLns.GetAt(j)->pNds.GetUpperBound())->bvalid = true;
					}
					else {
						bexist = false;
						for(l = 0; l < p1->pPLns.GetAt(j)->pNds.GetSize(); l++) {
							lx = p1->pPLns.GetAt(j)->pNds.GetAt(l)->x;
							ly = p1->pPLns.GetAt(j)->pNds.GetAt(l)->y;
							lz = p1->pPLns.GetAt(j)->pNds.GetAt(l)->z;
						//	bexist = bexist || SameNode(i0->x, i0->y, i0->z, lx, ly, lz);
							bexist = bexist || sqrt((i0->x - lx) * (i0->x - lx) + (i0->y - ly) * (i0->y - ly)) < 0.00001;
						}
						if( !bexist ) { //不存在，保存
							p1->pPLns.GetAt(j)->pNds.Add(new CNode(j, i0->x, i0->y, i0->z)); 
							p1->pPLns.GetAt(j)->pNds.GetAt( p1->pPLns.GetAt(j)->pNds.GetUpperBound())->bvalid = true;
						}
						
					}	
				}
				

			}
		}
	}
	//将交点和原始点按照次序保存到Loop
	double tx[3], ty[3], dist[2];
	for(i = 0; i < 3; i++) {
		p0->pPLns.GetAt(i)->pNds.InsertAt(0, new CNode(i, pT0->x[i], pT0->y[i], pT0->z[i]));
		p1->pPLns.GetAt(i)->pNds.InsertAt(0, new CNode(i, pT1->x[i], pT1->y[i], pT1->z[i]));
		p0->pPLns.GetAt(i)->pNds.GetAt(0)->bvalid = p0->pNds.GetAt(i)->bvalid;
		p1->pPLns.GetAt(i)->pNds.GetAt(0)->bvalid = p1->pNds.GetAt(i)->bvalid;

		//存在两个交点。加上上面新加入，共3个点
		if( p0->pPLns.GetAt(i)->pNds.GetSize() == 3) { //p0
			for(j = 0; j < 3; j++) { tx[j] = p0->pPLns.GetAt(i)->pNds.GetAt(j)->x; ty[j] = p0->pPLns.GetAt(i)->pNds.GetAt(j)->y;}
			for(j = 0; j < 2; j++) { dist[j] = ((tx[j+1]-tx[0])*(tx[j+1]-tx[0]) + (ty[j+1]-ty[0])*(ty[j+1]-ty[0])); }
            if( dist[0] > dist[1] ) { //调换节点在环中的位置
				p0->pPLns.GetAt(i)->pNds.GetAt(1)->x = tx[2];  p0->pPLns.GetAt(i)->pNds.GetAt(1)->y = ty[2];
				p0->pPLns.GetAt(i)->pNds.GetAt(2)->x = tx[1];  p0->pPLns.GetAt(i)->pNds.GetAt(2)->y = ty[1];
			}

			//temp and need to be deleted
			if(fabs(dist[0] - dist[1]) < 0.00001)  MessageBox(_T(" T1线段上存在相同的节点！"));

		}
		if( p1->pPLns.GetAt(i)->pNds.GetSize() == 3) { //p1
			for(j = 0; j < 3; j++) { tx[j] = p1->pPLns.GetAt(i)->pNds.GetAt(j)->x; ty[j] = p1->pPLns.GetAt(i)->pNds.GetAt(j)->y;}
			for(j = 0; j < 2; j++) { dist[j] = ((tx[j+1]-tx[0])*(tx[j+1]-tx[0]) + (ty[j+1]-ty[0])*(ty[j+1]-ty[0])); }
            if( dist[0] > dist[1] ) { //调换节点在环中的位置
				p1->pPLns.GetAt(i)->pNds.GetAt(1)->x = tx[2];  p1->pPLns.GetAt(i)->pNds.GetAt(1)->y = ty[2];
				p1->pPLns.GetAt(i)->pNds.GetAt(2)->x = tx[1];  p1->pPLns.GetAt(i)->pNds.GetAt(2)->y = ty[1];
			}
			
		}
	}

	//更新 CLoop -> pNds
	for(i = p0->pNds.GetUpperBound(); i > -1; i--) { delete p0->pNds.GetAt(i);  p0->pNds.RemoveAt(i); }
	for(i = p1->pNds.GetUpperBound(); i > -1; i--) { delete p1->pNds.GetAt(i);  p1->pNds.RemoveAt(i); }
	for(i = 0; i < p0->pPLns.GetSize(); i++) {
		for(j = 0; j < p0->pPLns.GetAt(i)->pNds.GetSize(); j++) {
			p0->pNds.Add( p0->pPLns.GetAt(i)->pNds.GetAt(j));
		}
	}
	for(i = 0; i < p1->pPLns.GetSize(); i++) {
		for(j = 0; j < p1->pPLns.GetAt(i)->pNds.GetSize(); j++) {
			p1->pNds.Add( p1->pPLns.GetAt(i)->pNds.GetAt(j));
		}
	}
    p0->pNds.Add( p0->pPLns.GetAt(0)->pNds.GetAt(0) ); //保存第一个点，保证环的首尾点相同
	p1->pNds.Add( p1->pPLns.GetAt(0)->pNds.GetAt(0) ); //保存第一个点，保证环的首尾点相同
	
	// 重新计算 inside vectex
	for(i = 0; i < 3; i++) {
		x0[i] = pT0->x[i];  y0[i] = pT0->y[i];  z0[i] = pT0->z[i];
		x1[i] = pT1->x[i];  y1[i] = pT1->y[i];  z1[i] = pT1->z[i];
	}
	for(i = 0; i < p0->pNds.GetSize(); i++) {
		p0->pNds.GetAt(i)->bvalid = true;
		nCase = IsNdInTrgl(p0->pNds.GetAt(i)->x, p0->pNds.GetAt(i)->y, x1[0], y1[0], x1[1], y1[1], x1[2], y1[2]);
		if(nCase == 3)  p0->pNds.GetAt(i)->bvalid = false;
	}
	for(i = 0; i < p1->pNds.GetSize(); i++) {
		p1->pNds.GetAt(i)->bvalid = true;
		nCase = IsNdInTrgl(p1->pNds.GetAt(i)->x, p1->pNds.GetAt(i)->y, x0[0], y0[0], x0[1], y0[1], x0[2], y0[2]);
		if(nCase == 3)  p1->pNds.GetAt(i)->bvalid = false;
	}

	/*
	//Temperary
	//保存Node
	double xx,yy,zz;
	int ID, NumOfNodes;
	CString fnNode;
	bool bvalid;
	if(AfxMessageBox("确定保存节点文件?", MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, "Data File(*.txt)|*.txt", NULL);
	if(Dlg.DoModal() == IDOK) {
		NumOfNodes = p0->pNds.GetSize();
		fnNode = Dlg.GetPathName(); 
		if(fnNode.Right(4)!=".txt")   fnNode += ".txt";
		ofstream fin3(fnNode);
		fin3 <<"// CLoop 0 : "<<endl;
		for(i = 0; i < NumOfNodes; i++){
			ID = p0->pNds.GetAt(i)->id;
			xx = p0->pNds.GetAt(i)->x;  yy = p0->pNds.GetAt(i)->y;  zz = p0->pNds.GetAt(i)->z;
			bvalid = p0->pNds.GetAt(i)->bvalid;
			fin3 <<ID<<"   "<<  xx <<"   "<< yy<<"   "<< zz<< "   " << bvalid << endl;
		}
        NumOfNodes = p1->pNds.GetSize();
		fin3 <<"// CLoop 1 : "<<endl;
		for(i = 0; i < NumOfNodes; i++){
			ID = p1->pNds.GetAt(i)->id;
			xx = p1->pNds.GetAt(i)->x;  yy = p1->pNds.GetAt(i)->y;  zz = p1->pNds.GetAt(i)->z;
			//fin3 <<ID<<"   "<<  xx <<"   "<< yy<<"   "<< zz<<endl;
			bvalid = p1->pNds.GetAt(i)->bvalid;
			fin3 <<ID<<"   "<<  xx <<"   "<< yy<<"   "<< zz<< "   " << bvalid << endl;
		}
	}
	// return;
	*/
   
	//step 3: 调用FormIntrLoop2Loop2D(p0, p1, pIntr)得到相交
	int NumNdsIntr = FormIntrLoop2Loop2D(p0, p1, pIntr);

}

//2010.11.26 By Mei Gang in freiburg Germany
//To compute the intersections of 2 triangle mesh in 3D and 2D
//parameters: CRegion * pSurf0, CRegion * pSurf1, CBlock * pBlk
//result    : firstly, all the intersections will be saved into m_aBlks
// secondly, transform all elements in M_aBlks to polyhedrons, which will be saved in pBlk->pPhds
void CRealModelView::InsectOfTrglSurf_2DAnd3D(CSurf * pSurf0, CSurf * pSurf1, CBlock * pBlk) 
{
#ifndef EGC
	CArray<CNode*, CNode*>pNds;
	CArray<CPair*, CPair*>m_aPrs2D; 
	CArray<CPair*, CPair*>m_aPrs3D;
	CArray<CBlock*, CBlock*>m_aBlks;

	int Add = pSurf0->pTrs.GetSize();

	// intersection in 2D
	int i, j, k, ii, jj, kk, id, tid[3];
	double x, y, z;
	CArray<CTrgl*, CTrgl*>pTrs;
	CTrgl * pTr0; CTrgl * pTr1;

	CArray<CSurf*, CSurf*>Rgns;
	CSurf * r0 = new CSurf();  CSurf * r1 = new CSurf();
	Rgns.Add(r0);  Rgns.Add(r1);

	for(i = 0; i < pSurf0->pTrs.GetSize(); i++) {
		id = pSurf0->pTrs.GetAt(i)->ID;
		for(j = 0; j < 3; j++) tid[j] = pSurf0->pTrs.GetAt(i)->nID[j];
		r0->pTrs.Add(new CTrgl(id, tid[0], tid[1], tid[2]));
		for(j = 0; j < 3; j++) {
			r0->pTrs.GetAt(i)->x[j] = pSurf0->pTrs.GetAt(i)->x[j];
			r0->pTrs.GetAt(i)->y[j] = pSurf0->pTrs.GetAt(i)->y[j];
			r0->pTrs.GetAt(i)->z[j] = 0.00;
		}
	}
	for(i = 0; i < pSurf0->pNds.GetSize(); i++) {
		id = pSurf0->pNds.GetAt(i)->ID;
		x = pSurf0->pNds.GetAt(i)->x; y = pSurf0->pNds.GetAt(i)->y; z = 0.00;
		r0->pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < pSurf0->pNds.GetSize(); i++){ //记录经过每个点的三角形ID 2010.1.29
		for(j = 0; j < pSurf0->pNds.GetAt(i)->sPsdTrgl.GetSize(); j++){
			r0->pNds.GetAt(i)->sPsdTrgl.Add(pSurf0->pNds.GetAt(i)->sPsdTrgl.GetAt(j));
		}
	}
	for(i = 0; i < pSurf0->pTrs.GetSize(); i++){ //记录每个三角形的邻域三角形，不包括自身
		for(j = 0; j < pSurf0->pTrs.GetAt(i)->iRgnPassTr.GetSize(); j++){
			r0->pTrs.GetAt(i)->iRgnPassTr.Add(pSurf0->pTrs.GetAt(i)->iRgnPassTr.GetAt(j));
		}
	}

	for(i = 0; i < pSurf1->pTrs.GetSize(); i++) {
		id = pSurf1->pTrs.GetAt(i)->ID;
		for(j = 0; j < 3; j++) tid[j] = pSurf1->pTrs.GetAt(i)->nID[j];
		r1->pTrs.Add(new CTrgl(id, tid[0], tid[1], tid[2]));
		for(j = 0; j < 3; j++) {
			r1->pTrs.GetAt(i)->x[j] = pSurf1->pTrs.GetAt(i)->x[j];
			r1->pTrs.GetAt(i)->y[j] = pSurf1->pTrs.GetAt(i)->y[j];
			r1->pTrs.GetAt(i)->z[j] = 0.00;
		}
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++) {
		id = pSurf1->pNds.GetAt(i)->ID;
		x = pSurf1->pNds.GetAt(i)->x; y = pSurf1->pNds.GetAt(i)->y; z = 0.00;
		r1->pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++){ //记录经过每个点的三角形ID 2010.1.29
		for(j = 0; j < pSurf1->pNds.GetAt(i)->sPsdTrgl.GetSize(); j++){
			r1->pNds.GetAt(i)->sPsdTrgl.Add(pSurf1->pNds.GetAt(i)->sPsdTrgl.GetAt(j));
		}
	}
	for(i = 0; i < pSurf1->pTrs.GetSize(); i++){ //记录每个三角形的邻域三角形，不包括自身
		for(j = 0; j < pSurf1->pTrs.GetAt(i)->iRgnPassTr.GetSize(); j++){
			r1->pTrs.GetAt(i)->iRgnPassTr.Add(pSurf1->pTrs.GetAt(i)->iRgnPassTr.GetAt(j));
		}
	}

//	SearchIntrTrgls(r0, r1, myEps); // to search the intersected triangles for each triangle
	SearchInsectPair2D_OBBTree(r0, r1, myEps);

	for(i = 0; i < 2; i++) {
		for(j = 0; j < Rgns.GetAt(i)->pTrs.GetSize(); j++) {
			pTrs.Add( Rgns.GetAt(i)->pTrs.GetAt(j) );
		}
	}

 	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CLoop * pIntr = new CLoop();
			IntrTriangle2Triangle2(pTr0, pTr1, pIntr);
			m_aLpsIntr.Add(pIntr); // Save
			pTr0->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
			pTr1->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
		}
	}

	//统一到一个三角形对的集合内2009.12.17
	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CPair * pr = new CPair();
			pr->FstID = pTr0->ID;  pr->SedID = pTr1->ID;
            m_aPrs2D.Add(pr); // 与m_aLpsIntr一一对应
		}
	}

	CombiLps2D(); // Combine all loops in 2D, 2010.2.8

	////intersection in 3D
	int t;
	int count, nCase;
	double *nx, *ny, *nz, *para;
	double xmin, ymin, zmin, xmax, ymax, zmax;
	int xdivi, ydivi, zdivi;
	double xtmp, ytmp, ztmp;
	double sum, dstep;
	int n0, n1, n2, n3, n4, n5, n6, n7;
	CArray<CNode*, CNode*>pSonNds;
	CArray<CHehd*, CHehd*>pSonBox;
	CNode * pMin, * pMax;
	CArray<CLine*, CLine*> pLns;
	float v0[3], v1[3], v2[3], u0[3], u1[3], u2[3], isec0[3], isec1[3]; // prepare for Moller.dll
	int copla;
	int * coplanar = &copla;
	
	//Find the smallest and the biggest x, y and z, and form a box;
	count = pSurf0->pNds.GetSize() + pSurf1->pNds.GetSize();
	nx = new double[count];  ny = new double[count];  nz = new double[count];
	for(i = 0; i < pSurf0->pNds.GetSize(); i++) {
		nx[i] = pSurf0->pNds.GetAt(i)->x;
		ny[i] = pSurf0->pNds.GetAt(i)->y;
		nz[i] = pSurf0->pNds.GetAt(i)->z;
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++) {
		j = i + pSurf0->pNds.GetSize();
		nx[j] = pSurf1->pNds.GetAt(i)->x;
		ny[j] = pSurf1->pNds.GetAt(i)->y;
		nz[j] = pSurf1->pNds.GetAt(i)->z;
	}
	prhap(nx,count);  prhap(ny,count);  prhap(nz,count);
	xmin = nx[0];        ymin = ny[0];        zmin = nz[0];
	xmax = nx[count-1];  ymax = ny[count-1];  zmax = nz[count-1];
	delete [] nx;  delete [] ny;  delete [] nz;
	
	//Compute all triangles' edges' length, and use it to divide the Box
	sum = 0.0;
	for(i = 0; i < pSurf0->pTrs.GetSize(); i++) {
		CTrgl * tr = pSurf0->pTrs.GetAt(i);
		sum += (tr->x[0] - tr->x[1]) * (tr->x[0] - tr->x[1]) + 
			   (tr->y[0] - tr->y[1]) * (tr->y[0] - tr->y[1]) +
			   (tr->z[0] - tr->z[1]) * (tr->z[0] - tr->z[1]) ;
        sum += (tr->x[1] - tr->x[2]) * (tr->x[1] - tr->x[2]) + 
			   (tr->y[1] - tr->y[2]) * (tr->y[1] - tr->y[2]) +
			   (tr->z[1] - tr->z[2]) * (tr->z[1] - tr->z[2]) ;
        sum += (tr->x[2] - tr->x[0]) * (tr->x[2] - tr->x[0]) + 
			   (tr->y[2] - tr->y[0]) * (tr->y[2] - tr->y[0]) +
			   (tr->z[2] - tr->z[0]) * (tr->z[2] - tr->z[0]) ;
	}
	for(i = 0; i < pSurf1->pTrs.GetSize(); i++) {
		CTrgl * tr = pSurf1->pTrs.GetAt(i);
		sum += (tr->x[0] - tr->x[1]) * (tr->x[0] - tr->x[1]) + 
			   (tr->y[0] - tr->y[1]) * (tr->y[0] - tr->y[1]) +
			   (tr->z[0] - tr->z[1]) * (tr->z[0] - tr->z[1]) ;
        sum += (tr->x[1] - tr->x[2]) * (tr->x[1] - tr->x[2]) + 
			   (tr->y[1] - tr->y[2]) * (tr->y[1] - tr->y[2]) +
			   (tr->z[1] - tr->z[2]) * (tr->z[1] - tr->z[2]) ;
        sum += (tr->x[2] - tr->x[0]) * (tr->x[2] - tr->x[0]) + 
			   (tr->y[2] - tr->y[0]) * (tr->y[2] - tr->y[0]) +
			   (tr->z[2] - tr->z[0]) * (tr->z[2] - tr->z[0]) ;
	}
	sum /= (pSurf0->pTrs.GetSize() + pSurf1->pTrs.GetSize()) * 3; // average length
	dstep = sqrt(sum);
	
	//divisions
	xtmp = (xmax - xmin) / dstep; //x
    xdivi = (int)(xtmp) + 1;
	ytmp = (ymax - ymin) / dstep; //y
    ydivi = (int)(ytmp) + 1;
	ztmp = (zmax - zmin) / dstep; //z
    zdivi = (int)(ztmp) + 1;
	
	// save all son-boxes
	//Nodes
	id = 0;
	for(k = 0; k < zdivi + 1; k++){
		for(j = 0; j < ydivi + 1; j++){
			for(i = 0; i < xdivi + 1; i++){
				x = xmin + i * dstep;    y = ymin + j * dstep;    z = zmin + k * dstep;
				pSonNds.Add(new CNode(id++, x, y, z));
			}
		}
	}
	
	//Son-boxes
	for(k = 0; k < zdivi; k++) {
		for(j = 0; j < ydivi; j++) {
			for(i = 0; i < xdivi; i++) {
				n0 = k * (ydivi + 1) * (xdivi + 1) + j * (xdivi + 1) +i;
				n1=k*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i+1;
				n2=k*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i+1;
				n3=k*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i;
				n4=(k+1)*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i;
				n5=(k+1)*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i+1;
				n6=(k+1)*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i+1;
				n7=(k+1)*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i;
				pSonBox.Add(new CHehd(n0,n1,n2,n3,n4,n5,n6,n7));
			}
		}
	}
	
	//test which triangles intersect each son-box
	for(i = 0; i < pSonBox.GetSize(); i++) {
		pMin = pSonNds.GetAt(pSonBox.GetAt(i)->nID[0]);
		pMax = pSonNds.GetAt(pSonBox.GetAt(i)->nID[6]);
		for(j = 0; j < pSurf0->pTrs.GetSize(); j++) {
			if(IsTrglInsectBox(pSurf0->pTrs.GetAt(j), pMin, pMax) == 1)
				pSonBox.GetAt(i)->iIntrTriRgn0.Add(j);
		}
		for(j = 0; j < pSurf1->pTrs.GetSize(); j++) {
			if(IsTrglInsectBox(pSurf1->pTrs.GetAt(j), pMin, pMax) == 1)
				pSonBox.GetAt(i)->iIntrTriRgn1.Add(j);
		}
	}
	
	//compute the intersection of each triangle pair which is restricted in one son-box
	for(i = 0; i < pSonBox.GetSize(); i++) {
		for(j = 0; j < pSonBox.GetAt(i)->iIntrTriRgn0.GetSize(); j++) {
			n0 = pSonBox.GetAt(i)->iIntrTriRgn0.GetAt(j);
			v0[0] = pSurf0->pTrs.GetAt(n0)->x[0]; 
			v0[1] = pSurf0->pTrs.GetAt(n0)->y[0]; 
			v0[2] = pSurf0->pTrs.GetAt(n0)->z[0];

			v1[0] = pSurf0->pTrs.GetAt(n0)->x[1]; 
			v1[1] = pSurf0->pTrs.GetAt(n0)->y[1]; 
			v1[2] = pSurf0->pTrs.GetAt(n0)->z[1];

			v2[0] = pSurf0->pTrs.GetAt(n0)->x[2]; 
			v2[1] = pSurf0->pTrs.GetAt(n0)->y[2]; 
			v2[2] = pSurf0->pTrs.GetAt(n0)->z[2];

			for(k = 0; k < pSonBox.GetAt(i)->iIntrTriRgn1.GetSize(); k++) {
				n1 = pSonBox.GetAt(i)->iIntrTriRgn1.GetAt(k);
				u0[0] = pSurf1->pTrs.GetAt(n1)->x[0]; 
				u0[1] = pSurf1->pTrs.GetAt(n1)->y[0]; 
				u0[2] = pSurf1->pTrs.GetAt(n1)->z[0];

				u1[0] = pSurf1->pTrs.GetAt(n1)->x[1]; 
				u1[1] = pSurf1->pTrs.GetAt(n1)->y[1]; 
				u1[2] = pSurf1->pTrs.GetAt(n1)->z[1];

				u2[0] = pSurf1->pTrs.GetAt(n1)->x[2]; 
				u2[1] = pSurf1->pTrs.GetAt(n1)->y[2]; 
				u2[2] = pSurf1->pTrs.GetAt(n1)->z[2];
				
				// Load function from DLL Moller.dll 2011.9.30
			    typedef int (*lpFun)(float [3],float [3],float [3],
				                 float [3],float [3],float [3],int *,
				                 float [3],float [3]); // 宏定义函数指针类型
	            HINSTANCE hDll; //DLL 句柄
                lpFun trglFun; // 函数指针

	            hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
	
                if (NULL==hDll) {
                    MessageBox(_T("Fail to load DLL Moller.dll ! "));
	            }
	            else {
		             trglFun = (lpFun) GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
                     if (trglFun != NULL) {
					     nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		             }
		             else {
			             MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		             }
                     FreeLibrary(hDll); // 释放函数
	            }
			    // End loading
				
				if(nCase == 1) { //save intersection segment
					if(pLns.GetSize() < 1) { // save directly
						CLine * ptmp = new CLine();
						ptmp->FstTri = n0;  ptmp->SecTri = n1;
						for(t = 0; t < 3; t++) { ptmp->FstNd[t] = isec0[t];  ptmp->SecNd[t] = isec1[t];} //交点坐标
						pLns.Add(ptmp);
					}
					else {
						bool bexist = false;
						for(t = 0; t < pLns.GetSize(); t++){ 
							if(pLns.GetAt(t)->FstTri == n0 && pLns.GetAt(t)->SecTri == n1){ bexist = true;  break; }
						}
						if(!bexist) { // not exist
							CLine * ptmp = new CLine();
							ptmp->FstTri = n0;  ptmp->SecTri = n1;
							for(t = 0; t < 3; t++) { 
								ptmp->FstNd[t] = isec0[t];  
								ptmp->SecNd[t] = isec1[t];
							} //交点坐标
							pLns.Add(ptmp);
						}
					}
				}
			}
		}
	}
	
	//统一保存到一个三角形对的集合中。2009.12.17
	for(i = 0; i < pLns.GetSize(); i++) {
		CPair * pr = new CPair();
		pr->FstID = pLns.GetAt(i)->FstTri;  pr->SedID = pLns.GetAt(i)->SecTri;
		m_aPrs3D.Add(pr); //与pLns一一对应
	}
	////end of intersection in 3D 2009.1.22

	//2010.1.22 Mei Gang, Freiburg, Germany
	//2009.12.17 23:30  freiburg germany
	////use the 3D intersecton line to cut the 2D intersection loop
	//step 1: 确定2D相交三角形对有那些同时3D相交
	for(i = 0; i < m_aPrs2D.GetSize(); i++) { //initiate
		m_aPrs2D.GetAt(i)->bIntr2D = true;  m_aPrs2D.GetAt(i)->bIntr3D = false;}
	for(i = 0; i < m_aPrs3D.GetSize(); i++) {
		m_aPrs3D.GetAt(i)->bIntr2D = false; m_aPrs3D.GetAt(i)->bIntr3D = true; }

	for(i = 0; i < m_aPrs2D.GetSize(); i++) { //search out the pairs which intersect in both 2D and 3D
		for(j = 0; j < m_aPrs3D.GetSize(); j++) {
			if(m_aPrs2D.GetAt(i)->FstID == m_aPrs3D.GetAt(j)->FstID &&
			   m_aPrs2D.GetAt(i)->SedID == m_aPrs3D.GetAt(j)->SedID ){
				m_aPrs2D.GetAt(i)->bIntr3D = true;  m_aPrs3D.GetAt(j)->bIntr2D = true;
				break;
			}
		}
	}

	/*
	// output temperarily
	CString fnMesh;
	int NumOfPrs = m_aPrs3D.GetSize();
	if( AfxMessageBox("确定保存Temp文件?", MB_YESNOCANCEL) != 6 ) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, "Data File(*.txt)|*.txt", NULL);
	if( Dlg.DoModal() == IDOK ) 
	{
		fnMesh = Dlg.GetPathName(); 
		fnMesh += "-Temp";
		if(fnMesh.Right(4) != ".txt")   fnMesh += ".txt";
		ofstream fin( fnMesh );
		for(i = 0; i < NumOfPrs; i++){
			fin << i << "  " << m_aPrs3D.GetAt(i)->FstID << "  " << m_aPrs3D.GetAt(i)->SedID;
			fin << endl;
		}
		fin.close(); 
		
	}
	exit(0);
	// end tmp
	*/

	// For progress window
	CProgressWnd wndProgress(this, _T("Progress"), TRUE);
	int myRance = m_aPrs2D.GetSize();
	wndProgress.SetRange(0, myRance);
	wndProgress.SetText(_T("This is a progress window...\n\n")
						_T("Try dragging it around or hitting Cancel."));

	// form the blocks
	int situ[2], N;
    CLoop * plp = new CLoop();
	CLoop * pson = new CLoop[2];
	N = m_aPrs2D.GetSize();
//	for(i = 5; i < m_aPrs2D.GetSize(); i++) {
	for(i = 0; i < m_aPrs2D.GetSize(); i++) {
		// For progress windown
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) {
			MessageBox(_T("Progress Cancelled"));
			::PostQuitMessage(0);
		}
		// End
		// For output window
		CString str;
		str.Format(_T("Forming Polyhedrons :  i = %d  [ / %d]"), i, N);
		pFrame->AddStrToOutputWnd(1, str);

		if(m_aLpsIntr.GetAt(i)->pNds.GetSize() < 3)  continue;  //intersection in 2D is a point or a segment

		plp->Clear();  pson[0].Clear();  pson[1].Clear(); // initiate, very important!
		
		CBlock * blk = new CBlock();
		blk->FstTri = m_aPrs2D.GetAt(i)->FstID;  blk->SecTri = m_aPrs2D.GetAt(i)->SedID; //index of the two triangles
		if(!m_aPrs2D.GetAt(i)->bIntr3D) { // intersect only in 2D
			for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
				x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;// at present, just need to save x and y
				y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
				blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
			}
			//update Z, according to the equation of the triangle
			n0 = m_aPrs2D.GetAt(i)->FstID;
			nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
			for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
				nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
				ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
				nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
			}
			EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
			for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
				x = blk->DnLpNds.GetAt(j)->x;
				y = blk->DnLpNds.GetAt(j)->y;
				z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
				blk->DnLpNds.GetAt(j)->z = z;
			}
			delete [] nx;  delete [] ny; delete [] nz; delete [] para;
			
			n1 = m_aPrs2D.GetAt(i)->SedID;
			nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
			for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
				nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
				ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
				nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
			}
			EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
			for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
				x = blk->UpLpNds.GetAt(j)->x;
				y = blk->UpLpNds.GetAt(j)->y;
				z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
				blk->UpLpNds.GetAt(j)->z = z;
			}
			delete [] nx;  delete [] ny; delete [] nz; delete [] para;
			
			/*
			// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
			// Begin : Check whether each polyhedron is valid
			bool bValid = true;
			double zup = 0.0, zdn = 0.0;
			if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
				AfxMessageBox("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()",MB_OK|MB_ICONSTOP);
				return;
			}
			for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
				zup += blk->UpLpNds.GetAt(j)->z;
				zdn += blk->DnLpNds.GetAt(j)->z;
			}
			if(zup > zdn )  
			// End.2010.11.25
			*/

			m_aBlks.Add(blk);//save
		}
		else { // intersect in both 2D and 3D
			bool bexist = false;
			n0 = m_aPrs2D.GetAt(i)->FstID;
			v0[0] = pSurf0->pTrs.GetAt(n0)->x[0]; v0[1] = pSurf0->pTrs.GetAt(n0)->y[0]; v0[2] = pSurf0->pTrs.GetAt(n0)->z[0];
			v1[0] = pSurf0->pTrs.GetAt(n0)->x[1]; v1[1] = pSurf0->pTrs.GetAt(n0)->y[1]; v1[2] = pSurf0->pTrs.GetAt(n0)->z[1];
			v2[0] = pSurf0->pTrs.GetAt(n0)->x[2]; v2[1] = pSurf0->pTrs.GetAt(n0)->y[2]; v2[2] = pSurf0->pTrs.GetAt(n0)->z[2];
			
			n1 = m_aPrs2D.GetAt(i)->SedID;
			u0[0] = pSurf1->pTrs.GetAt(n1)->x[0]; u0[1] = pSurf1->pTrs.GetAt(n1)->y[0]; u0[2] = pSurf1->pTrs.GetAt(n1)->z[0];
			u1[0] = pSurf1->pTrs.GetAt(n1)->x[1]; u1[1] = pSurf1->pTrs.GetAt(n1)->y[1]; u1[2] = pSurf1->pTrs.GetAt(n1)->z[1];
			u2[0] = pSurf1->pTrs.GetAt(n1)->x[2]; u2[1] = pSurf1->pTrs.GetAt(n1)->y[2]; u2[2] = pSurf1->pTrs.GetAt(n1)->z[2];
			
			// Load function from DLL Moller.dll 2011.9.30
			typedef int (*lpFun)(float V0[3],float V1[3],float V2[3],
				                 float U0[3],float U1[3],float U2[3],int *coplanar,
				                 float isectpt1[3],float isectpt2[3]); // 宏定义函数指针类型
	        HINSTANCE hDll; //DLL 句柄
            lpFun trglFun; // 函数指针

	        hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
	
            if (NULL==hDll) {
                MessageBox(_T("Fail to load DLL Moller.dll ! "));
	        }
	        else {
				trglFun = (lpFun)GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
                if (trglFun != NULL) {
					nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		        }
		        else {
			        MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		        }
                FreeLibrary(hDll); // 释放函数
	        }
			// End loading

			if(nCase == 1) { //intersection is a segment
				//case 1 : 相交线段恰好为2D相交环上的一条边，只生成一个Block
				for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; j++) {
					x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;
					y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
					z = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->z;
					xtmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->x;
					ytmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->y;
					ztmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->z;
					
					if( //判断两条线段相同
						(sqrt((x-isec0[0])*(x-isec0[0])+(y-isec0[1])*(y-isec0[1])) < 0.00001 &&
						sqrt((xtmp-isec1[0])*(xtmp-isec1[0])+(ytmp-isec1[1])*(ytmp-isec1[1]) )< 0.00001) 
						||
						(sqrt((x-isec1[0])*(x-isec1[0])+(y-isec1[1])*(y-isec1[1])) < 0.00001 &&
						sqrt((xtmp-isec0[0])*(xtmp-isec0[0])+(ytmp-isec0[1])*(ytmp-isec0[1]) )< 0.00001)
						) {
						bexist = true;
					}
				}
				
				if(bexist) {
                    CBlock * blk = new CBlock();
					for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;// at present, just need to save x and y
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
						blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
					}

					//update Z, according to the equation of the triangle
					n0 = m_aPrs2D.GetAt(i)->FstID;
					nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
					for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
						nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
						ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
						nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
					}
					EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
					for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
						x = blk->DnLpNds.GetAt(j)->x;
						y = blk->DnLpNds.GetAt(j)->y;
						z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
						blk->DnLpNds.GetAt(j)->z = z;
					}
					delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
					n1 = m_aPrs2D.GetAt(i)->SedID;
					nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
					for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
						nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
						ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
						nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
					}
					EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
					for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
						x = blk->UpLpNds.GetAt(j)->x;
						y = blk->UpLpNds.GetAt(j)->y;
						z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
						blk->UpLpNds.GetAt(j)->z = z;
					}
					delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
					/*
					// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
					// Begin : Check whether each polyhedron is valid
					// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
					// Begin : Check whether each polyhedron is valid
					bool bValid = true;
					double zup = 0.0, zdn = 0.0;
					if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
						AfxMessageBox("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()",MB_OK|MB_ICONSTOP);
						return;
					}
					for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
						zup += blk->UpLpNds.GetAt(j)->z;
						zdn += blk->DnLpNds.GetAt(j)->z;
					}
					if(zup > zdn )  
					// End.2010.11.25
					*/
						
					m_aBlks.Add(blk);//save

				}
				else { //case 2 : 相交线段将2D相交环分割为两个子环，生成二个Block
					// 2011.5.21 一下代码有问题，目前为解决！

//					OutputIntr();  exit(0);
					
					int pos[2];//index of the position
					//以isec0所在的线段为新环的起始线段
					for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; j++) {
						CNode * sp0 = m_aLpsIntr.GetAt(i)->pNds.GetAt(j);
						CNode * sp1 = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1);
						CNode * point = new CNode(isec0[0],isec0[1]);
						float p0x = (float) sp0->x;  float p0y = (float) sp0->y;
						float p1x = (float) sp1->x;  float p1y = (float) sp1->y;
						if(IsPointOnSegment(p0x, p0y, p1x, p1y, isec0[0], isec0[1], myEps) == true) {
							pos[0] = j;  break;
						}
					}

					plp->Clear();
					for(t = pos[0]; t < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; t++) {//没有保存最后一个点
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->x;
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->y;
						plp->pNds.Add(new CNode(x,y));
					}
					for(t = 0; t < pos[0]; t++) {//保存了第一个点
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->x;
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->y;
						plp->pNds.Add(new CNode(x,y));
					}
					pos[0] = 0; //update
					plp->pNds.Add(new CNode(plp->pNds.GetAt(0)->x,plp->pNds.GetAt(0)->y)); //最合一个点要和第一个点相同
					
					//isec1的位置
					for(j = 0; j < plp->pNds.GetSize() - 1; j++) {
						CNode * sp0 = plp->pNds.GetAt(j);
						CNode * sp1 = plp->pNds.GetAt(j+1);
						float p0x = (float) sp0->x;  float p0y = (float) sp0->y;
						float p1x = (float) sp1->x;  float p1y = (float) sp1->y;
						if(IsPointOnSegment(p0x, p0y, p1x, p1y, isec1[0], isec1[1], myEps) == true) {
							pos[1] = j;  break;
						}
					}
					
					//form 2 son loops
					pson[0].pNds.Add(new CNode(isec0[0], isec0[1])); //first point
					for(t = 0; t < pos[1]; t++) {
						pson[0].pNds.Add(new CNode(plp->pNds.GetAt(t + 1)->x, plp->pNds.GetAt(t + 1)->y)); //must + 1
					}
                    pson[0].pNds.Add(new CNode(isec1[0], isec1[1])); //last point
					pson[0].pNds.Add(new CNode(isec0[0], isec0[1])); //first point again
					
					pson[1].pNds.Add(new CNode(isec0[0], isec0[1])); //first point
					pson[1].pNds.Add(new CNode(isec1[0], isec1[1])); //second point
					for(t = pos[1]; t < plp->pNds.GetSize() - 1; t++) {
						pson[1].pNds.Add(new CNode(plp->pNds.GetAt(t + 1)->x, plp->pNds.GetAt(t + 1)->y)); //must + 1
					}
					pson[1].pNds.Add(new CNode(isec0[0], isec0[1])); //first point again
					
					//update Z and form blocks
					//m_aBlks
					for(ii = 0; ii < 2; ii++) {
						CBlock * blk = new CBlock();
						for(jj = 0; jj < pson[ii].pNds.GetSize(); jj++) {
							x = pson[ii].pNds.GetAt(jj)->x;  y = pson[ii].pNds.GetAt(jj)->y;
							blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
						}
						//update Z, according to the equation of the triangle
						n0 = m_aPrs2D.GetAt(i)->FstID;
						nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
						for(kk = 0; kk < 3; kk++) { // real coordinates of the triangle, z is not zero
							nx[kk] = pSurf0->pTrs.GetAt(n0)->x[kk];
							ny[kk] = pSurf0->pTrs.GetAt(n0)->y[kk];
							nz[kk] = pSurf0->pTrs.GetAt(n0)->z[kk];
						}
						EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
						for(jj = 0; jj < blk->DnLpNds.GetSize(); jj++) {
							x = blk->DnLpNds.GetAt(jj)->x;
							y = blk->DnLpNds.GetAt(jj)->y;
							z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
							blk->DnLpNds.GetAt(jj)->z = z;
						}
						delete [] nx;  delete [] ny; delete [] nz; delete [] para;
						
						n1 = m_aPrs2D.GetAt(i)->SedID;
						nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
						for(kk = 0; kk < 3; kk++) { // real coordinates of the triangle, z is not zero
							nx[kk] = pSurf1->pTrs.GetAt(n1)->x[kk];
							ny[kk] = pSurf1->pTrs.GetAt(n1)->y[kk];
							nz[kk] = pSurf1->pTrs.GetAt(n1)->z[kk];
						}
						EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
						for(jj = 0; jj < blk->UpLpNds.GetSize(); jj++) {
							x = blk->UpLpNds.GetAt(jj)->x;
							y = blk->UpLpNds.GetAt(jj)->y;
							z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
							blk->UpLpNds.GetAt(jj)->z = z;
						}
						delete [] nx;  delete [] ny; delete [] nz; delete [] para;
						
						/*
						// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
						// Begin : Check whether each polyhedron is valid
						bool bValid = true;
						double zup = 0.0, zdn = 0.0;
						if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
							AfxMessageBox("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()",MB_OK|MB_ICONSTOP);
							return;
						}
						for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
							zup += blk->UpLpNds.GetAt(j)->z;
							zdn += blk->DnLpNds.GetAt(j)->z;
						}
						if(zup > zdn )  
							// End.2010.11.25
						*/

						m_aBlks.Add(blk);
					}

					
				} // end else
			}
		}
	
	}



	/*
	// 2011.5.20 注释
	// 原因：当相交的两个面为平面时，以下代码不会产生问题；当为曲面时，出现错误，故注释
	
	//避免实际在平面上的点不落在同一个平面上，统一计算相同面上点的Z 2010.2.7
	//update Z, according to the equation of the triangle
	n0 = pSurf0->pTrs.GetSize() - 1;
	nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
	for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
		nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
		ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
		nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
	}
	EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
			x = blk->DnLpNds.GetAt(j)->x;
			y = blk->DnLpNds.GetAt(j)->y;
			z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
			blk->DnLpNds.GetAt(j)->z = z;
		}
	}
	delete [] nx;  delete [] ny; delete [] nz; delete [] para;
	
	n1 = pSurf1->pTrs.GetSize() - 1;
	nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
	for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
		nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
		ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
		nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
	}
	EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
			x = blk->UpLpNds.GetAt(j)->x;
			y = blk->UpLpNds.GetAt(j)->y;
			z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
			blk->UpLpNds.GetAt(j)->z = z;
		}
	}
	delete [] nx;  delete [] ny; delete [] nz; delete [] para;
	*/

	//2010.1.14
	//emerge all the blocks , and then rearrange and update them
	for(i = 0; i < m_aBlks.GetSize(); i++) { // original nodes of blocks
		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID;
			x = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->x;
			y = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->y;
			z = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->z;
			pNds.Add(new CNode(id, x, y, z));
		}
		for(j = 0; j < m_aBlks.GetAt(i)->DnLpNds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID;
			x = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->x;
			y = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->y;
			z = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->z;
			pNds.Add(new CNode(id, x, y, z));
		}
	}

	//sort the nodes
	int cnt_1, idx; // int count

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; p[idx].y = pNds.GetAt(j)->y; p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll();  
	//sort nodes
    SortNode(p, cnt_1);
	
	//re-arrange nodes
	int* iID = new int[cnt_1];	int* nID = new int[cnt_1];
	for(i = 0; i < cnt_1; i++)iID[i] = p[i].ID;

	nID[0] = 0; idx = 0;

	pNds.Add(new CNode(idx, p[0].x, p[0].y, p[0].z)); 

	//save each node for one time
	bool boolexist;
	for(i = 1; i < cnt_1; i++){
		boolexist = false;
		for(j = 0; j < pNds.GetSize(); j++) {
			if(IsSameNode(&p[i], pNds.GetAt(j), myEps)){ boolexist = true;  break; }
		}
		if(!boolexist) pNds.Add(new CNode(pNds.GetSize(), p[i].x, p[i].y, p[i].z));
	}
	delete [] p;

	delete[] iID;  delete[] nID;
    
	//最直接、最笨的方法合并节点
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		for(j = 0; j < m_aBlks.GetAt(i)->DnLpNds.GetSize(); j++) {
			for(k = 0; k < pNds.GetSize(); k++) {
				if(IsSameNode(m_aBlks.GetAt(i)->DnLpNds.GetAt(j), pNds.GetAt(k), myEps)) {
					m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID = k;  break;
				}
			}
		}
		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize(); j++) {
			for(k = 0; k < pNds.GetSize(); k++) {
				if(IsSameNode(m_aBlks.GetAt(i)->UpLpNds.GetAt(j), pNds.GetAt(k), myEps)) {
					m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID = k;  break;
				}
			}
		}
	}

	// transform from block to polyhedron, now a block includes a groups of polyhedrons
	// not just represents only one polyhedron
	for(i = 0; i < m_aBlks.GetSize(); i++ ) { //根据拓扑关系生成Areas
		CLoop * up = new CLoop();
		for(t = 0; t < m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; t++) { //首尾点相同，去掉最后一个点
			up->iNds.Add(m_aBlks.GetAt(i)->UpLpNds.GetAt(t)->ID);
		}
		CLoop * dn = new CLoop();
		for(t = 0; t < m_aBlks.GetAt(i)->DnLpNds.GetSize() - 1; t++) { //首尾点相同，去掉最后一个点
			dn->iNds.Add(m_aBlks.GetAt(i)->DnLpNds.GetAt(t)->ID);
		}

		m_aBlks.GetAt(i)->pLps.Add(up);  m_aBlks.GetAt(i)->pLps.Add(dn);

		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; j++) {
			CLoop * tmp = new CLoop();
			n0 = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID;
			n1 = m_aBlks.GetAt(i)->DnLpNds.GetAt(j + 1)->ID;
			n2 = m_aBlks.GetAt(i)->UpLpNds.GetAt(j + 1)->ID;
			n3 = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID;

			if(n0!= n3 && n1 != n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n2); tmp->iNds.Add(n3);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
			if(n0 != n3 && n1 == n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n3);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
			if(n0 == n3 && n1 != n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n2);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
		}
	}

	// Begin : transform from block to polyhedron
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CPohd * phd = new CPohd();
		for(j = 0; j < m_aBlks.GetAt(i)->pLps.GetSize(); j++) {
			CLoop * lp = new CLoop();
			for(k = 0; k < m_aBlks.GetAt(i)->pLps.GetAt(j)->iNds.GetSize(); k++) {
				id = m_aBlks.GetAt(i)->pLps.GetAt(j)->iNds.GetAt(k);
				x = pNds.GetAt(id)->x;  y = pNds.GetAt(id)->y;  z = pNds.GetAt(id)->z;
				lp->pNds.Add(new CNode(id, x, y, z));
			}
			phd->pLps.Add(lp);
		}
		pBlk->pPhs.Add(phd);
	}
	// End

	////////Remove the memory
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 
	
	cnt_1 = m_aBlks.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		for(j = m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; j >= 0; j--){
			delete m_aBlks.GetAt(i)->UpLpNds.GetAt(j);  m_aBlks.GetAt(i)->UpLpNds.RemoveAt(j);
		}
		for(j = m_aBlks.GetAt(i)->DnLpNds.GetSize() - 1; j >= 0; j--){
			delete m_aBlks.GetAt(i)->DnLpNds.GetAt(j);  m_aBlks.GetAt(i)->DnLpNds.RemoveAt(j);
		}
	}
	cnt_1 = m_aBlks.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		m_aBlks.GetAt(i)->Clear(); 
		delete m_aBlks.GetAt(i); m_aBlks.RemoveAt(i);    
	}
	m_aBlks.RemoveAll(); 
	cnt_1 = m_aPrs2D.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		delete m_aPrs2D.GetAt(i);  m_aPrs2D.RemoveAt(i);    
	}
	m_aPrs2D.RemoveAll();
	cnt_1 = m_aPrs3D.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		delete m_aPrs3D.GetAt(i);  m_aPrs3D.RemoveAt(i);    
	}
	m_aPrs3D.RemoveAll(); 
	
	Rgns.GetAt(1)->Clear();  delete Rgns.GetAt(1);  Rgns.RemoveAt(1);
	Rgns.GetAt(0)->Clear();  delete Rgns.GetAt(0);  Rgns.RemoveAt(0);

#else
   // to be continue

#endif
	
}


// 2013.04.22 By Mei Gang in freiburg Germany
//To compute the intersections of 2 triangle mesh in 3D and 2D
//parameters: CRegion * pSurf0, CRegion * pSurf1, CBlock * pBlk
//result    : firstly, all the intersections will be saved into m_aBlks
// secondly, transform all elements in M_aBlks to polyhedrons, which will be saved in pBlk->pPhds
void CRealModelView::InsectOfTrglSurf_2DAnd3DBest(CSurf * pSurf0, CSurf * pSurf1, CBlock * pBlk) 
{
#ifndef EGC
	CArray<CPair*, CPair*>m_aPrs2D; 
	CArray<CPair*, CPair*>m_aPrs3D;
	CArray<CBlock*, CBlock*>m_aBlks;

	int Add = pSurf0->pTrs.GetSize();

	// intersection in 2D
	int i, j, k, ii, jj, kk, id, tid[3];
	double x, y, z;
	CArray<CTrgl*, CTrgl*>pTrs;
	CTrgl * pTr0; CTrgl * pTr1;

	CArray<CSurf*, CSurf*>Rgns;
	CSurf * r0 = new CSurf();  CSurf * r1 = new CSurf();
	Rgns.Add(r0);  Rgns.Add(r1);

	for(i = 0; i < pSurf0->pTrs.GetSize(); i++) {
		id = pSurf0->pTrs.GetAt(i)->ID;
		for(j = 0; j < 3; j++) tid[j] = pSurf0->pTrs.GetAt(i)->nID[j];
		r0->pTrs.Add(new CTrgl(id, tid[0], tid[1], tid[2]));
		for(j = 0; j < 3; j++) {
			r0->pTrs.GetAt(i)->x[j] = pSurf0->pTrs.GetAt(i)->x[j];
			r0->pTrs.GetAt(i)->y[j] = pSurf0->pTrs.GetAt(i)->y[j];
			r0->pTrs.GetAt(i)->z[j] = 0.00;
		}
	}
	for(i = 0; i < pSurf0->pNds.GetSize(); i++) {
		id = pSurf0->pNds.GetAt(i)->ID;
		x = pSurf0->pNds.GetAt(i)->x; y = pSurf0->pNds.GetAt(i)->y; z = 0.00;
		r0->pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < pSurf0->pNds.GetSize(); i++){ //记录经过每个点的三角形ID 2010.1.29
		for(j = 0; j < pSurf0->pNds.GetAt(i)->sPsdTrgl.GetSize(); j++){
			r0->pNds.GetAt(i)->sPsdTrgl.Add(pSurf0->pNds.GetAt(i)->sPsdTrgl.GetAt(j));
		}
	}
	for(i = 0; i < pSurf0->pTrs.GetSize(); i++){ //记录每个三角形的邻域三角形，不包括自身
		for(j = 0; j < pSurf0->pTrs.GetAt(i)->iRgnPassTr.GetSize(); j++){
			r0->pTrs.GetAt(i)->iRgnPassTr.Add(pSurf0->pTrs.GetAt(i)->iRgnPassTr.GetAt(j));
		}
	}

	for(i = 0; i < pSurf1->pTrs.GetSize(); i++) {
		id = pSurf1->pTrs.GetAt(i)->ID;
		for(j = 0; j < 3; j++) tid[j] = pSurf1->pTrs.GetAt(i)->nID[j];
		r1->pTrs.Add(new CTrgl(id, tid[0], tid[1], tid[2]));
		for(j = 0; j < 3; j++) {
			r1->pTrs.GetAt(i)->x[j] = pSurf1->pTrs.GetAt(i)->x[j];
			r1->pTrs.GetAt(i)->y[j] = pSurf1->pTrs.GetAt(i)->y[j];
			r1->pTrs.GetAt(i)->z[j] = 0.00;
		}
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++) {
		id = pSurf1->pNds.GetAt(i)->ID;
		x = pSurf1->pNds.GetAt(i)->x; y = pSurf1->pNds.GetAt(i)->y; z = 0.00;
		r1->pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++){ //记录经过每个点的三角形ID 2010.1.29
		for(j = 0; j < pSurf1->pNds.GetAt(i)->sPsdTrgl.GetSize(); j++){
			r1->pNds.GetAt(i)->sPsdTrgl.Add(pSurf1->pNds.GetAt(i)->sPsdTrgl.GetAt(j));
		}
	}
	for(i = 0; i < pSurf1->pTrs.GetSize(); i++){ //记录每个三角形的邻域三角形，不包括自身
		for(j = 0; j < pSurf1->pTrs.GetAt(i)->iRgnPassTr.GetSize(); j++){
			r1->pTrs.GetAt(i)->iRgnPassTr.Add(pSurf1->pTrs.GetAt(i)->iRgnPassTr.GetAt(j));
		}
	}

//	SearchIntrTrgls(r0, r1, myEps); // to search the intersected triangles for each triangle
	SearchInsectPair2D_OBBTree(r0, r1, myEps);

	for(i = 0; i < 2; i++) {
		for(j = 0; j < Rgns.GetAt(i)->pTrs.GetSize(); j++) {
			pTrs.Add( Rgns.GetAt(i)->pTrs.GetAt(j) );
		}
	}

 	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CLoop * pIntr = new CLoop();
			IntrTriangle2Triangle2(pTr0, pTr1, pIntr);
			m_aLpsIntr.Add(pIntr); // Save
			pTr0->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
			pTr1->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
		}
	}

	//统一到一个三角形对的集合内2009.12.17
	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CPair * pr = new CPair();
			pr->FstID = pTr0->ID;  pr->SedID = pTr1->ID;
            m_aPrs2D.Add(pr); // 与m_aLpsIntr一一对应
		}
	}

	CombiLps2D(); // Combine all loops in 2D, 2010.2.8

	////intersection in 3D
	int t;
	int count, nCase;
	double *nx, *ny, *nz, *para;
	double xmin, ymin, zmin, xmax, ymax, zmax;
	int xdivi, ydivi, zdivi;
	double xtmp, ytmp, ztmp;
	double sum, dstep;
	int n0, n1, n2, n3, n4, n5, n6, n7;
	CArray<CNode*, CNode*>pSonNds;
	CArray<CHehd*, CHehd*>pSonBox;
	CNode * pMin, * pMax;
	CArray<CLine*, CLine*> pLns;
	float v0[3], v1[3], v2[3], u0[3], u1[3], u2[3], isec0[3], isec1[3]; // prepare for Moller.dll
	int copla;
	int * coplanar = &copla;
	
	//Find the smallest and the biggest x, y and z, and form a box;
	count = pSurf0->pNds.GetSize() + pSurf1->pNds.GetSize();
	nx = new double[count];  ny = new double[count];  nz = new double[count];
	for(i = 0; i < pSurf0->pNds.GetSize(); i++) {
		nx[i] = pSurf0->pNds.GetAt(i)->x;
		ny[i] = pSurf0->pNds.GetAt(i)->y;
		nz[i] = pSurf0->pNds.GetAt(i)->z;
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++) {
		j = i + pSurf0->pNds.GetSize();
		nx[j] = pSurf1->pNds.GetAt(i)->x;
		ny[j] = pSurf1->pNds.GetAt(i)->y;
		nz[j] = pSurf1->pNds.GetAt(i)->z;
	}
	prhap(nx,count);  prhap(ny,count);  prhap(nz,count);
	xmin = nx[0];        ymin = ny[0];        zmin = nz[0];
	xmax = nx[count-1];  ymax = ny[count-1];  zmax = nz[count-1];
	delete [] nx;  delete [] ny;  delete [] nz;
	
	//Compute all triangles' edges' length, and use it to divide the Box
	sum = 0.0;
	for(i = 0; i < pSurf0->pTrs.GetSize(); i++) {
		CTrgl * tr = pSurf0->pTrs.GetAt(i);
		sum += (tr->x[0] - tr->x[1]) * (tr->x[0] - tr->x[1]) + 
			   (tr->y[0] - tr->y[1]) * (tr->y[0] - tr->y[1]) +
			   (tr->z[0] - tr->z[1]) * (tr->z[0] - tr->z[1]) ;
        sum += (tr->x[1] - tr->x[2]) * (tr->x[1] - tr->x[2]) + 
			   (tr->y[1] - tr->y[2]) * (tr->y[1] - tr->y[2]) +
			   (tr->z[1] - tr->z[2]) * (tr->z[1] - tr->z[2]) ;
        sum += (tr->x[2] - tr->x[0]) * (tr->x[2] - tr->x[0]) + 
			   (tr->y[2] - tr->y[0]) * (tr->y[2] - tr->y[0]) +
			   (tr->z[2] - tr->z[0]) * (tr->z[2] - tr->z[0]) ;
	}
	for(i = 0; i < pSurf1->pTrs.GetSize(); i++) {
		CTrgl * tr = pSurf1->pTrs.GetAt(i);
		sum += (tr->x[0] - tr->x[1]) * (tr->x[0] - tr->x[1]) + 
			   (tr->y[0] - tr->y[1]) * (tr->y[0] - tr->y[1]) +
			   (tr->z[0] - tr->z[1]) * (tr->z[0] - tr->z[1]) ;
        sum += (tr->x[1] - tr->x[2]) * (tr->x[1] - tr->x[2]) + 
			   (tr->y[1] - tr->y[2]) * (tr->y[1] - tr->y[2]) +
			   (tr->z[1] - tr->z[2]) * (tr->z[1] - tr->z[2]) ;
        sum += (tr->x[2] - tr->x[0]) * (tr->x[2] - tr->x[0]) + 
			   (tr->y[2] - tr->y[0]) * (tr->y[2] - tr->y[0]) +
			   (tr->z[2] - tr->z[0]) * (tr->z[2] - tr->z[0]) ;
	}
	sum /= (pSurf0->pTrs.GetSize() + pSurf1->pTrs.GetSize()) * 3; // average length
	dstep = sqrt(sum);
	
	//divisions
	xtmp = (xmax - xmin) / dstep; //x
    xdivi = (int)(xtmp) + 1;
	ytmp = (ymax - ymin) / dstep; //y
    ydivi = (int)(ytmp) + 1;
	ztmp = (zmax - zmin) / dstep; //z
    zdivi = (int)(ztmp) + 1;
	
	// save all son-boxes
	//Nodes
	id = 0;
	for(k = 0; k < zdivi + 1; k++){
		for(j = 0; j < ydivi + 1; j++){
			for(i = 0; i < xdivi + 1; i++){
				x = xmin + i * dstep;    y = ymin + j * dstep;    z = zmin + k * dstep;
				pSonNds.Add(new CNode(id++, x, y, z));
			}
		}
	}
	
	//Son-boxes
	for(k = 0; k < zdivi; k++) {
		for(j = 0; j < ydivi; j++) {
			for(i = 0; i < xdivi; i++) {
				n0 = k * (ydivi + 1) * (xdivi + 1) + j * (xdivi + 1) +i;
				n1=k*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i+1;
				n2=k*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i+1;
				n3=k*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i;
				n4=(k+1)*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i;
				n5=(k+1)*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i+1;
				n6=(k+1)*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i+1;
				n7=(k+1)*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i;
				pSonBox.Add(new CHehd(n0,n1,n2,n3,n4,n5,n6,n7));
			}
		}
	}
	
	//test which triangles intersect each son-box
	for(i = 0; i < pSonBox.GetSize(); i++) {
		pMin = pSonNds.GetAt(pSonBox.GetAt(i)->nID[0]);
		pMax = pSonNds.GetAt(pSonBox.GetAt(i)->nID[6]);
		for(j = 0; j < pSurf0->pTrs.GetSize(); j++) {
			if(IsTrglInsectBox(pSurf0->pTrs.GetAt(j), pMin, pMax) == 1)
				pSonBox.GetAt(i)->iIntrTriRgn0.Add(j);
		}
		for(j = 0; j < pSurf1->pTrs.GetSize(); j++) {
			if(IsTrglInsectBox(pSurf1->pTrs.GetAt(j), pMin, pMax) == 1)
				pSonBox.GetAt(i)->iIntrTriRgn1.Add(j);
		}
	}
	
	//compute the intersection of each triangle pair which is restricted in one son-box
	for(i = 0; i < pSonBox.GetSize(); i++) {
		for(j = 0; j < pSonBox.GetAt(i)->iIntrTriRgn0.GetSize(); j++) {
			n0 = pSonBox.GetAt(i)->iIntrTriRgn0.GetAt(j);
			v0[0] = pSurf0->pTrs.GetAt(n0)->x[0]; 
			v0[1] = pSurf0->pTrs.GetAt(n0)->y[0]; 
			v0[2] = pSurf0->pTrs.GetAt(n0)->z[0];

			v1[0] = pSurf0->pTrs.GetAt(n0)->x[1]; 
			v1[1] = pSurf0->pTrs.GetAt(n0)->y[1]; 
			v1[2] = pSurf0->pTrs.GetAt(n0)->z[1];

			v2[0] = pSurf0->pTrs.GetAt(n0)->x[2]; 
			v2[1] = pSurf0->pTrs.GetAt(n0)->y[2]; 
			v2[2] = pSurf0->pTrs.GetAt(n0)->z[2];

			for(k = 0; k < pSonBox.GetAt(i)->iIntrTriRgn1.GetSize(); k++) {
				n1 = pSonBox.GetAt(i)->iIntrTriRgn1.GetAt(k);
				u0[0] = pSurf1->pTrs.GetAt(n1)->x[0]; 
				u0[1] = pSurf1->pTrs.GetAt(n1)->y[0]; 
				u0[2] = pSurf1->pTrs.GetAt(n1)->z[0];

				u1[0] = pSurf1->pTrs.GetAt(n1)->x[1]; 
				u1[1] = pSurf1->pTrs.GetAt(n1)->y[1]; 
				u1[2] = pSurf1->pTrs.GetAt(n1)->z[1];

				u2[0] = pSurf1->pTrs.GetAt(n1)->x[2]; 
				u2[1] = pSurf1->pTrs.GetAt(n1)->y[2]; 
				u2[2] = pSurf1->pTrs.GetAt(n1)->z[2];
				
				// Load function from DLL Moller.dll 2011.9.30
			    typedef int (*lpFun)(float [3],float [3],float [3],
				                 float [3],float [3],float [3],int *,
				                 float [3],float [3]); // 宏定义函数指针类型
	            HINSTANCE hDll; //DLL 句柄
                lpFun trglFun; // 函数指针

	            hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
	
                if (NULL==hDll) {
                    MessageBox(_T("Fail to load DLL Moller.dll ! "));
	            }
	            else {
		             trglFun = (lpFun) GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
                     if (trglFun != NULL) {
					     nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		             }
		             else {
			             MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		             }
                     FreeLibrary(hDll); // 释放函数
	            }
			    // End loading
				
				if(nCase == 1) { //save intersection segment
					if(pLns.GetSize() < 1) { // save directly
						CLine * ptmp = new CLine();
						ptmp->FstTri = n0;  ptmp->SecTri = n1;
						for(t = 0; t < 3; t++) { ptmp->FstNd[t] = isec0[t];  ptmp->SecNd[t] = isec1[t];} //交点坐标
						pLns.Add(ptmp);
					}
					else {
						bool bexist = false;
						for(t = 0; t < pLns.GetSize(); t++){ 
							if(pLns.GetAt(t)->FstTri == n0 && pLns.GetAt(t)->SecTri == n1){ bexist = true;  break; }
						}
						if(!bexist) { // not exist
							CLine * ptmp = new CLine();
							ptmp->FstTri = n0;  ptmp->SecTri = n1;
							for(t = 0; t < 3; t++) { 
								ptmp->FstNd[t] = isec0[t];  
								ptmp->SecNd[t] = isec1[t];
							} //交点坐标
							pLns.Add(ptmp);
						}
					}
				}
			}
		}
	}
	
	//统一保存到一个三角形对的集合中。2009.12.17
	for(i = 0; i < pLns.GetSize(); i++) {
		CPair * pr = new CPair();
		pr->FstID = pLns.GetAt(i)->FstTri;  pr->SedID = pLns.GetAt(i)->SecTri;
		m_aPrs3D.Add(pr); //与pLns一一对应
	}
	////end of intersection in 3D 2009.1.22

	//2010.1.22 Mei Gang, Freiburg, Germany
	//2009.12.17 23:30  freiburg germany
	////use the 3D intersecton line to cut the 2D intersection loop
	//step 1: 确定2D相交三角形对有那些同时3D相交
	for(i = 0; i < m_aPrs2D.GetSize(); i++) { //initiate
		m_aPrs2D.GetAt(i)->bIntr2D = true;  m_aPrs2D.GetAt(i)->bIntr3D = false;}
	for(i = 0; i < m_aPrs3D.GetSize(); i++) {
		m_aPrs3D.GetAt(i)->bIntr2D = false; m_aPrs3D.GetAt(i)->bIntr3D = true; }

	for(i = 0; i < m_aPrs2D.GetSize(); i++) { //search out the pairs which intersect in both 2D and 3D
		for(j = 0; j < m_aPrs3D.GetSize(); j++) {
			if(m_aPrs2D.GetAt(i)->FstID == m_aPrs3D.GetAt(j)->FstID &&
			   m_aPrs2D.GetAt(i)->SedID == m_aPrs3D.GetAt(j)->SedID ){
				m_aPrs2D.GetAt(i)->bIntr3D = true;  m_aPrs3D.GetAt(j)->bIntr2D = true;
				break;
			}
		}
	}

	/*
	// output temperarily
	CString fnMesh;
	int NumOfPrs = m_aPrs3D.GetSize();
	if( AfxMessageBox("确定保存Temp文件?", MB_YESNOCANCEL) != 6 ) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, "Data File(*.txt)|*.txt", NULL);
	if( Dlg.DoModal() == IDOK ) 
	{
		fnMesh = Dlg.GetPathName(); 
		fnMesh += "-Temp";
		if(fnMesh.Right(4) != ".txt")   fnMesh += ".txt";
		ofstream fin( fnMesh );
		for(i = 0; i < NumOfPrs; i++){
			fin << i << "  " << m_aPrs3D.GetAt(i)->FstID << "  " << m_aPrs3D.GetAt(i)->SedID;
			fin << endl;
		}
		fin.close(); 
		
	}
	exit(0);
	// end tmp
	*/

	// For progress window
	CProgressWnd wndProgress(this, _T("Progress"), TRUE);
	int myRance = m_aPrs2D.GetSize();
	wndProgress.SetRange(0, myRance);
	wndProgress.SetText(_T("This is a progress window...\n\n")
						_T("Try dragging it around or hitting Cancel."));

	// form the blocks
	int situ[2], N;
    CLoop * plp = new CLoop();
	CLoop * pson = new CLoop[2];
	N = m_aPrs2D.GetSize();
	for(i = 0; i < m_aPrs2D.GetSize(); i++) {
		// For progress windown
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) {
			MessageBox(_T("Progress Cancelled"));
			::PostQuitMessage(0);
		}
		// End
		// For output window
		CString str;
		str.Format(_T("Forming Polyhedrons :  i = %d  [ / %d]"), i, N);
		pFrame->AddStrToOutputWnd(1, str);

		if(m_aLpsIntr.GetAt(i)->pNds.GetSize() < 3)  continue;  //intersection in 2D is a point or a segment

		plp->Clear();  pson[0].Clear();  pson[1].Clear(); // initiate, very important!
		
		CBlock * blk = new CBlock();
		blk->FstTri = m_aPrs2D.GetAt(i)->FstID;  blk->SecTri = m_aPrs2D.GetAt(i)->SedID; //index of the two triangles
		if(!m_aPrs2D.GetAt(i)->bIntr3D) { // intersect only in 2D
			for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
				x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;// at present, just need to save x and y
				y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
				blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
			}
			//update Z, according to the equation of the triangle
			n0 = m_aPrs2D.GetAt(i)->FstID;
			nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
			for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
				nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
				ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
				nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
			}
			EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
			for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
				x = blk->DnLpNds.GetAt(j)->x;
				y = blk->DnLpNds.GetAt(j)->y;
				z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
				blk->DnLpNds.GetAt(j)->z = z;
			}
			delete [] nx;  delete [] ny; delete [] nz; delete [] para;
			
			n1 = m_aPrs2D.GetAt(i)->SedID;
			nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
			for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
				nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
				ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
				nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
			}
			EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
			for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
				x = blk->UpLpNds.GetAt(j)->x;
				y = blk->UpLpNds.GetAt(j)->y;
				z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
				blk->UpLpNds.GetAt(j)->z = z;
			}
			delete [] nx;  delete [] ny; delete [] nz; delete [] para;
			
			//* meigang
			// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
			// Begin : Check whether each polyhedron is valid
			bool bValid = true;
			double zup = 0.0, zdn = 0.0;
			if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
				AfxMessageBox(_T("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()"),MB_OK|MB_ICONSTOP);
				return;
			}
			for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
				zup += blk->UpLpNds.GetAt(j)->z;
				zdn += blk->DnLpNds.GetAt(j)->z;
			}
			if(zup > zdn )  
			// End.2010.11.25
			//*/

			m_aBlks.Add(blk);//save
		}
		else { // intersect in both 2D and 3D
			bool bexist = false;
			n0 = m_aPrs2D.GetAt(i)->FstID;
			v0[0] = pSurf0->pTrs.GetAt(n0)->x[0]; v0[1] = pSurf0->pTrs.GetAt(n0)->y[0]; v0[2] = pSurf0->pTrs.GetAt(n0)->z[0];
			v1[0] = pSurf0->pTrs.GetAt(n0)->x[1]; v1[1] = pSurf0->pTrs.GetAt(n0)->y[1]; v1[2] = pSurf0->pTrs.GetAt(n0)->z[1];
			v2[0] = pSurf0->pTrs.GetAt(n0)->x[2]; v2[1] = pSurf0->pTrs.GetAt(n0)->y[2]; v2[2] = pSurf0->pTrs.GetAt(n0)->z[2];
			
			n1 = m_aPrs2D.GetAt(i)->SedID;
			u0[0] = pSurf1->pTrs.GetAt(n1)->x[0]; u0[1] = pSurf1->pTrs.GetAt(n1)->y[0]; u0[2] = pSurf1->pTrs.GetAt(n1)->z[0];
			u1[0] = pSurf1->pTrs.GetAt(n1)->x[1]; u1[1] = pSurf1->pTrs.GetAt(n1)->y[1]; u1[2] = pSurf1->pTrs.GetAt(n1)->z[1];
			u2[0] = pSurf1->pTrs.GetAt(n1)->x[2]; u2[1] = pSurf1->pTrs.GetAt(n1)->y[2]; u2[2] = pSurf1->pTrs.GetAt(n1)->z[2];
			
			// Load function from DLL Moller.dll 2011.9.30
			typedef int (*lpFun)(float V0[3],float V1[3],float V2[3],
				                 float U0[3],float U1[3],float U2[3],int *coplanar,
				                 float isectpt1[3],float isectpt2[3]); // 宏定义函数指针类型
	        HINSTANCE hDll; //DLL 句柄
            lpFun trglFun; // 函数指针

	        hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
	
            if (NULL==hDll) {
                MessageBox(_T("Fail to load DLL Moller.dll ! "));
	        }
	        else {
				trglFun = (lpFun)GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
                if (trglFun != NULL) {
					nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		        }
		        else {
			        MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		        }
                FreeLibrary(hDll); // 释放函数
	        }
			// End loading

			if(nCase == 1) { //intersection is a segment
				//case 1 : 相交线段恰好为2D相交环上的一条边，只生成一个Block
				for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; j++) {
					x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;
					y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
					z = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->z;
					xtmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->x;
					ytmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->y;
					ztmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->z;
					
					if( //判断两条线段相同
						(sqrt((x-isec0[0])*(x-isec0[0])+(y-isec0[1])*(y-isec0[1])) < 0.00001 &&
						sqrt((xtmp-isec1[0])*(xtmp-isec1[0])+(ytmp-isec1[1])*(ytmp-isec1[1]) )< 0.00001) 
						||
						(sqrt((x-isec1[0])*(x-isec1[0])+(y-isec1[1])*(y-isec1[1])) < 0.00001 &&
						sqrt((xtmp-isec0[0])*(xtmp-isec0[0])+(ytmp-isec0[1])*(ytmp-isec0[1]) )< 0.00001)
						) {
						bexist = true;
					}
				}
				
				if(bexist) {
                    CBlock * blk = new CBlock();
					for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;// at present, just need to save x and y
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
						blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
					}

					//update Z, according to the equation of the triangle
					n0 = m_aPrs2D.GetAt(i)->FstID;
					nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
					for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
						nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
						ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
						nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
					}
					EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
					for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
						x = blk->DnLpNds.GetAt(j)->x;
						y = blk->DnLpNds.GetAt(j)->y;
						z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
						blk->DnLpNds.GetAt(j)->z = z;
					}
					delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
					n1 = m_aPrs2D.GetAt(i)->SedID;
					nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
					for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
						nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
						ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
						nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
					}
					EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
					for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
						x = blk->UpLpNds.GetAt(j)->x;
						y = blk->UpLpNds.GetAt(j)->y;
						z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
						blk->UpLpNds.GetAt(j)->z = z;
					}
					delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
					//* meigang
					// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
					// Begin : Check whether each polyhedron is valid
					// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
					// Begin : Check whether each polyhedron is valid
					bool bValid = true;
					double zup = 0.0, zdn = 0.0;
					if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
						AfxMessageBox(_T("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()"),MB_OK|MB_ICONSTOP);
						return;
					}
					for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
						zup += blk->UpLpNds.GetAt(j)->z;
						zdn += blk->DnLpNds.GetAt(j)->z;
					}
					if(zup > zdn )  
					// End.2010.11.25
					//*/
						
					m_aBlks.Add(blk);//save

				}
				else { //case 2 : 相交线段将2D相交环分割为两个子环，生成二个Block
					// 2011.5.21 一下代码有问题，目前为解决！

//					OutputIntr();  exit(0);
					
					int pos[2];//index of the position
					//以isec0所在的线段为新环的起始线段
					for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; j++) {
						CNode * sp0 = m_aLpsIntr.GetAt(i)->pNds.GetAt(j);
						CNode * sp1 = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1);
						CNode * point = new CNode(isec0[0],isec0[1]);
						float p0x = (float) sp0->x;  float p0y = (float) sp0->y;
						float p1x = (float) sp1->x;  float p1y = (float) sp1->y;
						if(IsPointOnSegment(p0x, p0y, p1x, p1y, isec0[0], isec0[1], myEps) == true) {
							pos[0] = j;  break;
						}
					}

					plp->Clear();
					for(t = pos[0]; t < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; t++) {//没有保存最后一个点
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->x;
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->y;
						plp->pNds.Add(new CNode(x,y));
					}
					for(t = 0; t < pos[0]; t++) {//保存了第一个点
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->x;
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->y;
						plp->pNds.Add(new CNode(x,y));
					}
					pos[0] = 0; //update
					plp->pNds.Add(new CNode(plp->pNds.GetAt(0)->x,plp->pNds.GetAt(0)->y)); //最合一个点要和第一个点相同
					
					//isec1的位置
					for(j = 0; j < plp->pNds.GetSize() - 1; j++) {
						CNode * sp0 = plp->pNds.GetAt(j);
						CNode * sp1 = plp->pNds.GetAt(j+1);
						float p0x = (float) sp0->x;  float p0y = (float) sp0->y;
						float p1x = (float) sp1->x;  float p1y = (float) sp1->y;
						if(IsPointOnSegment(p0x, p0y, p1x, p1y, isec1[0], isec1[1], myEps) == true) {
							pos[1] = j;  break;
						}
					}
					
					//form 2 son loops
					pson[0].pNds.Add(new CNode(isec0[0], isec0[1])); //first point
					for(t = 0; t < pos[1]; t++) {
						pson[0].pNds.Add(new CNode(plp->pNds.GetAt(t + 1)->x, plp->pNds.GetAt(t + 1)->y)); //must + 1
					}
                    pson[0].pNds.Add(new CNode(isec1[0], isec1[1])); //last point
					pson[0].pNds.Add(new CNode(isec0[0], isec0[1])); //first point again
					
					pson[1].pNds.Add(new CNode(isec0[0], isec0[1])); //first point
					pson[1].pNds.Add(new CNode(isec1[0], isec1[1])); //second point
					for(t = pos[1]; t < plp->pNds.GetSize() - 1; t++) {
						pson[1].pNds.Add(new CNode(plp->pNds.GetAt(t + 1)->x, plp->pNds.GetAt(t + 1)->y)); //must + 1
					}
					pson[1].pNds.Add(new CNode(isec0[0], isec0[1])); //first point again
					
					//update Z and form blocks
					//m_aBlks
					for(ii = 0; ii < 2; ii++) {
						CBlock * blk = new CBlock();
						for(jj = 0; jj < pson[ii].pNds.GetSize(); jj++) {
							x = pson[ii].pNds.GetAt(jj)->x;  y = pson[ii].pNds.GetAt(jj)->y;
							blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
						}
						//update Z, according to the equation of the triangle
						n0 = m_aPrs2D.GetAt(i)->FstID;
						nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
						for(kk = 0; kk < 3; kk++) { // real coordinates of the triangle, z is not zero
							nx[kk] = pSurf0->pTrs.GetAt(n0)->x[kk];
							ny[kk] = pSurf0->pTrs.GetAt(n0)->y[kk];
							nz[kk] = pSurf0->pTrs.GetAt(n0)->z[kk];
						}
						EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
						for(jj = 0; jj < blk->DnLpNds.GetSize(); jj++) {
							x = blk->DnLpNds.GetAt(jj)->x;
							y = blk->DnLpNds.GetAt(jj)->y;
							z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
							blk->DnLpNds.GetAt(jj)->z = z;
						}
						delete [] nx;  delete [] ny; delete [] nz; delete [] para;
						
						n1 = m_aPrs2D.GetAt(i)->SedID;
						nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
						for(kk = 0; kk < 3; kk++) { // real coordinates of the triangle, z is not zero
							nx[kk] = pSurf1->pTrs.GetAt(n1)->x[kk];
							ny[kk] = pSurf1->pTrs.GetAt(n1)->y[kk];
							nz[kk] = pSurf1->pTrs.GetAt(n1)->z[kk];
						}
						EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
						for(jj = 0; jj < blk->UpLpNds.GetSize(); jj++) {
							x = blk->UpLpNds.GetAt(jj)->x;
							y = blk->UpLpNds.GetAt(jj)->y;
							z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
							blk->UpLpNds.GetAt(jj)->z = z;
						}
						delete [] nx;  delete [] ny; delete [] nz; delete [] para;
						
						//* meigang
						// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
						// Begin : Check whether each polyhedron is valid
						bool bValid = true;
						double zup = 0.0, zdn = 0.0;
						if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
						    AfxMessageBox(_T("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()"),MB_OK|MB_ICONSTOP);
							return;
						}
						for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
							zup += blk->UpLpNds.GetAt(j)->z;
							zdn += blk->DnLpNds.GetAt(j)->z;
						}
						if(zup > zdn )  
							// End.2010.11.25
						//*/

						m_aBlks.Add(blk);
					}

					
				} // end else
			}
		}
	
	}



	/*
	// 2011.5.20 注释
	// 原因：当相交的两个面为平面时，以下代码不会产生问题；当为曲面时，出现错误，故注释
	
	//避免实际在平面上的点不落在同一个平面上，统一计算相同面上点的Z 2010.2.7
	//update Z, according to the equation of the triangle
	n0 = pSurf0->pTrs.GetSize() - 1;
	nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
	for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
		nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
		ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
		nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
	}
	EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
			x = blk->DnLpNds.GetAt(j)->x;
			y = blk->DnLpNds.GetAt(j)->y;
			z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
			blk->DnLpNds.GetAt(j)->z = z;
		}
	}
	delete [] nx;  delete [] ny; delete [] nz; delete [] para;
	
	n1 = pSurf1->pTrs.GetSize() - 1;
	nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
	for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
		nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
		ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
		nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
	}
	EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
			x = blk->UpLpNds.GetAt(j)->x;
			y = blk->UpLpNds.GetAt(j)->y;
			z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
			blk->UpLpNds.GetAt(j)->z = z;
		}
	}
	delete [] nx;  delete [] ny; delete [] nz; delete [] para;
	*/

	// 2013.04.22
	// merge all the blocks , and then rearrange and update them
	CString str; // For output window
	str.Format(_T("Starting merging all points !"));
	pFrame->AddStrToOutputWnd(1, str);

	CArray<CNode*, CNode*> pNds;

	id = 0;
	for(i = 0; i < m_aBlks.GetSize(); i++) { // original nodes of blocks
		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize(); j++) {
			x = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->x;
			y = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->y;
			z = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->z;
			m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID = id;
			pNds.Add(new CNode(id++, x, y, z));
		}
		for(j = 0; j < m_aBlks.GetAt(i)->DnLpNds.GetSize(); j++) {
			x = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->x;
			y = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->y;
			z = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->z;
			m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID = id;
			pNds.Add(new CNode(id++, x, y, z));
		}
	}

	//sort the nodes
	int cnt_1, idx; // int count

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; 
		p[idx].y = pNds.GetAt(j)->y; 
		p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}

	//sort nodes
    SortNode(p, cnt_1);

	//save each node for one time
	int * newIDs = new int[cnt_1];

	id = 0;
	newIDs[0] = 0;

	for(i = 0; i < cnt_1 - 1; i++){ // new IDs, important
		j = i + 1;
		if(IsSameNode(p[i].x, p[i].y, p[i].z, p[j].x, p[j].y, p[j].z, myEps) == false ) { 
//		if(IsSameNode(p[i].x, p[i].y, p[i].z, p[j].x, p[j].y, p[j].z, 0.0001) == false ) { 
			id++;
		}
		newIDs[j] = id;
	}

	// Update : 1st step
	for(i = 0; i < cnt_1; i++) {
		id = p[i].ID;
		pNds.GetAt(id)->ID = newIDs[i];
	}

	// Update : 2nd step
	for(i = 0; i < m_aBlks.GetSize(); i++) { // original nodes of blocks
		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID;
			m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID = pNds.GetAt(id)->ID;
		}
		for(j = 0; j < m_aBlks.GetAt(i)->DnLpNds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID;
			m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID = pNds.GetAt(id)->ID;
		}
	}

	// Save : after updating , important 
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 

	i = 0;
	x = p[i].x;  y = p[i].y;  z = p[i].z;
	pNds.Add(new CNode(i, x, y, z));
	for(i = 1; i < cnt_1; i++) {
		if(newIDs[i] != newIDs[i-1]) {
			x = p[i].x;  y = p[i].y;  z = p[i].z;
	        pNds.Add(new CNode(i, x, y, z));
		}
	}

	// transform from block to polyhedron, now a block includes a groups of polyhedrons
	// not just represents only one polyhedron
	for(i = 0; i < m_aBlks.GetSize(); i++ ) { //根据拓扑关系生成Areas
		CLoop * up = new CLoop();
		for(t = 0; t < m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; t++) { //首尾点相同，去掉最后一个点
			up->iNds.Add(m_aBlks.GetAt(i)->UpLpNds.GetAt(t)->ID);
		}
		CLoop * dn = new CLoop();
		for(t = 0; t < m_aBlks.GetAt(i)->DnLpNds.GetSize() - 1; t++) { //首尾点相同，去掉最后一个点
			dn->iNds.Add(m_aBlks.GetAt(i)->DnLpNds.GetAt(t)->ID);
		}

		m_aBlks.GetAt(i)->pLps.Add(up);  m_aBlks.GetAt(i)->pLps.Add(dn);

		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; j++) {
			CLoop * tmp = new CLoop();
			n0 = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID;
			n1 = m_aBlks.GetAt(i)->DnLpNds.GetAt(j + 1)->ID;
			n2 = m_aBlks.GetAt(i)->UpLpNds.GetAt(j + 1)->ID;
			n3 = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID;

			if(n0!= n3 && n1 != n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n2); tmp->iNds.Add(n3);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
			if(n0 != n3 && n1 == n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n3);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
			if(n0 == n3 && n1 != n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n2);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
		}
	}

	// Begin : transform from block to polyhedron
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CPohd * phd = new CPohd();
		for(j = 0; j < m_aBlks.GetAt(i)->pLps.GetSize(); j++) {
			CLoop * lp = new CLoop();
			for(k = 0; k < m_aBlks.GetAt(i)->pLps.GetAt(j)->iNds.GetSize(); k++) {
				id = m_aBlks.GetAt(i)->pLps.GetAt(j)->iNds.GetAt(k);
				x = pNds.GetAt(id)->x;  y = pNds.GetAt(id)->y;  z = pNds.GetAt(id)->z;
				lp->pNds.Add(new CNode(id, x, y, z));
			}
			phd->pLps.Add(lp);
		}
		pBlk->pPhs.Add(phd);
	}
	// End

	////////Remove the memory
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 
	
	cnt_1 = m_aBlks.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		for(j = m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; j >= 0; j--){
			delete m_aBlks.GetAt(i)->UpLpNds.GetAt(j);  m_aBlks.GetAt(i)->UpLpNds.RemoveAt(j);
		}
		for(j = m_aBlks.GetAt(i)->DnLpNds.GetSize() - 1; j >= 0; j--){
			delete m_aBlks.GetAt(i)->DnLpNds.GetAt(j);  m_aBlks.GetAt(i)->DnLpNds.RemoveAt(j);
		}
	}
	cnt_1 = m_aBlks.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		m_aBlks.GetAt(i)->Clear(); 
		delete m_aBlks.GetAt(i); m_aBlks.RemoveAt(i);    
	}
	m_aBlks.RemoveAll(); 
	cnt_1 = m_aPrs2D.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		delete m_aPrs2D.GetAt(i);  m_aPrs2D.RemoveAt(i);    
	}
	m_aPrs2D.RemoveAll();
	cnt_1 = m_aPrs3D.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		delete m_aPrs3D.GetAt(i);  m_aPrs3D.RemoveAt(i);    
	}
	m_aPrs3D.RemoveAll(); 
	
	Rgns.GetAt(1)->Clear();  delete Rgns.GetAt(1);  Rgns.RemoveAt(1);
	Rgns.GetAt(0)->Clear();  delete Rgns.GetAt(0);  Rgns.RemoveAt(0);

#else
   // to be continue

	CArray<CPair*, CPair*>m_aPrs2D; 
	CArray<CPair*, CPair*>m_aPrs3D;
	CArray<CBlock*, CBlock*>m_aBlks;

	int Add = pSurf0->pTrs.GetSize();

	// intersection in 2D
	int i, j, k, ii, jj, kk, id, tid[3];
	double x, y, z;
	CArray<CTrgl*, CTrgl*>pTrs;
	CTrgl * pTr0; CTrgl * pTr1;

	CArray<CSurf*, CSurf*>Rgns;
	CSurf * r0 = new CSurf();  CSurf * r1 = new CSurf();
	Rgns.Add(r0);  Rgns.Add(r1);

	for(i = 0; i < pSurf0->pTrs.GetSize(); i++) {
		id = pSurf0->pTrs.GetAt(i)->ID;
		for(j = 0; j < 3; j++) tid[j] = pSurf0->pTrs.GetAt(i)->nID[j];
		r0->pTrs.Add(new CTrgl(id, tid[0], tid[1], tid[2]));
		for(j = 0; j < 3; j++) {
			r0->pTrs.GetAt(i)->x[j] = pSurf0->pTrs.GetAt(i)->x[j];
			r0->pTrs.GetAt(i)->y[j] = pSurf0->pTrs.GetAt(i)->y[j];
			r0->pTrs.GetAt(i)->z[j] = 0.00;
		}
	}
	for(i = 0; i < pSurf0->pNds.GetSize(); i++) {
		id = pSurf0->pNds.GetAt(i)->ID;
		x = pSurf0->pNds.GetAt(i)->x; y = pSurf0->pNds.GetAt(i)->y; z = 0.00;
		r0->pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < pSurf0->pNds.GetSize(); i++){ //记录经过每个点的三角形ID 2010.1.29
		for(j = 0; j < pSurf0->pNds.GetAt(i)->sPsdTrgl.GetSize(); j++){
			r0->pNds.GetAt(i)->sPsdTrgl.Add(pSurf0->pNds.GetAt(i)->sPsdTrgl.GetAt(j));
		}
	}
	for(i = 0; i < pSurf0->pTrs.GetSize(); i++){ //记录每个三角形的邻域三角形，不包括自身
		for(j = 0; j < pSurf0->pTrs.GetAt(i)->iRgnPassTr.GetSize(); j++){
			r0->pTrs.GetAt(i)->iRgnPassTr.Add(pSurf0->pTrs.GetAt(i)->iRgnPassTr.GetAt(j));
		}
	}

	for(i = 0; i < pSurf1->pTrs.GetSize(); i++) {
		id = pSurf1->pTrs.GetAt(i)->ID;
		for(j = 0; j < 3; j++) tid[j] = pSurf1->pTrs.GetAt(i)->nID[j];
		r1->pTrs.Add(new CTrgl(id, tid[0], tid[1], tid[2]));
		for(j = 0; j < 3; j++) {
			r1->pTrs.GetAt(i)->x[j] = pSurf1->pTrs.GetAt(i)->x[j];
			r1->pTrs.GetAt(i)->y[j] = pSurf1->pTrs.GetAt(i)->y[j];
			r1->pTrs.GetAt(i)->z[j] = 0.00;
		}
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++) {
		id = pSurf1->pNds.GetAt(i)->ID;
		x = pSurf1->pNds.GetAt(i)->x; y = pSurf1->pNds.GetAt(i)->y; z = 0.00;
		r1->pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++){ //记录经过每个点的三角形ID 2010.1.29
		for(j = 0; j < pSurf1->pNds.GetAt(i)->sPsdTrgl.GetSize(); j++){
			r1->pNds.GetAt(i)->sPsdTrgl.Add(pSurf1->pNds.GetAt(i)->sPsdTrgl.GetAt(j));
		}
	}
	for(i = 0; i < pSurf1->pTrs.GetSize(); i++){ //记录每个三角形的邻域三角形，不包括自身
		for(j = 0; j < pSurf1->pTrs.GetAt(i)->iRgnPassTr.GetSize(); j++){
			r1->pTrs.GetAt(i)->iRgnPassTr.Add(pSurf1->pTrs.GetAt(i)->iRgnPassTr.GetAt(j));
		}
	}

//	SearchIntrTrgls(r0, r1, myEps); // to search the intersected triangles for each triangle
	SearchInsectPair2D_OBBTree(r0, r1, myEps);

	for(i = 0; i < 2; i++) {
		for(j = 0; j < Rgns.GetAt(i)->pTrs.GetSize(); j++) {
			pTrs.Add( Rgns.GetAt(i)->pTrs.GetAt(j) );
		}
	}

 	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CLoop * pIntr = new CLoop();
			IntrTriangle2Triangle2(pTr0, pTr1, pIntr);
			m_aLpsIntr.Add(pIntr); // Save
			pTr0->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
			pTr1->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
		}
	}

	//统一到一个三角形对的集合内2009.12.17
	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CPair * pr = new CPair();
			pr->FstID = pTr0->ID;  pr->SedID = pTr1->ID;
            m_aPrs2D.Add(pr); // 与m_aLpsIntr一一对应
		}
	}

	CombiLps2D(); // Combine all loops in 2D, 2010.2.8

	////intersection in 3D
	int t;
	int count, nCase;
	double *nx, *ny, *nz, *para;
	double xmin, ymin, zmin, xmax, ymax, zmax;
	int xdivi, ydivi, zdivi;
	double xtmp, ytmp, ztmp;
	double sum, dstep;
	int n0, n1, n2, n3, n4, n5, n6, n7;
	CArray<CNode*, CNode*>pSonNds;
	CArray<CHehd*, CHehd*>pSonBox;
	CNode * pMin, * pMax;
	CArray<CLine*, CLine*> pLns;
	float v0[3], v1[3], v2[3], u0[3], u1[3], u2[3], isec0[3], isec1[3]; // prepare for Moller.dll
	int copla;
	int * coplanar = &copla;
	
	//Find the smallest and the biggest x, y and z, and form a box;
	count = pSurf0->pNds.GetSize() + pSurf1->pNds.GetSize();
	nx = new double[count];  ny = new double[count];  nz = new double[count];
	for(i = 0; i < pSurf0->pNds.GetSize(); i++) {
		nx[i] = pSurf0->pNds.GetAt(i)->x;
		ny[i] = pSurf0->pNds.GetAt(i)->y;
		nz[i] = pSurf0->pNds.GetAt(i)->z;
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++) {
		j = i + pSurf0->pNds.GetSize();
		nx[j] = pSurf1->pNds.GetAt(i)->x;
		ny[j] = pSurf1->pNds.GetAt(i)->y;
		nz[j] = pSurf1->pNds.GetAt(i)->z;
	}
	prhap(nx,count);  prhap(ny,count);  prhap(nz,count);
	xmin = nx[0];        ymin = ny[0];        zmin = nz[0];
	xmax = nx[count-1];  ymax = ny[count-1];  zmax = nz[count-1];
	delete [] nx;  delete [] ny;  delete [] nz;
	
	//Compute all triangles' edges' length, and use it to divide the Box
	sum = 0.0;
	for(i = 0; i < pSurf0->pTrs.GetSize(); i++) {
		CTrgl * tr = pSurf0->pTrs.GetAt(i);
		sum += (tr->x[0] - tr->x[1]) * (tr->x[0] - tr->x[1]) + 
			   (tr->y[0] - tr->y[1]) * (tr->y[0] - tr->y[1]) +
			   (tr->z[0] - tr->z[1]) * (tr->z[0] - tr->z[1]) ;
        sum += (tr->x[1] - tr->x[2]) * (tr->x[1] - tr->x[2]) + 
			   (tr->y[1] - tr->y[2]) * (tr->y[1] - tr->y[2]) +
			   (tr->z[1] - tr->z[2]) * (tr->z[1] - tr->z[2]) ;
        sum += (tr->x[2] - tr->x[0]) * (tr->x[2] - tr->x[0]) + 
			   (tr->y[2] - tr->y[0]) * (tr->y[2] - tr->y[0]) +
			   (tr->z[2] - tr->z[0]) * (tr->z[2] - tr->z[0]) ;
	}
	for(i = 0; i < pSurf1->pTrs.GetSize(); i++) {
		CTrgl * tr = pSurf1->pTrs.GetAt(i);
		sum += (tr->x[0] - tr->x[1]) * (tr->x[0] - tr->x[1]) + 
			   (tr->y[0] - tr->y[1]) * (tr->y[0] - tr->y[1]) +
			   (tr->z[0] - tr->z[1]) * (tr->z[0] - tr->z[1]) ;
        sum += (tr->x[1] - tr->x[2]) * (tr->x[1] - tr->x[2]) + 
			   (tr->y[1] - tr->y[2]) * (tr->y[1] - tr->y[2]) +
			   (tr->z[1] - tr->z[2]) * (tr->z[1] - tr->z[2]) ;
        sum += (tr->x[2] - tr->x[0]) * (tr->x[2] - tr->x[0]) + 
			   (tr->y[2] - tr->y[0]) * (tr->y[2] - tr->y[0]) +
			   (tr->z[2] - tr->z[0]) * (tr->z[2] - tr->z[0]) ;
	}
	sum /= (pSurf0->pTrs.GetSize() + pSurf1->pTrs.GetSize()) * 3; // average length
	dstep = sqrt(sum);
	
	//divisions
	xtmp = (xmax - xmin) / dstep; //x
	xdivi = xtmp.intValue() + 1;
	ytmp = (ymax - ymin) / dstep; //y
	ydivi = ytmp.intValue() + 1;
	ztmp = (zmax - zmin) / dstep; //z
	zdivi = ztmp.intValue() + 1;
	
	// save all son-boxes
	//Nodes
	id = 0;
	for(k = 0; k < zdivi + 1; k++){
		for(j = 0; j < ydivi + 1; j++){
			for(i = 0; i < xdivi + 1; i++){
				x = xmin + i * dstep;    y = ymin + j * dstep;    z = zmin + k * dstep;
				pSonNds.Add(new CNode(id++, x, y, z));
			}
		}
	}
	
	//Son-boxes
	for(k = 0; k < zdivi; k++) {
		for(j = 0; j < ydivi; j++) {
			for(i = 0; i < xdivi; i++) {
				n0 = k * (ydivi + 1) * (xdivi + 1) + j * (xdivi + 1) +i;
				n1=k*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i+1;
				n2=k*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i+1;
				n3=k*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i;
				n4=(k+1)*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i;
				n5=(k+1)*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i+1;
				n6=(k+1)*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i+1;
				n7=(k+1)*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i;
				pSonBox.Add(new CHehd(n0,n1,n2,n3,n4,n5,n6,n7));
			}
		}
	}
	
	//test which triangles intersect each son-box
	for(i = 0; i < pSonBox.GetSize(); i++) {
		pMin = pSonNds.GetAt(pSonBox.GetAt(i)->nID[0]);
		pMax = pSonNds.GetAt(pSonBox.GetAt(i)->nID[6]);
		for(j = 0; j < pSurf0->pTrs.GetSize(); j++) {
			if(IsTrglInsectBox(pSurf0->pTrs.GetAt(j), pMin, pMax) == 1)
				pSonBox.GetAt(i)->iIntrTriRgn0.Add(j);
		}
		for(j = 0; j < pSurf1->pTrs.GetSize(); j++) {
			if(IsTrglInsectBox(pSurf1->pTrs.GetAt(j), pMin, pMax) == 1)
				pSonBox.GetAt(i)->iIntrTriRgn1.Add(j);
		}
	}
	
	//compute the intersection of each triangle pair which is restricted in one son-box
	for(i = 0; i < pSonBox.GetSize(); i++) {
		for(j = 0; j < pSonBox.GetAt(i)->iIntrTriRgn0.GetSize(); j++) {
			n0 = pSonBox.GetAt(i)->iIntrTriRgn0.GetAt(j);
			v0[0] = pSurf0->pTrs.GetAt(n0)->x[0].floatValue(); 
			v0[1] = pSurf0->pTrs.GetAt(n0)->y[0].floatValue(); 
			v0[2] = pSurf0->pTrs.GetAt(n0)->z[0].floatValue();

			v1[0] = pSurf0->pTrs.GetAt(n0)->x[1].floatValue(); 
			v1[1] = pSurf0->pTrs.GetAt(n0)->y[1].floatValue(); 
			v1[2] = pSurf0->pTrs.GetAt(n0)->z[1].floatValue();

			v2[0] = pSurf0->pTrs.GetAt(n0)->x[2].floatValue(); 
			v2[1] = pSurf0->pTrs.GetAt(n0)->y[2].floatValue(); 
			v2[2] = pSurf0->pTrs.GetAt(n0)->z[2].floatValue();

			for(k = 0; k < pSonBox.GetAt(i)->iIntrTriRgn1.GetSize(); k++) {
				n1 = pSonBox.GetAt(i)->iIntrTriRgn1.GetAt(k);
				u0[0] = pSurf1->pTrs.GetAt(n1)->x[0].floatValue(); 
				u0[1] = pSurf1->pTrs.GetAt(n1)->y[0].floatValue(); 
				u0[2] = pSurf1->pTrs.GetAt(n1)->z[0].floatValue();

				u1[0] = pSurf1->pTrs.GetAt(n1)->x[1].floatValue(); 
				u1[1] = pSurf1->pTrs.GetAt(n1)->y[1].floatValue(); 
				u1[2] = pSurf1->pTrs.GetAt(n1)->z[1].floatValue();

				u2[0] = pSurf1->pTrs.GetAt(n1)->x[2].floatValue(); 
				u2[1] = pSurf1->pTrs.GetAt(n1)->y[2].floatValue(); 
				u2[2] = pSurf1->pTrs.GetAt(n1)->z[2].floatValue();
				
				// Load function from DLL Moller.dll 2011.9.30
			    typedef int (*lpFun)(float [3],float [3],float [3],
				                 float [3],float [3],float [3],int *,
				                 float [3],float [3]); // 宏定义函数指针类型
	            HINSTANCE hDll; //DLL 句柄
                lpFun trglFun; // 函数指针

	            hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
	
                if (NULL==hDll) {
                    MessageBox(_T("Fail to load DLL Moller.dll ! "));
	            }
	            else {
		             trglFun = (lpFun) GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
                     if (trglFun != NULL) {
					     nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		             }
		             else {
			             MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		             }
                     FreeLibrary(hDll); // 释放函数
	            }
			    // End loading
				
				if(nCase == 1) { //save intersection segment
					if(pLns.GetSize() < 1) { // save directly
						CLine * ptmp = new CLine();
						ptmp->FstTri = n0;  ptmp->SecTri = n1;
						for(t = 0; t < 3; t++) { ptmp->FstNd[t] = isec0[t];  ptmp->SecNd[t] = isec1[t];} //交点坐标
						pLns.Add(ptmp);
					}
					else {
						bool bexist = false;
						for(t = 0; t < pLns.GetSize(); t++){ 
							if(pLns.GetAt(t)->FstTri == n0 && pLns.GetAt(t)->SecTri == n1){ bexist = true;  break; }
						}
						if(!bexist) { // not exist
							CLine * ptmp = new CLine();
							ptmp->FstTri = n0;  ptmp->SecTri = n1;
							for(t = 0; t < 3; t++) { 
								ptmp->FstNd[t] = isec0[t];  
								ptmp->SecNd[t] = isec1[t];
							} //交点坐标
							pLns.Add(ptmp);
						}
					}
				}
			}
		}
	}
	
	//统一保存到一个三角形对的集合中。2009.12.17
	for(i = 0; i < pLns.GetSize(); i++) {
		CPair * pr = new CPair();
		pr->FstID = pLns.GetAt(i)->FstTri;  pr->SedID = pLns.GetAt(i)->SecTri;
		m_aPrs3D.Add(pr); //与pLns一一对应
	}
	////end of intersection in 3D 2009.1.22

	//2010.1.22 Mei Gang, Freiburg, Germany
	//2009.12.17 23:30  freiburg germany
	////use the 3D intersecton line to cut the 2D intersection loop
	//step 1: 确定2D相交三角形对有那些同时3D相交
	for(i = 0; i < m_aPrs2D.GetSize(); i++) { //initiate
		m_aPrs2D.GetAt(i)->bIntr2D = true;  m_aPrs2D.GetAt(i)->bIntr3D = false;}
	for(i = 0; i < m_aPrs3D.GetSize(); i++) {
		m_aPrs3D.GetAt(i)->bIntr2D = false; m_aPrs3D.GetAt(i)->bIntr3D = true; }

	for(i = 0; i < m_aPrs2D.GetSize(); i++) { //search out the pairs which intersect in both 2D and 3D
		for(j = 0; j < m_aPrs3D.GetSize(); j++) {
			if(m_aPrs2D.GetAt(i)->FstID == m_aPrs3D.GetAt(j)->FstID &&
			   m_aPrs2D.GetAt(i)->SedID == m_aPrs3D.GetAt(j)->SedID ){
				m_aPrs2D.GetAt(i)->bIntr3D = true;  m_aPrs3D.GetAt(j)->bIntr2D = true;
				break;
			}
		}
	}

	/*
	// output temperarily
	CString fnMesh;
	int NumOfPrs = m_aPrs3D.GetSize();
	if( AfxMessageBox("确定保存Temp文件?", MB_YESNOCANCEL) != 6 ) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, "Data File(*.txt)|*.txt", NULL);
	if( Dlg.DoModal() == IDOK ) 
	{
		fnMesh = Dlg.GetPathName(); 
		fnMesh += "-Temp";
		if(fnMesh.Right(4) != ".txt")   fnMesh += ".txt";
		ofstream fin( fnMesh );
		for(i = 0; i < NumOfPrs; i++){
			fin << i << "  " << m_aPrs3D.GetAt(i)->FstID << "  " << m_aPrs3D.GetAt(i)->SedID;
			fin << endl;
		}
		fin.close(); 
		
	}
	exit(0);
	// end tmp
	*/

	/*
	// For progress window
	CProgressWnd wndProgress(this, _T("Progress"), TRUE);
	int myRance = m_aPrs2D.GetSize();
	wndProgress.SetRange(0, myRance);
	wndProgress.SetText(_T("This is a progress window...\n\n")
						_T("Try dragging it around or hitting Cancel."));
    */

	// form the blocks
	int situ[2], N;
    CLoop * plp = new CLoop();
	CLoop * pson = new CLoop[2];
	N = m_aPrs2D.GetSize();
	for(i = 0; i < m_aPrs2D.GetSize(); i++) {
		/*
		// For progress windown
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) {
			MessageBox(_T("Progress Cancelled"));
			::PostQuitMessage(0);
		}
		// End
		*/

		// For output window
		CString str;
		str.Format(_T("Forming Polyhedrons :  i = %d  [ / %d]"), i, N);
		pFrame->AddStrToOutputWnd(1, str);
		

		if(m_aLpsIntr.GetAt(i)->pNds.GetSize() < 3)  continue;  //intersection in 2D is a point or a segment

		plp->Clear();  pson[0].Clear();  pson[1].Clear(); // initiate, very important!
		
		CBlock * blk = new CBlock();
		blk->FstTri = m_aPrs2D.GetAt(i)->FstID;  blk->SecTri = m_aPrs2D.GetAt(i)->SedID; //index of the two triangles
		if(!m_aPrs2D.GetAt(i)->bIntr3D) { // intersect only in 2D
			for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
				x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;// at present, just need to save x and y
				y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
				blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
			}
			//update Z, according to the equation of the triangle
			n0 = m_aPrs2D.GetAt(i)->FstID;
			nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
			for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
				nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
				ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
				nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
			}
			EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
			for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
				x = blk->DnLpNds.GetAt(j)->x;
				y = blk->DnLpNds.GetAt(j)->y;
				z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
				blk->DnLpNds.GetAt(j)->z = z;
			}
			delete [] nx;  delete [] ny; delete [] nz; delete [] para;
			
			n1 = m_aPrs2D.GetAt(i)->SedID;
			nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
			for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
				nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
				ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
				nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
			}
			EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
			for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
				x = blk->UpLpNds.GetAt(j)->x;
				y = blk->UpLpNds.GetAt(j)->y;
				z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
				blk->UpLpNds.GetAt(j)->z = z;
			}
			delete [] nx;  delete [] ny; delete [] nz; delete [] para;
			
			/* meigang
			// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
			// Begin : Check whether each polyhedron is valid
			bool bValid = true;
			double zup = 0.0, zdn = 0.0;
			if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
				AfxMessageBox(_T("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()"),MB_OK|MB_ICONSTOP);
				return;
			}
			for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
				zup += blk->UpLpNds.GetAt(j)->z;
				zdn += blk->DnLpNds.GetAt(j)->z;
			}
			if(zup > zdn )  
			// End.2010.11.25
			//*/

			m_aBlks.Add(blk);//save
		}
		else { // intersect in both 2D and 3D
			bool bexist = false;
			n0 = m_aPrs2D.GetAt(i)->FstID;
			v0[0] = pSurf0->pTrs.GetAt(n0)->x[0].floatValue(); 
			v0[1] = pSurf0->pTrs.GetAt(n0)->y[0].floatValue(); 
			v0[2] = pSurf0->pTrs.GetAt(n0)->z[0].floatValue();
			v1[0] = pSurf0->pTrs.GetAt(n0)->x[1].floatValue(); 
			v1[1] = pSurf0->pTrs.GetAt(n0)->y[1].floatValue(); 
			v1[2] = pSurf0->pTrs.GetAt(n0)->z[1].floatValue();
			v2[0] = pSurf0->pTrs.GetAt(n0)->x[2].floatValue(); 
			v2[1] = pSurf0->pTrs.GetAt(n0)->y[2].floatValue(); 
			v2[2] = pSurf0->pTrs.GetAt(n0)->z[2].floatValue();
			
			n1 = m_aPrs2D.GetAt(i)->SedID;
			u0[0] = pSurf1->pTrs.GetAt(n1)->x[0].floatValue(); 
			u0[1] = pSurf1->pTrs.GetAt(n1)->y[0].floatValue(); 
			u0[2] = pSurf1->pTrs.GetAt(n1)->z[0].floatValue();
			u1[0] = pSurf1->pTrs.GetAt(n1)->x[1].floatValue(); 
			u1[1] = pSurf1->pTrs.GetAt(n1)->y[1].floatValue(); 
			u1[2] = pSurf1->pTrs.GetAt(n1)->z[1].floatValue();
			u2[0] = pSurf1->pTrs.GetAt(n1)->x[2].floatValue(); 
			u2[1] = pSurf1->pTrs.GetAt(n1)->y[2].floatValue(); 
			u2[2] = pSurf1->pTrs.GetAt(n1)->z[2].floatValue();
			
			// Load function from DLL Moller.dll 2011.9.30
			typedef int (*lpFun)(float V0[3],float V1[3],float V2[3],
				                 float U0[3],float U1[3],float U2[3],int *coplanar,
				                 float isectpt1[3],float isectpt2[3]); // 宏定义函数指针类型
	        HINSTANCE hDll; //DLL 句柄
            lpFun trglFun; // 函数指针

	        hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
	
            if (NULL==hDll) {
                MessageBox(_T("Fail to load DLL Moller.dll ! "));
	        }
	        else {
				trglFun = (lpFun)GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
                if (trglFun != NULL) {
					nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		        }
		        else {
			        MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		        }
                FreeLibrary(hDll); // 释放函数
	        }
			// End loading

			if(nCase == 1) { //intersection is a segment
				//case 1 : 相交线段恰好为2D相交环上的一条边，只生成一个Block
				for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; j++) {
					x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;
					y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
					z = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->z;
					xtmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->x;
					ytmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->y;
					ztmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->z;
					
					if( //判断两条线段相同
						(sqrt((x-isec0[0])*(x-isec0[0])+(y-isec0[1])*(y-isec0[1])) < 0.00001 &&
						sqrt((xtmp-isec1[0])*(xtmp-isec1[0])+(ytmp-isec1[1])*(ytmp-isec1[1]) )< 0.00001) 
						||
						(sqrt((x-isec1[0])*(x-isec1[0])+(y-isec1[1])*(y-isec1[1])) < 0.00001 &&
						sqrt((xtmp-isec0[0])*(xtmp-isec0[0])+(ytmp-isec0[1])*(ytmp-isec0[1]) )< 0.00001)
						) {
						bexist = true;
					}
				}
				
				if(bexist) {
                    CBlock * blk = new CBlock();
					for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;// at present, just need to save x and y
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
						blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
					}

					//update Z, according to the equation of the triangle
					n0 = m_aPrs2D.GetAt(i)->FstID;
					nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
					for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
						nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
						ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
						nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
					}
					EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
					for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
						x = blk->DnLpNds.GetAt(j)->x;
						y = blk->DnLpNds.GetAt(j)->y;
						z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
						blk->DnLpNds.GetAt(j)->z = z;
					}
					delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
					n1 = m_aPrs2D.GetAt(i)->SedID;
					nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
					for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
						nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
						ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
						nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
					}
					EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
					for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
						x = blk->UpLpNds.GetAt(j)->x;
						y = blk->UpLpNds.GetAt(j)->y;
						z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
						blk->UpLpNds.GetAt(j)->z = z;
					}
					delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
					/* meigang
					// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
					// Begin : Check whether each polyhedron is valid
					// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
					// Begin : Check whether each polyhedron is valid
					bool bValid = true;
					double zup = 0.0, zdn = 0.0;
					if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
						AfxMessageBox(_T("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()"),MB_OK|MB_ICONSTOP);
						return;
					}
					for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
						zup += blk->UpLpNds.GetAt(j)->z;
						zdn += blk->DnLpNds.GetAt(j)->z;
					}
					if(zup > zdn )  
					// End.2010.11.25
					//*/
						
					m_aBlks.Add(blk);//save

				}
				else { //case 2 : 相交线段将2D相交环分割为两个子环，生成二个Block
					// 2011.5.21 一下代码有问题，目前为解决！

//					OutputIntr();  exit(0);
					
					int pos[2];//index of the position
					//以isec0所在的线段为新环的起始线段
					double p0x, p0y, p1x, p1y;
					for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; j++) {
						CNode * sp0 = m_aLpsIntr.GetAt(i)->pNds.GetAt(j);
						CNode * sp1 = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1);
						CNode * point = new CNode(isec0[0],isec0[1]);
						p0x = sp0->x;  p0y = sp0->y;
						p1x = sp1->x;  p1y = sp1->y;
						if(IsPointOnSegment(p0x, p0y, p1x, p1y, isec0[0], isec0[1], myEps) == true) {
							pos[0] = j;  break;
						}
					}

					plp->Clear();
					for(t = pos[0]; t < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; t++) {//没有保存最后一个点
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->x;
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->y;
						plp->pNds.Add(new CNode(x,y));
					}
					for(t = 0; t < pos[0]; t++) {//保存了第一个点
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->x;
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->y;
						plp->pNds.Add(new CNode(x,y));
					}
					pos[0] = 0; //update
					plp->pNds.Add(new CNode(plp->pNds.GetAt(0)->x,plp->pNds.GetAt(0)->y)); //最合一个点要和第一个点相同
					
					//isec1的位置
					for(j = 0; j < plp->pNds.GetSize() - 1; j++) {
						CNode * sp0 = plp->pNds.GetAt(j);
						CNode * sp1 = plp->pNds.GetAt(j+1);
						p0x = sp0->x;  p0y = sp0->y;
						p1x = sp1->x;  p1y = sp1->y;
						if(IsPointOnSegment(p0x, p0y, p1x, p1y, isec1[0], isec1[1], myEps) == true) {
							pos[1] = j;  break;
						}
					}
					
					//form 2 son loops
					pson[0].pNds.Add(new CNode(isec0[0], isec0[1])); //first point
					for(t = 0; t < pos[1]; t++) {
						pson[0].pNds.Add(new CNode(plp->pNds.GetAt(t + 1)->x, plp->pNds.GetAt(t + 1)->y)); //must + 1
					}
                    pson[0].pNds.Add(new CNode(isec1[0], isec1[1])); //last point
					pson[0].pNds.Add(new CNode(isec0[0], isec0[1])); //first point again
					
					pson[1].pNds.Add(new CNode(isec0[0], isec0[1])); //first point
					pson[1].pNds.Add(new CNode(isec1[0], isec1[1])); //second point
					for(t = pos[1]; t < plp->pNds.GetSize() - 1; t++) {
						pson[1].pNds.Add(new CNode(plp->pNds.GetAt(t + 1)->x, plp->pNds.GetAt(t + 1)->y)); //must + 1
					}
					pson[1].pNds.Add(new CNode(isec0[0], isec0[1])); //first point again
					
					//update Z and form blocks
					//m_aBlks
					for(ii = 0; ii < 2; ii++) {
						CBlock * blk = new CBlock();
						for(jj = 0; jj < pson[ii].pNds.GetSize(); jj++) {
							x = pson[ii].pNds.GetAt(jj)->x;  y = pson[ii].pNds.GetAt(jj)->y;
							blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
						}
						//update Z, according to the equation of the triangle
						n0 = m_aPrs2D.GetAt(i)->FstID;
						nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
						for(kk = 0; kk < 3; kk++) { // real coordinates of the triangle, z is not zero
							nx[kk] = pSurf0->pTrs.GetAt(n0)->x[kk];
							ny[kk] = pSurf0->pTrs.GetAt(n0)->y[kk];
							nz[kk] = pSurf0->pTrs.GetAt(n0)->z[kk];
						}
						EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
						for(jj = 0; jj < blk->DnLpNds.GetSize(); jj++) {
							x = blk->DnLpNds.GetAt(jj)->x;
							y = blk->DnLpNds.GetAt(jj)->y;
							z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
							blk->DnLpNds.GetAt(jj)->z = z;
						}
						delete [] nx;  delete [] ny; delete [] nz; delete [] para;
						
						n1 = m_aPrs2D.GetAt(i)->SedID;
						nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
						for(kk = 0; kk < 3; kk++) { // real coordinates of the triangle, z is not zero
							nx[kk] = pSurf1->pTrs.GetAt(n1)->x[kk];
							ny[kk] = pSurf1->pTrs.GetAt(n1)->y[kk];
							nz[kk] = pSurf1->pTrs.GetAt(n1)->z[kk];
						}
						EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
						for(jj = 0; jj < blk->UpLpNds.GetSize(); jj++) {
							x = blk->UpLpNds.GetAt(jj)->x;
							y = blk->UpLpNds.GetAt(jj)->y;
							z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
							blk->UpLpNds.GetAt(jj)->z = z;
						}
						delete [] nx;  delete [] ny; delete [] nz; delete [] para;
						
						/* meigang
						// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
						// Begin : Check whether each polyhedron is valid
						bool bValid = true;
						double zup = 0.0, zdn = 0.0;
						if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
						    AfxMessageBox(_T("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()"),MB_OK|MB_ICONSTOP);
							return;
						}
						for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
							zup += blk->UpLpNds.GetAt(j)->z;
							zdn += blk->DnLpNds.GetAt(j)->z;
						}
						if(zup > zdn )  
							// End.2010.11.25
						//*/

						m_aBlks.Add(blk);
					}

					
				} // end else
			}
		}
	
	}



	/*
	// 2011.5.20 注释
	// 原因：当相交的两个面为平面时，以下代码不会产生问题；当为曲面时，出现错误，故注释
	
	//避免实际在平面上的点不落在同一个平面上，统一计算相同面上点的Z 2010.2.7
	//update Z, according to the equation of the triangle
	n0 = pSurf0->pTrs.GetSize() - 1;
	nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
	for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
		nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
		ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
		nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
	}
	EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
			x = blk->DnLpNds.GetAt(j)->x;
			y = blk->DnLpNds.GetAt(j)->y;
			z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
			blk->DnLpNds.GetAt(j)->z = z;
		}
	}
	delete [] nx;  delete [] ny; delete [] nz; delete [] para;
	
	n1 = pSurf1->pTrs.GetSize() - 1;
	nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
	for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
		nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
		ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
		nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
	}
	EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
			x = blk->UpLpNds.GetAt(j)->x;
			y = blk->UpLpNds.GetAt(j)->y;
			z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
			blk->UpLpNds.GetAt(j)->z = z;
		}
	}
	delete [] nx;  delete [] ny; delete [] nz; delete [] para;
	*/

	// 2013.04.22
	// merge all the blocks , and then rearrange and update them
	CString str; // For output window
	str.Format(_T("Starting merging all points !"));
	pFrame->AddStrToOutputWnd(1, str);

	CArray<CNode*, CNode*> pNds;

	id = 0;
	for(i = 0; i < m_aBlks.GetSize(); i++) { // original nodes of blocks
		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize(); j++) {
			x = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->x;
			y = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->y;
			z = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->z;
			m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID = id;
			pNds.Add(new CNode(id++, x, y, z));
		}
		for(j = 0; j < m_aBlks.GetAt(i)->DnLpNds.GetSize(); j++) {
			x = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->x;
			y = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->y;
			z = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->z;
			m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID = id;
			pNds.Add(new CNode(id++, x, y, z));
		}
	}

	//sort the nodes
	int cnt_1, idx; // int count

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; 
		p[idx].y = pNds.GetAt(j)->y; 
		p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}

	//sort nodes
    SortNode(p, cnt_1);

	//save each node for one time
	int * newIDs = new int[cnt_1];

	id = 0;
	newIDs[0] = 0;

	for(i = 0; i < cnt_1 - 1; i++){ // new IDs, important
		j = i + 1;
		if(IsSameNode(p[i].x, p[i].y, p[i].z, p[j].x, p[j].y, p[j].z, myEps) == false ) { 
//		if(IsSameNode(p[i].x, p[i].y, p[i].z, p[j].x, p[j].y, p[j].z, 0.0001) == false ) { 
			id++;
		}
		newIDs[j] = id;
	}

	// Update : 1st step
	for(i = 0; i < cnt_1; i++) {
		id = p[i].ID;
		pNds.GetAt(id)->ID = newIDs[i];
	}

	// Update : 2nd step
	for(i = 0; i < m_aBlks.GetSize(); i++) { // original nodes of blocks
		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID;
			m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID = pNds.GetAt(id)->ID;
		}
		for(j = 0; j < m_aBlks.GetAt(i)->DnLpNds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID;
			m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID = pNds.GetAt(id)->ID;
		}
	}

	// Save : after updating , important 
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 

	i = 0;
	x = p[i].x;  y = p[i].y;  z = p[i].z;
	pNds.Add(new CNode(i, x, y, z));
	for(i = 1; i < cnt_1; i++) {
		if(newIDs[i] != newIDs[i-1]) {
			x = p[i].x;  y = p[i].y;  z = p[i].z;
	        pNds.Add(new CNode(i, x, y, z));
		}
	}

	// transform from block to polyhedron, now a block includes a groups of polyhedrons
	// not just represents only one polyhedron
	for(i = 0; i < m_aBlks.GetSize(); i++ ) { //根据拓扑关系生成Areas
		CLoop * up = new CLoop();
		for(t = 0; t < m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; t++) { //首尾点相同，去掉最后一个点
			up->iNds.Add(m_aBlks.GetAt(i)->UpLpNds.GetAt(t)->ID);
		}
		CLoop * dn = new CLoop();
		for(t = 0; t < m_aBlks.GetAt(i)->DnLpNds.GetSize() - 1; t++) { //首尾点相同，去掉最后一个点
			dn->iNds.Add(m_aBlks.GetAt(i)->DnLpNds.GetAt(t)->ID);
		}

		m_aBlks.GetAt(i)->pLps.Add(up);  m_aBlks.GetAt(i)->pLps.Add(dn);

		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; j++) {
			CLoop * tmp = new CLoop();
			n0 = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID;
			n1 = m_aBlks.GetAt(i)->DnLpNds.GetAt(j + 1)->ID;
			n2 = m_aBlks.GetAt(i)->UpLpNds.GetAt(j + 1)->ID;
			n3 = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID;

			if(n0!= n3 && n1 != n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n2); tmp->iNds.Add(n3);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
			if(n0 != n3 && n1 == n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n3);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
			if(n0 == n3 && n1 != n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n2);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
		}
	}

	// Begin : transform from block to polyhedron
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CPohd * phd = new CPohd();
		for(j = 0; j < m_aBlks.GetAt(i)->pLps.GetSize(); j++) {
			CLoop * lp = new CLoop();
			for(k = 0; k < m_aBlks.GetAt(i)->pLps.GetAt(j)->iNds.GetSize(); k++) {
				id = m_aBlks.GetAt(i)->pLps.GetAt(j)->iNds.GetAt(k);
				x = pNds.GetAt(id)->x;  y = pNds.GetAt(id)->y;  z = pNds.GetAt(id)->z;
				lp->pNds.Add(new CNode(id, x, y, z));
			}
			phd->pLps.Add(lp);
		}
		pBlk->pPhs.Add(phd);
	}
	// End

	////////Remove the memory
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 
	
	cnt_1 = m_aBlks.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		for(j = m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; j >= 0; j--){
			delete m_aBlks.GetAt(i)->UpLpNds.GetAt(j);  m_aBlks.GetAt(i)->UpLpNds.RemoveAt(j);
		}
		for(j = m_aBlks.GetAt(i)->DnLpNds.GetSize() - 1; j >= 0; j--){
			delete m_aBlks.GetAt(i)->DnLpNds.GetAt(j);  m_aBlks.GetAt(i)->DnLpNds.RemoveAt(j);
		}
	}
	cnt_1 = m_aBlks.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		m_aBlks.GetAt(i)->Clear(); 
		delete m_aBlks.GetAt(i); m_aBlks.RemoveAt(i);    
	}
	m_aBlks.RemoveAll(); 
	cnt_1 = m_aPrs2D.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		delete m_aPrs2D.GetAt(i);  m_aPrs2D.RemoveAt(i);    
	}
	m_aPrs2D.RemoveAll();
	cnt_1 = m_aPrs3D.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		delete m_aPrs3D.GetAt(i);  m_aPrs3D.RemoveAt(i);    
	}
	m_aPrs3D.RemoveAll(); 
	
	Rgns.GetAt(1)->Clear();  delete Rgns.GetAt(1);  Rgns.RemoveAt(1);
	Rgns.GetAt(0)->Clear();  delete Rgns.GetAt(0);  Rgns.RemoveAt(0);
#endif
	
}

// 2013.04.25 By Mei Gang in freiburg Germany
// To compute the intersections of 2 polygonal mesh by mapping in 2.5D
void CRealModelView::InsectOfPolySurf_2AndHalfDBest(CSurf * pSurf0, CSurf * pSurf1, CBlock * pBlk) 
{
    int i, j, k, t, id;
	int n0, n1, n2, n3;
	double x, y, z;
	int N = pSurf0->pNds.GetSize();
	int L = pSurf0->pLps.GetSize();

	// m_aNds
	m_aNds.RemoveAll();
	for(i = 0; i < N; i++) {
		x = pSurf0->pNds.GetAt(i)->x;
		y = pSurf0->pNds.GetAt(i)->y;
		z = pSurf0->pNds.GetAt(i)->z;
		m_aNds.Add(new CNode(i, x, y, z));
	}
	for(i = 0; i < N; i++) {
		x = pSurf1->pNds.GetAt(i)->x;
		y = pSurf1->pNds.GetAt(i)->y;
		z = pSurf1->pNds.GetAt(i)->z;
		m_aNds.Add(new CNode(i+N, x, y, z));
	}

	// m_aLps
	for(i = 0; i < L; i++) { // The IDs must be updated 
		for(j = 0; j < pSurf1->pLps.GetAt(i)->iNds.GetSize(); j++) {
			pSurf1->pLps.GetAt(i)->iNds.GetAt(j) += N;
		}
	}
	
	for(i = 0; i < L; i++) { // Creat Polyhedrons 1st time
		CPohd * phd = new CPohd();

		// 假如首尾点相同，去掉最后一个点
		CLoop * up = new CLoop();
		N = pSurf0->pLps.GetAt(i)->iNds.GetSize();
		if( pSurf0->pLps.GetAt(i)->iNds.GetAt(0) ==
			pSurf0->pLps.GetAt(i)->iNds.GetAt(N-1) ) {
				--N;
		}
		for(t = 0; t < N; t++) { 
			id = pSurf0->pLps.GetAt(i)->iNds.GetAt(t);
			up->iNds.Add(id);
		}
		phd->pLps.Add(up); 

		//首尾点相同，去掉最后一个点
		CLoop * dn = new CLoop();
		N = pSurf1->pLps.GetAt(i)->iNds.GetSize();
		if( pSurf1->pLps.GetAt(i)->iNds.GetAt(0) ==
			pSurf1->pLps.GetAt(i)->iNds.GetAt(N-1) ) {
				--N;
		}
		for(t = 0; t < N; t++) { 
			id = pSurf1->pLps.GetAt(i)->iNds.GetAt(t);
			dn->iNds.Add(id);
		}
		phd->pLps.Add(dn);

		// Repeat the first point
		N = pSurf0->pLps.GetAt(i)->iNds.GetSize();
		if( pSurf0->pLps.GetAt(i)->iNds.GetAt(0) !=
			pSurf0->pLps.GetAt(i)->iNds.GetAt(N-1) ) {
				id = pSurf0->pLps.GetAt(i)->iNds.GetAt(0);
				pSurf0->pLps.GetAt(i)->iNds.Add(id);

				id = pSurf1->pLps.GetAt(i)->iNds.GetAt(0);
				pSurf1->pLps.GetAt(i)->iNds.Add(id);

				++N;
		}
		for(j = 0; j < N - 1; j++) {
			CLoop * tmp = new CLoop();
			n0 = pSurf1->pLps.GetAt(i)->iNds.GetAt(j);
			n1 = pSurf1->pLps.GetAt(i)->iNds.GetAt(j + 1);
			n2 = pSurf0->pLps.GetAt(i)->iNds.GetAt(j + 1);
			n3 = pSurf0->pLps.GetAt(i)->iNds.GetAt(j);

			tmp->iNds.Add(n0);
			tmp->iNds.Add(n1);
			tmp->iNds.Add(n2);
			tmp->iNds.Add(n3);

			phd->pLps.Add(tmp);
		}

		pBlk->pPhs.Add(phd);
	}

	// Update all loops
	int index = 0;
	bool bexist = false;

	//// The first one
	for(j = 0; j < pBlk->pPhs.GetAt(0)->pLps.GetSize(); j++) { // add the 1st one
		CLoop * tmp = pBlk->pPhs.GetAt(0)->pLps.GetAt(j);
		CLoop * lp = new CLoop();
		for(k = 0; k < tmp->iNds.GetSize(); k++) {
			lp->iNds.Add(tmp->iNds.GetAt(k));
		}
		m_aLps.Add(lp);

		pBlk->pPhs.GetAt(0)->iLps.Add(j); // faces of the 1st polyhedron
	}

	//// The rest ones
	for(i = 1; i < pBlk->pPhs.GetSize(); i++) { // i is not 0, but 1
		for(j = 0; j < pBlk->pPhs.GetAt(i)->pLps.GetSize(); j++) {
			CLoop * tmp = pBlk->pPhs.GetAt(i)->pLps.GetAt(j);

			// Check
			bexist = false;
			for(k = 0; k < m_aLps.GetSize(); k++) {
				if(IsTwoLoopSameByID(m_aLps.GetAt(k), tmp) == true) {
					index = k;  bexist = true;  break;
				}
			}

			if (bexist == true) { // If exist, directly save the ID
				pBlk->pPhs.GetAt(i)->iLps.Add(index);  continue;
			}
			else { // Not exist, add newly, and get the ID
		        CLoop * lp = new CLoop();
		        for(t = 0; t < tmp->iNds.GetSize(); t++) {
			        lp->iNds.Add(tmp->iNds.GetAt(t));
		        }
				m_aLps.Add(lp);  index = m_aLps.GetUpperBound(); // ID
				pBlk->pPhs.GetAt(i)->iLps.Add(index);
			}
		}
	}

	// re-updating, use the new IDs of all loops
	for(i = 0; i < pBlk->pPhs.GetSize(); i++) {
		pBlk->pPhs.GetAt(i)->pLps.RemoveAll(); // Remove
		for(j = 0; j < pBlk->pPhs.GetAt(i)->iLps.GetSize(); j++) {
			id = pBlk->pPhs.GetAt(i)->iLps.GetAt(j);
			pBlk->pPhs.GetAt(i)->pLps.Add(m_aLps.GetAt(id));
		}
	}

	// m_aPhds
	m_aPhds.RemoveAll();
	for(i = 0; i < pBlk->pPhs.GetSize(); i++) {
		m_aPhds.Add(pBlk->pPhs.GetAt(i));
	}

	// m_aBlks
	m_aBlks.RemoveAll();
	pBlk->iPhds.RemoveAll();
	for(i = 0; i < pBlk->pPhs.GetSize(); i++) { // Add the IDs
		pBlk->iPhds.Add(i);
	}
	m_aBlks.Add(pBlk);

}


//2011.12.5 By Mei Gang in freiburg Germany
//To compute the intersections of 2 triangle mesh in 3D and 2D
//parameters: CRegion * pSurf0, CRegion * pSurf1, CBlock * pBlk
//result    : firstly, all the intersections will be saved into m_aBlks
// secondly, transform all elements in M_aBlks to polyhedrons, which will be saved in pBlk->pPhds
void CRealModelView::InsectOfTrglSurf_2DAnd3DV2(CSurf * pSurf0, CSurf * pSurf1, CBlock * pBlk, double eps) 
{
#ifndef EGC
	CArray<CNode*, CNode*>pNds;
	CArray<CPair*, CPair*>m_aPrs2D; 
	CArray<CPair*, CPair*>m_aPrs3D;
	CArray<CBlock*, CBlock*>m_aBlks;

	int Add = pSurf0->pTrs.GetSize();

	// intersection in 2D
	int i, j, k, ii, jj, kk, id, tid[3];
	double x, y, z;
	CArray<CTrgl*, CTrgl*>pTrs;
	CTrgl * pTr0; CTrgl * pTr1;

	CArray<CSurf*, CSurf*>Rgns;
	CSurf * r0 = new CSurf();  CSurf * r1 = new CSurf();
	Rgns.Add(r0);  Rgns.Add(r1);

	for(i = 0; i < pSurf0->pTrs.GetSize(); i++) {
		id = pSurf0->pTrs.GetAt(i)->ID;
		for(j = 0; j < 3; j++) tid[j] = pSurf0->pTrs.GetAt(i)->nID[j];
		r0->pTrs.Add(new CTrgl(id, tid[0], tid[1], tid[2]));
		for(j = 0; j < 3; j++) {
			r0->pTrs.GetAt(i)->x[j] = pSurf0->pTrs.GetAt(i)->x[j];
			r0->pTrs.GetAt(i)->y[j] = pSurf0->pTrs.GetAt(i)->y[j];
			r0->pTrs.GetAt(i)->z[j] = 0.00;
		}
	}
	for(i = 0; i < pSurf0->pNds.GetSize(); i++) {
		id = pSurf0->pNds.GetAt(i)->ID;
		x = pSurf0->pNds.GetAt(i)->x; y = pSurf0->pNds.GetAt(i)->y; z = 0.00;
		r0->pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < pSurf0->pNds.GetSize(); i++){ //记录经过每个点的三角形ID 2010.1.29
		for(j = 0; j < pSurf0->pNds.GetAt(i)->sPsdTrgl.GetSize(); j++){
			r0->pNds.GetAt(i)->sPsdTrgl.Add(pSurf0->pNds.GetAt(i)->sPsdTrgl.GetAt(j));
		}
	}
	for(i = 0; i < pSurf0->pTrs.GetSize(); i++){ //记录每个三角形的邻域三角形，不包括自身
		for(j = 0; j < pSurf0->pTrs.GetAt(i)->iRgnPassTr.GetSize(); j++){
			r0->pTrs.GetAt(i)->iRgnPassTr.Add(pSurf0->pTrs.GetAt(i)->iRgnPassTr.GetAt(j));
		}
	}

	for(i = 0; i < pSurf1->pTrs.GetSize(); i++) {
		id = pSurf1->pTrs.GetAt(i)->ID;
		for(j = 0; j < 3; j++) tid[j] = pSurf1->pTrs.GetAt(i)->nID[j];
		r1->pTrs.Add(new CTrgl(id, tid[0], tid[1], tid[2]));
		for(j = 0; j < 3; j++) {
			r1->pTrs.GetAt(i)->x[j] = pSurf1->pTrs.GetAt(i)->x[j];
			r1->pTrs.GetAt(i)->y[j] = pSurf1->pTrs.GetAt(i)->y[j];
			r1->pTrs.GetAt(i)->z[j] = 0.00;
		}
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++) {
		id = pSurf1->pNds.GetAt(i)->ID;
		x = pSurf1->pNds.GetAt(i)->x; y = pSurf1->pNds.GetAt(i)->y; z = 0.00;
		r1->pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++){ //记录经过每个点的三角形ID 2010.1.29
		for(j = 0; j < pSurf1->pNds.GetAt(i)->sPsdTrgl.GetSize(); j++){
			r1->pNds.GetAt(i)->sPsdTrgl.Add(pSurf1->pNds.GetAt(i)->sPsdTrgl.GetAt(j));
		}
	}
	for(i = 0; i < pSurf1->pTrs.GetSize(); i++){ //记录每个三角形的邻域三角形，不包括自身
		for(j = 0; j < pSurf1->pTrs.GetAt(i)->iRgnPassTr.GetSize(); j++){
			r1->pTrs.GetAt(i)->iRgnPassTr.Add(pSurf1->pTrs.GetAt(i)->iRgnPassTr.GetAt(j));
		}
	}

	SearchIntrTrgls(r0, r1, myEps); // to search the intersected triangles for each triangle

	for(i = 0; i < 2; i++) {
		for(j = 0; j < Rgns.GetAt(i)->pTrs.GetSize(); j++) {
			pTrs.Add( Rgns.GetAt(i)->pTrs.GetAt(j) );
		}
	}

 	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CLoop * pIntr = new CLoop();
			IntrTriangle2Triangle2(pTr0, pTr1, pIntr);
			m_aLpsIntr.Add(pIntr); // Save
			pTr0->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
			pTr1->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
		}
	}

	//统一到一个三角形对的集合内2009.12.17
	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CPair * pr = new CPair();
			pr->FstID = pTr0->ID;  pr->SedID = pTr1->ID;
            m_aPrs2D.Add(pr); // 与m_aLpsIntr一一对应
		}
	}

	CombiLps2D(); // Combine all loops in 2D, 2010.2.8

	////intersection in 3D
	int t;
	int count, nCase;
	double *nx, *ny, *nz, *para;
	double xmin, ymin, zmin, xmax, ymax, zmax;
	int xdivi, ydivi, zdivi;
	double xtmp, ytmp, ztmp;
	double sum, dstep;
	int n0, n1, n2, n3, n4, n5, n6, n7;
	CArray<CNode*, CNode*>pSonNds;
	CArray<CHehd*, CHehd*>pSonBox;
	CNode * pMin, * pMax;
	CArray<CLine*, CLine*> pLns;
	float v0[3], v1[3], v2[3], u0[3], u1[3], u2[3], isec0[3], isec1[3]; // prepare for Moller.dll
	int copla;
	int * coplanar = &copla;
	
	//Find the smallest and the biggest x, y and z, and form a box;
	count = pSurf0->pNds.GetSize() + pSurf1->pNds.GetSize();
	nx = new double[count];  ny = new double[count];  nz = new double[count];
	for(i = 0; i < pSurf0->pNds.GetSize(); i++) {
		nx[i] = pSurf0->pNds.GetAt(i)->x;
		ny[i] = pSurf0->pNds.GetAt(i)->y;
		nz[i] = pSurf0->pNds.GetAt(i)->z;
	}
	for(i = 0; i < pSurf1->pNds.GetSize(); i++) {
		j = i + pSurf0->pNds.GetSize();
		nx[j] = pSurf1->pNds.GetAt(i)->x;
		ny[j] = pSurf1->pNds.GetAt(i)->y;
		nz[j] = pSurf1->pNds.GetAt(i)->z;
	}
	prhap(nx,count);  prhap(ny,count);  prhap(nz,count);
	xmin = nx[0];        ymin = ny[0];        zmin = nz[0];
	xmax = nx[count-1];  ymax = ny[count-1];  zmax = nz[count-1];
	delete [] nx;  delete [] ny;  delete [] nz;
	
	//compute the intersection of each triangle pair which is restricted in one son-box
		for(j = 0; j < pSurf0->pTrs.GetSize(); j++) {
			n0 = j;
			v0[0] = pSurf0->pTrs.GetAt(n0)->x[0]; 
			v0[1] = pSurf0->pTrs.GetAt(n0)->y[0]; 
			v0[2] = pSurf0->pTrs.GetAt(n0)->z[0];

			v1[0] = pSurf0->pTrs.GetAt(n0)->x[1]; 
			v1[1] = pSurf0->pTrs.GetAt(n0)->y[1]; 
			v1[2] = pSurf0->pTrs.GetAt(n0)->z[1];

			v2[0] = pSurf0->pTrs.GetAt(n0)->x[2]; 
			v2[1] = pSurf0->pTrs.GetAt(n0)->y[2]; 
			v2[2] = pSurf0->pTrs.GetAt(n0)->z[2];

			for(k = 0; k < pSurf1->pTrs.GetSize(); k++) {
				n1 = k;
				u0[0] = pSurf1->pTrs.GetAt(n1)->x[0]; 
				u0[1] = pSurf1->pTrs.GetAt(n1)->y[0]; 
				u0[2] = pSurf1->pTrs.GetAt(n1)->z[0];

				u1[0] = pSurf1->pTrs.GetAt(n1)->x[1]; 
				u1[1] = pSurf1->pTrs.GetAt(n1)->y[1]; 
				u1[2] = pSurf1->pTrs.GetAt(n1)->z[1];

				u2[0] = pSurf1->pTrs.GetAt(n1)->x[2]; 
				u2[1] = pSurf1->pTrs.GetAt(n1)->y[2]; 
				u2[2] = pSurf1->pTrs.GetAt(n1)->z[2];
				
				// Load function from DLL Moller.dll 2011.9.30
			    typedef int (*lpFun)(float [3],float [3],float [3],
				                 float [3],float [3],float [3],int *,
				                 float [3],float [3]); // 宏定义函数指针类型
	            HINSTANCE hDll; //DLL 句柄
                lpFun trglFun; // 函数指针

	            hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
	
                if (NULL==hDll) {
                    MessageBox(_T("Fail to load DLL Moller.dll ! "));
	            }
	            else {
		             trglFun = (lpFun) GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
                     if (trglFun != NULL) {
					     nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		             }
		             else {
			             MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		             }
                     FreeLibrary(hDll); // 释放函数
	            }
			    // End loading
				
				if(nCase == 1) { //save intersection segment
					if(pLns.GetSize() < 1) { // save directly
						CLine * ptmp = new CLine();
						ptmp->FstTri = n0;  ptmp->SecTri = n1;
						for(t = 0; t < 3; t++) { ptmp->FstNd[t] = isec0[t];  ptmp->SecNd[t] = isec1[t];} //交点坐标
						pLns.Add(ptmp);
					}
					else {
						bool bexist = false;
						for(t = 0; t < pLns.GetSize(); t++){ 
							if(pLns.GetAt(t)->FstTri == n0 && pLns.GetAt(t)->SecTri == n1){ bexist = true;  break; }
						}
						if(!bexist) { // not exist
							CLine * ptmp = new CLine();
							ptmp->FstTri = n0;  ptmp->SecTri = n1;
							for(t = 0; t < 3; t++) { 
								ptmp->FstNd[t] = isec0[t];  
								ptmp->SecNd[t] = isec1[t];
							} //交点坐标
							pLns.Add(ptmp);
						}
					}
				}
			}
	}
	
	//统一保存到一个三角形对的集合中。2009.12.17
	for(i = 0; i < pLns.GetSize(); i++) {
		CPair * pr = new CPair();
		pr->FstID = pLns.GetAt(i)->FstTri;  pr->SedID = pLns.GetAt(i)->SecTri;
		m_aPrs3D.Add(pr); //与pLns一一对应
	}
	////end of intersection in 3D 2009.1.22

	//2010.1.22 Mei Gang, Freiburg, Germany
	//2009.12.17 23:30  freiburg germany
	////use the 3D intersecton line to cut the 2D intersection loop
	//step 1: 确定2D相交三角形对有那些同时3D相交
	for(i = 0; i < m_aPrs2D.GetSize(); i++) { //initiate
		m_aPrs2D.GetAt(i)->bIntr2D = true;  m_aPrs2D.GetAt(i)->bIntr3D = false;}
	for(i = 0; i < m_aPrs3D.GetSize(); i++) {
		m_aPrs3D.GetAt(i)->bIntr2D = false; m_aPrs3D.GetAt(i)->bIntr3D = true; }

	for(i = 0; i < m_aPrs2D.GetSize(); i++) { //search out the pairs which intersect in both 2D and 3D
		for(j = 0; j < m_aPrs3D.GetSize(); j++) {
			if(m_aPrs2D.GetAt(i)->FstID == m_aPrs3D.GetAt(j)->FstID &&
			   m_aPrs2D.GetAt(i)->SedID == m_aPrs3D.GetAt(j)->SedID ){
				m_aPrs2D.GetAt(i)->bIntr3D = true;  m_aPrs3D.GetAt(j)->bIntr2D = true;
				break;
			}
		}
	}

	/*
	// output temperarily
	CString fnMesh;
	int NumOfPrs = m_aPrs3D.GetSize();
	if( AfxMessageBox("确定保存Temp文件?", MB_YESNOCANCEL) != 6 ) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, "Data File(*.txt)|*.txt", NULL);
	if( Dlg.DoModal() == IDOK ) 
	{
		fnMesh = Dlg.GetPathName(); 
		fnMesh += "-Temp";
		if(fnMesh.Right(4) != ".txt")   fnMesh += ".txt";
		ofstream fin( fnMesh );
		for(i = 0; i < NumOfPrs; i++){
			fin << i << "  " << m_aPrs3D.GetAt(i)->FstID << "  " << m_aPrs3D.GetAt(i)->SedID;
			fin << endl;
		}
		fin.close(); 
		
	}
	exit(0);
	// end tmp
	*/

	// For progress window
	CProgressWnd wndProgress(this, _T("Progress"), TRUE);
	int myRance = m_aPrs2D.GetSize();
	wndProgress.SetRange(0, myRance);
	wndProgress.SetText(_T("This is a progress window...\n\n")
						_T("Try dragging it around or hitting Cancel."));

	// form the blocks
	int situ[2];
    CLoop * plp = new CLoop();
	CLoop * pson = new CLoop[2];
	for(i = 0; i < m_aPrs2D.GetSize(); i++) {
		// For progress windown
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) {
			MessageBox(_T("Progress Cancelled"));
			break;
		}
		// End

		if(m_aLpsIntr.GetAt(i)->pNds.GetSize() < 3)  continue;  //intersection in 2D is a point or a segment

		plp->Clear();  pson[0].Clear();  pson[1].Clear(); // initiate, very important!
		
		CBlock * blk = new CBlock();
		blk->FstTri = m_aPrs2D.GetAt(i)->FstID;  //index of the two triangles
		blk->SecTri = m_aPrs2D.GetAt(i)->SedID; 

		if(!m_aPrs2D.GetAt(i)->bIntr3D) { // intersect only in 2D
			for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
				x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;// at present, just need to save x and y
				y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
				blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
			}
			//update Z, according to the equation of the triangle
			n0 = m_aPrs2D.GetAt(i)->FstID;
			nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
			for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
				nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
				ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
				nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
			}
			EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
			for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
				x = blk->DnLpNds.GetAt(j)->x;
				y = blk->DnLpNds.GetAt(j)->y;
				z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
				blk->DnLpNds.GetAt(j)->z = z;
			}
			delete [] nx;  delete [] ny; delete [] nz; delete [] para;
			
			n1 = m_aPrs2D.GetAt(i)->SedID;
			nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
			for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
				nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
				ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
				nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
			}
			EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
			for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
				x = blk->UpLpNds.GetAt(j)->x;
				y = blk->UpLpNds.GetAt(j)->y;
				z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
				blk->UpLpNds.GetAt(j)->z = z;
			}
			delete [] nx;  delete [] ny; delete [] nz; delete [] para;
			
			/*
			// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
			// Begin : Check whether each polyhedron is valid
			bool bValid = true;
			double zup = 0.0, zdn = 0.0;
			if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
				AfxMessageBox("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()",MB_OK|MB_ICONSTOP);
				return;
			}
			for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
				zup += blk->UpLpNds.GetAt(j)->z;
				zdn += blk->DnLpNds.GetAt(j)->z;
			}
			if(zup > zdn )  
			// End.2010.11.25
			*/

			m_aBlks.Add(blk);//save
		}
	
		else { // intersect in both 2D and 3D
			bool bexist = false;
			n0 = m_aPrs2D.GetAt(i)->FstID;
			v0[0] = pSurf0->pTrs.GetAt(n0)->x[0]; 
			v0[1] = pSurf0->pTrs.GetAt(n0)->y[0]; 
			v0[2] = pSurf0->pTrs.GetAt(n0)->z[0];
			v1[0] = pSurf0->pTrs.GetAt(n0)->x[1]; 
			v1[1] = pSurf0->pTrs.GetAt(n0)->y[1]; 
			v1[2] = pSurf0->pTrs.GetAt(n0)->z[1];
			v2[0] = pSurf0->pTrs.GetAt(n0)->x[2]; 
			v2[1] = pSurf0->pTrs.GetAt(n0)->y[2]; 
			v2[2] = pSurf0->pTrs.GetAt(n0)->z[2];
			
			n1 = m_aPrs2D.GetAt(i)->SedID;
			u0[0] = pSurf1->pTrs.GetAt(n1)->x[0]; 
			u0[1] = pSurf1->pTrs.GetAt(n1)->y[0]; 
			u0[2] = pSurf1->pTrs.GetAt(n1)->z[0];
			u1[0] = pSurf1->pTrs.GetAt(n1)->x[1]; 
			u1[1] = pSurf1->pTrs.GetAt(n1)->y[1]; 
			u1[2] = pSurf1->pTrs.GetAt(n1)->z[1];
			u2[0] = pSurf1->pTrs.GetAt(n1)->x[2]; 
			u2[1] = pSurf1->pTrs.GetAt(n1)->y[2]; 
			u2[2] = pSurf1->pTrs.GetAt(n1)->z[2];
			
			// Load function from DLL Moller.dll 2011.9.30
			typedef int (*lpFun)(float V0[3],float V1[3],float V2[3],
				                 float U0[3],float U1[3],float U2[3],int *coplanar,
				                 float isectpt1[3],float isectpt2[3]); // 宏定义函数指针类型
	        HINSTANCE hDll; //DLL 句柄
            lpFun trglFun; // 函数指针

	        hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
	
            if (NULL==hDll) {
                MessageBox(_T("Fail to load DLL Moller.dll ! "));
	        }
	        else {
				trglFun = (lpFun)GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
                if (trglFun != NULL) {
					nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		        }
		        else {
			        MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		        }
                FreeLibrary(hDll); // 释放函数
	        }
			// End loading

			if(nCase == 1) { //intersection is a segment
				//case 1 : 相交线段恰好为2D相交环上的一条边，只生成一个Block
				for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; j++) {
					x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;
					y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
					z = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->z;
					xtmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->x;
					ytmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->y;
					ztmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->z;
					
					if( //判断两条线段相同
						(sqrt((x-isec0[0])*(x-isec0[0])+(y-isec0[1])*(y-isec0[1])) < 0.00001 &&
						sqrt((xtmp-isec1[0])*(xtmp-isec1[0])+(ytmp-isec1[1])*(ytmp-isec1[1]) )< 0.00001) 
						||
						(sqrt((x-isec1[0])*(x-isec1[0])+(y-isec1[1])*(y-isec1[1])) < 0.00001 &&
						sqrt((xtmp-isec0[0])*(xtmp-isec0[0])+(ytmp-isec0[1])*(ytmp-isec0[1]) )< 0.00001)
						) {
						bexist = true;
					}
				}
				
				if(bexist) {
                    CBlock * blk = new CBlock();
					for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;// at present, just need to save x and y
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
						blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
					}

					//update Z, according to the equation of the triangle
					n0 = m_aPrs2D.GetAt(i)->FstID;
					nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
					for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
						nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
						ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
						nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
					}
					EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
					for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
						x = blk->DnLpNds.GetAt(j)->x;
						y = blk->DnLpNds.GetAt(j)->y;
						z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
						blk->DnLpNds.GetAt(j)->z = z;
					}
					delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
					n1 = m_aPrs2D.GetAt(i)->SedID;
					nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
					for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
						nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
						ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
						nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
					}
					EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
					for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
						x = blk->UpLpNds.GetAt(j)->x;
						y = blk->UpLpNds.GetAt(j)->y;
						z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
						blk->UpLpNds.GetAt(j)->z = z;
					}
					delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
					/*
					// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
					// Begin : Check whether each polyhedron is valid
					// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
					// Begin : Check whether each polyhedron is valid
					bool bValid = true;
					double zup = 0.0, zdn = 0.0;
					if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
						AfxMessageBox("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()",MB_OK|MB_ICONSTOP);
						return;
					}
					for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
						zup += blk->UpLpNds.GetAt(j)->z;
						zdn += blk->DnLpNds.GetAt(j)->z;
					}
					if(zup > zdn )  
					// End.2010.11.25
					*/
						
					m_aBlks.Add(blk);//save

				}
				else { //case 2 : 相交线段将2D相交环分割为两个子环，生成二个Block
					// 2011.5.21 一下代码有问题，目前为解决！

//					OutputIntr();  exit(0);
					
					int pos[2];//index of the position
					//以isec0所在的线段为新环的起始线段
					for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; j++) {
						CNode * sp0 = m_aLpsIntr.GetAt(i)->pNds.GetAt(j);
						CNode * sp1 = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1);
						CNode * point = new CNode(isec0[0],isec0[1]);
						double p0x = sp0->x;  double p0y = sp0->y;
						double p1x = sp1->x;  double p1y = sp1->y;
						if(IsPointOnSegment(p0x, p0y, p1x, p1y, isec0[0], isec0[1], eps) == true) {
							pos[0] = j;  break;
						}
					}

					plp->Clear();
					for(t = pos[0]; t < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; t++) {//没有保存最后一个点
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->x;
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->y;
						plp->pNds.Add(new CNode(x,y));
					}
					for(t = 0; t < pos[0]; t++) {//保存了第一个点
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->x;
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->y;
						plp->pNds.Add(new CNode(x,y));
					}
					pos[0] = 0; //update
					plp->pNds.Add(new CNode(plp->pNds.GetAt(0)->x,plp->pNds.GetAt(0)->y)); //最合一个点要和第一个点相同
					
					//isec1的位置
					for(j = 0; j < plp->pNds.GetSize() - 1; j++) {
						CNode * sp0 = plp->pNds.GetAt(j);
						CNode * sp1 = plp->pNds.GetAt(j+1);
						double p0x = sp0->x;  double p0y = sp0->y;
						double p1x = sp1->x;  double p1y = sp1->y;
						if(IsPointOnSegment(p0x, p0y, p1x, p1y, isec1[0], isec1[1], eps) == true) {
							pos[1] = j;  break;
						}
					}
					
					//form 2 son loops
					pson[0].pNds.Add(new CNode(isec0[0], isec0[1])); //first point
					for(t = 0; t < pos[1]; t++) {
						pson[0].pNds.Add(new CNode(plp->pNds.GetAt(t + 1)->x, plp->pNds.GetAt(t + 1)->y)); //must + 1
					}
                    pson[0].pNds.Add(new CNode(isec1[0], isec1[1])); //last point
					pson[0].pNds.Add(new CNode(isec0[0], isec0[1])); //first point again
					
					pson[1].pNds.Add(new CNode(isec0[0], isec0[1])); //first point
					pson[1].pNds.Add(new CNode(isec1[0], isec1[1])); //second point
					for(t = pos[1]; t < plp->pNds.GetSize() - 1; t++) {
						pson[1].pNds.Add(new CNode(plp->pNds.GetAt(t + 1)->x, plp->pNds.GetAt(t + 1)->y)); //must + 1
					}
					pson[1].pNds.Add(new CNode(isec0[0], isec0[1])); //first point again
					
					//update Z and form blocks
					//m_aBlks
					for(ii = 0; ii < 2; ii++) {
						CBlock * blk = new CBlock();
						for(jj = 0; jj < pson[ii].pNds.GetSize(); jj++) {
							x = pson[ii].pNds.GetAt(jj)->x;  y = pson[ii].pNds.GetAt(jj)->y;
							blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
						}
						//update Z, according to the equation of the triangle
						n0 = m_aPrs2D.GetAt(i)->FstID;
						nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
						for(kk = 0; kk < 3; kk++) { // real coordinates of the triangle, z is not zero
							nx[kk] = pSurf0->pTrs.GetAt(n0)->x[kk];
							ny[kk] = pSurf0->pTrs.GetAt(n0)->y[kk];
							nz[kk] = pSurf0->pTrs.GetAt(n0)->z[kk];
						}
						EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
						for(jj = 0; jj < blk->DnLpNds.GetSize(); jj++) {
							x = blk->DnLpNds.GetAt(jj)->x;
							y = blk->DnLpNds.GetAt(jj)->y;
							z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
							blk->DnLpNds.GetAt(jj)->z = z;
						}
						delete [] nx;  delete [] ny; delete [] nz; delete [] para;
						
						n1 = m_aPrs2D.GetAt(i)->SedID;
						nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
						for(kk = 0; kk < 3; kk++) { // real coordinates of the triangle, z is not zero
							nx[kk] = pSurf1->pTrs.GetAt(n1)->x[kk];
							ny[kk] = pSurf1->pTrs.GetAt(n1)->y[kk];
							nz[kk] = pSurf1->pTrs.GetAt(n1)->z[kk];
						}
						EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
						for(jj = 0; jj < blk->UpLpNds.GetSize(); jj++) {
							x = blk->UpLpNds.GetAt(jj)->x;
							y = blk->UpLpNds.GetAt(jj)->y;
							z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
							blk->UpLpNds.GetAt(jj)->z = z;
						}
						delete [] nx;  delete [] ny; delete [] nz; delete [] para;
						
						/*
						// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
						// Begin : Check whether each polyhedron is valid
						bool bValid = true;
						double zup = 0.0, zdn = 0.0;
						if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
							AfxMessageBox("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()",MB_OK|MB_ICONSTOP);
							return;
						}
						for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
							zup += blk->UpLpNds.GetAt(j)->z;
							zdn += blk->DnLpNds.GetAt(j)->z;
						}
						if(zup > zdn )  
							// End.2010.11.25
						*/

						m_aBlks.Add(blk);
					}

					
				} // end else
			}
		}
	
	}

	/*
	// 2011.5.20 注释
	// 原因：当相交的两个面为平面时，以下代码不会产生问题；当为曲面时，出现错误，故注释
	
	//避免实际在平面上的点不落在同一个平面上，统一计算相同面上点的Z 2010.2.7
	//update Z, according to the equation of the triangle
	n0 = pSurf0->pTrs.GetSize() - 1;
	nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
	for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
		nx[k] = pSurf0->pTrs.GetAt(n0)->x[k];
		ny[k] = pSurf0->pTrs.GetAt(n0)->y[k];
		nz[k] = pSurf0->pTrs.GetAt(n0)->z[k];
	}
	EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
			x = blk->DnLpNds.GetAt(j)->x;
			y = blk->DnLpNds.GetAt(j)->y;
			z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
			blk->DnLpNds.GetAt(j)->z = z;
		}
	}
	delete [] nx;  delete [] ny; delete [] nz; delete [] para;
	
	n1 = pSurf1->pTrs.GetSize() - 1;
	nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
	for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
		nx[k] = pSurf1->pTrs.GetAt(n1)->x[k];
		ny[k] = pSurf1->pTrs.GetAt(n1)->y[k];
		nz[k] = pSurf1->pTrs.GetAt(n1)->z[k];
	}
	EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
			x = blk->UpLpNds.GetAt(j)->x;
			y = blk->UpLpNds.GetAt(j)->y;
			z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
			blk->UpLpNds.GetAt(j)->z = z;
		}
	}
	delete [] nx;  delete [] ny; delete [] nz; delete [] para;
	*/

	//2010.1.14
	//emerge all the blocks , and then rearrange and update them
	for(i = 0; i < m_aBlks.GetSize(); i++) { // original nodes of blocks
		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID;
			x = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->x;
			y = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->y;
			z = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->z;
			pNds.Add(new CNode(id, x, y, z));
		}
		for(j = 0; j < m_aBlks.GetAt(i)->DnLpNds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID;
			x = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->x;
			y = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->y;
			z = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->z;
			pNds.Add(new CNode(id, x, y, z));
		}
	}

	//sort the nodes
	int cnt_1, idx; // int count

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; p[idx].y = pNds.GetAt(j)->y; p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll();  
	//sort nodes
    SortNode(p, cnt_1);
	
	//re-arrange nodes
	int* iID = new int[cnt_1];	int* nID = new int[cnt_1];
	for(i = 0; i < cnt_1; i++)iID[i] = p[i].ID;

	nID[0] = 0; idx = 0;

	pNds.Add(new CNode(idx, p[0].x, p[0].y, p[0].z)); 

	//save each node for one time
	bool boolexist;
	for(i = 1; i < cnt_1; i++){
		boolexist = false;
		for(j = 0; j < pNds.GetSize(); j++) {
			if(IsSameNode(&p[i], pNds.GetAt(j), myEps)){ boolexist = true;  break; }
		}
		if(!boolexist) pNds.Add(new CNode(pNds.GetSize(), p[i].x, p[i].y, p[i].z));
	}
	delete [] p;

	delete[] iID;  delete[] nID;
    
	//最直接、最笨的方法合并节点
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		for(j = 0; j < m_aBlks.GetAt(i)->DnLpNds.GetSize(); j++) {
			for(k = 0; k < pNds.GetSize(); k++) {
				if(IsSameNode(m_aBlks.GetAt(i)->DnLpNds.GetAt(j), pNds.GetAt(k), myEps)) {
					m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID = k;  break;
				}
			}
		}
		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize(); j++) {
			for(k = 0; k < pNds.GetSize(); k++) {
				if(IsSameNode(m_aBlks.GetAt(i)->UpLpNds.GetAt(j), pNds.GetAt(k), myEps)) {
					m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID = k;  break;
				}
			}
		}
	}

	// transform from block to polyhedron, now a block includes a groups of polyhedrons
	// not just represents only one polyhedron
	for(i = 0; i < m_aBlks.GetSize(); i++ ) { //根据拓扑关系生成Areas
		CLoop * up = new CLoop();
		for(t = 0; t < m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; t++) { //首尾点相同，去掉最后一个点
			up->iNds.Add(m_aBlks.GetAt(i)->UpLpNds.GetAt(t)->ID);
		}
		CLoop * dn = new CLoop();
		for(t = 0; t < m_aBlks.GetAt(i)->DnLpNds.GetSize() - 1; t++) { //首尾点相同，去掉最后一个点
			dn->iNds.Add(m_aBlks.GetAt(i)->DnLpNds.GetAt(t)->ID);
		}

		m_aBlks.GetAt(i)->pLps.Add(up);  m_aBlks.GetAt(i)->pLps.Add(dn);

		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; j++) {
			CLoop * tmp = new CLoop();
			n0 = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID;
			n1 = m_aBlks.GetAt(i)->DnLpNds.GetAt(j + 1)->ID;
			n2 = m_aBlks.GetAt(i)->UpLpNds.GetAt(j + 1)->ID;
			n3 = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID;

			if(n0!= n3 && n1 != n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n2); tmp->iNds.Add(n3);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
			if(n0 != n3 && n1 == n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n3);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
			if(n0 == n3 && n1 != n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n2);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
		}
	}

	// Begin : transform from block to polyhedron
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CPohd * phd = new CPohd();
		for(j = 0; j < m_aBlks.GetAt(i)->pLps.GetSize(); j++) {
			CLoop * lp = new CLoop();
			for(k = 0; k < m_aBlks.GetAt(i)->pLps.GetAt(j)->iNds.GetSize(); k++) {
				id = m_aBlks.GetAt(i)->pLps.GetAt(j)->iNds.GetAt(k);
				x = pNds.GetAt(id)->x;  y = pNds.GetAt(id)->y;  z = pNds.GetAt(id)->z;
				lp->pNds.Add(new CNode(id, x, y, z));
			}
			phd->pLps.Add(lp);
		}
		pBlk->pPhs.Add(phd);
	}
	// End

	////////Remove the memory
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 
	
	cnt_1 = m_aBlks.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		for(j = m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; j >= 0; j--){
			delete m_aBlks.GetAt(i)->UpLpNds.GetAt(j);  m_aBlks.GetAt(i)->UpLpNds.RemoveAt(j);
		}
		for(j = m_aBlks.GetAt(i)->DnLpNds.GetSize() - 1; j >= 0; j--){
			delete m_aBlks.GetAt(i)->DnLpNds.GetAt(j);  m_aBlks.GetAt(i)->DnLpNds.RemoveAt(j);
		}
	}
	cnt_1 = m_aBlks.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		m_aBlks.GetAt(i)->Clear(); 
		delete m_aBlks.GetAt(i); m_aBlks.RemoveAt(i);    
	}
	m_aBlks.RemoveAll(); 
	cnt_1 = m_aPrs2D.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		delete m_aPrs2D.GetAt(i);  m_aPrs2D.RemoveAt(i);    
	}
	m_aPrs2D.RemoveAll();
	cnt_1 = m_aPrs3D.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		delete m_aPrs3D.GetAt(i);  m_aPrs3D.RemoveAt(i);    
	}
	m_aPrs3D.RemoveAll(); 
	
	Rgns.GetAt(1)->Clear();  delete Rgns.GetAt(1);  Rgns.RemoveAt(1);
	Rgns.GetAt(0)->Clear();  delete Rgns.GetAt(0);  Rgns.RemoveAt(0);
	
#else
   // to be continue

#endif

}

// 2010.2.8
// to combine loops which are generated in 2D
void CRealModelView::CombiLps2D() 
{
	int i, j, id;
	double x, y, z;
	//emerge all the points and rename the IDs
	for(i = 0; i < m_aSurfs.GetSize(); i++) {  // original nodes of triangles
		for(j = 0; j < m_aSurfs.GetAt(i)->pNds.GetSize(); j++) {
			id = m_aSurfs.GetAt(i)->pNds.GetAt(j)->ID;
			x = m_aSurfs.GetAt(i)->pNds.GetAt(j)->x; 
			y = m_aSurfs.GetAt(i)->pNds.GetAt(j)->y; 
			z = 0.0000; 
			pNds.Add(new CNode(id, x, y, z));
		}
	}
	for(i = 0; i < m_aLpsIntr.GetSize(); i++) { // original nodes of intersections(loops)
		for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
			id = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->ID;
			x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;
			y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
			z = 0.0000;
			pNds.Add(new CNode(id, x, y, z));
		}
	}

	//sort the nodes
	int n, cnt_1, idx, count;

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; 
		p[idx].y = pNds.GetAt(j)->y; 
		p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll();  

	//sort nodes
    SortNode(p, cnt_1);

	//re-arrange nodes
	int* iID = new int[cnt_1];	int* nID = new int[cnt_1];
	for(i = 0; i < cnt_1; i++)iID[i] = p[i].ID;

	nID[0] = 0; idx = 0;

	pNds.Add(new CNode(idx, p[0].x, p[0].y, p[0].z)); 
	
	//re-create nodes
	for(i = 1; i < cnt_1; i++){
		if(!IsSameNode(&p[i], &p[i-1], myEps)) {
			idx++; pNds.Add(new CNode(idx, p[i].x, p[i].y, p[i].z));
		}
		nID[i] = idx;
	}
	delete [] p;
	//sort id
	prhap(iID, nID, cnt_1); idx = 0;  count = 0;

	// renew all the IDs for points
	//copy new nodes for region
	n = 0; 
	for(i = 0; i < m_aSurfs.GetSize(); i++) { 
		for(j = 0; j < m_aSurfs.GetAt(i)->pNds.GetSize(); j++) {
			id = nID[n];
			m_aSurfs.GetAt(i)->pNds.GetAt(j)->ID = id;
			n++;
		}
	}
	//copy new nodes for intersections
	for(i = 0; i < m_aLpsIntr.GetSize(); i++) { 
		for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
			id = nID[n];
			m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->ID = id;
			m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x = pNds.GetAt(id)->x;
			m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y = pNds.GetAt(id)->y;
			m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->z = pNds.GetAt(id)->z;
			n++;		
		}
	}

	delete[] iID;  delete[] nID;
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 
	
}

//2009.12.1 by Mei Gang In freiburg germany
//to test whether or not the box of a triangle intersect another box
//parameters: CTrgl * pTrgl, CNode * pBoxMin, pBoxMax;
//result    : returns 1 if intersect, elsewise returns 0;
int CRealModelView::IsTrglInsectBox(CTrgl * pTr, CNode * pBoxMin, CNode * pBoxMax) 
{
	int i;
	double x, y, z;
	double epsilon = 0.00001;
	for(i = 0; i < 3; i++) {
		x = pTr->x[i];  y = pTr->y[i];  z = pTr->z[i];
		if(x > pBoxMin->x - epsilon && x < pBoxMax->x + epsilon &&
		   y > pBoxMin->y - epsilon && y < pBoxMax->y + epsilon &&
		   z > pBoxMin->z - epsilon && z < pBoxMax->z + epsilon   )
		   return 1;
	}
	return 0;
}

//2009.12.17 accepted from "RockModel"
//the surface equation of a trgl: Ax + By + Cz + D = 0
void CRealModelView::EquaOfTrgl(double *x, double *y, double *z, double *Para)
{
	double x1, y1, z1, x2, y2, z2;
	double A, B, C, D;
	double factor;

	x1 = x[1] - x[0];	y1 = y[1] - y[0];	z1 = z[1] - z[0];
	x2 = x[2] - x[0];	y2 = y[2] - y[0];	z2 = z[2] - z[0];

	A = y1 * z2 - y2 * z1;
	B = x2 * z1 - x1 * z2;
	C = x1 * y2 - x2 * y1;
	D = A * x[0] + B * y[0] + C * z[0];
	D = -1 * D;
	factor = sqrt(A * A + B * B + C * C);
	if(D > 0.0)  factor = -1 * factor;
	Para[0] = A / factor; Para[1] = B / factor; Para[2] = C / factor; Para[3] = D / factor;

}

void CRealModelView::EquaOfTrgl(CTrgl * pTrgl, CPlane * pPla)
{
	double x[3], y[3], z[3], para[4];
    for(int i = 0; i < 3; i++) {
		x[i] = pTrgl->x[i]; y[i] = pTrgl->y[i]; z[i] = pTrgl->z[i]; 
	}
	EquaOfTrgl(x, y, z, para);
	for(int j = 0; j < 4; j++) {
		pPla->Para[j] = para[j];
	}

}

//2009.12.18 by Mei Gang, in Freiburg, Germany
//purpose: to check whether a point is on a segment or not
//parameters: sp0 and sp1 are the points of a segment. point, is to be checked
//results: 0, is not on ; 1 on the vertex of the segment; 2 on the middle of the segement
int CRealModelView::IsPointOnSegment(CNode * sp0, CNode * sp1, CNode * point, double eps)
{
	double dist, cross;
//	double epsilon = 0.00001; // 2011.5.23
	double epsilon = eps; 
	dist = (sp0->x - point->x)*(sp0->x - point->x) + (sp0->y - point->y)*(sp0->y - point->y);
	dist = sqrt(dist);
	if(dist < epsilon)  return 1;
	dist = (sp1->x - point->x)*(sp1->x - point->x) + (sp1->y - point->y)*(sp1->y - point->y);
	dist = sqrt(dist);
	if(dist < epsilon)  return 1;

	double a = sqrt((sp0->x-sp1->x)*(sp0->x-sp1->x) + (sp0->y-sp1->y)*(sp0->y-sp1->y));
	double b = sqrt((sp1->x-point->x)*(sp1->x-point->x) + (sp1->y-point->y)*(sp1->y-point->y));
	double c = sqrt((sp0->x-point->x)*(sp0->x-point->x) + (sp0->y-point->y)*(sp0->y-point->y));
	double p = (a + b +c) / 2.0;
    cross = sqrt(p*(p-a)*(p-b)*(p-c));

	cross = (point->x - sp0->x)*( sp1->y - sp0->y ) - (point->y - sp0->y)*(sp1->x - sp0->x);
	if(fabs(cross) < epsilon && (sp0->x - point->x)*(sp1->x - point->x) < epsilon
		                     && (sp0->y - point->y)*(sp1->y - point->y) < epsilon)  
	return 2;

	return 0;
}

// 2011.5.25 by MeiGang
// to check whether a point is on a segment
bool CRealModelView::IsPointOnSegment(double p0x, double p0y, double p1x, double p1y, double ptx, double pty, double eps)
{
//	double epsilon = 0.00001; // 2011.5.23
//	double epsilon = 0.001; // 2011.5.23
	double epsilon = eps;

    /*
	double a = sqrt((p0x-p1x)*(p0x-p1x) + (p0y-p1y)*(p0y-p1y));
	double b = sqrt((p1x-ptx)*(p1x-ptx) + (p1y-pty)*(p1y-pty));
	double c = sqrt((p0x-ptx)*(p0x-ptx) + (p0y-pty)*(p0y-pty));
	double p = (a + b +c) / 2.0;
    double cross = sqrt(p*(p-a)*(p-b)*(p-c));

	double x0, y0, z0, x1, y1, z1, x2, y2, z2;
	x0 = p0x;  y0 = p0y;  z0 = 0.0f;
	x1 = ptx;  y1 = pty;  z1 = 0.0f;
	x2 = p1x;  y2 = p1y;  z2 = 0.0f;
	
	double u1, u2, u3, v1, v2, v3;
	u1 = x2 - x0;  u2 = y2 - y0;  u3 = z2 - z0;
	v1 = x1 - x0;  v2 = y1 - y0;  v3 = z1 - z0;
	cross = (u2*v3 + u3*v2) + (u3*v1-u1*v3) +(u1*v2-u2*v1);
	*/
	double cross = (ptx - p0x)*(p1y - p0y) - (pty - p0y)*(p1x - p0x);

	if(fabs(cross) < epsilon && (p0x - ptx)*(p1x - ptx) < epsilon
		                     && (p0y - pty)*(p1y - pty) < epsilon)  
	return true;
	else return false;

}

int CRealModelView::IsPointOnSegment3D(CNode * sp0, CNode * sp1, CNode * point, double eps)
{
	double dist, cross;
//	double epsilon = 0.00001;
	double epsilon = eps;
	dist = (sp0->x - point->x)*(sp0->x - point->x) + 
		   (sp0->y - point->y)*(sp0->y - point->y) +
		   (sp0->z - point->z)*(sp0->z - point->z);
	dist = sqrt(dist);
	if(dist < epsilon)  return 1;
	dist = (sp1->x - point->x)*(sp1->x - point->x) + 
		   (sp1->y - point->y)*(sp1->y - point->y) +
		   (sp1->z - point->z)*(sp1->z - point->z);
	dist = sqrt(dist);
	if(dist < epsilon)  return 1;

	double u1, u2, u3, v1, v2, v3;
	u1 = sp0->x - point->x;  u2 = sp0->y - point->y;  u3 = sp0->z - point->z;
	v1 = point->x - sp1->x;  v2 = point->y - sp1->y;  v3 = point->z - sp1->z;
	cross = u2 * v3 - u3 * v2 + u3 * v1 - u1 * v3 + u1 * v2 - u2 * v1;
	if(fabs(cross) < epsilon && (sp0->x - point->x)*(sp1->x - point->x) < epsilon
		                     && (sp0->y - point->y)*(sp1->y - point->y) < epsilon
							 && (sp0->z - point->z)*(sp1->z - point->z) < epsilon )  
	return 2;

	return 0;
}

// True: if collinear, else FALSE
bool CRealModelView::Is3PointCollinear(CNode * sp0, CNode * sp1, CNode * point, double eps)
{
	double u1, u2, u3, v1, v2, v3;

	u1 = sp0->x - point->x;  u2 = sp0->y - point->y;  u3 = sp0->z - point->z;

	v1 = point->x - sp1->x;  v2 = point->y - sp1->y;  v3 = point->z - sp1->z;

	double cross = u2 * v3 - u3 * v2 + u3 * v1 - u1 * v3 + u1 * v2 - u2 * v1;

	if(fabs(cross) < eps )  return true;
	else return false;

}

// 2010.12.6 by Mei Gang, in Germany
// create a RGB color according to the object's ID
void CRealModelView::CreateRGBColor(int * RGB, int ID)
{
	//*
	int index;
	index = ID % 10;
	switch (index) {
		case 0: { RGB[0] = 176;  RGB[1] = 23;  RGB[2] = 31; break; }
//	    case 1: { RGB[0] = 25;  RGB[1] = 25;  RGB[2] = 112; break; }
//	    case 2: { RGB[0] = 250;  RGB[1] = 18;  RGB[2] = 250;  break; }
//		case 1: { RGB[0] = 255;  RGB[1] = 128;  RGB[2] = 0; break; }
		case 1: { RGB[0] = 200;  RGB[1] = 34;  RGB[2] = 15; break; }
		case 2: { RGB[0] = 11;  RGB[1] = 69;  RGB[2] = 19;  break; }
	    case 3: { RGB[0] = 227;  RGB[1] = 23;  RGB[2] = 13; break; }
	    case 4: { RGB[0] = 139;  RGB[1] = 69;  RGB[2] = 19; break; }
		case 5: { RGB[0] = 250;  RGB[1] = 18;  RGB[2] = 250;  break; }
	    case 6: { RGB[0] = 135;  RGB[1] = 51;  RGB[2] = 36; break; }
	    case 7: { RGB[0] = 138;  RGB[1] = 54;  RGB[2] = 15; break; }
	    case 8: { RGB[0] = 255;  RGB[1] = 69;  RGB[2] = 0;  break;}
	    case 9: { RGB[0] = 8;  RGB[1] = 46;  RGB[2] = 84; }	
	    default: { RGB[0] = 20;  RGB[1] = 255;  RGB[2] = 255; }	
	}
	//*/
	
	/*
	int index;
	index = ID % 10;
	switch (index) {
		case 1: { RGB[0] = 176;  RGB[1] = 23;  RGB[2] = 31; break; }
//	    case 1: { RGB[0] = 25;  RGB[1] = 25;  RGB[2] = 112; break; }
//	    case 2: { RGB[0] = 250;  RGB[1] = 18;  RGB[2] = 250;  break; }
//		case 1: { RGB[0] = 255;  RGB[1] = 128;  RGB[2] = 0; break; }
		case 0: { RGB[0] = 200;  RGB[1] = 34;  RGB[2] = 15; break; }
		case 2: { RGB[0] = 11;  RGB[1] = 69;  RGB[2] = 19;  break; }
	    case 3: { RGB[0] = 227;  RGB[1] = 23;  RGB[2] = 13; break; }
	    case 4: { RGB[0] = 139;  RGB[1] = 69;  RGB[2] = 19; break; }
		case 5: { RGB[0] = 250;  RGB[1] = 18;  RGB[2] = 250;  break; }
	    case 6: { RGB[0] = 135;  RGB[1] = 51;  RGB[2] = 36; break; }
	    case 7: { RGB[0] = 138;  RGB[1] = 54;  RGB[2] = 15; break; }
	    case 8: { RGB[0] = 255;  RGB[1] = 69;  RGB[2] = 0;  break;}
	    case 9: { RGB[0] = 8;  RGB[1] = 46;  RGB[2] = 84; }	
	    default: { RGB[0] = 20;  RGB[1] = 255;  RGB[2] = 255; }	
	}
	//*/
	
}

// 2010.11.23 by Mei Gang at University of Freiburg, Germany.
// Compile polygon surface for displaying
void CRealModelView::CompilePolygonSurfaceList(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int j, k, n, id;
	float *x, *y, *z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];     

    //begin a list
	/////////////////////////////////////////////
	glNewList(list, GL_COMPILE);
	if(lf == 0)  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else 	     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for(j = 0; j < Surf->pLps.GetSize(); j++){
		//get the index and coordinates of nodes
		n = Surf->pLps.GetAt(j)->iNds.GetSize();
		x = new float[n];  y = new float[n];  z = new float[n];
		for(k = 0; k < n; k++){
			id = Surf->pLps.GetAt(j)->iNds.GetAt(k);
			x[k] = Surf->pNds.GetAt(id)->x; 
			y[k] = Surf->pNds.GetAt(id)->y;  
			z[k] = Surf->pNds.GetAt(id)->z; 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		/*
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		*/
		
		//draw polygon
		glBegin(GL_POLYGON); // very important!
		{
			for(k = 0; k < n; k++)  glVertex3d(x[k], y[k], z[k]); 
		}
		glEnd();
		delete [] x;  delete [] y;  delete [] z;
	}				
	glEndList();	

#else
	int j, k, n, id;
	float *x, *y, *z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];     

    //begin a list
	/////////////////////////////////////////////
	glNewList(list, GL_COMPILE);
	if(lf == 0)  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else 	     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for(j = 0; j < Surf->pLps.GetSize(); j++){
		//get the index and coordinates of nodes
		n = Surf->pLps.GetAt(j)->iNds.GetSize();
		x = new float[n];  y = new float[n];  z = new float[n];
		for(k = 0; k < n; k++){
			id = Surf->pLps.GetAt(j)->iNds.GetAt(k);
			x[k] = Surf->pNds.GetAt(id)->x.floatValue(); 
			y[k] = Surf->pNds.GetAt(id)->y.floatValue();  
			z[k] = Surf->pNds.GetAt(id)->z.floatValue(); 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		
		
		//draw polygon
		glBegin(GL_POLYGON); // very important!
		{
			for(k = 0; k < n; k++)  glVertex3d(x[k], y[k], z[k]); 
		}
		glEnd();
		delete [] x;  delete [] y;  delete [] z;
	}				
	glEndList();	

#endif
	
}

/*
//compile tin lists for draw
//newly created by MeiGang, Freiburg, Germany
void CRealModelView::glCompileTinList(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
	int i, j, k, n;
	double x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			x[k] = Surf->pNds.GetAt(n)->x; 
			y[k] = Surf->pNds.GetAt(n)->y;  
			z[k] = Surf->pNds.GetAt(n)->z; 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		
		//draw trgl
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glColor3ub(255, 0, 255);
		glColor3ub(RGB[0], RGB[1], RGB[2]);

	//	glColor3ub(128, 42, 42);
		
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3ub(255, 255, 255);
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
	}			
	
	glEndList();
	
}
*/
// 2010.12.27 by Mei Gang at University of Freiburg, Germany.
// Compile polygon surface for displaying
void CRealModelView::glCompilePolygonSurfaceList(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
#ifndef EGC
#define GL_MULTISAMPLE GLUT_MULTISAMPLE

	int i, j, k, ID;
	GLfloat fPoint0[3], fPoint1[3], fPoint2[3];
	GLint RGB[3]; // RGB color for each BLOCK
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // Adding light source

	// Multisampling
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

	// Create a new display list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5); // Set line width

	// For all vertices, adjust to screen coordinates
	float tx, ty, tz;
	int N = Surf->pNds.GetSize(); // Number
	GLfloat * vertices = new GLfloat[3*N];
	for(i = 0; i < N; i++) {
		tx = Surf->pNds.GetAt(i)->x; 
		ty = Surf->pNds.GetAt(i)->y;  
		tz = Surf->pNds.GetAt(i)->z; 
		vertices[i*3] = (tx - Crt->u) * Crt->Scale;
		vertices[i*3+1] = (ty - Crt->v) * Crt->Scale;
		vertices[i*3+2] = (tz - Crt->w) * Crt->Scale;
	}

	// Create a vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
	
	// For each triangle of a surface mesh
	for(j = 0; j < Surf->pLps.GetSize(); j++){
		if( lf == 1 ) {
			CreateRGBColor(RGB, j);
		}

		CLoop * lp = Surf->pLps.GetAt(j); // The jth triangle

		/*
		// Draw filled polygons with Polygon offset
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glBegin(GL_POLYGON);
		for(k = 0; k < lp->iNds.GetSize(); k++)  {
			ID = lp->iNds.GetAt(k);
			glArrayElement(ID);
		}
		glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	*/
		// Draw wireframe polygons with white color 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Line
		glColor3ub(255, 255, 255);
		glColor3ub(255, 215, 0);
		glBegin(GL_POLYGON);
		for(k = 0; k < lp->iNds.GetSize(); k++)  {
			ID = lp->iNds.GetAt(k);
			glArrayElement(ID);
		}
		glEnd();
	}			
	
	delete [] vertices;
	glEndList(); // End display list
	glDisable(GL_MULTISAMPLE); // End multisampling

#else
	#define GL_MULTISAMPLE GLUT_MULTISAMPLE

	int i, j, k, ID;
	GLfloat fPoint0[3], fPoint1[3], fPoint2[3];
	GLint RGB[3]; // RGB color for each BLOCK
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // Adding light source

	// Multisampling
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

	// Create a new display list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5); // Set line width

	// For all vertices, adjust to screen coordinates
	float tx, ty, tz;
	int N = Surf->pNds.GetSize(); // Number
	GLfloat * vertices = new GLfloat[3*N];
	for(i = 0; i < N; i++) {
		tx = Surf->pNds.GetAt(i)->x.floatValue(); 
		ty = Surf->pNds.GetAt(i)->y.floatValue();  
		tz = Surf->pNds.GetAt(i)->z.floatValue(); 
		vertices[i*3] = (tx - Crt->u) * Crt->Scale;
		vertices[i*3+1] = (ty - Crt->v) * Crt->Scale;
		vertices[i*3+2] = (tz - Crt->w) * Crt->Scale;
	}

	// Create a vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
	
	// For each triangle of a surface mesh
	for(j = 0; j < Surf->pLps.GetSize(); j++){
		if( lf == 1 ) {
			CreateRGBColor(RGB, j);
		}

		CLoop * lp = Surf->pLps.GetAt(j); // The jth triangle

		/*
		// Draw filled polygons with Polygon offset
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glBegin(GL_POLYGON);
		for(k = 0; k < lp->iNds.GetSize(); k++)  {
			ID = lp->iNds.GetAt(k);
			glArrayElement(ID);
		}
		glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	*/
		// Draw wireframe polygons with white color 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Line
		glColor3ub(255, 255, 255);
		glColor3ub(255, 215, 0);
		glBegin(GL_POLYGON);
		for(k = 0; k < lp->iNds.GetSize(); k++)  {
			ID = lp->iNds.GetAt(k);
			glArrayElement(ID);
		}
		glEnd();
	}			
	
	delete [] vertices;
	glEndList(); // End display list
	glDisable(GL_MULTISAMPLE); // End multisampling
#endif

}



void CRealModelView::CompileBlockGroupList(CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int j, k, n;
	double * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];     

	glNewList(list, GL_COMPILE);
	glLineWidth(1.0);
//	glColor3d(0.0, 0.0, 0.5);
//	if(lf == 0)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	else 	   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	for(j = 0; j < m_aLps.GetSize(); j++){
		//get the index and coordinates of nodes
		x = new double[m_aLps.GetAt(j)->iNds.GetSize()];
		y = new double[m_aLps.GetAt(j)->iNds.GetSize()];
		z = new double[m_aLps.GetAt(j)->iNds.GetSize()];
		for(k = 0; k < m_aLps.GetAt(j)->iNds.GetSize(); k++){
			n = m_aLps.GetAt(j)->iNds.GetAt(k);  

			x[k] = m_aNds.GetAt(n)->x; 
			y[k] = m_aNds.GetAt(n)->y;  
			z[k] = m_aNds.GetAt(n)->z; 

			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		/*
		//calculate the normal. can be opened or closed, different effect!
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		//draw trgl
		*/
	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3d(0.0, 0.0, 1.0);
		glBegin(GL_POLYGON);
		{
			for(k = 0; k < m_aLps.GetAt(j)->iNds.GetSize(); k++)  glVertex3d(x[k], y[k], z[k]);//
		}	
		glEnd();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3d(1.0, 0.0, 0.0);
		glLineWidth(2.0);
		glBegin(GL_POLYGON);
		{
			for(k = 0; k < m_aLps.GetAt(j)->iNds.GetSize(); k++) 
				glVertex3d(x[k], y[k], z[k]);//
		}	
		glEnd();

		delete [] x;  delete [] y;  delete [] z;
	}
	
	glEndList();	

#else
	int j, k, n;
	float * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];     

	glNewList(list, GL_COMPILE);
	glLineWidth(1.0);
//	glColor3d(0.0, 0.0, 0.5);
//	if(lf == 0)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	else 	   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	for(j = 0; j < m_aLps.GetSize(); j++){
		//get the index and coordinates of nodes
		x = new float[m_aLps.GetAt(j)->iNds.GetSize()];
		y = new float[m_aLps.GetAt(j)->iNds.GetSize()];
		z = new float[m_aLps.GetAt(j)->iNds.GetSize()];
		for(k = 0; k < m_aLps.GetAt(j)->iNds.GetSize(); k++){
			n = m_aLps.GetAt(j)->iNds.GetAt(k);  

			x[k] = m_aNds.GetAt(n)->x.floatValue(); 
			y[k] = m_aNds.GetAt(n)->y.floatValue();  
			z[k] = m_aNds.GetAt(n)->z.floatValue(); 

			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		/*
		//calculate the normal. can be opened or closed, different effect!
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		//draw trgl
		*/
	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3d(0.0, 0.0, 1.0);
		glBegin(GL_POLYGON);
		{
			for(k = 0; k < m_aLps.GetAt(j)->iNds.GetSize(); k++)  glVertex3d(x[k], y[k], z[k]);//
		}	
		glEnd();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3d(1.0, 0.0, 0.0);
		glLineWidth(2.0);
		glBegin(GL_POLYGON);
		{
			for(k = 0; k < m_aLps.GetAt(j)->iNds.GetSize(); k++) 
				glVertex3d(x[k], y[k], z[k]);//
		}	
		glEnd();

		delete [] x;  delete [] y;  delete [] z;
	}
	
	glEndList();	

#endif

}

void CRealModelView::glCompileBlockGroupList(CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j, k, t, n, id_Nds, id_Lps, id_Phd;
	double * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(0.5);
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new double[lp->iNds.GetSize()];
				y = new double[lp->iNds.GetSize()];
				z = new double[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x; 
					y[t] = m_aNds.GetAt(n)->y;  
					z[t] = m_aNds.GetAt(n)->z; 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				//draw trgl, with Polygon offset, very important !
		        glEnable(GL_POLYGON_OFFSET_FILL);
		        glPolygonOffset(1.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
		        glDisable(GL_POLYGON_OFFSET_FILL);

				/*
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(0.5);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
	}
	
	glEndList();

#else
	int i, j, k, t, n, id_Nds, id_Lps, id_Phd;
	float * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new float[lp->iNds.GetSize()];
				y = new float[lp->iNds.GetSize()];
				z = new float[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x.floatValue(); 
					y[t] = m_aNds.GetAt(n)->y.floatValue();  
					z[t] = m_aNds.GetAt(n)->z.floatValue(); 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
	}
	
	glEndList();

#endif

}

void CRealModelView::glCompileBlockGroupListWithArray(CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j, k, t, n, ID, id_Nds, id_Lps, id_Phd;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	// Multisampling
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH|GLUT_MULTISAMPLE);
    glEnable(GLUT_MULTISAMPLE);

	glNewList(list, GL_COMPILE);
	glLineWidth(0.5);

	// Create Vertex Array
	int N = m_aNds.GetSize();
	GLdouble * vertices = new GLdouble[3*N];
	double tx, ty, tz;

	for(i = 0; i < m_aNds.GetSize(); i++) {
		tx = m_aNds.GetAt(i)->x; 
		ty = m_aNds.GetAt(i)->y;  
		tz = m_aNds.GetAt(i)->z; 
		tx = (tx - Crt->u) * Crt->Scale;	
		ty = (ty - Crt->v) * Crt->Scale;	
		tz = (tz - Crt->w) * Crt->Scale;
		vertices[i*3] = tx;
		vertices[i*3+1] = ty;
		vertices[i*3+2] = tz;
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, vertices);
    
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				//draw trgl, with Polygon offset, very important !
		        glEnable(GL_POLYGON_OFFSET_FILL);
		        glPolygonOffset(1.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						ID = lp->iNds.GetAt(t);
						glArrayElement(ID);
					}
				}	
				glEnd();
		        glDisable(GL_POLYGON_OFFSET_FILL);
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(0.5);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						ID = lp->iNds.GetAt(t);
						glArrayElement(ID);
					}
				}	
				glEnd();

			}
		}
	}
	
	delete [] vertices;
	glEndList();

	// End multisampling
	glDisable(GLUT_MULTISAMPLE);
#else

#endif

}

void CRealModelView::glCompileBlockGroupListBest_Boundary(CCordnt* Crt, int list)
{
#ifndef EGC

#define GL_MULTISAMPLE GLUT_MULTISAMPLE

	int i, j, k, t, ID, id_Nds, id_Lps, id_Phd;
	GLfloat fPoint0[3], fPoint1[3], fPoint2[3];
	GLint RGB[3]; // RGB color for each BLOCK

	glMyLight(); // Adding light source

	// Multisampling
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

	// Create a new display list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5); // Set line width

	// For all vertices, adjust to screen coordinates
	float tx, ty, tz;
	int N = m_aNds.GetSize(); // Number
	GLfloat * vertices = new GLfloat[3*N];
	for(i = 0; i < N; i++) {
		tx = m_aNds.GetAt(i)->x; 
		ty = m_aNds.GetAt(i)->y;  
		tz = m_aNds.GetAt(i)->z; 
		vertices[i*3] = (tx - Crt->u) * Crt->Scale;
		vertices[i*3+1] = (ty - Crt->v) * Crt->Scale;
		vertices[i*3+2] = (tz - Crt->w) * Crt->Scale;
	}

	// Create a vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
	
	// Check Boundary Phds
	for(i = 0; i < m_aLps.GetSize(); i++) { // Clear
		m_aLps.GetAt(i)->UseTime = 0;
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) { // Use time
		for(j = 0; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			id_Lps = m_aPhds.GetAt(i)->iLps.GetAt(j);
			m_aLps.GetAt(id_Lps)->UseTime += 1;
		}
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		m_aPhds.GetAt(i)->flag = 0; // Set as interior
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = 0; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			id_Lps = m_aPhds.GetAt(i)->iLps.GetAt(j);
			if(m_aLps.GetAt(id_Lps)->UseTime == 1) {
				m_aPhds.GetAt(i)->flag = 1;  break;
			}
		}
	}

	// For each BLOCK
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}

		// For each POLYHEDRON of a BLOCK
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);

			// Check
			if(phd->flag == 0)  continue;

			// For each polygonal face of a POLYHEDRON
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				// Draw filled polygons with Polygon offset
		        glEnable(GL_POLYGON_OFFSET_FILL);
		        glPolygonOffset(1.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						ID = lp->iNds.GetAt(t);
						glArrayElement(ID);
					}
				glEnd();
		        glDisable(GL_POLYGON_OFFSET_FILL);
				
				// Draw wireframe polygons with white color 
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glBegin(GL_POLYGON);
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						ID = lp->iNds.GetAt(t);
						glArrayElement(ID);
					}
				glEnd();
			}
		}
	}
	
	delete [] vertices;
	glEndList(); // End display list
	glDisable(GL_MULTISAMPLE); // End multisampling

#else
	#define GL_MULTISAMPLE GLUT_MULTISAMPLE

	int i, j, k, t, ID, id_Nds, id_Lps, id_Phd;
	GLfloat fPoint0[3], fPoint1[3], fPoint2[3];
	GLint RGB[3]; // RGB color for each BLOCK

	glMyLight(); // Adding light source

	// Multisampling
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

	// Create a new display list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5); // Set line width

	// For all vertices, adjust to screen coordinates
	float tx, ty, tz;
	int N = m_aNds.GetSize(); // Number
	GLfloat * vertices = new GLfloat[3*N];
	for(i = 0; i < N; i++) {
		tx = m_aNds.GetAt(i)->x.floatValue(); 
		ty = m_aNds.GetAt(i)->y.floatValue();  
		tz = m_aNds.GetAt(i)->z.floatValue(); 
		vertices[i*3] = (tx - Crt->u) * Crt->Scale;
		vertices[i*3+1] = (ty - Crt->v) * Crt->Scale;
		vertices[i*3+2] = (tz - Crt->w) * Crt->Scale;
	}

	// Create a vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
	
	// For each BLOCK
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}

		// For each POLYHEDRON of a BLOCK
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);

			// For each polygonal face of a POLYHEDRON
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				// Draw filled polygons with Polygon offset
		        glEnable(GL_POLYGON_OFFSET_FILL);
		        glPolygonOffset(1.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						ID = lp->iNds.GetAt(t);
						glArrayElement(ID);
					}
				glEnd();
		        glDisable(GL_POLYGON_OFFSET_FILL);
				
				// Draw wireframe polygons with white color 
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glBegin(GL_POLYGON);
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						ID = lp->iNds.GetAt(t);
						glArrayElement(ID);
					}
				glEnd();
			}
		}
	}
	
	delete [] vertices;
	glEndList(); // End display list
	glDisable(GL_MULTISAMPLE); // End multisampling

#endif

}

void CRealModelView::glCompileBlockGroupListBest(CCordnt* Crt, int list)
{
#ifndef EGC

#define GL_MULTISAMPLE GLUT_MULTISAMPLE

	int i, j, k, t, ID, id_Nds, id_Lps, id_Phd;
	GLfloat fPoint0[3], fPoint1[3], fPoint2[3];
	GLint RGB[3]; // RGB color for each BLOCK

	glMyLight(); // Adding light source

	// Multisampling
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

	// Create a new display list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5); // Set line width

	// For all vertices, adjust to screen coordinates
	float tx, ty, tz;
	int N = m_aNds.GetSize(); // Number
	GLfloat * vertices = new GLfloat[3*N];
	for(i = 0; i < N; i++) {
		tx = m_aNds.GetAt(i)->x; 
		ty = m_aNds.GetAt(i)->y;  
		tz = m_aNds.GetAt(i)->z; 
		vertices[i*3] = (tx - Crt->u) * Crt->Scale;
		vertices[i*3+1] = (ty - Crt->v) * Crt->Scale;
		vertices[i*3+2] = (tz - Crt->w) * Crt->Scale;
	}

	// Create a vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
	
	// For each BLOCK
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}

		// For each POLYHEDRON of a BLOCK
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);

			// For each polygonal face of a POLYHEDRON
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				// Draw filled polygons with Polygon offset
		        glEnable(GL_POLYGON_OFFSET_FILL);
		        glPolygonOffset(1.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						ID = lp->iNds.GetAt(t);
						glArrayElement(ID);
					}
				glEnd();
		        glDisable(GL_POLYGON_OFFSET_FILL);
				
				// Draw wireframe polygons with white color 
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glBegin(GL_POLYGON);
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						ID = lp->iNds.GetAt(t);
						glArrayElement(ID);
					}
				glEnd();
			}
		}
	}
	
	delete [] vertices;
	glEndList(); // End display list
	glDisable(GL_MULTISAMPLE); // End multisampling

#else
	#define GL_MULTISAMPLE GLUT_MULTISAMPLE

	int i, j, k, t, ID, id_Nds, id_Lps, id_Phd;
	GLfloat fPoint0[3], fPoint1[3], fPoint2[3];
	GLint RGB[3]; // RGB color for each BLOCK

	glMyLight(); // Adding light source

	// Multisampling
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

	// Create a new display list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5); // Set line width

	// For all vertices, adjust to screen coordinates
	float tx, ty, tz;
	int N = m_aNds.GetSize(); // Number
	GLfloat * vertices = new GLfloat[3*N];
	for(i = 0; i < N; i++) {
		tx = m_aNds.GetAt(i)->x.floatValue(); 
		ty = m_aNds.GetAt(i)->y.floatValue();  
		tz = m_aNds.GetAt(i)->z.floatValue(); 
		vertices[i*3] = (tx - Crt->u) * Crt->Scale;
		vertices[i*3+1] = (ty - Crt->v) * Crt->Scale;
		vertices[i*3+2] = (tz - Crt->w) * Crt->Scale;
	}

	// Create a vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
	
	// For each BLOCK
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}

		// For each POLYHEDRON of a BLOCK
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);

			// For each polygonal face of a POLYHEDRON
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				// Draw filled polygons with Polygon offset
		        glEnable(GL_POLYGON_OFFSET_FILL);
		        glPolygonOffset(1.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						ID = lp->iNds.GetAt(t);
						glArrayElement(ID);
					}
				glEnd();
		        glDisable(GL_POLYGON_OFFSET_FILL);
				
				// Draw wireframe polygons with white color 
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glBegin(GL_POLYGON);
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						ID = lp->iNds.GetAt(t);
						glArrayElement(ID);
					}
				glEnd();
			}
		}
	}
	
	delete [] vertices;
	glEndList(); // End display list
	glDisable(GL_MULTISAMPLE); // End multisampling

#endif

}

//compile tin lists for draw with Antialiasing
//newly created by MeiGang, Freiburg, Germany. Antialising is not valid !
void CRealModelView::glCompileSurfaceMeshListBest(CSurf* Surf, CCordnt* Crt, int list)
{
    #define GL_MULTISAMPLE GLUT_MULTISAMPLE

	int i, j, k, ID;
	GLfloat fPoint0[3], fPoint1[3], fPoint2[3];
	GLint RGB[3]; // RGB color for each BLOCK
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // Adding light source

	// Multisampling
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

	// Create a new display list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5); // Set line width

	// For all vertices, adjust to screen coordinates
	float tx, ty, tz;
	int N = Surf->pNds.GetSize(); // Number
	GLfloat * vertices = new GLfloat[3*N];
	for(i = 0; i < N; i++) {
#ifndef EGC
		tx = Surf->pNds.GetAt(i)->x; 
		ty = Surf->pNds.GetAt(i)->y;  
		tz = Surf->pNds.GetAt(i)->z; 
#else
		tx = Surf->pNds.GetAt(i)->x.floatValue(); 
		ty = Surf->pNds.GetAt(i)->y.floatValue();  
		tz = Surf->pNds.GetAt(i)->z.floatValue(); 
#endif
		vertices[i*3] = (tx - Crt->u) * Crt->Scale;
		vertices[i*3+1] = (ty - Crt->v) * Crt->Scale;
		vertices[i*3+2] = (tz - Crt->w) * Crt->Scale;
	}

	// Create a vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
	
	// For each triangle of a surface mesh
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		CTrgl * tr = Surf->pTrs.GetAt(j); // The jth triangle

		// Draw filled polygons with Polygon offset
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glBegin(GL_TRIANGLES);
	        for(k = 0; k < 3; k++)  {
				ID = tr->nID[k];
				glArrayElement(ID);
			}
		glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	
		// Draw wireframe polygons with white color 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Line
		glColor3ub(255, 255, 255); // glColor3ub(255, 215, 0);
		glBegin(GL_TRIANGLES);
	        for(k = 0; k < 3; k++)  {
				ID = tr->nID[k];
				glArrayElement(ID);
			}
		glEnd();
	}			
	
	delete [] vertices;
	glEndList(); // End display list
	glDisable(GL_MULTISAMPLE); // End multisampling
}


void CRealModelView::glCompileBlockGroupList(CCordnt* Crt, int *list, int lf)
{
#ifndef EGC
	int i, j, k, t, n, id_Nds, id_Lps, id_Phd;
	double * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	for(i = 0; i < m_aBlks.GetSize(); i++) {
		glNewList(list[i], GL_COMPILE);
	    glLineWidth(2.0);

		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new double[lp->iNds.GetSize()];
				y = new double[lp->iNds.GetSize()];
				z = new double[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x; 
					y[t] = m_aNds.GetAt(n)->y;  
					z[t] = m_aNds.GetAt(n)->z; 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
		glEndList();
	}

#else
	int i, j, k, t, n, id_Nds, id_Lps, id_Phd;
	float * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	for(i = 0; i < m_aBlks.GetSize(); i++) {
		glNewList(list[i], GL_COMPILE);
	    glLineWidth(2.0);

		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new float[lp->iNds.GetSize()];
				y = new float[lp->iNds.GetSize()];
				z = new float[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x.floatValue(); 
					y[t] = m_aNds.GetAt(n)->y.floatValue();  
					z[t] = m_aNds.GetAt(n)->z.floatValue(); 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
		glEndList();
	}
	
#endif

}

// 2011.11.26
// Compile block, point view
void CRealModelView::glCompileBlockGroupListPoint(CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j;
	double x, y, z;
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	glPointSize(3.5f);
	glBegin(GL_POINTS);
	for(i = 0; i < m_aNds.GetSize(); i++) {
		x = m_aNds.GetAt(i)->x; 
		y = m_aNds.GetAt(i)->y;  
		z = m_aNds.GetAt(i)->z; 
		x = (x - Crt->u) * Crt->Scale;	
		y = (y - Crt->v) * Crt->Scale;	
		z = (z - Crt->w) * Crt->Scale;

		// Create color
		CreateRGBColor(RGB, i);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glVertex3d(x, y, z);
	}
	glEnd();
	glEndList();

#else
	int i, j;
	float x, y, z;
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	glPointSize(3.5f);
	glBegin(GL_POINTS);
	for(i = 0; i < m_aNds.GetSize(); i++) {
		x = m_aNds.GetAt(i)->x.floatValue(); 
		y = m_aNds.GetAt(i)->y.floatValue();  
		z = m_aNds.GetAt(i)->z.floatValue(); 
		x = (x - Crt->u) * Crt->Scale;	
		y = (y - Crt->v) * Crt->Scale;	
		z = (z - Crt->w) * Crt->Scale;

		// Create color
		CreateRGBColor(RGB, i);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glVertex3d(x, y, z);
	}
	glEnd();
	glEndList();

#endif

}

// 2011.11.26
// Segment view
void CRealModelView::glCompileBlockGroupListSegment(CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j, k, t, n, id_Nds, id_Lps, id_Phd;
	double * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(1.0);
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new double[lp->iNds.GetSize()];
				y = new double[lp->iNds.GetSize()];
				z = new double[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x; 
					y[t] = m_aNds.GetAt(n)->y;  
					z[t] = m_aNds.GetAt(n)->z; 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				/*
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				CreateRGBColor(RGB, id_Lps);
				glLineWidth(1.0);
				glBegin(GL_POLYGON);
					glColor3ub(RGB[0], RGB[1], RGB[2]);
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						glVertex3d(x[t], y[t], z[t]);
					}
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
	}
	
	glEndList();

#else
	int i, j, k, t, n, id_Nds, id_Lps, id_Phd;
	float * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new float[lp->iNds.GetSize()];
				y = new float[lp->iNds.GetSize()];
				z = new float[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x.floatValue(); 
					y[t] = m_aNds.GetAt(n)->y.floatValue();  
					z[t] = m_aNds.GetAt(n)->z.floatValue(); 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				/*
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				CreateRGBColor(RGB, id_Lps);
				glLineWidth(1.0);
				glBegin(GL_POLYGON);
					glColor3ub(RGB[0], RGB[1], RGB[2]);
					for(t = 0; t < lp->iNds.GetSize(); t++) {
						glVertex3d(x[t], y[t], z[t]);
					}
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
	}
	
	glEndList();

#endif

}

void CRealModelView::glCompileBlockGroupListArea(CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j, k, t, n, id_Nds, id_Lps, id_Phd;
	double * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(0.5);
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new double[lp->iNds.GetSize()];
				y = new double[lp->iNds.GetSize()];
				z = new double[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x; 
					y[t] = m_aNds.GetAt(n)->y;  
					z[t] = m_aNds.GetAt(n)->z; 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/


				// Draw filled polygons with Polygon offset
		        glEnable(GL_POLYGON_OFFSET_FILL);
		        glPolygonOffset(1.0, 1.0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				CreateRGBColor(RGB, id_Lps);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				glDisable(GL_POLYGON_OFFSET_FILL);
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(0.5);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
	}
	
	glEndList();

#else
	int i, j, k, t, n, id_Nds, id_Lps, id_Phd;
	float * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new float[lp->iNds.GetSize()];
				y = new float[lp->iNds.GetSize()];
				z = new float[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x.floatValue(); 
					y[t] = m_aNds.GetAt(n)->y.floatValue();  
					z[t] = m_aNds.GetAt(n)->z.floatValue(); 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				CreateRGBColor(RGB, id_Lps);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
	}
	
	glEndList();

#endif

}

void CRealModelView::glCompileBlockGroupList_Select(CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int j, k, t, n, id_Nds, id_Lps, id_Phd;
	double * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	
	glMyLight(); // add light
	
	// initiate name stack
	glInitNames();
	glPushName(0);
	
	if( bPicked == false) {
		
		//begin a list
		/////////////////////////////////////////////
		glNewList(list, GL_COMPILE);
		
		glLineWidth(2.0);
		for(j = 0; j < m_aPhds.GetSize(); j++) {
			glLoadName(j+1); // not j, but j+1
			
			CPohd * phd = m_aPhds.GetAt(j);
			// create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}
			
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new double[lp->iNds.GetSize()];
				y = new double[lp->iNds.GetSize()];
				z = new double[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x; 
					y[t] = m_aNds.GetAt(n)->y;  
					z[t] = m_aNds.GetAt(n)->z; 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
		
		glEndList();
		
	}
	
	else {
		if(nPrePicked != nCurPicked) {
			// a single polyhedron is picked
			glNewList(list, GL_COMPILE);
			
			glLineWidth(2.0);
			j = nPicked - 1;
			glLoadName(j+1); // not j, but j+1
			
			CPohd * phd = m_aPhds.GetAt(j);
			// create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}
			
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new double[lp->iNds.GetSize()];
				y = new double[lp->iNds.GetSize()];
				z = new double[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x; 
					y[t] = m_aNds.GetAt(n)->y;  
					z[t] = m_aNds.GetAt(n)->z; 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
			
			glEndList();
		}

		if(nPrePicked == nCurPicked) {
			// a single polyhedron is picked
			glNewList(list, GL_COMPILE);
			
			glLineWidth(2.0);
			j = nPicked - 1;
			glLoadName(j+1); // not j, but j+1
			
			CPohd * phd = m_aPhds.GetAt(j);
			// create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}
			
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new double[lp->iNds.GetSize()];
				y = new double[lp->iNds.GetSize()];
				z = new double[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x; 
					y[t] = m_aNds.GetAt(n)->y;  
					z[t] = m_aNds.GetAt(n)->z; 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
			
			// neighbor polyhedrons
			for(j = 0; j < phd->pNbs.GetSize(); j++) {
				CPohd * nb = phd->pNbs.GetAt(j);
				// create color for each polyhedron
				if( lf == 1 ) {
					CreateRGBColor(RGB, phd->iNbs.GetAt(j));
				}
				
				for(k = 0; k < nb->iLps.GetSize(); k++) {
					id_Lps = nb->iLps.GetAt(k);
					CLoop * lp = m_aLps.GetAt(id_Lps);
					
					//get the index and coordinates of nodes
					x = new double[lp->iNds.GetSize()];
					y = new double[lp->iNds.GetSize()];
					z = new double[lp->iNds.GetSize()];
					for(t = 0; t < lp->iNds.GetSize(); t++){
						n = lp->iNds.GetAt(t);  
						x[t] = m_aNds.GetAt(n)->x; 
						y[t] = m_aNds.GetAt(n)->y;  
						z[t] = m_aNds.GetAt(n)->z; 
						x[t] = (x[t] - Crt->u) * Crt->Scale;	
						y[t] = (y[t] - Crt->v) * Crt->Scale;	
						z[t] = (z[t] - Crt->w) * Crt->Scale;
					}
					
					/*
					//calculate the normal. can be opened or closed, different effect!
					fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
					fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
					fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
					CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
					glNormal3dv(fNormal);
					*/
					
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glColor3ub(RGB[0], RGB[1], RGB[2]);
					glBegin(GL_POLYGON);
					{
						for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
					}	
					glEnd();
					
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glColor3ub(255, 255, 255);
					glLineWidth(2.0);
					glBegin(GL_POLYGON);
					{
						for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
					}	
					glEnd();
					
					delete [] x;  delete [] y;  delete [] z;
				}
				
			}
			glEndList();
		}

	}

#else
	int j, k, t, n, id_Nds, id_Lps, id_Phd;
	float * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	
	glMyLight(); // add light
	
	// initiate name stack
	glInitNames();
	glPushName(0);
	
	if( bPicked == false) {
		
		//begin a list
		/////////////////////////////////////////////
		glNewList(list, GL_COMPILE);
		
		glLineWidth(2.0);
		for(j = 0; j < m_aPhds.GetSize(); j++) {
			glLoadName(j+1); // not j, but j+1
			
			CPohd * phd = m_aPhds.GetAt(j);
			// create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}
			
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new float[lp->iNds.GetSize()];
				y = new float[lp->iNds.GetSize()];
				z = new float[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x.floatValue(); 
					y[t] = m_aNds.GetAt(n)->y.floatValue();  
					z[t] = m_aNds.GetAt(n)->z.floatValue(); 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
		
		glEndList();
		
	}
	
	else {
		if(nPrePicked != nCurPicked) {
			// a single polyhedron is picked
			glNewList(list, GL_COMPILE);
			
			glLineWidth(2.0);
			j = nPicked - 1;
			glLoadName(j+1); // not j, but j+1
			
			CPohd * phd = m_aPhds.GetAt(j);
			// create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}
			
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new float[lp->iNds.GetSize()];
				y = new float[lp->iNds.GetSize()];
				z = new float[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x.floatValue(); 
					y[t] = m_aNds.GetAt(n)->y.floatValue();  
					z[t] = m_aNds.GetAt(n)->z.floatValue(); 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
			
			glEndList();
		}

		if(nPrePicked == nCurPicked) {
			// a single polyhedron is picked
			glNewList(list, GL_COMPILE);
			
			glLineWidth(2.0);
			j = nPicked - 1;
			glLoadName(j+1); // not j, but j+1
			
			CPohd * phd = m_aPhds.GetAt(j);
			// create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}
			
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new float[lp->iNds.GetSize()];
				y = new float[lp->iNds.GetSize()];
				z = new float[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x.floatValue(); 
					y[t] = m_aNds.GetAt(n)->y.floatValue();  
					z[t] = m_aNds.GetAt(n)->z.floatValue(); 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
			
			// neighbor polyhedrons
			for(j = 0; j < phd->pNbs.GetSize(); j++) {
				CPohd * nb = phd->pNbs.GetAt(j);
				// create color for each polyhedron
				if( lf == 1 ) {
					CreateRGBColor(RGB, phd->iNbs.GetAt(j));
				}
				
				for(k = 0; k < nb->iLps.GetSize(); k++) {
					id_Lps = nb->iLps.GetAt(k);
					CLoop * lp = m_aLps.GetAt(id_Lps);
					
					//get the index and coordinates of nodes
					x = new float[lp->iNds.GetSize()];
					y = new float[lp->iNds.GetSize()];
					z = new float[lp->iNds.GetSize()];
					for(t = 0; t < lp->iNds.GetSize(); t++){
						n = lp->iNds.GetAt(t);  
						x[t] = m_aNds.GetAt(n)->x.floatValue(); 
						y[t] = m_aNds.GetAt(n)->y.floatValue();  
						z[t] = m_aNds.GetAt(n)->z.floatValue(); 
						x[t] = (x[t] - Crt->u) * Crt->Scale;	
						y[t] = (y[t] - Crt->v) * Crt->Scale;	
						z[t] = (z[t] - Crt->w) * Crt->Scale;
					}
					
					/*
					//calculate the normal. can be opened or closed, different effect!
					fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
					fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
					fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
					CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
					glNormal3dv(fNormal);
					*/
					
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glColor3ub(RGB[0], RGB[1], RGB[2]);
					glBegin(GL_POLYGON);
					{
						for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
					}	
					glEnd();
					
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glColor3ub(255, 255, 255);
					glLineWidth(2.0);
					glBegin(GL_POLYGON);
					{
						for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
					}	
					glEnd();
					
					delete [] x;  delete [] y;  delete [] z;
				}
				
			}
			glEndList();
		}

	}

#endif

}

//compile tin lists for draw
//newly created by MeiGang, Freiburg, Germany. 2013.03.25
void CRealModelView::glCompileTinList(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j, k, n;
	double x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			x[k] = Surf->pNds.GetAt(n)->x; 
			y[k] = Surf->pNds.GetAt(n)->y;  
			z[k] = Surf->pNds.GetAt(n)->z; 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		/*
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		*/
		
		//draw trgl, with Polygon offset, very important !
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glBegin(GL_TRIANGLES);
		    for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	
		// draw wireframe
		// Note: antianlising is NOT valid here, very important !
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Line
		glColor3ub(255, 255, 255);
		glBegin(GL_TRIANGLES);
	        for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
	}			
	glEndList();

#else
	int i, j, k, n;
	float x[3], y[3], z[3];
	GLfloat fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			x[k] = Surf->pNds.GetAt(n)->x.floatValue(); 
			y[k] = Surf->pNds.GetAt(n)->y.floatValue();  
			z[k] = Surf->pNds.GetAt(n)->z.floatValue(); 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		/*
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormalEGC(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3fv(fNormal);
		*/
		//draw trgl
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
	//	glColor3ub(255, 0, 255);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
	//	glColor3ub(128, 42, 42);
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Line
		glColor3ub(255, 255, 255);
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
	}			
	
	glEndList();

#endif
	
}

//compile tin lists for draw with Antialiasing
//newly created by MeiGang, Freiburg, Germany. Antialising is not valid !
void CRealModelView::glCompileTinListAnti(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j, k, n;
	double x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			x[k] = Surf->pNds.GetAt(n)->x; 
			y[k] = Surf->pNds.GetAt(n)->y;  
			z[k] = Surf->pNds.GetAt(n)->z; 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		/*
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		*/
		
		//draw trgl, with Polygon offset, very important !
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glBegin(GL_TRIANGLES);
		    for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	
		// draw wireframe
		// Note: antianlising is NOT valid here, very important !
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Line
		glColor3ub(255, 255, 255);
		glBegin(GL_TRIANGLES);
	        for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
	}			
	glEndList();

#else
	int i, j, k, n;
	float x[3], y[3], z[3];
	GLfloat fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			x[k] = Surf->pNds.GetAt(n)->x.floatValue(); 
			y[k] = Surf->pNds.GetAt(n)->y.floatValue();  
			z[k] = Surf->pNds.GetAt(n)->z.floatValue(); 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		/*
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormalEGC(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3fv(fNormal);
		*/
		//draw trgl
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
	//	glColor3ub(255, 0, 255);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
	//	glColor3ub(128, 42, 42);
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Line
		glColor3ub(255, 255, 255);
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
	}			
	
	glEndList();

#endif
	
	
}

// 2011.11.26
//compile tin lists for draw
//newly created by MeiGang, Freiburg, Germany
void CRealModelView::glCompileTinListPoint(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j;
	double x, y, z;
	GLint RGB[3];
	
	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	for(j = 0; j < Surf->pNds.GetSize(); j++){
		//get the index and coordinates of nodes
		x = Surf->pNds.GetAt(j)->x; 
		y = Surf->pNds.GetAt(j)->y;  
		z = Surf->pNds.GetAt(j)->z; 
		x = (x - Crt->u) * Crt->Scale;	
		y = (y - Crt->v) * Crt->Scale;	
		z = (z - Crt->w) * Crt->Scale;
		
	    // Create color
		CreateRGBColor(RGB, j);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glVertex3f(x, y, z);
	}			
	glEnd();
	
	glEndList();

#else
	int i, j;
	float x, y, z;
	GLint RGB[3];
	
	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	glPointSize(3.5f);
	glBegin(GL_POINTS);
	for(j = 0; j < Surf->pNds.GetSize(); j++){
		//get the index and coordinates of nodes
		x = Surf->pNds.GetAt(j)->x.floatValue(); 
		y = Surf->pNds.GetAt(j)->y.floatValue();  
		z = Surf->pNds.GetAt(j)->z.floatValue(); 
		x = (x - Crt->u) * Crt->Scale;	
		y = (y - Crt->v) * Crt->Scale;	
		z = (z - Crt->w) * Crt->Scale;
		
	    // Create color
		CreateRGBColor(RGB, j);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glVertex3f(x, y, z);
	}			
	glEnd();
	
	glEndList();

#endif

}

// 2012.11.18 Antialising
//compile tin lists for draw
//newly created by MeiGang, Freiburg, Germany
void CRealModelView::glCompileTinListPointAnti(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j;
	double x, y, z;
	GLint RGB[3];
	
	glMyLight(); // add light

	// Anti-alising
	glEnable(GL_BLEND); // 启用混合
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_ALPHA); // 设置函数
    glEnable(GL_POINT_SMOOTH); // 启用点反走样
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	glPointSize(3.5f);
	glBegin(GL_POINTS);
	for(j = 0; j < Surf->pNds.GetSize(); j++){
		//get the index and coordinates of nodes
		x = Surf->pNds.GetAt(j)->x; 
		y = Surf->pNds.GetAt(j)->y;  
		z = Surf->pNds.GetAt(j)->z; 
		x = (x - Crt->u) * Crt->Scale;	
		y = (y - Crt->v) * Crt->Scale;	
		z = (z - Crt->w) * Crt->Scale;
		
	    // Create color
		CreateRGBColor(RGB, j);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glVertex3f(x, y, z);
	}			
	glEnd();
	
	glEndList();

#else
	int i, j;
	float x, y, z;
	GLint RGB[3];
	
	glMyLight(); // add light

	// Anti-alising
	glEnable(GL_BLEND); // 启用混合
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_ALPHA); // 设置函数
    glEnable(GL_POINT_SMOOTH); // 启用点反走样
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	glPointSize(3.5f);
	glBegin(GL_POINTS);
	for(j = 0; j < Surf->pNds.GetSize(); j++){
		//get the index and coordinates of nodes
		x = Surf->pNds.GetAt(j)->x.floatValue(); 
		y = Surf->pNds.GetAt(j)->y.floatValue();  
		z = Surf->pNds.GetAt(j)->z.floatValue(); 
		x = (x - Crt->u) * Crt->Scale;	
		y = (y - Crt->v) * Crt->Scale;	
		z = (z - Crt->w) * Crt->Scale;
		
	    // Create color
		CreateRGBColor(RGB, j);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glVertex3f(x, y, z);
	}			
	glEnd();
	
	glEndList();

#endif

}

// 2011.11.26
//compile tin lists for draw
//newly created by MeiGang, Freiburg, Germany
void CRealModelView::glCompileTinListSegment(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j, k, n, t, nID[3];
	double x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3], RGB1[3], RGB2[3];
	GLint tRGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			nID[k] = Surf->pTrs.GetAt(j)->nID[k]; 
			x[k] = Surf->pNds.GetAt(n)->x; 
			y[k] = Surf->pNds.GetAt(n)->y;  
			z[k] = Surf->pNds.GetAt(n)->z; 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		/*
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		*/

		//draw trgl
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	CreateRGBColor(RGB, j);
	//	glColor3ub(RGB[0], RGB[1], RGB[2]);

		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++) {
			CreateRGBColor(RGB, nID[k]);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			glVertex3d(x[k], y[k], z[k]);//
		}
		glEnd();
	}			
	glEndList();

#else
	int i, j, k, n, t, nID[3];
	float x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3], RGB1[3], RGB2[3];
	GLint tRGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(1.3);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			nID[k] = Surf->pTrs.GetAt(j)->nID[k]; 
			x[k] = Surf->pNds.GetAt(n)->x.floatValue(); 
			y[k] = Surf->pNds.GetAt(n)->y.floatValue();  
			z[k] = Surf->pNds.GetAt(n)->z.floatValue(); 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		
		//draw trgl
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	CreateRGBColor(RGB, j);
	//	glColor3ub(RGB[0], RGB[1], RGB[2]);

		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++) {
			CreateRGBColor(RGB, nID[k]);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			glVertex3d(x[k], y[k], z[k]);//
		}
		glEnd();
	}			
	glEndList();

#endif
	
}

// 2011.11.26
//compile tin lists for draw
//newly created by MeiGang, Freiburg, Germany
void CRealModelView::glCompileTinListSegmentAnti(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j, k, n, t, nID[3], Count = 0;
	double x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3], RGB1[3], RGB2[3];
	GLint tRGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

	// Anti-alising
	glEnable(GL_BLEND); // 启用混合
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_ALPHA); // 设置函3
	glEnable(GL_POINT_SMOOTH); // 启用点反走样
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_LINE_SMOOTH); // 启用点反走样
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5);
//	glPointSize(5.0);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			nID[k] = Surf->pTrs.GetAt(j)->nID[k]; 
			x[k] = Surf->pNds.GetAt(n)->x; 
			y[k] = Surf->pNds.GetAt(n)->y;  
			z[k] = Surf->pNds.GetAt(n)->z; 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		/*
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		*/
		
		/*
		glBegin(GL_POINTS);
		for(k = 0; k < 3; k++) {
			CreateRGBColor(RGB, nID[k]);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			glVertex3d(x[k], y[k], z[k]);//
		}
		glEnd();
		*/

		glBegin(GL_LINES);
			CreateRGBColor(RGB, Count++);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			k = 0;  glVertex3d(x[k], y[k], z[k]);
			k = 1;  glVertex3d(x[k], y[k], z[k]);

			CreateRGBColor(RGB, Count++);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			k = 1;  glVertex3d(x[k], y[k], z[k]);
			k = 2;  glVertex3d(x[k], y[k], z[k]);

			CreateRGBColor(RGB, Count++);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			k = 2;  glVertex3d(x[k], y[k], z[k]);
			k = 0;  glVertex3d(x[k], y[k], z[k]);
		glEnd();



		//draw trgl
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	CreateRGBColor(RGB, j);
	//	glColor3ub(RGB[0], RGB[1], RGB[2]);

		/*
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++) {
			CreateRGBColor(RGB, nID[k]);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			glVertex3d(x[k], y[k], z[k]);//
		}
		glEnd();
		*/
	}			
	glEndList();

#else
	int i, j, k, n, t, nID[3], Count = 0;
	float x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3], RGB1[3], RGB2[3];
	GLint tRGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

	// Anti-alising
	glEnable(GL_BLEND); // 启用混合
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_ALPHA); // 设置函3
	glEnable(GL_POINT_SMOOTH); // 启用点反走样
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_LINE_SMOOTH); // 启用点反走样
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(1.3);
//	glPointSize(5.0);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			nID[k] = Surf->pTrs.GetAt(j)->nID[k]; 
			x[k] = Surf->pNds.GetAt(n)->x.floatValue(); 
			y[k] = Surf->pNds.GetAt(n)->y.floatValue();  
			z[k] = Surf->pNds.GetAt(n)->z.floatValue(); 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		
		/*
		glBegin(GL_POINTS);
		for(k = 0; k < 3; k++) {
			CreateRGBColor(RGB, nID[k]);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			glVertex3d(x[k], y[k], z[k]);//
		}
		glEnd();
		*/

		glBegin(GL_LINES);
			CreateRGBColor(RGB, Count++);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			k = 0;  glVertex3d(x[k], y[k], z[k]);
			k = 1;  glVertex3d(x[k], y[k], z[k]);

			CreateRGBColor(RGB, Count++);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			k = 1;  glVertex3d(x[k], y[k], z[k]);
			k = 2;  glVertex3d(x[k], y[k], z[k]);

			CreateRGBColor(RGB, Count++);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			k = 2;  glVertex3d(x[k], y[k], z[k]);
			k = 0;  glVertex3d(x[k], y[k], z[k]);
		glEnd();



		//draw trgl
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	CreateRGBColor(RGB, j);
	//	glColor3ub(RGB[0], RGB[1], RGB[2]);

		/*
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++) {
			CreateRGBColor(RGB, nID[k]);
		    glColor3ub(RGB[0], RGB[1], RGB[2]);
			glVertex3d(x[k], y[k], z[k]);//
		}
		glEnd();
		*/
	}			
	glEndList();

#endif
	
}

//compile tin lists for draw
//newly created by MeiGang, Freiburg, Germany
void CRealModelView::glCompileTinListArea(CSurf* Surf, CCordnt* Crt, int list, int lf)
{
#ifndef EGC
	int i, j, k, n;
	double x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(0.5);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			x[k] = Surf->pNds.GetAt(n)->x; 
			y[k] = Surf->pNds.GetAt(n)->y;  
			z[k] = Surf->pNds.GetAt(n)->z; 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		/*
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		*/

		
		//draw trgl
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	    CreateRGBColor(RGB, j);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		

	//	glColor3ub(128, 42, 42);
		
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3ub(255, 255, 255);
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
	}			
	
	glEndList();

#else
	int i, j, k, n;
	float x[3], y[3], z[3];
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	for(i = 0; i < 3; i++) {
		RGB[i] = Surf->RGB[i];
	}

	glMyLight(); // add light

    //begin a list
	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	for(j = 0; j < Surf->pTrs.GetSize(); j++){
		//get the index and coordinates of nodes
		for(k = 0; k < 3; k++){
			n = Surf->pTrs.GetAt(j)->nID[k];  
			x[k] = Surf->pNds.GetAt(n)->x.floatValue(); 
			y[k] = Surf->pNds.GetAt(n)->y.floatValue();  
			z[k] = Surf->pNds.GetAt(n)->z.floatValue(); 
			x[k] = (x[k] - Crt->u) * Crt->Scale;	
			y[k] = (y[k] - Crt->v) * Crt->Scale;	
			z[k] = (z[k] - Crt->w) * Crt->Scale;
		}
		
		//calculate the normal
		fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
		fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
		fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
		CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
		glNormal3dv(fNormal);
		
		//draw trgl
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	    CreateRGBColor(RGB, j);
		glColor3ub(RGB[0], RGB[1], RGB[2]);

	//	glColor3ub(128, 42, 42);
		
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3ub(255, 255, 255);
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++)  glVertex3d(x[k], y[k], z[k]);//
		glEnd();
	}			
	
	glEndList();

#endif
	
}

// 2010.6.28 by Mei Gang , Freiburg, Germany
// create 6 lights
void CRealModelView::glMyLight(void)
{
	int i;
	// xy plane, along z axis
	//max z
	float light0_position[4];
	GLfloat position0[4];
	light0_position[0] = ((pCrt->minX + pCrt->maxX) / 2 - pCrt->u) * pCrt->Scale;
	light0_position[1] = ((pCrt->minY + pCrt->maxY) / 2 - pCrt->v) * pCrt->Scale;
	light0_position[2] = (pCrt->maxZ + (pCrt->maxZ - pCrt->minZ) * 0.618 - pCrt->w) * pCrt->Scale;
	light0_position[3] = 0.0;
	for(i = 0; i < 4; i++)  position0[i] = light0_position[i];
	//min z
	float light1_position[4];
	GLfloat position1[4];
	light1_position[0] = ((pCrt->minX + pCrt->maxX) / 2 - pCrt->u) * pCrt->Scale;
	light1_position[1] = ((pCrt->minY + pCrt->maxY) / 2 - pCrt->v) * pCrt->Scale;
	light1_position[2] = (pCrt->minZ - (pCrt->maxZ - pCrt->minZ) * 0.618 - pCrt->w) * pCrt->Scale;
	light1_position[3] = 0.0;
	for(i = 0; i < 4; i++)  position1[i] = light1_position[i];
	
	// yz plane, along x axis
	//max x
	float light2_position[4];
	GLfloat position2[4];
	light2_position[0] = (pCrt->maxX + (pCrt->maxX - pCrt->minX) * 0.618 - pCrt->u) * pCrt->Scale;
	light2_position[1] = ((pCrt->minY + pCrt->maxY) / 2 - pCrt->v) * pCrt->Scale;
	light2_position[2] = ((pCrt->minZ + pCrt->maxZ) / 2 - pCrt->w) * pCrt->Scale;
	light2_position[3] = 0.0;
	for(i = 0; i < 4; i++)  position2[i] = light2_position[i];
	//min x
	float light3_position[4];
	GLfloat position3[4];
	light3_position[0] = (pCrt->minX - (pCrt->maxX - pCrt->minX) * 0.618 - pCrt->u) * pCrt->Scale;
	light3_position[1] = ((pCrt->minY + pCrt->maxY) / 2 - pCrt->v) * pCrt->Scale;
	light3_position[2] = ((pCrt->minZ + pCrt->maxZ) / 2 - pCrt->w) * pCrt->Scale;
	light3_position[3] = 0.0;
	for(i = 0; i < 4; i++)  position3[i] = light3_position[i];
	
	// zx plane, along y axis
	//max y
	float light4_position[4];
	GLfloat position4[4];
	light4_position[0] = ((pCrt->minX + pCrt->maxX) / 2 - pCrt->u) * pCrt->Scale;
	light4_position[1] = (pCrt->maxY + (pCrt->maxY - pCrt->minY) * 0.618 - pCrt->v) * pCrt->Scale;
	light4_position[2] = ((pCrt->minZ + pCrt->maxZ) / 2 - pCrt->w) * pCrt->Scale;
	light4_position[3] = 0.0;
	for(i = 0; i < 4; i++)  position4[i] = light4_position[i];
	//min z
	float light5_position[4];
	GLfloat position5[4];
	light5_position[0] = ((pCrt->minX + pCrt->maxX) / 2 - pCrt->u) * pCrt->Scale;
	light5_position[1] = (pCrt->minY - (pCrt->maxY - pCrt->minY) * 0.618 - pCrt->v) * pCrt->Scale;
	light5_position[2] = ((pCrt->minZ + pCrt->maxZ) / 2 - pCrt->w) * pCrt->Scale;
	light5_position[3] = 0.0;
	for(i = 0; i < 4; i++)  position5[i] = light5_position[i];
	
	GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
	
	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat local_view[] = {0.0};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, position2);
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT3, GL_POSITION, position3);
	glLightfv(GL_LIGHT4, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT4, GL_POSITION, position4);
	glLightfv(GL_LIGHT5, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT5, GL_POSITION, position5);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	
	glFrontFace(GL_CW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	/*  be efficient--make a display list  */
}


void CRealModelView::On2DModelOpenSurface()
{
	// TODO: Add your command handler code here
	CSurf * pSurf = new CSurf();
//	ReadData(pRgn); // old format

#ifndef EGC
	ReadDataSurface(pSurf); // new format
#else
	ReadDataSurfaceEGC(pSurf); // for EGC
#endif

	m_aSurfs.Add(pSurf);

	pCrt = new CCordnt();
    pCrt = GetNewCordntSystem(pSurf);
	int nregion = m_aSurfs.GetSize();
	int * list = new int[nregion];
	for(int j = 0; j < nregion; j++){
		list[j] = j+1;
		
		CSurf * Surf = m_aSurfs.GetAt(j);
		CompileTinList(Surf, pCrt, list[j], 1);
	}
	m_bOpenGL = 1;
	Invalidate(TRUE);
	delete[] list;
}


void CRealModelView::On2DModelIntersect()
{
	// TODO: Add your command handler code here
	int i, j, k, id, ID;
	double x, y, z;
	CArray<CTrgl*, CTrgl*>pTrs;
	CTrgl * pTr0; CTrgl * pTr1;

	SearchIntrTrgls(m_aSurfs.GetAt(0), m_aSurfs.GetAt(1), myEps); // to search the intersected triangles for each triangle

	for(i = 0; i < 2; i++) {
		for(j = 0; j < m_aSurfs.GetAt(i)->pTrs.GetSize(); j++) {
			pTrs.Add( m_aSurfs.GetAt(i)->pTrs.GetAt(j) );
		}
	}

	// For Progress
	CProgressWnd wndProgress(this, _T("Computing Progress"), TRUE);
	bool bCancel = false;
	int myRange = m_aSurfs.GetAt(0)->pTrs.GetSize();
	wndProgress.SetRange(0, myRange);
	wndProgress.SetText(_T("This is a progress window...\n\n")
						_T("Computing is finished."));
	Sleep(1); // To show clearly, can be removed

 	for(i = 0; i < m_aSurfs.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
        
		CLoop * lp0 = new CLoop();
		for(k = 0; k < 3; k++) {
			x = pTr0->x[k];  y = pTr0->y[k];  z = pTr0->z[k];
			lp0->pNds.Add(new CNode(k, x, y, z));
		}
		x = pTr0->x[0];  y = pTr0->y[0];  z = pTr0->z[0];
		lp0->pNds.Add(new CNode(0, x, y, z));

		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CLoop * pIntr = new CLoop();
            
			CLoop * lp1 = new CLoop();
			for(k = 0; k < 3; k++) {
				x = pTr1->x[k];  y = pTr1->y[k];  z = pTr1->z[k];
				lp1->pNds.Add(new CNode(k, x, y, z));
			}
			x = pTr1->x[0];  y = pTr1->y[0];  z = pTr1->z[0];
			lp1->pNds.Add(new CNode(0, x, y, z));
			IntrOfPolygonAndPolygon(lp0, lp1, pIntr, myEps);
			
			m_aLpsIntr.Add(pIntr); // Save
			pTr0->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
			pTr1->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index

	
		}

	    // For Progress
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) {
			  AfxMessageBox(_T("Progress Cancelled"));
			  bCancel = true;
			  break;
		}
	}
	wndProgress.Hide();
	wndProgress.DestroyWindow();


	
	//emerge all the points and rename the IDs
	for(i = 0; i < m_aSurfs.GetSize(); i++) {  // original nodes of triangles
		for(j = 0; j < m_aSurfs.GetAt(i)->pNds.GetSize(); j++) {
			id = m_aSurfs.GetAt(i)->pNds.GetAt(j)->ID;
			x = m_aSurfs.GetAt(i)->pNds.GetAt(j)->x; 
			y = m_aSurfs.GetAt(i)->pNds.GetAt(j)->y; 
			z = m_aSurfs.GetAt(i)->pNds.GetAt(j)->z; 
			pNds.Add(new CNode(id, x, y, z));
		}
	}
	for(i = 0; i < m_aLpsIntr.GetSize(); i++) { // original nodes of intersections(loops)
		for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
			id = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->ID;
			x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;
			y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
			z = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->z;
			pNds.Add(new CNode(id, x, y, z));
		}
	}

	//sort the nodes
	int n, cnt_1, idx, count;

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; p[idx].y = pNds.GetAt(j)->y; p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}
	for(i = cnt_1 - 1; i >= 0; i--){
	    pNds.GetAt(i)->Clear(); 
		delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll();  
	//sort nodes
    SortNode(p, cnt_1);

	//re-arrange nodes
	int* iID = new int[cnt_1];	int* nID = new int[cnt_1];
	for(i = 0; i < cnt_1; i++)iID[i] = p[i].ID;

	nID[0] = 0; idx = 0;

	pNds.Add(new CNode(idx, p[0].x, p[0].y, p[0].z)); 
	
	//re-create nodes
	for(i = 1; i < cnt_1; i++){
		if(!IsSameNode(&p[i], &p[i-1], myEps ) ) {idx++; pNds.Add(new CNode(idx, p[i].x, p[i].y, p[i].z));}
		nID[i] = idx;
	}
	delete [] p;
	//sort id
	prhap(iID, nID, cnt_1); idx = 0;  count = 0;

	// renew all the IDs for points
	//copy new nodes for region
	n = 0; 
	for(i = 0; i < m_aSurfs.GetSize(); i++) { 
		for(j = 0; j < m_aSurfs.GetAt(i)->pNds.GetSize(); j++) {
			id = nID[n];
			m_aSurfs.GetAt(i)->pNds.GetAt(j)->ID = id;
			n++;
		}
	}
	//copy new nodes for intersections
	for(i = 0; i < m_aLpsIntr.GetSize(); i++) { 
		for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
			id = nID[n];
			m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->ID = id;
			n++;		
		}
	}

	delete[] iID;  delete[] nID;

	////////output data
	double xx,yy,zz;
	CString fnMesh,fnNode;
	int NumOfNds = pNds.GetSize();   
	int NumOfLps = m_aLpsIntr.GetSize();
	if( AfxMessageBox(_T("Save intersection in 2D ?"), MB_YESNOCANCEL ) != 6 ) return;
	CFileDialog Dlg(false, NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Data File(*.txt)|*.txt"), NULL);

	if( Dlg.DoModal() == IDOK ) 
	{ 
		fnMesh = Dlg.GetPathName();
		fnMesh += "-Mesh";
		if(fnMesh.Right(4) != ".txt")   fnMesh += ".txt";
		ofstream fin( fnMesh );
		for(i = 0; i < NumOfLps-1; i++){
			fin << i << "  " ;
			for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
				fin << m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->ID << "  ";
			}
			fin << endl;
		}
		fin << i << "  " ;
		for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
			fin << m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->ID << "  ";
		}
		fin.close(); 
		
		fnNode = Dlg.GetPathName();
		fnNode += "-Node";
		if( fnNode.Right(4) != ".txt")   fnNode += ".txt";
		ofstream fin3( fnNode );
		for(i = 0; i < NumOfNds - 1; i++){
			ID = pNds.GetAt(i)->ID;
			xx = pNds.GetAt(i)->x;  yy = pNds.GetAt(i)->y;  zz = pNds.GetAt(i)->z;
			fin3 << ID << "   " <<  xx << "   " <<  yy << "   " << zz << endl;
		}
		ID = pNds.GetAt(i)->ID;
		xx = pNds.GetAt(i)->x;  yy = pNds.GetAt(i)->y;  zz = pNds.GetAt(i)->z;
		fin3 << ID << "   " <<  xx << "   " << yy << "   " << zz;
		fin3.close(); 
	}

	////////Remove the memory
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
	    // pNds.GetAt(i)->Clear(); 
		delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 
	cnt_1 = pTrs.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
	    // pTrs.GetAt(i)->Clear(); 
		delete pTrs.GetAt(i); pTrs.RemoveAt(i);    
	}
	pTrs.RemoveAll(); 
	cnt_1 = m_aLpsIntr.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		// aLpsIntr.GetAt(i)->Clear(); 
		delete m_aLpsIntr.GetAt(i); m_aLpsIntr.RemoveAt(i);    
	}
	m_aLpsIntr.RemoveAll(); 

}


void CRealModelView::On2DModelOpenPolySurface()
{
	// TODO: Add your command handler code here
	pSurf = new CSurf(); //  must be a global varible
	ReadPolygonSurface(pSurf); 
	m_aSurfs.Add(pSurf);
	
	// Creat color
	for(int i = 0; i < m_aSurfs.GetSize(); i++) {
		m_aSurfs.GetAt(i)->ID = i;
		CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
		CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	}

	if(pCrt == NULL) {
	    pCrt = new CCordnt();
        pCrt = GetNewCordntSystem(pSurf);
	}

	int nSurf = m_aSurfs.GetSize();
	int * list = new int[nSurf];
	for(int j = 0; j < nSurf; j++){
		list[j] = j+1;
		pSurf = m_aSurfs.GetAt(j);
		glCompilePolygonSurfaceList(pSurf, pCrt, list[j], 1); // 1 : each element has different color
	}

	m_bOpenGL = 1;
	Invalidate(TRUE);
	delete[] list;

}


void CRealModelView::On2DModelMapping()
{
	// TODO: Add your command handler code here
	CBlock * pBlk = new CBlock();

	// Insect by mapping, has been merged ! Not need EmergeBlockGroupsBest();
	InsectOfPolySurf_2AndHalfDBest(m_aSurfs.GetAt(0), m_aSurfs.GetAt(1), pBlk);

	// display
	DisplayPolyhedraColor();

	// save
	SaveBlockGroups();

//	m_aSurfs.GetAt(1)->Clear();  delete m_aSurfs.GetAt(1);  m_aSurfs.RemoveAt(1);
//	m_aSurfs.GetAt(0)->Clear();  delete m_aSurfs.GetAt(0);  m_aSurfs.RemoveAt(0);
}

//Read Blocks groups: node, loops/areas, polyhedrons, block groups
void CRealModelView::ReadData()
{
	int i, j, k, t, ndata;
	int nNds, nLps, nPhds, nGroup, num;
	double x, y, z;
	CArray<int, int> nCount;
	int id_nds, id_lps, id_phds; // id_blks;
	double *xset, *yset, *zset;
	
	m_nObjectType = OBJECT_TYPE_POLYHEDRA;

	ndata = AfxMessageBox(_T("Read Block Groups ?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6) {
		CFileDialog Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"));
    	if(Dlg.DoModal() == IDOK){

			 // For Output
	         if(pFrame == NULL) {
	             pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	         }
	         CString str;
	         str = _T("Open Block Groups : ") + Dlg.GetPathName();
	         pFrame->AddStrToOutputWnd(1, str);

			// Dlg.GetPathName() not GetFileName();
			ifstream fin0(Dlg.GetPathName());
			fin0 >> nNds >> nLps >> nPhds >> nGroup; 
			
			//Nodes
			for(i = 0; i < nNds; i++) {
				fin0 >> id_nds >> x >> y >> z;
				m_aNds.Add(new CNode(id_nds, x, y, z));  // Numbering begins at 0
			}

			//Loops
			for(i = 0; i < nLps; i++) {
                fin0 >> num;
				nCount.Add( num );
			}
			for(i = 0; i < nLps; i++) {
				m_aLps.Add(new CLoop());
				fin0 >> id_lps;
				for(j = 0; j < nCount.GetAt(i); j++) {
					fin0 >> id_nds;
					m_aLps.GetAt(i)->iNds.Add(id_nds); // at present, only store the id of the node
				}
			}
			nCount.RemoveAll();

			//Polyhedrons
			for(i = 0; i < nPhds; i++) {
				fin0 >> num;
				nCount.Add( num );
			}
			for(i = 0; i < nPhds; i++) {
				m_aPhds.Add(new CPohd());
				fin0 >> id_phds;
				for(j = 0; j < nCount.GetAt(i); j++) {
					fin0 >> id_lps;
					m_aPhds.GetAt(i)->iLps.Add(id_lps); // at present, only store the id of the loop/area
				}
			}
			nCount.RemoveAll();

			//Block groups
			int iProperty, numPhd;
			for(i = 0; i < nGroup; i++) {
				CBlock * blk = new CBlock();
				fin0 >> iProperty >> numPhd;
				blk->iProperty = iProperty;
				for(j = 0; j < numPhd; j++) {
					fin0 >> id_phds;
					blk->iPhds.Add(id_phds);
					m_aPhds.GetAt(id_phds)->iProperty = iProperty;
					blk->pPhs.Add( m_aPhds.GetAt(id_phds) );
				}
				m_aBlks.Add( blk );
			}

			fin0.close();

            // Creat boundary box
			xset = new double[nNds];  yset = new double[nNds];  zset = new double[nNds];
			for(i = 0; i < nNds; i++) {
				xset[i] = m_aNds.GetAt(i)->x;
				yset[i] = m_aNds.GetAt(i)->y;
				zset[i] = m_aNds.GetAt(i)->z;
			}

			prhap(xset, nNds);
			minx = xset[0];  maxx = xset[nNds-1];
			prhap(yset, nNds);
			miny = yset[0];  maxy = yset[nNds-1];
			prhap(zset, nNds);
			minz = zset[0];  maxz = zset[nNds-1];

			delete []xset;  delete []yset;  delete []zset;

		}
	}
	else return;

	// save details into every polyhedron. 2010.11.5
	// everything is the nodes in loops
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = 0; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			CLoop * newLp = new CLoop();
			id_lps = m_aPhds.GetAt(i)->iLps.GetAt(j);
			for(k = 0; k < m_aLps.GetAt(id_lps)->iNds.GetSize(); k++) {
				id_nds = m_aLps.GetAt(id_lps)->iNds.GetAt(k);
				x = m_aNds.GetAt(id_nds)->x;
				y = m_aNds.GetAt(id_nds)->y;
				z = m_aNds.GetAt(id_nds)->z;
				newLp->pNds.Add(new CNode(k, x, y, z));
			}
			m_aPhds.GetAt(i)->pLps.Add(newLp);
		}
	}
	bool bexist;
	for(i = 0; i < m_aPhds.GetSize(); i++) { // emerge IDs of nodes
		for(j = 0; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			id_lps = m_aPhds.GetAt(i)->iLps.GetAt(j);
			for(k = 0; k < m_aLps.GetAt(id_lps)->iNds.GetSize(); k++) {
				id_nds = m_aLps.GetAt(id_lps)->iNds.GetAt(k);
				if(j == 0) {
					m_aPhds.GetAt(i)->iNds.Add(id_nds);
				}
				else {
					bexist = false;
					for(t = 0; t < m_aPhds.GetAt(i)->iNds.GetSize(); t++) {
						if(m_aPhds.GetAt(i)->iNds.GetAt(t) == id_nds) {
							bexist = true;  break;
						}
					}
					if(bexist == false) {
						m_aPhds.GetAt(i)->iNds.Add(id_nds);
					}

				}
			}
		}
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) { // record the pointer of nodes
		for(j = 0; j < m_aPhds.GetAt(i)->iNds.GetSize(); j++) {
			id_nds = m_aPhds.GetAt(i)->iNds.GetAt(j);
			m_aPhds.GetAt(i)->pNds.Add(m_aNds.GetAt(id_nds));
		}
	}

	// color
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CreateRGBColor(m_aBlks.GetAt(i)->RGB, i + m_aSurfs.GetSize());
	}

}

//Read Blocks groups: node, loops/areas, polyhedrons, block groups
void CRealModelView::ReadModel(CModel * model)
{
	int i, j, k, t, ndata;
	int nNds, nLps, nPhds, nGroup, num;
	double x, y, z;
	CArray<int, int> nCount;
	int id_nds, id_lps, id_phds; // id_blks;
	double *xset, *yset, *zset;
	
	m_nObjectType = OBJECT_TYPE_POLYHEDRA;

	ndata = AfxMessageBox(_T("Read a Model ?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6) {
		CFileDialog Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"));
    	if(Dlg.DoModal() == IDOK){

			 // For Output
	         if(pFrame == NULL) {
	             pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	         }
	         CString str;
	         str = _T("Open Block Groups : ") + Dlg.GetPathName();
	         pFrame->AddStrToOutputWnd(1, str);

			// Dlg.GetPathName() not GetFileName();
			ifstream fin0(Dlg.GetPathName());
			fin0 >> nNds >> nLps >> nPhds >> nGroup; 
			
			//Nodes
			for(i = 0; i < nNds; i++) {
				fin0 >> id_nds >> x >> y >> z;
				model->m_aNds.Add(new CNode(id_nds, x, y, z));  // Numbering begins at 0
			}

			//Loops
			for(i = 0; i < nLps; i++) {
                fin0 >> num;
				nCount.Add( num );
			}
			for(i = 0; i < nLps; i++) {
				model->m_aLps.Add(new CLoop());
				fin0 >> id_lps;
				for(j = 0; j < nCount.GetAt(i); j++) {
					fin0 >> id_nds;
					model->m_aLps.GetAt(i)->iNds.Add(id_nds); // at present, only store the id of the node
				}
			}
			nCount.RemoveAll();

			//Polyhedrons
			for(i = 0; i < nPhds; i++) {
				fin0 >> num;
				nCount.Add( num );
			}
			for(i = 0; i < nPhds; i++) {
				model->m_aPhds.Add(new CPohd());
				fin0 >> id_phds;
				for(j = 0; j < nCount.GetAt(i); j++) {
					fin0 >> id_lps;
					model->m_aPhds.GetAt(i)->iLps.Add(id_lps); // at present, only store the id of the loop/area
				}
			}
			nCount.RemoveAll();

			//Block groups
			int iProperty, numPhd;
			for(i = 0; i < nGroup; i++) {
				CBlock * blk = new CBlock();
				fin0 >> iProperty >> numPhd;
				blk->iProperty = iProperty;
				for(j = 0; j < numPhd; j++) {
					fin0 >> id_phds;
					blk->iPhds.Add(id_phds);
					model->m_aPhds.GetAt(id_phds)->iProperty = iProperty;
					blk->pPhs.Add( model->m_aPhds.GetAt(id_phds) );
				}
				model->m_aBlks.Add( blk );
			}

			fin0.close();

            // Creat boundary box
			xset = new double[nNds];  yset = new double[nNds];  zset = new double[nNds];
			for(i = 0; i < nNds; i++) {
				xset[i] = model->m_aNds.GetAt(i)->x;
				yset[i] = model->m_aNds.GetAt(i)->y;
				zset[i] = model->m_aNds.GetAt(i)->z;
			}

			prhap(xset, nNds);
			minx = xset[0];  maxx = xset[nNds-1];
			prhap(yset, nNds);
			miny = yset[0];  maxy = yset[nNds-1];
			prhap(zset, nNds);
			minz = zset[0];  maxz = zset[nNds-1];

			delete []xset;  delete []yset;  delete []zset;

		}
	}
	else return;

	// save details into every polyhedron. 2010.11.5
	// everything is the nodes in loops
	for(i = 0; i < model->m_aPhds.GetSize(); i++) {
		for(j = 0; j < model->m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			CLoop * newLp = new CLoop();
			id_lps = model->m_aPhds.GetAt(i)->iLps.GetAt(j);
			for(k = 0; k < model->m_aLps.GetAt(id_lps)->iNds.GetSize(); k++) {
				id_nds = model->m_aLps.GetAt(id_lps)->iNds.GetAt(k);
				x = model->m_aNds.GetAt(id_nds)->x;
				y = model->m_aNds.GetAt(id_nds)->y;
				z = model->m_aNds.GetAt(id_nds)->z;
				newLp->pNds.Add(new CNode(k, x, y, z));
			}
			model->m_aPhds.GetAt(i)->pLps.Add(newLp);
		}
	}
	bool bexist;
	for(i = 0; i < model->m_aPhds.GetSize(); i++) { // emerge IDs of nodes
		for(j = 0; j < model->m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			id_lps = model->m_aPhds.GetAt(i)->iLps.GetAt(j);
			for(k = 0; k < model->m_aLps.GetAt(id_lps)->iNds.GetSize(); k++) {
				id_nds = model->m_aLps.GetAt(id_lps)->iNds.GetAt(k);
				if(j == 0) {
					model->m_aPhds.GetAt(i)->iNds.Add(id_nds);
				}
				else {
					bexist = false;
					for(t = 0; t < model->m_aPhds.GetAt(i)->iNds.GetSize(); t++) {
						if(model->m_aPhds.GetAt(i)->iNds.GetAt(t) == id_nds) {
							bexist = true;  break;
						}
					}
					if(bexist == false) {
						model->m_aPhds.GetAt(i)->iNds.Add(id_nds);
					}

				}
			}
		}
	}
	for(i = 0; i < model->m_aPhds.GetSize(); i++) { // record the pointer of nodes
		for(j = 0; j < model->m_aPhds.GetAt(i)->iNds.GetSize(); j++) {
			id_nds = model->m_aPhds.GetAt(i)->iNds.GetAt(j);
			model->m_aPhds.GetAt(i)->pNds.Add(model->m_aNds.GetAt(id_nds));
		}
	}

	// color
	for(i = 0; i < model->m_aBlks.GetSize(); i++) {
		CreateRGBColor(model->m_aBlks.GetAt(i)->RGB, i + model->m_aSurfs.GetSize());
	}

}

CModel* CRealModelView::EmergeModel(CModel * ma, CModel * mb)
{
	CModel * all = new CModel();

	int i, j, k, t, id, N, index;
	bool bexist = false;
	CArray<int, int> tmp;

	// Step 1: Node
	// From: ma
	for(i = 0; i < ma->m_aNds.GetSize(); i++) {
		all->m_aNds.Add(ma->m_aNds.GetAt(i));
	}

	//// From: mb
	N = mb->m_aLps.GetSize();
	int * newIDs = new int[N];
	for(i = 0; i < mb->m_aNds.GetSize(); i++) {
		bexist = false;
		for(j = 0; j < all->m_aNds.GetSize(); j++) {
			if(IsSameNode(mb->m_aNds.GetAt(i), all->m_aNds.GetAt(j), myEps) == true) {
				bexist = true;  index = j;  break;
			}
		}

		if(bexist == true) {
			newIDs[i] = index;
		}
		else {
			all->m_aNds.Add(mb->m_aNds.GetAt(i));
			index = all->m_aNds.GetUpperBound();
			newIDs[i] = index;
		}
	}

	// Step 2: Loop
	//// Update from mb
	for(i = 0; i < mb->m_aLps.GetSize(); i++) {
		tmp.RemoveAll();
		for(j = 0; j < mb->m_aLps.GetAt(i)->iNds.GetSize(); j++) {
			id = mb->m_aLps.GetAt(i)->iNds.GetAt(j);
			tmp.Add(newIDs[id]); // Save
		}
		mb->m_aLps.GetAt(i)->iNds.RemoveAll();
		for(j = 0; j < tmp.GetSize(); j++) {
			id = tmp.GetAt(j);
			mb->m_aLps.GetAt(i)->iNds.Add(id); // Update
		}
	}
	delete [] newIDs;
	//// Add loops from ma
	for(i = 0; i < ma->m_aLps.GetSize(); i++) {
		all->m_aLps.Add(ma->m_aLps.GetAt(i));
	}
	//// Add loops from mb

	N = mb->m_aLps.GetSize();
	newIDs = new int[N];

	//// Updating before adding
	for(i = 0; i < mb->m_aLps.GetSize(); i++) {
		bexist = false;
		for(j = 0; j < all->m_aLps.GetSize(); j++) {
			if(IsTwoLoopSameByID(mb->m_aLps.GetAt(i), all->m_aLps.GetAt(j)) == true) {
				bexist = true;  index = j;  break;
			}
		}

		if(bexist == true) {
			newIDs[i] = index;
		}
		else {
			all->m_aLps.Add(mb->m_aLps.GetAt(i));
			index = all->m_aLps.GetUpperBound();
			newIDs[i] = index;
		}
	}
	//// Add after updating
	for(i = 0; i < mb->m_aLps.GetSize(); i++) {
		all->m_aLps.Add(mb->m_aLps.GetAt(i));
	}

	// Step 3: Polyhedron
	//// Update from mb
	for(i = 0; i < mb->m_aPhds.GetSize(); i++) {
		tmp.RemoveAll();
		for(j = 0; j < mb->m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			id = mb->m_aPhds.GetAt(i)->iLps.GetAt(j);
			tmp.Add(newIDs[id]);
		}
		mb->m_aPhds.GetAt(i)->iLps.RemoveAll();
		for(j = 0; j < tmp.GetSize(); j++) {
			mb->m_aPhds.GetAt(i)->iLps.Add(tmp.GetAt(j));
		}
	}
	//// Add from ma
	for(i = 0; i < ma->m_aPhds.GetSize(); i++) {
		all->m_aPhds.Add(ma->m_aPhds.GetAt(i));
	}
	//// Add from mb
	for(i = 0; i < mb->m_aPhds.GetSize(); i++) {
		all->m_aPhds.Add(mb->m_aPhds.GetAt(i));
	}

	// Step 4: Block
	//// Update from mb
	int add = ma->m_aPhds.GetSize();
	for(i = 0; i < mb->m_aBlks.GetSize(); i++) {
		for(j = 0; j < mb->m_aBlks.GetAt(i)->iPhds.GetSize(); j++) {
			mb->m_aBlks.GetAt(i)->iPhds.GetAt(j) += add;
		}
	}
	//// Add from ma
	for(i = 0; i < ma->m_aBlks.GetSize(); i++) {
		all->m_aBlks.Add(ma->m_aBlks.GetAt(i));
	}
	//// Add from mb
	for(i = 0; i < mb->m_aBlks.GetSize(); i++) {
		all->m_aBlks.Add(mb->m_aBlks.GetAt(i));
	}

	return all;
}

void CRealModelView::ReadDataProperty()
{
	int i, j, k, t, ndata;
	int nNds, nLps, nPhds, nGroup, nProp, num;
	double x, y, z;
	CArray<int, int> nCount;
	int ID, id_nds, id_lps, id_phds; // id_blks;
	double *xset, *yset, *zset;
	
	m_nObjectType = OBJECT_TYPE_POLYHEDRA;

	ndata = AfxMessageBox(_T("Read Block Groups ?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6) {
		CFileDialog Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"));
    	if(Dlg.DoModal() == IDOK){

			 // For Output
	         if(pFrame == NULL) {
	             pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	         }
	         CString str;
	         str = _T("Open Block Groups : ") + Dlg.GetPathName();
	         pFrame->AddStrToOutputWnd(1, str);

			// Dlg.GetPathName() not GetFileName();
			ifstream fin0(Dlg.GetPathName());
			fin0 >> nNds >> nLps >> nPhds >> nGroup; 
			fin0 >> nProp; // Number of properties

			//Nodes
			for(i = 0; i < nNds; i++) {
				fin0 >> id_nds >> x >> y >> z;
				m_aNds.Add(new CNode(id_nds, x, y, z));  // Numbering begins at 0
			}

			//Loops
			for(i = 0; i < nLps; i++) {
                fin0 >> num;
				nCount.Add( num );
			}
			for(i = 0; i < nLps; i++) {
				m_aLps.Add(new CLoop());
				fin0 >> id_lps;
				for(j = 0; j < nCount.GetAt(i); j++) {
					fin0 >> id_nds;
					m_aLps.GetAt(i)->iNds.Add(id_nds); // at present, only store the id of the node
				}
			}
			nCount.RemoveAll();

			//Polyhedrons
			for(i = 0; i < nPhds; i++) {
				fin0 >> num;
				nCount.Add( num );
			}
			for(i = 0; i < nPhds; i++) {
				m_aPhds.Add(new CPohd());
				fin0 >> id_phds;
				for(j = 0; j < nCount.GetAt(i); j++) {
					fin0 >> id_lps;
					m_aPhds.GetAt(i)->iLps.Add(id_lps); // at present, only store the id of the loop/area
				}
			}
			nCount.RemoveAll();

			//Block groups
			int iProperty, numPhd;
			for(i = 0; i < nGroup; i++) {
				CBlock * blk = new CBlock();
				fin0 >> iProperty >> numPhd;
				blk->iProperty = iProperty;
				for(j = 0; j < numPhd; j++) {
					fin0 >> id_phds;
					blk->iPhds.Add(id_phds);
					m_aPhds.GetAt(id_phds)->iProperty = iProperty;
					blk->pPhs.Add( m_aPhds.GetAt(id_phds) );
				}
				m_aBlks.Add( blk );
			}

			// List of Properties
	        CString Name;
	        int Age;
	        CString Statement;
	        int RGB[3]; // Color by RGB. 2010.12.5
	        string sName;
	        string sMaterial;

	        for(i = 0; i < nProp; i++) {
			    fin0 >> ID >> sName >> Age >> sMaterial >> RGB[0] >> RGB[1] >> RGB[2]; // Read without Statement
				// Statement
				string tStr;
				fin0 >> tStr;
				str = tStr.c_str();

			    while(tStr.size() != 0) {
				   tStr.clear();
				   fin0>> tStr;
				   if(tStr == "//END") { 
					   str += _T(" //END");
					   break;
				   }
				   else {
					  str += _T(" ");
					  str += tStr.c_str();
				   }
			    }

				// New and add into global
				CProp* pNewProp = new CProp();
				pNewProp->ID = ID;
				pNewProp->Name = sName.c_str();
				pNewProp->Age = Age;
				pNewProp->Material = sMaterial.c_str();
				pNewProp->RGB[0] = RGB[0];
				pNewProp->RGB[1] = RGB[1];
				pNewProp->RGB[2] = RGB[2];
				pNewProp->Statement = str; // Special one

				m_aProps.Add(pNewProp);
			}

			// Set Block's Property
			for(i = 0; i < nGroup; i++) {
				fin0 >> ID >> iProperty;
				m_aBlks.GetAt(i)->iProperty = iProperty;
			}

			fin0.close(); // Close

            // Creat boundary box
			xset = new double[nNds];  yset = new double[nNds];  zset = new double[nNds];
			for(i = 0; i < nNds; i++) {
				xset[i] = m_aNds.GetAt(i)->x;
				yset[i] = m_aNds.GetAt(i)->y;
				zset[i] = m_aNds.GetAt(i)->z;
			}

			prhap(xset, nNds);
			minx = xset[0];  maxx = xset[nNds-1];
			prhap(yset, nNds);
			miny = yset[0];  maxy = yset[nNds-1];
			prhap(zset, nNds);
			minz = zset[0];  maxz = zset[nNds-1];

			delete []xset;  delete []yset;  delete []zset;

		}
	}
	else return;

	// save details into every polyhedron. 2010.11.5
	// everything is the nodes in loops
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = 0; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			CLoop * newLp = new CLoop();
			id_lps = m_aPhds.GetAt(i)->iLps.GetAt(j);
			for(k = 0; k < m_aLps.GetAt(id_lps)->iNds.GetSize(); k++) {
				id_nds = m_aLps.GetAt(id_lps)->iNds.GetAt(k);
				x = m_aNds.GetAt(id_nds)->x;
				y = m_aNds.GetAt(id_nds)->y;
				z = m_aNds.GetAt(id_nds)->z;
				newLp->pNds.Add(new CNode(k, x, y, z));
			}
			m_aPhds.GetAt(i)->pLps.Add(newLp);
		}
	}
	bool bexist;
	for(i = 0; i < m_aPhds.GetSize(); i++) { // emerge IDs of nodes
		for(j = 0; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			id_lps = m_aPhds.GetAt(i)->iLps.GetAt(j);
			for(k = 0; k < m_aLps.GetAt(id_lps)->iNds.GetSize(); k++) {
				id_nds = m_aLps.GetAt(id_lps)->iNds.GetAt(k);
				if(j == 0) {
					m_aPhds.GetAt(i)->iNds.Add(id_nds);
				}
				else {
					bexist = false;
					for(t = 0; t < m_aPhds.GetAt(i)->iNds.GetSize(); t++) {
						if(m_aPhds.GetAt(i)->iNds.GetAt(t) == id_nds) {
							bexist = true;  break;
						}
					}
					if(bexist == false) {
						m_aPhds.GetAt(i)->iNds.Add(id_nds);
					}

				}
			}
		}
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) { // record the pointer of nodes
		for(j = 0; j < m_aPhds.GetAt(i)->iNds.GetSize(); j++) {
			id_nds = m_aPhds.GetAt(i)->iNds.GetAt(j);
			m_aPhds.GetAt(i)->pNds.Add(m_aNds.GetAt(id_nds));
		}
	}

	// color
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CreateRGBColor(m_aBlks.GetAt(i)->RGB, i + m_aSurfs.GetSize());
	}

}


// 2012.1.10 by Mei Gang at University of Freiburg, Germany.
void CRealModelView::OpenProperty()
{
	// Open Property
	int i, j;
	CString fn;
	int NumOfProps;

	int ID;
	CString Name;
	int Age;
	CString Statement;
	int RGB[3]; // Color by RGB. 2010.12.5
	string sName;
	string sMaterial;

	int ndata = AfxMessageBox(_T("Read Properties ?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6) {
		CFileDialog Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"));
    	if(Dlg.DoModal() == IDOK){

			 // For Output
	         if(pFrame == NULL) {
	             pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	         }
	         CString str;
	         str = _T("Open Properties : ") + Dlg.GetPathName();
	         pFrame->AddStrToOutputWnd(1, str);

			// Dlg.GetPathName() not GetFileName();
			ifstream fin0(Dlg.GetPathName());
			
			fin0 >> NumOfProps; // Number of properties
			for(i = 0; i < NumOfProps; i++) {
				fin0 >> ID >> sName >> Age >> sMaterial >> RGB[0] >> RGB[1] >> RGB[2]; // Read without Statement

				// Statement
				string tStr;
				fin0 >> tStr;
				str = tStr.c_str();

			    while(tStr.size() != 0) {
				   tStr.clear();
				   fin0>> tStr;
				
				   if(tStr == "//END") { 
					   str += _T(" //END");
					   break;
				   }
				   else {
					  str += _T(" ");
					  str += tStr.c_str();
				   }
			    }
			    // MessageBox(str);

				// New and add into global
				CProp* pNewProp = new CProp();
				pNewProp->ID = ID;
				pNewProp->Name = sName.c_str();
				pNewProp->Age = Age;
				pNewProp->Material = sMaterial.c_str();
				pNewProp->RGB[0] = RGB[0];
				pNewProp->RGB[1] = RGB[1];
				pNewProp->RGB[2] = RGB[2];
				pNewProp->Statement = str; // Special one

				m_aProps.Add(pNewProp);

			}
			fin0.close();
		}
	}
	else return;

}

// 2012.1.9 by Mei Gang at University of Freiburg, Germany.
void CRealModelView::SaveProperty()
{
	// Save Property
	int i, j;
	CString fn;
	int NumOfProps = m_aProps.GetSize();
	if(NumOfProps == 0) {
		AfxMessageBox(_T("No Property ! The List of Properties is Empty !"));
		return;
	}

	int ID;
	CString Name;
	int Age;
	CString Material;
	CString Statement;
	int RGB[3]; // Color by RGB. 2010.12.5
	string sName;
	string sMaterial;
	string sStatement;
	CStringW strw;
	CStringA stra;

	if(AfxMessageBox(_T("Saving Properties ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);
		fin << NumOfProps << endl;
		for(i = 0; i < NumOfProps; i++) { 
			ID = m_aProps.GetAt(i)->ID;
			Name = m_aProps.GetAt(i)->Name;
			Age = m_aProps.GetAt(i)->Age;
			Material = m_aProps.GetAt(i)->Material;
			Statement = m_aProps.GetAt(i)->Statement;
			for(j = 0; j < 3; j++) {
				RGB[j] = m_aProps.GetAt(i)->RGB[j];
			}

			strw = Name;
            stra = strw.GetBuffer(0);  strw.ReleaseBuffer();
            sName = stra.GetBuffer(0);  stra.ReleaseBuffer();

			strw = Material;
            stra = strw.GetBuffer(0);  strw.ReleaseBuffer();
            sMaterial = stra.GetBuffer(0);  stra.ReleaseBuffer();

			strw = Statement;
            stra = strw.GetBuffer(0);  strw.ReleaseBuffer();
            sStatement = stra.GetBuffer(0);  stra.ReleaseBuffer();
             
	    	fin << ID <<"   "<<  sName <<"   "<< Age <<"   "<< sMaterial << "   "
				<<RGB[0] <<" " << RGB[1]<<" "<< RGB[2]<<"  "<< sStatement << endl;
		}
		fin.close();
	}

}

void CRealModelView::CreatBoundaryBox(CTrgl *pTrgl, CNode *leftBottom, CNode *topRight)
{
	double x[3], y[3], z[3];
	double xmin, ymin, zmin, xmax, ymax, zmax;

	for(int i = 0; i < 3; i++) {
		x[i] = pTrgl->x[i];  y[i] = pTrgl->y[i];  z[i] = pTrgl->z[i];
	}

	prhap(x, 3);
	xmin = x[0]; xmax = x[2];
	prhap(y, 3);
	ymin = y[0]; ymax = y[2];
	prhap(z, 3);
	zmin = z[0]; zmax = z[2];

	leftBottom->x = xmin;
	leftBottom->y = ymin;
	leftBottom->z = zmin;

	topRight->x = xmax;
	topRight->y = ymax;
	topRight->z = zmax;

}

void CRealModelView::CreatBoundaryBox(CLoop *pLp, CNode *leftBottom, CNode *topRight)
{
	int N = pLp->pNds.GetSize();
	if(N < 3)  {
		AfxMessageBox(_T("Error: CreatBoundaryBox(CLoop* pLp)"));
		PostQuitMessage(0);
	}
	double *x = new double[N], *y = new double[N], *z = new double[N];
	double xmin, ymin, zmin, xmax, ymax, zmax;

	for(int i = 0; i < N; i++) {
		x[i] = pLp->pNds.GetAt(i)->x;
		y[i] = pLp->pNds.GetAt(i)->y;
		z[i] = pLp->pNds.GetAt(i)->z;
	}

	prhap(x, N);
	xmin = x[0];  xmax = x[N-1];
	prhap(y, N);
	ymin = y[0];  ymax = y[N-1];
	prhap(z, N);
	zmin = z[0];  zmax = z[N-1];

	leftBottom->x = xmin;
	leftBottom->y = ymin;
	leftBottom->z = zmin;

	topRight->x = xmax;
	topRight->y = ymax;
	topRight->z = zmax;
	delete [] x;  delete [] y;  delete [] z;

}

void CRealModelView::CreatBoundaryBox(CPohd * pPhd, CNode *leftBottom, CNode *topRight)
{
	int N = pPhd->pNds.GetSize();
	double *x = new double[N], *y = new double[N], *z = new double[N];
	double xmin, ymin, zmin, xmax, ymax, zmax;

	for(int i = 0; i < N; i++) {
		x[i] = pPhd->pNds.GetAt(i)->x;
		y[i] = pPhd->pNds.GetAt(i)->y;
		z[i] = pPhd->pNds.GetAt(i)->z;
	}

	prhap(x, N);
	xmin = x[0];  xmax = x[N-1];
	prhap(y, N);
	ymin = y[0];  ymax = y[N-1];
	prhap(z, N);
	zmin = z[0];  zmax = z[N-1];

	leftBottom->x = xmin;
	leftBottom->y = ymin;
	leftBottom->z = zmin;

	topRight->x = xmax;
	topRight->y = ymax;
	topRight->z = zmax;
	delete [] x;  delete [] y;  delete [] z;

}

void CRealModelView::CreatBoundaryBox(CSurf * pSurf, CNode *leftBottom, CNode *topRight)
{
	int N = pSurf->pNds.GetSize();
	double *x = new double[N], *y = new double[N], *z = new double[N];
	double xmin, ymin, zmin, xmax, ymax, zmax;

	for(int i = 0; i < N; i++) {
		x[i] = pSurf->pNds.GetAt(i)->x;
		y[i] = pSurf->pNds.GetAt(i)->y;
		z[i] = pSurf->pNds.GetAt(i)->z;
	}

	prhap(x, N);
	xmin = x[0];  xmax = x[N-1];
	prhap(y, N);
	ymin = y[0];  ymax = y[N-1];
	prhap(z, N);
	zmin = z[0];  zmax = z[N-1];

	leftBottom->x = xmin;
	leftBottom->y = ymin;
	leftBottom->z = zmin;

	topRight->x = xmax;
	topRight->y = ymax;
	topRight->z = zmax;
	delete [] x;  delete [] y;  delete [] z;

}


void CRealModelView::CreatBoundaryBox(CNode *leftBottom, CNode *topRight)
{
	int N = m_aNds.GetSize();
	double *x = new double[N], *y = new double[N], *z = new double[N];
	double xmin, ymin, zmin, xmax, ymax, zmax;

	for(int i = 0; i < N; i++) {
		x[i] = m_aNds.GetAt(i)->x;
		y[i] = m_aNds.GetAt(i)->y;
		z[i] = m_aNds.GetAt(i)->z;
	}

	prhap(x, N);
	xmin = x[0];  xmax = x[N-1];
	prhap(y, N);
	ymin = y[0];  ymax = y[N-1];
	prhap(z, N);
	zmin = z[0];  zmax = z[N-1];

	leftBottom->x = xmin;
	leftBottom->y = ymin;
	leftBottom->z = zmin;

	topRight->x = xmax;
	topRight->y = ymax;
	topRight->z = zmax;

	delete [] x;  delete [] y;  delete [] z;

}

bool CRealModelView::BoundaryBoxIntrTest(CNode *pLB0, CNode *pRT0, CNode *pLB1, CNode *pRT1)
{
	if(pLB0->x > pRT1->x || pLB0->y > pRT1->y || pLB0->z > pRT1->z) {
		return false;
	}
	if(pLB1->x > pRT0->x || pLB1->y > pRT0->y || pLB1->z > pRT0->z) {
		return false;
	}

	return true;

}


void CRealModelView::OnDisplayBlockGroup()
{
	ReadData();
	m_iPhdNumBefAdd = m_aPhds.GetSize(); // original number before adding a new surface, important.
	m_iBlkNumBefAdd = m_aBlks.GetSize(); // 

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}

	GLuint allBlockGroups;
	allBlockGroups = 1 + m_aSurfs.GetSize();
	glCompileBlockGroupListBest_Boundary(pCrt, allBlockGroups);  

	m_bOpenGL = 1;
	Invalidate(TRUE);
	
}

void CRealModelView::On3DModelOpenBlockGroup()
{
	ReadData();
	m_iPhdNumBefAdd = m_aPhds.GetSize(); // original number before adding a new surface, important.
	m_iBlkNumBefAdd = m_aBlks.GetSize(); // 

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}

	GLuint allBlockGroups;
//	allBlockGroups = glGenLists(1); 
	allBlockGroups = 1 + m_aSurfs.GetSize();
//	glCompileBlockGroupList(pCrt, allBlockGroups, 0);  // new rendering function
	glCompileBlockGroupListBest(pCrt, allBlockGroups);  
//	glCompileBlockGroupListWithArray(pCrt, allBlockGroups, 0);  // new rendering function

	m_bOpenGL = 1;
	Invalidate(TRUE);

}

// Open Block groups including properties
void CRealModelView::On3DModelOpenBlockGroupProp()
{
	ReadDataProperty();
	m_iPhdNumBefAdd = m_aPhds.GetSize(); // original number before adding a new surface, important.
	m_iBlkNumBefAdd = m_aBlks.GetSize(); // 

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}

	GLuint allBlockGroups;
//	allBlockGroups = glGenLists(1); 
	allBlockGroups = 1 + m_aSurfs.GetSize();
	glCompileBlockGroupList(pCrt, allBlockGroups, 0);  // new rendering function

	m_bOpenGL = 1;
	Invalidate(TRUE);
}

void CRealModelView::On3DModelAddSurface()
{
	pSurf = new CSurf(); //  must be a global varible
	ReadDataSurface(pSurf);  // new file format
	m_aSurfs.Add(pSurf);
	
	// Creat color
	for(int i = 0; i < m_aSurfs.GetSize(); i++) {
		m_aSurfs.GetAt(i)->ID = i;
		CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+1+m_aBlks.GetSize() );
	}
	pCrt = new CCordnt();
    pCrt = GetNewCordntSystem(pSurf);
	int nSurface = m_aSurfs.GetSize();
	int * list = new int[nSurface];
	for(int j = 0; j < nSurface; j++){
		list[j] = j+1;
		pSurf = m_aSurfs.GetAt(j);
		glCompileTinList(pSurf, pCrt, list[j], 0); // edge and elements, new
	}

	// Also display blocks
	GLuint allBlockGroups;
//	allBlockGroups = glGenLists(1); 
	allBlockGroups = 1 + m_aSurfs.GetSize();
	glCompileBlockGroupList(pCrt, allBlockGroups, 0);  // new rendering function

	m_bOpenGL = 1;
	Invalidate(TRUE);
	delete[] list;
}

// 2010.10.25 by MeiGang
// creat the top surface via all polyhedrons
void CRealModelView::CreatTopSurface(CSurf* topSurf)
{
	int i, j, k, t, n;
	double cx, cy, cz;
	double x[3], y[3], z[3], para[4];
	bool bFind;

	// creat all boundary areas
	CArray<CLoop*, CLoop*> boudry; // all boundary areas
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		for(j = 0; j < m_aPhds.GetAt(i)->pLps.GetSize(); j++) {
			boudry.Add(m_aPhds.GetAt(i)->pLps.GetAt(j));
		}
	}
	for(i = 0; i < boudry.GetSize()-1; i++) {
		for(j = i+1; j < boudry.GetSize(); j++) {
			if(IsTwoLoopSame(boudry.GetAt(i), boudry.GetAt(j)) == true) {
				boudry.GetAt(i)->UseTime = 2;
				boudry.GetAt(j)->UseTime = 2;
				break;
			}
		}
	}
	for(i = boudry.GetSize() - 1; i > -1; i--) {
		if(boudry.GetAt(i)->UseTime == 2) {
			boudry.RemoveAt(i);
		}
	}

	// creat the top surface
	//// flag = 0 : loop is not identified
	//// flag = 1 : loop is on top surface
	//// flag = -1: loop is on botton surface
	for(i = 0; i < boudry.GetSize(); i++) {
		boudry.GetAt(i)->flag = 0;
	}
	for(i = 0; i < boudry.GetSize()-1; i++) {
		if(boudry.GetAt(i)->flag != 0) {
			continue;
		}
		// 重心
		cx = 0.0;  cy = 0.0;  cz = 0.0;
		n = boudry.GetAt(i)->pNds.GetSize();
		if(IsSameNode(boudry.GetAt(i)->pNds.GetAt(0), boudry.GetAt(i)->pNds.GetAt(n-1), myEps))
		{
			--n; // the first and the last nodes are the same
		}
		for(t = 0; t < n; t++) {
			cx += boudry.GetAt(i)->pNds.GetAt(t)->x;
			cy += boudry.GetAt(i)->pNds.GetAt(t)->y;
			cz += boudry.GetAt(i)->pNds.GetAt(t)->z;
		}
		cx /= n;  cy /= n;  cz /= n;
		CNode * nd = new CNode(i, cx, cy, cz);

		for(j = i+1; j < boudry.GetSize(); j++) {
			if(IsNdInPolygon(nd, boudry.GetAt(j), myEps) == false ) {
				continue;
			}
           
			for(t = 0; t < 3; t++) {
				x[t] = boudry.GetAt(j)->pNds.GetAt(t)->x;
				y[t] = boudry.GetAt(j)->pNds.GetAt(t)->y;
				z[t] = boudry.GetAt(j)->pNds.GetAt(t)->z;
			}
			EquaOfTrgl(x, y, z, para);
			double newz = (0.0 - para[0]*cx - para[1]*cy - para[3]) / para[2];
			bFind = false;
			if(cz > newz + 0.00001) { // 比较同一映射点的Z坐标
				boudry.GetAt(i)->flag = 1;  boudry.GetAt(j)->flag = -1;
				bFind = true;
			}
			else if(cz < newz - 0.00001){
				boudry.GetAt(i)->flag = -1;  boudry.GetAt(j)->flag = 1;
				bFind = true;
			}

			if(bFind == true) {
				break;
			}
		}
		delete nd;
	}

	for(i = 0; i < boudry.GetSize(); i++) {
		if(boudry.GetAt(i)->flag == 1) {
			CLoop * lp = new CLoop();
			for(j = 0; j < boudry.GetAt(i)->pNds.GetSize(); j++) {
				cx = boudry.GetAt(i)->pNds.GetAt(j)->x;
				cy = boudry.GetAt(i)->pNds.GetAt(j)->y;
				cz = boudry.GetAt(i)->pNds.GetAt(j)->z;
				lp->pNds.Add(new CNode(j, cx, cy, cz));
			}
			topSurf->pLps.Add(lp);
		}
	}

	EmergePolygonSurface(topSurf); // Merge

}

// 2013.04.24 by MeiGang
// creat the top surface via all polyhedrons
void CRealModelView::CreatTopSurfaceBest(CSurf* topSurf)
{
	// For Output
	if(pFrame == NULL) {
	     pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	}
	CString str;
	str = _T("Start to Creat Top Surface... ");
	pFrame->AddStrToOutputWnd(1, str);

	int i, j, k, t, n, m, id;
	double cx, cy, cz, tx, ty, tz;
	double x[3], y[3], z[3], para[4];
	bool bFind, bExist;

	// Step 1: Creat all boundary faces, Only used once
	CArray<CLoop*, CLoop*> boudry; // all boundary areas
	for(i = 0; i < m_aLps.GetSize(); i++) { // Initiate
		m_aLps.GetAt(i)->UseTime = 0;
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) { // Use time
		for(j = 0; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			id = m_aPhds.GetAt(i)->iLps.GetAt(j);
			m_aLps.GetAt(id)->UseTime++;
		}
	}
	for(i = 0; i < m_aLps.GetSize(); i++) { // Only once
		if(m_aLps.GetAt(i)->UseTime == 1) {
			boudry.Add(m_aLps.GetAt(i)); // Only add, not new
		}
	}

	// Step 2: Remove vertical faces via equation
	CArray<CLoop*, CLoop*> tmp; // tmp boundary areas
	for(i = 0; i < boudry.GetSize(); i++) {
		for(t = 0; t < 3; t++) {
			id = boudry.GetAt(i)->iNds.GetAt(t);
			x[t] = m_aNds.GetAt(id)->x;
			y[t] = m_aNds.GetAt(id)->y;
			z[t] = m_aNds.GetAt(id)->z;
		}
		EquaOfTrgl(x, y, z, para);
		if(fabs(para[2]) > 0.00001)  tmp.Add(boudry.GetAt(i));
	}
	boudry.RemoveAll(); // Only remove, not delete
	for(i = 0; i < tmp.GetSize(); i++) {
		boudry.Add(tmp.GetAt(i));
	}
	tmp.RemoveAll();
	
	// Step 3: Remove the bottom faces
	//// flag = 0 : loop is not identified
	//// flag = 1 : loop is on top surface
	//// flag = -1: loop is on botton surface
	CLoop * lpa, * lpb;
	for(i = 0; i < boudry.GetSize(); i++) {
		boudry.GetAt(i)->flag = 0;
	}
	for(i = 0; i < boudry.GetSize()-1; i++) {
		lpa = boudry.GetAt(i);
		if(lpa->flag != 0)  continue;
	
		// 重心 center
		cx = 0.0;  cy = 0.0;  cz = 0.0;
		n = lpa->iNds.GetSize();
		if(lpa->iNds.GetAt(0) == lpa->iNds.GetAt(n-1)) {
			--n; // the first and the last nodes are the same
		}
		for(t = 0; t < n; t++) {
			id = lpa->iNds.GetAt(t);
			cx += m_aNds.GetAt(id)->x;
			cy += m_aNds.GetAt(id)->y;
			cz += m_aNds.GetAt(id)->z;
		}
		cx /= n;  cy /= n;  cz /= n;
		CNode * nd = new CNode(i, cx, cy, cz);

		for(j = i+1; j < boudry.GetSize(); j++) {
			lpb = boudry.GetAt(j);
			m = lpb->iNds.GetSize();
			if(lpb->iNds.GetAt(0) == lpb->iNds.GetAt(m-1)) {
				--m; // Check for next step
			}

			// Create a polygon 
			CLoop * face = new CLoop();
			for(t = 0; t < m; t++) {
				id = lpb->iNds.GetAt(t);
				tx = m_aNds.GetAt(id)->x;
				ty = m_aNds.GetAt(id)->y;
				tz = m_aNds.GetAt(id)->z;
				face->pNds.Add(new CNode(id, tx, ty, tz));
			}
			t = 0; // Add the first again for "IsNdInPolygon"
			id = lpb->iNds.GetAt(t);
			tx = m_aNds.GetAt(id)->x;
			ty = m_aNds.GetAt(id)->y;
			tz = m_aNds.GetAt(id)->z;
			face->pNds.Add(new CNode(id, tx, ty, tz));
			
			if(IsNdInPolygon(nd, face, myEps) == false) {
				face->Clear();  delete face;  continue; // Outside
			}
			face->Clear();  delete face;
           
			for(t = 0; t < 3; t++) {
				id = lpb->iNds.GetAt(t);
				x[t] = m_aNds.GetAt(id)->x;
				y[t] = m_aNds.GetAt(id)->y;
				z[t] = m_aNds.GetAt(id)->z;
			}
			EquaOfTrgl(x, y, z, para);
			double newz = (0.0 - para[0]*cx - para[1]*cy - para[3]) / para[2];

			bFind = false;
			if(cz > newz + 0.00001) { // 比较同一映射点的Z坐标
				boudry.GetAt(i)->flag = 1;  boudry.GetAt(j)->flag = -1;
				bFind = true;
			}
			else if(cz < newz - 0.00001){
				boudry.GetAt(i)->flag = -1;  boudry.GetAt(j)->flag = 1;
				bFind = true;
			}

			if(bFind == true) {
				break;
			}
		}
		delete nd;
	}

	/* // Old Merging, too slow
	for(i = 0; i < boudry.GetSize(); i++) {
		if(boudry.GetAt(i)->flag == 1) {
		    CLoop * lp = new CLoop();
		    for(j = 0; j < boudry.GetAt(i)->iNds.GetSize(); j++) {
			    id = boudry.GetAt(i)->iNds.GetAt(j);
			    tx = m_aNds.GetAt(id)->x;
			    ty = m_aNds.GetAt(id)->y;
			    tz = m_aNds.GetAt(id)->z;
			    lp->pNds.Add(new CNode(id, tx, ty, tz));
		    }
		    topSurf->pLps.Add(lp);
		}
	}
	boudry.RemoveAll();
	
	EmergePolygonSurface(topSurf); // merge
	*/

	// New Merging
	// For Output
	if(pFrame == NULL) {
	     pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	}
	str = _T("Start to Creat Top Surface... ");
	pFrame->AddStrToOutputWnd(1, str);

	for(i = 0; i < boudry.GetSize(); i++) {
		if(boudry.GetAt(i)->flag == 1) {
			topSurf->pLps.Add(boudry.GetAt(i));
		}
	}
	boudry.RemoveAll();

	CArray<int, int> tNds;
	id = topSurf->pLps.GetAt(0)->iNds.GetAt(0);
	tNds.Add(id);

	int index;
	CLoop lp;
	for(i = 0; i < topSurf->pLps.GetSize(); i++) {
		for(j = 0; j < topSurf->pLps.GetAt(i)->iNds.GetSize(); j++) {
			id = topSurf->pLps.GetAt(i)->iNds.GetAt(j);

			bFind = false;
			for(k = 0; k < tNds.GetSize(); k++) {
				if(tNds.GetAt(k) == id) { bFind = true;  index = k; break; }
			}
			if(bFind == true) {
				lp.iNds.Add(index);
			}
			else {
				tNds.Add(id);  index = tNds.GetUpperBound();
				lp.iNds.Add(index);
			}
		}

		topSurf->pLps.GetAt(i)->iNds.RemoveAll();
		for(j = 0; j < lp.iNds.GetSize(); j++) {
			topSurf->pLps.GetAt(i)->iNds.Add(lp.iNds.GetAt(j));
		}
		lp.iNds.RemoveAll();

	}

	topSurf->pNds.RemoveAll();
	for(i = 0; i < tNds.GetSize(); i++) {
		id = tNds.GetAt(i);
		topSurf->pNds.Add(m_aNds.GetAt(id));
	}
	tNds.RemoveAll();

}

// 2013.04.24 by MeiGang
// creat the top surface via all polyhedrons
void CRealModelView::CreatBottomSurfaceBest(CSurf* topSurf)
{
	int i, j, k, t, n, m, id;
	double cx, cy, cz, tx, ty, tz;
	double x[3], y[3], z[3], para[4];
	bool bFind, bExist;

	// Step 1: Creat all boundary faces, Only used once
	CArray<CLoop*, CLoop*> boudry; // all boundary areas
	for(i = 0; i < m_aLps.GetSize(); i++) { // Initiate
		m_aLps.GetAt(i)->UseTime = 0;
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) { // Use time
		for(j = 0; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++) {
			id = m_aPhds.GetAt(i)->iLps.GetAt(j);
			m_aLps.GetAt(id)->UseTime++;
		}
	}
	for(i = 0; i < m_aLps.GetSize(); i++) { // Only once
		if(m_aLps.GetAt(i)->UseTime == 1) {
			boudry.Add(m_aLps.GetAt(i)); // Only add, not new
		}
	}

	// Step 2: Remove vertical faces via equation
	CArray<CLoop*, CLoop*> tmp; // tmp boundary areas
	for(i = 0; i < boudry.GetSize(); i++) {
		for(t = 0; t < 3; t++) {
			id = boudry.GetAt(i)->iNds.GetAt(t);
			x[t] = m_aNds.GetAt(id)->x;
			y[t] = m_aNds.GetAt(id)->y;
			z[t] = m_aNds.GetAt(id)->z;
		}
		EquaOfTrgl(x, y, z, para);
		if(fabs(para[2]) > 0.00001)  tmp.Add(boudry.GetAt(i));
	}
	boudry.RemoveAll(); // Only remove, not delete
	for(i = 0; i < tmp.GetSize(); i++) {
		boudry.Add(tmp.GetAt(i));
	}
	tmp.RemoveAll();
	
	// Step 3: No longer Remove the bottom faces
	// but to remove the TOP faces. Very important
	//// flag = 0 : loop is not identified
	//// flag = 1 : loop is on top surface
	//// flag = -1: loop is on botton surface
	CLoop * lpa, * lpb;
	for(i = 0; i < boudry.GetSize(); i++) {
		boudry.GetAt(i)->flag = 0;
	}
	for(i = 0; i < boudry.GetSize()-1; i++) {
		lpa = boudry.GetAt(i);
		if(lpa->flag != 0)  continue;
	
		// 重心 center
		cx = 0.0;  cy = 0.0;  cz = 0.0;
		n = lpa->iNds.GetSize();
		if(lpa->iNds.GetAt(0) == lpa->iNds.GetAt(n-1)) {
			--n; // the first and the last nodes are the same
		}
		for(t = 0; t < n; t++) {
			id = lpa->iNds.GetAt(t);
			cx += m_aNds.GetAt(id)->x;
			cy += m_aNds.GetAt(id)->y;
			cz += m_aNds.GetAt(id)->z;
		}
		cx /= n;  cy /= n;  cz /= n;
		CNode * nd = new CNode(i, cx, cy, cz);

		for(j = i+1; j < boudry.GetSize(); j++) {
			lpb = boudry.GetAt(j);
			m = lpb->iNds.GetSize();
			if(lpb->iNds.GetAt(0) == lpb->iNds.GetAt(m-1)) {
				--m; // Check for next step
			}

			// Create a polygon 
			CLoop * face = new CLoop();
			for(t = 0; t < m; t++) {
				id = lpb->iNds.GetAt(t);
				tx = m_aNds.GetAt(id)->x;
				ty = m_aNds.GetAt(id)->y;
				tz = m_aNds.GetAt(id)->z;
				face->pNds.Add(new CNode(id, tx, ty, tz));
			}
			t = 0; // Add the first again for "IsNdInPolygon"
			id = lpb->iNds.GetAt(t);
			tx = m_aNds.GetAt(id)->x;
			ty = m_aNds.GetAt(id)->y;
			tz = m_aNds.GetAt(id)->z;
			face->pNds.Add(new CNode(id, tx, ty, tz));
			
			if(IsNdInPolygon(nd, face, myEps) == false) {
				delete face;  continue; // Outside
			}
			delete face;
           
			for(t = 0; t < 3; t++) {
				id = lpb->iNds.GetAt(t);
				x[t] = m_aNds.GetAt(id)->x;
				y[t] = m_aNds.GetAt(id)->y;
				z[t] = m_aNds.GetAt(id)->z;
			}
			EquaOfTrgl(x, y, z, para);
			double newz = (0.0 - para[0]*cx - para[1]*cy - para[3]) / para[2];

			bFind = false;
			/*
			if(cz > newz + 0.00001) { // 比较同一映射点的Z坐标
			//	boudry.GetAt(i)->flag = 1;  boudry.GetAt(j)->flag = -1; // for top surface
				boudry.GetAt(i)->flag = -1;  boudry.GetAt(j)->flag = 1; // for bottom surface
				bFind = true;
			}
			else if(cz < newz - 0.00001){
			//	boudry.GetAt(i)->flag = -1;  boudry.GetAt(j)->flag = 1; // for top surface
				boudry.GetAt(i)->flag = 1;  boudry.GetAt(j)->flag = -1; // for bottom surface
				bFind = true;
			}
			*/
			if(cz > newz + myEps) { // 比较同一映射点的Z坐标
			//	boudry.GetAt(i)->flag = 1;  boudry.GetAt(j)->flag = -1; // for top surface
				boudry.GetAt(i)->flag = -1;  boudry.GetAt(j)->flag = 1; // for bottom surface
				bFind = true;
			}
			else if(cz < newz - myEps){
			//	boudry.GetAt(i)->flag = -1;  boudry.GetAt(j)->flag = 1; // for top surface
				boudry.GetAt(i)->flag = 1;  boudry.GetAt(j)->flag = -1; // for bottom surface
				bFind = true;
			}
			else {
				// For Output
                if(pFrame == NULL) {
	                pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	            }
	            CString str;
	            str = _T("Error: CreatBottomSurfaceBest() ");
	            pFrame->AddStrToOutputWnd(1, str);
			}

			if(bFind == true) {
				break;
			}
		}
		delete nd;
	}

	/* // Old Merging, too slow
	for(i = 0; i < boudry.GetSize(); i++) {
		if(boudry.GetAt(i)->flag == 1) {
		    CLoop * lp = new CLoop();
		    for(j = 0; j < boudry.GetAt(i)->iNds.GetSize(); j++) {
			    id = boudry.GetAt(i)->iNds.GetAt(j);
			    tx = m_aNds.GetAt(id)->x;
			    ty = m_aNds.GetAt(id)->y;
			    tz = m_aNds.GetAt(id)->z;
			    lp->pNds.Add(new CNode(id, tx, ty, tz));
		    }
		    topSurf->pLps.Add(lp);
		}
	}
	boudry.RemoveAll();
	
	EmergePolygonSurface(topSurf); // merge
	*/

	// New Merging
	for(i = 0; i < boudry.GetSize(); i++) {
		if(boudry.GetAt(i)->flag == 1) {
			topSurf->pLps.Add(boudry.GetAt(i));
		}
	}
	boudry.RemoveAll();

	CArray<int, int> tNds;
	id = topSurf->pLps.GetAt(0)->iNds.GetAt(0);
	tNds.Add(id);

	int index;
	CLoop lp;
	for(i = 0; i < topSurf->pLps.GetSize(); i++) {
		for(j = 0; j < topSurf->pLps.GetAt(i)->iNds.GetSize(); j++) {
			id = topSurf->pLps.GetAt(i)->iNds.GetAt(j);

			bFind = false;
			for(k = 0; k < tNds.GetSize(); k++) {
				if(tNds.GetAt(k) == id) { bFind = true;  index = k; break; }
			}
			if(bFind == true) {
				lp.iNds.Add(index);
			}
			else {
				tNds.Add(id);  index = tNds.GetUpperBound();
				lp.iNds.Add(index);
			}
		}

		topSurf->pLps.GetAt(i)->iNds.RemoveAll();
		for(j = 0; j < lp.iNds.GetSize(); j++) {
			topSurf->pLps.GetAt(i)->iNds.Add(lp.iNds.GetAt(j));
		}
		lp.iNds.RemoveAll();

	}

	topSurf->pNds.RemoveAll();
	for(i = 0; i < tNds.GetSize(); i++) {
		id = tNds.GetAt(i);
		topSurf->pNds.Add(m_aNds.GetAt(id));
	}
	tNds.RemoveAll();

}


// 2010.11.25 by MeiGang
// after creating the top surface via all polyhedrons, transform the polygon
// surface to triangular surface
void CRealModelView::TransPolygonSurfToTriangularSurf(CSurf* topSurf)
{
	int i, j, k, t, n;

	// polygon partion, to transform polygon mesh to triangular mesh
	for(i = 0; i < topSurf->pLps.GetSize(); i++) {
		CLoop * lp = topSurf->pLps.GetAt(i);
		PolygonPartion(lp);
		for(j = 0; j < lp->pTrs.GetSize(); j++) {
			topSurf->pTrs.Add(lp->pTrs.GetAt(j));
		}
	}

	// emerger triangular surface to save the nodes into topSurf->pNds
	// and the mesh into nID[3]
	EmergeTriangleSurface(topSurf);

}

bool CRealModelView::IsTwoLoopSame(CLoop* lp1, CLoop* lp2)
{
	int i;
	int n1 = lp1->pNds.GetSize();
	int n2 = lp2->pNds.GetSize();
	if(n1 == 0 || n2 == 0 || n1 != n2) {
		return false;
	} 
	if(IsSameNode(lp1->pNds.GetAt(0), lp1->pNds.GetAt(n1-1), myEps)) { // first one and the last are the same
		--n1;
	}
	if(IsSameNode(lp2->pNds.GetAt(0), lp2->pNds.GetAt(n2-1), myEps)) {
		--n2;
	}
	int node = n1;
	double * x0 = new double[node], * y0 = new double[node], * z0 = new double[node];
	double * x1 = new double[node], * y1 = new double[node], * z1 = new double[node];
	for(i = 0; i < node; i++) {
		x0[i] = lp1->pNds.GetAt(i)->x;
		y0[i] = lp1->pNds.GetAt(i)->y;
		z0[i] = lp1->pNds.GetAt(i)->z;
		x1[i] = lp2->pNds.GetAt(i)->x;
		y1[i] = lp2->pNds.GetAt(i)->y;
		z1[i] = lp2->pNds.GetAt(i)->z;
	}
    return IsTwoElesAreSame(x0, y0, z0, x1, y1, z1, node);

}

bool CRealModelView::IsTwoLoopSameByID(CLoop* lp1, CLoop* lp2)
{
	int i, j;
	int n1 = lp1->iNds.GetSize();
	int n2 = lp2->iNds.GetSize();
	if(n1 == 0 || n2 == 0 || n1 != n2) {
		return false;
	} 
	if(lp1->iNds.GetAt(0) == lp1->iNds.GetAt(n1-1)) { // first one and the last are the same
		--n1;
	}
	if(lp2->iNds.GetAt(0) == lp2->iNds.GetAt(n2-1)) {
		--n2;
	}

	bool bexist = false;
	for(i = 0; i < n1; i++) {
		bexist = false;
		for(j = 0; j < n2; j++) {
			if(lp1->iNds.GetAt(i) == lp2->iNds.GetAt(j)) { 
				bexist = true; break;
			}
		}
		if(!bexist) return false;
	}

	return true;

}

// last modification on 2013.4.30
void CRealModelView::CutWhenAddNewSurfaceFast(CSurf* newSurf)
{
	int i, j, k, t, id, icase, iProperty;
	double x, y, z;
	CArray<CPohd*, CPohd*> newPhds; // new polyhedrons

	// Collision detection
	//// Substep 1: Divide any loop into triangles 
	CArray<CLoop*, CLoop*> tmpLps;
	for(i = 0; i < m_aLps.GetSize(); i++) {
		CLoop * lp = new CLoop();
		for(j = 0; j < m_aLps.GetAt(i)->iNds.GetSize(); j++) {
			id = m_aLps.GetAt(i)->iNds.GetAt(j);
			x = m_aNds.GetAt(id)->x;
			y = m_aNds.GetAt(id)->y;
			z = m_aNds.GetAt(id)->z;
			lp->pNds.Add(new CNode(id, x, y, z));
		}
		PolygonPartion3D(lp); // Triangulate
		tmpLps.Add(lp); // Save
	}

	//// Substep 2: Save all triangles
	CArray<CTrgl*, CTrgl*> tmpTrs;
	for(i = 0; i < tmpLps.GetSize(); i++) {
		for(j = 0; j < tmpLps.GetAt(i)->pTrs.GetSize(); j++) {
			tmpTrs.Add(tmpLps.GetAt(i)->pTrs.GetAt(j));
		}
	}

#ifndef EGC
	//// Substep 3: RAPID library
	CArray<CPair*, CPair*> tmpInsectPair;
	CTrgl *ta;

	// Construct
    RAPID_model *b1 = new RAPID_model;
    RAPID_model *b2 = new RAPID_model;

	// Add triangles
	double p0[3], p1[3], p2[3];

	//// tmp Trs : model-1
	b1->BeginModel();
	for(i = 0; i < tmpTrs.GetSize(); i++) {
		ta = tmpTrs.GetAt(i);
		p0[0] = ta->x[0];  p0[1] = ta->y[0];  p0[2] = ta->z[0];
		p1[0] = ta->x[1];  p1[1] = ta->y[1];  p1[2] = ta->z[1];
		p2[0] = ta->x[2];  p2[1] = ta->y[2];  p2[2] = ta->z[2];
		b1->AddTri(p0, p1, p2, i);
	}
	b1->EndModel();

	//// surf : model-2
	b2->BeginModel();
	for(i = 0; i < newSurf->pTrs.GetSize(); i++) {
		ta = newSurf->pTrs.GetAt(i);
		p0[0] = ta->x[0];  p0[1] = ta->y[0];  p0[2] = ta->z[0];
		p1[0] = ta->x[1];  p1[1] = ta->y[1];  p1[2] = ta->z[1];
		p2[0] = ta->x[2];  p2[1] = ta->y[2];  p2[2] = ta->z[2];
		b2->AddTri(p0, p1, p2, i);
	}
	b2->EndModel();

	// this placement causes them to overlap a large amount.
    double R1[3][3], R2[3][3], T1[3], T2[3];
  
    R1[0][0] = R1[1][1] = R1[2][2] = 1.0; // No scale
    R1[0][1] = R1[1][0] = R1[2][0] = 0.0;
    R1[0][2] = R1[1][2] = R1[2][1] = 0.0;

    R2[0][0] = R2[1][1] = R2[2][2] = 1.0;
    R2[0][1] = R2[1][0] = R2[2][0] = 0.0;
    R2[0][2] = R2[1][2] = R2[2][1] = 0.0;
  
    T1[0] = 0.0;  T1[1] = 0.0; T1[2] = 0.0; // No transform
    T2[0] = 0.0;  T2[1] = 0.0; T2[2] = 0.0;

    // now we can perform a collision query:
    RAPID_Collide(R1, T1, b1, R2, T2, b2, RAPID_ALL_CONTACTS);

	// Save intersected triangle-pair
	int id1, id2;
	for(i = 0; i < RAPID_num_contacts; i++) {
		id1 = RAPID_contact[i].id1;
		id2 = RAPID_contact[i].id2;
		tmpInsectPair.Add(new CPair(id1, id2));
	}

	delete b1;  delete b2;

	CString str;
	str.Format(_T("Intersect Pairs = %d \n"), tmpInsectPair.GetSize() );
	AfxMessageBox(str);
	
#else
	//// Substep 3: RAPID library
	CArray<CPair*, CPair*> tmpInsectPair;
	CTrgl *ta;

	// Construct
    RAPID_model *b1 = new RAPID_model;
    RAPID_model *b2 = new RAPID_model;

	// Add triangles
	float p0[3], p1[3], p2[3];

	//// tmp Trs : model-1
	b1->BeginModel();
	for(i = 0; i < tmpTrs.GetSize(); i++) {
		ta = tmpTrs.GetAt(i);
		p0[0] = ta->x[0].floatValue();  p0[1] = ta->y[0].floatValue();  p0[2] = ta->z[0].floatValue();
		p1[0] = ta->x[1].floatValue();  p1[1] = ta->y[1].floatValue();  p1[2] = ta->z[1].floatValue();
		p2[0] = ta->x[2].floatValue();  p2[1] = ta->y[2].floatValue();  p2[2] = ta->z[2].floatValue();
		b1->AddTriEGC(p0, p1, p2, i);
	}
	b1->EndModel();

	//// surf : model-2
	b2->BeginModel();
	for(i = 0; i < newSurf->pTrs.GetSize(); i++) {
		ta = newSurf->pTrs.GetAt(i);
		p0[0] = ta->x[0].floatValue();  p0[1] = ta->y[0].floatValue();  p0[2] = ta->z[0].floatValue();
		p1[0] = ta->x[1].floatValue();  p1[1] = ta->y[1].floatValue();  p1[2] = ta->z[1].floatValue();
		p2[0] = ta->x[2].floatValue();  p2[1] = ta->y[2].floatValue();  p2[2] = ta->z[2].floatValue();
		b2->AddTriEGC(p0, p1, p2, i);
	}
	b2->EndModel();

	// this placement causes them to overlap a large amount.
    float R1[3][3], R2[3][3], T1[3], T2[3];
  
    R1[0][0] = R1[1][1] = R1[2][2] = 1.0; // No scale
    R1[0][1] = R1[1][0] = R1[2][0] = 0.0;
    R1[0][2] = R1[1][2] = R1[2][1] = 0.0;

    R2[0][0] = R2[1][1] = R2[2][2] = 1.0;
    R2[0][1] = R2[1][0] = R2[2][0] = 0.0;
    R2[0][2] = R2[1][2] = R2[2][1] = 0.0;
  
    T1[0] = 0.0;  T1[1] = 0.0; T1[2] = 0.0; // No transform
    T2[0] = 0.0;  T2[1] = 0.0; T2[2] = 0.0;

    // now we can perform a collision query:
    RAPID_CollideEGC(R1, T1, b1, R2, T2, b2, RAPID_ALL_CONTACTS);

	// Save intersected triangle-pair
	int id1, id2;
	for(i = 0; i < RAPID_num_contacts; i++) {
		id1 = RAPID_contact[i].id1;
		id2 = RAPID_contact[i].id2;
		tmpInsectPair.Add(new CPair(id1, id2));
	}

	delete b1;  delete b2;

	CString str;
	str.Format(_T("Intersect Pairs = %d \n"), tmpInsectPair.GetSize() );
	AfxMessageBox(str);

#endif

	//// Substep 4: Update intersected pairs for each Trgl
	CPair * pr;
	for(i = 0; i < tmpInsectPair.GetSize(); i++) {
		pr = tmpInsectPair.GetAt(i);
		tmpTrs.GetAt(pr->FstID)->iRgnIntrTr.Add(pr->SedID);
	}

	//// Substep 5: For each Loop AND polyhdeon
	bool bexist = false;
	CTrgl * tr = NULL;
	CLoop * lp = NULL;
	CPohd * ph = NULL;
	for(i = 0; i < tmpLps.GetSize(); i++) { // Loop
		lp = tmpLps.GetAt(i);

		for(j = 0; j < lp->pTrs.GetSize(); j++) {
			tr = lp->pTrs.GetAt(j);
			for(k = 0; k < tr->iRgnIntrTr.GetSize(); k++) {
				id = tr->iRgnIntrTr.GetAt(k);
				bexist = false;
				for(t = 0; t < lp->iRgnIntrTr.GetSize(); t++) {
					if(id == lp->iRgnIntrTr.GetAt(t)) { bexist = true; break; }
				}
				if(bexist == false)  lp->iRgnIntrTr.Add(id);
			}
		}
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) { // Polyhedron
		ph = m_aPhds.GetAt(i);

		for(j = 0; j < ph->iLps.GetSize(); j++) {
			lp = tmpLps.GetAt(ph->iLps.GetAt(j));
			for(k = 0; k < lp->iRgnIntrTr.GetSize(); k++) {
				id = lp->iRgnIntrTr.GetAt(k);
				bexist = false;
				for(t = 0; t < ph->iRgnIntrTr.GetSize(); t++) {
					if(id == ph->iRgnIntrTr.GetAt(t)) { bexist = true; break; }
				}
				if(bexist == false)  ph->iRgnIntrTr.Add(id);
			}
		}
	}
	

	// noticeablly, m_iPhdNumBefAdd represents the original polyhedrons' 
	// number before adding a new surface. it is very important to use this.
	// it will have some mistakes when m_aPhds.GetSize() is used.
	// For output window
	if(pFrame == NULL) {
	   pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	}
	str.Format(_T("Starting to Cut Polyhedrons "));
	pFrame->AddStrToOutputWnd(1, str);

	int id_blk, id_phd, num_cut = 0, num_phd = m_aPhds.GetSize();
	for(i = 0; i < m_iBlkNumBefAdd; i++) {
	
		// For progress window
	    CProgressWnd wndProgress(this, _T("Progress"), TRUE);
	    int myRance = m_aBlks.GetAt(i)->iPhds.GetSize();
	    wndProgress.SetRange(0, myRance);
	    wndProgress.SetText(_T("This is a progress window...\n\n")
						    _T("Try dragging it around or hitting Cancel."));

		m_aBlks.GetAt(i)->pPhs.RemoveAll();
		for(j = 0; j < m_aBlks.GetAt(i)->iPhds.GetSize(); j++) {
			// For progress windown
		    wndProgress.StepIt();
		    wndProgress.PeekAndPump();
		    if (wndProgress.Cancelled()) {
			   MessageBox(_T("Progress Cancelled"));
			   ::PostQuitMessage(0);
		    }

			// For output window
			CString str;
		    str.Format(_T("Cutting Polyhedrons :  i = %d  [ / %d]"), j, num_phd);
		    pFrame->AddStrToOutputWnd(1, str);

			id_phd = m_aBlks.GetAt(i)->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_phd);

			if(j == 4177 || j == 4588)  continue;

			/*
			// Information
			if(phd->iRgnIntrTr.IsEmpty() == true) {
				CString str;
		        str.Format(_T("   i = %d  [ / %d] Not intersected "), num_cut, num_phd);
		        pFrame->AddStrToOutputWnd(1, str);
			}
			*/

			phd->flag = 0;
			iProperty = phd->iProperty;
			CPohd * posi = new CPohd();
			CPohd * nega = new CPohd();
			icase = IntrOfTriSurfaceAndPolyhedronFast(newSurf, phd, posi, nega);
			if(icase == 0) {
				posi->Clear();  delete posi;  
				nega->Clear();  delete nega;
			}
			else if(icase == 1) {
				nega->Clear();  delete nega;
				posi->iProperty = phd->iProperty;
				newPhds.Add(posi);
				phd->flag = -1;
			}
			else {
				posi->iProperty = iProperty;
				nega->iProperty = iProperty;
				newPhds.Add(posi);  newPhds.Add(nega);
				phd->flag = -1;
			}

			// add
			m_aBlks.GetAt(i)->pPhs.Add(phd);
		}
	}

	// update polyhedron and block groups
	// if flag == -1, it needs to remove
	double dist;
	//// for all old polyhedrons
	for(i = 0; i < m_iBlkNumBefAdd; i++) {
		for(j = 0; j < m_aBlks.GetAt(i)->pPhs.GetSize(); j++) {
			CPohd * phd = m_aBlks.GetAt(i)->pPhs.GetAt(j);
			if(phd->flag != 0)  continue;
			dist = DistOfPolyhedronToSurface(phd, newSurf);
			if(dist > 0.0)  phd->flag = -1; // original, correct, for up
	//		if(dist < 0.0)  phd->flag = -1; // for down
			else phd->flag = 1;
		}
	}
	// remove
	for(i = 0; i < m_iBlkNumBefAdd; i++) {
		for(j = m_aBlks.GetAt(i)->pPhs.GetSize()-1; j > -1; j--) {
			CPohd * phd = m_aBlks.GetAt(i)->pPhs.GetAt(j);
            if(phd->flag == -1) {
                m_aBlks.GetAt(i)->pPhs.RemoveAt(j);
			}
		}
	}

	//// for all new polyhedrons
	for(i = 0; i < newPhds.GetSize(); i++) {
		CPohd * phd = newPhds.GetAt(i);
		dist = DistOfPolyhedronToSurface(phd, newSurf);
		if(dist > 0.0)  phd->flag = -1;  // original, for up 
	//	if(dist < 0.0)  phd->flag = -1;  // for down
		else phd->flag = 1;
	}
	// remove invalid polyhedrons
	for(i = newPhds.GetSize()-1; i > -1; i--) { // remove those polyhedrons which need to delete
		if(newPhds.GetAt(i)->flag == -1) {
			delete newPhds.GetAt(i);  newPhds.RemoveAt(i);
		}
	}

	for(i = 0; i < newPhds.GetSize(); i++) {
		iProperty = newPhds.GetAt(i)->iProperty;
		m_aBlks.GetAt(iProperty)->pPhs.Add(newPhds.GetAt(i));
	}
	
	// update block groups
	for(i = m_aBlks.GetSize() - 1; i > -1; i--) { // delete some invalid group
		if(m_aBlks.GetAt(i)->pPhs.GetSize() == 0) {
			//	delete m_aBlks.GetAt(i);  
			m_aBlks.RemoveAt(i);
		}
	}

	// update polyhedron set
	m_aPhds.RemoveAll();
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		for(j = 0; j < m_aBlks.GetAt(i)->pPhs.GetSize(); j++) {
			m_aPhds.Add(m_aBlks.GetAt(i)->pPhs.GetAt(j));
		}
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		m_aPhds.GetAt(i)->ID = i;
	}
	id = 0; 
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		m_aBlks.GetAt(i)->iProperty = i;
		m_aBlks.GetAt(i)->iPhds.RemoveAll();
		for(j = 0; j < m_aBlks.GetAt(i)->pPhs.GetSize(); j++) {
			m_aBlks.GetAt(i)->pPhs.GetAt(j)->iProperty = i;
			id_phd = id++;
			m_aBlks.GetAt(i)->pPhs.GetAt(j)->ID = id_phd;
			m_aBlks.GetAt(i)->iPhds.Add(id_phd);
		}
	}

}

// last modification on 2010.12.7
void CRealModelView::CutWhenAddNewSurface(CSurf* newSurf)
{
	/*
	int i, j, id, icase, iProperty;
	CArray<CPohd*, CPohd*> newPhds; // new polyhedrons

	// noticeablly, m_iPhdNumBefAdd represents the original polyhedrons' 
	// number before adding a new surface. it is very important to use this.
	// it will have some mistakes when m_aPhds.GetSize() is used.
    for(i = 0; i < m_iPhdNumBefAdd; i++) { // creat and add new polyhedrons
		m_aPhds.GetAt(i)->flag = 0; // initiate
		CPolyhedron * phd = m_aPhds.GetAt(i);
		iProperty = phd->iProperty;
		CPolyhedron * posi = new CPolyhedron();
		CPolyhedron * nega = new CPolyhedron();
		icase = IntrOfTriSurfaceAndPolyhedron(newSurf, phd, posi, nega);
		if(icase == 0) {
			delete posi;  delete nega;
		}
		else if(icase == 1) {
			delete nega;
			posi->iProperty = phd->iProperty;
			newPhds.Add(posi);
			m_aPhds.GetAt(i)->flag = -1;
		}
		else {
			posi->iProperty = iProperty;
			nega->iProperty = iProperty;
			newPhds.Add(posi);  newPhds.Add(nega);
			m_aPhds.GetAt(i)->flag = -1;
		}
	}

	// first time : update block groups
	for(i = 0; i < m_aBlks.GetSize(); i++) { // 可能出错！！！
		for(j = m_aBlks.GetAt(i)->pPhds.GetSize()-1; j > -1; j--) {
		//	delete m_aBlks.GetAt(i)->pPhds.GetAt(j);
			m_aBlks.GetAt(i)->pPhds.RemoveAt(j);
		}
		m_aBlks.GetAt(i)->pPhds.RemoveAll();
		
		for(j = 0; j < m_aBlks.GetAt(i)->iPhds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->iPhds.GetAt(j);
			m_aBlks.GetAt(i)->pPhds.Add(m_aPhds.GetAt(id));
		}
	}

	// update polyhedron and block groups
	// if flag == -1, it needs to remove
	double dist;
	//// for all old polyhedrons
	for(i = 0; i < m_iPhdNumBefAdd; i++) {
		CPolyhedron * phd = m_aPhds.GetAt(i);
		if(phd->flag == -1)  continue;
		dist = DistOfPolyhedronToSurface(phd, newSurf);
		if(dist > 0.0)  phd->flag = -1;
		else phd->flag = 1;
	}
	// remove invalid polyhedrons
	for(i = m_aPhds.GetSize()-1; i > -1; i--) { // remove those polyhedrons which need to delete
		if(m_aPhds.GetAt(i)->flag == -1) {
			delete m_aPhds.GetAt(i);  m_aPhds.RemoveAt(i);
		}
	}

	//// for all new polyhedrons
	for(i = 0; i < newPhds.GetSize(); i++) {
		CPolyhedron * phd = newPhds.GetAt(i);
		dist = DistOfPolyhedronToSurface(phd, newSurf);
		if(dist > 0.0)  phd->flag = -1;
		else phd->flag = 1;
	}
	// remove invalid polyhedrons
	for(i = newPhds.GetSize()-1; i > -1; i--) { // remove those polyhedrons which need to delete
		if(newPhds.GetAt(i)->flag == -1) {
			delete newPhds.GetAt(i);  newPhds.RemoveAt(i);
		}
	}

	for(i = 0; i < newPhds.GetSize(); i++) {
		iProperty = newPhds.GetAt(i)->iProperty;
		m_aBlks.GetAt(iProperty)->pPhds.Add(newPhds.GetAt(i));
	}
	
    // combine new and old polyhedrons together
	for(i = 0; i < newPhds.GetSize(); i++) {
		m_aPhds.Add(newPhds.GetAt(i));
	}

	// update block groups
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		m_aPhds.GetAt(i)->ID = i;
	}

	for(i = m_aBlks.GetSize() - 1; i > -1; i--) {
		if(m_aBlks.GetAt(i)->pPhds.GetSize() == 0) {
		//	delete m_aBlks.GetAt(i);  
			m_aBlks.RemoveAt(i);
		}
	}
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		m_aBlks.GetAt(i)->iProperty = i;
		m_aBlks.GetAt(i)->iPhds.RemoveAll();
		for(j = 0; j < m_aBlks.GetAt(i)->pPhds.GetSize(); j++) {
			m_aBlks.GetAt(i)->pPhds.GetAt(j)->iProperty = i;
			m_aBlks.GetAt(i)->iPhds.Add(m_aBlks.GetAt(i)->pPhds.GetAt(j)->ID);
		}
	}
	*/

	if(pFrame == NULL) {
	   pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	}

	int i, j, id, icase, iProperty;
	CArray<CPohd*, CPohd*>newPhds; // new polyhedrons

	// noticeablly, m_iPhdNumBefAdd represents the original polyhedrons' 
	// number before adding a new surface. it is very important to use this.
	// it will have some mistakes when m_aPhds.GetSize() is used.

	int id_blk, id_phd;
	for(i = 0; i < m_iBlkNumBefAdd; i++) {
		// For output window
		CString str;
		str.Format(_T("Cutting Polyhedrons :  i = %d  [ / %d]"), i, m_iBlkNumBefAdd);
		pFrame->AddStrToOutputWnd(1, str);

		m_aBlks.GetAt(i)->pPhs.RemoveAll();
		for(j = 0; j < m_aBlks.GetAt(i)->iPhds.GetSize(); j++) {
			id_phd = m_aBlks.GetAt(i)->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_phd);
			phd->flag = 0;
			iProperty = phd->iProperty;
			CPohd * posi = new CPohd();
			CPohd * nega = new CPohd();
			icase = IntrOfTriSurfaceAndPolyhedron(newSurf, phd, posi, nega);
			if(icase == 0) {
				delete posi;  delete nega;
			}
			else if(icase == 1) {
				delete nega;
				posi->iProperty = phd->iProperty;
				newPhds.Add(posi);
				phd->flag = -1;
			}
			else {
				posi->iProperty = iProperty;
				nega->iProperty = iProperty;
				newPhds.Add(posi);  newPhds.Add(nega);
				phd->flag = -1;
			}

			// add
			m_aBlks.GetAt(i)->pPhs.Add(phd);
		}
	}

	// update polyhedron and block groups
	// if flag == -1, it needs to remove
	double dist;
	//// for all old polyhedrons
	for(i = 0; i < m_iBlkNumBefAdd; i++) {
		for(j = 0; j < m_aBlks.GetAt(i)->pPhs.GetSize(); j++) {
			CPohd * phd = m_aBlks.GetAt(i)->pPhs.GetAt(j);
			if(phd->flag != 0)  continue;
			dist = DistOfPolyhedronToSurface(phd, newSurf);
			if(dist > 0.0)  phd->flag = -1; // original, correct, for up
	//		if(dist < 0.0)  phd->flag = -1; // for down
			else phd->flag = 1;
		}
	}
	// remove
	for(i = 0; i < m_iBlkNumBefAdd; i++) {
		for(j = m_aBlks.GetAt(i)->pPhs.GetSize()-1; j > -1; j--) {
			CPohd * phd = m_aBlks.GetAt(i)->pPhs.GetAt(j);
            if(phd->flag == -1) {
                m_aBlks.GetAt(i)->pPhs.RemoveAt(j);
			}
		}
	}

	//// for all new polyhedrons
	for(i = 0; i < newPhds.GetSize(); i++) {
		CPohd * phd = newPhds.GetAt(i);
		dist = DistOfPolyhedronToSurface(phd, newSurf);
		if(dist > 0.0)  phd->flag = -1;  // original, for up 
	//	if(dist < 0.0)  phd->flag = -1;  // for down
		else phd->flag = 1;
	}
	// remove invalid polyhedrons
	for(i = newPhds.GetSize()-1; i > -1; i--) { // remove those polyhedrons which need to delete
		if(newPhds.GetAt(i)->flag == -1) {
			delete newPhds.GetAt(i);  newPhds.RemoveAt(i);
		}
	}

	for(i = 0; i < newPhds.GetSize(); i++) {
		iProperty = newPhds.GetAt(i)->iProperty;
		m_aBlks.GetAt(iProperty)->pPhs.Add(newPhds.GetAt(i));
	}
	
	// update block groups
	for(i = m_aBlks.GetSize() - 1; i > -1; i--) { // delete some invalid group
		if(m_aBlks.GetAt(i)->pPhs.GetSize() == 0) {
			//	delete m_aBlks.GetAt(i);  
			m_aBlks.RemoveAt(i);
		}
	}

	// update polyhedron set
	m_aPhds.RemoveAll();
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		for(j = 0; j < m_aBlks.GetAt(i)->pPhs.GetSize(); j++) {
			m_aPhds.Add(m_aBlks.GetAt(i)->pPhs.GetAt(j));
		}
	}
	for(i = 0; i < m_aPhds.GetSize(); i++) {
		m_aPhds.GetAt(i)->ID = i;
	}
	id = 0; 
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		m_aBlks.GetAt(i)->iProperty = i;
		m_aBlks.GetAt(i)->iPhds.RemoveAll();
		for(j = 0; j < m_aBlks.GetAt(i)->pPhs.GetSize(); j++) {
			m_aBlks.GetAt(i)->pPhs.GetAt(j)->iProperty = i;
			id_phd = id++;
			m_aBlks.GetAt(i)->pPhs.GetAt(j)->ID = id_phd;
			m_aBlks.GetAt(i)->iPhds.Add(id_phd);
		}
	}

}

void CRealModelView::IntersectWhenAddNewSurface(CSurf* newSurf)
{
	int i, j, nID[3];
	double x, y, z;

	// step 1 : creat top surface
	CSurf* topSurf = new CSurf();
	CreatTopSurface(topSurf);

	// step 2 : polygon partion, to transform polygon mesh to triangular mesh
    TransPolygonSurfToTriangularSurf(topSurf);
    //// Begin : copy it to a new surface, very important!
	CSurf * tmptop = new CSurf();
	for(i = 0; i < topSurf->pNds.GetSize(); i++) {
		x = topSurf->pNds.GetAt(i)->x;
		y = topSurf->pNds.GetAt(i)->y;
		z = topSurf->pNds.GetAt(i)->z;
		tmptop->pNds.Add(new CNode(i, x, y, z));
	}
	for(i = 0; i < topSurf->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			nID[j] = topSurf->pTrs.GetAt(i)->nID[j];
		}
		tmptop->pTrs.Add(new CTrgl(i, nID[0], nID[1], nID[2]));
		// 如果将上述代码改为如下代码，将出错！千万注意！
		// CTrgl * tr = new CTrgl();
		// for(j = 0; j < 3; j++) {
		// 	tr->nID[j] = topSurf->pTrs.GetAt(i)->nID[j];
		// }
		// tmptop->pTrs.Add(tr);
	}
	CopyDetailsForSurface(tmptop); // very important
	//// End

	// step 3 : intersect to form new polyhedrons. also, remove the invailid polyhedrons
	CBlock * newBlk = new CBlock();

	// InsectOfTrglSurf_2DAnd3D(tmptop, newSurf, newBlk); // noticeablly, tmptop is the first while newSurf is the second!
	IntrTriMesh2DAnd3D_Float(tmptop, newSurf, newBlk);

	for(i = 0; i < newBlk->pPhs.GetSize(); i++) {
		newBlk->pPhs.GetAt(i)->iProperty = m_aBlks.GetSize(); // property is the groups ID
		m_aPhds.Add(newBlk->pPhs.GetAt(i));
	}
	m_aBlks.Add(newBlk);

	// emerge polyhedrons. there is no need to emerge elements at this time. this will be done when all 
	// both intesecting and cutting are finished.
	
}


void CRealModelView::On3DModelIntersectAndCut()
{
	// boundary box test
	CNode * rgnLB = new CNode();
	CNode * rgnTR = new CNode();
	CreatBoundaryBox(pSurf, rgnLB, rgnTR);

	CNode * blkLB = new CNode();
	CNode * blkTR = new CNode();
	CreatBoundaryBox(blkLB, blkTR);

	bool bIntersect = BoundaryBoxIntrTest(rgnLB, rgnTR, blkLB, blkTR);

//	CutWhenAddNewSurface(pSurf); // cut some polyhedrons down
	CutWhenAddNewSurfaceFast(pSurf); // cut some polyhedrons down

	/*
	if(bIntersect == false) {
		AfxMessageBox(_T("Only intersecting needs to do !"), MB_OK|MB_ICONINFORMATION);
		IntersectWhenAddNewSurface(pSurf); // intersect to form some new polyhedrons
	}
	else {
		AfxMessageBox(_T("Both intersecting and cutting need to do !"), MB_OK|MB_ICONINFORMATION);
		IntersectWhenAddNewSurface(pSurf); // intersect to form some new polyhedrons
		CutWhenAddNewSurface(pSurf); // cut some polyhedrons down
	}
	*/

	// emerge all polyhedrons, block groups
//	EmergeBlockGroups();
	EmergeBlockGroupsBest();

	// indentify the group and properity

	// notice
	AfxMessageBox(_T("Everything is Done !"), MB_OK|MB_ICONINFORMATION);

	RenderBlockGroups();
	
}


void CRealModelView::On3DModelSaveBlockGroup()
{
	// TODO: Add your command handler code here
	SaveBlockGroups();
}

// Save Block groups including properties
void CRealModelView::On3DModelSaveBlockGroupProp()
{
	SaveBlockGroupsProperty();
}

double CRealModelView::DistOfTwoNode(CNode *nd1, CNode *nd2)
{
	double d = sqrt((nd1->x - nd2->x) * (nd1->x - nd2->x) + 
		            (nd1->y - nd2->y) * (nd1->y - nd2->y) + 
					(nd1->z - nd2->z) * (nd1->z - nd2->z));
    return d;
}

/******************************************************************************
                   Signed Distance from Point to Plane
                       Author: Mei Gang
					   Freiburg, Germany, 2010.5.7
******************************************************************************/
/*
Compute the distance from a point to a plane. The point is pt[3]. The plane is
para[0] * X + para[1] * Y + para[2] * Z + para[3] = 0.
The distance is positive if on same side as the normal, otherwise negative.
Assume the plane normal to be of unit length.
*/
double CRealModelView::DistOfPointToPlane(double * pt, double * para)
{
    return ((para[0] * pt[0] + para[1] * pt[1] + para[2] * pt[2]) + para[3]);
}

double CRealModelView::DistOfPolyhedronToSurface(CPohd * phd, CSurf* surf)
{
	int i, j, k;
	double dist = 0.0, xn, yn, zn, center[3];
	double x[3], y[3], z[3], para[4];
	bool bexist;
	CArray<CNode*, CNode*> nds; 
	nds.Add(phd->pLps.GetAt(0)->pNds.GetAt(0));
	for(i = 0; i < phd->pLps.GetSize(); i++) {
		for(j = 0; j < phd->pLps.GetAt(i)->pNds.GetSize(); j++) {
			CNode * nd = phd->pLps.GetAt(i)->pNds.GetAt(j);
			bexist = false;
			for(k = 0; k < nds.GetSize(); k++) {
				if( IsSameNode(nd, nds.GetAt(k), myEps) == 1 ) {
					bexist = true;
				}
			}
			if( bexist == false ) {
				nds.Add(nd);
			}
		}
	}
	// 重心
	xn = 0.0;  yn = 0.0;  zn = 0.0;
	for(i = 0; i < nds.GetSize(); i++) {
		xn += nds.GetAt(i)->x;
		yn += nds.GetAt(i)->y;
		zn += nds.GetAt(i)->z;
	}
	xn /= nds.GetSize();  yn /= nds.GetSize();  zn /= nds.GetSize();
	center[0] = xn;  center[1] = yn;  center[2] = zn;
	// 重心落在那个三角形内
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			x[j] = surf->pTrs.GetAt(i)->x[j];
			y[j] = surf->pTrs.GetAt(i)->y[j];
			z[j] = surf->pTrs.GetAt(i)->z[j];
		}
		if(IsNdInTrgl(xn, yn, x[0], y[0], x[1], y[1], x[2], y[2]) != 3) {
			EquaOfTrgl(x, y, z, para);

			double newz = (0.0 - para[0]*xn - para[1]*yn - para[3]) / para[2];
			dist = zn - newz;
			// dist = DistOfPointToPlane(center, para);
			break;
		}
	}

	return dist;

}

// 2010.11.9 By MeiGang
// to compute the cross intersection, not to form new polyhedronss
bool CRealModelView::IntrOfPlaneAndPolyhedron_CrossIntersection(CPlane * pPla, CPohd * pPhd, CLoop * pIntr)
{
	//compute all signed distances
	double dist;
	double pt[3], para[4];
	int posi = 0, zero = 0, nega = 0;
	int i, j, id;
	
	for(i = 0; i < 4; i++) para[i] = pPla->Para[i];
	for(i = 0; i < pPhd->pLps.GetSize(); i++) {
		for(j = 0; j < pPhd->pLps.GetAt(i)->pNds.GetSize(); j++) {
			pt[0] = pPhd->pLps.GetAt(i)->pNds.GetAt(j)->x;
			pt[1] = pPhd->pLps.GetAt(i)->pNds.GetAt(j)->y;
			pt[2] = pPhd->pLps.GetAt(i)->pNds.GetAt(j)->z;
			dist = DistOfPointToPlane(pt, para);
			if(dist > Epsilon)  posi++;
			else if(fabs(dist) < Epsilon) zero++;
			else nega++;
		}
	}

	if( !(posi > 0 && nega > 0) ) {
		return false;
	}
    
	//Compute each face of polyhedron with plane
	int iType;
	CArray<CEdge*, CEdge*>m_aEgs;
	for(i = 0; i < pPhd->pLps.GetSize(); i++) {
		CLoop * pPlg = pPhd->pLps.GetAt(i);
        CEdge * pEg = new CEdge();
		CLoop *pLp0 = new CLoop();  CLoop *pLp1 = new CLoop();

//		iType = IntrOfPolygonAndPlane(pPlg, pPla, pEg, pLp0, pLp1);
		iType = IntrOfPolygonAndPlaneNew(pPlg, pPla, pEg);

		//Save old face
        if(iType == 0 || iType == 1) {
		}
		//Save 1 new face and intersection line
		if(iType == 2) {
            m_aEgs.Add(pEg);
		}
		//Save 1 new face and intersection line
		if(iType == 3) {
            m_aEgs.Add(pEg);
		}
	}

	//Form intersection face(cross intersection), use m_aEgs
	pIntr->pNds.Add(m_aEgs.GetAt(0)->fstNode);
	pIntr->pNds.Add(m_aEgs.GetAt(0)->lstNode);

	while( !IsSameNode(pIntr->pNds.GetAt(0), pIntr->pNds.GetAt(pIntr->pNds.GetUpperBound()), myEps) ) {
		for( i = 1; i < m_aEgs.GetSize(); i++) {
			if(IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 1), m_aEgs.GetAt(i)->fstNode, myEps)
				&&
			   !IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 2), m_aEgs.GetAt(i)->lstNode, myEps)) {
				pIntr->pNds.Add(m_aEgs.GetAt(i)->lstNode);
				break;
			}
			
			if(IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 1), m_aEgs.GetAt(i)->lstNode, myEps)
				&&
			   !IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 2), m_aEgs.GetAt(i)->fstNode, myEps)) {
				pIntr->pNds.Add(m_aEgs.GetAt(i)->fstNode);
				break;
			}
			
		}
	}
	//Delete last node, very important!
	pIntr->pNds.RemoveAt(pIntr->pNds.GetSize() - 1);
	// Begin : copy
	double x, y, z;
	CArray<CNode*, CNode*> tmpNode;
	for(i = 0; i < pIntr->pNds.GetSize(); i++) {
		tmpNode.Add(pIntr->pNds.GetAt(i));
	}
	pIntr->pNds.RemoveAll();
	for(i = 0; i < tmpNode.GetSize(); i++) {
		x = tmpNode.GetAt(i)->x;
		y = tmpNode.GetAt(i)->y;
		z = tmpNode.GetAt(i)->z;
		pIntr->pNds.Add(new CNode(i, x, y, z));
	}
	tmpNode.RemoveAll();
	// End

	//Remove
	for(i = m_aEgs.GetSize() - 1; i > -1; i--) {
		delete m_aEgs.GetAt(i);  m_aEgs.RemoveAt(i);
	}

	return true;

}

// 2010.12.1
// 2010.5.15 by Mei Gang, freiburg, germany
// intersection of polygon and plane
// CLoop * pPlg: a polygon
// CPlane * pPla: a plane
// CEdge * pEg: intersection line of polygon and plane
int CRealModelView::IntrOfPolygonAndPlaneNew(CLoop * pPlg, CPlane * pPla, CEdge * pEg)
{
	//compute all signed distances
	int n = pPlg->pNds.GetSize();
	double dist;
	double pt[3], para[4];
	int i, j, posi = 0, zero = 0, nega = 0;
	for(i = 0; i < 4; i++) para[i] = pPla->Para[i];
	for(i = 0; i < n; i++) {
		pt[0] = pPlg->pNds.GetAt(i)->x;
        pt[1] = pPlg->pNds.GetAt(i)->y;
		pt[2] = pPlg->pNds.GetAt(i)->z;
		dist = DistOfPointToPlane(pt, para);
		if(dist > Epsilon)  { posi++; }
		else if(fabs(dist) < Epsilon)  { zero++; }
		else { nega++; }
	}

	//case 0: not intersect
	if(posi == n || nega == n) {
		return 0;
	}

	//case 1: coplanar
	if(zero == n) {
		return 1;
	}

    //case 2 or 3: can form a intersection line
	CArray<CNode*, CNode*>intrNds;
	CNode * ps, * pe, * pIntrNd;
	for(i = 0; i < n - 1; i++) {
		ps = pPlg->pNds.GetAt(i);  pe = pPlg->pNds.GetAt(i+1);
		CNode *pIntrNd = new CNode();
		if(IntrOfSegAndPlane(ps, pe, pPla, pIntrNd) == true ) { // intersect, and then divide
			intrNds.Add(pIntrNd);
		}
	}
	ps = pPlg->pNds.GetAt(n-1);
	pe = pPlg->pNds.GetAt(0);
	pIntrNd = new CNode();
	if(IntrOfSegAndPlane(ps, pe, pPla, pIntrNd) == true ) {
		intrNds.Add(pIntrNd);
	}
    
	//pEg:
	bool bexist;
	CArray<CNode*, CNode*>tmpNds;
	tmpNds.Add(intrNds.GetAt(0));
	for(i = 1; i < intrNds.GetSize(); i++) {
		bexist = false;
		for(j = 0; j < tmpNds.GetSize(); j++) {
			if(IsSameNode(intrNds.GetAt(i), tmpNds.GetAt(j), myEps) == 1) {
				bexist = true;  break;
			}
		}
		if( bexist == false) {
			tmpNds.Add(intrNds.GetAt(i));
		}
	}
	if(tmpNds.GetSize() == 1) {
		return 0;
	}
	if(tmpNds.GetSize() == 2) {
		pEg->fstNode = new CNode(0, tmpNds.GetAt(0)->x, tmpNds.GetAt(0)->y, tmpNds.GetAt(0)->z);
		pEg->lstNode = new CNode(1, tmpNds.GetAt(1)->x, tmpNds.GetAt(1)->y, tmpNds.GetAt(1)->z);
		return 2;
	}
	if(tmpNds.GetSize() > 2) {
		AfxMessageBox(_T("Can't form a intersection line ! \n Function : IntrOfPolygonAndPlane()."),MB_OK|MB_ICONSTOP);
		return 0;
	}

	return 0;

}

/*
intersection of segment and plane;
ps, pe: segment
pPla: plane
pIntrNd: intersection point
*/
bool CRealModelView::IntrOfSegAndPlane(CNode * ps, CNode * pe, CPlane * pPla, CNode * pIntrNd)
{
	double distps, distpe;
	double pt[3], para[4];

	for(int i = 0; i < 4; i++) para[i] = pPla->Para[i];

	pt[0] = ps->x;  pt[1] = ps->y;  pt[2] = ps->z;
	distps = DistOfPointToPlane(pt, para);
	pt[0] = pe->x;  pt[1] = pe->y;  pt[2] = pe->z;
	distpe = DistOfPointToPlane(pt, para);

	//case 1:
	if( (distps > Epsilon && distpe > Epsilon) || // not intersect
		(distps < -1*Epsilon && distpe < -1*Epsilon) ) {
		return false;
	}

	//case 2:
	if(fabs(distps) < Epsilon && fabs(distpe) < Epsilon) { // coplanar
		return false;
	}

	//case 3:
	if(fabs(distps) > Epsilon && fabs(distpe) < Epsilon) { // node pe is just on the plane
		pIntrNd->x = pe->x;  pIntrNd->y = pe->y;  pIntrNd->z = pe->z;
		return true;
	}
	if(fabs(distps) < Epsilon && fabs(distpe) > Epsilon) { // node ps is just on the plane
		pIntrNd->x = ps->x;  pIntrNd->y = ps->y;  pIntrNd->z = ps->z;
		return true;
	}

	//case 4: the most common situation
	double t;
	double px, py, pz; // S(t) = P0 + t(P1 - P0), S(t) = P + td;
	double dx, dy, dz; // direction of segment
    px = ps->x;  py = ps->y;  pz = ps->z; 
	dx = pe->x - ps->x;  dy = pe->y - ps->y;  dz = pe->z - ps->z;
	t = -1 * (para[0] * px + para[1] * py + para[2] * pz + para[3]);
	t = t / (para[0] * dx + para[1] * dy + para[2] * dz);

	pIntrNd->x = ps->x + t * dx;
	pIntrNd->y = ps->y + t * dy;
	pIntrNd->z = ps->z + t * dz;

	return true;
}

double CRealModelView::AreaOfTrgl(double x0, double y0, double x1, double y1, double x2, double y2)
{
	return((x0 * y1 + x1 * y2 + x2 * y0 - x2 * y1 - x1 * y0 - x0 * y2) / 2.0);
}

double CRealModelView::AreaOfTrgl(CTrgl * tr)
{
	int i;
	double x[3], y[3];
	for(i = 0; i < 3; i++) {
		x[i] = tr->x[i];  y[i] = tr->y[i];
	}
	return((x[0] * y[1] + x[1] * y[2] + x[2] * y[0] - x[2] * y[1] - x[1] * y[0] - x[0] * y[2]) / 2.0);
}

// last modified on 2010.11.9
int CRealModelView::IntrOfPlaneAndPolyhedron(CPlane * pPla, CPohd * pPhd, CLoop * pIntr)
{
	return 0;
	/*
	//compute all signed distances
	double dist;
	double pt[3], para[4];
	int posi = 0, zero = 0, nega = 0;
	int i, j, k, nid, lid;
	for(i = 0; i < 4; i++) para[i] = pPla->para[i];
	for(i = 0; i < pPhd->iNds.GetSize(); i++) {
		nid = pPhd->iNds.GetAt(i);
		pt[0] = m_aNds.GetAt(nid)->x;
        pt[1] = m_aNds.GetAt(nid)->y;
		pt[2] = m_aNds.GetAt(nid)->z;
		dist = DistOfPointToPlane(pt, para);
		if(dist > Epsilon)  posi++;
		else if(fabs(dist) < Epsilon) zero++;
		else nega++;
	}

	//case 1: all points are on the same side
	if(posi == pPhd->pNds.GetSize() || nega == pPhd->pNds.GetSize()) {
		return false;
	}

	//case 2: a face on the plane
	bool bcoplanar;
	int iface;
	if( (posi >= 1 && zero >= 3 && nega == 0) ||
		(posi == 0 && zero >= 3 && nega >= 1) ){
		//which face is on the plane?
		for(i = 0; i < pPhd->iLps.GetSize(); i++) {
			bcoplanar = true;
			lid = pPhd->iLps.GetAt(i);
			for(j = 0; j < m_aLps.GetAt(lid)->iNds.GetSize(); j++) {
				nid = m_aLps.GetAt(lid)->iNds.GetAt(j);
				pt[0] = m_aNds.GetAt(nid)->x;
				pt[1] = m_aNds.GetAt(nid)->y;
				pt[2] = m_aNds.GetAt(nid)->z;
				dist = DistOfPointToPlane(pt, para);
				if(fabs(dist) < Epsilon) {
					bcoplanar = bcoplanar && true;
				}
				else {
					bcoplanar = bcoplanar && false;  break;
				}
			}
			if(bcoplanar) {
				iface = i;  break;
			}
		}

		//intersection
		lid = pPhd->iLps.GetAt(iface);
		for(i = 0; i < m_aLps.GetAt(lid)->iNds.GetSize(); i++) {
			nid = m_aLps.GetAt(lid)->iNds.GetAt(i);
			pt[0] = m_aNds.GetAt(nid)->x;
			pt[1] = m_aNds.GetAt(nid)->y;
			pt[2] = m_aNds.GetAt(nid)->z;
			pIntr->pNds.Add(new CNode(i, pt[0], pt[1], pt[2]));
		}

		return true;
	}

	//case 3: plane intersects polyhedron, 2 new polyhedrons formed
	CNode * ps, * pe;
	int ids, ide;
	CArray<CNode*, CNode*>pIntrNds;
	CArray<CLoop*, CLoop*>pNewFace;
	CArray<int, int>iorder;
	if(posi > 0 && nega > 0) {
		for(i = 0; i < pPhd->iLps.GetSize(); i++) {
			
			for(j = pIntrNds.GetUpperBound(); j > -1; j--){ 
				pIntrNds.GetAt(i)->DeleteObj();  delete pIntrNds.GetAt(j);  pIntrNds.RemoveAt(j);
			}
			iorder.RemoveAll();

			lid = pPhd->iLps.GetAt(i);
			for(j = 0; j < m_aLps.GetAt(lid)->iNds.GetSize() - 1; j++) {
				ids = m_aLps.GetAt(lid)->iNds.GetAt(j);
				ide = m_aLps.GetAt(lid)->iNds.GetAt(j + 1);
				ps = m_aNds.GetAt(ids);  pe = m_aNds.GetAt(ide);

				CNode * pIntrNd = new CNode();
				//intersect
				if( IntrOfSegAndPlane(ps, pe, pPla, pIntrNd) ) {
					pIntrNds.Add(pIntrNd);  iorder.Add(j);
				}

			}

			// if there are 2 intersection points,form a segment, divide the old face
			if(pIntrNds.GetSize() == 2) {
				//divide old face

				//save new face, update or delete old face

				//save intersection segment, prepare to form cross intersection
			}

		}

		//form cross intersection: CLoop * pIntr


	}	
	*/

}

// last modified on 2010.11.15 by MeiGang
bool CRealModelView::IntrOfTriangleAndPolyhedron(CTrgl * pTrgl, CPohd * pPhd, CLoop * pIntr)
{
	// test boundary box
	CNode * TrglLB = new CNode();  CNode * TrglTR = new CNode();
	CNode * PhdLB = new CNode();   CNode * PhdTR = new CNode();
	
	CreatBoundaryBox(pTrgl, TrglLB , TrglTR);
	CreatBoundaryBox(pPhd , PhdLB, PhdTR);

	if(TrglTR->x < PhdLB->x || TrglTR->y < PhdLB->y || TrglTR->z < PhdLB->z ||
		PhdTR->x < TrglLB->x || PhdTR->x < TrglLB->x || PhdTR->x < TrglLB->x ) {
		delete TrglLB;  delete TrglTR;  delete PhdLB;  delete PhdTR;
		return false;
	}

	// plane intersects with polyhedron
    CPlane * pPla = new CPlane();
	CLoop * pIntrTmp = new CLoop();

	EquaOfTrgl(pTrgl, pPla);
	bool bIntersect = IntrOfPlaneAndPolyhedron_CrossIntersection(pPla, pPhd, pIntrTmp);
	if( bIntersect == false) {
		delete TrglLB;  delete TrglTR;  delete PhdLB;  delete PhdTR;
		delete pPla;    pIntrTmp->Clear();  delete pIntrTmp;
		return false;
	}

	// change pIntrTmp and pTrgl to XY plane
	int i;
	double * x, * y, * z, * a;
    CCordnt * tCrdt = new CCordnt();
	CTrgl * newTrgl = new CTrgl();
	GetNewCoordt(pTrgl, tCrdt);
	x = new double[3];  y = new double[3];  z = new double[3];
	for(i = 0; i < 3; i++) { // transform
		x[i] = pTrgl->x[i] - tCrdt->u;
		y[i] = pTrgl->y[i] - tCrdt->v;
		z[i] = pTrgl->z[i] - tCrdt->w;
	}
	// 旋转前，矩阵求逆. very important
	a = new double[9];
	a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
	a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
	a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;
    Dcinv(a, 3);
	for(i = 0; i < 3; i++) {
		newTrgl->x[i] = x[i] * a[0] + y[i] * a[1] + z[i] * a[2];
		newTrgl->y[i] = x[i] * a[3] + y[i] * a[4] + z[i] * a[5];
		newTrgl->z[i] = x[i] * a[6] + y[i] * a[7] + z[i] * a[8];
	}
	delete [] x;  delete [] y;  delete [] z; 

	CLoop * newPoly = new CLoop();
	int n = pIntrTmp->pNds.GetSize();
	x = new double[n];  y = new double[n];  z = new double[n];
	for(i = 0; i < n; i++) { // transform
		x[i] = pIntrTmp->pNds.GetAt(i)->x - tCrdt->u;
		y[i] = pIntrTmp->pNds.GetAt(i)->y - tCrdt->v;
		z[i] = pIntrTmp->pNds.GetAt(i)->z - tCrdt->w;
		newPoly->pNds.Add(new CNode(i, x[i], y[i], z[i]));
	}
	// 旋转前，矩阵求逆. very important
	for(i = 0; i < n; i++) {
		newPoly->pNds.GetAt(i)->x = x[i] * a[0] + y[i] * a[1] + z[i] * a[2];
		newPoly->pNds.GetAt(i)->y = x[i] * a[3] + y[i] * a[4] + z[i] * a[5];
		newPoly->pNds.GetAt(i)->z = x[i] * a[6] + y[i] * a[7] + z[i] * a[8];
	}
	delete [] x;  delete [] y;  delete [] z;  

	// 判断环是否逆时针排列
	n = newPoly->pNds.GetSize();
	CNode * p = new CNode[newPoly->pNds.GetSize()];
	for(i = 0; i < newPoly->pNds.GetSize(); i++) {
		p[i].x = newPoly->pNds.GetAt(i)->x;
		p[i].y = newPoly->pNds.GetAt(i)->y;
	}
	if(IsCCwize(p, newPoly->pNds.GetSize()) == false) { // 改变环绕向
		for(int j = 0; j < n; j++) {
			newPoly->pNds.GetAt(j)->x = p[n-1-j].x;
			newPoly->pNds.GetAt(j)->y = p[n-1-j].y;
		}
	}
	delete [] p;

	CLoop * trglPoly = new CLoop();
	for(i = 0; i < 3; i++) {
		trglPoly->pNds.Add(new CNode(i, newTrgl->x[i], newTrgl->y[i], newTrgl->z[i]));
	}
    // add the first node as the last one. record it twice. important!
	trglPoly->pNds.Add(trglPoly->pNds.GetAt(0));
	newPoly->pNds.Add(newPoly->pNds.GetAt(0));

	// intersection between two polygons
//  bIntersect = IntrOfPolygonAndPolygon(trglPoly, newPoly, pIntr, myEps); // invalid in some cases
	bIntersect = IntrOfConvexPolygon(trglPoly, newPoly, pIntr);
	if(bIntersect == false) {
		return false;
	}

	// change it back!
	n = pIntr->pNds.GetSize();
	x = new double[n];  y = new double[n];  z = new double[n];
	for(i = 0; i < n; i++) { // save temporily
		x[i] = pIntr->pNds.GetAt(i)->x;
		y[i] = pIntr->pNds.GetAt(i)->y;
		z[i] = pIntr->pNds.GetAt(i)->z;
	}
    // get the original version of new coordinate, local coordinate system
	a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
	a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
	a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;

	for(i = 0; i < n; i++) { // rotate
		pIntr->pNds.GetAt(i)->x = x[i] * a[0] + y[i] * a[1] + z[i] * a[2];
		pIntr->pNds.GetAt(i)->y = x[i] * a[3] + y[i] * a[4] + z[i] * a[5];
		pIntr->pNds.GetAt(i)->z = x[i] * a[6] + y[i] * a[7] + z[i] * a[8];
	}
	for(i = 0; i < n; i++) { // transform
		pIntr->pNds.GetAt(i)->x += tCrdt->u;
		pIntr->pNds.GetAt(i)->y += tCrdt->v;
		pIntr->pNds.GetAt(i)->z += tCrdt->w;
	}
	delete [] x;  delete [] y;  delete [] z;  delete [] a;

	delete TrglLB;  delete TrglTR;  delete PhdLB;  delete PhdTR;
	delete pPla;  delete pIntrTmp;  
	delete tCrdt;  delete newTrgl;  
//	delete newPoly;

//	SaveOneLoop(pIntr);   return false;

	return true;

}

// last modified on 2010.12.3
// intersection of a polygon with another one
// assume the first point and the last point of loop are the same one
bool CRealModelView::IntrOfConvexPolygon(CLoop * plp0, CLoop * plp1, CLoop * pIntr)
{
	int i, j, k;
	double tx, ty, tz;
	CNode * p0, * d0, * p1, * d1, * i0, * i1;
	bool * b0 = new bool[plp0->pNds.GetSize()]; // classify
	bool * b1 = new bool[plp1->pNds.GetSize()];
	int * n0 = new int [plp0->pNds.GetSize()]; // count
	int * n1 = new int [plp1->pNds.GetSize()];

	CArray<CEdge*, CEdge*>m_aEgs;
	CArray<CPair*, CPair*>pairs;
	for(i = 0; i < plp0->pNds.GetSize(); i++) { // points of plp0 in plp1
		b0[i] = false;  n0[i] = 0;
		if( IsNdInPolygon(plp0->pNds.GetAt(i), plp1, myEps) )  b0[i] = true;
	}
	for(i = 0; i < plp1->pNds.GetSize(); i++) { // points of plp1 in plp0
		b1[i] = false;  n1[i] = 0;
		if( IsNdInPolygon(plp1->pNds.GetAt(i), plp0, myEps) )  b1[i] = true;
	}

	// special case: one polygon is in the other 
	bool bInside = true;
	for(i = 0; i < plp0->pNds.GetSize(); i++) {
		bInside = bInside && b0[i];
	}
	if(bInside == true) { // loop plp0 is in loop plp1
		pIntr->pNds.RemoveAll();
		for(i = 0; i < plp0->pNds.GetSize() - 1; i++) { // make sure that the first and last nodes are not the same
			tx = plp0->pNds.GetAt(i)->x;
			ty = plp0->pNds.GetAt(i)->y;
			tz = plp0->pNds.GetAt(i)->z;
			pIntr->pNds.Add(new CNode(i, tx, ty, tz));
		}
		return true;
	}
	bInside = true;
	for(i = 0; i < plp1->pNds.GetSize(); i++) {
		bInside = bInside && b1[i];
	}
	if(bInside == true) { // loop plp1 is in loop plp0
		for(i = 0; i < plp1->pNds.GetSize() - 1; i++) { // make sure that the first and last nodes are not the same
			tx = plp1->pNds.GetAt(i)->x;
			ty = plp1->pNds.GetAt(i)->y;
			tz = plp1->pNds.GetAt(i)->z;
			pIntr->pNds.Add(new CNode(i, tx, ty, tz));
		}
		return true;
	}

	// General case
	CArray<CNode*, CNode*> sectionNode;
	CArray<CEdge*, CEdge*> tmpEdge;

	//// node which is in the other polygon
	for(i = 0; i < plp0->pNds.GetSize() - 1; i++) {
		if(b0[i] == true) {
			sectionNode.Add(plp0->pNds.GetAt(i));
		}
	}
	for(i = 0; i < plp1->pNds.GetSize() - 1; i++) {
		if(b1[i] == true) {
			sectionNode.Add(plp1->pNds.GetAt(i));
		}
	}
	//// node which is the intersection of two segment
	for(i = 0; i < plp0->pNds.GetSize() - 1; i++) {
		p0 = plp0->pNds.GetAt(i);
		tx = plp0->pNds.GetAt(i+1)->x - plp0->pNds.GetAt(i)->x;
		ty = plp0->pNds.GetAt(i+1)->y - plp0->pNds.GetAt(i)->y;
		tz = plp0->pNds.GetAt(i+1)->z - plp0->pNds.GetAt(i)->z; 
		
		d0 = new CNode(i+1, tx, ty, tz);
		for(j = 0; j < plp1->pNds.GetSize() - 1; j++) {
			p1 = plp1->pNds.GetAt(j);
			tx = plp1->pNds.GetAt(j+1)->x - plp1->pNds.GetAt(j)->x;
			ty = plp1->pNds.GetAt(j+1)->y - plp1->pNds.GetAt(j)->y;
			tz = plp1->pNds.GetAt(j+1)->z - plp1->pNds.GetAt(j)->z;
			
			d1 = new CNode(j+1, tx, ty, tz);
			i0 = new CNode();  i1 = new CNode();
			if( IntrSegment2Segment2D(p0, d0, p1, d1, i0, i1) == 1) {
				sectionNode.Add(i0);
			}
			delete d1; 
		}
		delete d0; 
	}
	// remove some same node
	for(i = sectionNode.GetSize()-1; i > 0; i--) {
		for(j = i-1; j > -1; j--)
		if(IsSameNode(sectionNode.GetAt(i), sectionNode.GetAt(j), myEps)) {
			sectionNode.RemoveAt(j);  break;
		}
	}

	// form the boundary edges of intersection
	for(i = 0; i < plp0->pNds.GetSize() - 1; i++) {
		CEdge * eg = new  CEdge();
		eg->fstNode = plp0->pNds.GetAt(i);
		eg->lstNode = plp0->pNds.GetAt(i+1);
		tmpEdge.Add(eg);
	}
	for(i = 0; i < plp1->pNds.GetSize() - 1; i++) {
		CEdge * eg = new  CEdge();
		eg->fstNode = plp1->pNds.GetAt(i);
		eg->lstNode = plp1->pNds.GetAt(i+1);
		tmpEdge.Add(eg);
	}
	// check which two node are on a same segment
	CArray<int, int> index;
    for(i = 0; i < tmpEdge.GetSize(); i++) {
		index.RemoveAll();
		CEdge * eg = tmpEdge.GetAt(i);
		for(j = 0; j < sectionNode.GetSize(); j++) {
			CNode * pt = sectionNode.GetAt(j);
			if( IsPointOnSegment(eg->fstNode, eg->lstNode, pt, myEps) ) {
				index.Add(j);
			}
		}
		if(index.GetSize() == 2) {
			CEdge * edge = new CEdge();
			edge->fstNode = sectionNode.GetAt(index.GetAt(0));
			edge->lstNode = sectionNode.GetAt(index.GetAt(1));
			m_aEgs.Add(edge);
		}
		if(index.GetSize() > 2) {
			AfxMessageBox(_T("Error in Function : IntrOfConvexPolygon()."), MB_OK|MB_ICONSTOP);
			SaveOneLoop(plp0);  SaveOneLoop(plp1);
			return false;
			// exit(0);
		}
	}

	//form intersection loop
	for(i = m_aEgs.GetUpperBound(); i > -1; i--) {
		if(IsSameNode(m_aEgs.GetAt(i)->fstNode, m_aEgs.GetAt(i)->lstNode, myEps)) {
			delete m_aEgs.GetAt(i);  m_aEgs.RemoveAt(i);
		}
	}
	
	if(m_aEgs.GetSize() < 3) {
	//	AfxMessageBox("Can't form a loop because edges are less than 3. \n Function : IntrOfPolygonAndPolygon().", MB_OK|MB_ICONSTOP);
		return false; // can not form a loop
	}
    /*
	pIntr->pNds.Add(m_aEgs.GetAt(0)->fstNode);
	pIntr->pNds.Add(m_aEgs.GetAt(0)->lstNode);
	while( !IsSameNode(pIntr->pNds.GetAt(0), pIntr->pNds.GetAt(pIntr->pNds.GetUpperBound())) ) {
		for( i = 1; i < m_aEgs.GetSize(); i++) {
			if(IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetUpperBound()), m_aEgs.GetAt(i)->fstNode)) {
				pIntr->pNds.Add(m_aEgs.GetAt(i)->lstNode);
			}
		}
	}
	*/
	
	//Form intersection face(cross intersection), use m_aEgs
	pIntr->pNds.RemoveAll();
	pIntr->pNds.Add(m_aEgs.GetAt(0)->fstNode);
	pIntr->pNds.Add(m_aEgs.GetAt(0)->lstNode);
	while( !IsSameNode(pIntr->pNds.GetAt(0), pIntr->pNds.GetAt(pIntr->pNds.GetUpperBound()), myEps) ) {
		for( i = 1; i < m_aEgs.GetSize(); i++) {
			if(IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 1), m_aEgs.GetAt(i)->fstNode, myEps)
				&&
			   !IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 2), m_aEgs.GetAt(i)->lstNode, myEps )) {
				pIntr->pNds.Add(m_aEgs.GetAt(i)->lstNode);
				break;
			}
			
			// sometimes, this must be used, in other times, this is not neccessary ! 2010.11.30
			if(IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 1), m_aEgs.GetAt(i)->lstNode, myEps)
				&&
			   !IsSameNode(pIntr->pNds.GetAt(pIntr->pNds.GetSize() - 2), m_aEgs.GetAt(i)->fstNode, myEps )) {
				pIntr->pNds.Add(m_aEgs.GetAt(i)->fstNode);
				break;
			}
			
		}
	}
	//Delete last node, very important!
	pIntr->pNds.RemoveAt(pIntr->pNds.GetSize() - 1);
	// Begin : copy, very important, 2010.11.30
	CArray<CNode*, CNode*> tmpNode;
	for(i = 0; i < pIntr->pNds.GetSize(); i++) {
		tmpNode.Add(pIntr->pNds.GetAt(i));
	}
	pIntr->pNds.RemoveAll();
	for(i = 0; i < tmpNode.GetSize(); i++) {
		tx = tmpNode.GetAt(i)->x;
		ty = tmpNode.GetAt(i)->y;
		tz = tmpNode.GetAt(i)->z;
		pIntr->pNds.Add(new CNode(i, tx, ty, tz));
	}
	tmpNode.RemoveAll();
	// End

	return true;
}

void CRealModelView::SaveOneLoop(CLoop * lp) 
{
	//For next step, save everything: Node, Area, Polyhedron, Block group
	CString fnBlk, fnameBlk;
	int i, nGroup = 1;
	if(AfxMessageBox(_T("Save a Loop / Area ?"), MB_YESNOCANCEL|MB_ICONQUESTION) != 6) return;
	CFileDialog Dlg4(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg4.DoModal() == IDOK) 
	{
		fnBlk = Dlg4.GetPathName(); 
		fnameBlk = fnBlk + _T("_LoopPoint");
		if(fnameBlk.Right(4)!="txt")   fnameBlk += ".txt";
		ofstream fout0(fnameBlk);
		// Node
		for(i = 0; i < lp->pNds.GetSize(); i++){ 
			fout0 << lp->pNds.GetAt(i)->ID << "      " << lp->pNds.GetAt(i)->x << "      " << lp->pNds.GetAt(i)->y << "      " <<  lp->pNds.GetAt(i)->z <<endl;
		}

		fout0.close();
	}

}

//2009.11.18 by Mei Gang
// 矢量叉积判断多边形顶点排列方向的方法
bool CRealModelView::IsCCwize(CNode* p, int vcount) 
{ 
	int i, index; 
	CNode a, b, v;
	v.x = p[0].x;  v.y = p[0].y;
	index = 0; 
	for(i = 1; i < vcount; i++) // 找到最低且最左顶点，肯定是凸顶点 
	{ 
		if( ( p[i].y < v.y || (fabs( p[i].y - v.y ) < 0.00001) ) && p[i].x < v.x )
		{
            v.x = p[i].x;  v.y = p[i].y;
			index = i;
		}
	} 

	a.x = p[(index-1+vcount)%vcount].x;  a.y = p[(index-1+vcount)%vcount].y;// 顶点v的前一顶点 
	b.x = p[(index+1)%vcount].x;  b.y = p[(index+1)%vcount].y;// 顶点v的后一顶点

	double sum = ((v.x-a.x)*(b.y-a.y)-(b.x-a.x)*(v.y-a.y)); 
	if( sum > 0)  return true;
	else return false;
} 

bool CRealModelView::IsCCwize(double * px, double * py, int vcount) 
{ 
	int i, index; 
	CNode a, b, v;
	v.x = px[0];  v.y = py[0];
	index = 0; 
	for(i = 1; i < vcount; i++) // 找到最低且最左顶点，肯定是凸顶点 
	{ 
		if( ( py[i] < v.y || (fabs( py[i] - v.y ) < 0.00001) ) && px[i] < v.x )
		{
            v.x = px[i];  v.y = py[i];
			index = i;
		}
	} 

	a.x = px[(index-1+vcount)%vcount];  a.y = py[(index-1+vcount)%vcount];// 顶点v的前一顶点 
	b.x = px[(index+1)%vcount];  b.y = py[(index+1)%vcount];// 顶点v的后一顶点

	double sum = ((v.x-a.x)*(b.y-a.y)-(b.x-a.x)*(v.y-a.y)); 
	if( sum > 0)  return true;
	else return false;
} 

// 2010.10.19 by Mei Gang, last modified on 2010.11.26
// return 0: when surface does not intersect with polyhedron
// return 1: when there is only 1 new polyhedron created
// return 2: when there are 2 new polyhedrons created
// in short, return the number of new polyhedron created
int CRealModelView::IntrOfTriSurfaceAndPolyhedron(CSurf * pRgn, CPohd * pPhd, CPohd * pPosi, CPohd * pNega)
{
	int i, j, k, t, n;
	double x[3], y[3], z[3], para[4], d[2];
	double xx, yy, zz, dist;
		
	// boundary box test
	CNode * rgnLB = new CNode();  CNode * rgnTR = new CNode();
	CNode * phdLB = new CNode();  CNode * phdTR = new CNode();
	CreatBoundaryBox(pRgn, rgnLB, rgnTR);
	CreatBoundaryBox(pPhd, phdLB, phdTR);
	bool bIntersect = BoundaryBoxIntrTest(rgnLB, rgnTR, phdLB, phdTR);
	if(bIntersect == false) {
		delete rgnLB;  delete rgnTR;  delete phdLB;  delete phdTR;
		return 0;
	}

	// intersect normally
	CArray<CLoop*, CLoop*> section; // cross section
	CArray<CEdge*, CEdge*> boudry; // boundary edges
	CArray<CLoop*, CLoop*> face; // all faces after cutting
	
	for(i = 0; i < pRgn->pTrs.GetSize(); i++) { // intersection
		CTrgl * tr = pRgn->pTrs.GetAt(i);
		CLoop * lp = new CLoop();
		if (IntrOfTriangleAndPolyhedron(tr, pPhd, lp) == true ) {
			section.Add(lp);
		}
		else {
			delete lp;
		}
	}

	if(section.GetSize() == 0) { // no cross section, not intersect
		return 0;
	}

	for(i = 0; i < section.GetSize(); i++) { // save edges
		for(j = 0; j < section.GetAt(i)->pNds.GetSize() - 1; j++) {
			CEdge * eg = new CEdge();
			eg->fstNode = section.GetAt(i)->pNds.GetAt(j);
			eg->lstNode = section.GetAt(i)->pNds.GetAt(j+1);
			boudry.Add(eg);
		}
		CEdge * eg = new CEdge();
		eg->fstNode = section.GetAt(i)->pNds.GetAt(j);
		eg->lstNode = section.GetAt(i)->pNds.GetAt(0);
		boudry.Add(eg);
	}
	for(i = 0; i < boudry.GetSize(); i++) { // find boundary edges
		boudry.GetAt(i)->UseTime = 1;
	}
	for(i = 0; i < boudry.GetSize() - 1; i++) { 
		CNode * fst0 = boudry.GetAt(i)->fstNode;
		CNode * lst0 = boudry.GetAt(i)->lstNode;
		for(j = i + 1; j < boudry.GetSize(); j++) {
			CNode * fst1 = boudry.GetAt(j)->fstNode;
			CNode * lst1 = boudry.GetAt(j)->lstNode;
			if( (IsSameNode(fst0, fst1, myEps) && IsSameNode(lst0, lst1, myEps)) || 
				(IsSameNode(fst0, lst1, myEps) && IsSameNode(lst0, fst1, myEps)) ){
				boudry.GetAt(i)->UseTime = 2;
				boudry.GetAt(j)->UseTime = 2;
			}
		}
	}
	for(i = boudry.GetSize()-1; i > -1; i--) { // remove interior edges, keep boundary ones
		if(boudry.GetAt(i)->UseTime == 2) {
			delete boudry.GetAt(i);  boudry.RemoveAt(i);
		}
	}

	// divide some original areas and form new faces
	CArray<CEdge*, CEdge*> faceEdge; // face edges
    
	for(i = 0; i < pPhd->pLps.GetSize(); i++) {
		for(t = 0; t < 3; t++) {
			x[t] = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->x;
			y[t] = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->y;
			z[t] = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->z;
		}
		EquaOfTrgl(x, y, z, para);
		faceEdge.RemoveAll();
		for(j = 0; j < boudry.GetSize(); j++) { // find which edges are on the face
			if(boudry.GetAt(j)->UseTime == 2)  continue; // not a boundary edge
			x[0] = boudry.GetAt(j)->fstNode->x;
			y[0] = boudry.GetAt(j)->fstNode->y;
			z[0] = boudry.GetAt(j)->fstNode->z;
			x[1] = boudry.GetAt(j)->lstNode->x;
			y[1] = boudry.GetAt(j)->lstNode->y;
			z[1] = boudry.GetAt(j)->lstNode->z;
			d[0] = x[0] * para[0] + y[0] * para[1] + z[0] * para[2] + para[3];
			d[1] = x[1] * para[0] + y[1] * para[1] + z[1] * para[2] + para[3];
			if(fabs(d[0]) < 0.00001 && fabs(d[1]) < 0.00001) {
				faceEdge.Add(boudry.GetAt(j));
			}
		}
		if(faceEdge.GetSize() == 0) { // area does't intersect surface
			face.Add(pPhd->pLps.GetAt(i)); // add the original area directly
			continue;
		}
		
        // last modification 2010.12.01
		// Begin : 将同一个面的boundary edges连成一条线, 保存在tmpEdge
		CArray<CEdge*, CEdge*> tmpEdge;

		n = faceEdge.GetSize();
		CEdge * tmp = new CEdge();
		xx = faceEdge.GetAt(0)->fstNode->x;
		yy = faceEdge.GetAt(0)->fstNode->y;
		zz = faceEdge.GetAt(0)->fstNode->z;
		tmp->fstNode = new CNode(0, xx, yy, zz);
		xx = faceEdge.GetAt(0)->lstNode->x;
		yy = faceEdge.GetAt(0)->lstNode->y;
		zz = faceEdge.GetAt(0)->lstNode->z;
		tmp->lstNode = new CNode(1, xx, yy, zz);
		tmpEdge.Add(tmp); // first one
		while (tmpEdge.GetSize() != n) {
			CNode * fstNode = tmpEdge.GetAt(0)->fstNode; // the first node of polyline
			CNode * fstSecond = tmpEdge.GetAt(0)->lstNode; // the first second node
			CNode * lstNode = tmpEdge.GetAt(tmpEdge.GetSize()-1)->lstNode; // the last node of polyline
			CNode * lstSecond = tmpEdge.GetAt(tmpEdge.GetSize()-1)->fstNode; // the last second node
			for(j = 1; j < faceEdge.GetSize(); j++) {
				CNode * fst0 = faceEdge.GetAt(j)->fstNode;
				CNode * lst0 = faceEdge.GetAt(j)->lstNode;
				// there are 4 cases:
				//// 头 head 
				if(IsSameNode(fst0, fstNode, myEps) == 1 && IsSameNode(lst0, fstSecond, myEps) == 0) {
					CEdge * tmp = new CEdge();
					xx = faceEdge.GetAt(j)->fstNode->x;
					yy = faceEdge.GetAt(j)->fstNode->y;
					zz = faceEdge.GetAt(j)->fstNode->z;
					tmp->lstNode = new CNode(1, xx, yy, zz);
					xx = faceEdge.GetAt(j)->lstNode->x;
					yy = faceEdge.GetAt(j)->lstNode->y;
					zz = faceEdge.GetAt(j)->lstNode->z;
					tmp->fstNode = new CNode(0, xx, yy, zz);
					tmpEdge.InsertAt(0, tmp);
				}
				if(IsSameNode(lst0, fstNode, myEps) == 1 && IsSameNode(fst0, fstSecond, myEps) == 0) {
					CEdge * tmp = new CEdge();
					xx = faceEdge.GetAt(j)->fstNode->x;
					yy = faceEdge.GetAt(j)->fstNode->y;
					zz = faceEdge.GetAt(j)->fstNode->z;
					tmp->fstNode = new CNode(0, xx, yy, zz);
					xx = faceEdge.GetAt(j)->lstNode->x;
					yy = faceEdge.GetAt(j)->lstNode->y;
					zz = faceEdge.GetAt(j)->lstNode->z;
					tmp->lstNode = new CNode(1, xx, yy, zz);
					tmpEdge.InsertAt(0, tmp);
				}

				//// 尾 tail
				if(IsSameNode(fst0, lstNode, myEps) == 1 && IsSameNode(lst0, lstSecond, myEps) == 0) {
					CEdge * tmp = new CEdge();
					xx = faceEdge.GetAt(j)->fstNode->x;
					yy = faceEdge.GetAt(j)->fstNode->y;
					zz = faceEdge.GetAt(j)->fstNode->z;
					tmp->fstNode = new CNode(0, xx, yy, zz);
					xx = faceEdge.GetAt(j)->lstNode->x;
					yy = faceEdge.GetAt(j)->lstNode->y;
					zz = faceEdge.GetAt(j)->lstNode->z;
					tmp->lstNode = new CNode(1, xx, yy, zz);
					tmpEdge.Add(tmp);
				}
				if(IsSameNode(lst0, lstNode, myEps) == 1 && IsSameNode(fst0, lstSecond, myEps) == 0) {
					CEdge * tmp = new CEdge();
					xx = faceEdge.GetAt(j)->lstNode->x;
					yy = faceEdge.GetAt(j)->lstNode->y;
					zz = faceEdge.GetAt(j)->lstNode->z;
					tmp->fstNode = new CNode(0, xx, yy, zz);
					xx = faceEdge.GetAt(j)->fstNode->x;
					yy = faceEdge.GetAt(j)->fstNode->y;
					zz = faceEdge.GetAt(j)->fstNode->z;
					tmp->lstNode = new CNode(1, xx, yy, zz);
					tmpEdge.Add(tmp);
				}

			}
		}
		// End
		
		// the original edges before dividing, saved into faceEdge
		faceEdge.RemoveAll(); // clear out
		for(t = 0; t < pPhd->pLps.GetAt(i)->pNds.GetSize()-1; t++) {
			CEdge * eg = new CEdge();
			xx = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->x;
			yy = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->y;
			zz = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->z;
			eg->fstNode = new CNode(0, xx, yy, zz);
			xx = pPhd->pLps.GetAt(i)->pNds.GetAt(t+1)->x;
			yy = pPhd->pLps.GetAt(i)->pNds.GetAt(t+1)->y;
			zz = pPhd->pLps.GetAt(i)->pNds.GetAt(t+1)->z;
			eg->lstNode = new CNode(1, xx, yy, zz);
			faceEdge.Add(eg);
		}
		t = pPhd->pLps.GetAt(i)->pNds.GetSize();
		CEdge * eg = new CEdge(); // last edge
		xx = pPhd->pLps.GetAt(i)->pNds.GetAt(t-1)->x;
		yy = pPhd->pLps.GetAt(i)->pNds.GetAt(t-1)->y;
		zz = pPhd->pLps.GetAt(i)->pNds.GetAt(t-1)->z;
		eg->fstNode = new CNode(0, xx, yy, zz);
		xx = pPhd->pLps.GetAt(i)->pNds.GetAt(0)->x;
		yy = pPhd->pLps.GetAt(i)->pNds.GetAt(0)->y;
		zz = pPhd->pLps.GetAt(i)->pNds.GetAt(0)->z;
		eg->lstNode = new CNode(1, xx, yy, zz);
		faceEdge.Add(eg);

        // form 1 or 2 new faces by dividing the original one
		//// step 1: divide edge
		int original = faceEdge.GetSize(); // number of edges before dividing
		CNode * fstNode = tmpEdge.GetAt(0)->fstNode;
		CNode * lstNode = tmpEdge.GetAt(tmpEdge.GetSize()-1)->lstNode;
		for(t = 0; t < original; t++) {
			CNode * sp0 = faceEdge.GetAt(t)->fstNode;
			CNode * sp1 = faceEdge.GetAt(t)->lstNode;
			if(IsPointOnSegment3D(sp0, sp1, fstNode, myEps) == 2) {
				// add a new edge
				CEdge * eg = new CEdge();
				xx = fstNode->x;  yy = fstNode->y;  zz = fstNode->z;
				eg->fstNode = new CNode(0, xx, yy, zz);
				xx = sp1->x;  yy = sp1->y;  zz = sp1->z;
				eg->lstNode = new CNode(1, xx, yy, zz);
				faceEdge.Add(eg);
				// update the original edge
				sp1->x = fstNode->x;  sp1->y = fstNode->y;  sp1->z = fstNode->z;
			}
			if(IsPointOnSegment3D(sp0, sp1, lstNode, myEps) == 2) {
				// add a new edge
				CEdge * eg = new CEdge();
				xx = lstNode->x;  yy = lstNode->y;  zz = lstNode->z;
				eg->fstNode = new CNode(0, xx, yy, zz);
				xx = sp1->x;  yy = sp1->y;  zz = sp1->z;
				eg->lstNode = new CNode(1, xx, yy, zz);
				faceEdge.Add(eg);
				// update the original edge
				sp1->x = lstNode->x;  sp1->y = lstNode->y;  sp1->z = lstNode->z;
			}
		}

		//// step 2: form new areas
		////// negative area
		CLoop * negaLoop = new CLoop();
		for(t = 0; t < tmpEdge.GetSize(); t++) {
			xx = tmpEdge.GetAt(t)->fstNode->x;
			yy = tmpEdge.GetAt(t)->fstNode->y;
			zz = tmpEdge.GetAt(t)->fstNode->z;
			negaLoop->pNds.Add(new CNode(t, xx, yy, zz));
		}
		t = tmpEdge.GetSize() - 1;
		xx = tmpEdge.GetAt(t)->lstNode->x;
		yy = tmpEdge.GetAt(t)->lstNode->y;
		zz = tmpEdge.GetAt(t)->lstNode->z;
		negaLoop->pNds.Add(new CNode(t, xx, yy, zz));
	
		CNode * first = negaLoop->pNds.GetAt(0);
		int index = 1;
		bool bFind;
		while (IsSameNode(first, negaLoop->pNds.GetAt(index), myEps) == 0)
		{
			index = negaLoop->pNds.GetSize() - 1;
			CNode * last = negaLoop->pNds.GetAt(index);
			for(t = 0; t < faceEdge.GetSize(); t++) {
				bFind = false;
				CNode * fstNode = faceEdge.GetAt(t)->fstNode;
				CNode * lstNode = faceEdge.GetAt(t)->lstNode;
				if( IsSameNode(last, fstNode, myEps) == 1 && 
					IsSameNode(negaLoop->pNds.GetAt(index-1), lstNode, myEps) == 0 ) {
					xx = lstNode->x;  yy = lstNode->y;  zz = lstNode->z;
					negaLoop->pNds.Add(new CNode(t, xx, yy, zz));
					bFind = true;
				}
				if( IsSameNode(last, lstNode, myEps) == 1 && 
					IsSameNode(negaLoop->pNds.GetAt(index-1), fstNode, myEps) == 0 ) {
					xx = fstNode->x;  yy = fstNode->y;  zz = fstNode->z;
					negaLoop->pNds.Add(new CNode(t, xx, yy, zz));
					bFind = true;
				}
				
                index = negaLoop->pNds.GetSize() - 1;
				if(bFind == true)  break;
			}
		}
		face.Add(negaLoop); // add to area set
		
		////// positive area
		CLoop * posiLoop = new CLoop();
		for(t = tmpEdge.GetSize() - 1; t > -1; t--) {
			xx = tmpEdge.GetAt(t)->lstNode->x;
			yy = tmpEdge.GetAt(t)->lstNode->y;
			zz = tmpEdge.GetAt(t)->lstNode->z;
			posiLoop->pNds.Add(new CNode(t, xx, yy, zz));
		}
		t = 0;
		xx = tmpEdge.GetAt(t)->fstNode->x;
		yy = tmpEdge.GetAt(t)->fstNode->y;
		zz = tmpEdge.GetAt(t)->fstNode->z;
		posiLoop->pNds.Add(new CNode(t, xx, yy, zz));
		
		first = posiLoop->pNds.GetAt(0);
		index = 1;
		while (IsSameNode(first, posiLoop->pNds.GetAt(index), myEps) == false)
		{
			index = posiLoop->pNds.GetSize() - 1;
			CNode * last = posiLoop->pNds.GetAt(index);
			for(t = 0; t < faceEdge.GetSize(); t++) {
				bFind = false;
				CNode * fstNode = faceEdge.GetAt(t)->fstNode;
				CNode * lstNode = faceEdge.GetAt(t)->lstNode;
				if( IsSameNode(last, fstNode, myEps) == 1 && 
					IsSameNode(posiLoop->pNds.GetAt(index-1), lstNode, myEps) == 0 ) {
					xx = lstNode->x;  yy = lstNode->y;  zz = lstNode->z;
					posiLoop->pNds.Add(new CNode(t, xx, yy, zz));
					bFind = true;
				}
				if( IsSameNode(last, lstNode, myEps) == 1 && 
					IsSameNode(posiLoop->pNds.GetAt(index-1), fstNode, myEps) == 0 ) {
					xx = fstNode->x;  yy = fstNode->y;  zz = fstNode->z;
					posiLoop->pNds.Add(new CNode(t, xx, yy, zz));
					bFind = true;
				}
				
                index = posiLoop->pNds.GetSize() - 1;
				if(bFind == true)  break;
			}
		}
		face.Add(posiLoop); // add to area set
     }
	 
	 // form new polyhedrons
	 //// form edges in loops
	 for(j = 0; j < face.GetSize(); j++) {
		 CLoop * tmp = face.GetAt(j);
		 CNode * fst = tmp->pNds.GetAt(0);
		 CNode * lst = tmp->pNds.GetAt(tmp->pNds.GetSize()-1);
		 if(IsSameNode(fst, lst, myEps) == false) {
			 tmp->pNds.Add(fst);
		 }
		 for(k = 0; k < tmp->pNds.GetSize()-1; k++) {
			 CEdge * eg = new CEdge();
			 eg->fstNode = tmp->pNds.GetAt(k);
			 eg->lstNode = tmp->pNds.GetAt(k+1);
			 tmp->pEgs.Add(eg);
		 }
	 }
	 
	 CEdge * any = boudry.GetAt(0);
	 CLoop * posiLp, * negaLp;
	 CArray<CEdge*, CEdge*> egSet; // temporay edges set
	 CArray<int, int> loops;
	 bool bexist, bfind, bboudry;
	 for(j = 0; j < face.GetSize(); j++) {
		 CLoop * tmp = face.GetAt(j);
		 for(k = 0; k < tmp->pEgs.GetSize(); k++) {
			 if(IsTwoEdgeSame(any, tmp->pEgs.GetAt(k)) == true) {
				 loops.Add(j);
			 }
		 }
	 }
	 if(loops.GetSize() == 2) {
		 posiLp = face.GetAt(loops.GetAt(0));
		 negaLp = face.GetAt(loops.GetAt(1));
	 }
	 else {
		 AfxMessageBox(_T("Cannot form polyhedrons because areas are not enough! \n Function : IntrOfTriSurfaceAndPolyhedron()."), MB_OK|MB_ICONSTOP);
		 return 0;
	 }

	 ////// positive polyhedron
	 for(j = 0; j < posiLp->pEgs.GetSize(); j++) {
		 bboudry = false;
		 for(k = 0; k < boudry.GetSize(); k++) {
			 if( IsTwoEdgeSame(posiLp->pEgs.GetAt(j), boudry.GetAt(k)) == true ) {
				 bboudry = true;  break;
			 }
		 }
		 if(bboudry == false) { // add the edge when it is not a boundary edge
			 egSet.Add(posiLp->pEgs.GetAt(j));
		 }
	 }
	 pPosi->pLps.Add(posiLp); // add the first area
	 while (egSet.GetSize() != 0) {
		 int lastNum = egSet.GetSize(); // very important, can not use the dynatical size but fixed one
		 for(j = 0; j < lastNum; j++) {
			 CEdge * eg = egSet.GetAt(j);
			 for(k = 0; k < face.GetSize(); k++) { // search an area needed
				 bfind = false;
				 CLoop * lp = face.GetAt(k);
				 for(t = 0; t < lp->pEgs.GetSize(); t++) {
					 if( IsTwoEdgeSame(eg, lp->pEgs.GetAt(t)) == true ) {
						 bfind = true;  break;
					 }
				 }
				 if(bfind == true) { 
					 bexist = false; // check the loop whether it exists
					 for(t = 0; t < pPosi->pLps.GetSize(); t++) {
						 if(IsTwoLoopSame(lp, pPosi->pLps.GetAt(t)) == true) {
							 bexist = true;  break;
						 }
					 }
					 if(bexist == false) { // if not exist, add it 
						 pPosi->pLps.Add(lp);
						 for(t = 0; t < lp->pEgs.GetSize(); t++) {
							 egSet.Add(lp->pEgs.GetAt(t));
						 }
					 }
				 }
			 }
		 }

		 // delete the edges which is shared by two loops
		 for(k = 0; k < egSet.GetSize() - 1; k++) {
			 for(t = k+1; t < egSet.GetSize(); t++) {
				 if( IsTwoEdgeSame(egSet.GetAt(k), egSet.GetAt(t)) == true ) {
					 egSet.GetAt(k)->UseTime = 2;
					 egSet.GetAt(t)->UseTime = 2;
				 }
			 }
		 }
		 for(k = 0; k < egSet.GetSize(); k++) { // is it a boundary edge?
			 for(t = 0; t < boudry.GetSize(); t++) {
				 if( IsTwoEdgeSame(egSet.GetAt(k), boudry.GetAt(t)) == true ) {
					 egSet.GetAt(k)->UseTime = 2;
					 break;
				 }
			 }
		 }
		 for(k = egSet.GetSize() - 1; k > -1; k--) {
			 if(egSet.GetAt(k)->UseTime == 2) {
				 egSet.RemoveAt(k);
			 }
		 }
		 //// removing is done
		 
	 } // while
	 for(j = 0; j < section.GetSize(); j++) { // Don't forget section areas
		 pPosi->pLps.Add(section.GetAt(j));
	 }
	 
	 ////// negative polyhedron
	 for(j = 0; j < negaLp->pEgs.GetSize(); j++) {
		 bboudry = false;
		 for(k = 0; k < boudry.GetSize(); k++) {
			 if( IsTwoEdgeSame(negaLp->pEgs.GetAt(j), boudry.GetAt(k)) == true ) {
				 bboudry = true;  break;
			 }
		 }
		 if(bboudry == false) { // add the edge when it is not a boundary edge
			 egSet.Add(negaLp->pEgs.GetAt(j));
		 }
	 }
	 pNega->pLps.Add(negaLp); // add the first area
	 while (egSet.GetSize() != 0) {
		 int lastNum = egSet.GetSize(); // very important, can not use the dynatical number
		 for(j = 0; j < lastNum; j++) {
			 CEdge * eg = egSet.GetAt(j);
			 for(k = 0; k < face.GetSize(); k++) { // search an area needed
				 bfind = false;
				 CLoop * lp = face.GetAt(k);
				 for(t = 0; t < lp->pEgs.GetSize(); t++) {
					 if( IsTwoEdgeSame(eg, lp->pEgs.GetAt(t)) == true ) {
						 bfind = true;  break;
					 }
				 }
				 if(bfind == true) {
					 bexist = false; // check the loop whether it exists
					 for(t = 0; t < pNega->pLps.GetSize(); t++) {
						 if(IsTwoLoopSame(lp, pNega->pLps.GetAt(t)) == true) {
							 bexist = true;  break;
						 }
					 }
					 if(bexist == false) { // if not exist, add it
						 pNega->pLps.Add(lp);
						 for(t = 0; t < lp->pEgs.GetSize(); t++) {
							 egSet.Add(lp->pEgs.GetAt(t));
						 }
					 }
				 }
			 }
		 }
			 
		 // delete the edges which is shared by two loops
		 for(k = 0; k < egSet.GetSize() - 1; k++) {
			 for(t = k+1; t < egSet.GetSize(); t++) {
				 if( IsTwoEdgeSame(egSet.GetAt(k), egSet.GetAt(t)) == true ) {
					 egSet.GetAt(k)->UseTime = 2;
					 egSet.GetAt(t)->UseTime = 2;
				 }
			 }
		 }
		 for(k = 0; k < egSet.GetSize(); k++) { // is it a boundary edge?
			 for(t = 0; t < boudry.GetSize(); t++) {
				 if( IsTwoEdgeSame(egSet.GetAt(k), boudry.GetAt(t)) == true ) {
					 egSet.GetAt(k)->UseTime = 2;
					 break;
				 }
			 }
		 }
		 for(k = egSet.GetSize() - 1; k > -1; k--) {
			 if(egSet.GetAt(k)->UseTime == 2) {
				 egSet.RemoveAt(k);
			 }
		 }
		 //// removing is done
		 
	 } // while
	 for(j = 0; j < section.GetSize(); j++) { // Don't forget section areas
		 pNega->pLps.Add(section.GetAt(j));
	 }

	 // Begin : copy or remove some data, very important!
	 //// copy
	 CArray<CLoop*, CLoop*> tmp;
	 for(i = 0; i < pPosi->pLps.GetSize(); i++) {
		 tmp.Add(pPosi->pLps.GetAt(i));
	 }
	 pPosi->pLps.RemoveAll();
	 for(i = 0; i < tmp.GetSize(); i++) {
		 CLoop * lp = new CLoop();
		 n = tmp.GetAt(i)->pNds.GetSize();
		 if(IsSameNode(tmp.GetAt(i)->pNds.GetAt(0), tmp.GetAt(i)->pNds.GetAt(n-1), myEps)) {
			 --n; // when the first node and the last are the same
		 }
		 for(j = 0; j < n; j++) {
			 xx = tmp.GetAt(i)->pNds.GetAt(j)->x;
			 yy = tmp.GetAt(i)->pNds.GetAt(j)->y;
			 zz = tmp.GetAt(i)->pNds.GetAt(j)->z;
			 lp->pNds.Add(new CNode(j, xx, yy, zz));
		 }
		 pPosi->pLps.Add(lp);
	 }

	 tmp.RemoveAll();
	 for(i = 0; i < pNega->pLps.GetSize(); i++) {
		 tmp.Add(pNega->pLps.GetAt(i));
	 }
	 pNega->pLps.RemoveAll();
	 for(i = 0; i < tmp.GetSize(); i++) {
		 CLoop * lp = new CLoop();
		 n = tmp.GetAt(i)->pNds.GetSize();
		 if(IsSameNode(tmp.GetAt(i)->pNds.GetAt(0), tmp.GetAt(i)->pNds.GetAt(n-1), myEps)) {
			 --n; // when the first node and the last are the same
		 }
		 for(j = 0; j < tmp.GetAt(i)->pNds.GetSize(); j++) {
			 xx = tmp.GetAt(i)->pNds.GetAt(j)->x;
			 yy = tmp.GetAt(i)->pNds.GetAt(j)->y;
			 zz = tmp.GetAt(i)->pNds.GetAt(j)->z;
			 lp->pNds.Add(new CNode(j, xx, yy, zz));
		 }
		 pNega->pLps.Add(lp);
	 }

	 //// remove
	 for(i = section.GetSize()-1; i > -1; i--) {
		 delete section.GetAt(i);  section.RemoveAt(i);
	 }
	 for(i = boudry.GetSize()-1; i > -1; i--) {
		 delete boudry.GetAt(i);  boudry.RemoveAt(i);
	 }
//	 for(i = face.GetSize()-1; i > -1; i--) { // if not delete, some prolems will happen
//		 delete face.GetAt(i);  face.RemoveAt(i);
//	 }
	 for(i = faceEdge.GetSize()-1; i > -1; i--) {
		 delete faceEdge.GetAt(i);  faceEdge.RemoveAt(i);
	 }
	 delete posiLp;  delete negaLp;
	 section.RemoveAll();  boudry.RemoveAll();  face.RemoveAll();  faceEdge.RemoveAll();  loops.RemoveAll();
	 // End

	 return 2;
	 
}

// 2010.10.19 by Mei Gang, last modified on 2013.5.1
// return 0: when surface does not intersect with polyhedron
// return 1: when there is only 1 new polyhedron created
// return 2: when there are 2 new polyhedrons created
// in short, return the number of new polyhedron created
int CRealModelView::IntrOfTriSurfaceAndPolyhedronFast(CSurf * pRgn, CPohd * pPhd, CPohd * pPosi, CPohd * pNega)
{
	int i, j, k, t, id, n;
	double x[3], y[3], z[3], para[4], d[2];
	double xx, yy, zz, dist;
		
	// Pre-Check 2013.4.30
	if(pPhd->iRgnIntrTr.IsEmpty() == true)  return 0; // Not intersect

	// boundary box test
	CNode * rgnLB = new CNode();  CNode * rgnTR = new CNode();
	CNode * phdLB = new CNode();  CNode * phdTR = new CNode();
	CreatBoundaryBox(pRgn, rgnLB, rgnTR);
	CreatBoundaryBox(pPhd, phdLB, phdTR);
	bool bIntersect = BoundaryBoxIntrTest(rgnLB, rgnTR, phdLB, phdTR);
	if(bIntersect == false) {
		delete rgnLB;  delete rgnTR;  delete phdLB;  delete phdTR;
		return 0;
	}

	// intersect normally
	CArray<CLoop*, CLoop*> section; // cross section
	CArray<CEdge*, CEdge*> boudry; // boundary edges
	CArray<CLoop*, CLoop*> face; // all faces after cutting
	
	for(i = 0; i < pPhd->iRgnIntrTr.GetSize(); i++) {
		id = pPhd->iRgnIntrTr.GetAt(i);
		CTrgl * tr = pRgn->pTrs.GetAt(id);
//	for(i = 0; i < pRgn->pTrs.GetSize(); i++) { // intersection
//		CTrgl * tr = pRgn->pTrs.GetAt(i);
		CLoop * lp = new CLoop();
		if (IntrOfTriangleAndPolyhedron(tr, pPhd, lp) == true ) {
			section.Add(lp);
		}
		else {
			lp->Clear();  delete lp;
		}
	}

	if(section.GetSize() == 0) { // no cross section, not intersect
		delete rgnLB;  delete rgnTR;  delete phdLB;  delete phdTR;
		return 0;
	}

	for(i = 0; i < section.GetSize(); i++) { // save edges
		for(j = 0; j < section.GetAt(i)->pNds.GetSize() - 1; j++) {
			CEdge * eg = new CEdge();
			eg->fstNode = section.GetAt(i)->pNds.GetAt(j);
			eg->lstNode = section.GetAt(i)->pNds.GetAt(j+1);
			boudry.Add(eg);
		}
		CEdge * eg = new CEdge();
		eg->fstNode = section.GetAt(i)->pNds.GetAt(j);
		eg->lstNode = section.GetAt(i)->pNds.GetAt(0);
		boudry.Add(eg);
	}
	for(i = 0; i < boudry.GetSize(); i++) { // find boundary edges
		boudry.GetAt(i)->UseTime = 1;
	}
	for(i = 0; i < boudry.GetSize() - 1; i++) { 
		CNode * fst0 = boudry.GetAt(i)->fstNode;
		CNode * lst0 = boudry.GetAt(i)->lstNode;
		for(j = i + 1; j < boudry.GetSize(); j++) {
			CNode * fst1 = boudry.GetAt(j)->fstNode;
			CNode * lst1 = boudry.GetAt(j)->lstNode;
			if( (IsSameNode(fst0, fst1, myEps) && IsSameNode(lst0, lst1, myEps)) || 
				(IsSameNode(fst0, lst1, myEps) && IsSameNode(lst0, fst1, myEps)) ){
				boudry.GetAt(i)->UseTime = 2;
				boudry.GetAt(j)->UseTime = 2;
			}
		}
	}
	for(i = boudry.GetSize()-1; i > -1; i--) { // remove interior edges, keep boundary ones
		if(boudry.GetAt(i)->UseTime == 2) {
			boudry.GetAt(i)->Clear();
			delete boudry.GetAt(i);  boudry.RemoveAt(i);
		}
	}

	// divide some original areas and form new faces
	CArray<CEdge*, CEdge*> faceEdge; // face edges
    
	for(i = 0; i < pPhd->pLps.GetSize(); i++) {
		for(t = 0; t < 3; t++) {
			x[t] = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->x;
			y[t] = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->y;
			z[t] = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->z;
		}
		EquaOfTrgl(x, y, z, para);
		faceEdge.RemoveAll();

		for(j = 0; j < boudry.GetSize(); j++) { // find which edges are on the face
			if(boudry.GetAt(j)->UseTime == 2)  continue; // not a boundary edge
			x[0] = boudry.GetAt(j)->fstNode->x;
			y[0] = boudry.GetAt(j)->fstNode->y;
			z[0] = boudry.GetAt(j)->fstNode->z;
			x[1] = boudry.GetAt(j)->lstNode->x;
			y[1] = boudry.GetAt(j)->lstNode->y;
			z[1] = boudry.GetAt(j)->lstNode->z;
			d[0] = x[0] * para[0] + y[0] * para[1] + z[0] * para[2] + para[3];
			d[1] = x[1] * para[0] + y[1] * para[1] + z[1] * para[2] + para[3];
			if(fabs(d[0]) < 0.00001 && fabs(d[1]) < 0.00001) {
				faceEdge.Add(boudry.GetAt(j));
			}
		}
		if(faceEdge.IsEmpty()) { // area does't intersect surface
			face.Add(pPhd->pLps.GetAt(i)); // add the original area directly
			continue;
		}
		
        // last modification 2010.12.01
		// Begin : 将同一个面的boundary edges连成一条线, 保存在tmpEdge
		CArray<CEdge*, CEdge*> tmpEdge;

		n = faceEdge.GetSize();
		CEdge * tmp = new CEdge();
		xx = faceEdge.GetAt(0)->fstNode->x;
		yy = faceEdge.GetAt(0)->fstNode->y;
		zz = faceEdge.GetAt(0)->fstNode->z;
		tmp->fstNode = new CNode(0, xx, yy, zz);
		xx = faceEdge.GetAt(0)->lstNode->x;
		yy = faceEdge.GetAt(0)->lstNode->y;
		zz = faceEdge.GetAt(0)->lstNode->z;
		tmp->lstNode = new CNode(1, xx, yy, zz);
		tmpEdge.Add(tmp); // first one

		while (tmpEdge.GetSize() != n) {
			CNode * fstNode = tmpEdge.GetAt(0)->fstNode; // the first node of polyline
			CNode * fstSecond = tmpEdge.GetAt(0)->lstNode; // the first second node
			CNode * lstNode = tmpEdge.GetAt(tmpEdge.GetSize()-1)->lstNode; // the last node of polyline
			CNode * lstSecond = tmpEdge.GetAt(tmpEdge.GetSize()-1)->fstNode; // the last second node
			for(j = 1; j < faceEdge.GetSize(); j++) {
				CNode * fst0 = faceEdge.GetAt(j)->fstNode;
				CNode * lst0 = faceEdge.GetAt(j)->lstNode;
				// there are 4 cases:
				//// 头 head 
				if(IsSameNode(fst0, fstNode, myEps) == 1 && IsSameNode(lst0, fstSecond, myEps) == 0) {
					CEdge * tmp = new CEdge();
					xx = faceEdge.GetAt(j)->fstNode->x;
					yy = faceEdge.GetAt(j)->fstNode->y;
					zz = faceEdge.GetAt(j)->fstNode->z;
					tmp->lstNode = new CNode(1, xx, yy, zz);
					xx = faceEdge.GetAt(j)->lstNode->x;
					yy = faceEdge.GetAt(j)->lstNode->y;
					zz = faceEdge.GetAt(j)->lstNode->z;
					tmp->fstNode = new CNode(0, xx, yy, zz);
					tmpEdge.InsertAt(0, tmp);
				}
				if(IsSameNode(lst0, fstNode, myEps) == 1 && IsSameNode(fst0, fstSecond, myEps) == 0) {
					CEdge * tmp = new CEdge();
					xx = faceEdge.GetAt(j)->fstNode->x;
					yy = faceEdge.GetAt(j)->fstNode->y;
					zz = faceEdge.GetAt(j)->fstNode->z;
					tmp->fstNode = new CNode(0, xx, yy, zz);
					xx = faceEdge.GetAt(j)->lstNode->x;
					yy = faceEdge.GetAt(j)->lstNode->y;
					zz = faceEdge.GetAt(j)->lstNode->z;
					tmp->lstNode = new CNode(1, xx, yy, zz);
					tmpEdge.InsertAt(0, tmp);
				}

				//// 尾 tail
				if(IsSameNode(fst0, lstNode, myEps) == 1 && IsSameNode(lst0, lstSecond, myEps) == 0) {
					CEdge * tmp = new CEdge();
					xx = faceEdge.GetAt(j)->fstNode->x;
					yy = faceEdge.GetAt(j)->fstNode->y;
					zz = faceEdge.GetAt(j)->fstNode->z;
					tmp->fstNode = new CNode(0, xx, yy, zz);
					xx = faceEdge.GetAt(j)->lstNode->x;
					yy = faceEdge.GetAt(j)->lstNode->y;
					zz = faceEdge.GetAt(j)->lstNode->z;
					tmp->lstNode = new CNode(1, xx, yy, zz);
					tmpEdge.Add(tmp);
				}
				if(IsSameNode(lst0, lstNode, myEps) == 1 && IsSameNode(fst0, lstSecond, myEps) == 0) {
					CEdge * tmp = new CEdge();
					xx = faceEdge.GetAt(j)->lstNode->x;
					yy = faceEdge.GetAt(j)->lstNode->y;
					zz = faceEdge.GetAt(j)->lstNode->z;
					tmp->fstNode = new CNode(0, xx, yy, zz);
					xx = faceEdge.GetAt(j)->fstNode->x;
					yy = faceEdge.GetAt(j)->fstNode->y;
					zz = faceEdge.GetAt(j)->fstNode->z;
					tmp->lstNode = new CNode(1, xx, yy, zz);
					tmpEdge.Add(tmp);
				}

			}
		}
		// End
		
		// the original edges before dividing, saved into faceEdge
		faceEdge.RemoveAll(); // clear out
		for(t = 0; t < pPhd->pLps.GetAt(i)->pNds.GetSize()-1; t++) {
			CEdge * eg = new CEdge();
			xx = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->x;
			yy = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->y;
			zz = pPhd->pLps.GetAt(i)->pNds.GetAt(t)->z;
			eg->fstNode = new CNode(0, xx, yy, zz);
			xx = pPhd->pLps.GetAt(i)->pNds.GetAt(t+1)->x;
			yy = pPhd->pLps.GetAt(i)->pNds.GetAt(t+1)->y;
			zz = pPhd->pLps.GetAt(i)->pNds.GetAt(t+1)->z;
			eg->lstNode = new CNode(1, xx, yy, zz);
			faceEdge.Add(eg);
		}
		t = pPhd->pLps.GetAt(i)->pNds.GetSize();
		CEdge * eg = new CEdge(); // last edge
		xx = pPhd->pLps.GetAt(i)->pNds.GetAt(t-1)->x;
		yy = pPhd->pLps.GetAt(i)->pNds.GetAt(t-1)->y;
		zz = pPhd->pLps.GetAt(i)->pNds.GetAt(t-1)->z;
		eg->fstNode = new CNode(0, xx, yy, zz);
		xx = pPhd->pLps.GetAt(i)->pNds.GetAt(0)->x;
		yy = pPhd->pLps.GetAt(i)->pNds.GetAt(0)->y;
		zz = pPhd->pLps.GetAt(i)->pNds.GetAt(0)->z;
		eg->lstNode = new CNode(1, xx, yy, zz);
		faceEdge.Add(eg);

        // form 1 or 2 new faces by dividing the original one
		//// step 1: divide edge
		int original = faceEdge.GetSize(); // number of edges before dividing
		CNode * fstNode = tmpEdge.GetAt(0)->fstNode;
		CNode * lstNode = tmpEdge.GetAt(tmpEdge.GetSize()-1)->lstNode;
		for(t = 0; t < original; t++) {
			CNode * sp0 = faceEdge.GetAt(t)->fstNode;
			CNode * sp1 = faceEdge.GetAt(t)->lstNode;
			if(IsPointOnSegment3D(sp0, sp1, fstNode, myEps) == 2) {
				// add a new edge
				CEdge * eg = new CEdge();
				xx = fstNode->x;  yy = fstNode->y;  zz = fstNode->z;
				eg->fstNode = new CNode(0, xx, yy, zz);
				xx = sp1->x;  yy = sp1->y;  zz = sp1->z;
				eg->lstNode = new CNode(1, xx, yy, zz);
				faceEdge.Add(eg);
				// update the original edge
				sp1->x = fstNode->x;  sp1->y = fstNode->y;  sp1->z = fstNode->z;
			}
			if(IsPointOnSegment3D(sp0, sp1, lstNode, myEps) == 2) {
				// add a new edge
				CEdge * eg = new CEdge();
				xx = lstNode->x;  yy = lstNode->y;  zz = lstNode->z;
				eg->fstNode = new CNode(0, xx, yy, zz);
				xx = sp1->x;  yy = sp1->y;  zz = sp1->z;
				eg->lstNode = new CNode(1, xx, yy, zz);
				faceEdge.Add(eg);
				// update the original edge
				sp1->x = lstNode->x;  sp1->y = lstNode->y;  sp1->z = lstNode->z;
			}
		}

		//// step 2: form new areas
		////// negative area
		CLoop * negaLoop = new CLoop();
		for(t = 0; t < tmpEdge.GetSize(); t++) {
			xx = tmpEdge.GetAt(t)->fstNode->x;
			yy = tmpEdge.GetAt(t)->fstNode->y;
			zz = tmpEdge.GetAt(t)->fstNode->z;
			negaLoop->pNds.Add(new CNode(t, xx, yy, zz));
		}
		t = tmpEdge.GetSize() - 1;
		xx = tmpEdge.GetAt(t)->lstNode->x;
		yy = tmpEdge.GetAt(t)->lstNode->y;
		zz = tmpEdge.GetAt(t)->lstNode->z;
		negaLoop->pNds.Add(new CNode(t, xx, yy, zz));
	
		CNode * first = negaLoop->pNds.GetAt(0);
		int index = 1;
		bool bFind;
		while (IsSameNode(first, negaLoop->pNds.GetAt(index), myEps) == 0)
		{
			index = negaLoop->pNds.GetSize() - 1;
			CNode * last = negaLoop->pNds.GetAt(index);
			for(t = 0; t < faceEdge.GetSize(); t++) {
				bFind = false;
				CNode * fstNode = faceEdge.GetAt(t)->fstNode;
				CNode * lstNode = faceEdge.GetAt(t)->lstNode;
				if( IsSameNode(last, fstNode, myEps) == 1 && 
					IsSameNode(negaLoop->pNds.GetAt(index-1), lstNode, myEps) == 0 ) {
					xx = lstNode->x;  yy = lstNode->y;  zz = lstNode->z;
					negaLoop->pNds.Add(new CNode(t, xx, yy, zz));
					bFind = true;
				}
				if( IsSameNode(last, lstNode, myEps) == 1 && 
					IsSameNode(negaLoop->pNds.GetAt(index-1), fstNode, myEps) == 0 ) {
					xx = fstNode->x;  yy = fstNode->y;  zz = fstNode->z;
					negaLoop->pNds.Add(new CNode(t, xx, yy, zz));
					bFind = true;
				}
				
                index = negaLoop->pNds.GetSize() - 1;
				if(bFind == true)  break;
			}
		}
		face.Add(negaLoop); // add to area set
		
		////// positive area
		CLoop * posiLoop = new CLoop();
		for(t = tmpEdge.GetSize() - 1; t > -1; t--) {
			xx = tmpEdge.GetAt(t)->lstNode->x;
			yy = tmpEdge.GetAt(t)->lstNode->y;
			zz = tmpEdge.GetAt(t)->lstNode->z;
			posiLoop->pNds.Add(new CNode(t, xx, yy, zz));
		}
		t = 0;
		xx = tmpEdge.GetAt(t)->fstNode->x;
		yy = tmpEdge.GetAt(t)->fstNode->y;
		zz = tmpEdge.GetAt(t)->fstNode->z;
		posiLoop->pNds.Add(new CNode(t, xx, yy, zz));
		
		first = posiLoop->pNds.GetAt(0);
		index = 1;
		while (IsSameNode(first, posiLoop->pNds.GetAt(index), myEps) == false)
		{
			index = posiLoop->pNds.GetSize() - 1;
			CNode * last = posiLoop->pNds.GetAt(index);
			for(t = 0; t < faceEdge.GetSize(); t++) {
				bFind = false;
				CNode * fstNode = faceEdge.GetAt(t)->fstNode;
				CNode * lstNode = faceEdge.GetAt(t)->lstNode;
				if( IsSameNode(last, fstNode, myEps) == 1 && 
					IsSameNode(posiLoop->pNds.GetAt(index-1), lstNode, myEps) == 0 ) {
					xx = lstNode->x;  yy = lstNode->y;  zz = lstNode->z;
					posiLoop->pNds.Add(new CNode(t, xx, yy, zz));
					bFind = true;
				}
				if( IsSameNode(last, lstNode, myEps) == 1 && 
					IsSameNode(posiLoop->pNds.GetAt(index-1), fstNode, myEps) == 0 ) {
					xx = fstNode->x;  yy = fstNode->y;  zz = fstNode->z;
					posiLoop->pNds.Add(new CNode(t, xx, yy, zz));
					bFind = true;
				}
				
                index = posiLoop->pNds.GetSize() - 1;
				if(bFind == true)  break;
			}
		}
		face.Add(posiLoop); // add to area set
     }
	 
	 // form new polyhedrons
	 //// form edges in loops
	 for(j = 0; j < face.GetSize(); j++) {
		 CLoop * tmp = face.GetAt(j);
		 CNode * fst = tmp->pNds.GetAt(0);
		 CNode * lst = tmp->pNds.GetAt(tmp->pNds.GetSize()-1);
		 if(IsSameNode(fst, lst, myEps) == false) {
			 tmp->pNds.Add(fst);
		 }
		 for(k = 0; k < tmp->pNds.GetSize()-1; k++) {
			 CEdge * eg = new CEdge();
			 eg->fstNode = tmp->pNds.GetAt(k);
			 eg->lstNode = tmp->pNds.GetAt(k+1);
			 tmp->pEgs.Add(eg);
		 }
	 }
	 
	 CEdge * any = boudry.GetAt(0);
	 CLoop * posiLp, * negaLp;
	 CArray<CEdge*, CEdge*> egSet; // temporay edges set
	 CArray<int, int> loops;
	 bool bexist, bfind, bboudry;
	 for(j = 0; j < face.GetSize(); j++) {
		 CLoop * tmp = face.GetAt(j);
		 for(k = 0; k < tmp->pEgs.GetSize(); k++) {
			 if(IsTwoEdgeSame(any, tmp->pEgs.GetAt(k)) == true) {
				 loops.Add(j);
			 }
		 }
	 }
	 if(loops.GetSize() == 2) {
		 posiLp = face.GetAt(loops.GetAt(0));
		 negaLp = face.GetAt(loops.GetAt(1));
	 }
	 else {
		 AfxMessageBox(_T("Cannot form polyhedrons because areas are not enough! \n Function : IntrOfTriSurfaceAndPolyhedron()."), MB_OK|MB_ICONSTOP);
		 return 0;
	 }

	 ////// positive polyhedron
	 for(j = 0; j < posiLp->pEgs.GetSize(); j++) {
		 bboudry = false;
		 for(k = 0; k < boudry.GetSize(); k++) {
			 if( IsTwoEdgeSame(posiLp->pEgs.GetAt(j), boudry.GetAt(k)) == true ) {
				 bboudry = true;  break;
			 }
		 }
		 if(bboudry == false) { // add the edge when it is not a boundary edge
			 egSet.Add(posiLp->pEgs.GetAt(j));
		 }
	 }
	 pPosi->pLps.Add(posiLp); // add the first area
	 while (egSet.GetSize() != 0) {
		 int lastNum = egSet.GetSize(); // very important, can not use the dynatical size but fixed one
		 for(j = 0; j < lastNum; j++) {
			 CEdge * eg = egSet.GetAt(j);
			 for(k = 0; k < face.GetSize(); k++) { // search an area needed
				 bfind = false;
				 CLoop * lp = face.GetAt(k);
				 for(t = 0; t < lp->pEgs.GetSize(); t++) {
					 if( IsTwoEdgeSame(eg, lp->pEgs.GetAt(t)) == true ) {
						 bfind = true;  break;
					 }
				 }
				 if(bfind == true) { 
					 bexist = false; // check the loop whether it exists
					 for(t = 0; t < pPosi->pLps.GetSize(); t++) {
						 if(IsTwoLoopSame(lp, pPosi->pLps.GetAt(t)) == true) {
							 bexist = true;  break;
						 }
					 }
					 if(bexist == false) { // if not exist, add it 
						 pPosi->pLps.Add(lp);
						 for(t = 0; t < lp->pEgs.GetSize(); t++) {
							 egSet.Add(lp->pEgs.GetAt(t));
						 }
					 }
				 }
			 }
		 }

		 // delete the edges which is shared by two loops
		 for(k = 0; k < egSet.GetSize() - 1; k++) {
			 for(t = k+1; t < egSet.GetSize(); t++) {
				 if( IsTwoEdgeSame(egSet.GetAt(k), egSet.GetAt(t)) == true ) {
					 egSet.GetAt(k)->UseTime = 2;
					 egSet.GetAt(t)->UseTime = 2;
				 }
			 }
		 }
		 for(k = 0; k < egSet.GetSize(); k++) { // is it a boundary edge?
			 for(t = 0; t < boudry.GetSize(); t++) {
				 if( IsTwoEdgeSame(egSet.GetAt(k), boudry.GetAt(t)) == true ) {
					 egSet.GetAt(k)->UseTime = 2;
					 break;
				 }
			 }
		 }
		 for(k = egSet.GetSize() - 1; k > -1; k--) {
			 if(egSet.GetAt(k)->UseTime == 2) {
				 egSet.RemoveAt(k);
			 }
		 }
		 //// removing is done
		 
	 } // while
	 for(j = 0; j < section.GetSize(); j++) { // Don't forget section areas
		 pPosi->pLps.Add(section.GetAt(j));
	 }
	 
	 ////// negative polyhedron
	 for(j = 0; j < negaLp->pEgs.GetSize(); j++) {
		 bboudry = false;
		 for(k = 0; k < boudry.GetSize(); k++) {
			 if( IsTwoEdgeSame(negaLp->pEgs.GetAt(j), boudry.GetAt(k)) == true ) {
				 bboudry = true;  break;
			 }
		 }
		 if(bboudry == false) { // add the edge when it is not a boundary edge
			 egSet.Add(negaLp->pEgs.GetAt(j));
		 }
	 }
	 pNega->pLps.Add(negaLp); // add the first area
	 while (egSet.GetSize() != 0) {
		 int lastNum = egSet.GetSize(); // very important, can not use the dynatical number
		 for(j = 0; j < lastNum; j++) {
			 CEdge * eg = egSet.GetAt(j);
			 for(k = 0; k < face.GetSize(); k++) { // search an area needed
				 bfind = false;
				 CLoop * lp = face.GetAt(k);
				 for(t = 0; t < lp->pEgs.GetSize(); t++) {
					 if( IsTwoEdgeSame(eg, lp->pEgs.GetAt(t)) == true ) {
						 bfind = true;  break;
					 }
				 }
				 if(bfind == true) {
					 bexist = false; // check the loop whether it exists
					 for(t = 0; t < pNega->pLps.GetSize(); t++) {
						 if(IsTwoLoopSame(lp, pNega->pLps.GetAt(t)) == true) {
							 bexist = true;  break;
						 }
					 }
					 if(bexist == false) { // if not exist, add it
						 pNega->pLps.Add(lp);
						 for(t = 0; t < lp->pEgs.GetSize(); t++) {
							 egSet.Add(lp->pEgs.GetAt(t));
						 }
					 }
				 }
			 }
		 }
			 
		 // delete the edges which is shared by two loops
		 for(k = 0; k < egSet.GetSize() - 1; k++) {
			 for(t = k+1; t < egSet.GetSize(); t++) {
				 if( IsTwoEdgeSame(egSet.GetAt(k), egSet.GetAt(t)) == true ) {
					 egSet.GetAt(k)->UseTime = 2;
					 egSet.GetAt(t)->UseTime = 2;
				 }
			 }
		 }
		 for(k = 0; k < egSet.GetSize(); k++) { // is it a boundary edge?
			 for(t = 0; t < boudry.GetSize(); t++) {
				 if( IsTwoEdgeSame(egSet.GetAt(k), boudry.GetAt(t)) == true ) {
					 egSet.GetAt(k)->UseTime = 2;
					 break;
				 }
			 }
		 }
		 for(k = egSet.GetSize() - 1; k > -1; k--) {
			 if(egSet.GetAt(k)->UseTime == 2) {
				 egSet.RemoveAt(k);
			 }
		 }
		 //// removing is done
		 
	 } // while
	 for(j = 0; j < section.GetSize(); j++) { // Don't forget section areas
		 pNega->pLps.Add(section.GetAt(j));
	 }

	 // Begin : copy or remove some data, very important!
	 //// copy
	 CArray<CLoop*, CLoop*> tmp;
	 for(i = 0; i < pPosi->pLps.GetSize(); i++) {
		 tmp.Add(pPosi->pLps.GetAt(i));
	 }
	 pPosi->pLps.RemoveAll();
	 for(i = 0; i < tmp.GetSize(); i++) {
		 CLoop * lp = new CLoop();
		 n = tmp.GetAt(i)->pNds.GetSize();
		 if(IsSameNode(tmp.GetAt(i)->pNds.GetAt(0), tmp.GetAt(i)->pNds.GetAt(n-1), myEps)) {
			 --n; // when the first node and the last are the same
		 }
		 for(j = 0; j < n; j++) {
			 xx = tmp.GetAt(i)->pNds.GetAt(j)->x;
			 yy = tmp.GetAt(i)->pNds.GetAt(j)->y;
			 zz = tmp.GetAt(i)->pNds.GetAt(j)->z;
			 lp->pNds.Add(new CNode(j, xx, yy, zz));
		 }
		 pPosi->pLps.Add(lp);
	 }

	 tmp.RemoveAll();
	 for(i = 0; i < pNega->pLps.GetSize(); i++) {
		 tmp.Add(pNega->pLps.GetAt(i));
	 }
	 pNega->pLps.RemoveAll();
	 for(i = 0; i < tmp.GetSize(); i++) {
		 CLoop * lp = new CLoop();
		 n = tmp.GetAt(i)->pNds.GetSize();
		 if(IsSameNode(tmp.GetAt(i)->pNds.GetAt(0), tmp.GetAt(i)->pNds.GetAt(n-1), myEps)) {
			 --n; // when the first node and the last are the same
		 }
		 for(j = 0; j < tmp.GetAt(i)->pNds.GetSize(); j++) {
			 xx = tmp.GetAt(i)->pNds.GetAt(j)->x;
			 yy = tmp.GetAt(i)->pNds.GetAt(j)->y;
			 zz = tmp.GetAt(i)->pNds.GetAt(j)->z;
			 lp->pNds.Add(new CNode(j, xx, yy, zz));
		 }
		 pNega->pLps.Add(lp);
	 }

	 //// remove
	 for(i = section.GetSize()-1; i > -1; i--) {
		 section.GetAt(i)->Clear();
		 delete section.GetAt(i);  section.RemoveAt(i);
	 }
	 for(i = boudry.GetSize()-1; i > -1; i--) {
		 boudry.GetAt(i)->Clear();
		 delete boudry.GetAt(i);  boudry.RemoveAt(i);
	 }
//	 for(i = face.GetSize()-1; i > -1; i--) { // if not delete, some prolems will happen
//		 delete face.GetAt(i);  face.RemoveAt(i);
//	 }
	 for(i = faceEdge.GetSize()-1; i > -1; i--) {
		 faceEdge.GetAt(i)->Clear();
		 delete faceEdge.GetAt(i);  faceEdge.RemoveAt(i);
	 }
	 delete posiLp;  delete negaLp;
	 section.RemoveAll();  boudry.RemoveAll();  face.RemoveAll();  faceEdge.RemoveAll();  loops.RemoveAll();
	 // End

	 return 2;
	 
}

bool CRealModelView::IsTwoEdgeSame(CEdge * eg1, CEdge * eg2) 
{
	CNode * fst1 = eg1->fstNode;  CNode * lst1 = eg1->lstNode;
	CNode * fst2 = eg2->fstNode;  CNode * lst2 = eg2->lstNode;
	if(IsSameNode(fst1, fst2, myEps) && IsSameNode(lst1, lst2, myEps)) {
		return true;
	}
	if(IsSameNode(fst1, lst2, myEps) && IsSameNode(lst1, fst2, myEps)) {
		return true;
	}
	return false;
}

bool CRealModelView::IsLoopCCW(CLoop * lp)
{
	int i, j, k, n;
	int * id, min, nID[3];
	double * angle;
	double x0, y0, z0, x1, y1, z1, x2, y2, z2;

	n = lp->pNds.GetSize();
	if(n < 3) { // Can't form a polygon
		AfxMessageBox(_T("Can't deposite polygon for points < 3! \n Function : PolygonPartion()."), MB_OK|MB_ICONSTOP);
		return false;
	}
	else {
	    CLoop * tmp = new CLoop();
	    for(i = 0; i < lp->pNds.GetSize(); i++) {
		    tmp->pNds.Add(lp->pNds.GetAt(i));
	    }

		n = tmp->pNds.GetSize();
		id = new int[n];
		angle = new double[n];

		for(i = 0; i < n; i++) {
			id[i] = i;
			x2 = tmp->pNds.GetAt(i)->x;
			y2 = tmp->pNds.GetAt(i)->y;
			z2 = tmp->pNds.GetAt(i)->z;

			if(i == 0) {
				x0 = tmp->pNds.GetAt(1)->x;
				y0 = tmp->pNds.GetAt(1)->y;
				z0 = tmp->pNds.GetAt(1)->z;

				x1 = tmp->pNds.GetAt(n-1)->x;
				y1 = tmp->pNds.GetAt(n-1)->y;
				z1 = tmp->pNds.GetAt(n-1)->z;
			}
			else if(i == n-1) {
				x0 = tmp->pNds.GetAt(0)->x;
				y0 = tmp->pNds.GetAt(0)->y;
				z0 = tmp->pNds.GetAt(0)->z;

				x1 = tmp->pNds.GetAt(n-2)->x;
				y1 = tmp->pNds.GetAt(n-2)->y;
				z1 = tmp->pNds.GetAt(n-2)->z;
			}
			else {
				x0 = tmp->pNds.GetAt(i+1)->x;
				y0 = tmp->pNds.GetAt(i+1)->y;
				z0 = tmp->pNds.GetAt(i+1)->z;

				x1 = tmp->pNds.GetAt(i-1)->x;
				y1 = tmp->pNds.GetAt(i-1)->y;
				z1 = tmp->pNds.GetAt(i-1)->z;
			}
			angle[i] = AngleOfVecters(x0, y0, z0, x2, y2, z2, x1, y1, z1); // 注意编号
		}

		double sum = 0.0;
		for(i = 0; i < n; i++) {
			sum += angle[i];
		}
		if(fabs(sum - (n-2)*180.0) < 0.1)  return true;
		else return false;

		delete [] angle;  delete [] id;
	
	}

}

void CRealModelView::PolygonPartion(CLoop * lp)
{
	int i, j, k, n;
	int * id, min, nID[3];
	double * angle;
	double x0, y0, z0, x1, y1, z1, x2, y2, z2;

	n = lp->pNds.GetSize();
	if(n < 3) { // Can't form a polygon
		AfxMessageBox(_T("Can't triangulate polygon since: points < 3! \n Function : PolygonPartion()."), MB_OK|MB_ICONSTOP);
		return;
	}
	if(n == 3) { // a triangle, save directly
		double x[3], y[3], z[3];
		for(i = 0 ; i < 3; i++) {
			x[i] = lp->pNds.GetAt(i)->x;
			y[i] = lp->pNds.GetAt(i)->y;
			z[i] = lp->pNds.GetAt(i)->z;
		}
		lp->pTrs.Add(new CTrgl(0, x[0], y[0], z[0], 1, x[1], y[1], z[1], 2, x[2], y[2], z[2]));
		return;
	}

	CLoop * tmp = new CLoop();

	for(i = 0; i < lp->pNds.GetSize(); i++) {
		tmp->pNds.Add(lp->pNds.GetAt(i));
	}

	bool bFinish = false;
	while (bFinish != true) {
		n = tmp->pNds.GetSize();
		id = new int[n];
		angle = new double[n];

		for(i = 0; i < n; i++) {
			id[i] = i;
			x2 = tmp->pNds.GetAt(i)->x;
			y2 = tmp->pNds.GetAt(i)->y;
			z2 = tmp->pNds.GetAt(i)->z;

			if(i == 0) {
				x0 = tmp->pNds.GetAt(1)->x;
				y0 = tmp->pNds.GetAt(1)->y;
				z0 = tmp->pNds.GetAt(1)->z;

				x1 = tmp->pNds.GetAt(n-1)->x;
				y1 = tmp->pNds.GetAt(n-1)->y;
				z1 = tmp->pNds.GetAt(n-1)->z;
			}
			else if(i == n-1) {
				x0 = tmp->pNds.GetAt(0)->x;
				y0 = tmp->pNds.GetAt(0)->y;
				z0 = tmp->pNds.GetAt(0)->z;

				x1 = tmp->pNds.GetAt(n-2)->x;
				y1 = tmp->pNds.GetAt(n-2)->y;
				z1 = tmp->pNds.GetAt(n-2)->z;
			}
			else {
				x0 = tmp->pNds.GetAt(i+1)->x;
				y0 = tmp->pNds.GetAt(i+1)->y;
				z0 = tmp->pNds.GetAt(i+1)->z;

				x1 = tmp->pNds.GetAt(i-1)->x;
				y1 = tmp->pNds.GetAt(i-1)->y;
				z1 = tmp->pNds.GetAt(i-1)->z;
			}

			angle[i] = AngleToASideInTrgl(x0, y0, z0, x1, y1, z1, x2, y2, z2);
		}

        prhap(angle, id, n);
	    min = id[n-1];

		i = min;
		x2 = tmp->pNds.GetAt(i)->x;
		y2 = tmp->pNds.GetAt(i)->y;
		z2 = tmp->pNds.GetAt(i)->z;
		
		if(i == 0) {
			x0 = tmp->pNds.GetAt(1)->x;
			y0 = tmp->pNds.GetAt(1)->y;
			z0 = tmp->pNds.GetAt(1)->z;
			
			x1 = tmp->pNds.GetAt(n-1)->x;
			y1 = tmp->pNds.GetAt(n-1)->y;
			z1 = tmp->pNds.GetAt(n-1)->z;
		}
		else if(i == n-1) {
			x0 = tmp->pNds.GetAt(0)->x;
			y0 = tmp->pNds.GetAt(0)->y;
			z0 = tmp->pNds.GetAt(0)->z;
			
			x1 = tmp->pNds.GetAt(n-2)->x;
			y1 = tmp->pNds.GetAt(n-2)->y;
			z1 = tmp->pNds.GetAt(n-2)->z;
		}
		else {
			x0 = tmp->pNds.GetAt(i+1)->x;
			y0 = tmp->pNds.GetAt(i+1)->y;
			z0 = tmp->pNds.GetAt(i+1)->z;
			
			x1 = tmp->pNds.GetAt(i-1)->x;
			y1 = tmp->pNds.GetAt(i-1)->y;
			z1 = tmp->pNds.GetAt(i-1)->z;
		}

		lp->pTrs.Add(new CTrgl(0, x0, y0, z0, 1, x1, y1, z1, 2, x2, y2, z2));
		tmp->pNds.RemoveAt(min);

		delete [] angle;  delete [] id;

		if(tmp->pNds.GetSize() < 3) 
			bFinish = true;
	}

	// update the IDs for all nodes, sometimes, this can be deleted!
	for(i = 0; i < lp->pTrs.GetSize(); i++) {
		CTrgl * tr = lp->pTrs.GetAt(i);
		for(j = 0; j < 3; j++) {
			CNode * tmp1 = new CNode(j, tr->x[j], tr->y[j], tr->z[j]);
			for(k = 0; k < lp->pNds.GetSize(); k++) {
				CNode * tmp2 = lp->pNds.GetAt(k);
				if(IsSameNode(tmp1, tmp2, myEps) == 1) {
					lp->pTrs.GetAt(i)->nID[j] = k;
					break;
				}
			}
		}
	}

}

void CRealModelView::PolygonPartionExtended(CLoop * lp)
{
	int i, j, k, n, index;
	int * id, min, nID[3];
	double * angle;
	double x, y, z, x0, y0, z0, x1, y1, z1, x2, y2, z2;
	CLoop * tmp = new CLoop();

	for(i = 0; i < lp->pNds.GetSize(); i++) {
		tmp->pNds.Add(lp->pNds.GetAt(i));
	}

	bool bFinish = false;  bool bCreate = false;  bool bInside = false;
	while (bFinish != true) {
		n = tmp->pNds.GetSize(); 
		id = new int[n];
		angle = new double[n];
		
		for(i = 0; i < n; i++) {
			id[i] = i;
			x1 = tmp->pNds.GetAt(i)->x;
			y1 = tmp->pNds.GetAt(i)->y;
			z1 = tmp->pNds.GetAt(i)->z;
			
			if(i == 0) {
				x2 = tmp->pNds.GetAt(1)->x;
				y2 = tmp->pNds.GetAt(1)->y;
				z2 = tmp->pNds.GetAt(1)->z;
				
				x0 = tmp->pNds.GetAt(n-1)->x;
				y0 = tmp->pNds.GetAt(n-1)->y;
				z0 = tmp->pNds.GetAt(n-1)->z;
			}
			else if(i == n-1) {
				x2 = tmp->pNds.GetAt(0)->x;
				y2 = tmp->pNds.GetAt(0)->y;
				z2 = tmp->pNds.GetAt(0)->z;
				
				x0 = tmp->pNds.GetAt(n-2)->x;
				y0 = tmp->pNds.GetAt(n-2)->y;
				z0 = tmp->pNds.GetAt(n-2)->z;
			}
			else {
				x2 = tmp->pNds.GetAt(i+1)->x;
				y2 = tmp->pNds.GetAt(i+1)->y;
				z2 = tmp->pNds.GetAt(i+1)->z;
				
				x0 = tmp->pNds.GetAt(i-1)->x;
				y0 = tmp->pNds.GetAt(i-1)->y;
				z0 = tmp->pNds.GetAt(i-1)->z;
			}
			
			angle[i] = AngleOfVecters(x0, y0, z0, x1, y1, z1, x2, y2, z2);
		}

	// 保存 角度 by Mei Gang
	/////////////////////////////////////////////////////////////////////////////////////
	/*
	int NumOfNodes;
	CString fnNode;
	if(AfxMessageBox("Save Angles?", MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg1(false,NULL,NULL,OFN_HIDEREADONLY, "Data File(*.txt)|*.txt", NULL);
	if(Dlg1.DoModal() == IDOK) {
		fnNode = Dlg1.GetPathName(); 
		if(fnNode.Right(4)!=".txt")   fnNode += ".txt";
		ofstream fin3(fnNode);

		NumOfNodes = tmp->pNds.GetSize();
		for(i = 0; i < NumOfNodes; i++){
			x = tmp->pNds.GetAt(i)->x;
			y = tmp->pNds.GetAt(i)->y;
			z = tmp->pNds.GetAt(i)->z;
			fin3 << tmp->pNds.GetAt(i)->id <<"   "<<  x <<"   "<< y <<"   "<< z 
				 <<"  : " << angle[i] << endl;
		}
		
		fin3.close();
	}
	*/
	//////////////////////////////////////////////////////////////////////////////////////
		
		prhap(angle, id, n);
		index = 0;
		bCreate = false;
		while (bCreate != true) {
			i = id[index];
			x2 = tmp->pNds.GetAt(i)->x;
			y2 = tmp->pNds.GetAt(i)->y;
			z2 = tmp->pNds.GetAt(i)->z;
			nID[2] = i;
			
			if(i == 0) {
				x0 = tmp->pNds.GetAt(1)->x;
				y0 = tmp->pNds.GetAt(1)->y;
				z0 = tmp->pNds.GetAt(1)->z;
				
				x1 = tmp->pNds.GetAt(n-1)->x;
				y1 = tmp->pNds.GetAt(n-1)->y;
				z1 = tmp->pNds.GetAt(n-1)->z;
				nID[0] = 1;  nID[1] = n-1;
			}
			else if(i == n-1) {
				x0 = tmp->pNds.GetAt(0)->x;
				y0 = tmp->pNds.GetAt(0)->y;
				z0 = tmp->pNds.GetAt(0)->z;
				
				x1 = tmp->pNds.GetAt(n-2)->x;
				y1 = tmp->pNds.GetAt(n-2)->y;
				z1 = tmp->pNds.GetAt(n-2)->z;
				nID[0] = 0;  nID[1] = n-2;
			}
			else {
				x0 = tmp->pNds.GetAt(i+1)->x;
				y0 = tmp->pNds.GetAt(i+1)->y;
				z0 = tmp->pNds.GetAt(i+1)->z;
				
				x1 = tmp->pNds.GetAt(i-1)->x;
				y1 = tmp->pNds.GetAt(i-1)->y;
				z1 = tmp->pNds.GetAt(i-1)->z;
				nID[0] = i+1;  nID[1] = i-1;
			}
			
			bInside = false;
			for(j = 0; j < tmp->pNds.GetSize(); j++) {
				if(j == nID[0] || j == nID[1] || j == nID[2]) {
					continue;
				}
				x = tmp->pNds.GetAt(j)->x;  y = tmp->pNds.GetAt(j)->y;
				
				// 0-Node is in triangle;  1-Node is on vertex;  2-Node is on edge;  3-Node is outside of the triangle
				if(IsNdInTrgl(x, y, x0, y0, x1, y1, x2, y2) == 0 ||
					IsNdInTrgl(x, y, x0, y0, x1, y1, x2, y2) == 2 ) { // 1-Node is on vertex is OK
					bInside = true;  break;
				}
			}
			
			if(bInside == false) {
				lp->pTrs.Add(new CTrgl(0, x0, y0, z0, 1, x1, y1, z1, 2, x2, y2, z2));
				bCreate = true;  break;
			}
			else {
			//	index--;
				index++;
			}
			//  if(index > 0) {AfxMessageBox("Attention!");}
		}
		
		tmp->pNds.RemoveAt(id[index]);
		
		delete [] angle;  delete [] id;
		
		if(tmp->pNds.GetSize() < 3) 
			bFinish = true;
	}
	
	// update the IDs for all nodes, sometimes, this can be deleted!
	for(i = 0; i < lp->pTrs.GetSize(); i++) {
		CTrgl * tr = lp->pTrs.GetAt(i);
		for(j = 0; j < 3; j++) {
			CNode * tmp1 = new CNode(j, tr->x[j], tr->y[j], tr->z[j]);
			for(k = 0; k < lp->pNds.GetSize(); k++) {
				CNode * tmp2 = lp->pNds.GetAt(k);
				if(IsSameNode(tmp1, tmp2, myEps) == 1) {
					lp->pTrs.GetAt(i)->nID[j] = k;
					break;
				}
			}
		}
	}

}

void CRealModelView::PolygonPartion3D(CLoop * lp)
{
	// Pre-check
	int M = lp->pNds.GetSize();
	CNode* na = lp->pNds.GetAt(0);
	CNode* nb = lp->pNds.GetAt(M-1);
	if( fabs(na->x - nb->x) < myEps && 
		fabs(na->y - nb->y) < myEps && 
		fabs(na->z - nb->z) < myEps ){
			lp->pNds.RemoveAt(M-1);
			AfxMessageBox(_T("PolygonPartion3D(CLoop * lp) \n\nFirst node == last node. Remove last one !"));
	}

	if(lp->pNds.GetSize() < 3) { // Can't form a polygon
		AfxMessageBox(_T("Can't divide polygon : points < 3! \n Function : PolygonPartion()."), MB_OK|MB_ICONSTOP);
	//	exit(0);
		return;
	}

	// Step 1: compute local coordinates system and transfer
	// change pIntrTmp and pTrgl to XY plane
	int i, j, n, nID[3];
	double * x, * y, * z, * a, avgx = 0.0, avgy = 0.0, avgz = 0.0;
    CCordnt * tCrdt = new CCordnt();
	CTrgl * pTrgl = new CTrgl();

	n = lp->pNds.GetSize();
	x = new double[n];  y = new double[n];  z = new double[n];
	for(i = 0; i < n; i++) {
		x[i] = lp->pNds.GetAt(i)->x;
		y[i] = lp->pNds.GetAt(i)->y;
		z[i] = lp->pNds.GetAt(i)->z;
		avgx += x[i];  avgy += y[i];  avgz += z[i];
	}
	avgx /= n;  avgy /= n;  avgz /= n;
	pTrgl->x[0] = x[0];  pTrgl->y[0] = y[0];  pTrgl->z[0] = z[0];
	pTrgl->x[1] = x[1];  pTrgl->y[1] = y[1];  pTrgl->z[1] = z[1];
	pTrgl->x[2] = avgx;  pTrgl->y[2] = avgy;  pTrgl->z[2] = avgz;
	GetNewCoordt(pTrgl, tCrdt);

	CLoop * newPoly = new CLoop();
	for(i = 0; i < n; i++) { // transform
		x[i] -= tCrdt->u;  y[i] -= tCrdt->v;  z[i] -= tCrdt->w;
		newPoly->pNds.Add(new CNode(i, x[i], y[i], z[i]));
	}
	// 旋转前，矩阵求逆. very important
	a = new double[9];
	a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
	a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
	a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;
    Dcinv(a, 3);

	for(i = 0; i < n; i++) {
		newPoly->pNds.GetAt(i)->x = x[i] * a[0] + y[i] * a[1] + z[i] * a[2];
		newPoly->pNds.GetAt(i)->y = x[i] * a[3] + y[i] * a[4] + z[i] * a[5];
		newPoly->pNds.GetAt(i)->z = x[i] * a[6] + y[i] * a[7] + z[i] * a[8];
	}
	delete [] x;  delete [] y;  delete [] z;  delete [] a;

	// Step 2 : Check
	// 判断环是否逆时针排列
	n = newPoly->pNds.GetSize();
	CNode * p = new CNode[newPoly->pNds.GetSize()];
	for(i = 0; i < newPoly->pNds.GetSize(); i++) {
		p[i].x = newPoly->pNds.GetAt(i)->x;
		p[i].y = newPoly->pNds.GetAt(i)->y;
	}
	if(IsCCwize(p, newPoly->pNds.GetSize()) == false) { // 改变环绕向
		for(int j = 0; j < n; j++) {
			newPoly->pNds.GetAt(j)->x = p[n-1-j].x;
			newPoly->pNds.GetAt(j)->y = p[n-1-j].y;
		}
	}
	delete [] p;

	// Step 3 : polygon partion and save 
//	PolygonPartion(newPoly); // Wrong, cause error
	PolygonPartionExtended(newPoly); // right, in most case

	for(i = 0; i < newPoly->pTrs.GetSize(); i++) {
		CTrgl* tr = newPoly->pTrs.GetAt(i);
		for(j = 0; j < 3; j++)  nID[j] = tr->nID[j];
		
		CTrgl* newTr = new CTrgl(i, nID[0], nID[1], nID[2]);

		for(j = 0; j < 3; j++) {
			nID[j] = newTr->nID[j];
			newTr->x[j] = lp->pNds.GetAt(nID[j])->x;
			newTr->y[j] = lp->pNds.GetAt(nID[j])->y;
			newTr->z[j] = lp->pNds.GetAt(nID[j])->z;
		}

		lp->pTrs.Add(newTr);
	}

	delete pTrgl;  delete tCrdt;  delete newPoly;

}

void CRealModelView::PolygonPartion3D_WithCheck(CLoop * lp)
{
	// Pre-check
	int M = lp->pNds.GetSize();
	CNode* na = lp->pNds.GetAt(0);
	CNode* nb = lp->pNds.GetAt(M-1);
	if( fabs(na->x - nb->x) < myEps && 
		fabs(na->y - nb->y) < myEps && 
		fabs(na->z - nb->z) < myEps ){
			lp->pNds.RemoveAt(M-1);
			AfxMessageBox(_T("PolygonPartion3D(CLoop * lp) \n\nFirst node == last node. Remove last one !"));
	}

	if(lp->pNds.GetSize() < 3) { // Can't form a polygon
		AfxMessageBox(_T("Can't divide polygon : points < 3! \n Function : PolygonPartion()."), MB_OK|MB_ICONSTOP);
	//	exit(0);
		return;
	}

	// Step 1: compute local coordinates system and transfer
	// change pIntrTmp and pTrgl to XY plane
	int i, j, n, nID[3];
	double * x, * y, * z, * a, avgx = 0.0, avgy = 0.0, avgz = 0.0;
    CCordnt * tCrdt = new CCordnt();
	CTrgl * pTrgl = new CTrgl();

	n = lp->pNds.GetSize();
	x = new double[n];  y = new double[n];  z = new double[n];
	for(i = 0; i < n; i++) {
		x[i] = lp->pNds.GetAt(i)->x;
		y[i] = lp->pNds.GetAt(i)->y;
		z[i] = lp->pNds.GetAt(i)->z;
		avgx += x[i];  avgy += y[i];  avgz += z[i];
	}
	avgx /= n;  avgy /= n;  avgz /= n;
	pTrgl->x[0] = x[0];  pTrgl->y[0] = y[0];  pTrgl->z[0] = z[0];
	pTrgl->x[1] = x[1];  pTrgl->y[1] = y[1];  pTrgl->z[1] = z[1];
	pTrgl->x[2] = avgx;  pTrgl->y[2] = avgy;  pTrgl->z[2] = avgz;
	GetNewCoordt(pTrgl, tCrdt);

	CLoop * newPoly = new CLoop();
	for(i = 0; i < n; i++) { // transform
		x[i] -= tCrdt->u;  y[i] -= tCrdt->v;  z[i] -= tCrdt->w;
		newPoly->pNds.Add(new CNode(i, x[i], y[i], z[i]));
	}
	// 旋转前，矩阵求逆. very important
	a = new double[9];
	a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
	a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
	a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;
    Dcinv(a, 3);

	for(i = 0; i < n; i++) {
		newPoly->pNds.GetAt(i)->x = x[i] * a[0] + y[i] * a[1] + z[i] * a[2];
		newPoly->pNds.GetAt(i)->y = x[i] * a[3] + y[i] * a[4] + z[i] * a[5];
		newPoly->pNds.GetAt(i)->z = x[i] * a[6] + y[i] * a[7] + z[i] * a[8];
	}
	delete [] x;  delete [] y;  delete [] z;  delete [] a;

	// Step 2 : Check
	// 判断环是否逆时针排列
	n = newPoly->pNds.GetSize();
	CNode * p = new CNode[newPoly->pNds.GetSize()];
	for(i = 0; i < newPoly->pNds.GetSize(); i++) {
		p[i].x = newPoly->pNds.GetAt(i)->x;
		p[i].y = newPoly->pNds.GetAt(i)->y;
	}
	if(IsCCwize(p, newPoly->pNds.GetSize()) == false) { // 改变环绕向
		for(int j = 0; j < n; j++) {
			newPoly->pNds.GetAt(j)->x = p[n-1-j].x;
			newPoly->pNds.GetAt(j)->y = p[n-1-j].y;
		}
	}
	delete [] p;

	// Step 3 : polygon partion and save 
//	PolygonPartion(newPoly); // Wrong, cause error
	PolygonPartionExtended(newPoly); // right, in most case

	// Normal of parent triangle
	double parentEqua[4], sonEqua[4];

	CTrgl * pParent = lp->pParent;

	EquaOfTrgl(pParent->x, pParent->y, pParent->z, parentEqua);

	for(i = 0; i < newPoly->pTrs.GetSize(); i++) {
		CTrgl* tr = newPoly->pTrs.GetAt(i);

		for(j = 0; j < 3; j++)  nID[j] = tr->nID[j];
		
		CTrgl* newTr = new CTrgl(i, nID[0], nID[1], nID[2]);

		for(j = 0; j < 3; j++) {
			nID[j] = newTr->nID[j];
			newTr->x[j] = lp->pNds.GetAt(nID[j])->x;
			newTr->y[j] = lp->pNds.GetAt(nID[j])->y;
			newTr->z[j] = lp->pNds.GetAt(nID[j])->z;
		}

		// With Check, very important, compare normal
		EquaOfTrgl(newTr->x, newTr->y, newTr->z, sonEqua);

		// if normals are opposite, change
		if( (parentEqua[0] * sonEqua[0] < 0.0 ) || 
			(parentEqua[1] * sonEqua[1] < 0.0 ) ||
			(parentEqua[2] * sonEqua[2] < 0.0 ) ){

			for(j = 0; j < 3; j++) { // Reverse
				newTr->nID[j] = tr->nID[3-j];
		    }
		}

		lp->pTrs.Add(newTr);
	}

	delete pTrgl;  delete tCrdt;  delete newPoly;

}

//三角形中，一边（x0,x1）所对的内角的余弦值
double CRealModelView::AngleToASideInTrgl(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2)
{
	double a, b, c, ca;
	c = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) + (z1-z0)*(z1-z0);
	a = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1);
	b = (x2-x0)*(x2-x0) + (y2-y0)*(y2-y0) + (z2-z0)*(z2-z0);
    ca = (a + b - c) / 2 / sqrt(a) / sqrt(b);
	return ca;
}

//三点0，1，2组成向量01，12，求其夹角, 不是余弦值
//根据折线段拐向判断
double CRealModelView::AngleOfVecters(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2)
{
	double a, b, c, dire, ca;
	
	a = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) + (z1-z0)*(z1-z0); // 向量01的二模
	b = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1); // 向量12的二模
	c = (x1-x0)*(x2-x1) + (y1-y0)*(y2-y1) + (z1-z0)*(z2-z1); // 01 与 12的点积

	double u1, u2, u3, v1, v2, v3;
	u1 = x2 - x0;  u2 = y2 - y0;  u3 = z2 - z0;
	v1 = x1 - x0;  v2 = y1 - y0;  v3 = z1 - z0;
	dire = (u2*v3 + u3*v2) + (u3*v1-u1*v3) +(u1*v2-u2*v1);

    ca = c / sqrt(a) / sqrt(b);

#ifndef EGC
	ca = acos(ca) * 180 / 3.1415926;
#else
	ca = acos(ca.doubleValue()) * 180 / 3.1415926;
#endif

	if(dire < 0.0) {
		ca = 180 - ca;
	}
	else {
		ca = 180 + ca;
	}

	return ca;
}

// by Mei Gang
// last modified 2010.9.1
bool CRealModelView::whetherTriNeedDivide(CTrgl * tr, double length)
{
	double x[3], y[3], z[3];
	double dist;

	for(int i = 0; i < 3; i++) {
		x[i] = tr->x[i];  y[i] = tr->y[i];  z[i] = tr->z[i];
	}
	dist = (x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]) + (z[0] - z[1]) * (z[0] - z[1]);
	dist = sqrt(dist);
	if(dist > 2 * length) {
		return true;
	}
	dist = (x[1] - x[2]) * (x[1] - x[2]) + (y[1] - y[2]) * (y[1] - y[2]) + (z[1] - z[2]) * (z[1] - z[2]);
	dist = sqrt(dist);
	if(dist > 2 * length) {
		return true;
	}
	dist = (x[0] - x[2]) * (x[0] - x[2]) + (y[0] - y[2]) * (y[0] - y[2]) + (z[0] - z[2]) * (z[0] - z[2]);
	dist = sqrt(dist);
	if(dist > 2 * length) {
		return true;
	}

	return false;

}

// 2010.11.26 by Mei Gang at University of Freiburg, Germany
// after reading a triangle surface / plane, copy all details to each node and element
// this is very important to prepare for the intersection of surfaces in both 2D and 3D
void CRealModelView::CopyDetailsForSurface(CSurf* rgn)
{
	int i, j, k, m;

	//将每个三角形节点坐标保存到各自三角形中 2009.11.02
	int id;
	for(i = 0; i < rgn->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			id = rgn->pTrs.GetAt(i)->nID[j];
			rgn->pTrs.GetAt(i)->x[j] = rgn->pNds.GetAt(id)->x;
			rgn->pTrs.GetAt(i)->y[j] = rgn->pNds.GetAt(id)->y;
			rgn->pTrs.GetAt(i)->z[j] = rgn->pNds.GetAt(id)->z;
		}
	}

	//记录经过每个点的三角形ID。 CArray<int, int>sPassedTrgl;//经过该点的三角形  2009.11.11
	for(i = 0; i < rgn->pTrs.GetSize(); i++) {
		for(j = 0; j < 3; j++) {
			id = rgn->pTrs.GetAt(i)->nID[j];
			rgn->pNds.GetAt(id)->sPsdTrgl.Add(i);
		}
	}

	//记录每个三角形的邻域三角形，不包括自身   	CArray<int, int>iRgnPassTr;//相邻单元号  2009.11.11
	CArray<int, int>idset;
	bool bexist;
	for(i = 0; i < rgn->pTrs.GetSize(); i++) {
		idset.RemoveAll();
		for(j = 0; j < 3; j++) {
			id = rgn->pTrs.GetAt(i)->nID[j];
			for(k = 0; k < rgn->pNds.GetAt(id)->sPsdTrgl.GetSize(); k++) {
				bexist = false;
				for(m = 0; m < idset.GetSize(); m++) {
					if( idset.GetAt(m) == rgn->pNds.GetAt(id)->sPsdTrgl.GetAt(k)) { bexist = true;  break; }
				}
				if( !bexist && rgn->pNds.GetAt(id)->sPsdTrgl.GetAt(k) != i)  
					idset.Add( rgn->pNds.GetAt(id)->sPsdTrgl.GetAt(k) );
			}

		}
		for(j = 0; j < idset.GetSize(); j++) {
			rgn->pTrs.GetAt(i)->iRgnPassTr.Add(idset.GetAt(j));
		}
	}

}

void CRealModelView::RenderBlockGroups()
{
	// delete existing display lists, very important
//	glDeleteLists(1, m_aSurfs.GetSize());
//	glDeleteLists(1, m_aBlks.GetSize());
	
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glClearDepth(1.0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_iPhdNumBefAdd = m_aPhds.GetSize(); // original number before adding a new surface, important.
	m_iBlkNumBefAdd = m_aBlks.GetSize(); // 

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}

	GLuint allBlockGroups;
	allBlockGroups = 1 + m_aSurfs.GetSize();
//	glCompileBlockGroupList(pCrt, allBlockGroups, 0);  // new rendering function
	glCompileBlockGroupListBest(pCrt, allBlockGroups);  
//	glCompileBlockGroupListWithArray(pCrt, allBlockGroups, 0);  // new rendering function

	m_bOpenGL = 1;
	Invalidate(TRUE);

}

void CRealModelView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CRenderView::OnTimer(nIDEvent);
}

void CRealModelView::OnImageZoomIn()
{
	SceneZoom -= (50.0 /500.0) * SceneZoom;
	if(SceneZoom>=500.0)  SceneZoom=500.0;
	else if(SceneZoom<=0.01)  SceneZoom=0.01;
	Invalidate();
}

void CRealModelView::OnImageZoomOut()
{
	SceneZoom += (50.0 /500.0) * SceneZoom;
	if(SceneZoom>=500.0)  SceneZoom=500.0;
	else if(SceneZoom<=0.01)  SceneZoom=0.01;
	Invalidate();
}

void CRealModelView::OnImageZoomFit()
{
	// TODO: Add your command handler code here
	m_operator = OPERATOR_VIEW_ROTATE;
	m_uShowMode = SHOW_MODE_LINE;

	ScenePos.x=0.0;
	ScenePos.y=0.0;
	ScenePos.z=0.0;
	
	SceneAngle.x=90.0;
	SceneAngle.y=0.0;
	SceneAngle.z=0.0;

	SceneZoom = 1.25;
	SceneDepth = 300.0;

	SceneZoom -= (5 * 50.0 /500.0) * SceneZoom;
	if(SceneZoom>=500.0)  SceneZoom=500.0;
	else if(SceneZoom<=0.01)  SceneZoom=0.01;
	Invalidate();

	AddNewViewPara();
}

void CRealModelView::OnImageZoomOriginal()
{
	SceneZoom = 1.25;
	Invalidate();
}


void CRealModelView::OnImageInitialize()
{
	// Set default value
	m_operator = OPERATOR_VIEW_ROTATE;
	m_uShowMode = SHOW_MODE_LINE;

	ScenePos.x=0.0;
	ScenePos.y=0.0;
	ScenePos.z=0.0;
	
	SceneAngle.x=0.0;
	SceneAngle.y=0.0;
	SceneAngle.z=0.0;

	SceneZoom = 1.25;
	SceneDepth = 300.0;
	Invalidate();
}


void CRealModelView::OnGetCoordinateSystem()
{
	// open polyhedrons
	ReadData();
	pCrt = new CCordnt();
    pCrt = GetNewCordntSystemBlockGroup();
	GLuint allBlockGroups;
	allBlockGroups = glGenLists(1);
	CompileBlockGroupList(pCrt, allBlockGroups, 0);
	glCompileBlockGroupList(pCrt, allBlockGroups, 1); // IF == 1

	m_bOpenGL = 1;
	Invalidate(TRUE);

	SaveDisplayControls();
}

void CRealModelView::OnSetCoordinateSystem()
{
	if(pCrt == NULL)  pCrt = new CCordnt();
	RestoreDisplayControls();
}

void CRealModelView::OnOpenBlockGroup()
{
	On3DModelOpenBlockGroup();
}

void CRealModelView::OnAddSurface()
{
//	CSurf * pSurf = new CSurf();
	pSurf = new CSurf();
	ReadDataSurface(pSurf); // new format
	m_aSurfs.Add(pSurf);

	// Creat color
	for(int i = 0; i < m_aSurfs.GetSize(); i++) {
		m_aSurfs.GetAt(i)->ID = i;
		CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	}
	pCrt = new CCordnt();
    pCrt = GetNewCordntSystem(pSurf);
	int nregion = m_aSurfs.GetSize();
	int * list = new int[nregion];
	for(int j = 0; j < nregion; j++){
		list[j] = j+1;
		CSurf * Surf = m_aSurfs.GetAt(j);
		glCompileTinList(Surf, pCrt, list[j], 0); // edge and elements, new
	}

	// Display block group
	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	GLuint allBlockGroups;
	allBlockGroups = glGenLists(1);
	glCompileBlockGroupList(pCrt, allBlockGroups, 1); // IF == 1

	m_bOpenGL = 1;
	Invalidate(TRUE);
	delete[] list;
}

void CRealModelView::OnIntersectAndCut()
{
	// TODO: Add your command handler code here
	int nSurf = m_aSurfs.GetSize();
	int nBlock = m_aBlks.GetSize();

	if(nBlock == 0) {
		switch (nSurf)
	    {
	    case 0 :
		{
			AfxMessageBox(_T("No surface available ! Please add at least 1 surface !"));
			break;
		}
		case 1 :
		{
			AfxMessageBox(_T("Only 1 surface available! Please add another surface !"));
			break;
		}
		case 2 :
		{
			AfxMessageBox(_T("Two (2) surfaces available. Intersection will be done !"));
			On3DModelIntersect();
			break;
		}
		default : {
			AfxMessageBox(_T("More than 2 surfaces available. Only 2 surfaces are needed !"));
		}
	  }

	}
	else {
	    switch (nSurf)
	    {
	    case 0 :
		{
			AfxMessageBox(_T("No newly added surface, Please add a new surface ! "));
			break;
		}
		case 1 :
		{
			AfxMessageBox(_T("A newly added surfaces available. Intersecting and cutting will be done !"));
			On3DModelIntersectAndCut();
			break;
		}
		default :
		{
			AfxMessageBox(_T(" More than 1 new surface are added ! Only 1 new surface is needed !"));
		}
	    }
	}

}

void CRealModelView::OnSaveBlockGroup()
{
	SaveBlockGroups();
}

// Position view
void CRealModelView::OnPositionFrontView()
{
	// TODO: Add your command handler code here
	m_operator = OPERATOR_VIEW_ROTATE;
	m_uShowMode = SHOW_MODE_LINE;

	ScenePos.x=0.0;
	ScenePos.y=0.0;
	ScenePos.z=0.0;
	
	SceneAngle.x=0.0;
	SceneAngle.y=0.0;
	SceneAngle.z=0.0;

	SceneZoom = 1.25;
	SceneDepth = 300.0;
	Invalidate();

	AddNewViewPara();
}

void CRealModelView::OnPositionBackView()
{
	// TODO: Add your command handler code here
	m_operator = OPERATOR_VIEW_ROTATE;
	m_uShowMode = SHOW_MODE_LINE;

	ScenePos.x = 0.0;
	ScenePos.y = 0.0;
	ScenePos.z = 0.0;
	
	SceneAngle.x = 0.0;
	SceneAngle.y = 0.0;
	SceneAngle.z = 180.0;

	SceneZoom = 1.25;
	SceneDepth = 300.0;
	Invalidate();

	AddNewViewPara();
}

void CRealModelView::OnPositionLeftView()
{
	// TODO: Add your command handler code here
	m_operator = OPERATOR_VIEW_ROTATE;
	m_uShowMode = SHOW_MODE_LINE;

	ScenePos.x=0.0;
	ScenePos.y=0.0;
	ScenePos.z=0.0;
	
	SceneAngle.x=0.0;
	SceneAngle.y=0.0;
	SceneAngle.z=90.0;

	SceneZoom = 1.25;
	SceneDepth = 300.0;
	Invalidate();

	AddNewViewPara();
}

void CRealModelView::OnPositionRightView()
{
	// TODO: Add your command handler code here
	m_operator = OPERATOR_VIEW_ROTATE;
	m_uShowMode = SHOW_MODE_LINE;

	ScenePos.x=0.0;
	ScenePos.y=0.0;
	ScenePos.z=0.0;
	
	SceneAngle.x=0.0;
	SceneAngle.y=0.0;
	SceneAngle.z=-90.0;

	SceneZoom = 1.25;
	SceneDepth = 300.0;
	Invalidate();

	AddNewViewPara();
}

void CRealModelView::OnPositionTopView()
{
	// TODO: Add your command handler code here
	m_operator = OPERATOR_VIEW_ROTATE;
	m_uShowMode = SHOW_MODE_LINE;

	ScenePos.x=0.0;
	ScenePos.y=0.0;
	ScenePos.z=0.0;
	
	SceneAngle.x=90.0;
	SceneAngle.y=0.0;
	SceneAngle.z=0.0;

	SceneZoom = 1.25;
	SceneDepth = 300.0;
	Invalidate();

	AddNewViewPara();

}

void CRealModelView::OnPositionBottomView()
{
	// TODO: Add your command handler code here
	m_operator = OPERATOR_VIEW_ROTATE;
	m_uShowMode = SHOW_MODE_LINE;

	ScenePos.x=0.0;
	ScenePos.y=0.0;
	ScenePos.z=0.0;
	
	SceneAngle.x=-90.0;
	SceneAngle.y=0.0;
	SceneAngle.z=0.0;

	SceneZoom = 1.25;
	SceneDepth = 300.0;
	Invalidate();

	AddNewViewPara();
}
// End Position view

void CRealModelView::OnAxisXLeft()
{
	// TODO: Add your command handler code here
	SceneAngle.x -= 15.0;
	Invalidate();

	AddNewViewPara();
}

void CRealModelView::OnAxisXRight()
{
	// TODO: Add your command handler code here
	SceneAngle.x += 15.0;
	Invalidate();

	AddNewViewPara();
}

void CRealModelView::OnAxisYLeft()
{
	// TODO: Add your command handler code here
	SceneAngle.y -= 15.0;
	Invalidate();

	AddNewViewPara();
}

void CRealModelView::OnAxisYRight()
{
	// TODO: Add your command handler code here
	SceneAngle.y += 15.0;
	Invalidate();

	AddNewViewPara();
}

void CRealModelView::OnAxisZLeft()
{
	// TODO: Add your command handler code here
	SceneAngle.z -= 15.0;
	Invalidate();

	AddNewViewPara();
}

void CRealModelView::OnAxisZRight()
{
	// TODO: Add your command handler code here
	SceneAngle.z += 15.0;
	Invalidate();

	AddNewViewPara();
}


void CRealModelView::OnGetCoordSystem()
{
	// open polyhedrons
	ReadData();
	pCrt = new CCordnt();
    pCrt = GetNewCordntSystemBlockGroup();
	GLuint allBlockGroups;
	allBlockGroups = glGenLists(1);
	CompileBlockGroupList(pCrt, allBlockGroups, 0);
	glCompileBlockGroupList(pCrt, allBlockGroups, 1); // IF == 1

	m_bOpenGL = 1;
	Invalidate(TRUE);

	SaveDisplayControls();
}


void CRealModelView::OnSetCoordSystem()
{
	if(pCrt == NULL)  pCrt = new CCordnt();
	RestoreDisplayControls();
}

void CRealModelView::SaveDisplayControls()
{
	//Save display controls
	int i, j, ndata;
	CString fn, fnBlk, fnameBlk;
	ndata = AfxMessageBox(_T("Save Display Controls ?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6) {
	    CFileDialog Dlg4(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	    if(Dlg4.DoModal() == IDOK) 
	    {
		   fn = Dlg4.GetPathName(); 
		   fnameBlk = _T("DisplayControls_") + fn;
		   if(fnameBlk.Right(4)!="txt")   fnameBlk += ".txt";
		   ofstream fout0(fnameBlk);
		   /*
		   float minX, minY, minZ, maxX, maxY, maxZ;
		   float lx, mx, nx; //新坐标系中X轴的方向余弦
		   float ly, my, ny; //新坐标系中Y轴的方向余弦
		   float lz, mz, nz; //新坐标系中Z轴的方向余弦
		   float u, v, w; ////新坐标系坐标原点的平移量
		   float mu, mv, mw; ////新坐标系坐标原点的平移量
		   float Scale;//zoom scale
		   */
		   // parameters
		   fout0 << pCrt->minX << "  " << pCrt->minY << "  " << pCrt->minZ << "  " <<  pCrt->maxX << "  " << pCrt->maxY << "  " << pCrt->maxZ<<endl;
		   fout0 << pCrt->lx << "  " << pCrt->mx << "  " << pCrt->nx <<endl;
		   fout0 << pCrt->ly << "  " << pCrt->my << "  " << pCrt->ny <<endl;
		   fout0 << pCrt->lz << "  " << pCrt->mz << "  " << pCrt->nz <<endl;
		   fout0 << pCrt->u << "  " << pCrt->v << "  " << pCrt->w <<endl;
		   fout0 << pCrt->mu << "  " << pCrt->mv << "  " << pCrt->mw <<endl;
		   fout0 << pCrt->Scale;

		   fout0.close();
	    }
	}
	else return;

}

void CRealModelView::RestoreDisplayControls()
{
	int i, j, k, m, n0, n1, n2, ndata;
	float x, y, z;
	int nNds, nTrgls;
	ndata = AfxMessageBox(_T("Restore Display Controls ?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6){
		CFileDialog Dlg0(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"), NULL);
    	if(Dlg0.DoModal() == IDOK){
			ifstream fin0(Dlg0.GetPathName());
			/*
			float minX, minY, minZ, maxX, maxY, maxZ;
			float lx, mx, nx; //新坐标系中X轴的方向余弦
			float ly, my, ny; //新坐标系中Y轴的方向余弦
			float lz, mz, nz; //新坐标系中Z轴的方向余弦
			float u, v, w; ////新坐标系坐标原点的平移量
			float mu, mv, mw; ////新坐标系坐标原点的平移量
			float Scale;//zoom scale
			*/
			// parameters
			fin0 >> pCrt->minX >> pCrt->minY >> pCrt->minZ  >>  pCrt->maxX  >> pCrt->maxY >> pCrt->maxZ;
			fin0 >> pCrt->lx>> pCrt->mx >> pCrt->nx;
			fin0 >> pCrt->ly >> pCrt->my>> pCrt->ny;
			fin0 >> pCrt->lz >> pCrt->mz>> pCrt->nz;
			fin0 >> pCrt->u >> pCrt->v >> pCrt->w;
			fin0 >> pCrt->mu >> pCrt->mv >> pCrt->mw;
			fin0 >> pCrt->Scale;
			
			fin0.close();
		}
	}
	else return;
	
}

// Move : Left, Right, Up, Down
void CRealModelView::OnMoveLeft()
{
	ScenePos.x -= 0.02;
	ScenePos.y += 0.00;
	ScenePos.z += 0.00;
	glTranslatef(ScenePos.x, ScenePos.y, ScenePos.z);
	glFlush();
	Invalidate();
}

void CRealModelView::OnMoveUp()
{
	ScenePos.x += 0.00;
	ScenePos.y += 0.00;
	ScenePos.z += 0.02;
	glTranslatef(ScenePos.x, ScenePos.y, ScenePos.z);
	glFlush();
	Invalidate();
}

void CRealModelView::OnMoveRight()
{
	ScenePos.x += 0.02;
	ScenePos.y += 0.00;
	ScenePos.z += 0.00;
	glTranslatef(ScenePos.x, ScenePos.y, ScenePos.z);
	glFlush();
	Invalidate();
}

void CRealModelView::OnMoveDown()
{
	ScenePos.x += 0.00;
	ScenePos.y += 0.00;
	ScenePos.z -= 0.02;
	glTranslatef(ScenePos.x, ScenePos.y, ScenePos.z);
	glFlush();
	Invalidate();
}

// Intersect in 2D, slow and direct version  2011.11.22
void CRealModelView::On2DModelIntersectSlow()
{
	int i, j, k, id, ID;
	double x, y, z;
	CTrgl * pTr0; CTrgl * pTr1;

	// For Output
	if(pFrame == NULL) {
	   pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	}

	// For Progress
	CProgressWnd wndProgress(this, _T("Computation Progress"), TRUE);
	bool bCancel = false;
	int myRange = m_aSurfs.GetAt(0)->pTrs.GetSize() * m_aSurfs.GetAt(1)->pTrs.GetSize();
	wndProgress.SetRange(0, myRange);
	wndProgress.SetText(_T("This is a progress window...\n\n")
						_T("Program is running. Please wait..."));
	Sleep(1); // To show clearly, can be removed

 	for(i = 0; i < m_aSurfs.GetAt(0)->pTrs.GetSize(); i++) {
		CLoop * lp0 = new CLoop();
		pTr0 = m_aSurfs.GetAt(0)->pTrs.GetAt(i);
		for(k = 0; k < 3; k++) {
			x = pTr0->x[k];  y = pTr0->y[k];  z = pTr0->z[k];
			lp0->pNds.Add(new CNode(k, x, y, z));
		}
		x = pTr0->x[0];  y = pTr0->y[0];  z = pTr0->z[0];
		lp0->pNds.Add(new CNode(0, x, y, z));

		for(j = 0; j < m_aSurfs.GetAt(1)->pTrs.GetSize(); j++) {

			// For Output
		    CString str;
			str.Format(_T(" i = %d,   j = %d"), i, j);
			pFrame->AddStrToOutputWnd(1, str);
			// For Progress
		    wndProgress.StepIt();
		    wndProgress.PeekAndPump();
		    if (wndProgress.Cancelled()) {
			     AfxMessageBox(_T("Progress Cancelled"));
			     bCancel = true;
			     break;
		    }

			CLoop * lp1 = new CLoop();
			pTr1 = m_aSurfs.GetAt(1)->pTrs.GetAt(j);
			for(k = 0; k < 3; k++) {
				x = pTr1->x[k];  y = pTr1->y[k];  z = pTr1->z[k];
				lp1->pNds.Add(new CNode(k, x, y, z));
			}
			x = pTr1->x[0];  y = pTr1->y[0];  z = pTr1->z[0];
			lp1->pNds.Add(new CNode(0, x, y, z));

			CLoop * pIntr = new CLoop();
			
			if(IntrOfConvexPolygonV2(lp0, lp1, pIntr, myEps) == true) { // has bug!!!
			    m_aLpsIntr.Add(pIntr); // Save
			}
			/*
			if(IntrOfPolygonAndPolygon(lp0, lp1, pIntr, myEps) == true) { // has bug!!!
			    m_aLpsIntr.Add(pIntr); // Save
			}
			*/
			/*
			if(IsTwoTrglIntr(pTr0, pTr1, myEps)) {
				IntrTriangle2Triangle2(pTr0, pTr1, pIntr);
				m_aLpsIntr.Add(pIntr); // Save
			}
			*/

		}
		if(bCancel == true)  break;
	}
	if(bCancel == true)  
	wndProgress.Hide();
	wndProgress.DestroyWindow();
	
	//emerge all the points and rename the IDs
	for(i = 0; i < m_aSurfs.GetSize(); i++) {  // original nodes of triangles
		for(j = 0; j < m_aSurfs.GetAt(i)->pNds.GetSize(); j++) {
			id = m_aSurfs.GetAt(i)->pNds.GetAt(j)->ID;
			x = m_aSurfs.GetAt(i)->pNds.GetAt(j)->x; 
			y = m_aSurfs.GetAt(i)->pNds.GetAt(j)->y; 
			z = m_aSurfs.GetAt(i)->pNds.GetAt(j)->z; 
			pNds.Add(new CNode(id, x, y, z));
		}
	}
	for(i = 0; i < m_aLpsIntr.GetSize(); i++) { // original nodes of intersections(loops)
		for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
			id = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->ID;
			x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;
			y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
			z = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->z;
			pNds.Add(new CNode(id, x, y, z));
		}
	}

	//sort the nodes
	int n, cnt_1, idx, count;

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; 
		p[idx].y = pNds.GetAt(j)->y; 
		p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}
	for(i = cnt_1 - 1; i >= 0; i--){
	    pNds.GetAt(i)->Clear(); 
		delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll();  
	//sort nodes
    SortNode(p, cnt_1);

	//re-arrange nodes
	int* iID = new int[cnt_1];	int* nID = new int[cnt_1];
	for(i = 0; i < cnt_1; i++)iID[i] = p[i].ID;

	nID[0] = 0; idx = 0;

	pNds.Add(new CNode(idx, p[0].x, p[0].y, p[0].z)); 
	
	//re-create nodes
	for(i = 1; i < cnt_1; i++){
		if(!IsSameNode(&p[i], &p[i-1], myEps ) ) { 
			idx++; pNds.Add(new CNode(idx, p[i].x, p[i].y, p[i].z));
		}
		nID[i] = idx;
	}
	delete [] p;
	//sort id
	prhap(iID, nID, cnt_1); idx = 0;  count = 0;

	// renew all the IDs for points
	//copy new nodes for region
	n = 0; 
	for(i = 0; i < m_aSurfs.GetSize(); i++) { 
		for(j = 0; j < m_aSurfs.GetAt(i)->pNds.GetSize(); j++) {
			id = nID[n];
			m_aSurfs.GetAt(i)->pNds.GetAt(j)->ID = id;
			n++;
		}
	}
	//copy new nodes for intersections
	for(i = 0; i < m_aLpsIntr.GetSize(); i++) { 
		for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
			id = nID[n];
			m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->ID = id;
			n++;		
		}
	}

	delete[] iID;  delete[] nID;

	////////output data
	double xx,yy,zz;
	CString fnMesh,fnNode;
	int NumOfNds = pNds.GetSize();   
	int NumOfLps = m_aLpsIntr.GetSize();
	if( AfxMessageBox(_T("Save intersection in 2D ?"), MB_YESNOCANCEL ) != 6 ) return;
	CFileDialog Dlg(false, NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Data File(*.txt)|*.txt"), NULL);

	if( Dlg.DoModal() == IDOK ) 
	{ 
		fnMesh = Dlg.GetPathName();
		fnMesh += "-Mesh";
		if(fnMesh.Right(4) != ".txt")   fnMesh += ".txt";
		ofstream fin( fnMesh );
		for(i = 0; i < NumOfLps-1; i++){
			fin << i << "  " ;
			for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
				fin << m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->ID << "  ";
			}
			fin << endl;
		}
		fin << i << "  " ;
		for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
			fin << m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->ID << "  ";
		}
		fin.close(); 
		
		fnNode = Dlg.GetPathName();
		fnNode += "-Node";
		if( fnNode.Right(4) != ".txt")   fnNode += ".txt";
		ofstream fin3( fnNode );
		for(i = 0; i < NumOfNds - 1; i++){
			ID = pNds.GetAt(i)->ID;
			xx = pNds.GetAt(i)->x;  yy = pNds.GetAt(i)->y;  zz = pNds.GetAt(i)->z;
			fin3 << ID << "   " <<  xx << "   " <<  yy << "   " << zz << endl;
		}
		ID = pNds.GetAt(i)->ID;
		xx = pNds.GetAt(i)->x;  yy = pNds.GetAt(i)->y;  zz = pNds.GetAt(i)->z;
		fin3 << ID << "   " <<  xx << "   " << yy << "   " << zz;
		fin3.close(); 
	}

	////////Remove the memory
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
	    // pNds.GetAt(i)->Clear(); 
		delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 
	cnt_1 = pTrs.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
	    // pTrs.GetAt(i)->Clear(); 
		delete pTrs.GetAt(i); pTrs.RemoveAt(i);    
	}
	pTrs.RemoveAll(); 
	cnt_1 = m_aLpsIntr.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		// m_aLpsIntr.GetAt(i)->Clear(); 
		delete m_aLpsIntr.GetAt(i); m_aLpsIntr.RemoveAt(i);    
	}
	m_aLpsIntr.RemoveAll(); 
}

// Update the set of view parameters  2011.11.22
void CRealModelView::UpdateViewPara(int iView)
{
	SceneAngle.x = m_aViewPara.GetAt(iView)->SceneAngle.x;
	SceneAngle.y = m_aViewPara.GetAt(iView)->SceneAngle.y;
	SceneAngle.z = m_aViewPara.GetAt(iView)->SceneAngle.z;
	ScenePos.x = m_aViewPara.GetAt(iView)->ScenePos.x;
	ScenePos.y = m_aViewPara.GetAt(iView)->ScenePos.y;
	ScenePos.z = m_aViewPara.GetAt(iView)->ScenePos.z;
	SceneDepth = m_aViewPara.GetAt(iView)->SceneDepth;
	SceneZoom = m_aViewPara.GetAt(iView)->SceneZoom;
}

// Add a set of view parameters  2011.11.22
void CRealModelView::AddNewViewPara()
{
	CViewPara* pView = new CViewPara();
	pView->SceneAngle.x = SceneAngle.x;
	pView->SceneAngle.y = SceneAngle.y;
	pView->SceneAngle.z = SceneAngle.z;
	pView->ScenePos.x = ScenePos.x;
	pView->ScenePos.y = ScenePos.y;
	pView->ScenePos.z = ScenePos.z;
	pView->SceneDepth = SceneDepth;
	pView->SceneZoom = SceneZoom;
	m_aViewPara.Add(pView);
	m_iView = m_aViewPara.GetUpperBound();
}

void CRealModelView::OnImagePreviousView()
{
	--m_iView;
	UpdateViewPara(m_iView);
	Invalidate();
}

void CRealModelView::OnImageNextView()
{  
	++m_iView;
	UpdateViewPara(m_iView);
    Invalidate();
}


void CRealModelView::OnDisplayTrglSurface()
{
	// TODO: Add your command handler code here
}


void CRealModelView::OnDisplayQuadSurface()
{
	// TODO: Add your command handler code here
}


void CRealModelView::OnDisplayPolySurface()
{
	CSurf * surf = new CSurf();
	ReadPolygonSurface(surf);
	m_aSurfs.Add(surf);

	pCrt = new CCordnt();
    pCrt = GetNewCordntSystem(surf);
	int nregion = m_aSurfs.GetSize();
	int * list = new int[nregion];
	for(int j = 0; j < nregion; j++){
		list[j] = j+1;
		surf = m_aSurfs.GetAt(j);
		glCompilePolygonSurfaceList(surf, pCrt, list[j], 1); // 1 : each element has different color
	}
	m_bOpenGL = 1;
	Invalidate(TRUE);
	delete[] list;
}


void CRealModelView::OnDisplayPolyhedron()
{
	// TODO: Add your command handler code here
}


void CRealModelView::OnDisplayPointView()
{
	m_nDisplayMode = DISPLAY_MODE_POINT;
	UpdateDisplayMode(m_nObjectType, m_nDisplayMode);
}

void CRealModelView::OnDisplayLineView()
{
	m_nDisplayMode = DISPLAY_MODE_LINE;
	UpdateDisplayMode(m_nObjectType, m_nDisplayMode);
}

void CRealModelView::OnDisplayAreaView()
{
	m_nDisplayMode = DISPLAY_MODE_AREA;
	UpdateDisplayMode(m_nObjectType, m_nDisplayMode);
}

void CRealModelView::OnDisplaySurfaceView()
{
	m_nDisplayMode = DISPLAY_MODE_SURFACE;
	UpdateDisplayMode(m_nObjectType, m_nDisplayMode);
}

void CRealModelView::OnDisplayPolyhedraView()
{
	m_nDisplayMode = DISPLAY_MODE_POLYHEDRA;
	UpdateDisplayMode(m_nObjectType, m_nDisplayMode);
}

void CRealModelView::OnDisplayBlockView()
{
	m_nDisplayMode = DISPLAY_MODE_BLOCK;
	UpdateDisplayMode(m_nObjectType, m_nDisplayMode);
}

void CRealModelView::UpdateDisplayMode(int nObjectType, int nDisplayMode)
{
//	m_nAntialising = DISPLAY_ANTIALISING_OFF;

	m_nAntialising = DISPLAY_ANTIALISING_ON;

	int i, j;
	if(nObjectType == OBJECT_TYPE_SURFACE)
	{
	  switch (nDisplayMode) 
	  {
	    case DISPLAY_MODE_POINT: {
			for(int i = 0; i < m_aSurfs.GetSize(); i++) { // Creat color
		        m_aSurfs.GetAt(i)->ID = i;
		        CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	        }
	        int nSurf = m_aSurfs.GetSize();
	        int * list = new int[nSurf];
	        for(int j = 0; j < nSurf; j++){
		        list[j] = j+1;
		        pSurf = m_aSurfs.GetAt(j);
				if(m_nAntialising == DISPLAY_ANTIALISING_OFF) {
		            glCompileTinListPoint(pSurf, pCrt, list[j], 0); // Point, NO Antialising
				}
				else if(m_nAntialising == DISPLAY_ANTIALISING_ON) {
				    glCompileTinListPointAnti(pSurf, pCrt, list[j], 0); // Point, Antialising
				}
				else AfxMessageBox(_T("Antialising is not SET ! \nON  -- 1\nOFF -- 0"));
	        }
	        m_bOpenGL = 1;
	        Invalidate(TRUE);
	        delete[] list;
		    break;
	    }
	    case DISPLAY_MODE_LINE: {
		    for(int i = 0; i < m_aSurfs.GetSize(); i++) { // Creat color
		        m_aSurfs.GetAt(i)->ID = i;
		        CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	        }
	        int nSurf = m_aSurfs.GetSize();
	        int * list = new int[nSurf];
	        for(int j = 0; j < nSurf; j++){
		        list[j] = j+1;
		        pSurf = m_aSurfs.GetAt(j);
				if(m_nAntialising == DISPLAY_ANTIALISING_OFF) {
		            glCompileTinListSegment(pSurf, pCrt, list[j], 0); // edge and elements, new
				}
				else if(m_nAntialising == DISPLAY_ANTIALISING_ON) {
				    glCompileTinListSegmentAnti(pSurf, pCrt, list[j], 0); // edge and elements, new
				}
				else AfxMessageBox(_T("Antialising is not SET ! \nON  -- 1\nOFF -- 0"));
	        }
	        m_bOpenGL = 1;
	        Invalidate(TRUE);
	        delete[] list;
		    break;
	    }
	    case DISPLAY_MODE_AREA: {
			for(int i = 0; i < m_aSurfs.GetSize(); i++) { // Creat color
		        m_aSurfs.GetAt(i)->ID = i;
		        CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	        }
	        int nSurf = m_aSurfs.GetSize();
	        int * list = new int[nSurf];
	        for(int j = 0; j < nSurf; j++){
		        list[j] = j+1;
		        pSurf = m_aSurfs.GetAt(j);
		        glCompileTinListArea(pSurf, pCrt, list[j], 0); // edge and elements, new
	        }
	        m_bOpenGL = 1;
	        Invalidate(TRUE);
	        delete[] list;
		    break;
	    }
	    case DISPLAY_MODE_SURFACE: {
	        for(int i = 0; i < m_aSurfs.GetSize(); i++) { // Creat color
		        m_aSurfs.GetAt(i)->ID = i;
		        CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	        }
	        int nSurf = m_aSurfs.GetSize();
	        int * list = new int[nSurf];
	        for(int j = 0; j < nSurf; j++){
		        list[j] = j+1;
		        pSurf = m_aSurfs.GetAt(j);
		        glCompileTinList(pSurf, pCrt, list[j], 0); // edge and elements, new
	        }
	        m_bOpenGL = 1;
	        Invalidate(TRUE);
	        delete[] list;
		    break;
	    }
	    case DISPLAY_MODE_POLYHEDRA: {
			AfxMessageBox(_T("Surfaces cannot be displayed in POLYHEDRA mode !"));
		    break;
	    }
	    case DISPLAY_MODE_BLOCK: {
			AfxMessageBox(_T("Surfaces cannot be displayed in BLOCK mode !"));
		    break;
	    }
	    default : {}
	  }
	}

	if(nObjectType == OBJECT_TYPE_POLYHEDRA)
	{
	  switch (nDisplayMode) 
	  {
	    case DISPLAY_MODE_POINT: {
			// delete existed display lists, very important!
//			if(m_aSurfs.IsEmpty() == true) glDeleteLists(1, 1);
//			else glDeleteLists(1, m_aSurfs.GetSize()); 
	        GLuint allBlockGroups;
//	        allBlockGroups = glGenLists(1);
			allBlockGroups = 1 + m_aSurfs.GetSize();
	        // CompileBlockGroupList(pCrt, allBlockGroups, 0);
	        glCompileBlockGroupListPoint(pCrt, allBlockGroups, 1); // IF == 1
	        m_bOpenGL = 1;
	        Invalidate(TRUE);
		    break;
	    }
	    case DISPLAY_MODE_LINE: {
			// delete existed display lists, very important!
//			if(m_aSurfs.IsEmpty() == true) glDeleteLists(1, 1);
//			else glDeleteLists(1, m_aSurfs.GetSize());
	        GLuint allBlockGroups;
//	        allBlockGroups = glGenLists(1); 

			allBlockGroups = 1 + m_aSurfs.GetSize();
	        glCompileBlockGroupListSegment(pCrt, allBlockGroups, 0);  // new rendering function
	        m_bOpenGL = 1;
	        Invalidate(TRUE);
		    break;
	    }
	    case DISPLAY_MODE_AREA: {
			// delete existed display lists, very important!
//			if(m_aSurfs.IsEmpty() == true) glDeleteLists(1, 1);
//			else glDeleteLists(1, m_aSurfs.GetSize());
	        GLuint allBlockGroups;
//	        allBlockGroups = glGenLists(1); 
			allBlockGroups = 1 + m_aSurfs.GetSize();
	        glCompileBlockGroupListArea(pCrt, allBlockGroups, 0);  // new rendering function
	        m_bOpenGL = 1;
	        Invalidate(TRUE);
		    break;
	    }
	    case DISPLAY_MODE_SURFACE: {
	        AfxMessageBox(_T("Polyhedrons cannot be displayed in SURFACE mode !"));
		    break;
	    }
	    case DISPLAY_MODE_POLYHEDRA: {
			// delete existed display lists, very important!
//			if(m_aSurfs.IsEmpty() == true) glDeleteLists(1, 1);
//          else glDeleteLists(1, m_aSurfs.GetSize()); 
	        GLuint allBlockGroups;
//	        allBlockGroups = glGenLists(1);
			allBlockGroups = 1 + m_aSurfs.GetSize();
	        CompileBlockGroupList(pCrt, allBlockGroups, 0);
	        glCompileBlockGroupList(pCrt, allBlockGroups, 1); // IF == 1
	        m_bOpenGL = 1;
	        Invalidate(TRUE);
		    break;
	    }
	    case DISPLAY_MODE_BLOCK: {
			// delete existed display lists, very important!
//			if(m_aSurfs.IsEmpty() == true) glDeleteLists(1, 1);
//			else glDeleteLists(1, m_aSurfs.GetSize());
	        GLuint allBlockGroups;
//	        allBlockGroups = glGenLists(1); 
            allBlockGroups = 1 + m_aSurfs.GetSize();
	        glCompileBlockGroupList(pCrt, allBlockGroups, 0);  // new rendering function
	        m_bOpenGL = 1;
	        Invalidate(TRUE);
		    break;
	    }
	    default : {}
	  }

	}

}

// 2011.12.5
// intersection of 2 surfaces, divide some triangles into polygons
// compute the intersection line of 2 surfaces, some triangles are updated into polygons
// all original triangles are stored in pTrs, results are stored in pLps including triangles
// the first and the last node of a loop is different!!
void CRealModelView::CutAndUpdateTwoTrglSurf(CSurf* sa, CSurf* sb, CSurf* newa, CSurf* newb, double eps)
{
#ifndef EGC
	// boundary box test
	CNode * saLB = new CNode();
	CNode * saTR = new CNode();
	CreatBoundaryBox(sa, saLB, saTR);

	CNode * sbLB = new CNode();
	CNode * sbTR = new CNode();
	CreatBoundaryBox(sb, sbLB, sbTR);

	bool bIntersect = BoundaryBoxIntrTest(saLB, saTR, sbLB, sbTR);

	if(bIntersect == false) {
		// no intersection, save directly
		int i, j, k, nID[3];
		double x, y, z;
		for(i = 0; i < sa->pNds.GetSize(); i++) {
			x = sa->pNds.GetAt(i)->x;
			y = sa->pNds.GetAt(i)->y;
			z = sa->pNds.GetAt(i)->z;
			newa->pNds.Add(new CNode(i, x, y, z));
		}
		for(i = 0; i < sa->pTrs.GetSize(); i++) {
			CLoop* lp = new CLoop();
			for(j = 0;j < 3; j++) {
				nID[j] = sa->pTrs.GetAt(i)->nID[j];
				lp->iNds.Add(nID[j]);
			}
			newa->pLps.Add(lp);
		}

		for(i = 0; i < sb->pNds.GetSize(); i++) {
			x = sb->pNds.GetAt(i)->x;
			y = sb->pNds.GetAt(i)->y;
			z = sb->pNds.GetAt(i)->z;
			newb->pNds.Add(new CNode(i, x, y, z));
		}
		for(i = 0; i < sb->pTrs.GetSize(); i++) {
			CLoop* lp = new CLoop();
			for(j = 0;j < 3; j++) {
				nID[j] = sb->pTrs.GetAt(i)->nID[j];
				lp->iNds.Add(nID[j]);
			}
			newb->pLps.Add(lp);
		}
	}
	else {  //intersection in 3D
	    int i, j, k, n0, n1, id;
		double x, y, z;
	    int t;
	    int count, nCase;
	    double *nx, *ny, *nz, *para;
	    CNode * pMin, * pMax;
	    CArray<CLine*, CLine*> pLns;
	    float v0[3], v1[3], v2[3], u0[3], u1[3], u2[3], isec0[3], isec1[3]; // prepare for Moller.dll
	    int copla;
	    int * coplanar = &copla;
	    CSurf* pSurf0 = sa;  CSurf* pSurf1 = sb;
	
		// Load function from DLL Moller.dll 2011.9.30
	    typedef int (*lpFun)(float [3],float [3],float [3],
				             float [3],float [3],float [3],int *,
				             float [3],float [3]); // 宏定义函数指针类型
	    HINSTANCE hDll; //DLL 句柄
        lpFun trglFun; // 函数指针
	    hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
        if(NULL==hDll) {
            MessageBox(_T("Fail to load DLL Moller.dll ! "));
	    }
	    else {
		    trglFun = (lpFun) GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
	    }
	    // End loading

	    //compute the intersection of each triangle pair which is restricted in one son-box
	    for(j = 0; j < pSurf0->pTrs.GetSize(); j++) {
			n0 = j;
			v0[0] = pSurf0->pTrs.GetAt(n0)->x[0]; 
			v0[1] = pSurf0->pTrs.GetAt(n0)->y[0]; 
			v0[2] = pSurf0->pTrs.GetAt(n0)->z[0];
			v1[0] = pSurf0->pTrs.GetAt(n0)->x[1]; 
			v1[1] = pSurf0->pTrs.GetAt(n0)->y[1]; 
			v1[2] = pSurf0->pTrs.GetAt(n0)->z[1];
			v2[0] = pSurf0->pTrs.GetAt(n0)->x[2]; 
			v2[1] = pSurf0->pTrs.GetAt(n0)->y[2]; 
			v2[2] = pSurf0->pTrs.GetAt(n0)->z[2];

			for(k = 0; k < pSurf1->pTrs.GetSize(); k++) {
				// For output window
				CString str;
				str.Format(_T("Intersecting: j = %d,  k = %d"), j, k);
				pFrame->AddStrToOutputWnd(1, str);

				n1 = k;
				u0[0] = pSurf1->pTrs.GetAt(n1)->x[0]; 
				u0[1] = pSurf1->pTrs.GetAt(n1)->y[0]; 
				u0[2] = pSurf1->pTrs.GetAt(n1)->z[0];
				u1[0] = pSurf1->pTrs.GetAt(n1)->x[1]; 
				u1[1] = pSurf1->pTrs.GetAt(n1)->y[1]; 
				u1[2] = pSurf1->pTrs.GetAt(n1)->z[1];
				u2[0] = pSurf1->pTrs.GetAt(n1)->x[2]; 
				u2[1] = pSurf1->pTrs.GetAt(n1)->y[2]; 
				u2[2] = pSurf1->pTrs.GetAt(n1)->z[2];
				
                if (trglFun != NULL) {
					 nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		        }
		        else {
			         MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		        }
       
				if(nCase == 1) { //save intersection segment
					if(pLns.IsEmpty() == TRUE) { // if empty, save directly
						CLine * ptmp = new CLine();
						ptmp->FstTri = n0;  ptmp->SecTri = n1;
						for(t = 0; t < 3; t++) { // coordinates of intersection
							ptmp->FstNd[t] = isec0[t];  
							ptmp->SecNd[t] = isec1[t];
						} 
						pLns.Add(ptmp);
					}
					else {
						bool bexist = false;
						for(t = 0; t < pLns.GetSize(); t++){ 
							if(pLns.GetAt(t)->FstTri == n0 && pLns.GetAt(t)->SecTri == n1) {
								bexist = true;  break; 
							}
						}
						if(!bexist) { // not exist
							CLine * ptmp = new CLine();
							ptmp->FstTri = n0;  ptmp->SecTri = n1;
							for(t = 0; t < 3; t++) { // coordinates of intersection
								ptmp->FstNd[t] = isec0[t];  
								ptmp->SecNd[t] = isec1[t];
							} 
							pLns.Add(ptmp);
						} // End if()
					}// End else{}
				}// End if()
			}// End for(k)
	    }// End for(j)

		FreeLibrary(hDll); // 释放函数

	//	return;
	
		// emerge points for all intersection lines
		bool bexist;
		double coord[3];
		CArray<CNode*, CNode*> tNds;
		tNds.Add(new CNode(0, pLns.GetAt(0)->FstNd[0],pLns.GetAt(0)->FstNd[1],pLns.GetAt(0)->FstNd[2]));
		tNds.Add(new CNode(1, pLns.GetAt(0)->SecNd[0],pLns.GetAt(0)->SecNd[1],pLns.GetAt(0)->SecNd[2]));
		for(i = 1; i < pLns.GetSize(); i++) {
			CNode* nd;
			for(t = 0; t < 3; t++) { // first point
				coord[t] = pLns.GetAt(i)->FstNd[t];
			}
			nd = new CNode(0, coord[0], coord[1], coord[2]);
			bexist = false;
			for(j = 0; j < tNds.GetSize(); j++) {
				if(IsSameNode(nd,tNds.GetAt(j), eps)) {
					bexist = true;  break;
				}
			}
			if(bexist == false) { tNds.Add(nd); }
			else { delete nd; }

			for(t = 0; t < 3; t++) { // second point
				coord[t] = pLns.GetAt(i)->SecNd[t];
			}
			nd = new CNode(0, coord[0], coord[1], coord[2]);
			bexist = false;
			for(j = 0; j < tNds.GetSize(); j++) {
				if(IsSameNode(nd,tNds.GetAt(j), eps)) {
					bexist = true;  break;
				}
			}
			if(bexist == false) { tNds.Add(nd); }
			else { delete nd; }
		}

		for(i = 0; i < tNds.GetSize(); i++) {
			tNds.GetAt(i)->ID = i;
		}

		for(i = 0; i < pLns.GetSize(); i++) { // Give new ID
			CNode* nd;
			for(t = 0; t < 3; t++) { // first point
				coord[t] = pLns.GetAt(i)->FstNd[t];
			}
			nd = new CNode(0, coord[0], coord[1], coord[2]);
			for(j = 0; j < tNds.GetSize(); j++) {
				if(IsSameNode(nd,tNds.GetAt(j), eps)) {
					pLns.GetAt(i)->nID[0] = j;   break;
				}
			}
			delete nd; 

			for(t = 0; t < 3; t++) { // second point
				coord[t] = pLns.GetAt(i)->SecNd[t];
			}
			nd = new CNode(0, coord[0], coord[1], coord[2]);
			for(j = 0; j < tNds.GetSize(); j++) {
				if(IsSameNode(nd,tNds.GetAt(j), eps)) {
					pLns.GetAt(i)->nID[1] = j;   break;
				}
			}
            delete nd; 
		}

		// form new loops
		CArray<CLine*, CLine*> lines;

		// for surface 0
		for(i = 0; i < pSurf0->pTrs.GetSize(); i++) {
			// For output window
			CString str;
			str.Format(_T("Forming: i = %d [/ %d]"), i, pSurf0->pTrs.GetSize());
			pFrame->AddStrToOutputWnd(1, str);

			lines.RemoveAll();
			for(j = 0; j < pLns.GetSize(); j++) { // Get all intersection lines
				if(pLns.GetAt(j)->FstTri == i) {
					lines.Add(pLns.GetAt(j));
				}
			}

			// no intersection line, save x, y, z directly
			if(lines.IsEmpty() == TRUE) {
				CLoop* lp = new CLoop();
				for(t = 0; t < 3; t++) {
					x = pSurf0->pTrs.GetAt(i)->x[t];
					y = pSurf0->pTrs.GetAt(i)->y[t];
					z = pSurf0->pTrs.GetAt(i)->z[t];
					lp->pNds.Add(new CNode(t, x, y, z));
				}
				newa->pLps.Add(lp);
			}
			else { // Combine all intersection lines
				CArray<int, int> ids;
				ids.Add(lines.GetAt(0)->nID[0]);
				ids.Add(lines.GetAt(0)->nID[1]);

				while(ids.GetSize() != (lines.GetSize() + 1)) {
					// Head
					n0 = ids.GetAt(ids.GetUpperBound()); // last one
					n1 = ids.GetAt(ids.GetUpperBound()-1); // second last
					for(j = 0; j < lines.GetSize(); j++) {
						if(lines.GetAt(j)->nID[0] == n0 && lines.GetAt(j)->nID[1] != n1) {
							ids.Add(lines.GetAt(j)->nID[1]);
						}
						if(lines.GetAt(j)->nID[0] != n0 && lines.GetAt(j)->nID[1] == n1) {
							ids.Add(lines.GetAt(j)->nID[0]);
						}
					}
					// Tail
					n0 = ids.GetAt(0); // first
					n1 = ids.GetAt(1); // second
					for(j = 0; j < lines.GetSize(); j++) {
						if(lines.GetAt(j)->nID[0] == n0 && lines.GetAt(j)->nID[1] != n1) {
							ids.InsertAt(0, lines.GetAt(j)->nID[1]);
						}
						if(lines.GetAt(j)->nID[1] == n0 && lines.GetAt(j)->nID[0] != n1) {
							ids.InsertAt(0, lines.GetAt(j)->nID[0]);
						}
					}

				}

				// Save intersection points
				CPLine* pline = new CPLine();
				for(t = 0; t < ids.GetSize(); t++) {
					id = ids.GetAt(t);
					x = tNds.GetAt(id)->x;
					y = tNds.GetAt(id)->y;
					z = tNds.GetAt(id)->z;
					pline->pNds.Add(new CNode(id, x, y, z));
				}

				// Divide trgl into 2 loops
				CLoop* lp = new CLoop();
				for(t = 0; t < 3; t++) {
					x = pSurf0->pTrs.GetAt(i)->x[t];
					y = pSurf0->pTrs.GetAt(i)->y[t];
					z = pSurf0->pTrs.GetAt(i)->z[t];
					lp->pNds.Add(new CNode(t, x, y, z));
				}
				CLoop* lpa = new CLoop();  CLoop* lpb = new CLoop();
				if(DividePolygonIntoTwoLps(lp, pline, lpa, lpb, eps) == true) {
					newa->pLps.Add(lpa);  newa->pLps.Add(lpb);
				}
			}
		}

		// for surface 1
		for(i = 0; i < pSurf1->pTrs.GetSize(); i++) {
			// For output window
			CString str;
			str.Format(_T("Forming: i = %d [/ %d]"), i, pSurf0->pTrs.GetSize());
			pFrame->AddStrToOutputWnd(1, str);

			lines.RemoveAll();
			for(j = 0; j < pLns.GetSize(); j++) { // Get all intersection lines
				if(pLns.GetAt(j)->SecTri == i) {
					lines.Add(pLns.GetAt(j));
				}
			}

			// no intersection line, save x, y, z directly
			if(lines.IsEmpty() == TRUE) {
				CLoop* lp = new CLoop();
				for(t = 0; t < 3; t++) {
					x = pSurf1->pTrs.GetAt(i)->x[t];
					y = pSurf1->pTrs.GetAt(i)->y[t];
					z = pSurf1->pTrs.GetAt(i)->z[t];
					lp->pNds.Add(new CNode(t, x, y, z));
				}
				newb->pLps.Add(lp);
			}
			else { // Combine all intersection lines
				CArray<int, int> ids;
				ids.Add(lines.GetAt(0)->nID[0]);
				ids.Add(lines.GetAt(0)->nID[1]);
				while(ids.GetSize() != (lines.GetSize() + 1)) {
					// Head
					n0 = ids.GetAt(ids.GetUpperBound()); // last one
					n1 = ids.GetAt(ids.GetUpperBound()-1); // second last
					for(j = 0; j < lines.GetSize(); j++) {
						if(lines.GetAt(j)->nID[0] == n0 && lines.GetAt(j)->nID[1] != n1) {
							ids.Add(lines.GetAt(j)->nID[1]);
						}
						if(lines.GetAt(j)->nID[0] != n0 && lines.GetAt(j)->nID[1] == n1) {
							ids.Add(lines.GetAt(j)->nID[0]);
						}
					}
					// Tail
					n0 = ids.GetAt(0); // first
					n1 = ids.GetAt(1); // second
					for(j = 0; j < lines.GetSize(); j++) {
						if(lines.GetAt(j)->nID[0] == n0 && lines.GetAt(j)->nID[1] != n1) {
							ids.InsertAt(0, lines.GetAt(j)->nID[1]);
						}
						if(lines.GetAt(j)->nID[1] == n0 && lines.GetAt(j)->nID[0] != n1) {
							ids.InsertAt(0, lines.GetAt(j)->nID[0]);
						}
					}

				}

				// Save intersection points
				CPLine* pline = new CPLine();
				for(t = 0; t < ids.GetSize(); t++) {
					id = ids.GetAt(t);
					x = tNds.GetAt(id)->x;
					y = tNds.GetAt(id)->y;
					z = tNds.GetAt(id)->z;
					pline->pNds.Add(new CNode(id, x, y, z));
				}

				// Divide trgl into 2 loops
				CLoop* lp = new CLoop();
				for(t = 0; t < 3; t++) {
					x = pSurf1->pTrs.GetAt(i)->x[t];
					y = pSurf1->pTrs.GetAt(i)->y[t];
					z = pSurf1->pTrs.GetAt(i)->z[t];
					lp->pNds.Add(new CNode(t, x, y, z));
				}
				CLoop* lpa = new CLoop();  CLoop* lpb = new CLoop();
				if(DividePolygonIntoTwoLps(lp, pline, lpa, lpb, eps) == true) {
					newb->pLps.Add(lpa);  newb->pLps.Add(lpb);
				}
			}
		}

		EmergePolygonSurface(newa);
		EmergePolygonSurface(newb);
	}

#else
   // to be continue

#endif

}

// 2011.12.5
// divide a polygon into 2 loops by a polyline
// pPg polygon;  pPline polyline;  lpa lpb  two new loops
bool CRealModelView::DividePolygonIntoTwoLps(CLoop* pPg, CPLine* pPline, CLoop* lpa, CLoop* lpb, double eps)
{
	int i, j, t, N;
	double x, y, z;
	CNode * point;
	int pos[2] = {-1, -1};//index of the position

	// Check whether first and last points are same
	CNode* fst = pPg->pNds.GetAt(0);
	CNode* lst = pPg->pNds.GetAt(pPg->pNds.GetUpperBound());
	if(IsSameNode(fst, lst, eps) == false) {
		pPg->pNds.Add(fst);
	}

	// 以head所在的线段为新环的起始线段
	point = pPline->pNds.GetAt(0); // first point
	for(j = 0; j < pPg->pNds.GetSize() - 1; j++) {	
		CNode * sp0 = pPg->pNds.GetAt(j);	
		CNode * sp1 = pPg->pNds.GetAt(j+1);	
		if(IsPointOnSegment(sp0, sp1, point, eps) != 0) {
			pos[0] = j;  break;
		}
	}
	if(pos[0] == -1)  return false;

	// tail的位置
	N = pPline->pNds.GetUpperBound();
	point = pPline->pNds.GetAt(N); // last point
	for(j = 0; j < pPg->pNds.GetSize() - 1; j++) {
		CNode * sp0 = pPg->pNds.GetAt(j);
		CNode * sp1 = pPg->pNds.GetAt(j+1);
	//	if(IsPointOnSegment(sp0, sp1, point, eps) != 0) {
		if(IsPointOnSegment(sp0, sp1, point, 0.01) != 0) {
			pos[1] = j;  break;
		}
	}
	if(pos[1] == -1)  return false;

	if(pos[0] < pos[1]) {
		// new loop lpa
		for(t = 0; t < pos[0] + 1; t++) {
			x = pPg->pNds.GetAt(t)->x;
			y = pPg->pNds.GetAt(t)->y;
			z = pPg->pNds.GetAt(t)->z;
			lpa->pNds.Add(new CNode(t, x, y, z));
		}
	    for(t = 0; t < pPline->pNds.GetSize(); t++) {
		    x = pPline->pNds.GetAt(t)->x;
		    y = pPline->pNds.GetAt(t)->y;
		    z = pPline->pNds.GetAt(t)->z;
		    lpa->pNds.Add(new CNode(t, x, y, z));
	    }
		for(t = pos[1]; t < pPg->pNds.GetSize() - 1; t++) {
			x = pPg->pNds.GetAt(t + 1)->x;
			y = pPg->pNds.GetAt(t + 1)->y;
			z = pPg->pNds.GetAt(t + 1)->z;
			lpa->pNds.Add(new CNode(t, x, y, z));
		}

		// new loop lpb
		for(t = pos[0]; t < pos[1]; t++) {
			x = pPg->pNds.GetAt(t + 1)->x;
			y = pPg->pNds.GetAt(t + 1)->y;
			z = pPg->pNds.GetAt(t + 1)->z;
			lpb->pNds.Add(new CNode(t, x, y, z));
		}
		for(t = pPline->pNds.GetUpperBound(); t > -1; t--) {
		    x = pPline->pNds.GetAt(t)->x;
		    y = pPline->pNds.GetAt(t)->y;
		    z = pPline->pNds.GetAt(t)->z;
		    lpb->pNds.Add(new CNode(t, x, y, z));
		}
    }
	else {
		// new loop lpa
		for(t = 0; t < pos[1] + 1; t++) {
			x = pPg->pNds.GetAt(t)->x;
			y = pPg->pNds.GetAt(t)->y;
			z = pPg->pNds.GetAt(t)->z;
			lpa->pNds.Add(new CNode(t, x, y, z));
		}
		for(t = pPline->pNds.GetUpperBound(); t > -1; t--) {
		    x = pPline->pNds.GetAt(t)->x;
		    y = pPline->pNds.GetAt(t)->y;
		    z = pPline->pNds.GetAt(t)->z;
		    lpa->pNds.Add(new CNode(t, x, y, z));
		}
		for(t = pos[0]; t < pPg->pNds.GetSize() - 1; t++) {
			x = pPg->pNds.GetAt(t + 1)->x;
			y = pPg->pNds.GetAt(t + 1)->y;
			z = pPg->pNds.GetAt(t + 1)->z;
			lpa->pNds.Add(new CNode(t, x, y, z));
		}

		// new loop lpb
		for(t = pos[1]; t < pos[0]; t++) {
			x = pPg->pNds.GetAt(t + 1)->x;
			y = pPg->pNds.GetAt(t + 1)->y;
			z = pPg->pNds.GetAt(t + 1)->z;
			lpb->pNds.Add(new CNode(t, x, y, z));
		}
		for(t = 0; t < pPline->pNds.GetSize(); t++) {
		    x = pPline->pNds.GetAt(t)->x;
		    y = pPline->pNds.GetAt(t)->y;
		    z = pPline->pNds.GetAt(t)->z;
		    lpb->pNds.Add(new CNode(t, x, y, z));
	    }

	}

	// Check last time
	N = lpa->pNds.GetUpperBound();
	if(IsSameNode(lpa->pNds.GetAt(0), lpa->pNds.GetAt(N), eps) == true) {
		lpa->pNds.RemoveAt(N);
	}
	N = lpb->pNds.GetUpperBound();
	if(IsSameNode(lpb->pNds.GetAt(0), lpb->pNds.GetAt(N), eps) == true) {
		lpb->pNds.RemoveAt(N);
	}

	return true;
		
}


// 2011.12.6
// Create polyhedrons by only mapping
void CRealModelView::CreatePolyhedronByOnlyMapping(CSurf* sa, CSurf* sb, CBlock* pBlk, double eps) 
{
	int i, j, k, t, id, N;
	double x, y, z, *nx, *ny, *nz, *para;
	CLoop* lpa, *lpb;

	// Check and copy data
	for(i = 0; i < sa->pLps.GetSize(); i++) {
		lpa = sa->pLps.GetAt(i);
		lpa->pNds.RemoveAll();
		for(t = 0; t < lpa->iNds.GetSize(); t++) {
			id = lpa->iNds.GetAt(t);
			lpa->pNds.Add(sa->pNds.GetAt(id));
		}
	}
	for(i = 0; i < sb->pLps.GetSize(); i++) {
		lpb = sb->pLps.GetAt(i);
		lpb->pNds.RemoveAll();
		for(t = 0; t < lpb->iNds.GetSize(); t++) {
			id = lpb->iNds.GetAt(t);
			lpb->pNds.Add(sb->pNds.GetAt(id));
		}
	}

	// For Progress
	CProgressWnd wndProgress(this, _T("Computation Progress"), TRUE);
	bool bCancel = false;
	int myRange = sa->pLps.GetSize() * sb->pLps.GetSize();
	wndProgress.SetRange(0, myRange);
	wndProgress.SetText(_T("This is a progress window...\n\n")
						_T("Program is running. Please wait..."));
	Sleep(1); // To show clearly, can be removed

	for(i = 0; i < sa->pLps.GetSize(); i++) {
		lpa = sa->pLps.GetAt(i);
		for(j = 0; j < sb->pLps.GetSize(); j++) {
			// for output window
			CString str;
			str.Format(_T("i = %d [/ %d],  j = %d [/ %d]"), i, sa->pLps.GetSize(), j, sb->pLps.GetSize());
			pFrame->AddStrToOutputWnd(1, str);

			// For Progress
		    wndProgress.StepIt();
		    wndProgress.PeekAndPump();
		    if (wndProgress.Cancelled()) {
			     AfxMessageBox(_T("Progress Cancelled"));
			     bCancel = true;
			     exit(0);  break; 
		    }

			lpb = sb->pLps.GetAt(j);
			CLoop* pIntr = new CLoop();
			if(IntrOfConvexPolygonV2(lpa, lpb, pIntr, eps) == true) {
				// Update Z
				nx = new double[3]; ny = new double[3]; nz = new double[3];
				para = new double[4];
				for(k = 0; k < 3; k++) { 
					nx[k] = lpa->pNds.GetAt(k)->x;
					ny[k] = lpa->pNds.GetAt(k)->y;
					nz[k] = lpa->pNds.GetAt(k)->z;
				}
				EquaOfTrgl(nx, ny, nz, para);

				// Up loop
				CLoop* lpUp = new CLoop();
				for(k = 0; k < pIntr->pNds.GetSize(); k++) {
					x = pIntr->pNds.GetAt(k)->x;
					y = pIntr->pNds.GetAt(k)->y;
					z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
					lpUp->pNds.Add(new CNode(k, x, y, z));
				}
				delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
				nx = new double[3]; ny = new double[3]; nz = new double[3];
				para = new double[4];
				for(k = 0; k < 3; k++) { 
					nx[k] = lpb->pNds.GetAt(k)->x;
					ny[k] = lpb->pNds.GetAt(k)->y;
					nz[k] = lpb->pNds.GetAt(k)->z;
				}
				EquaOfTrgl(nx, ny, nz, para);

				// Dn loop
				CLoop* lpDn = new CLoop();
				for(k = 0; k < pIntr->pNds.GetSize(); k++) {
					x = pIntr->pNds.GetAt(k)->x;
					y = pIntr->pNds.GetAt(k)->y;
					z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
					lpDn->pNds.Add(new CNode(k, x, y, z));
				}
				delete [] nx;  delete [] ny; delete [] nz; delete [] para;

				// Create Polyhedron
				CPohd* pPh = new CPohd();
				pPh->pLps.Add(lpUp);  pPh->pLps.Add(lpDn);
				for(k = 0; k < pIntr->pNds.GetSize() - 1; k++) {
					CLoop* lp = new CLoop();
					x = lpDn->pNds.GetAt(k)->x;   // Dn-0
					y = lpDn->pNds.GetAt(k)->y;
					z = lpDn->pNds.GetAt(k)->z;
					lp->pNds.Add(new CNode(k, x, y, z));

					x = lpDn->pNds.GetAt(k+1)->x; // Dn-1
					y = lpDn->pNds.GetAt(k+1)->y;
					z = lpDn->pNds.GetAt(k+1)->z;
					lp->pNds.Add(new CNode(k, x, y, z));

					x = lpUp->pNds.GetAt(k+1)->x; // Up-1
					y = lpUp->pNds.GetAt(k+1)->y;
					z = lpUp->pNds.GetAt(k+1)->z;
					lp->pNds.Add(new CNode(k, x, y, z));

					x = lpUp->pNds.GetAt(k)->x;   // Up-0
					y = lpUp->pNds.GetAt(k)->y;
					z = lpUp->pNds.GetAt(k)->z;
					lp->pNds.Add(new CNode(k, x, y, z));

					bool bsame = IsSameNode(lp->pNds.GetAt(0), lp->pNds.GetAt(3), eps) &&
						         IsSameNode(lp->pNds.GetAt(1), lp->pNds.GetAt(2), eps);

					if(bsame == false) { pPh->pLps.Add(lp); } // Check
					
				}
				// Special last area / loop
				N = pIntr->pNds.GetUpperBound();
				CLoop* lp = new CLoop();
				x = lpDn->pNds.GetAt(N)->x;   // Dn-0
				y = lpDn->pNds.GetAt(N)->y;
				z = lpDn->pNds.GetAt(N)->z;
				lp->pNds.Add(new CNode(N, x, y, z));

				x = lpDn->pNds.GetAt(0)->x; // Dn-1
				y = lpDn->pNds.GetAt(0)->y;
				z = lpDn->pNds.GetAt(0)->z;
				lp->pNds.Add(new CNode(0, x, y, z));

				x = lpUp->pNds.GetAt(0)->x; // Up-1
				y = lpUp->pNds.GetAt(0)->y;
				z = lpUp->pNds.GetAt(0)->z;
				lp->pNds.Add(new CNode(0, x, y, z));

				x = lpUp->pNds.GetAt(N)->x;   // Up-0
				y = lpUp->pNds.GetAt(N)->y;
				z = lpUp->pNds.GetAt(N)->z;
				lp->pNds.Add(new CNode(N, x, y, z));

				bool bsame = IsSameNode(lp->pNds.GetAt(0), lp->pNds.GetAt(3), eps) &&
						     IsSameNode(lp->pNds.GetAt(1), lp->pNds.GetAt(2), eps);
			    if(bsame == false) { pPh->pLps.Add(lp); } // Check
				
				if(!pPh->pLps.IsEmpty()) { // if valid, add
				    pBlk->pPhs.Add(pPh);
				}
			}
			pIntr->Clear();
		}
	}

	wndProgress.Hide();
	wndProgress.DestroyWindow();

}

void CRealModelView::OnDebugCutUpdateSurface()
{
	// TODO: Add your command handler code here
	CSurf* newa = new CSurf();
	CSurf* newb = new CSurf();
	CSurf* sa = m_aSurfs.GetAt(0);
	CSurf* sb = m_aSurfs.GetAt(1);
	CutAndUpdateTwoTrglSurf(sa, sb, newa, newb, myEps);

	// Save
	SavePolygonSurface(newa);
	SavePolygonSurface(newb);

	m_aSurfs.GetAt(1)->Clear();  delete m_aSurfs.GetAt(1);  m_aSurfs.RemoveAt(1);
	m_aSurfs.GetAt(0)->Clear();  delete m_aSurfs.GetAt(0);  m_aSurfs.RemoveAt(0);

}

void CRealModelView::OnDebugOnlyMapping()
{
	// TODO: Add your command handler code here
	CSurf* newa = new CSurf();
	CSurf* newb = new CSurf();
	CSurf* sa = m_aSurfs.GetAt(0);
	CSurf* sb = m_aSurfs.GetAt(1);
	CutAndUpdateTwoTrglSurf(sa, sb, newa, newb, myEps);

	CBlock * pBlk = new CBlock();

//	CreatePolyhedronByOnlyMapping(m_aSurfs.GetAt(0), m_aSurfs.GetAt(1), pBlk, myEps);
	CreatePolyhedronByOnlyMapping(newa, newb, pBlk, myEps);

	// add it into global set
	for(int i = 0; i < pBlk->pPhs.GetSize(); i++) {
		m_aPhds.Add(pBlk->pPhs.GetAt(i));
	}
	m_aBlks.Add(pBlk);

	// Emerge
	EmergeBlockGroups();

	// display
	DisplayPolyhedraColor();

	// save
	SaveBlockGroups();

	m_aSurfs.GetAt(1)->Clear();  delete m_aSurfs.GetAt(1);  m_aSurfs.RemoveAt(1);
	m_aSurfs.GetAt(0)->Clear();  delete m_aSurfs.GetAt(0);  m_aSurfs.RemoveAt(0);
}

//2010.11.26 By Mei Gang in freiburg Germany, Copied on 2011.12.7
//To compute the intersections of 2 triangle mesh in 3D and 2D
//parameters: CRegion * pRgn0, CRegion * pRgn1, CBlock * pBlk
//result    : firstly, all the intersections will be saved into m_aBlks
// secondly, transform all elements in M_aBlks to polyhedrons, which will be saved in pBlk->pPhds
void CRealModelView::IntrTriMesh2DAnd3D_Float(CSurf* pRgn0, CSurf* pRgn1, CBlock* pBlk) 
{
#ifndef EGC
	CArray<CNode*, CNode*>pNds;
    CArray<CBlock*, CBlock*>m_aBlks;
	CArray<CPair*, CPair*>m_aPrs2D; 
	CArray<CPair*, CPair*>m_aPrs3D;

	int Add = pRgn0->pTrs.GetSize();

	////intersection in 2D
	int i, j, k, ii, jj, kk, id, tid[3];
	float x, y, z;
	CArray<CTrgl*, CTrgl*>pTrs;
	CTrgl * pTr0; CTrgl * pTr1;
	CArray<CSurf*, CSurf*>Rgns;
	CSurf * r0 = new CSurf();  CSurf * r1 = new CSurf();
	Rgns.Add(r0);  Rgns.Add(r1);

	for(i = 0; i < pRgn0->pTrs.GetSize(); i++) {
		id = pRgn0->pTrs.GetAt(i)->ID;
		for(j = 0; j < 3; j++) tid[j] = pRgn0->pTrs.GetAt(i)->nID[j];
		r0->pTrs.Add(new CTrgl(id, tid[0], tid[1], tid[2]));
		for(j = 0; j < 3; j++) {
			r0->pTrs.GetAt(i)->x[j] = pRgn0->pTrs.GetAt(i)->x[j];
			r0->pTrs.GetAt(i)->y[j] = pRgn0->pTrs.GetAt(i)->y[j];
			r0->pTrs.GetAt(i)->z[j] = 0.00;
		}
	}
	for(i = 0; i < pRgn0->pNds.GetSize(); i++) {
		id = pRgn0->pNds.GetAt(i)->ID;
		x = pRgn0->pNds.GetAt(i)->x; y = pRgn0->pNds.GetAt(i)->y; z = 0.00;
		r0->pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < pRgn0->pNds.GetSize(); i++){ //记录经过每个点的三角形ID 2010.1.29
		for(j = 0; j < pRgn0->pNds.GetAt(i)->sPsdTrgl.GetSize(); j++){
			r0->pNds.GetAt(i)->sPsdTrgl.Add(pRgn0->pNds.GetAt(i)->sPsdTrgl.GetAt(j));
		}
	}
	for(i = 0; i < pRgn0->pTrs.GetSize(); i++){ //记录每个三角形的邻域三角形，不包括自身
		for(j = 0; j < pRgn0->pTrs.GetAt(i)->iRgnPassTr.GetSize(); j++){
			r0->pTrs.GetAt(i)->iRgnPassTr.Add(pRgn0->pTrs.GetAt(i)->iRgnPassTr.GetAt(j));
		}
	}

	for(i = 0; i < pRgn1->pTrs.GetSize(); i++) {
		id = pRgn1->pTrs.GetAt(i)->ID;
		for(j = 0; j < 3; j++) tid[j] = pRgn1->pTrs.GetAt(i)->nID[j];
		r1->pTrs.Add(new CTrgl(id, tid[0], tid[1], tid[2]));
		for(j = 0; j < 3; j++) {
			r1->pTrs.GetAt(i)->x[j] = pRgn1->pTrs.GetAt(i)->x[j];
			r1->pTrs.GetAt(i)->y[j] = pRgn1->pTrs.GetAt(i)->y[j];
			r1->pTrs.GetAt(i)->z[j] = 0.00;
		}
	}
	for(i = 0; i < pRgn1->pNds.GetSize(); i++) {
		id = pRgn1->pNds.GetAt(i)->ID;
		x = pRgn1->pNds.GetAt(i)->x; y = pRgn1->pNds.GetAt(i)->y; z = 0.00;
		r1->pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < pRgn1->pNds.GetSize(); i++){ //记录经过每个点的三角形ID 2010.1.29
		for(j = 0; j < pRgn1->pNds.GetAt(i)->sPsdTrgl.GetSize(); j++){
			r1->pNds.GetAt(i)->sPsdTrgl.Add(pRgn1->pNds.GetAt(i)->sPsdTrgl.GetAt(j));
		}
	}
	for(i = 0; i < pRgn1->pTrs.GetSize(); i++){ //记录每个三角形的邻域三角形，不包括自身
		for(j = 0; j < pRgn1->pTrs.GetAt(i)->iRgnPassTr.GetSize(); j++){
			r1->pTrs.GetAt(i)->iRgnPassTr.Add(pRgn1->pTrs.GetAt(i)->iRgnPassTr.GetAt(j));
		}
	}

	SearchIntrTrgls(r0, r1, 0.0001); // to search the intersected triangles for each triangle

	for(i = 0; i < 2; i++) {
		for(j = 0; j < Rgns.GetAt(i)->pTrs.GetSize(); j++) {
			pTrs.Add( Rgns.GetAt(i)->pTrs.GetAt(j) );
		}
	}

	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {

		// For output window
		CString str;
		str.Format( _T("Intersecting of Triangles in 2D...  i = %d [/ %d]"), i, Rgns.GetAt(0)->pTrs.GetSize());
		pFrame->AddStrToOutputWnd(1, str);
					
		pTr0 = pTrs.GetAt(i);
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CLoop * pIntr = new CLoop();
			IntrTriangle2Triangle2(pTr0, pTr1, pIntr);
			m_aLpsIntr.Add(pIntr); // Save
			pTr0->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
			pTr1->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
		}
	}
	

/* // 有错
	int tt;
	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
		CLoop * pLp0 = new CLoop();
		for(tt = 0; tt < 3; tt++) {
			x = pTr0->x[tt];  y = pTr0->y[tt];  z = pTr0->z[tt];
			pLp0->pNds.Add(new CNode(tt, x, y, z));
		}
		
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CLoop * pIntr = new CLoop();
			
			CLoop * pLp1 = new CLoop();
			for(tt = 0; tt < 3; tt++) {
				x = pTr1->x[tt];  y = pTr1->y[tt];  z = pTr1->z[tt];
				pLp1->pNds.Add(new CNode(tt, x, y, z));
			}
			
			if(IntrOfPolygonAndPolygon(pLp0, pLp1, pIntr) == false)  continue;
			
			m_aLpsIntr.Add(pIntr); // Save
			pTr0->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
			pTr1->iRgnIntrLps.Add( m_aLpsIntr.GetUpperBound() ); // save loop index
		}
	}
	*/
	
	//统一到一个三角形对的集合内2009.12.17
	for(i = 0; i < Rgns.GetAt(0)->pTrs.GetSize(); i++) {
		pTr0 = pTrs.GetAt(i);
		for(j = 0; j < pTr0->iRgnIntrTr.GetSize(); j++) {
			pTr1 = pTrs.GetAt( pTr0->iRgnIntrTr.GetAt(j) );
			CPair * pr = new CPair();
			pr->FstID = pTr0->ID;  pr->SedID = pTr1->ID;
            m_aPrs2D.Add(pr); // 与m_aLpsIntr一一对应
		}
	}

	CombiLps2D(); //2010.2.8, to combine all loops in 2D

	////intersection in 3D
	int t;
	int count, nCase;
	double *nx, *ny, *nz, *para;
	float xmin, ymin, zmin, xmax, ymax, zmax;
	int xdivi, ydivi, zdivi;
	float xtmp, ytmp, ztmp;
	float sum, dstep;
	int n0, n1, n2, n3, n4, n5, n6, n7;
	CArray<CNode*, CNode*>pSonNds;
	CArray<CHehd*, CHehd*>pSonBox;
	CNode * pMin, * pMax;
	CArray<CLine*, CLine*> pLns;
	float v0[3], v1[3], v2[3], u0[3], u1[3], u2[3], isec0[3], isec1[3];
	int copla;
	int * coplanar = &copla;
	
	//Find the smallest and the biggest x, y and z, and form a box;
	count = pRgn0->pNds.GetSize() + pRgn1->pNds.GetSize();
	nx = new double[count];  ny = new double[count];  nz = new double[count];
	for(i = 0; i < pRgn0->pNds.GetSize(); i++) {
		nx[i] = pRgn0->pNds.GetAt(i)->x;
		ny[i] = pRgn0->pNds.GetAt(i)->y;
		nz[i] = pRgn0->pNds.GetAt(i)->z;
	}
	for(i = 0; i < pRgn1->pNds.GetSize(); i++) {
		j = i + pRgn0->pNds.GetSize();
		nx[j] = pRgn1->pNds.GetAt(i)->x;
		ny[j] = pRgn1->pNds.GetAt(i)->y;
		nz[j] = pRgn1->pNds.GetAt(i)->z;
	}
	prhap(nx,count);  prhap(ny,count);  prhap(nz,count);
	xmin = nx[0];        ymin = ny[0];        zmin = nz[0];
	xmax = nx[count-1];  ymax = ny[count-1];  zmax = nz[count-1];
	delete [] nx;  delete [] ny;  delete [] nz;
	
	//Compute all triangles' edges' length, and use it to divide the Box
	sum = 0.0;
	for(i = 0; i < pRgn0->pTrs.GetSize(); i++) {
		CTrgl * tr = pRgn0->pTrs.GetAt(i);
		sum += (tr->x[0] - tr->x[1]) * (tr->x[0] - tr->x[1]) + 
			(tr->y[0] - tr->y[1]) * (tr->y[0] - tr->y[1]) +
			(tr->z[0] - tr->z[1]) * (tr->z[0] - tr->z[1]) ;
        sum += (tr->x[1] - tr->x[2]) * (tr->x[1] - tr->x[2]) + 
			(tr->y[1] - tr->y[2]) * (tr->y[1] - tr->y[2]) +
			(tr->z[1] - tr->z[2]) * (tr->z[1] - tr->z[2]) ;
        sum += (tr->x[2] - tr->x[0]) * (tr->x[2] - tr->x[0]) + 
			(tr->y[2] - tr->y[0]) * (tr->y[2] - tr->y[0]) +
			(tr->z[2] - tr->z[0]) * (tr->z[2] - tr->z[0]) ;
	}
	for(i = 0; i < pRgn1->pTrs.GetSize(); i++) {
		CTrgl * tr = pRgn1->pTrs.GetAt(i);
		sum += (tr->x[0] - tr->x[1]) * (tr->x[0] - tr->x[1]) + 
			(tr->y[0] - tr->y[1]) * (tr->y[0] - tr->y[1]) +
			(tr->z[0] - tr->z[1]) * (tr->z[0] - tr->z[1]) ;
        sum += (tr->x[1] - tr->x[2]) * (tr->x[1] - tr->x[2]) + 
			(tr->y[1] - tr->y[2]) * (tr->y[1] - tr->y[2]) +
			(tr->z[1] - tr->z[2]) * (tr->z[1] - tr->z[2]) ;
        sum += (tr->x[2] - tr->x[0]) * (tr->x[2] - tr->x[0]) + 
			(tr->y[2] - tr->y[0]) * (tr->y[2] - tr->y[0]) +
			(tr->z[2] - tr->z[0]) * (tr->z[2] - tr->z[0]) ;
	}
	sum /= (pRgn0->pTrs.GetSize() + pRgn1->pTrs.GetSize()) * 3; // average length
	dstep = sqrt(sum);
	
	//divisions
	xtmp = (xmax - xmin) / dstep; //x
    xdivi = (int)(xtmp) + 1;
	ytmp = (ymax - ymin) / dstep; //y
    ydivi = (int)(ytmp) + 1;
	ztmp = (zmax - zmin) / dstep; //z
    zdivi = (int)(ztmp) + 1;
	
	// save all son-boxes
	//Nodes
	id = 0;
	for(k = 0; k < zdivi + 1; k++){
		for(j = 0; j < ydivi + 1; j++){
			for(i = 0; i < xdivi + 1; i++){
				x = xmin + i * dstep;    y = ymin + j * dstep;    z = zmin + k * dstep;
				pSonNds.Add(new CNode(id++, x, y, z));
			}
		}
	}
	
	//Son-boxes
	for(k = 0; k < zdivi; k++) {
		for(j = 0; j < ydivi; j++) {
			for(i = 0; i < xdivi; i++) {
				n0 = k * (ydivi + 1) * (xdivi + 1) + j * (xdivi + 1) +i;
				n1=k*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i+1;
				n2=k*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i+1;
				n3=k*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i;
				n4=(k+1)*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i;
				n5=(k+1)*(ydivi+1)*(xdivi+1)+j*(xdivi+1)+i+1;
				n6=(k+1)*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i+1;
				n7=(k+1)*(ydivi+1)*(xdivi+1)+(j+1)*(xdivi+1)+i;
				pSonBox.Add(new CHehd(n0,n1,n2,n3,n4,n5,n6,n7));
			}
		}
	}
	
	//test which triangles intersect each son-box
	for(i = 0; i < pSonBox.GetSize(); i++) {
		pMin = pSonNds.GetAt(pSonBox.GetAt(i)->nID[0]);
		pMax = pSonNds.GetAt(pSonBox.GetAt(i)->nID[6]);
		for(j = 0; j < pRgn0->pTrs.GetSize(); j++) {
			if(IsTrglInsectBox(pRgn0->pTrs.GetAt(j), pMin, pMax) == 1)
				pSonBox.GetAt(i)->iIntrTriRgn0.Add(j);
		}
		for(j = 0; j < pRgn1->pTrs.GetSize(); j++) {
			if(IsTrglInsectBox(pRgn1->pTrs.GetAt(j), pMin, pMax) == 1)
				pSonBox.GetAt(i)->iIntrTriRgn1.Add(j);
		}
	}
	
	//compute the intersection of each triangle pair which is restricted in one son-box
	for(i = 0; i < pSonBox.GetSize(); i++) {
		// For output window
		CString str;
		str.Format( _T("Intersecting of Triangles in 3D...  i = %d [/ %d]"), i, pSonBox.GetSize());
		pFrame->AddStrToOutputWnd(1, str);

		for(j = 0; j < pSonBox.GetAt(i)->iIntrTriRgn0.GetSize(); j++) {
			n0 = pSonBox.GetAt(i)->iIntrTriRgn0.GetAt(j);
			v0[0] = pRgn0->pTrs.GetAt(n0)->x[0]; v0[1] = pRgn0->pTrs.GetAt(n0)->y[0]; v0[2] = pRgn0->pTrs.GetAt(n0)->z[0];
			v1[0] = pRgn0->pTrs.GetAt(n0)->x[1]; v1[1] = pRgn0->pTrs.GetAt(n0)->y[1]; v1[2] = pRgn0->pTrs.GetAt(n0)->z[1];
			v2[0] = pRgn0->pTrs.GetAt(n0)->x[2]; v2[1] = pRgn0->pTrs.GetAt(n0)->y[2]; v2[2] = pRgn0->pTrs.GetAt(n0)->z[2];
			for(k = 0; k < pSonBox.GetAt(i)->iIntrTriRgn1.GetSize(); k++) {
				n1 = pSonBox.GetAt(i)->iIntrTriRgn1.GetAt(k);
				u0[0] = pRgn1->pTrs.GetAt(n1)->x[0]; u0[1] = pRgn1->pTrs.GetAt(n1)->y[0]; u0[2] = pRgn1->pTrs.GetAt(n1)->z[0];
				u1[0] = pRgn1->pTrs.GetAt(n1)->x[1]; u1[1] = pRgn1->pTrs.GetAt(n1)->y[1]; u1[2] = pRgn1->pTrs.GetAt(n1)->z[1];
				u2[0] = pRgn1->pTrs.GetAt(n1)->x[2]; u2[1] = pRgn1->pTrs.GetAt(n1)->y[2]; u2[2] = pRgn1->pTrs.GetAt(n1)->z[2];
				
								// Load function from DLL Moller.dll 2011.9.30
			    typedef int (*lpFun)(float [3],float [3],float [3],
				                 float [3],float [3],float [3],int *,
				                 float [3],float [3]); // 宏定义函数指针类型
	            HINSTANCE hDll; //DLL 句柄
                lpFun trglFun; // 函数指针

	            hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
	
                if (NULL==hDll) {
                    MessageBox(_T("Fail to load DLL Moller.dll ! "));
	            }
	            else {
		             trglFun = (lpFun) GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
                     if (trglFun != NULL) {
					     nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		             }
		             else {
			             MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		             }
                     FreeLibrary(hDll); // 释放函数
	            }
			    // End loading
				

		//		nCase = tri_tri_intersect_with_isectline(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
				
				if(nCase == 1) { //save intersection segment
					if(pLns.GetSize() < 1) { // save directly
						CLine * ptmp = new CLine();
						ptmp->FstTri = n0;  ptmp->SecTri = n1;
						for(t = 0; t < 3; t++) { ptmp->FstNd[t] = isec0[t];  ptmp->SecNd[t] = isec1[t];} //交点坐标
						pLns.Add(ptmp);
					}
					else {
						bool bexist = false;
						for(t = 0; t < pLns.GetSize(); t++){ 
							if(pLns.GetAt(t)->FstTri == n0 && pLns.GetAt(t)->SecTri == n1){ bexist = true;  break; }
						}
						if(!bexist) { // not exist
							CLine * ptmp = new CLine();
							ptmp->FstTri = n0;  ptmp->SecTri = n1;
							for(t = 0; t < 3; t++) { ptmp->FstNd[t] = isec0[t];  ptmp->SecNd[t] = isec1[t];} //交点坐标
							pLns.Add(ptmp);
						}
					}
				}
			}
		}
	}
	
	//统一保存到一个三角形对的集合中。2009.12.17
	for(i = 0; i < pLns.GetSize(); i++) {
		CPair * pr = new CPair();
		pr->FstID = pLns.GetAt(i)->FstTri;  pr->SedID = pLns.GetAt(i)->SecTri;
		m_aPrs3D.Add(pr); //与pLns一一对应
	}
	////end of intersection in 3D 2009.1.22

	//2010.1.22 Mei Gang, Freiburg, Germany
	//2009.12.17 23:30  freiburg germany
	////use the 3D intersecton line to cut the 2D intersection loop
	//step 1: 确定2D相交三角形对有那些同时3D相交
	for(i = 0; i < m_aPrs2D.GetSize(); i++) { //initiate
		m_aPrs2D.GetAt(i)->bIntr2D = true;  m_aPrs2D.GetAt(i)->bIntr3D = false;}
	for(i = 0; i < m_aPrs3D.GetSize(); i++) {
		m_aPrs3D.GetAt(i)->bIntr2D = false; m_aPrs3D.GetAt(i)->bIntr3D = true; }

	for(i = 0; i < m_aPrs2D.GetSize(); i++) { //search out the pairs which intersect in both 2D and 3D
		for(j = 0; j < m_aPrs3D.GetSize(); j++) {
			if(m_aPrs2D.GetAt(i)->FstID == m_aPrs3D.GetAt(j)->FstID &&
			   m_aPrs2D.GetAt(i)->SedID == m_aPrs3D.GetAt(j)->SedID ){
				m_aPrs2D.GetAt(i)->bIntr3D = true;  m_aPrs3D.GetAt(j)->bIntr2D = true;
				break;
			}
		}
	}

	/*
	// output temperarily
	CString fnMesh;
	int NumOfPrs = m_aPrs3D.GetSize();
	if( AfxMessageBox("确定保存Temp文件?", MB_YESNOCANCEL) != 6 ) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, "Data File(*.txt)|*.txt", NULL);
	if( Dlg.DoModal() == IDOK ) 
	{
		fnMesh = Dlg.GetFileName(); 
		fnMesh += "-Temp";
		if(fnMesh.Right(4) != ".txt")   fnMesh += ".txt";
		ofstream fin( fnMesh );
		for(i = 0; i < NumOfPrs; i++){
			fin << i << "  " << m_aPrs3D.GetAt(i)->FstID << "  " << m_aPrs3D.GetAt(i)->SedID;
			fin << endl;
		}
		fin.close(); 
		
	}
	exit(0);
	// end tmp
	*/

	//form the blocks
    CLoop * plp = new CLoop();
	CLoop * pson = new CLoop[2];
	for(i = 0; i < m_aPrs2D.GetSize(); i++) {
		// For output window
		CString str;
		str.Format( _T("Forming Polyhedrons in 2D and 3D...  i = %d [/ %d]"), i, m_aPrs2D.GetSize());
		pFrame->AddStrToOutputWnd(1, str);

		if(m_aLpsIntr.GetAt(i)->pNds.GetSize() < 3)  continue;  //intersection in 2D is a point or a segment

		plp->Clear();  pson[0].Clear();  pson[1].Clear(); // initiate, very important!
		
		CBlock * blk = new CBlock();
		blk->FstTri = m_aPrs2D.GetAt(i)->FstID;  blk->SecTri = m_aPrs2D.GetAt(i)->SedID; //index of the two triangles
		if(!m_aPrs2D.GetAt(i)->bIntr3D) { // intersect only in 2D
			for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
				x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;// at present, just need to save x and y
				y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
				blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
			}
			//update Z, according to the equation of the triangle
			n0 = m_aPrs2D.GetAt(i)->FstID;
			nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
			for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
				nx[k] = pRgn0->pTrs.GetAt(n0)->x[k];
				ny[k] = pRgn0->pTrs.GetAt(n0)->y[k];
				nz[k] = pRgn0->pTrs.GetAt(n0)->z[k];
			}
			EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
			for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
				x = blk->DnLpNds.GetAt(j)->x;
				y = blk->DnLpNds.GetAt(j)->y;
				z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
				blk->DnLpNds.GetAt(j)->z = z;
			}
			delete [] nx;  delete [] ny; delete [] nz; delete [] para;
			
			n1 = m_aPrs2D.GetAt(i)->SedID;
			nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
			for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
				nx[k] = pRgn1->pTrs.GetAt(n1)->x[k];
				ny[k] = pRgn1->pTrs.GetAt(n1)->y[k];
				nz[k] = pRgn1->pTrs.GetAt(n1)->z[k];
			}
			EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
			for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
				x = blk->UpLpNds.GetAt(j)->x;
				y = blk->UpLpNds.GetAt(j)->y;
				z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
				blk->UpLpNds.GetAt(j)->z = z;
			}
			delete [] nx;  delete [] ny; delete [] nz; delete [] para;
			
			/**/
			// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
			// Begin : Check whether each polyhedron is valid
			bool bValid = true;
			float zup = 0.0, zdn = 0.0;
			if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
			//	AfxMessageBox("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()",MB_OK|MB_ICONSTOP);
				return;
			}
			for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
				zup += blk->UpLpNds.GetAt(j)->z;
				zdn += blk->DnLpNds.GetAt(j)->z;
			}
			if(zup > zdn )  
			// End.2010.11.25
			/**/

			m_aBlks.Add(blk);//save
		}
	
		else { // intersect in both 2D and 3D
           
			bool bexist = false;
			n0 = m_aPrs2D.GetAt(i)->FstID;
			v0[0] = pRgn0->pTrs.GetAt(n0)->x[0]; v0[1] = pRgn0->pTrs.GetAt(n0)->y[0]; v0[2] = pRgn0->pTrs.GetAt(n0)->z[0];
			v1[0] = pRgn0->pTrs.GetAt(n0)->x[1]; v1[1] = pRgn0->pTrs.GetAt(n0)->y[1]; v1[2] = pRgn0->pTrs.GetAt(n0)->z[1];
			v2[0] = pRgn0->pTrs.GetAt(n0)->x[2]; v2[1] = pRgn0->pTrs.GetAt(n0)->y[2]; v2[2] = pRgn0->pTrs.GetAt(n0)->z[2];
			
			n1 = m_aPrs2D.GetAt(i)->SedID;
			u0[0] = pRgn1->pTrs.GetAt(n1)->x[0]; u0[1] = pRgn1->pTrs.GetAt(n1)->y[0]; u0[2] = pRgn1->pTrs.GetAt(n1)->z[0];
			u1[0] = pRgn1->pTrs.GetAt(n1)->x[1]; u1[1] = pRgn1->pTrs.GetAt(n1)->y[1]; u1[2] = pRgn1->pTrs.GetAt(n1)->z[1];
			u2[0] = pRgn1->pTrs.GetAt(n1)->x[2]; u2[1] = pRgn1->pTrs.GetAt(n1)->y[2]; u2[2] = pRgn1->pTrs.GetAt(n1)->z[2];
			
											// Load function from DLL Moller.dll 2011.9.30
			    typedef int (*lpFun)(float [3],float [3],float [3],
				                 float [3],float [3],float [3],int *,
				                 float [3],float [3]); // 宏定义函数指针类型
	            HINSTANCE hDll; //DLL 句柄
                lpFun trglFun; // 函数指针

	            hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
	
                if (NULL==hDll) {
                    MessageBox(_T("Fail to load DLL Moller.dll ! "));
	            }
	            else {
		             trglFun = (lpFun) GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
                     if (trglFun != NULL) {
					     nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		             }
		             else {
			             MessageBox(_T("Fail to load function from DLL Moller.dll !"));
		             }
                     FreeLibrary(hDll); // 释放函数
	            }
			    // End loading
				

//			nCase = tri_tri_intersect_with_isectline(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
			
			if(nCase == 1) { //intersection is a segment
	
//	CString str1;
//	str1.Format("n0 = %d, n1 = %d", n0, n1);
//	AfxMessageBox(str1);

				//case 1 : 相交线段恰好为2D相交环上的一条边，只生成一个Block
				for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; j++) {
					x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;
					y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
					z = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->z;
					xtmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->x;
					ytmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->y;
					ztmp = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1)->z;
					
					if( //判断两条线段相同
						(sqrt((x-isec0[0])*(x-isec0[0])+(y-isec0[1])*(y-isec0[1])) < 0.00001 &&
						sqrt((xtmp-isec1[0])*(xtmp-isec1[0])+(ytmp-isec1[1])*(ytmp-isec1[1]) )< 0.00001) 
						||
						(sqrt((x-isec1[0])*(x-isec1[0])+(y-isec1[1])*(y-isec1[1])) < 0.00001 &&
						sqrt((xtmp-isec0[0])*(xtmp-isec0[0])+(ytmp-isec0[1])*(ytmp-isec0[1]) )< 0.00001)
						) {
						bexist = true;
					}
				}
				
				if(bexist) {
                    CBlock * blk = new CBlock();
					for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize(); j++) {
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->x;// at present, just need to save x and y
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(j)->y;
						blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
					}

					//update Z, according to the equation of the triangle
					n0 = m_aPrs2D.GetAt(i)->FstID;
					nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
					for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
						nx[k] = pRgn0->pTrs.GetAt(n0)->x[k];
						ny[k] = pRgn0->pTrs.GetAt(n0)->y[k];
						nz[k] = pRgn0->pTrs.GetAt(n0)->z[k];
					}
					EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
					for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
						x = blk->DnLpNds.GetAt(j)->x;
						y = blk->DnLpNds.GetAt(j)->y;
						z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
						blk->DnLpNds.GetAt(j)->z = z;
					}
					delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
					n1 = m_aPrs2D.GetAt(i)->SedID;
					nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
					for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
						nx[k] = pRgn1->pTrs.GetAt(n1)->x[k];
						ny[k] = pRgn1->pTrs.GetAt(n1)->y[k];
						nz[k] = pRgn1->pTrs.GetAt(n1)->z[k];
					}
					EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
					for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
						x = blk->UpLpNds.GetAt(j)->x;
						y = blk->UpLpNds.GetAt(j)->y;
						z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
						blk->UpLpNds.GetAt(j)->z = z;
					}
					delete [] nx;  delete [] ny; delete [] nz; delete [] para;
					
					/**/
					// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
					// Begin : Check whether each polyhedron is valid
					// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
					// Begin : Check whether each polyhedron is valid
					bool bValid = true;
					float zup = 0.0, zdn = 0.0;
					if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
						AfxMessageBox(_T("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()"),MB_OK|MB_ICONSTOP);
						return;
					}
					for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
						zup += blk->UpLpNds.GetAt(j)->z;
						zdn += blk->DnLpNds.GetAt(j)->z;
					}
					if(zup > zdn )  
					// End.2010.11.25
					/**/
						
					m_aBlks.Add(blk);//save

				}
				else { //case 2 : 相交线段将2D相交环分割为两个子环，生成二个Block
					// 2011.5.21 一下代码有问题，目前为解决！

//					OutputIntr();  exit(0);
					
					int pos[2];//index of the position
					//以isec0所在的线段为新环的起始线段
					for(j = 0; j < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; j++) {
						CNode * sp0 = m_aLpsIntr.GetAt(i)->pNds.GetAt(j);
						CNode * sp1 = m_aLpsIntr.GetAt(i)->pNds.GetAt(j+1);
						CNode * point = new CNode(isec0[0],isec0[1]);
						float p0x = (float) sp0->x;  float p0y = (float) sp0->y;
						float p1x = (float) sp1->x;  float p1y = (float) sp1->y;
						if(IsPointOnSegment(p0x, p0y, p1x, p1y, isec0[0], isec0[1], 0.01) == true) {
							pos[0] = j;  break;
						}
					}

					plp->Clear();
					for(t = pos[0]; t < m_aLpsIntr.GetAt(i)->pNds.GetSize() - 1; t++) {//没有保存最后一个点
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->x;
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->y;
						plp->pNds.Add(new CNode(x,y));
					}
					for(t = 0; t < pos[0]; t++) {//保存了第一个点
						x = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->x;
						y = m_aLpsIntr.GetAt(i)->pNds.GetAt(t)->y;
						plp->pNds.Add(new CNode(x,y));
					}
					pos[0] = 0; //update
					plp->pNds.Add(new CNode(plp->pNds.GetAt(0)->x,plp->pNds.GetAt(0)->y)); //最合一个点要和第一个点相同
					
					//isec1的位置
					for(j = 0; j < plp->pNds.GetSize() - 1; j++) {
						CNode * sp0 = plp->pNds.GetAt(j);
						CNode * sp1 = plp->pNds.GetAt(j+1);
						float p0x = (float) sp0->x;  float p0y = (float) sp0->y;
						float p1x = (float) sp1->x;  float p1y = (float) sp1->y;
						if(IsPointOnSegment(p0x, p0y, p1x, p1y, isec1[0], isec1[1], 0.01) == true) {
							pos[1] = j;  break;
						}
					}
					
					//form 2 son loops
					pson[0].pNds.Add(new CNode(isec0[0], isec0[1])); //first point
					for(t = 0; t < pos[1]; t++) {
						pson[0].pNds.Add(new CNode(plp->pNds.GetAt(t + 1)->x, plp->pNds.GetAt(t + 1)->y)); //must + 1
					}
                    pson[0].pNds.Add(new CNode(isec1[0], isec1[1])); //last point
					pson[0].pNds.Add(new CNode(isec0[0], isec0[1])); //first point again
					
					pson[1].pNds.Add(new CNode(isec0[0], isec0[1])); //first point
					pson[1].pNds.Add(new CNode(isec1[0], isec1[1])); //second point
					for(t = pos[1]; t < plp->pNds.GetSize() - 1; t++) {
						pson[1].pNds.Add(new CNode(plp->pNds.GetAt(t + 1)->x, plp->pNds.GetAt(t + 1)->y)); //must + 1
					}
					pson[1].pNds.Add(new CNode(isec0[0], isec0[1])); //first point again
					
					//update Z and form blocks
					//m_aBlks
					for(ii = 0; ii < 2; ii++) {
						CBlock * blk = new CBlock();
						for(jj = 0; jj < pson[ii].pNds.GetSize(); jj++) {
							x = pson[ii].pNds.GetAt(jj)->x;  y = pson[ii].pNds.GetAt(jj)->y;
							blk->UpLpNds.Add(new CNode(x,y));  blk->DnLpNds.Add(new CNode(x,y));
						}
						//update Z, according to the equation of the triangle
						n0 = m_aPrs2D.GetAt(i)->FstID;
						nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
						for(kk = 0; kk < 3; kk++) { // real coordinates of the triangle, z is not zero
							nx[kk] = pRgn0->pTrs.GetAt(n0)->x[kk];
							ny[kk] = pRgn0->pTrs.GetAt(n0)->y[kk];
							nz[kk] = pRgn0->pTrs.GetAt(n0)->z[kk];
						}
						EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
						for(jj = 0; jj < blk->DnLpNds.GetSize(); jj++) {
							x = blk->DnLpNds.GetAt(jj)->x;
							y = blk->DnLpNds.GetAt(jj)->y;
							z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
							blk->DnLpNds.GetAt(jj)->z = z;
						}
						delete [] nx;  delete [] ny; delete [] nz; delete [] para;
						
						n1 = m_aPrs2D.GetAt(i)->SedID;
						nx = new double[3]; ny = new double[3]; nz = new double[3]; para = new double[4];
						for(kk = 0; kk < 3; kk++) { // real coordinates of the triangle, z is not zero
							nx[kk] = pRgn1->pTrs.GetAt(n1)->x[kk];
							ny[kk] = pRgn1->pTrs.GetAt(n1)->y[kk];
							nz[kk] = pRgn1->pTrs.GetAt(n1)->z[kk];
						}
						EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
						for(jj = 0; jj < blk->UpLpNds.GetSize(); jj++) {
							x = blk->UpLpNds.GetAt(jj)->x;
							y = blk->UpLpNds.GetAt(jj)->y;
							z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
							blk->UpLpNds.GetAt(jj)->z = z;
						}
						delete [] nx;  delete [] ny; delete [] nz; delete [] para;
						
						/**/
						// 2010.11.25 by Mei Gang at University of Freiburg, Germany.
						// Begin : Check whether each polyhedron is valid
						bool bValid = true;
						float zup = 0.0, zdn = 0.0;
						if(blk->UpLpNds.GetSize() != blk->DnLpNds.GetSize()) {
							AfxMessageBox(_T("UpLpNds != DnLpNds ! \n Function : IntrTriMesh2DAnd3D()"),MB_OK|MB_ICONSTOP);
							return;
						}
						for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
							zup += blk->UpLpNds.GetAt(j)->z;
							zdn += blk->DnLpNds.GetAt(j)->z;
						}
						if(zup > zdn )  
						// End.2010.11.25
						/**/

						m_aBlks.Add(blk);
					}

					
				} // end else
			}
		}
	
	}

	// 2011.5.20 注释
	// 原因：当相交的两个面为平面时，以下代码不会产生问题；当为曲面时，出现错误，故注释
	/*
	//避免实际在平面上的点不落在同一个平面上，统一计算相同面上点的Z 2010.2.7
	//update Z, according to the equation of the triangle
	n0 = pRgn0->pTrs.GetSize() - 1;
	nx = new float[3]; ny = new float[3]; nz = new float[3]; para = new float[4];
	for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
		nx[k] = pRgn0->pTrs.GetAt(n0)->x[k];
		ny[k] = pRgn0->pTrs.GetAt(n0)->y[k];
		nz[k] = pRgn0->pTrs.GetAt(n0)->z[k];
	}
	EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(j = 0; j < blk->DnLpNds.GetSize(); j++) {
			x = blk->DnLpNds.GetAt(j)->x;
			y = blk->DnLpNds.GetAt(j)->y;
			z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
			blk->DnLpNds.GetAt(j)->z = z;
		}
	}
	delete [] nx;  delete [] ny; delete [] nz; delete [] para;
	
	n1 = pRgn1->pTrs.GetSize() - 1;
	nx = new float[3]; ny = new float[3]; nz = new float[3]; para = new float[4];
	for(k = 0; k < 3; k++) { // real coordinates of the triangle, z is not zero
		nx[k] = pRgn1->pTrs.GetAt(n1)->x[k];
		ny[k] = pRgn1->pTrs.GetAt(n1)->y[k];
		nz[k] = pRgn1->pTrs.GetAt(n1)->z[k];
	}
	EquaOfTrgl(nx, ny, nz, para);//可能有问题，不适用垂直于XY面的三角形
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CBlock * blk = m_aBlks.GetAt(i);
		for(j = 0; j < blk->UpLpNds.GetSize(); j++) {
			x = blk->UpLpNds.GetAt(j)->x;
			y = blk->UpLpNds.GetAt(j)->y;
			z = -1 * (para[0] * x + para[1] * y + para[3]) / para[2];
			blk->UpLpNds.GetAt(j)->z = z;
		}
	}
	delete [] nx;  delete [] ny; delete [] nz; delete [] para;
	*/

	//2010.1.14
	//emerge all the blocks , and then rearrange and update them
	for(i = 0; i < m_aBlks.GetSize(); i++) { // original nodes of blocks
		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID;
			x = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->x;
			y = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->y;
			z = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->z;
			pNds.Add(new CNode(id, x, y, z));
		}
		for(j = 0; j < m_aBlks.GetAt(i)->DnLpNds.GetSize(); j++) {
			id = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID;
			x = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->x;
			y = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->y;
			z = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->z;
			pNds.Add(new CNode(id, x, y, z));
		}
	}

	//sort the nodes
	int cnt_1, idx; // int count

	//get number of nodes on rgns
	cnt_1 = pNds.GetSize(); idx = 0;

	//copy nodes
	CNode* p = new CNode[cnt_1];
	for(j = 0; j < pNds.GetSize(); j++){
		p[idx].ID = idx;	
		p[idx].x = pNds.GetAt(j)->x; p[idx].y = pNds.GetAt(j)->y; p[idx].z = pNds.GetAt(j)->z;	
		idx++;
	}
	for(i = cnt_1 - 1; i >= 0; i--){
		delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll();  
	//sort nodes
    SortNode(p, cnt_1);

	//re-arrange nodes
	int* iID = new int[cnt_1];	int* nID = new int[cnt_1];
	for(i = 0; i < cnt_1; i++)iID[i] = p[i].ID;

	nID[0] = 0; idx = 0;

	pNds.Add(new CNode(idx, p[0].x, p[0].y, p[0].z)); 

	//save each node for one time
	bool boolexist;
	for(i = 1; i < cnt_1; i++){
		boolexist = false;
		for(j = 0; j < pNds.GetSize(); j++) {
			if(IsSameNode(&p[i], pNds.GetAt(j), 0.001)){ boolexist = true;  break; }
		}
		if(!boolexist) pNds.Add(new CNode(pNds.GetSize(), p[i].x, p[i].y, p[i].z));
	}
	delete [] p;

	delete[] iID;  delete[] nID;
    
	//最直接、最笨的方法合并节点
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		for(j = 0; j < m_aBlks.GetAt(i)->DnLpNds.GetSize(); j++) {
			for(k = 0; k < pNds.GetSize(); k++) {
				if(IsSameNode(m_aBlks.GetAt(i)->DnLpNds.GetAt(j), pNds.GetAt(k), 0.001)) {
					m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID = k;  break;
				}
			}
		}
		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize(); j++) {
			for(k = 0; k < pNds.GetSize(); k++) {
				if(IsSameNode(m_aBlks.GetAt(i)->UpLpNds.GetAt(j), pNds.GetAt(k), 0.001)) {
					m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID = k;  break;
				}
			}
		}
	}

	// transform from block to polyhedron, now a block includes a groups of polyhedrons
	// not just represents only one polyhedron
	for(i = 0; i < m_aBlks.GetSize(); i++ ) { //根据拓扑关系生成Areas
		CLoop * up = new CLoop();
		for(t = 0; t < m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; t++) { //首尾点相同，去掉最后一个点
			up->iNds.Add(m_aBlks.GetAt(i)->UpLpNds.GetAt(t)->ID);
		}
		CLoop * dn = new CLoop();
		for(t = 0; t < m_aBlks.GetAt(i)->DnLpNds.GetSize() - 1; t++) { //首尾点相同，去掉最后一个点
			dn->iNds.Add(m_aBlks.GetAt(i)->DnLpNds.GetAt(t)->ID);
		}

		m_aBlks.GetAt(i)->pLps.Add(up);  m_aBlks.GetAt(i)->pLps.Add(dn);

		for(j = 0; j < m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; j++) {
			CLoop * tmp = new CLoop();
			n0 = m_aBlks.GetAt(i)->DnLpNds.GetAt(j)->ID;
			n1 = m_aBlks.GetAt(i)->DnLpNds.GetAt(j + 1)->ID;
			n2 = m_aBlks.GetAt(i)->UpLpNds.GetAt(j + 1)->ID;
			n3 = m_aBlks.GetAt(i)->UpLpNds.GetAt(j)->ID;

			if(n0!= n3 && n1 != n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n2); tmp->iNds.Add(n3);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
			if(n0 != n3 && n1 == n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n3);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
			if(n0 == n3 && n1 != n2) {
				tmp->iNds.Add(n0); tmp->iNds.Add(n1); tmp->iNds.Add(n2);
				m_aBlks.GetAt(i)->pLps.Add(tmp);
			}
		}
	}

	// Begin : transform from block to polyhedron
	for(i = 0; i < m_aBlks.GetSize(); i++) {
		CPohd * phd = new CPohd();
		for(j = 0; j < m_aBlks.GetAt(i)->pLps.GetSize(); j++) {
			CLoop * lp = new CLoop();
			for(k = 0; k < m_aBlks.GetAt(i)->pLps.GetAt(j)->iNds.GetSize(); k++) {
				id = m_aBlks.GetAt(i)->pLps.GetAt(j)->iNds.GetAt(k);
				x = pNds.GetAt(id)->x;  y = pNds.GetAt(id)->y;  z = pNds.GetAt(id)->z;
				lp->pNds.Add(new CNode(id, x, y, z));
			}
			phd->pLps.Add(lp);
		}
		pBlk->pPhs.Add(phd);
	}
	// End

	////////Remove the memory
	cnt_1 = pNds.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); delete pNds.GetAt(i); pNds.RemoveAt(i);    
	}
	pNds.RemoveAll(); 
	
	cnt_1 = m_aBlks.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		for(j = m_aBlks.GetAt(i)->UpLpNds.GetSize() - 1; j >= 0; j--){
			delete m_aBlks.GetAt(i)->UpLpNds.GetAt(j);  m_aBlks.GetAt(i)->UpLpNds.RemoveAt(j);
		}
		for(j = m_aBlks.GetAt(i)->DnLpNds.GetSize() - 1; j >= 0; j--){
			delete m_aBlks.GetAt(i)->DnLpNds.GetAt(j);  m_aBlks.GetAt(i)->DnLpNds.RemoveAt(j);
		}
	}
	cnt_1 = m_aBlks.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		m_aBlks.GetAt(i)->Clear(); delete m_aBlks.GetAt(i); m_aBlks.RemoveAt(i);    
	}
	m_aBlks.RemoveAll(); 
	cnt_1 = m_aPrs2D.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		delete m_aPrs2D.GetAt(i);  m_aPrs2D.RemoveAt(i);    
	}
	m_aPrs2D.RemoveAll();
	cnt_1 = m_aPrs3D.GetSize();
	for(i = cnt_1 - 1; i >= 0; i--){
		delete m_aPrs3D.GetAt(i);  m_aPrs3D.RemoveAt(i);    
	}
	m_aPrs3D.RemoveAll(); 
	
	Rgns.GetAt(1)->Clear();  delete Rgns.GetAt(1);  Rgns.RemoveAt(1);
	Rgns.GetAt(0)->Clear();  delete Rgns.GetAt(0);  Rgns.RemoveAt(0);

#else

#endif
	
}

//2009.12.17 accepted from "RockModel"
//the surface equation of a trgl: Ax + By + Cz + D = 0
void CRealModelView::EquaOfTrgl(float *x, float *y, float *z, float *Para)
{
	float x1, y1, z1, x2, y2, z2;
	float A, B, C, D;
	float factor;

	x1 = x[1] - x[0];	y1 = y[1] - y[0];	z1 = z[1] - z[0];
	x2 = x[2] - x[0];	y2 = y[2] - y[0];	z2 = z[2] - z[0];

	A = y1 * z2 - y2 * z1;
	B = x2 * z1 - x1 * z2;
	C = x1 * y2 - x2 * y1;
	D = A * x[0] + B * y[0] + C * z[0];
	D = -1 * D;
	factor = sqrt(A * A + B * B + C * C);
	if(D > 0.0)  factor = -1 * factor;
	Para[0] = A / factor; Para[1] = B / factor; Para[2] = C / factor; Para[3] = D / factor;

}

// 2011.12.7
// Try to intersect 2 surface with float point
void CRealModelView::OnDebugIntersectFloat()
{
	// TODO: Add your command handler code here
	CBlock * pBlk = new CBlock();

	// Intersect in float-point
	IntrTriMesh2DAnd3D_Float(m_aSurfs.GetAt(0), m_aSurfs.GetAt(1), pBlk);

	// add it into global set
	for(int i = 0; i < pBlk->pPhs.GetSize(); i++) {
		m_aPhds.Add(pBlk->pPhs.GetAt(i));
	}
	m_aBlks.Add(pBlk);

	// Emerge
	EmergeBlockGroups();

	// display
	DisplayPolyhedraColor();

	// save
	SaveBlockGroups();

	m_aSurfs.GetAt(1)->Clear();  delete m_aSurfs.GetAt(1);  m_aSurfs.RemoveAt(1);
	m_aSurfs.GetAt(0)->Clear();  delete m_aSurfs.GetAt(0);  m_aSurfs.RemoveAt(0);
}

// For Select  2011.12.14
void CRealModelView::OnSelectEntities()
{
	// TODO: Add your command handler code here
	
	CSelOption* pSelOpt = new CSelOption;
	pSelOpt->Create(IDD_SEL_OPTION);
	pSelOpt->ShowWindow(SW_RESTORE);
	
}


void CRealModelView::OnSelectEverything()
{
	// TODO: Add your command handler code here
	/*
	CSelNumPick* pSelNum = new CSelNumPick;
	pSelNum->Create(IDD_SEL_NUM_PICK);
	pSelNum->ShowWindow(SW_RESTORE);
	
	
	CNotepadDlg* pNotepad = new CNotepadDlg;
	pNotepad->Create(IDD_NOTEPAD);
	pNotepad->ShowWindow(SW_SHOW);
	*/
	/*
	m_nSelectedBlocks.Add(1);
	Replot(SELECT_BLOCK);
	*/

	CSelNumPick* pSelNum = new CSelNumPick;
	pSelNum->Create(IDD_SEL_NUM_PICK);
	pSelNum->ShowWindow(SW_RESTORE);
	


	
}


void CRealModelView::Replot(int nSelectObj)
{
	switch (nSelectObj) {
	case SELECT_POINT:
		DisplaySelectedPoint();
		break;
	case SELECT_LINE:
		DisplaySelectedLine();
		break;
	case SELECT_AREA:
		DisplaySelectedArea();
		break;
	case SELECT_SURFACE:
		DisplaySelectedSurface();
		break;
	case SELECT_POLYHEDRA:
		DisplaySelectedPolyhedra();
		break;
	case SELECT_BLOCK:
		DisplaySelectedBlock();
		break;
	default:
		AfxMessageBox(_T("Error: No Object is selected !"));

	}

}

void CRealModelView::DisplaySelectedPoint()
{
#ifndef EGC
	int nNd = m_nSelectedPoints.GetSize();
	if(nNd == 0)  return;

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	CCordnt* Crt = pCrt;

	glDeleteLists(1, 1);
	GLuint list = glGenLists(1);

	int i, j, id_Nds;
	double x, y, z;
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	glPointSize(3.5f);
	glBegin(GL_POINTS);
	for(i = 0; i < m_nSelectedPoints.GetSize(); i++) {
		id_Nds = m_nSelectedPoints.GetAt(i);
		x = m_aNds.GetAt(id_Nds)->x; 
		y = m_aNds.GetAt(id_Nds)->y;  
		z = m_aNds.GetAt(id_Nds)->z; 
		x = (x - Crt->u) * Crt->Scale;	
		y = (y - Crt->v) * Crt->Scale;	
		z = (z - Crt->w) * Crt->Scale;

		// Create color
		CreateRGBColor(RGB, i);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glVertex3d(x, y, z);
	}
	glEnd();
	glEndList();

	m_bOpenGL = 1;
	Invalidate(TRUE);

#else
	int nNd = m_nSelectedPoints.GetSize();
	if(nNd == 0)  return;

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	CCordnt* Crt = pCrt;

	glDeleteLists(1, 1);
	GLuint list = glGenLists(1);

	int i, j, id_Nds;
	float x, y, z;
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	glPointSize(3.5f);
	glBegin(GL_POINTS);
	for(i = 0; i < m_nSelectedPoints.GetSize(); i++) {
		id_Nds = m_nSelectedPoints.GetAt(i);
		x = m_aNds.GetAt(id_Nds)->x.floatValue(); 
		y = m_aNds.GetAt(id_Nds)->y.floatValue();  
		z = m_aNds.GetAt(id_Nds)->z.floatValue(); 
		x = (x - Crt->u) * Crt->Scale;	
		y = (y - Crt->v) * Crt->Scale;	
		z = (z - Crt->w) * Crt->Scale;

		// Create color
		CreateRGBColor(RGB, i);
		glColor3ub(RGB[0], RGB[1], RGB[2]);
		glVertex3d(x, y, z);
	}
	glEnd();
	glEndList();

	m_bOpenGL = 1;
	Invalidate(TRUE);

#endif

}

void CRealModelView::DisplaySelectedLine()
{

}

void CRealModelView::DisplaySelectedArea()
{
#ifndef EGC
	int nLp = m_nSelectedAreas.GetSize();
	if(nLp == 0)  return;

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	CCordnt* Crt = pCrt;

	glDeleteLists(1, 1);
	GLuint list = glGenLists(1);

	int i, j, k, t, n, id_Nds, id_Lps, id_Phd;
	double * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);

			for(k = 0; k < m_nSelectedAreas.GetSize(); k++) {
				id_Lps = m_nSelectedAreas.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new double[lp->iNds.GetSize()];
				y = new double[lp->iNds.GetSize()];
				z = new double[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x; 
					y[t] = m_aNds.GetAt(n)->y;  
					z[t] = m_aNds.GetAt(n)->z; 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				CreateRGBColor(RGB, id_Lps);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		
	glEndList();

#else
	int nLp = m_nSelectedAreas.GetSize();
	if(nLp == 0)  return;

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	CCordnt* Crt = pCrt;

	glDeleteLists(1, 1);
	GLuint list = glGenLists(1);

	int i, j, k, t, n, id_Nds, id_Lps, id_Phd;
	float * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];

	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);

			for(k = 0; k < m_nSelectedAreas.GetSize(); k++) {
				id_Lps = m_nSelectedAreas.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new float[lp->iNds.GetSize()];
				y = new float[lp->iNds.GetSize()];
				z = new float[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x.floatValue(); 
					y[t] = m_aNds.GetAt(n)->y.floatValue();  
					z[t] = m_aNds.GetAt(n)->z.floatValue(); 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				CreateRGBColor(RGB, id_Lps);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		
	glEndList();

#endif

}

void CRealModelView::DisplaySelectedSurface()
{
#ifndef EGC
	// Creat color
	for(int i = 0; i < m_aSurfs.GetSize(); i++) {
		m_aSurfs.GetAt(i)->ID = i;
		CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	}
	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	glDeleteLists(1, m_aSurfs.GetSize()+m_aBlks.GetSize()); // Clear old lists
	int nSurface = m_nSelectedSurfaces.GetSize();
	int * list = new int[nSurface];
	for(int j = 0; j < nSurface; j++){
		list[j] = j+1;
		int id = m_nSelectedSurfaces.GetAt(j);
		pSurf = m_aSurfs.GetAt(id);
		glCompileTinList(pSurf, pCrt, list[j], 0); // edge and elements, new
	}

	m_bOpenGL = 1;
	Invalidate(TRUE);
	delete[] list;

#else
	// Creat color
	for(int i = 0; i < m_aSurfs.GetSize(); i++) {
		m_aSurfs.GetAt(i)->ID = i;
		CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	}
	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	glDeleteLists(1, m_aSurfs.GetSize()+m_aBlks.GetSize()); // Clear old lists
	int nSurface = m_nSelectedSurfaces.GetSize();
	int * list = new int[nSurface];
	for(int j = 0; j < nSurface; j++){
		list[j] = j+1;
		int id = m_nSelectedSurfaces.GetAt(j);
		pSurf = m_aSurfs.GetAt(id);
		glCompileTinList(pSurf, pCrt, list[j], 0); // edge and elements, new
	}

	m_bOpenGL = 1;
	Invalidate(TRUE);
	delete[] list;

#endif

}

void CRealModelView::DisplaySelectedPolyhedra()
{
#ifndef EGC
	int nPhd = m_nSelectedPolyhedrons.GetSize();
	if(nPhd == 0)  return;

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	CCordnt* Crt = pCrt;

	glDeleteLists(1, 1);
	GLuint list = glGenLists(1);

	// Compile a list
	int i, j, k, t, n, id, id_Nds, id_Lps, id_Phd;
	double * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	int lf = 1;
	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);

		for(j = 0; j < m_nSelectedPolyhedrons.GetSize(); j++) {
			id_Phd = m_nSelectedPolyhedrons.GetAt(j);
			CreateRGBColor(RGB, id_Phd); // create color for each polyhedron
			
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new double[lp->iNds.GetSize()];
				y = new double[lp->iNds.GetSize()];
				z = new double[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x; 
					y[t] = m_aNds.GetAt(n)->y;  
					z[t] = m_aNds.GetAt(n)->z; 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  
						glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  
						glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
	
	glEndList();

	m_bOpenGL = 1;
	Invalidate(TRUE);

#else
	int nPhd = m_nSelectedPolyhedrons.GetSize();
	if(nPhd == 0)  return;

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	CCordnt* Crt = pCrt;

	glDeleteLists(1, 1);
	GLuint list = glGenLists(1);

	// Compile a list
	int i, j, k, t, n, id, id_Nds, id_Lps, id_Phd;
	float * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	int lf = 1;
	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);

		for(j = 0; j < m_nSelectedPolyhedrons.GetSize(); j++) {
			id_Phd = m_nSelectedPolyhedrons.GetAt(j);
			CreateRGBColor(RGB, id_Phd); // create color for each polyhedron
			
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new float[lp->iNds.GetSize()];
				y = new float[lp->iNds.GetSize()];
				z = new float[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x.floatValue(); 
					y[t] = m_aNds.GetAt(n)->y.floatValue();  
					z[t] = m_aNds.GetAt(n)->z.floatValue(); 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  
						glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  
						glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
	
	glEndList();

	m_bOpenGL = 1;
	Invalidate(TRUE);

#endif


}

void CRealModelView::DisplaySelectedBlock()
{
#ifndef EGC
	int nBlock = m_nSelectedBlocks.GetSize();
	if(nBlock == 0)  return;

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	CCordnt* Crt = pCrt;

	glDeleteLists(1, 1);
	GLuint list = glGenLists(1);

	// Compile a list
	int i, j, k, t, n, id, id_Nds, id_Lps, id_Phd;
	double * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	int lf = 0;
	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	for(i = 0; i < m_nSelectedBlocks.GetSize(); i++) {
		id = m_nSelectedBlocks.GetAt(i);
		CBlock * blk = m_aBlks.GetAt(id);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new double[lp->iNds.GetSize()];
				y = new double[lp->iNds.GetSize()];
				z = new double[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x; 
					y[t] = m_aNds.GetAt(n)->y;  
					z[t] = m_aNds.GetAt(n)->z; 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  
						glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  
						glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
	}
	
	glEndList();

	m_bOpenGL = 1;
	Invalidate(TRUE);

#else
	int nBlock = m_nSelectedBlocks.GetSize();
	if(nBlock == 0)  return;

	if(pCrt == NULL) {
	   pCrt = new CCordnt();
       pCrt = GetNewCordntSystemBlockGroup();
	}
	CCordnt* Crt = pCrt;

	glDeleteLists(1, 1);
	GLuint list = glGenLists(1);

	// Compile a list
	int i, j, k, t, n, id, id_Nds, id_Lps, id_Phd;
	float * x, * y, * z;
	GLdouble fPoint0[3], fPoint1[3], fPoint2[3], fNormal[3];
	GLint RGB[3];
	int lf = 0;
	glMyLight();

	glNewList(list, GL_COMPILE);
	glLineWidth(2.0);
	for(i = 0; i < m_nSelectedBlocks.GetSize(); i++) {
		id = m_nSelectedBlocks.GetAt(i);
		CBlock * blk = m_aBlks.GetAt(id);
		for(t = 0; t < 3; t++) { 
			RGB[t] = blk->RGB[t];
		}
		for(j = 0; j < blk->iPhds.GetSize(); j++) {
            // create color for each polyhedron
			if( lf == 1 ) {
				CreateRGBColor(RGB, j);
			}

			id_Phd = blk->iPhds.GetAt(j);
			CPohd * phd = m_aPhds.GetAt(id_Phd);
			for(k = 0; k < phd->iLps.GetSize(); k++) {
				id_Lps = phd->iLps.GetAt(k);
				CLoop * lp = m_aLps.GetAt(id_Lps);
				
				//get the index and coordinates of nodes
				x = new float[lp->iNds.GetSize()];
				y = new float[lp->iNds.GetSize()];
				z = new float[lp->iNds.GetSize()];
				for(t = 0; t < lp->iNds.GetSize(); t++){
					n = lp->iNds.GetAt(t);  
					x[t] = m_aNds.GetAt(n)->x.floatValue(); 
					y[t] = m_aNds.GetAt(n)->y.floatValue();  
					z[t] = m_aNds.GetAt(n)->z.floatValue(); 
					x[t] = (x[t] - Crt->u) * Crt->Scale;	
					y[t] = (y[t] - Crt->v) * Crt->Scale;	
					z[t] = (z[t] - Crt->w) * Crt->Scale;
				}
				
				/*
				//calculate the normal. can be opened or closed, different effect!
				fPoint0[0] = x[0]; fPoint0[1] = y[0]; fPoint0[2] = z[0];
				fPoint1[0] = x[1]; fPoint1[1] = y[1]; fPoint1[2] = z[1];
				fPoint2[0] = x[2]; fPoint2[1] = y[2]; fPoint2[2] = z[2];
				CalculateNormal(fPoint0,fPoint1,fPoint2,fNormal);
				glNormal3dv(fNormal);
				*/
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ub(RGB[0], RGB[1], RGB[2]);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  
						glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ub(255, 255, 255);
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				{
					for(t = 0; t < lp->iNds.GetSize(); t++)  
						glVertex3d(x[t], y[t], z[t]);
				}	
				glEnd();
				
				delete [] x;  delete [] y;  delete [] z;
			}
		}
	}
	
	glEndList();

	m_bOpenGL = 1;
	Invalidate(TRUE);

#endif

}

// CSelNumPick message handlers
int CRealModelView::CStringToInt(CString str, int* Number) 
{ 
	int nLength = str.GetLength();
	int Position = 0; 
	int NumberOfArrary = 0; 
	CString tmp = str; // 临时字符串
	CString Cur; //当前要转换的数字   

	while(1) 
    { 
		Position = tmp.Find(',', 0);//寻找字符串中间的   , 
		if(Position != -1)//判断是否还存在   ， 
		{ 
			Cur = tmp.Left(Position);//得到字符串最左 
			//边的数字 
			char *chr=new char[Cur.GetLength()];
            WideCharToMultiByte(CP_ACP,0,Cur.GetBuffer(),-1,chr,Cur.GetLength(),NULL,NULL);
	        Cur.ReleaseBuffer();

			Number[NumberOfArrary]   =   atoi(chr);//转换 
			nLength = tmp.GetLength();
			tmp = tmp.Right(nLength - Position - 1);//得到剩余的字符串 
			NumberOfArrary++;  
		} 
		else 
		{ 
			Cur = tmp; 
			char *chr=new char[Cur.GetLength()];
            WideCharToMultiByte(CP_ACP,0,Cur.GetBuffer(),-1,chr,Cur.GetLength(),NULL,NULL);
	        Cur.ReleaseBuffer();

			Number[NumberOfArrary]   =   atoi(chr); 
			break; 
		}   
    }   

	return NumberOfArrary + 1;

} 

// Define properties for blocks
void CRealModelView::OnSettingsPropDef()
{
	CPropDef* pPropDef = new CPropDef;
	pPropDef->Create(IDD_PROP_DEF);
	pPropDef->ShowWindow(SW_RESTORE);
}

// Set properties for blocks
void CRealModelView::OnSettingsPropSet()
{
	CPropSet* pPropSet = new CPropSet;
	pPropSet->Create(IDD_PROP_SET);
	pPropSet->ShowWindow(SW_RESTORE);
}

// Save the list of properties
void CRealModelView::OnSettingsSaveProp()
{
	SaveProperty();
}

// Load the list of properties
void CRealModelView::OnSettingsLoadProp()
{
	OpenProperty();
}

void CRealModelView::OnDebugReadDataProp()
{
	// TODO: Add your command handler code here
	// ReadDataProperty();
}

// 2012.2.9 Import .OFF file
void CRealModelView::OnMeshImportOff()
{
//	pSurf = new CSurf(); //  must be a global varible
	CSurf* pSurf = new CSurf(); //  must be a global varible
	ImportOFF(pSurf);  // OFF file format
	m_aSurfs.Add(pSurf);
	
	// Creat color
	for(int i = 0; i < m_aSurfs.GetSize(); i++) {
		m_aSurfs.GetAt(i)->ID = i;
		CreateRGBColor(m_aSurfs.GetAt(i)->RGB, i+m_aBlks.GetSize() );
	}
	pCrt = new CCordnt();
    pCrt = GetNewCordntSystem(pSurf);
	int nSurf = m_aSurfs.GetSize();
	int * list = new int[nSurf];
	for(int j = 0; j < nSurf; j++){
		list[j] = j+1;
		pSurf = m_aSurfs.GetAt(j);
		glCompilePolygonSurfaceList(pSurf, pCrt, list[j], 0); // edge and elements, new
	}

	m_bOpenGL = 1;
	Invalidate(TRUE);
	delete[] list;
}

// 2012.2.9 Import .OFF file
void CRealModelView::ImportOFF(CSurf* surf)
{
	int i, j, id, num, ndata;
	double x, y, z;
	ndata = AfxMessageBox(_T("Import .OFF format file ?"), MB_YESNOCANCEL);
	if(ndata == 6){
		CFileDialog Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("OFF File(*.off)|*.off|All Files (*.*)|*.*"), NULL);
    	if(Dlg.DoModal() == IDOK){
			ifstream fin(Dlg.GetPathName());
			
				string tStr;
				int nVertex, nFace, nEdge;
				fin >> tStr >> nVertex >> nFace >> nEdge ; 
				for(i = 0; i < nVertex; i++) {
					fin >> x >> y >> z;
					surf->pNds.Add(new CNode(i, x, y, z));
				}
				for(i = 0; i < nFace; i++) {
					CLoop* lp = new CLoop();
					fin >> num;
					for(j = 0; j < num; j++) {
						fin >> id;
                        lp->iNds.Add(id);
					}
					surf->pLps.Add(lp);
				}
			fin.close();
		}
	}
	else return;

}

void CRealModelView::ConvertTrglSurfToOFF(CSurf* surf) 
{
	double x, y, z;
	int i, n0, n1, n2, n3, ID;
	CString fnHexMesh,fnHexNode;
	int N = surf->pNds.GetSize();    
	int T = surf->pTrs.GetSize();

	//node
	if(AfxMessageBox(_T("Saving OFF ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg3(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.off)|*.off"), NULL);
	if(Dlg3.DoModal() == IDOK) {
		fnHexNode = Dlg3.GetPathName(); 
		if(fnHexNode.Right(4)!=".txt")   fnHexNode+=".off";
		ofstream fin(fnHexNode);
		fin << "OFF" << endl;
		fin << N << "  " << T << "  " << 0 << endl;
		for(i=0;i< N;i++){
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin <<  x <<"   "<< y<<"   "<< z<<endl;
		}
		for(i=0;i< T;i++)	{
			n0 = surf->pTrs.GetAt(i)->nID[0];
			n1 = surf->pTrs.GetAt(i)->nID[1];
			n2 = surf->pTrs.GetAt(i)->nID[2];
			fin <<"3    "<<n0<<"   "<<n1<<"   "<<n2 << endl;
		}
		fin.close(); 
	}
}

void CRealModelView::ConvertQuadSurfToOFF(CSurf* surf) 
{
	double x, y, z;
	int i, n0, n1, n2, n3, ID;
	CString fnHexMesh,fnHexNode;
	int N = surf->pNds.GetSize();    
	int Q = surf->pQds.GetSize();

	//node
	if(AfxMessageBox(_T("Saving OFF ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg3(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.off)|*.off"), NULL);
	if(Dlg3.DoModal() == IDOK) {
		fnHexNode = Dlg3.GetPathName(); 
		if(fnHexNode.Right(4)!=".txt")   fnHexNode+=".off";
		ofstream fin(fnHexNode);
		fin << "OFF" << endl;
		fin << N << "  " << Q << "  " << 0 << endl;
		for(i=0;i< N;i++){
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin <<  x <<"   "<< y<<"   "<< z<<endl;
		}
		for(i=0;i< Q;i++)	{
			n0 = surf->pQds.GetAt(i)->nID[0];
			n1 = surf->pQds.GetAt(i)->nID[1];
			n2 = surf->pQds.GetAt(i)->nID[2];
            n3 = surf->pQds.GetAt(i)->nID[3];
			fin <<"4    "<<n0<<"   "<<n1<<"   "<<n2  << "  " << n3<< endl;
		}
		fin.close(); 
	}
}

void CRealModelView::ConvertPolySurfToOFF(CSurf * surf)
{
	double x, y, z;
	int i, j, n, ID;
	CString fnHexMesh,fnHexNode;
	int N = surf->pNds.GetSize();    
	int P = surf->pLps.GetSize();

	//node
	if(AfxMessageBox(_T("Saving OFF ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg3(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.off)|*.off"), NULL);
	if(Dlg3.DoModal() == IDOK) {
		fnHexNode = Dlg3.GetPathName(); 
		if(fnHexNode.Right(4)!=".txt")   fnHexNode+=".off";
		ofstream fin(fnHexNode);
		fin << "OFF" << endl;
		fin << N << "  " << P << "  " << 0 << endl;
		for(i=0;i< N;i++){
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin <<  x <<"   "<< y<<"   "<< z<<endl;
		}
		for(i = 0; i < P; i++) { // Mesh
			n = surf->pLps.GetAt(i)->iNds.GetSize();
			fin << n << "   ";
			for(j = 0; j < n; j++) {
				ID = surf->pLps.GetAt(i)->iNds.GetAt(j);
				fin << ID << "   ";
			}
			fin << endl;
		}

		fin.close(); 
	}
}

void CRealModelView::OnInterpolateKrigingOrdinary()
{
#ifndef EGC
	CArray<CNode*, CNode*> pInput;
	CArray<CNode*, CNode*> pOutput;

	int i,j,nv,np,nn,nm,ID;
	float x, y, z, ss;
	float * xx,* yy,* hh,* xxx,* yyy, *zzz;
	CString fnMesh, fnNode,fnResult;
	if(AfxMessageBox(_T("Known Points ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) 
	{
		i = 0;
		fnMesh = Dlg.GetPathName(); 
		ifstream fin(fnMesh);
		if(!fin){ AfxMessageBox(_T("Error : Known Points !")); return;}
		while(!fin.eof()){
			fin >>ID>> x >> y >> z;
			pInput.Add(new CNode(ID-1,x, y, z));
			i++;
		}
		fin.close();
	}
    
	//¶ÁÈ¡Î´ÖªµãµÄÊý¾Ý£¬´æ·ÅÔÚpoutputÖÐ
	if(AfxMessageBox(_T("Unknown Points ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg1(true,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg1.DoModal() == IDOK) 
	{
		i = 0;
		fnNode = Dlg1.GetPathName(); 
		ifstream fin1(fnNode);
		if(!fin1){AfxMessageBox(_T("´ò¿ªÒÑÖªµãÎÄ¼þ?")); return;}
		while(!fin1.eof()){
			fin1 >>ID>> x >> y>>z ;
			z=0;
			pOutput.Add(new CNode(ID-1,x, y, z)); 
			i++;
		}
		fin1.close(); 
	}

	nv=pInput.GetSize();
    //É¾³ýÖØ¸´µÄµã
	float r;
	for(i=0;i<pInput.GetSize()-1;i++)
		for(j=i+1;j<pInput.GetSize();j++)
		{
			r=sqrt(pow(pInput.GetAt(i)->x-pInput.GetAt(j)->x,2)+pow(pInput.GetAt(i)->y-pInput.GetAt(j)->y,2));
			if(r<0.001)    pInput.RemoveAt(j);

		}

	nv=pInput.GetSize();
	xx=new float[nv];    yy=new float[nv];   hh=new float[nv];
	for(j=0;j<nv;j++)
	{
		xx[j]=pInput.GetAt(j)->x;
		yy[j]=pInput.GetAt(j)->y;
		hh[j]=pInput.GetAt(j)->z;
	}
    
	np=pOutput.GetSize();
	xxx=new float[np];    yyy=new float[np];   zzz=new float[np];
	for(j=0;j<np;j++)
	{
		xxx[j]=pOutput.GetAt(j)->x;
		yyy[j]=pOutput.GetAt(j)->y;
		zzz[j]=pOutput.GetAt(j)->z;
	}

	nn=6;    //	nm=0;      ss=0;


	// Ordinary Kriging
	CInterpolate * pInpolate = new CInterpolate();
	pInpolate->OrdinaryKriging(xx, yy, hh, nv, xxx, yyy, zzz, np);


	//±£´æ²åÖµ½á¹û
	CFileDialog Dlg2(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg2.DoModal() == IDOK) 
	{
		fnResult = Dlg2.GetPathName(); 
		ofstream fin2(fnResult);
		for(i=0;i<np;i++)
		{
			ID=pOutput.GetAt(i)->ID;
			x=pOutput.GetAt(i)->x;
			y=pOutput.GetAt(i)->y;
            pOutput.GetAt(i)->z=zzz[i];
			z=zzz[i];
			fin2 <<ID<<"   "<<  x <<"   "<< y<<"   "<< z<<endl;
			
			
		}
		fin2.close(); 
	}

	delete[] xx;      delete[] yy;       delete[] hh;
	delete[] xxx;     delete[] yyy;      delete[] zzz;

	for(i=pInput.GetUpperBound(); i>=0;i--)//´Ó¶ÓÁÐÎ²²¿³¹µ×É¾³ý¶¯Ì¬Êý×é
	{
		delete pInput.GetAt(i);   pInput.RemoveAt(i);
	}
	
	for(i=pOutput.GetUpperBound(); i>=0;i--)//´Ó¶ÓÁÐÎ²²¿³¹µ×É¾³ý¶¯Ì¬Êý×é
	{
		delete pOutput.GetAt(i);   pOutput.RemoveAt(i);
	}

#else

#endif

}

void CRealModelView::OnInterpolateKrigingUniversal()
{
#ifndef EGC
	CArray<CNode*, CNode*> pInput;
	CArray<CNode*, CNode*> pOutput;

	int i,j,nv,np,nn,nm,ID;
	double x, y, z, ss;
	double* xx,* yy,* hh,* xxx,* yyy, *zzz;
	CString fnMesh, fnNode,fnResult;
	if(AfxMessageBox(_T("Known Points ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(true,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) 
	{
		i = 0;
		fnMesh = Dlg.GetPathName(); 
		ifstream fin(fnMesh);
		if(!fin) {AfxMessageBox(_T("´ò¿ªÒÑÖªµãÎÄ¼þ?")); return;}
		while(!fin.eof()){
			fin >>ID>> x >> y >> z;
				pInput.Add(new CNode(i , x, y, z));
			i++;
		}
		fin.close();
	}
    
	//¶ÁÈ¡Î´ÖªµãµÄÊý¾Ý£¬´æ·ÅÔÚpoutputÖÐ
	if(AfxMessageBox(_T("Unknown Points ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg1(true,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg1.DoModal() == IDOK) 
	{
		i = 0;
		fnNode = Dlg1.GetPathName(); 
		ifstream fin1(fnNode);
		if(!fin1) {AfxMessageBox(_T("´ò¿ªÒÑÖªµãÎÄ¼þ?")); return;}
		while(!fin1.eof()){
			fin1 >>ID>> x >> y>>z ;
			z=0;
			pOutput.Add(new CNode(i,x, y, z)); 
			i++;
		}
		fin1.close(); 
	}


nv=pInput.GetSize();
    //É¾³ýÖØ¸´µÄµã          //ÓÐÎÊÌâ£¬¶àÉ¾³ý»òÉÙÉ¾³ýÁË
	double r;
	for(i=0;i<pInput.GetSize()-1;i++)
		for(j=i+1;j<pInput.GetSize();j++)
		{
			r=sqrt(pow(pInput.GetAt(i)->x-pInput.GetAt(j)->x,2)+pow(pInput.GetAt(i)->y-pInput.GetAt(j)->y,2));
			if(r<0.01)    
			{ delete pInput.GetAt(j); pInput.RemoveAt(j);}

		}

	nv=pInput.GetSize();
 
	xx=new double[nv];    yy=new double[nv];   hh=new double[nv];
	for(j=0;j<nv;j++)
	{
		xx[j]=pInput.GetAt(j)->x;
		yy[j]=pInput.GetAt(j)->y;
		hh[j]=pInput.GetAt(j)->z;
	}
    
	np=pOutput.GetSize();
	xxx=new double[np];    yyy=new double[np];   zzz=new double[np];
	for(j=0;j<np;j++)
	{
		xxx[j]=pOutput.GetAt(j)->x;
		yyy[j]=pOutput.GetAt(j)->y;
		zzz[j]=pOutput.GetAt(j)->z;
	}

	nn=6;    	nm=0;      ss=0;

	// Universal Kriging
//	CInterpolate * pInpolate = new CInterpolate();
//  pInpolate->UniversalKriging(xx, yy, hh, nv,xxx, yyy, zzz, np);

	// Universal Kriging STL
	vector<Point> pts(nv);
    vector<Point> toInpt(np);

	for(i = 0; i < nv; i++) {
		pts[i].x = xx[i]; 
		pts[i].y = yy[i];
		pts[i].z = hh[i];
	}

	for(i = 0; i < np; i++) {
		toInpt[i].x = xxx[i];
		toInpt[i].y = yyy[i];
		toInpt[i].z = zzz[i];
    }

	CInterpolate * pInpolate = new CInterpolate();
    pInpolate->UniversalKriging_STL(pts, toInpt);

	for(i = 0; i < np; i++) {
	    zzz[i] = toInpt[i].z ;
    }

	//±£´æ²åÖµ½á¹û
	if(AfxMessageBox(_T("È·¶¨±£´æ²åÖµ½á¹û?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg2(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg2.DoModal() == IDOK) 
	{
		fnResult = Dlg2.GetPathName(); 
		if(fnResult.Right(4)!=".txt")   fnResult+=".txt";
		ofstream fin2(fnResult);
		for(i=0;i<np-1;i++)
		{
			ID=pOutput.GetAt(i)->ID;
			x=pOutput.GetAt(i)->x;
			y=pOutput.GetAt(i)->y;
            pOutput.GetAt(i)->z=zzz[i];
			z=zzz[i];
			fin2 <<ID<<"   "<<  x <<"   "<< y<<"   "<< z<<endl;
		}
		    ID=pOutput.GetAt(i)->ID;
			x=pOutput.GetAt(i)->x;
			y=pOutput.GetAt(i)->y;
            pOutput.GetAt(i)->z=zzz[i];
			z=zzz[i];
			fin2 <<ID<<"   "<<  x <<"   "<< y<<"   "<< z;
		fin2.close(); 
	}

	delete[] xx;      delete[] yy;       delete[] hh;
	delete[] xxx;     delete[] yyy;      delete[] zzz;

	for(i=pInput.GetUpperBound(); i>=0;i--)//´Ó¶ÓÁÐÎ²²¿³¹µ×É¾³ý¶¯Ì¬Êý×é
	{
		delete pInput.GetAt(i);   pInput.RemoveAt(i);
	}
	
	for(i=pOutput.GetUpperBound(); i>=0;i--)//´Ó¶ÓÁÐÎ²²¿³¹µ×É¾³ý¶¯Ì¬Êý×é
	{
		delete pOutput.GetAt(i);   pOutput.RemoveAt(i);
	}
#else

#endif
	
}


void CRealModelView::OnInterpolateDSI()
{
	// TODO: Add your command handler code here
}


void CRealModelView::OnInterpolateIDW()
{
	vector<CNode*> pts, toInpt;

	COpen fOpen;  CSave fSave;  
	fOpen.OpenOnlyPoint(pts);
	fOpen.OpenOnlyPoint(toInpt);
	
	CInterpolate * pInpolate = new CInterpolate();
    pInpolate->IDW_STL(pts, toInpt, 4.0);

    fSave.SaveOnlyPoint(toInpt);

}

#define PI 3.14159265
// Optimize triangle from 3D to 2D, then back to 3D
void CRealModelView::OperationsSimple(CSurf * surf, int iAxis)
{
	double TX, TY, TZ;
	double para = 180;
	
#ifndef EGC
	double rsin = sin (para*PI/180);
	double rcos = cos (para*PI/180);
#else
	double tmp = para * PI / 180.0 ;
	double rsin = sin(tmp.doubleValue());
	double rcos = cos(tmp.doubleValue());
#endif
	
	// Min, Max 
	CNode * pMin = new CNode(0, 0.0, 0.0, 0.0);
	CNode * pMax = new CNode(1, 0.0, 0.0, 0.0);
	
	int i;
	int N = surf->pNds.GetSize();
	
	for(i = 0; i < N; i++) {
		CNode * pNode = surf->pNds.GetAt(i);
		if(pMin->x > pNode->x)  pMin->x = pNode->x;
		if(pMin->y > pNode->y)  pMin->y = pNode->y;
		if(pMin->z > pNode->z)  pMin->z = pNode->z;
		
		if(pMax->x < pNode->x)  pMax->x = pNode->x;
		if(pMax->y < pNode->y)  pMax->y = pNode->y;
		if(pMax->z < pNode->z)  pMax->z = pNode->z;
	}
	
	TX = (pMax->x + pMin->x) / 2.0;
	TY = (pMax->y + pMin->y) / 2.0;
	TZ = (pMax->z + pMin->z) / 2.0;

//	TX = 0.0;  TY = 0.0; TZ = 0.0;
	
	// change pIntrTmp and pTrgl to XY plane
	double * x, * y, * z;
	
	x = new double[N];  y = new double[N];  z = new double[N];
	for(i = 0; i < N; i++) { // transform
		CNode * pNode = surf->pNds.GetAt(i);
		x[i] = pNode->x - TX;
		y[i] = pNode->y - TY;
		z[i] = pNode->z - TZ;
	}
	
	int iType = 0;

	if(iType == 0) { 
		if(iAxis == 1) { // along X axis
			for(i = 0; i < N; i++) {
				CNode * pNode = surf->pNds.GetAt(i);
				pNode->x = x[i];
				pNode->y = y[i] * rcos - z[i] * rsin;
				pNode->z = y[i] * rsin + z[i] * rcos;
			}
		}
		if(iAxis == 2) { // along X axis
			for(i = 0; i < N; i++) {
				CNode * pNode = surf->pNds.GetAt(i);
				pNode->x = z[i] * rsin + x[i] * rcos;
				pNode->y = y[i];
				pNode->z = z[i] * rcos - x[i] * rsin;
			}
		}
		if(iAxis == 3) { // along X axis
			for(i = 0; i < N; i++) {
				CNode * pNode = surf->pNds.GetAt(i);
				pNode->x = x[i] * rcos - y[i] * rsin;
				pNode->y = x[i] * rsin + y[i] * rcos;
				pNode->z = z[i];
			}
		}
	}
	
	for(i = 0; i < N; i++) {
		CNode * pNode = surf->pNds.GetAt(i);
		pNode->x += TX;
		pNode->y += TY;
		pNode->z += TZ;
	}
	
	delete [] x;  delete [] y;  delete [] z;

}

void CRealModelView::OnToolsByRotateX()
{
	CSurf * surf = new CSurf();

	int i, ndata;
	double x, y, z;
	ndata = AfxMessageBox(_T("Read Node ?"), MB_YESNOCANCEL);
	if(ndata == 6){
		CFileDialog Dlg1(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"), NULL);
		if(Dlg1.DoModal() == IDOK){
			ifstream fin1(Dlg1.GetPathName());
			while(!fin1.eof())
			{
				fin1 >> i >> x >> y >> z;
				surf->pNds.Add(new CNode(i, x, y, z));  // Numbering begins at 0
			}
			fin1.close();
		}		 
	}
	else return;

	OperationsSimple(surf, 1);

	if(AfxMessageBox(_T("Saving Node ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg3(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg3.DoModal() == IDOK) {
		CString fnHexNode = Dlg3.GetPathName(); 
		if(fnHexNode.Right(4)!=".txt")   fnHexNode+=".txt";
		ofstream fin(fnHexNode);
		int N = surf->pNds.GetSize();
		for(i = 0; i < N - 1;i++){
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin <<  i <<"  "<<x <<"   "<< y<<"   "<< z<<endl;
		}
		fin <<  i <<"  "<<x <<"   "<< y<<"   "<< z ;

		fin.close(); 
	}

	AfxMessageBox(_T("Everything is Done !"));
	
}


void CRealModelView::OnToolsByRotateY()
{
	CSurf * surf = new CSurf();

	int i, ndata;
	double x, y, z;
	ndata = AfxMessageBox(_T("Read Node ?"), MB_YESNOCANCEL);
	if(ndata == 6){
		CFileDialog Dlg1(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"), NULL);
		if(Dlg1.DoModal() == IDOK){
			ifstream fin1(Dlg1.GetPathName());
			while(!fin1.eof())
			{
				fin1 >> i >> x >> y >> z;
				surf->pNds.Add(new CNode(i, x, y, z));  // Numbering begins at 0
			}
			fin1.close();
		}		 
	}
	else return;

	OperationsSimple(surf, 2);

	if(AfxMessageBox(_T("Saving Node ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg3(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg3.DoModal() == IDOK) {
		CString fnHexNode = Dlg3.GetPathName(); 
		if(fnHexNode.Right(4)!=".txt")   fnHexNode+=".txt";
		ofstream fin(fnHexNode);
		int N = surf->pNds.GetSize();
		for(i = 0; i < N - 1;i++){
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin <<  i <<"  "<<x <<"   "<< y<<"   "<< z<<endl;
		}
		fin <<  i <<"  "<<x <<"   "<< y<<"   "<< z ;

		fin.close(); 
	}

	AfxMessageBox(_T("Everything is Done !"));
	
}


void CRealModelView::OnToolsByRotateZ()
{
	CSurf * surf = new CSurf();

	int i, ndata;
	double x, y, z;
	ndata = AfxMessageBox(_T("Read Node ?"), MB_YESNOCANCEL);
	if(ndata == 6){
		CFileDialog Dlg1(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"), NULL);
		if(Dlg1.DoModal() == IDOK){
			ifstream fin1(Dlg1.GetPathName());
			while(!fin1.eof())
			{
				fin1 >> i >> x >> y >> z;
				surf->pNds.Add(new CNode(i, x, y, z));  // Numbering begins at 0
			}
			fin1.close();
		}		 
	}
	else return;

	OperationsSimple(surf, 3);

	if(AfxMessageBox(_T("Saving Node ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg3(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg3.DoModal() == IDOK) {
		CString fnHexNode = Dlg3.GetPathName(); 
		if(fnHexNode.Right(4)!=".txt")   fnHexNode+=".txt";
		ofstream fin(fnHexNode);
		int N = surf->pNds.GetSize();
		for(i = 0; i < N - 1;i++){
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin <<  i <<"  "<<x <<"   "<< y<<"   "<< z<<endl;
		}
		fin <<  i <<"  "<<x <<"   "<< y<<"   "<< z ;

		fin.close(); 
	}

	AfxMessageBox(_T("Everything is Done !"));
	
}


void CRealModelView::OnToolsByMove()
{
	CSurf * surf = new CSurf();

	int i, ndata;
	double x, y, z;
	ndata = AfxMessageBox(_T("Read Node ?"), MB_YESNOCANCEL);
	if(ndata == 6){
		CFileDialog Dlg1(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"), NULL);
		if(Dlg1.DoModal() == IDOK){
			ifstream fin1(Dlg1.GetPathName());
			while(!fin1.eof())
			{
				fin1 >> i >> x >> y >> z;
				surf->pNds.Add(new CNode(i, x, y, z));  // Numbering begins at 0
			}
			fin1.close();
		}		 
	}
	else return;

	double distX, distY, distZ;

	double xFactor = 0.0;
	double yFactor = 1.0;
	double zFactor = -1.0;
	
	// Min, Max 
	CNode * pMin = new CNode(0, 0.0, 0.0, 0.0);
	CNode * pMax = new CNode(1, 0.0, 0.0, 0.0);
	
	int N = surf->pNds.GetSize();
	
	for(i = 0; i < N; i++) {
		CNode * pNode = surf->pNds.GetAt(i);
		if(pMin->x > pNode->x)  pMin->x = pNode->x;
		if(pMin->y > pNode->y)  pMin->y = pNode->y;
		if(pMin->z > pNode->z)  pMin->z = pNode->z;
		
		if(pMax->x < pNode->x)  pMax->x = pNode->x;
		if(pMax->y < pNode->y)  pMax->y = pNode->y;
		if(pMax->z < pNode->z)  pMax->z = pNode->z;
	}
	
	distX = (pMax->x - pMin->x) / 2.0;
	distY = (pMax->y - pMin->y) / 2.0;
	distZ = (pMax->z - pMin->z) / 2.0;
	
	for(i = 0; i < N; i++) {
		CNode * pNode = surf->pNds.GetAt(i);
		pNode->x += distX * xFactor;
		pNode->y += distY * yFactor;
		pNode->z += distZ * zFactor;
	}

	if(AfxMessageBox(_T("Saving Node ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg3(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg3.DoModal() == IDOK) {
		CString fnHexNode = Dlg3.GetPathName(); 
		if(fnHexNode.Right(4)!=".txt")   fnHexNode+=".txt";
		ofstream fin(fnHexNode);
		int N = surf->pNds.GetSize();
		for(i = 0; i < N - 1;i++){
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin <<  i <<"  "<<x <<"   "<< y<<"   "<< z<<endl;
		}
		fin <<  i <<"  "<<x <<"   "<< y<<"   "<< z ;

		fin.close(); 
	}

	AfxMessageBox(_T("Everything is Done !"));

}


void CRealModelView::OnToolsByScale()
{
	CSurf * surf = new CSurf();

	int i, ndata;
	double x, y, z;
	ndata = AfxMessageBox(_T("Read Node ?"), MB_YESNOCANCEL);
	if(ndata == 6){
		CFileDialog Dlg1(TRUE,NULL,NULL,OFN_HIDEREADONLY, _T("Text File(*.txt)|*.txt|Dat File (*.dat)|*.dat|All Files (*.*)|*.*"), NULL);
		if(Dlg1.DoModal() == IDOK){
			ifstream fin1(Dlg1.GetPathName());
			while(!fin1.eof())
			{
				fin1 >> i >> x >> y >> z;
				surf->pNds.Add(new CNode(i, x, y, z));  // Numbering begins at 0
			}
			fin1.close();
		}		 
	}
	else return;

	double Scale = 1000.0;

	int N = surf->pNds.GetSize();

	for(i = 0; i < N; i++) {
		CNode * pNode = surf->pNds.GetAt(i);
		pNode->x *= Scale;
		pNode->y *= Scale;
		pNode->z *= Scale;
	}

	if(AfxMessageBox(_T("Saving Node ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg3(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg3.DoModal() == IDOK) {
		CString fnHexNode = Dlg3.GetPathName(); 
		if(fnHexNode.Right(4)!=".txt")   fnHexNode+=".txt";
		ofstream fin(fnHexNode);
		int N = surf->pNds.GetSize();
		for(i = 0; i < N - 1;i++){
			x = surf->pNds.GetAt(i)->x;  
			y = surf->pNds.GetAt(i)->y;  
			z = surf->pNds.GetAt(i)->z;
			fin <<  i <<"  "<<x <<"   "<< y<<"   "<< z<<endl;
		}
		fin <<  i <<"  "<<x <<"   "<< y<<"   "<< z ;

		fin.close(); 
	}

	AfxMessageBox(_T("Everything is Done !"));

}


void CRealModelView::OnToolsOffsetZ()
{
	CSurf * surf = new CSurf();
	ReadDataSurface(surf);
	
	/*
	double offsetZ = -18.4;
	double scale = 0.8;
	*/

	/*
	// For new surface 1
	double offsetZ = -20.4;
	double scale = 0.8;
	*/
	
	/*
	double offsetZ = 1.5;
	double scale = 1.0;
	*/

	/*
	// For new surface 6
	double offsetZ = -3.0;
	double scale = 0.8;
	*/

	double offsetZ = 0.3;
	double scale = 1.0 ;

	for(int i = 0; i < surf->pNds.GetSize(); i++) {
		surf->pNds.GetAt(i)->z = surf->pNds.GetAt(i)->z + offsetZ;
		surf->pNds.GetAt(i)->z = surf->pNds.GetAt(i)->z * scale;
	}

	SaveTrglMesh(surf);

	surf->Clear();
}


// AntiAlising
void CRealModelView::OnDebugAntiAlising()
{
   ballList = glGenLists(1);
   glNewList (ballList, GL_COMPILE);
   glutSolidTeapot(1.0);
   glEndList ();

   m_bOpenGL = 1;
   Invalidate(TRUE);
}

void CRealModelView::OnDebugGetTopSurface()
{
	// open polyhedrons
	ReadData();
	pCrt = new CCordnt();
    pCrt = GetNewCordntSystemBlockGroup();
	GLuint allBlockGroups;
	allBlockGroups = glGenLists(1);
	CompileBlockGroupList(pCrt, allBlockGroups, 0);
	m_bOpenGL = 1;
	Invalidate(TRUE);

	// get top surface
	CSurf* topSurf = new CSurf();
	CreatTopSurface(topSurf);

	// save
	SavePolygonSurface(topSurf);

	// exit
	exit(0);
}

void CRealModelView::OnDebugTriangulatePolygonSurface()
{
	int i, j;
	CSurf * topSurf = new CSurf();
	ReadPolygonSurface(topSurf);
	int num = topSurf->pLps.GetSize();
	for(i = 0; i < num; i++) {
		// For Output
		if(pFrame == NULL) {
	      pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	    }
		CString str;
	    str.Format(_T(" i = %d [/ %d]"), i, num);
	    pFrame->AddStrToOutputWnd(1, str);

		CLoop * lp = topSurf->pLps.GetAt(i);
		PolygonPartion(lp);
		for(j = 0; j < lp->pTrs.GetSize(); j++) {
			topSurf->pTrs.Add(lp->pTrs.GetAt(j));
		}
	}
//	EmergeTriangleSurface(topSurf);
	EmergeTriangleSurfaceBest(topSurf);
	SaveTrglMesh(topSurf);
}

void CRealModelView::On3DModelGetTopSurface()
{
	// Open polyhedrons
	ReadData();
	pCrt = new CCordnt();
    pCrt = GetNewCordntSystemBlockGroup();
	GLuint allBlockGroups;
	allBlockGroups = glGenLists(1);
	glCompileBlockGroupListBest(pCrt, allBlockGroups);
	m_bOpenGL = 1;
	Invalidate(TRUE);

	// get top surface
	CSurf* topSurf = new CSurf();
//	CreatTopSurface(topSurf);
	CreatTopSurfaceBest(topSurf);

	// save
	SavePolygonSurface(topSurf);
}

void CRealModelView::OnToolsGetBottomSurface()
{
	// Open polyhedrons
	ReadData();
	pCrt = new CCordnt();
    pCrt = GetNewCordntSystemBlockGroup();
	GLuint allBlockGroups;
	allBlockGroups = glGenLists(1);
	glCompileBlockGroupListBest(pCrt, allBlockGroups);
	m_bOpenGL = 1;
	Invalidate(TRUE);

	// Get bottom surface
	CSurf* bottomSurf = new CSurf();
	CreatBottomSurfaceBest(bottomSurf);

	// save
	SavePolygonSurface(bottomSurf);
}

void CRealModelView::On3DModelCombineBlockGroup()
{
	CModel * ma = new CModel();  ReadModel(ma);
	CModel * mb = new CModel();  ReadModel(mb);
	CModel * ab = EmergeModel(ma, mb);
	SaveModel(ab);
	AfxMessageBox(_T("Everything is Done !"));
}


void CRealModelView::On3DModelBlockGroupToAnsys()
{
	// Open
	ReadData();

	// To Ansys: Node, Area, Block
	CString fn, fname;
	int i, j, t0, t1, t2, t3, t4;
	t0 = 1;  t1 = 1; t2 = 1; t3 = 1; t4 = 0;
	if(AfxMessageBox(_T("Save to Ansys ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) 
	{
		fn = Dlg.GetPathName(); 
		CString str = _T("_ForAnsys");
		fname = fn + str;
		if(fname.Right(4)!="txt")   fname += ".txt";
		ofstream fout(fname);
		fout << "/prep7" << endl;
		for(i = 0; i < m_aNds.GetSize(); i++){ //Nodes
			fout << "K," << i+1 <<", " 
				  << m_aNds.GetAt(i)->x << ", " 
				  << m_aNds.GetAt(i)->y << ", " 
				  << m_aNds.GetAt(i)->z << endl;
		}

		for(i = 0; i < m_aLps.GetSize(); i++){//Areas
			fout << "A, ";
			for(j = 0; j < m_aLps.GetAt(i)->iNds.GetSize() - 1; j++)
				fout << m_aLps.GetAt(i)->iNds.GetAt(j) + 1<< ", ";
			fout << m_aLps.GetAt(i)->iNds.GetAt(j) + 1;
			fout << endl;
		}
		
		/*
		for(i = 0; i < m_aPhds.GetSize(); i++){//volumes
			fout << "ASEL," << "S," << " ," << " ," << m_aPhds.GetAt(i)->iLps.GetAt(0) + 1 << endl;
			for(j = 1; j < m_aPhds.GetAt(i)->iLps.GetSize(); j++)
				fout << "ASEl," << "A," << " ," << " ," << m_aPhds.GetAt(i)->iLps.GetAt(j) + 1 << endl;
			fout << "VA," << "ALL"<<endl;
		}
		*/
		fout.close();
	}

}


// Cut block with surface. 2012.2.29
void CRealModelView::OnDebugSurfCutBlock()
{
	CInsect myInsect;
	myInsect.ReadSurface();
	myInsect.ReadBlock();
	myInsect.InsectOfSurfAndBlock();
}

void CRealModelView::SaveCorrLoops(CSurf* surf)
{
	if(AfxMessageBox(_T("Saving Corr Loops ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg3(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg3.DoModal() == IDOK) {
		CString fnHexNode = Dlg3.GetPathName(); 
		if(fnHexNode.Right(4)!=".txt")   fnHexNode+=".txt";
		ofstream fin(fnHexNode);
		CLoop * lp;
		for(int i = 0; i < surf->pLps.GetSize();i++){
			lp = surf->pLps.GetAt(i);
			fin << i << ":  ";
			for(int j = 0; j < lp->iCorrLps.GetSize(); j++) {
				fin << "   " << lp->iCorrLps.GetAt(j) ;
			}
			fin << endl;
		}
		fin.close(); 
	}

	AfxMessageBox(_T("Everything is Done !"));

}

// Create a block by mapping 2013.5.5
// A bottom polygon = several top polygons
CBlock * CRealModelView::CreatBlockByMapping(CSurf* pBottom, CSurf* pTop)
{ 
	CBlock * blk = new CBlock();
	
	int i, j, k, t, n, m, id;
	double cx, cy, cz, tx, ty, tz;
	double x[3], y[3], z[3], para[4];
	bool bFind, bExist;

	// Pre-Check
	CLoop * lp;
	for(i = 0; i < pBottom->pLps.GetSize(); i++) {
		lp = pBottom->pLps.GetAt(i);
		n = lp->iNds.GetSize();
		if(lp->iNds.GetAt(0) != lp->iNds.GetAt(n-1)) {
			id = lp->iNds.GetAt(0);
			lp->iNds.Add(id);
			lp->pNds.Add(pBottom->pNds.GetAt(id));
		}
	}
	for(i = 0; i < pTop->pLps.GetSize(); i++) {
		lp = pTop->pLps.GetAt(i);
		n = lp->iNds.GetSize();
		if(lp->iNds.GetAt(0) != lp->iNds.GetAt(n-1)) {
			id = lp->iNds.GetAt(0);
			lp->iNds.Add(id);
			lp->pNds.Add(pTop->pNds.GetAt(id));
		}
	}

	// Step 1 : Search corresponding polygons
	CLoop * lpa, * lpb;
	for(i = 0; i < pTop->pLps.GetSize(); i++) {
		bFind = false;
		lpa = pTop->pLps.GetAt(i);
	
		// 重心 center
		cx = 0.0;  cy = 0.0;  cz = 0.0;
		n = lpa->iNds.GetSize();
		if(lpa->iNds.GetAt(0) == lpa->iNds.GetAt(n-1)) {
			--n; // the first and the last nodes are the same
		}
		for(t = 0; t < n; t++) {
			id = lpa->iNds.GetAt(t);
			cx += pTop->pNds.GetAt(id)->x;
			cy += pTop->pNds.GetAt(id)->y;
			cz += pTop->pNds.GetAt(id)->z;
		}
		cx /= n;  cy /= n;  cz /= n;
		CNode * nd = new CNode(i, cx, cy, cz);

		for(j = 0; j < pBottom->pLps.GetSize(); j++) {
			lpb = pBottom->pLps.GetAt(j);
			m = lpb->iNds.GetSize();
			if(lpb->iNds.GetAt(0) == lpb->iNds.GetAt(m-1)) {
				--m; // Check for next step
			}

			// Create a polygon 
			CLoop * face = new CLoop();
			for(t = 0; t < m; t++) {
				id = lpb->iNds.GetAt(t);
				tx = pBottom->pNds.GetAt(id)->x;
				ty = pBottom->pNds.GetAt(id)->y;
				tz = pBottom->pNds.GetAt(id)->z;
				face->pNds.Add(new CNode(id, tx, ty, tz));
			}
			t = 0; // Add the first again for "IsNdInPolygon"
			id = lpb->iNds.GetAt(t);
			tx = pBottom->pNds.GetAt(id)->x;
			ty = pBottom->pNds.GetAt(id)->y;
			tz = pBottom->pNds.GetAt(id)->z;
			face->pNds.Add(new CNode(id, tx, ty, tz));
			
			if(IsNdInPolygon(nd, face, myEps) == false) {
				face->Clear();  delete face; // Outside
			}
			else {
				lpb->iCorrLps.Add(i);  
				lpa->iCorrLps.Add(j);
				bFind = true;
				face->Clear();  delete face;
			}
			
			if(bFind == true) {
				break;
			}
		}
		delete nd;
	}

//	SaveCorrLoops(pTop);
//	SaveCorrLoops(pBottom);

	// For progress window
	CProgressWnd wndProgress(this, _T("Progress of Forming Polyhedrons"), TRUE);
	int myRance = pBottom->pLps.GetSize();
	wndProgress.SetRange(0, myRance);
	wndProgress.SetText(_T("Forming Polyhedrons...\n\n")
						_T("Please Wait..."));

	// Step 2 : Create polyhedrons
	for(i = 0; i < pBottom->pLps.GetSize(); i++) {
		

		// For progress windown
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) {
			MessageBox(_T("Progress Cancelled"));
			::PostQuitMessage(0);
		}
		// For output window
		CString str;
	    if(pFrame == NULL) {
	       pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	    }
		str.Format(_T("Forming Polyhedrons :  i = %d  [ / %d]"), i, myRance);
		pFrame->AddStrToOutputWnd(1, str);

		CPohd * phd = new CPohd();
		blk->pPhs.Add(phd);

		// Bottom polygon
		lpb = pBottom->pLps.GetAt(i);
		m = lpb->iNds.GetSize();
		if(lpb->iNds.GetAt(0) == lpb->iNds.GetAt(m-1)) {
			--m; // Check for next step
		}
		CLoop * face = new CLoop(); // Create a polygon
		for(t = 0; t < m; t++) {
			id = lpb->iNds.GetAt(t);
			tx = pBottom->pNds.GetAt(id)->x;
			ty = pBottom->pNds.GetAt(id)->y;
			tz = pBottom->pNds.GetAt(id)->z;
			face->pNds.Add(new CNode(id, tx, ty, tz));
		}
		phd->pLps.Add(face);

		if(pBottom->pLps.GetAt(i)->iCorrLps.IsEmpty())  continue;

		// Corresponding Top polygons
		lpb = pBottom->pLps.GetAt(i);
		for(j = 0; j < lpb->iCorrLps.GetSize(); j++) {
			id = lpb->iCorrLps.GetAt(j);
			lpa = pTop->pLps.GetAt(id);
					
		    m = lpa->iNds.GetSize();
		    if(lpa->iNds.GetAt(0) == lpa->iNds.GetAt(m-1)) {
			   --m; // Check for next step
		    }
		    CLoop * face = new CLoop(); // Create a polygon
		    for(t = 0; t < m; t++) {
			    id = lpa->iNds.GetAt(t);
			    tx = pTop->pNds.GetAt(id)->x;
			    ty = pTop->pNds.GetAt(id)->y;
			    tz = pTop->pNds.GetAt(id)->z;
			    face->pNds.Add(new CNode(id, tx, ty, tz));
		    }
		    phd->pLps.Add(face);
		}

		// Side polygons
		//// Get top boundary
		CSurf * tPoly = new CSurf();
		lpb = pBottom->pLps.GetAt(i);
		for(j = 0; j < lpb->iCorrLps.GetSize(); j++) {
			id = lpb->iCorrLps.GetAt(j);
			lpa = pTop->pLps.GetAt(id);
			tPoly->pLps.Add(lpa);
		}
		CLoop * boundary = GetBoundaryLoop( tPoly );

		// Check 
		if(boundary->iNds.IsEmpty())  continue;

		//// Find corresponding vertex
		CArray<int, int> iCorrPts;
		CNode * pta, * ptb;
		for(j = 0; j < pBottom->pLps.GetAt(i)->iNds.GetSize(); j++) {
			id = pBottom->pLps.GetAt(i)->iNds.GetAt(j);
			pta = pBottom->pNds.GetAt(id); // Bottom
			for(k = 0; k < boundary->iNds.GetSize(); k++) {
				id = boundary->iNds.GetAt(k);
				ptb = pTop->pNds.GetAt(id); // Top 

				if (IsSameNode(pta->x, pta->y, 0.0, ptb->x, ptb->y, 0.0, myEps) == true) {
					iCorrPts.Add(k);  break;
				}
			}
		}

		// Check 
		if(iCorrPts.GetSize() != pBottom->pLps.GetAt(i)->iNds.GetSize())  continue;

		//// Form side faces
		int id_head, id_tail, id_last, size;
		for(j = 0; j < pBottom->pLps.GetAt(i)->iNds.GetSize() - 1; j++) {
			CLoop * face = new CLoop();

			// 1st point
			id = pBottom->pLps.GetAt(i)->iNds.GetAt(j);
			tx = pBottom->pNds.GetAt(id)->x;
			ty = pBottom->pNds.GetAt(id)->y;
			tz = pBottom->pNds.GetAt(id)->z;
			face->pNds.Add(new CNode(id, tx, ty, tz));

			// 2nd point
			id = pBottom->pLps.GetAt(i)->iNds.GetAt(j+1);
			tx = pBottom->pNds.GetAt(id)->x;
			ty = pBottom->pNds.GetAt(id)->y;
			tz = pBottom->pNds.GetAt(id)->z;
			face->pNds.Add(new CNode(id, tx, ty, tz));

			// Top points
			id_tail = iCorrPts.GetAt(j);
			id_head = iCorrPts.GetAt(j+1);

			if(id_head > id_tail) {
			    for(k = id_head; k >= id_tail; k--) {
				    id = boundary->iNds.GetAt(k);
				    tx = pTop->pNds.GetAt(id)->x;
			        ty = pTop->pNds.GetAt(id)->y;
			        tz = pTop->pNds.GetAt(id)->z;
					
					// Check
					CNode * nd = new CNode(id, tx, ty, tz);
					size = face->pNds.GetSize() - 1;
					if(IsSameNode(face->pNds.GetAt(size), nd, myEps) == false) {
			            face->pNds.Add(nd);
					}
					else { delete nd; }
			    }
			}
			//*
			else {
				for(k = id_head; k >= 0; k--) {
				    id = boundary->iNds.GetAt(k);
				    tx = pTop->pNds.GetAt(id)->x;
			        ty = pTop->pNds.GetAt(id)->y;
			        tz = pTop->pNds.GetAt(id)->z;
			        //face->pNds.Add(new CNode(id, tx, ty, tz));

					// Check
					CNode * nd = new CNode(id, tx, ty, tz);
					size = face->pNds.GetSize() - 1;
					if(IsSameNode(face->pNds.GetAt(size), nd, myEps) == false) {
			            face->pNds.Add(nd);
					}
					else { delete nd; }
			    }
				for(k = boundary->iNds.GetSize()-1; k >= id_tail; k--) {
				    id = boundary->iNds.GetAt(k);
				    tx = pTop->pNds.GetAt(id)->x;
			        ty = pTop->pNds.GetAt(id)->y;
			        tz = pTop->pNds.GetAt(id)->z;
			    //  face->pNds.Add(new CNode(id, tx, ty, tz));

					// Check
					CNode * nd = new CNode(id, tx, ty, tz);
					size = face->pNds.GetSize() - 1;
					if(IsSameNode(face->pNds.GetAt(size), nd, myEps) == false) {
			            face->pNds.Add(nd);
					}
					else { delete nd; }
			    }
			}
			//*/

			phd->pLps.Add(face);
		}

	//	blk->pPhs.Add(phd);
	}

	return blk;

}


// Get the boundary loop of a polygonal surface 2013.5.5
CLoop * CRealModelView::GetBoundaryLoop(CSurf* tPolySurf)
{ 
	CLoop * bound = new CLoop();

	int i, j, m, head, tail;

	// Save all Edges
	CArray<CEdge*, CEdge*> allegs;
	CLoop * lp;
	for(i = 0; i < tPolySurf->pLps.GetSize(); i++) {
		lp = tPolySurf->pLps.GetAt(i);
		m = lp->iNds.GetSize();
		if(lp->iNds.GetAt(0) != lp->iNds.GetAt(m-1)) { // Add the first one
			lp->iNds.Add(lp->iNds.GetAt(0));
		}
		for(j = 0; j < lp->iNds.GetSize() - 1; j++) {
			head = lp->iNds.GetAt(j);
			tail = lp->iNds.GetAt(j+1);
			allegs.Add(new CEdge(head, tail));
		}
	}

	// Check UseTime
	CEdge * ega, * egb;
	for(i = 0; i < allegs.GetSize(); i++) { 
		allegs.GetAt(i)->UseTime = 0;
	}
	for(i = 0; i < allegs.GetSize() - 1; i++) {
		ega = allegs.GetAt(i);
		for(j = i + 1; j < allegs.GetSize(); j++) {
			egb = allegs.GetAt(j);
			if ((ega->Start == egb->Start && ega->End == egb->End) || 
				(ega->Start == egb->End && ega->End == egb->Start) ){
					ega->UseTime = 2;  egb->UseTime = 2;  break;
			}
		}
	}

	// Form loop
	CArray<CEdge*, CEdge*> tmpegs;
	for(i = 0; i < allegs.GetSize(); i++) {
		if(allegs.GetAt(i)->UseTime != 2) {
			head = allegs.GetAt(i)->Start;
			tail = allegs.GetAt(i)->End;
			tmpegs.Add(new CEdge(head, tail));
		}
	}
	for(i = 0; i < tmpegs.GetSize(); i++) {
		tmpegs.GetAt(i)->bUsed = false;
	}
	int num_use = 1, last;
	head = tmpegs.GetAt(0)->Start;
	tail = tmpegs.GetAt(0)->End;
	tmpegs.GetAt(0)->bUsed = true;
	bound->iNds.Add(head);
	bound->iNds.Add(tail);

	int times = 0;
	while (num_use != tmpegs.GetSize() ) {
		// Check whether infinite loop
		times++;
		if(times > tmpegs.GetSize()) {
			// For output window
		    CString str;
	        if(pFrame == NULL) {
	            pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	        }
		    str.Format(_T("Cannot Form Boundary Loop !"));
		    pFrame->AddStrToOutputWnd(1, str);

			bound->iNds.RemoveAll(); 
			return bound;
			break;
		}

		m = bound->iNds.GetSize();
		last = bound->iNds.GetAt(m-1);

		for(i = 1; i < tmpegs.GetSize(); i++) {
			if(tmpegs.GetAt(i)->bUsed == true)  continue;
			if(tmpegs.GetAt(i)->Start == last) {
				bound->iNds.Add(tmpegs.GetAt(i)->End);  
				tmpegs.GetAt(i)->bUsed = true;
				num_use++;  continue;
			}
			if(tmpegs.GetAt(i)->End == last) {
				bound->iNds.Add(tmpegs.GetAt(i)->Start);
				tmpegs.GetAt(i)->bUsed = true;
				num_use++;  continue;
			}
		}
	}

	// Memory
	for(i = allegs.GetUpperBound(); i > -1; i--) { // clear up
		delete allegs.GetAt(i);   allegs.RemoveAt(i); 
	}
	for(i = tmpegs.GetUpperBound(); i > -1; i--) { // clear up
		delete tmpegs.GetAt(i);   tmpegs.RemoveAt(i); 
	}

	return bound;
}


void CRealModelView::On2DModelMappingComplex()
{
	// TODO: Add your command handler code here
	CBlock * pBlk = new CBlock();

	// Insect by mapping, has been merged ! Not need EmergeBlockGroupsBest();
	CSurf* pBot = m_aSurfs.GetAt(0);
	CSurf* pTop = m_aSurfs.GetAt(1);
	pBlk = CreatBlockByMapping(pBot, pTop);

	for(int i = 0; i < pBlk->pPhs.GetSize(); i++) {
		m_aPhds.Add(pBlk->pPhs.GetAt(i));
    }
	m_aBlks.Add(pBlk);
	EmergeBlockGroupsBest();

	// display
	DisplayPolyhedraColor();

	// save
	SaveBlockGroups();

//	m_aSurfs.GetAt(1)->Clear();  delete m_aSurfs.GetAt(1);  m_aSurfs.RemoveAt(1);
//	m_aSurfs.GetAt(0)->Clear();  delete m_aSurfs.GetAt(0);  m_aSurfs.RemoveAt(0);
}


void CRealModelView::OnConvertTrglToPoly()
{
	CSurf * pSurf = new CSurf(); //  must be a global varible

#ifndef EGC
	ReadDataSurface(pSurf); // new format
#else
	ReadDataSurfaceEGC(pSurf); // for EGC
#endif

	for(int i = 0; i < pSurf->pTrs.GetSize(); i++) {
		CLoop * lp = new CLoop();
		for(int j = 0; j < 3; j++) {
			lp->iNds.Add(pSurf->pTrs.GetAt(i)->nID[j]);
		}
		pSurf->pLps.Add(lp);
	}

	SavePolygonSurface(pSurf);
	pSurf->Clear();

}

void CRealModelView::OnConvertTrglToAnsys()
{
	CSurf * pSurf = new CSurf(); //  must be a global varible

#ifndef EGC
	ReadDataSurface(pSurf); // new format
#else
	ReadDataSurfaceEGC(pSurf); // for EGC
#endif
	CConvert Convert;
	Convert.ConvertTrglSurfToANSYS(pSurf);

}

void CRealModelView::OnConvertQuadToAnsys()
{

}


void CRealModelView::OnConvertPolyToAnsys()
{
	CSurf * pSurf = new CSurf(); //  must be a global varible

#ifndef EGC
	ReadPolygonSurface(pSurf); // new format
#else
	
#endif
	CConvert Convert;
	Convert.ConvertPolySurfToANSYS(pSurf);

}


void CRealModelView::OnConvertBlockToAnsys()
{
	CModel * model = new CModel();
	ReadModel(model);
	CConvert Convert;
	Convert.ConvertBlkGroupToANSYS(model);
}


void CRealModelView::OnToolsCheckCCW()
{
	CSurf * pSurf = new CSurf(); //  must be a global varible
	ReadPolygonSurface(pSurf);

	int i, j, k, m, id, *IDs;
	double * px, * py;
	CLoop * lp;
	for(i = 0; i < pSurf->pLps.GetSize(); i++) {
		lp = pSurf->pLps.GetAt(i);
		m = lp->iNds.GetSize();
		if(lp->iNds.GetAt(0) == lp->iNds.GetAt(m-1)) {
			--m;
		}
		px = new double[m];  py = new double[m];
		for(j = 0; j < m; j++) {
			id = lp->iNds.GetAt(j);
			px[j] = pSurf->pNds.GetAt(id)->x;
			py[j] = pSurf->pNds.GetAt(id)->y;
		}
		if(IsCCwize(px, py, m) == false) { // Not CCW, Change
			// For output window
		    CString str;
	        if(pFrame == NULL) {
	           pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	        }
		    str.Format(_T("Not CCW - Changing :  i = %d  [ / %d]"), i, pSurf->pLps.GetSize());
		    pFrame->AddStrToOutputWnd(1, str);

			IDs = new int[m];
			for(k = 0; k < m; k++) {
				IDs[k] = lp->iNds.GetAt(m-1-k);
			}
			lp->iNds.RemoveAll();
			for(k = 0; k < m; k++) {
				lp->iNds.Add(IDs[k]);
			}
			delete [] IDs;
		}
		delete [] px;  delete [] py;
	}

	SavePolygonSurface(pSurf);

}


void CRealModelView::OnToolsCheckSingleValueMesh()
{
	CSurf * pSurf = new CSurf(); //  must be a global varible
	ReadPolygonSurface(pSurf);
	EmergePolygonSurfaceBest(pSurf);

	CNode * na, * nb;
	int num = 0;
	for(int i = 0; i < pSurf->pNds.GetSize() - 1; i++) {
		na = pSurf->pNds.GetAt(i);
		nb = pSurf->pNds.GetAt(i+1);
		if( fabs(na->x - nb->x) <= myEps &&
			fabs(na->y - nb->y) <= myEps ){
				num++;
				// For output window
		        CString str;
	            if( pFrame == NULL ) {
	                pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	            }
		        str.Format(_T("Not single value :  i = %d  [ / %d]"), i, pSurf->pNds.GetSize());
		        pFrame->AddStrToOutputWnd(1, str);
		}
	}

	CString str;
	str.Format(_T(" %d Not single values !"), num);
	AfxMessageBox(str);

	SavePolygonSurface(pSurf);
}

void CRealModelView::OnToolsSelectAPolyhedron()
{
	CModel * model = new CModel();
	ReadModel(model);
	
//	int id = 4177;
	int id = 1816;
	CPohd * phd = model->m_aPhds.GetAt(id);
	m_aPhds.Add(phd);
	CBlock * blk = new CBlock();
	blk->iPhds.Add(0); 
	m_aBlks.Add(blk);

	EmergeBlockGroupsBest();
	SaveBlockGroups();

}


void CRealModelView::OnToolsCombinePolySurf()
{
	CSurf * surfA = new CSurf();
	CSurf * surfB = new CSurf();
	ReadPolygonSurface(surfA);
	ReadPolygonSurface(surfB);

	CSurf * surf = new CSurf();

	int i, j, k, id, add;
	double x, y, z;
	CLoop * lp;

	// Node
	for(i = 0; i < surfA->pNds.GetSize(); i++) {
		x = surfA->pNds.GetAt(i)->x;
		y = surfA->pNds.GetAt(i)->y;
		z = surfA->pNds.GetAt(i)->z;
		surf->pNds.Add(new CNode(i, x, y, z));
	}
	add = surfA->pNds.GetSize();
	for(i = 0; i < surfB->pNds.GetSize(); i++) {
		x = surfB->pNds.GetAt(i)->x;
		y = surfB->pNds.GetAt(i)->y;
		z = surfB->pNds.GetAt(i)->z;
		surf->pNds.Add(new CNode(i+add, x, y, z));
	}

	// Loop
	for(i = 0; i < surfA->pLps.GetSize(); i++) {
	    CLoop * lp = new CLoop();
		for(j = 0; j < surfA->pLps.GetAt(i)->iNds.GetSize(); j++) {
			id = surfA->pLps.GetAt(i)->iNds.GetAt(j);
			lp->iNds.Add(id);
		}
		surf->pLps.Add(lp);
	}
	for(i = 0; i < surfB->pLps.GetSize(); i++) {
	    CLoop * lp = new CLoop();
		for(j = 0; j < surfB->pLps.GetAt(i)->iNds.GetSize(); j++) {
			id = surfB->pLps.GetAt(i)->iNds.GetAt(j) + add;  // ID + add
			lp->iNds.Add(id);
		}
		surf->pLps.Add(lp);
	}

	// Copy Node
	for(i = 0; i < surf->pLps.GetSize(); i++) {
		lp = surf->pLps.GetAt(i);
		for(j = 0; j < lp->iNds.GetSize(); j++) {
			id = lp->iNds.GetAt(j);
			x = surf->pNds.GetAt(id)->x;
			y = surf->pNds.GetAt(id)->y;
			z = surf->pNds.GetAt(id)->z;
			lp->pNds.Add(new CNode(id, x, y, z));
		}
	}

	EmergePolygonSurfaceBest(surf);
//	EmergePolygonSurface(surf);
	SavePolygonSurface(surf);
}


void CRealModelView::OnToolsCombineTrglSurf()
{
	CSurf * surfA = new CSurf();
	CSurf * surfB = new CSurf();
	ReadDataSurface(surfA);
	ReadDataSurface(surfB);

}


void CRealModelView::OnInterpolateIdwBySurface()
{
	CSurf * known = new CSurf();
	ReadDataSurface(known);

	CSurf * unknown = new CSurf();
	ReadDataSurface(unknown);

	vector<CNode*> pts, toInpt;
	
	for(int i = 0; i < known->pNds.GetSize(); i++) {
		pts.push_back(known->pNds.GetAt(i));
	}
	for(int i = 0; i < unknown->pNds.GetSize(); i++) {
		toInpt.push_back(unknown->pNds.GetAt(i));
	}

	CInterpolate * pInpolate = new CInterpolate();
    pInpolate->IDW_STL(pts, toInpt, 4.0);

	SaveTrglMesh(unknown);

}


void CRealModelView::OnInterpolateIdwByPoints()
{
	// TODO: Add your command handler code here
}


