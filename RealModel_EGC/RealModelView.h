
// RealModelView.h : interface of the CRealModelView class
//

#pragma once

/* Close by MeiGang 2013.02.25
#ifndef EGC
   #define EGC // Switch for Exact Geometric Computation
#endif
//*/

#include"MainFrm.h"
#include"RealModelDoc.h"
#include"RealModel.h"
#include"RenderView.h"
#include <string>
#include <vector>

//#include"MeiGang.h"

#include <GL/glut.h>
#include"gl\gl.h"
#include"gl\glu.h"
//#include "gl\glaux.h"

#include"ViewPara.h"
#include"ProgressWnd.h"

#include"SelOption.h"
#include"SelNumPick.h"
#include"NotepadDlg.h"
#include"Prop.h"
#include"PropDef.h"
#include"PropSet.h"

#include"Insect.h" // Sequence is important !
#include"Interpolate.h"
#include"Convert.h"
#include"Open.h"
#include"Save.h"

#include"Line.h"
#include"Edge.h"
#include"Surf.h"
#include"Count.h"
#include"Quad.h"
#include"Block.h"
#include"Cordnt.h"
#include"Vector.h"
#include"Pair.h"
#include"Plane.h"
#include"Pohd.h"
#include"Hehd.h"
#include"Model.h"

class CRealModelView : public CRenderView
{
protected: // create from serialization only
	CRealModelView();
	DECLARE_DYNCREATE(CRealModelView)

// Attributes
public:
	CRealModelDoc* GetDocument() const;
	CMainFrame* pFrame;

	CString m_sOBFile; //name of given object
	CString m_sWorkSpace; //folder of work space
	long m_iOBColor; //color of given object
	int m_iMeshScale; //Scale of mesh
	int m_iBound; //bound type out - 0; inner - 1; polyline - 2;
	int m_iPushnum; //number of time push mouse
	bool m_bMouseinput;
	bool m_iMesh;
	double oldblc;
	int m_nDrawType;
	int m_iSelectMousePoint;
	int m_hvDraw;
	int m_nDrawState;
	int m_idclick;
	CLoop *pLp;
    CSurf* pSurf;
	CBlock* pBlk;
	CCordnt* pCrt;
	int m_iBlockGroup;
	int m_iPhdNumBefAdd; // number of polyhedra before adding a new surface
    int m_iBlkNumBefAdd; // number of block groups before adding a new surface
	int m_nObjectType;   // type of object
	int m_nDisplayMode;  // mode of displaying
	int m_nAntialising;  // antialising ON-1 or OFF-0

	// Boundary Box
	CNode *pBoxLB; // left botton cornor of boundary box
	CNode *pBoxRT; // right top cornor of boundary box
	double minx, miny, minz; // left botton cornor of boundary box
	double maxx, maxy, maxz; // right top cornor of boundary box

	// View Para
	HGLRC m_hRC;			// OpenGL渲染描述表
	CRect m_ClientRect;		// 客户区的大小
	double m_dAspectRatio;	// 窗口的比例
	UINT m_operator;        // 定义视图区操作标示
	UINT m_uShowMode;       // 定义显示模式
	point3D SceneAngle;     // 视图场景旋转角度
	point3D ScenePos;       // 视图场景平移位置
	float  SceneZoom;      // 视图比例系数
	float  SceneDepth;     // 视图区深度
	CPoint  MouseDownPoint; // 鼠标落下点
	point3D view_base_point;// 鼠标按下基点GL坐标系

	GLuint teapotList;
	GLuint ballList;
	
