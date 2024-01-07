#pragma once

#include"triangle.h"

#include"Pair.h"
#include"Node.h"
#include"Edge.h"
#include"Trgl.h"
#include"Quad.h"
#include"Loop.h"
#include"Surf.h"
#include"Hehd.h"
#include"Block.h"
#include"EarCut.h"

class CInsect
{
public:
	CArray<CPair*, CPair*> m_aInsectPair;
	CArray<CEdge*, CEdge*> m_aInsectEdge;
	CArray<CLoop*, CLoop*> m_aInsectLoop;
	CArray<CSurf*, CSurf*> m_aInsectSurf;
	CArray<CSurf*, CSurf*> m_aSubSurf;
	CArray<CBlock*, CBlock*> m_aSubBlock;
	CArray<CNode*, CNode*> m_aUpdateNode; // Updated Nodes
	CArray<CTrgl*, CTrgl*> m_aUpdateTrgl; // Updated Trgls
	CArray<CLoop*, CLoop*> fromBlk; // sub-loops come from intersection
	CArray<CLoop*, CLoop*> fromSurf; // sub-loops come from intersection
	CSurf* surf;  CBlock* blk;

	void SearchInsectPair(CSurf* surf, CBlock* blk);
	void ObtainInsectEdge(CSurf* surf, CBlock* blk);
	void ObtainInsectEdge_Parallel(CSurf* surf, CBlock* blk);
	void Update(CSurf* surf, CBlock* blk);
	void CreateInsectLoop(CSurf* surf);
	void ObtainInsectSurf(CSurf* surf);
	void CreateSubBlock(CSurf* surf, CBlock* blk);

	void prhap(double* p,int n);
	void rsift(double* p, int i, int n);
	void CreatBoundaryBox(CTrgl *pTrgl, CNode *leftBottom, CNode *topRight);
	void CreatBoundaryBox(CSurf *pSurf, CNode *leftBottom, CNode *topRight);
	void CreatBoundaryBox(CBlock* pBlk, CNode *leftBottom, CNode *topRight);
	bool BoundaryBoxIntrTest(CNode *pLB0, CNode *pRT0, CNode *pLB1, CNode *pRT1);
	int CreateSubLoop(CLoop* pOrig, CLoop* pEdge, CLoop* pSonA, CLoop* pSonB);
	int CreateSubLoop_Hole(CLoop* pOrig, CLoop* pEdge, CLoop* pSonA, CLoop* pSonB);
	void ConnectEdge(CLoop* lp);
	void Merge(CSurf* surf, CBlock* blk);
    void SaveTrglBlock(CBlock* blk);
	void InsectOfSurfAndBlock();

	void ReadSurface();
	void ReadBlock();
	void ReadDataSurface(CSurf* surf);
	void ReadDataSurfaceEGC(CSurf* surf);
	int IsPointOnSegment3D(CNode * sp0, CNode * sp1, CNode * point, double eps);
	void SaveLoop(CLoop* pOrig, CLoop* pEdge);
	void SaveEdge();
	void SaveUpdateNode();
	void SaveSubLoop(CLoop* pLoop);
	bool IsSameNode(CNode *nd1, CNode *nd2, double eps);
	bool IsSameNode(double ax, double ay, double az, double bx, double by, double bz, double eps);

    void Check(CSurf* surf, CBlock* blk);
	void SaveUpdateEdge();
	bool IsSameTrgl(CTrgl* ta, CTrgl*tb);
	void Check_Loop(CSurf* surf, CBlock* blk);
	void Check_Edge(CSurf* surf, CBlock* blk);

    void DivideTrgl_Surf(CSurf* surf);
	void DivideTrgl_Block(CBlock* blk);
	void DivideTrgl_Surf_Hole(CSurf* surf);

	void DivideEachTrgl_CDT(CTrgl* tr, CLoop* lp);

	void CreateSubSurface(CSurf* surf, CBlock* blk);
	void CreateSubBlock();
	void DetermineSubBlock();
	void SaveSubBlockInfo();
	
	void SearchInsectPair_Octree(CSurf* surf, CBlock* blk);
	void SearchInsectPair_OBBTree(CSurf* surf, CBlock* blk); // OBBTree: Rapid Package
	bool IsTrglInsectBoundaryBox(CTrgl* pTr, CNode *pLB, CNode *pRT);
	void CreateInsectLoop_Extend(CSurf* surf);

	void SavePair();
	bool IsPointInPolygon(int polySides, double * polyX, double * polyY, double x, double y);
	bool Is3PointCollinear(CNode * sp0, CNode * sp1, CNode * point, double eps);
	void Update_CDT(CSurf* surf, CBlock* blk);
	void Merge_Fast(CSurf* surf, CBlock* blk);
	void Merge_FirstTime(CSurf* surf, CBlock* blk);

	void SortNode(CNode* p, int n);
	int qrbsh(double* p, int n, double a, double b, int* m);
	void prhap(double* p, int* ID, int n);
	void rsift(double* p, int* ID, int i, int n);
	void pihap(int* p, int n);
	void isift(int *p, int i, int n);
	void SortNode(int n, double * px, double * py, double * pz, int * IDs);

	int IntrLine2Segment2D(CNode * p0, CNode * d0, CNode * p1, CNode * d1, CNode * i0, CNode * i1, double eps);
	void DivideEachTrgl_CDT_Robust(CTrgl* tr, CLoop* lp);

	CInsect(void);
	~CInsect(void);
};