	// Arrays
	CArray<CSurf*, CSurf*> m_aSurfs; // surfaces
 	CArray<CNode*, CNode*> pNds; //total set of regions
	CArray<CLine*, CLine*> pLns; //intersection lines among discontinuities
	CArray<CQuad*, CQuad*> m_aQudgl; //total set of Quadrangles
	CArray<CCount*, CCount*> pList;
	CArray<CCount*, CCount*> pCuts; //total set of regions
	CArray<CPLine*, CPLine*> m_aLns; //total set of polylines
    CArray<CLoop*, CLoop*> m_aLpsIntr; //All intersection (loops)
	CArray<CTrgl*, CTrgl*> pTrs; //total set of regions
	CArray<CNode*, CNode*> m_aNds; // all nodes of all groups
	CArray<CLoop*, CLoop*> m_aLps; // all areas of all groups
    CArray<CPohd*, CPohd*> m_aPhds; // all polyhedrons of all groups
	CArray<CBlock*, CBlock*> m_aBlks; // all goups
	CArray<CPlane*, CPlane*> m_aPlas;
	CArray<CViewPara*, CViewPara*> m_aViewPara; // all sets of view parameters
	int m_iView; // Index of view para
	
	// Select
	CArray<int, int> m_nSelectedPoints;
	CArray<int, int> m_nSelectedLines;
	CArray<int, int> m_nSelectedAreas;
	CArray<int, int> m_nSelectedSurfaces;
	CArray<int, int> m_nSelectedPolyhedrons;
	CArray<int, int> m_nSelectedBlocks;
	int m_nSelectObj;

	CArray<int, int> m_nSelectedIDs; // locoal selected IDs
	CArray<CProp*, CProp*> m_aProps; // all properties
	
    void Replot(int nSelectObj);

// Operations
public:

	// Mathematics
	int gaus(double *a, double *b, int n);
	void Dcinv(double *a, int n);
	void isift(int *p, int i, int n);
	void pihap(int* p, int n);
    void prhap(double* p,int n);	
	void prhap(float* p,int n);
    void rsift(double* p, int i, int n);
	void rsift(float* p, int i, int n);
    void prhap(double* p, int* ID, int n);	
	void prhap(float* p, int* ID, int n);
    void rsift(double* p, int* ID, int i, int n);
	void rsift(float* p, int* ID, int i, int n);
    int qrbsh(double* p, int n, double a, double b, int* m);
	int qrbsh(float* p, int n, float a, float b, int* m);
	void prhap(int* p, int* ID, int n);	
    void rsift(int* p, int* ID, int i, int n);
    int qrbsh(int* p, int n, int a, int b, int* m);
    void SortNode(CNode* p, int n);
	void Regress(int nn, int nv, double* xx, double* yy, double* zz, double* aa);
	int Gauss(double *a, double *b, int n, double *x);

	// Graphics
	void CompileTinList(CSurf* Surf, CCordnt* Crt, int list, int lf);
	void CompilePolygonSurfaceList(CSurf* Surf, CCordnt* Crt, int list, int lf);
	void CompileBlockGroupList(CCordnt* Crt, int list, int lf);
	void glCompilePolygonSurfaceList(CSurf* Surf, CCordnt* Crt, int list, int lf);
	void glCompileBlockGroupList(CCordnt* Crt, int list, int lf);
	void glCompileTinList(CSurf* Surf, CCordnt* Crt, int list, int lf);
	void glCompileTinListAnti(CSurf* Surf, CCordnt* Crt, int list, int lf);
	void glCompileTinListPoint(CSurf* Surf, CCordnt* Crt, int list, int lf);
	void glCompileTinListPointAnti(CSurf* Surf, CCordnt* Crt, int list, int lf);
	void glCompileTinListSegment(CSurf* Surf, CCordnt* Crt, int list, int lf);
	void glCompileTinListSegmentAnti(CSurf* Surf, CCordnt* Crt, int list, int lf);
	void glCompileTinListArea(CSurf* Surf, CCordnt* Crt, int list, int lf);
	void glCompileBlockGroupListPoint(CCordnt* Crt, int list, int lf);
	void glCompileBlockGroupListSegment(CCordnt* Crt, int list, int lf);
	void glCompileBlockGroupListArea(CCordnt* Crt, int list, int lf);
	void glCompileBlockGroupList(CCordnt* Crt, int *list, int lf);
	void glMyLight(void);
	void DrawScene();
	void DisplayPolyhedraColor();
	point2D GetGLValue(point2D pt);
	void SetViewMode();
	void CreateRGBColor(int * RGB, int ID);
	void SaveDisplayControls();
	void RestoreDisplayControls();
	void UpdateViewPara(int iView);
	void AddNewViewPara();
	void GetNewCoordt(CTrgl *tr, CCordnt *NewCordt);
	void UpdateDisplayMode(int nObjectType, int nDisplayMode);
	CCordnt* GetNewCordntSystem(CSurf *Surf);
	CCordnt* GetNewCordntSystemBlockGroup();
	BOOL SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpFileName ); // Capture
	HBITMAP CopyScreenToBitmap(LPRECT lpRect);

	// Intersect
	//// Basic
	  double DistOfTwoNode(CNode *nd1, CNode *nd2);
	  double DistOfPointToPlane(double * pt, double * para);
	  double DistOfPolyhedronToSurface(CPohd * phd, CSurf* surf);
	  double AreaOfTrgl(double x0, double y0, double x1, double y1, double x2, double y2);
	  double AreaOfTrgl(CTrgl * tr);
	  void EquaOfTrgl(double *x, double *y, double *z, double *Para);
	  void EquaOfTrgl(CTrgl * pTrgl, CPlane * pPla);
	  void EquaLine2D( double x1, double y1, double x2, double y2, double para[3]);
	  void EquaLine2D(double x1, double y1, double x2, double y2, CNode *p, CNode *d);
	  double VolOfTetra(double *x, double *y, double *z);
	  void NodeOnNormal(double *x, double *y, double *z, double eps);

	//// Check
	  bool IsSameNode(CNode *nd1, CNode *nd2, double eps);
	  bool IsSameNode(double ax, double ay, double az, double bx, double by, double bz, double eps);
	  int IsPointOnSegment(CNode * sp0, CNode * sp1, CNode * point, double eps);
	  bool IsPointOnSegment(double p0x, double p0y, double p1x, double p1y, double ptx, double pty, double eps);
	  int IsPointOnSegment3D(CNode * sp0, CNode * sp1, CNode * point, double eps);
	  bool IsTwoLpsSame(CLoop * plp1, CLoop * plp2);
	  bool IsTwoEdgeSame(CEdge * eg1, CEdge * eg2);
	  bool IsCCwize(CNode* p, int vcount);
	  bool IsCCwize(double * px, double * py, int vcount);
	  bool IsTwoElesAreSame(double *x0, double *y0, double *z0, double *x1, double * y1, double *z1, int node);
	  int IsNdInTrgl(double xn, double yn, double x0, double y0, double x1, double y1, double x2, double y2);
	  bool IsTwoLoopSame(CLoop* lp1, CLoop* lp2);
	  int IsTwoTrglIntr(CTrgl * pTr0, CTrgl * pTr1, double eps);
	  bool IsNdInPolygon(CNode *pNd, CLoop *pLp, double eps);
	  int IsNdInPolygonNew(CNode *pNd, CLoop *pLp, double eps);
	  int IsTrglInsectBox(CTrgl * pTr, CNode * pBoxMin, CNode * pBoxMax);
	//// Open / Save
	  void ReadData();
      void ReadDataProperty();
	  void ReadDataSurface(CSurf* surf);
	  void ReadDataSurfaceOld(CSurf * surf);
	  void ReadPolygonSurface(CSurf * surf);
	  void CombiLps2D();
	  void EmergeBlockGroups();
	  void EmergePolygonSurface(CSurf * surf);
	  void EmergeTriangleSurface(CSurf * surf);
	  void SaveTrglMesh(CSurf* surf);
	  void SaveQuadMesh(CSurf* surf);
	  void SavePolygonSurface(CSurf * surf);
	  void SaveBlockGroups();
	  void SaveOneLoop(CLoop * lp);
	  void SaveBlockGroupsProperty();
	  void SaveProperty();
	  void OpenProperty();
	  void ImportOFF(CSurf* surf);
	  void ConvertTrglSurfToOFF(CSurf * surf);
	  void ConvertQuadSurfToOFF(CSurf * surf);
	  void ConvertPolySurfToOFF(CSurf * surf);

	//// Key
	  void SearchIntrTrgls(CSurf * pSf0, CSurf * pSf1, double eps);
	  bool IntrOfPolygonAndPolygon(CLoop * plp0, CLoop * plp1, CLoop * pIntr, double eps);
	  int IntrSegment2Segment2D(CNode * p0, CNode * d0, CNode * p1, CNode * d1, CNode * i0, CNode * i1, double eps);
	  int FindIntersection(double u0, double u1, double v0, double v1, double w[2], double eps);
	  void CreatBoundaryBox(CTrgl *pTrgl, CNode *leftBottom, CNode *topRight);
	  void CreatBoundaryBox(CPohd * pPhd, CNode *leftBottom, CNode *topRight);
	  void CreatBoundaryBox(CSurf * pSurf, CNode *leftBottom, CNode *topRight);
	  void CreatBoundaryBox(CNode *leftBottom, CNode *topRight);
	  bool BoundaryBoxIntrTest(CNode *pLB0, CNode *pRT0, CNode *pLB1, CNode *pRT1);
	  int IntrSegment2Segment2D(CNode * p0, CNode * d0, CNode * p1, CNode * d1, CNode * i0, CNode * i1);
	  int IntrOfTriSurfaceAndPolyhedron(CSurf * pRgn, CPohd * pPhd, CPohd * pPosi, CPohd * pNega);
	  bool IntrOfTriangleAndPolyhedron(CTrgl * pTrgl, CPohd * pPhd, CLoop * pIntr);
	  bool IntrOfPlaneAndPolyhedron_CrossIntersection(CPlane * pPla, CPohd * pPhd, CLoop * pIntr);
	  int IntrOfPlaneAndPolyhedron(CPlane * pPla, CPohd * pPhd, CLoop * pIntr);
	  int IntrOfPolygonAndPlaneNew(CLoop * pPlg, CPlane * pPla, CEdge * pEg);
	  bool IntrOfSegAndPlane(CNode * ps, CNode * pe, CPlane * pPla, CNode * pIntrNd);
	  bool IntrOfConvexPolygon(CLoop * plp0, CLoop * plp1, CLoop * pIntr);
	  bool IntrOfConvexPolygonV2(CLoop * plp0, CLoop * plp1, CLoop * pIntr, double eps);
	  void IntrTriangle2Triangle2(CTrgl * pT0, CTrgl * pT1, CLoop * pIntr);
	  int FormIntrLoop2Loop2D(CLoop * p0, CLoop * p1, CLoop * pIntr);
	  void PolygonPartion(CLoop * lp);
	  void PolygonPartionExtended(CLoop * lp);
	  void PolygonPartion3D(CLoop * lp);
	  double AngleToASideInTrgl(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2);
	  double AngleOfVecters(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2);
	  bool whetherTriNeedDivide(CTrgl * tr, double length);
	  void CopyDetailsForSurface(CSurf* rgn);
	  void RenderBlockGroups();
	  void TransPolygonSurfToTriangularSurf(CSurf* topSurf);
	  void InsectOfTrglSurf_2DAnd3D(CSurf * pSurf0, CSurf * pSurf1, CBlock * pBlk);
	  void IntersectWhenAddNewSurface(CSurf* newSurf);
	  void CutWhenAddNewSurface(CSurf* newSurf);
	  void CreatTopSurface(CSurf* topSurf);
	// End Intersect

	// Select
	GLenum glMode; // GL_RENDER, GL_SELECT
	bool bPicked; // whether a polyhedron is picked
	int nPicked; // ID of a single picked polyhedron
	int nPrePicked; // ID of previous picked polyhedron
	int nCurPicked; // ID of current picked polyhedron
	/* void FindNeighborsForPolyhedron();
	void mouseProcess(int x, int y); */
	void glCompileBlockGroupList_Select(CCordnt* Crt, int list, int lf);

	void DisplaySelectedPoint();
	void DisplaySelectedLine();
	void DisplaySelectedArea();
	void DisplaySelectedSurface();
	void DisplaySelectedPolyhedra();
	void DisplaySelectedBlock();

	void InsectOfTrglSurf_2DAnd3DV2(CSurf * pSurf0, CSurf * pSurf1, CBlock * pBlk, double eps);
	void CutAndUpdateTwoTrglSurf(CSurf* sa, CSurf* sb, CSurf* newa, CSurf* newb, double eps);
	bool DividePolygonIntoTwoLps(CLoop* pPg, CPLine* pPline, CLoop* lpa, CLoop* lpb, double eps);
	void CreatePolyhedronByOnlyMapping(CSurf* sa, CSurf* sb, CBlock* pBlk, double eps);
	void CreatBoundaryBox(CLoop *pLp, CNode *leftBottom, CNode *topRight);
	void IntrTriMesh2DAnd3D_Float(CSurf* pRgn0, CSurf* pRgn1, CBlock* pBlk);
	void EquaOfTrgl(float *x, float *y, float *z, float *Para);
	int CStringToInt(CString str, int* Number);

	bool IsLoopCCW(CLoop * lp);
	bool Is3PointCollinear(CNode * sp0, CNode * sp1, CNode * point, double eps);
	void PolygonPartion3D_WithCheck(CLoop * lp);

	// For Exact Geometric Computation (EGC)
	float VolOfTetra(float *x, float *y, float *z);
	void NodeOnNormal(float *x, float *y, float *z, float eps);
	void ReadDataSurfaceEGC(CSurf* surf);

	// Tools
	void OperationsSimple(CSurf * surf, int iAxis);


	// OpenGL, with Vertex Array
	void glCompileBlockGroupListWithArray(CCordnt* Crt, int list, int lf);
	void glCompileBlockGroupListBest(CCordnt* Crt, int list);
	void glCompileSurfaceMeshListBest(CSurf* Surf, CCordnt* Crt, int list);
	void glCompileBlockGroupListBest_Boundary(CCordnt* Crt, int list);

	// New updates for PhD thesis
	void SearchInsectPair2D_OBBTree(CSurf * pSf0, CSurf * pSf1, double eps);
	void InsectOfTrglSurf_2DAnd3DBest(CSurf * pSurf0, CSurf * pSurf1, CBlock * pBlk);
	void EmergeBlockGroupsBest();
	void CreatTopSurfaceBest(CSurf* topSurf);
	void EmergePolygonSurfaceBest(CSurf * surf);
	void CreatBottomSurfaceBest(CSurf* topSurf);
	void InsectOfPolySurf_2AndHalfDBest(CSurf * pSurf0, CSurf * pSurf1, CBlock * pBlk);
	bool IsTwoLoopSameByID(CLoop* lp1, CLoop* lp2);
	void ReadModel(CModel * model);
	void SaveModel(CModel * model);
	void EmergeTriangleSurfaceBest(CSurf * surf);
	CModel * EmergeModel(CModel * ma, CModel * mb);
	void CutWhenAddNewSurfaceFast(CSurf* newSurf);
	int IntrOfTriSurfaceAndPolyhedronFast(CSurf * pRgn, CPohd * pPhd, CPohd * pPosi, CPohd * pNega);
	CBlock * CreatBlockByMapping(CSurf* pBottom, CSurf* pTop);
	CLoop * GetBoundaryLoop(CSurf* tPolySurf);
	void SaveCorrLoops(CSurf* surf);


// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRealModelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnImageZoom();
	afx_msg void OnImagePan();
	afx_msg void OnImageRotate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnImageCapture();
	afx_msg void On3DModelOpenSurface();
	afx_msg void On3DModelIntersect();
	afx_msg void On2DModelOpenSurface();
	afx_msg void On2DModelIntersect();
	afx_msg void On3DModelOpenBlockGroup();
	afx_msg void On3DModelAddSurface();
	afx_msg void On3DModelIntersectAndCut();
	afx_msg void On3DModelSaveBlockGroup();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnImageZoomIn();
	afx_msg void OnImageZoomOut();
	afx_msg void OnImageZoomFit();
	afx_msg void OnImageZoomOriginal();
	afx_msg void OnImageInitialize();
	afx_msg void OnAddSurface();
	afx_msg void OnGetCoordinateSystem();
	afx_msg void OnIntersectAndCut();
	afx_msg void OnOpenBlockGroup();
	afx_msg void OnSaveBlockGroup();
	afx_msg void OnSetCoordinateSystem();
	afx_msg void OnPositionFrontView();
	afx_msg void OnPositionBackView();
	afx_msg void OnPositionLeftView();
	afx_msg void OnPositionRightView();
	afx_msg void OnPositionTopView();
	afx_msg void OnPositionBottomView();
	afx_msg void OnAxisXLeft();
	afx_msg void OnAxisXRight();
	afx_msg void OnAxisYLeft();
	afx_msg void OnAxisYRight();
	afx_msg void OnAxisZLeft();
	afx_msg void OnAxisZRight();
	afx_msg void OnGetCoordSystem();
	afx_msg void OnSetCoordSystem();
	afx_msg void OnMoveLeft();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveDown();
	afx_msg void On2DModelIntersectSlow();
	afx_msg void OnImagePreviousView();
	afx_msg void OnImageNextView();
	afx_msg void OnDisplayTrglSurface();
	afx_msg void OnDisplayQuadSurface();
	afx_msg void OnDisplayPolySurface();
	afx_msg void OnDisplayPolyhedron();
	afx_msg void OnDisplayPointView();
	afx_msg void OnDisplayLineView();
	afx_msg void OnDisplayAreaView();
	afx_msg void OnDisplaySurfaceView();
	afx_msg void OnDisplayPolyhedraView();
	afx_msg void OnDisplayBlockView();
	afx_msg void OnDebugCutUpdateSurface();
	afx_msg void OnDebugOnlyMapping();
	afx_msg void OnDebugIntersectFloat();
	afx_msg void OnSelectEntities();
	afx_msg void OnSelectEverything();
	afx_msg void OnSettingsPropDef();
	afx_msg void OnSettingsPropSet();
	afx_msg void OnDebugReadDataProp();
	afx_msg void OnSettingsSaveProp();
	afx_msg void OnSettingsLoadProp();
	afx_msg void On3DModelOpenBlockGroupProp();
	afx_msg void On3DModelSaveBlockGroupProp();
	afx_msg void On3DModelOpenSurfaceOld();
	afx_msg void OnMeshImportOff();
	afx_msg void OnDebugSurfCutBlock();
	afx_msg void OnInterpolateKrigingOrdinary();
	afx_msg void OnInterpolateKrigingUniversal();
	afx_msg void OnInterpolateDSI();
	afx_msg void OnInterpolateIDW();
	afx_msg void OnToolsByRotateX();
	afx_msg void OnToolsByRotateY();
	afx_msg void OnToolsByRotateZ();
	afx_msg void OnToolsByMove();
	afx_msg void OnToolsByScale();
	afx_msg void OnDebugAntiAlising();
	afx_msg void OnDebugGetTopSurface();
	afx_msg void OnToolsOffsetZ();
	afx_msg void On3DModelGetTopSurface();
	afx_msg void On3DModelCombineBlockGroup();
	afx_msg void On3DModelBlockGroupToAnsys();
	afx_msg void OnToolsGetBottomSurface();
	afx_msg void On2DModelMapping();
	afx_msg void On2DModelOpenPolySurface();
	afx_msg void OnDebugTriangulatePolygonSurface();
	afx_msg void On2DModelMappingComplex();
	afx_msg void OnConvertTrglToPoly();
	afx_msg void OnToolsCheckCCW();
	afx_msg void OnConvertQuadToAnsys();
	afx_msg void OnConvertTrglToAnsys();
	afx_msg void OnConvertPolyToAnsys();
	afx_msg void OnConvertBlockToAnsys();
	afx_msg void OnToolsCheckSingleValueMesh();
	afx_msg void OnToolsSelectAPolyhedron();
	afx_msg void OnToolsCombinePolySurf();
	afx_msg void OnToolsCombineTrglSurf();
	afx_msg void OnInterpolateIdwBySurface();
	afx_msg void OnInterpolateIdwByPoints();
	afx_msg void OnDisplayBlockGroup();
};

#ifndef _DEBUG  // debug version in RealModelView.cpp
inline CRealModelDoc* CRealModelView::GetDocument() const
   { return reinterpret_cast<CRealModelDoc*>(m_pDocument); }
#endif

