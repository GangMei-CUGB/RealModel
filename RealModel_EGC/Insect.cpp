#include "StdAfx.h"

// RAPID package 
#include "RAPID.h"
// #pragma comment(lib, "RAPID.lib")

#include "Insect.h"
#include "MainFrm.h"
#include "RealModel.h"
#include "RealModelView.h"

#include "math.h"
#include <fstream>
#include <time.h>
#include "Moller.h"
#include "MollerEGC.h"

using namespace std;   //加上这句就行了

#define PI 3.141592653589793238462643383279502884197169399375105820974944592308
#define MAX 1000000000
#define MIN -1000000000

#define CIGMA 0.000000001
#define fCIGMA 0.00001
#define sqrEpsilon 0.000000001
#define Epsilon 0.00001
#define myEps 0.00001

#define MESH_MODEL_A  1  // indicate mesh model
#define MESH_MODEL_B  2  // indicate mesh model
#define Positive 1
#define Negative -1
#define OUTER 1 // outer sub-surface
#define INNER -1 // inner sub-surface

#define A_U_B 1001 // A union B
#define A_I_B 1002 // A insect B
#define A_S_B 1003 // A subtract B
#define B_S_A 1004 // B subtract A

#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */



CInsect::CInsect(void)
{
}


CInsect::~CInsect(void)
{
}

void CInsect::CreatBoundaryBox(CTrgl *pTrgl, CNode *leftBottom, CNode *topRight)
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

void CInsect::CreatBoundaryBox(CSurf * pSurf, CNode *leftBottom, CNode *topRight)
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

void CInsect::CreatBoundaryBox(CBlock* pBlk, CNode *leftBottom, CNode *topRight)
{
	int N = pBlk->pNds.GetSize();
	double *x = new double[N], *y = new double[N], *z = new double[N];
	double xmin, ymin, zmin, xmax, ymax, zmax;

	for(int i = 0; i < N; i++) {
		x[i] = pBlk->pNds.GetAt(i)->x;
		y[i] = pBlk->pNds.GetAt(i)->y;
		z[i] = pBlk->pNds.GetAt(i)->z;
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

//Node数的堆排序  according to axis x
void CInsect::SortNode(CNode* p, int n)
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
int CInsect::qrbsh(double* p, int n, double a, double b, int* m)
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

void CInsect::prhap(double* p, int* ID, int n)
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

//组堆
void CInsect::rsift(double* p, int* ID, int i, int n)
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

//double数的堆排序
void CInsect::prhap(double* p,int n)
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

//组堆
void CInsect::rsift(double* p, int i, int n)
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

void CInsect::pihap(int* p, int n)
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

void CInsect::isift(int *p, int i, int n)
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

bool CInsect::BoundaryBoxIntrTest(CNode *pLB0, CNode *pRT0, CNode *pLB1, CNode *pRT1)
{
	if(pLB0->x > pRT1->x || pLB0->y > pRT1->y || pLB0->z > pRT1->z) {
		return false;
	}
	if(pLB1->x > pRT0->x || pLB1->y > pRT0->y || pLB1->z > pRT0->z) {
		return false;
	}

	return true;

}

bool CInsect::IsTrglInsectBoundaryBox(CTrgl* pTr, CNode *pLB, CNode *pRT)
{
	double x, y, z;
	for(int i = 0; i < 3; i++) {
		x = pTr->x[i];  y = pTr->y[i];  z = pTr->z[i];
		if( (pLB->x < x && x < pRT->x) &&
			(pLB->y < y && y < pRT->y) && 
			(pLB->z < z && z < pRT->z) ) {
				return true;  break;
		}
	}

	return false;

}

// Search pairs of intersected triangles
void CInsect::SearchInsectPair(CSurf* surf, CBlock* blk)
{
	int i, j;
	CNode *lb_a, *tr_a, *lb_b, *tr_b;
	CTrgl *ta, *tb;
	for(i = 0; i < blk->pTrs.GetSize(); i++) {
		ta = blk->pTrs.GetAt(i);
		lb_a = new CNode();  tr_a = new CNode();
	    CreatBoundaryBox(ta, lb_a, tr_a);
		for(j = 0; j < surf->pTrs.GetSize(); j++) {
			tb = surf->pTrs.GetAt(j);
			lb_b = new CNode();  tr_b = new CNode();
	        CreatBoundaryBox(tb, lb_b, tr_b);

			bool bInsect = BoundaryBoxIntrTest(lb_a, tr_a, lb_b, tr_b);
			if(bInsect == true) {
				m_aInsectPair.Add(new CPair(i, j));
			}
			delete lb_b;  delete tr_b;
		}

		delete lb_a;  delete tr_a;
	}

	CString str;
	str.Format(_T("Intersect Pairs = %d \n"), m_aInsectPair.GetSize() );
	AfxMessageBox(str);

}

// Search pairs of intersected triangles based on Octree
void CInsect::SearchInsectPair_Octree(CSurf* surf, CBlock* blk)
{
	// Create boundary box
	CNode *lb_surf, *tr_surf, *lb_blk, *tr_blk;
	CreatBoundaryBox(surf, lb_surf, tr_surf); // surface
	CreatBoundaryBox(blk, lb_blk, tr_blk); // block

	// Test of boundary box
	CNode *lb_insect, *tr_insect;
	bool bInsect = BoundaryBoxIntrTest(lb_surf, tr_surf, lb_blk, tr_blk);
	if(bInsect == true) {
		lb_insect = new CNode();  tr_insect = new CNode();
		
		lb_insect->x = (lb_surf->x > lb_blk->x ? lb_surf->x : lb_blk->x);
		lb_insect->y = (lb_surf->y > lb_blk->y ? lb_surf->y : lb_blk->y);
		lb_insect->z = (lb_surf->z > lb_blk->z ? lb_surf->z : lb_blk->z);
		tr_insect->x = (tr_surf->x < tr_blk->x ? tr_surf->x : tr_blk->x);
		tr_insect->y = (tr_surf->y < tr_blk->y ? tr_surf->y : tr_blk->y);
		tr_insect->z = (tr_surf->z < tr_blk->z ? tr_surf->z : tr_blk->z);
	}
	else {
		AfxMessageBox(_T("Not intersect ! \nQuit !"));
		exit(0);
	}

	// Get inside or intersected trgls
	int i, j, k;
	CSurf* sonSurf = new CSurf();
	CSurf* sonBlk = new CSurf();
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		CTrgl* tr = surf->pTrs.GetAt(i);
		tr->ID = i;
		if(IsTrglInsectBoundaryBox(tr, lb_insect, tr_insect) == true) {
			sonSurf->pTrs.Add(tr);
		}
	}
	for(i = 0; i < blk->pTrs.GetSize(); i++) {
		CTrgl* tr = blk->pTrs.GetAt(i);
		tr->ID = i;
		if(IsTrglInsectBoundaryBox(tr, lb_insect, tr_insect) == true) {
			sonBlk->pTrs.Add(tr);
		}
	}

	// Octree test


	

}

// 2012.7.24 RAPID Package http://gamma.cs.unc.edu/OBB/
// Search pairs of intersected triangles based on OBB Tree
void CInsect::SearchInsectPair_OBBTree(CSurf* surf, CBlock* blk)
{

#ifndef EGC
	int i;
	CTrgl *ta;

	// Construct
    RAPID_model *b1 = new RAPID_model;
    RAPID_model *b2 = new RAPID_model;

	// Add triangles
	double p0[3], p1[3], p2[3];

	//// blk : model-1
	b1->BeginModel();
	for(i = 0; i < blk->pTrs.GetSize(); i++) {
		ta = blk->pTrs.GetAt(i);
		p0[0] = ta->x[0];  p0[1] = ta->y[0];  p0[2] = ta->z[0];
		p1[0] = ta->x[1];  p1[1] = ta->y[1];  p1[2] = ta->z[1];
		p2[0] = ta->x[2];  p2[1] = ta->y[2];  p2[2] = ta->z[2];
		b1->AddTri(p0, p1, p2, i);
	}
	b1->EndModel();

	//// surf : model-2
	b2->BeginModel();
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		ta = surf->pTrs.GetAt(i);
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
		m_aInsectPair.Add(new CPair(id1, id2));
	}

	delete b1;  delete b2;

	CString str;
	str.Format(_T("Intersect Pairs = %d \n"), m_aInsectPair.GetSize() );
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
	for(i = 0; i < blk->pTrs.GetSize(); i++) {
		ta = blk->pTrs.GetAt(i);
		p0[0] = ta->x[0].floatValue();  p0[1] = ta->y[0].floatValue();  p0[2] = ta->z[0].floatValue();
		p1[0] = ta->x[1].floatValue();  p1[1] = ta->y[1].floatValue();  p1[2] = ta->z[1].floatValue();
		p2[0] = ta->x[2].floatValue();  p2[1] = ta->y[2].floatValue();  p2[2] = ta->z[2].floatValue();
		b1->AddTriEGC(p0, p1, p2, i);  // EGC
	}
	b1->EndModel();

	//// surf : model-2
	b2->BeginModel();
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		ta = surf->pTrs.GetAt(i);
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
		m_aInsectPair.Add(new CPair(id1, id2));
	}

	delete b1;  delete b2;

	CString str;
	str.Format(_T("Intersect Pairs = %d \n"), m_aInsectPair.GetSize() );
	AfxMessageBox(str);
	// exit(0);
	
#endif

}

// Old version, serial code, correct!
// Compute intersection edge
void CInsect::ObtainInsectEdge(CSurf* surf, CBlock* blk)
{
#ifndef EGC
	// Time
	clock_t t1 = clock();

	// Serial Computing
	int i, t, nCase;
	float v0[3], v1[3], v2[3], u0[3], u1[3], u2[3], isec0[3], isec1[3]; // prepare for Moller.dll
	int copla;
	int * coplanar = &copla;
	CPair* pr; CTrgl* ta, *tb;
	
	for(i = 0; i < m_aInsectPair.GetSize(); i++) {
		pr = m_aInsectPair.GetAt(i);
		ta = blk->pTrs.GetAt(pr->FstID);
		tb = surf->pTrs.GetAt(pr->SedID);
		
		v0[0] = ta->x[0];  v0[1] = ta->y[0];  v0[2] = ta->z[0];
		v1[0] = ta->x[1];  v1[1] = ta->y[1];  v1[2] = ta->z[1];
		v2[0] = ta->x[2];  v2[1] = ta->y[2];  v2[2] = ta->z[2];
		
		u0[0] = tb->x[0];  u0[1] = tb->y[0];  u0[2] = tb->z[0];
		u1[0] = tb->x[1];  u1[1] = tb->y[1];  u1[2] = tb->z[1];
		u2[0] = tb->x[2];  u2[1] = tb->y[2];  u2[2] = tb->z[2];
		
		// Load function from DLL Moller.dll 2011.9.30
		typedef int (*lpFun)(float V0[3],float V1[3],float V2[3],
			float U0[3],float U1[3],float U2[3],int *coplanar,
			float isectpt1[3],float isectpt2[3]); // 宏定义函数指针类型
		HINSTANCE hDll; //DLL 句柄
		lpFun trglFun; // 函数指针
		
		hDll = LoadLibrary(_T("Moller.dll")); // 加载DLL
		
		if (NULL==hDll) {
			AfxMessageBox(_T("Fail to load DLL Moller.dll ! "));
		}
		else {
			trglFun = (lpFun)GetProcAddress(hDll, "tri_tri_intersect_with_isectline"); // 加载函数
			if (trglFun != NULL) {
				nCase = trglFun(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
			}
			else {
				AfxMessageBox(_T("Fail to load function from DLL Moller.dll !"));
			}
			FreeLibrary(hDll); // 释放函数
		}
		// End loading
		
		if(nCase == 1) { //save intersection segment
			if(m_aInsectEdge.GetSize() < 1) { // save directly
				CEdge * ptmp = new CEdge();
				ptmp->FstTri = pr->FstID;  ptmp->SecTri = pr->SedID;
				for(t = 0; t < 3; t++) { //交点坐标
					ptmp->FstNd[t] = isec0[t];  
					ptmp->SecNd[t] = isec1[t];
				} 
				m_aInsectEdge.Add(ptmp);
			}
			else {
				bool bexist = false;
				for(t = 0; t < m_aInsectEdge.GetSize(); t++){ 
					if( m_aInsectEdge.GetAt(t)->FstTri == pr->FstID && 
						m_aInsectEdge.GetAt(t)->SecTri == pr->SedID ){ 
						bexist = true;  break; 
					}
				}
				if(!bexist) { // not exist
					CEdge * ptmp = new CEdge();
					ptmp->FstTri = pr->FstID;  ptmp->SecTri = pr->SedID;
					for(t = 0; t < 3; t++) { //交点坐标
						ptmp->FstNd[t] = isec0[t];  
						ptmp->SecNd[t] = isec1[t];
					} 
					m_aInsectEdge.Add(ptmp);
				}
			}
		}
	}

	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		CEdge* eg = m_aInsectEdge.GetAt(i);
		eg->fstNode = new CNode(0, eg->FstNd[0], eg->FstNd[1], eg->FstNd[2]);
		eg->lstNode = new CNode(1, eg->SecNd[0], eg->SecNd[1], eg->SecNd[2]);
	}
	
	// SaveEdge(); 

	clock_t t2 = clock();
	CString str;
	str.Format(_T("Total time = %d\n"), t2-t1);
	AfxMessageBox(str);
	//exit(0);

#else
	// Time
	clock_t t1 = clock();

	// Prepare for Parallel Computing
	int i;
	m_aInsectEdge.RemoveAll();
	for(i = 0; i < m_aInsectPair.GetSize(); i++) {
		CEdge* eg = new CEdge();
		eg->bValid = false;
		m_aInsectEdge.Add(eg);
	}

	// Parallel Computing
    // #pragma omp parallel for  // Canceled
	for(i = 0; i < m_aInsectPair.GetSize(); i++) {
		CPair* pr = m_aInsectPair.GetAt(i);
		CTrgl* ta = blk->pTrs.GetAt(pr->FstID);
		CTrgl* tb = surf->pTrs.GetAt(pr->SedID);
		
		// Load function from DLL Moller.dll 2011.9.30
		double v0[3], v1[3], v2[3], u0[3], u1[3], u2[3], isec0[3], isec1[3]; // prepare for Moller.dll
	    int copla;
	    int * coplanar = &copla;

		v0[0] = ta->x[0];  v0[1] = ta->y[0];  v0[2] = ta->z[0];
		v1[0] = ta->x[1];  v1[1] = ta->y[1];  v1[2] = ta->z[1];
		v2[0] = ta->x[2];  v2[1] = ta->y[2];  v2[2] = ta->z[2];
		
		u0[0] = tb->x[0];  u0[1] = tb->y[0];  u0[2] = tb->z[0];
		u1[0] = tb->x[1];  u1[1] = tb->y[1];  u1[2] = tb->z[1];
		u2[0] = tb->x[2];  u2[1] = tb->y[2];  u2[2] = tb->z[2];

		// CMoller moller; // float-type, not support EGC
	    // int nCase = moller.tri_tri_intersect_with_isectline(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);

		CMollerEGC mollerEGC; // double-type, not support EGC
		int nCase = mollerEGC.tri_tri_intersect_with_isectline(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		
		if(nCase == 1) { //save intersection segment
			m_aInsectEdge.GetAt(i)->bValid = true;
			m_aInsectEdge.GetAt(i)->FstTri = pr->FstID;  
			m_aInsectEdge.GetAt(i)->SecTri = pr->SedID;
			for(int t = 0; t < 3; t++) { //交点坐标
				m_aInsectEdge.GetAt(i)->FstNd[t] = isec0[t];  
				m_aInsectEdge.GetAt(i)->SecNd[t] = isec1[t];
			} 
		}
	}
	// End Parallel Computing

	// Check and Save
	CArray<CEdge*, CEdge*> m_aTmpEdge; // Temp
	for(i = m_aInsectEdge.GetUpperBound(); i > -1; i--) {
		if(m_aInsectEdge.GetAt(i)->bValid == false) {
			delete m_aInsectEdge.GetAt(i);
			m_aInsectEdge.RemoveAt(i);
		}
	}

	m_aTmpEdge.Add(m_aInsectEdge.GetAt(0));
	for(i = 1; i < m_aInsectEdge.GetSize(); i++) {
		CEdge* eg = m_aInsectEdge.GetAt(i);

		bool bexist = false;
		for(int t = 0; t < m_aTmpEdge.GetSize(); t++){ 
			if( m_aTmpEdge.GetAt(t)->FstTri == eg->FstTri && 
				m_aTmpEdge.GetAt(t)->SecTri == eg->SecTri ){ 
				bexist = true;  break; 
			}
		}
		if(!bexist) { // not exist
			m_aTmpEdge.Add(eg);
		}
	}
	m_aInsectEdge.RemoveAll();

	for(i = 0; i < m_aTmpEdge.GetSize(); i++) {
		m_aInsectEdge.Add(m_aTmpEdge.GetAt(i));
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		CEdge* eg = m_aInsectEdge.GetAt(i);
		eg->fstNode = new CNode(0, eg->FstNd[0], eg->FstNd[1], eg->FstNd[2]);
		eg->lstNode = new CNode(1, eg->SecNd[0], eg->SecNd[1], eg->SecNd[2]);
	}

	// SaveEdge(); 

	/* clock_t t2 = clock();
	CString str;
	str.Format(_T("Total time = %d\n"), t2-t1);
	AfxMessageBox(str);
    exit(0); */

#endif

}

// new version, parallel code
// Compute intersection edge
void CInsect::ObtainInsectEdge_Parallel(CSurf* surf, CBlock* blk)
{
#ifndef EGC
	// Time
	clock_t t1 = clock();

	// Prepare for Parallel Computing
	int i;
	m_aInsectEdge.RemoveAll();
	for(i = 0; i < m_aInsectPair.GetSize(); i++) {
		CEdge* eg = new CEdge();
		eg->bValid = false;
		m_aInsectEdge.Add(eg);
	}

	// Parallel Computing
    #pragma omp parallel for
	for(i = 0; i < m_aInsectPair.GetSize(); i++) {
		CPair* pr = m_aInsectPair.GetAt(i);
		CTrgl* ta = blk->pTrs.GetAt(pr->FstID);
		CTrgl* tb = surf->pTrs.GetAt(pr->SedID);
		
		// Load function from DLL Moller.dll 2011.9.30
		float v0[3], v1[3], v2[3], u0[3], u1[3], u2[3], isec0[3], isec1[3]; // prepare for Moller.dll
	    int copla;
	    int * coplanar = &copla;

		v0[0] = ta->x[0];  v0[1] = ta->y[0];  v0[2] = ta->z[0];
		v1[0] = ta->x[1];  v1[1] = ta->y[1];  v1[2] = ta->z[1];
		v2[0] = ta->x[2];  v2[1] = ta->y[2];  v2[2] = ta->z[2];
		
		u0[0] = tb->x[0];  u0[1] = tb->y[0];  u0[2] = tb->z[0];
		u1[0] = tb->x[1];  u1[1] = tb->y[1];  u1[2] = tb->z[1];
		u2[0] = tb->x[2];  u2[1] = tb->y[2];  u2[2] = tb->z[2];

		CMoller moller;
	    int nCase = moller.tri_tri_intersect_with_isectline(v0, v1, v2, u0, u1, u2, coplanar, isec0, isec1);
		
		if(nCase == 1) { //save intersection segment
			m_aInsectEdge.GetAt(i)->bValid = true;
			m_aInsectEdge.GetAt(i)->FstTri = pr->FstID;  
			m_aInsectEdge.GetAt(i)->SecTri = pr->SedID;
			for(int t = 0; t < 3; t++) { //交点坐标
				m_aInsectEdge.GetAt(i)->FstNd[t] = isec0[t];  
				m_aInsectEdge.GetAt(i)->SecNd[t] = isec1[t];
			} 
		}
	}
	// End Parallel Computing

	// Check and Save
	CArray<CEdge*, CEdge*> m_aTmpEdge; // Temp
	for(i = m_aInsectEdge.GetUpperBound(); i > -1; i--) {
		if(m_aInsectEdge.GetAt(i)->bValid == false) {
			delete m_aInsectEdge.GetAt(i);
			m_aInsectEdge.RemoveAt(i);
		}
	}

	m_aTmpEdge.Add(m_aInsectEdge.GetAt(0));
	for(i = 1; i < m_aInsectEdge.GetSize(); i++) {
		CEdge* eg = m_aInsectEdge.GetAt(i);

		bool bexist = false;
		for(int t = 0; t < m_aTmpEdge.GetSize(); t++){ 
			if( m_aTmpEdge.GetAt(t)->FstTri == eg->FstTri && 
				m_aTmpEdge.GetAt(t)->SecTri == eg->SecTri ){ 
				bexist = true;  break; 
			}
		}
		if(!bexist) { // not exist
			m_aTmpEdge.Add(eg);
		}
	}
	m_aInsectEdge.RemoveAll();

	for(i = 0; i < m_aTmpEdge.GetSize(); i++) {
		m_aInsectEdge.Add(m_aTmpEdge.GetAt(i));
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		CEdge* eg = m_aInsectEdge.GetAt(i);
		eg->fstNode = new CNode(0, eg->FstNd[0], eg->FstNd[1], eg->FstNd[2]);
		eg->lstNode = new CNode(1, eg->SecNd[0], eg->SecNd[1], eg->SecNd[2]);
	}

	// SaveEdge(); 

	/* clock_t t2 = clock();
	CString str;
	str.Format(_T("Total time = %d\n"), t2-t1);
	AfxMessageBox(str);
    exit(0); */

#else

#endif

}

// Update node and triangles
void CInsect::Update(CSurf* surf, CBlock* blk)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();

	int i, j, n, id, nCase;
	double x, y, z;

	// Step 1: form divided polygon
	int fst, sec;
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		fst = m_aInsectEdge.GetAt(i)->FstTri;
		sec = m_aInsectEdge.GetAt(i)->SecTri;
		blk->pTrs.GetAt(fst)->iRgnIntrLns.Add(i);
		surf->pTrs.GetAt(sec)->iRgnIntrLns.Add(i);
	}

	DivideTrgl_Block(blk); // For Block
	DivideTrgl_Surf(surf); // For Surface

	// Step 2: polygon partion
	for(i = 0; i < fromBlk.GetSize(); i++) {
		
		// For output window
		CString str;
		str.Format(_T("fromBlk :  i = %d  [ / %d]"), i, fromBlk.GetSize());
		pMain->AddStrToOutputWnd(1, str);
		

		pView->PolygonPartion3D(fromBlk.GetAt(i));
	}

	for(i = 0; i < fromSurf.GetSize(); i++) {
		
		
		// For output window
		CString str;
		str.Format(_T("fromSurf :  i = %d  [ / %d]"), i, fromSurf.GetSize());
		pMain->AddStrToOutputWnd(1, str);
		
		
		pView->PolygonPartion3D(fromSurf.GetAt(i));
	}
	
	// Step 3: emerge node and element
	for(i = blk->pTrs.GetUpperBound(); i > -1; i--) {
		blk->pTrs.GetAt(i)->bNeedCheck = false;
		if(blk->pTrs.GetAt(i)->bValid == false) {
			delete blk->pTrs.GetAt(i);  blk->pTrs.RemoveAt(i);
		}
	}
	for(i = surf->pTrs.GetUpperBound(); i > -1; i--) {
		surf->pTrs.GetAt(i)->bNeedCheck = false;
		if(surf->pTrs.GetAt(i)->bValid == false) {
			delete surf->pTrs.GetAt(i);  surf->pTrs.RemoveAt(i);
		}
	}

	for(i = 0; i < fromBlk.GetSize(); i++) { // Add
		for(j = 0; j < fromBlk.GetAt(i)->pTrs.GetSize(); j++) {
			fromBlk.GetAt(i)->pTrs.GetAt(j)->bValid = false; // false before checking
			fromBlk.GetAt(i)->pTrs.GetAt(j)->bNeedCheck = true;
			blk->pTrs.Add(fromBlk.GetAt(i)->pTrs.GetAt(j));
		}
	}

	for(i = 0; i < fromSurf.GetSize(); i++) { // Add
		for(j = 0; j < fromSurf.GetAt(i)->pTrs.GetSize(); j++) {
			fromSurf.GetAt(i)->pTrs.GetAt(j)->bValid = false; // false before checking
			fromSurf.GetAt(i)->pTrs.GetAt(j)->bNeedCheck = true;
			surf->pTrs.Add(fromSurf.GetAt(i)->pTrs.GetAt(j));
		}
	}

	Merge_Fast(surf, blk); // Finally emerge
//	Check(surf, blk);  

	SaveUpdateNode();  

	/*
	// tmp
	CSurf* tSurf = new CSurf();
	for(i = 0; i < fromSurf.GetSize(); i++) {
		tSurf->pLps.Add(fromSurf.GetAt(i));
	}
	pView->EmergePolygonSurface(tSurf);
	pView->SavePolygonSurface(tSurf);
	pView->ConvertPolySurfToOFF(tSurf);

	CSurf* tmp = new CSurf();
	for(i = 0; i < fromSurf.GetSize(); i++) { // Add
		for(j = 0; j < fromSurf.GetAt(i)->pTrs.GetSize(); j++) {
			tmp->pTrs.Add(fromSurf.GetAt(i)->pTrs.GetAt(j));
		}
	}
	pView->EmergeTriangleSurface(tmp); 
	pView->SaveTrglMesh(tmp);
	pView->ConvertTrglSurfToOFF(tmp);
	exit(0);
	*/
	
	/*
	pView->EmergeTriangleSurface(surf);
	pView->SaveTrglMesh(surf);
	pView->ConvertTrglSurfToOFF(surf);
 
	exit(0);
	*/

	//	SaveTrglBlock(blk); 
	

}

// Update node and triangles
void CInsect::Update_CDT(CSurf* surf, CBlock* blk)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();

	int i, j, n, id, nCase;
	double x, y, z;

	// Step 1: form divided polygon
	int fst, sec;
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		fst = m_aInsectEdge.GetAt(i)->FstTri;
		sec = m_aInsectEdge.GetAt(i)->SecTri;
		blk->pTrs.GetAt(fst)->iRgnIntrLns.Add(i);
		surf->pTrs.GetAt(sec)->iRgnIntrLns.Add(i);
	}

	// Step 2: polygon partion / Constrained delaunay triangulation
	for(i = 0; i < blk->pTrs.GetSize(); i++) {
		
		// For output window
		CString str;
		str.Format(_T("Update_CDT()  blk->pTrs :  i = %d  [ / %d]"), i, blk->pTrs.GetSize());
		pMain->AddStrToOutputWnd(1, str);
		

		CTrgl * tr = blk->pTrs.GetAt(i);

		// No intersection, continue
		if(tr->iRgnIntrLns.GetSize() < 1) { 
			tr->bValid = true;
			continue;
		}
		else {
			tr->bValid = false;

		    // Constrained Delauny
		    CLoop * lp = new CLoop();
	        DivideEachTrgl_CDT(tr, lp);

		    // Save first time
		    fromBlk.Add(lp);
		}
	}

	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		
		// For output window
		CString str;
		str.Format(_T("Update_CDT()  surf->pTrs :  i = %d  [ / %d]"), i, surf->pTrs.GetSize());
		pMain->AddStrToOutputWnd(1, str);
		

		CTrgl * tr = surf->pTrs.GetAt(i);

		// No intersection, continue
		if(tr->iRgnIntrLns.GetSize() < 1) { 
			tr->bValid = true;
			continue;
		}
		else {
			tr->bValid = false;

		    // Constrained Delauny
		    CLoop * lp = new CLoop();
	        DivideEachTrgl_CDT(tr, lp);

		    // Save first time
		    fromSurf.Add(lp);
		}
	}

	// Step 3: emerge node and element
	for(i = blk->pTrs.GetUpperBound(); i > -1; i--) {
		blk->pTrs.GetAt(i)->bNeedCheck = false;
		if(blk->pTrs.GetAt(i)->bValid == false) {
			delete blk->pTrs.GetAt(i);  blk->pTrs.RemoveAt(i);
		}
	}
	for(i = surf->pTrs.GetUpperBound(); i > -1; i--) {
		surf->pTrs.GetAt(i)->bNeedCheck = false;
		if(surf->pTrs.GetAt(i)->bValid == false) {
			delete surf->pTrs.GetAt(i);  surf->pTrs.RemoveAt(i);
		}
	}

	for(i = 0; i < fromBlk.GetSize(); i++) { // Add
		for(j = 0; j < fromBlk.GetAt(i)->pTrs.GetSize(); j++) {
			fromBlk.GetAt(i)->pTrs.GetAt(j)->bValid = false; // false before checking
			fromBlk.GetAt(i)->pTrs.GetAt(j)->bNeedCheck = true;
			blk->pTrs.Add(fromBlk.GetAt(i)->pTrs.GetAt(j));
		}
	}

	for(i = 0; i < fromSurf.GetSize(); i++) { // Add
		for(j = 0; j < fromSurf.GetAt(i)->pTrs.GetSize(); j++) {
			fromSurf.GetAt(i)->pTrs.GetAt(j)->bValid = false; // false before checking
			fromSurf.GetAt(i)->pTrs.GetAt(j)->bNeedCheck = true;
			surf->pTrs.Add(fromSurf.GetAt(i)->pTrs.GetAt(j));
		}
	}

	Merge_Fast(surf, blk); AfxMessageBox(_T("Merge_Fast() is Done !"));
//	Check(surf, blk);  AfxMessageBox(_T("Check() is Done !"));
	SaveUpdateNode();  

	/*
	// tmp
	CSurf* tSurf = new CSurf();
	for(i = 0; i < fromSurf.GetSize(); i++) {
		tSurf->pLps.Add(fromSurf.GetAt(i));
	}
	pView->EmergePolygonSurface(tSurf);
	pView->SavePolygonSurface(tSurf);
	pView->ConvertPolySurfToOFF(tSurf);

	CSurf* tmp = new CSurf();
	for(i = 0; i < fromSurf.GetSize(); i++) { // Add
		for(j = 0; j < fromSurf.GetAt(i)->pTrs.GetSize(); j++) {
			tmp->pTrs.Add(fromSurf.GetAt(i)->pTrs.GetAt(j));
		}
	}
	pView->EmergeTriangleSurface(tmp); 
	pView->SaveTrglMesh(tmp);
	pView->ConvertTrglSurfToOFF(tmp);
	exit(0);
	*/
	
	/*
	pView->EmergeTriangleSurface(surf);
	pView->SaveTrglMesh(surf);
	pView->ConvertTrglSurfToOFF(surf);
 
	exit(0);
	*/

	/*
	Merge(surf, blk); // Finally emerge
	Check(surf, blk); 
	SaveUpdateNode(); 

	pView->SaveTrglMesh(surf);
	SaveTrglBlock(blk); exit(0);
	*/
	

}

// True: if collinear, else FALSE
bool CInsect::Is3PointCollinear(CNode * sp0, CNode * sp1, CNode * point, double eps)
{
	double u1, u2, u3, v1, v2, v3;

	u1 = sp0->x - point->x;  u2 = sp0->y - point->y;  u3 = sp0->z - point->z;

	v1 = point->x - sp1->x;  v2 = point->y - sp1->y;  v3 = point->z - sp1->z;

	double cross = u2 * v3 - u3 * v2 + u3 * v1 - u1 * v3 + u1 * v2 - u2 * v1;

	if(fabs(cross) < eps )  return true;
	else return false;

}

void CInsect::DivideTrgl_Surf(CSurf* surf)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();

	// Special data structure
	CArray<CLoop*, CLoop*> m_aSubEdge; // sub intersect edge 三角形内交线
	CArray<CLoop*, CLoop*> m_aSubLoop; // created sub loops  子多边形
	int i, j, n, id, nCase;
	double x, y, z;
	bool bFinish = false;
	
	// For Surface
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		
		// For output window
		CString str;
		str.Format(_T("DivideTrgl_Surf()  surf->pTrs :  i = %d  [ / %d]"), i, surf->pTrs.GetSize());
		pMain->AddStrToOutputWnd(1, str);
		

		m_aSubEdge.RemoveAll();  m_aSubLoop.RemoveAll();

		CTrgl* tr = surf->pTrs.GetAt(i);
		if(tr->iRgnIntrLns.GetSize() == 0)  continue; // Not intersect
		
		CLoop* pEdge = new CLoop();
		for(j = 0; j < tr->iRgnIntrLns.GetSize(); j++) {
			id = tr->iRgnIntrLns.GetAt(j);
			pEdge->pEgs.Add(m_aInsectEdge.GetAt(id));
		}
		
		if(pEdge->pEgs.IsEmpty() == false) {
			tr->bValid = false;
		}
		else {
			tr->bValid = true;
			delete pEdge;  continue;
		}
		
		//////// Create Sub-Edge
		CLoop* lp = pEdge;

		int ii, oldNum, newNum;
		
		// Initiate
		for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
			lp->pEgs.GetAt(ii)->bUsed = false;
		}
		
		CEdge* start;
		
		CNode* pHead, *pTail;
		
		bFinish = false;
	    while(bFinish != true) {
		    // Find non-used one
		    for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
			    if(lp->pEgs.GetAt(ii)->bUsed == false) {
				    start = lp->pEgs.GetAt(ii);  break;
			    }
		    }
		
		    CLoop* insectLoop = new CLoop();
		    insectLoop->pEgs.Add(start);  start->bUsed = true;
		    insectLoop->pNds.Add(start->fstNode); // Save Node orderly
		    insectLoop->pNds.Add(start->lstNode);
		
		    bool bStop = false;
		    while(bStop != true) { // No more added nodes
			    oldNum = insectLoop->pNds.GetSize();
			
			    pHead = insectLoop->pNds.GetAt(0);
			    pTail = insectLoop->pNds.GetAt(insectLoop->pNds.GetUpperBound());
			
			    for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
				    CEdge* eg = lp->pEgs.GetAt(ii);
				    if(eg->bUsed == true) {  continue;  }
				
					// Two cases: opersite direction
				    if( fabs(eg->SecNd[0] - pHead->x) < myEps && // 后面加入
					    fabs(eg->SecNd[1] - pHead->y) < myEps &&
					    fabs(eg->SecNd[2] - pHead->z) < myEps )
				    {
					    insectLoop->pNds.InsertAt(0, eg->fstNode); 
					    eg->bUsed = true;  break;
				    }
				    if( fabs(eg->FstNd[0] - pHead->x) < myEps && // 后面加入
					    fabs(eg->FstNd[1] - pHead->y) < myEps &&
					    fabs(eg->FstNd[2] - pHead->z) < myEps  )
				    {
					    insectLoop->pNds.InsertAt(0, eg->lstNode);
					    eg->bUsed = true;  break;
				    }
			    }
			    for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
				    CEdge* eg = lp->pEgs.GetAt(ii);
				    if(eg->bUsed == true) {  continue;  }
				
					// Two cases: opersite direction
				    if( fabs(eg->FstNd[0] - pTail->x) < myEps && // 前面加入
					    fabs(eg->FstNd[1] - pTail->y) < myEps &&
					    fabs(eg->FstNd[2] - pTail->z) < myEps )
				    {
					    insectLoop->pNds.Add(eg->lstNode); 
					    eg->bUsed = true;  break;
				    }
				    if( fabs(eg->SecNd[0] - pTail->x) < myEps && // 前面加入
					    fabs(eg->SecNd[1] - pTail->y) < myEps &&
					    fabs(eg->SecNd[2] - pTail->z) < myEps )
				    {
					    insectLoop->pNds.Add(eg->fstNode);
					    eg->bUsed = true;  break;
				    }
			    }
			
			    newNum = insectLoop->pNds.GetSize();
			    if(oldNum == newNum)  bStop = true;
		     } //End while: Create a open or closed edge chain!
		
		     // Save and add
		     m_aSubEdge.Add(insectLoop);

			 // Check
			 bFinish = true;
			 for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
				 if(lp->pEgs.GetAt(ii)->bUsed == false) {
					 bFinish = false;  break;
				 }
			 }
		}
		
		//////// Get new sub-loop
		m_aSubLoop.RemoveAll();
		
		pEdge = m_aSubEdge.GetAt(0);
		CLoop* pOrig = new CLoop();
		CLoop* pSonA = new CLoop();
		CLoop* pSonB = new CLoop();
		for(j = 0; j < 3; j++) {
			x = tr->x[j];  y = tr->y[j];  z = tr->z[j];
			pOrig->pNds.Add(new CNode(j, x, y, z));
		}

		// Divde a triangle into sub-polygon
		nCase = CreateSubLoop(pOrig, pEdge, pSonA, pSonB);

		if(nCase == 1)  { // Only one valid
			pSonA->bValid = true;
			m_aSubLoop.Add(pSonA); delete pSonB; 
		}

		if(nCase == 2)  { // Both two valid
			pSonA->bValid = true;  pSonB->bValid = true;
			m_aSubLoop.Add(pSonA);  m_aSubLoop.Add(pSonB); 
		}
        delete pOrig;
		
		m_aSubEdge.RemoveAt(0); // Use first edge chain !
		if(m_aSubEdge.IsEmpty())  bFinish = true;
		else bFinish = false;
		
		// Use other edge chain !
		while(bFinish != true){
			// Get new sub-loop
			CLoop* pEdge = m_aSubEdge.GetAt(0);
			for(ii = 0; ii < m_aSubLoop.GetSize(); ii++) {
				CLoop* pOrig = m_aSubLoop.GetAt(ii);
				
				CLoop* pSonA = new CLoop();
				CLoop* pSonB = new CLoop();
				
				nCase = CreateSubLoop(pOrig, pEdge, pSonA, pSonB);
				if(nCase == 1)  { 
					m_aSubLoop.Add(pSonA); delete pSonB;
				}
				if(nCase == 2)  { 
					pOrig->bValid = false;
					pSonA->bValid = true;  pSonB->bValid = true;
					m_aSubLoop.Add(pSonA);  m_aSubLoop.Add(pSonB); 
					break;
				}
			}
			
			m_aSubEdge.RemoveAt(0);
			for(ii = m_aSubLoop.GetUpperBound(); ii > - 1; ii--) {
				if(m_aSubLoop.GetAt(ii)->bValid == false)  
					m_aSubLoop.RemoveAt(ii);
			}
			
			// Check whether Stop
			if(m_aSubEdge.IsEmpty())  bFinish = true;
			else bFinish = false;
			
		}
		// End

		/*
		// To save temperily
		if(m_aSubLoop.GetSize() > 2) {
			CSurf* tmp = new CSurf();
			for(ii = 0; ii < m_aSubLoop.GetSize(); ii++) {
				tmp->pLps.Add(m_aSubLoop.GetAt(ii));
			}
			pView->EmergePolygonSurface(tmp);
			pView->SavePolygonSurface(tmp);
		}
		*/
	
		// Save and add
		for(ii = 0; ii < m_aSubLoop.GetSize(); ii++) {
			// Mark parent triangle
			m_aSubLoop.GetAt(ii)->pParent = surf->pTrs.GetAt(i);

			fromSurf.Add(m_aSubLoop.GetAt(ii));
		}
		m_aSubEdge.RemoveAll();  m_aSubLoop.RemoveAll();
		
	}
	
}

void CInsect::DivideTrgl_Block(CBlock* blk)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();

	// Special data structure
	CArray<CLoop*, CLoop*> m_aSubEdge; // sub intersect edge 三角形内交线
	CArray<CLoop*, CLoop*> m_aSubLoop; // created sub loops  子多边形
	int i, j, n, id, nCase;
	double x, y, z;
	bool bFinish = false;
	
	// For block
	for(i = 1018; i < 1019; i++) {
//	for(i = 0; i < blk->pTrs.GetSize(); i++) {
		
		// For output window
		CString str;
		str.Format(_T("DivideTrgl_Block()  blk->pTrs :  i = %d  [ / %d]"), i, blk->pTrs.GetSize());
		pMain->AddStrToOutputWnd(1, str);
		

		m_aSubEdge.RemoveAll();  m_aSubLoop.RemoveAll();

		CTrgl* tr = blk->pTrs.GetAt(i);
		if(tr->iRgnIntrLns.GetSize() == 0)  continue;
		
		CLoop* pEdge = new CLoop();
		for(j = 0; j < tr->iRgnIntrLns.GetSize(); j++) {
			id = tr->iRgnIntrLns.GetAt(j);
			pEdge->pEgs.Add(m_aInsectEdge.GetAt(id));
		}
		
		if(pEdge->pEgs.IsEmpty() == false) {
			tr->bValid = false;
		}
		else {
			tr->bValid = true;
			delete pEdge;  continue;
		}
		
		//////// Create Sub-Edge
		CLoop* lp = pEdge;

		int ii, oldNum, newNum;
		
		// Initiate
		for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
			lp->pEgs.GetAt(ii)->bUsed = false;
		}
		
		CEdge* start;
		
		CNode* pHead, *pTail;
		
		bFinish = false;
	    while(bFinish != true) {
		    // Find non-used one
		    for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
			    if(lp->pEgs.GetAt(ii)->bUsed == false) {
				    start = lp->pEgs.GetAt(ii);  break;
			    }
		    }
		
		    CLoop* insectLoop = new CLoop();
		    insectLoop->pEgs.Add(start);  start->bUsed = true;
		    insectLoop->pNds.Add(start->fstNode); // Save Node orderly
		    insectLoop->pNds.Add(start->lstNode);
		
		    bool bStop = false;
		    while(bStop != true) { // No more added nodes
			    oldNum = insectLoop->pNds.GetSize();
			
			    pHead = insectLoop->pNds.GetAt(0);
			    pTail = insectLoop->pNds.GetAt(insectLoop->pNds.GetUpperBound());
			
			    for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
				    CEdge* eg = lp->pEgs.GetAt(ii);
				    if(eg->bUsed == true) {  continue;  }
				
				    if( fabs(eg->SecNd[0] - pHead->x) < myEps && // 后面加入
					    fabs(eg->SecNd[1] - pHead->y) < myEps &&
					    fabs(eg->SecNd[2] - pHead->z) < myEps )
				    {
					    insectLoop->pNds.InsertAt(0, eg->fstNode); 
					    eg->bUsed = true;  break;
				    }
				    if( fabs(eg->FstNd[0] - pHead->x) < myEps && // 后面加入
					    fabs(eg->FstNd[1] - pHead->y) < myEps &&
					    fabs(eg->FstNd[2] - pHead->z) < myEps  )
				    {
					    insectLoop->pNds.InsertAt(0, eg->lstNode);
					    eg->bUsed = true;  break;
				    }
			    }
			    for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
				    CEdge* eg = lp->pEgs.GetAt(ii);
				    if(eg->bUsed == true) {  continue;  }
				
				    if( fabs(eg->FstNd[0] - pTail->x) < myEps && // 前面加入
					    fabs(eg->FstNd[1] - pTail->y) < myEps &&
					    fabs(eg->FstNd[2] - pTail->z) < myEps )
				    {
					    insectLoop->pNds.Add(eg->lstNode); 
					    eg->bUsed = true;  break;
				    }
				    if( fabs(eg->SecNd[0] - pTail->x) < myEps && // 前面加入
					    fabs(eg->SecNd[1] - pTail->y) < myEps &&
					    fabs(eg->SecNd[2] - pTail->z) < myEps )
				    {
					    insectLoop->pNds.Add(eg->fstNode);
					    eg->bUsed = true;  break;
				    }
			    }
			
			    newNum = insectLoop->pNds.GetSize();
			    if(oldNum == newNum)  bStop = true;
		     } //End while
		
		     // Save and add
		     m_aSubEdge.Add(insectLoop);

			 // tmp
			 SaveSubLoop(insectLoop);

			 // Check
			 bFinish = true;
			 for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
				 if(lp->pEgs.GetAt(ii)->bUsed == false) {
					 bFinish = false;  break;
				 }
			 }
		}

		//exit(0);
		
		//////// Get new sub-loop
		m_aSubLoop.RemoveAll();
		
		pEdge = m_aSubEdge.GetAt(0);
		CLoop* pOrig = new CLoop();
		CLoop* pSonA = new CLoop();
		CLoop* pSonB = new CLoop();
		for(j = 0; j < 3; j++) {
			x = tr->x[j];  y = tr->y[j];  z = tr->z[j];
			pOrig->pNds.Add(new CNode(j, x, y, z));
		}
		nCase = CreateSubLoop(pOrig, pEdge, pSonA, pSonB);
		if(nCase == 1)  { 
			pSonA->bValid = true;
			m_aSubLoop.Add(pSonA); delete pSonB; 
		}
		if(nCase == 2)  { 
			pSonA->bValid = true;  pSonB->bValid = true;
			m_aSubLoop.Add(pSonA);  m_aSubLoop.Add(pSonB); 
		}
        delete pOrig;
		
		m_aSubEdge.RemoveAt(0);
		if(m_aSubEdge.IsEmpty())  bFinish = true;
		else bFinish = false;
		
		while(bFinish != true){
			// Get new sub-loop
			CLoop* pEdge = m_aSubEdge.GetAt(0);
			for(ii = 0; ii < m_aSubLoop.GetSize(); ii++) {
				CLoop* pOrig = m_aSubLoop.GetAt(ii);
				
				CLoop* pSonA = new CLoop();
				CLoop* pSonB = new CLoop();
				
				nCase = CreateSubLoop(pOrig, pEdge, pSonA, pSonB);
				if(nCase == 1)  { 
					m_aSubLoop.Add(pSonA); delete pSonB;
				}
				if(nCase == 2)  { 
					pOrig->bValid = false;
					pSonA->bValid = true;  pSonB->bValid = true;
					m_aSubLoop.Add(pSonA);  m_aSubLoop.Add(pSonB); 
					break;
				}
			}
			
			m_aSubEdge.RemoveAt(0);
			for(ii = m_aSubLoop.GetUpperBound(); ii > - 1; ii--) {
				if(m_aSubLoop.GetAt(ii)->bValid == false)  
					m_aSubLoop.RemoveAt(ii);
			}
			
			// Check whether Stop
			if(m_aSubEdge.IsEmpty())  bFinish = true;
			else bFinish = false;
			
		}
		// End

		// Save and add
		for(ii = 0; ii < m_aSubLoop.GetSize(); ii++) {
			// Mark parent triangle
			m_aSubLoop.GetAt(ii)->pParent = blk->pTrs.GetAt(i);

			fromBlk.Add(m_aSubLoop.GetAt(ii));
		}
		m_aSubEdge.RemoveAll();  m_aSubLoop.RemoveAll();
		
	}
	
	
}

void CInsect::DivideTrgl_Surf_Hole(CSurf* surf)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();

	// Special data structure
	CArray<CLoop*, CLoop*> m_aSubEdge; // sub intersect edge 三角形内交线
	CArray<CLoop*, CLoop*> m_aSubLoop; // created sub loops  子多边形
	int i, j, n, id, nCase;
	double x, y, z;
	bool bFinish = false;
	
	// For Surface
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		/*
		// For output window
		CString str;
		str.Format(_T("surf->pTrs :  i = %d  [ / %d]"), i, surf->pTrs.GetSize());
		pMain->AddStrToOutputWnd(1, str);
		*/

		m_aSubEdge.RemoveAll();  m_aSubLoop.RemoveAll();

		CTrgl* tr = surf->pTrs.GetAt(i);
		if(tr->iRgnIntrLns.GetSize() == 0)  continue; // Not intersect
		
		CLoop* pEdge = new CLoop();
		for(j = 0; j < tr->iRgnIntrLns.GetSize(); j++) {
			id = tr->iRgnIntrLns.GetAt(j);
			pEdge->pEgs.Add(m_aInsectEdge.GetAt(id));
		}
		
		if(pEdge->pEgs.IsEmpty() == false) {
			tr->bValid = false;
		}
		else {
			tr->bValid = true;
			delete pEdge;  continue;
		}
		
		//////// Create Sub-Edge
		CLoop* lp = pEdge;

		int ii, oldNum, newNum;
		
		// Initiate
		for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
			lp->pEgs.GetAt(ii)->bUsed = false;
		}
		
		CEdge* start;
		
		CNode* pHead, *pTail;
		
		bFinish = false;
	    while(bFinish != true) {
		    // Find non-used one
		    for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
			    if(lp->pEgs.GetAt(ii)->bUsed == false) {
				    start = lp->pEgs.GetAt(ii);  break;
			    }
		    }
		
		    CLoop* insectLoop = new CLoop();
		    insectLoop->pEgs.Add(start);  start->bUsed = true;
		    insectLoop->pNds.Add(start->fstNode); // Save Node orderly
		    insectLoop->pNds.Add(start->lstNode);
		
		    bool bStop = false;
		    while(bStop != true) { // No more added nodes
			    oldNum = insectLoop->pNds.GetSize();
			
			    pHead = insectLoop->pNds.GetAt(0);
			    pTail = insectLoop->pNds.GetAt(insectLoop->pNds.GetUpperBound());
			
			    for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
				    CEdge* eg = lp->pEgs.GetAt(ii);
				    if(eg->bUsed == true) {  continue;  }
				
					// Two cases: opersite direction
				    if( fabs(eg->SecNd[0] - pHead->x) < myEps && // 后面加入
					    fabs(eg->SecNd[1] - pHead->y) < myEps &&
					    fabs(eg->SecNd[2] - pHead->z) < myEps )
				    {
					    insectLoop->pNds.InsertAt(0, eg->fstNode); 
					    eg->bUsed = true;  break;
				    }
				    if( fabs(eg->FstNd[0] - pHead->x) < myEps && // 后面加入
					    fabs(eg->FstNd[1] - pHead->y) < myEps &&
					    fabs(eg->FstNd[2] - pHead->z) < myEps  )
				    {
					    insectLoop->pNds.InsertAt(0, eg->lstNode);
					    eg->bUsed = true;  break;
				    }
			    }
			    for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
				    CEdge* eg = lp->pEgs.GetAt(ii);
				    if(eg->bUsed == true) {  continue;  }
				
					// Two cases: opersite direction
				    if( fabs(eg->FstNd[0] - pTail->x) < myEps && // 前面加入
					    fabs(eg->FstNd[1] - pTail->y) < myEps &&
					    fabs(eg->FstNd[2] - pTail->z) < myEps )
				    {
					    insectLoop->pNds.Add(eg->lstNode); 
					    eg->bUsed = true;  break;
				    }
				    if( fabs(eg->SecNd[0] - pTail->x) < myEps && // 前面加入
					    fabs(eg->SecNd[1] - pTail->y) < myEps &&
					    fabs(eg->SecNd[2] - pTail->z) < myEps )
				    {
					    insectLoop->pNds.Add(eg->fstNode);
					    eg->bUsed = true;  break;
				    }
			    }
			
			    newNum = insectLoop->pNds.GetSize();
			    if(oldNum == newNum)  bStop = true;

		     } //End while: Create a open or closed edge chain!
		
			 // tmp to save
		   	SaveSubLoop(insectLoop);

		     // Save and add
		     m_aSubEdge.Add(insectLoop);

			 // Check
			 bFinish = true;
			 for(ii = 0; ii < lp->pEgs.GetSize(); ii++) {
				 if(lp->pEgs.GetAt(ii)->bUsed == false) {
					 bFinish = false;  break;
				 }
			 }
		}
		
		//////// Get new sub-loop
		m_aSubLoop.RemoveAll();
		
		pEdge = m_aSubEdge.GetAt(0);
		CLoop* pOrig = new CLoop();
		CLoop* pSonA = new CLoop();
		CLoop* pSonB = new CLoop();
		for(j = 0; j < 3; j++) {
			x = tr->x[j];  y = tr->y[j];  z = tr->z[j];
			pOrig->pNds.Add(new CNode(j, x, y, z));
		}

		// Divde a triangle into sub-polygon
		// nCase = CreateSubLoop(pOrig, pEdge, pSonA, pSonB);
		nCase = CreateSubLoop_Hole(pOrig, pEdge, pSonA, pSonB);

		if(nCase == 1)  { // Only one valid
			pSonA->bValid = true;
			m_aSubLoop.Add(pSonA); delete pSonB; 
		}

		if(nCase == 2)  { // Both two valid
			pSonA->bValid = true;  pSonB->bValid = true;
			m_aSubLoop.Add(pSonA);  m_aSubLoop.Add(pSonB); 
		}
        delete pOrig;
		
		m_aSubEdge.RemoveAt(0); // Use first edge chain !
		if(m_aSubEdge.IsEmpty())  bFinish = true;
		else bFinish = false;
		
		// Use other edge chain !
		while(bFinish != true){
			// Get new sub-loop
			CLoop* pEdge = m_aSubEdge.GetAt(0);
			for(ii = 0; ii < m_aSubLoop.GetSize(); ii++) {
				CLoop* pOrig = m_aSubLoop.GetAt(ii);
				
				CLoop* pSonA = new CLoop();
				CLoop* pSonB = new CLoop();
				
				// nCase = CreateSubLoop(pOrig, pEdge, pSonA, pSonB);
				nCase = CreateSubLoop_Hole(pOrig, pEdge, pSonA, pSonB);

				if(nCase == 1)  { 
					m_aSubLoop.Add(pSonA); delete pSonB;
				}

				if(nCase == 2)  { 
					pOrig->bValid = false;
					pSonA->bValid = true;  pSonB->bValid = true;
					m_aSubLoop.Add(pSonA);  m_aSubLoop.Add(pSonB); 
					break;
				}

			}
			
			m_aSubEdge.RemoveAt(0);
			for(ii = m_aSubLoop.GetUpperBound(); ii > - 1; ii--) {
				if(m_aSubLoop.GetAt(ii)->bValid == false)  
					m_aSubLoop.RemoveAt(ii);
			}
			
			// Check whether Stop
			if(m_aSubEdge.IsEmpty())  bFinish = true;
			else bFinish = false;
			
		}
		// End

		/*
		// To save temperily
		if(m_aSubLoop.GetSize() > 2) {
			CSurf* tmp = new CSurf();
			for(ii = 0; ii < m_aSubLoop.GetSize(); ii++) {
				tmp->pLps.Add(m_aSubLoop.GetAt(ii));
			}
			pView->EmergePolygonSurface(tmp);
			pView->SavePolygonSurface(tmp);
		}
		*/
	
		// Save and add
		for(ii = 0; ii < m_aSubLoop.GetSize(); ii++) {
			fromSurf.Add(m_aSubLoop.GetAt(ii));
		}
		m_aSubEdge.RemoveAll();  m_aSubLoop.RemoveAll();
		
	}
	
	
}

void CInsect::DivideEachTrgl_CDT(CTrgl* tr, CLoop* lp)
{
#ifndef EGC
	int i, j, k, id, nid, eid, nID[3];
	double nx, ny, nz, x, y, z;
	
	// Create all Points and Segments
	CArray<CNode*, CNode*> allPt;
	CArray<CNode*, CNode*> newPt;
	CArray<CPair*, CPair*> allSg;
	
	// First 3 Point of triangle
	id= 0;
	for(i = 0; i < 3; i++) {
		nx = tr->x[i];  ny = tr->y[i];  nz = tr->z[i];
		allPt.Add(new CNode(id++, nx, ny, nz));
	}
	
	// Then, the intersection edges
	bool bexist, bsame;
	for(i = 0; i < tr->iRgnIntrLns.GetSize(); i++) {
		eid = tr->iRgnIntrLns.GetAt(i);
		CEdge * eg = m_aInsectEdge.GetAt(eid);
		
		CPair * pr = new CPair();
		
		// First node
		x = eg->FstNd[0];  y = eg->FstNd[1];  z = eg->FstNd[2];
		
		bexist = false;
		for(j = 0; j < allPt.GetSize(); j++) {
			nx = allPt.GetAt(j)->x;
			ny = allPt.GetAt(j)->y;
			nz = allPt.GetAt(j)->z;
			
			if(IsSameNode(nx, ny, nz, x, y, z, myEps) == true) {
				bexist = true;  nid = j;  break;
			}
		}
		
		if(bexist == true) {
			pr->FstID = nid; // Set ID
		}
		else {
			nid = allPt.GetSize(); // Set ID
			allPt.Add(new CNode(id++, x, y, z)); // Add new node
			pr->FstID = nid;
		}
		
		// Second node
		x = eg->SecNd[0];  y = eg->SecNd[1];  z = eg->SecNd[2];
		
		bexist = false;
		for(j = 0; j < allPt.GetSize(); j++) {
			nx = allPt.GetAt(j)->x;
			ny = allPt.GetAt(j)->y;
			nz = allPt.GetAt(j)->z;
			
			if(IsSameNode(nx, ny, nz, x, y, z, myEps) == true) {
				bexist = true;  nid = j;  break;
			}
		}
		
		if(bexist == true) {
			pr->SedID = nid;
		}
		else {
			nid = allPt.GetSize();
			allPt.Add(new CNode(id++, x, y, z));
			pr->SedID = nid;
		}
		
		// Finally, save segment / constrained edge
		allSg.Add(pr);
		
	}
	
	// Create Boundary edges, very important !
	int id1, id2;
	CNode * pt1, * pt2, * endpoint;
	CArray<int, int> boundaryID;
	boundaryID.Add(0); // first
	boundaryID.Add(1); // second
	boundaryID.Add(2); // third
	boundaryID.Add(0); // first again
	for(i = 0; i < allSg.GetSize(); i++) {
		// First endpoint
		id = allSg.GetAt(i)->FstID;
		endpoint = allPt.GetAt(id);

		for(j = 0; j < boundaryID.GetSize() - 1; j++) {
			id1 = boundaryID.GetAt(j);
			id2 = boundaryID.GetAt(j+1);

			pt1 = allPt.GetAt(id1);
			pt2 = allPt.GetAt(id2);

			if( IsPointOnSegment3D(pt1, pt2, endpoint, myEps) == 2 ) { // between, not on endpoints
				boundaryID.InsertAt(j+1, id);
				break;
			}
		}

		// Second endpoint
		id = allSg.GetAt(i)->SedID;
		endpoint = allPt.GetAt(id);

		for(j = 0; j < boundaryID.GetSize() - 1; j++) {
			id1 = boundaryID.GetAt(j);
			id2 = boundaryID.GetAt(j+1);

			pt1 = allPt.GetAt(id1);
			pt2 = allPt.GetAt(id2);

			if( IsPointOnSegment3D(pt1, pt2, endpoint, myEps) == 2 ) { // between, not on endpoints
				boundaryID.InsertAt(j+1, id);
				break;
			}
		}
	}


	// 3D to 2D
	
	// Step 1: compute local coordinates system and transfer
	CCordnt * tCrdt = new CCordnt();
	CEarCut *earCut = new CEarCut();
	earCut->GetNewCoordt(tr, tCrdt);
	
	// change to XY plane
	double * tx, * ty, * tz, * a;
	
	int N = allPt.GetSize();
	
	tx = new double[N];  ty = new double[N];  tz = new double[N];
	
	for(i = 0; i < N; i++) {
		tx[i] = allPt.GetAt(i)->x;
		ty[i] = allPt.GetAt(i)->y;
		tz[i] = allPt.GetAt(i)->z;
	}
	
	// very important
	a = new double[9];
	a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
	a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
	a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;
	earCut->Dcinv(a, 3);
	
	// new points, planar
	for(i = 0; i < N; i++) { // transform
		tx[i] -= tCrdt->u;  ty[i] -= tCrdt->v;  tz[i] -= tCrdt->w;
		newPt.Add(new CNode(i, tx[i], ty[i], tz[i]));
	}
	for(i = 0; i < N; i++) {
		newPt.GetAt(i)->x = tx[i] * a[0] + ty[i] * a[1] + tz[i] * a[2];
		newPt.GetAt(i)->y = tx[i] * a[3] + ty[i] * a[4] + tz[i] * a[5];
		newPt.GetAt(i)->z = tx[i] * a[6] + ty[i] * a[7] + tz[i] * a[8];
	}

	// Avoid float-point problem
	for(i = 0; i < N; i++) {
		if (fabs(newPt.GetAt(i)->x) < myEps) {
			newPt.GetAt(i)->x = 0.0;
		}
		if (fabs(newPt.GetAt(i)->y) < myEps) {
			newPt.GetAt(i)->y = 0.0;
		}
		if (fabs(newPt.GetAt(i)->z) < myEps) {
			newPt.GetAt(i)->z = 0.0;
		}
	}
	
	delete [] tx;  delete [] ty;  delete [] tz;  delete [] a;
	
	// Now, use CDT
	triangulateio in, mid, out, vorout;
	
	// Define input points. 
	in.numberofpoints = newPt.GetSize();
	in.numberofpointattributes = 0;
	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
	for(i = 0; i < in.numberofpoints; i++) {
		in.pointlist[2*i] = newPt.GetAt(i)->x;
		in.pointlist[2*i+1] = newPt.GetAt(i)->y;
	}

	// Define constrained edges + boundary edges
	in.numberofsegments = allSg.GetSize() + (boundaryID.GetSize() - 1);
	in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));
	in.segmentmarkerlist = (int *) malloc(in.numberofsegments * sizeof(int));
	for(i = 0; i < allSg.GetSize(); i++) {
		in.segmentlist[2*i] = allSg.GetAt(i)->FstID;
		in.segmentlist[2*i+1] = allSg.GetAt(i)->SedID;

		in.segmentmarkerlist[i] = 0; // Not boundary edge, interior edge
	}
	for(i = 0; i < (boundaryID.GetSize() - 1); i++) {
		in.segmentlist[ 2*(i+allSg.GetSize()) ] = boundaryID.GetAt(i);
		in.segmentlist[2*(i+allSg.GetSize())+1] = boundaryID.GetAt(i+1);

		in.segmentmarkerlist[i+allSg.GetSize()] = 1;
	}

	in.numberofholes = 0;
	in.numberofregions = 0;
	in.regionlist = (REAL *) NULL;

	in.pointattributelist = (REAL *) NULL;
	in.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
	in.triangleattributelist = (REAL *) NULL;
	in.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */

	/* Make necessary initializations so that Triangle can return a */
	/*   triangulation in `mid' and a voronoi diagram in `vorout'.  */
	mid.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
	/* Not needed if -N switch used or number of point attributes is zero: */
	mid.pointattributelist = (REAL *) NULL;
	mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
	mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
	/* Not needed if -E switch used or number of triangle attributes is zero: */
	mid.triangleattributelist = (REAL *) NULL;
	mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
	/* Needed only if segments are output (-p or -c) and -P not used: */
	mid.segmentlist = (int *) NULL;
	/* Needed only if segments are output (-p or -c) and -P and -B not used: */
	mid.segmentmarkerlist = (int *) NULL;
	mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
	mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */
	
	/* Triangulate the points.  Switches are chosen to read and write a  */
	/*   PSLG (p), preserve the convex hull (c), number everything from  */
	/*   zero (z), assign a regional attribute to each element (A), and  */
	/*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
	/*   neighbor list (n). 
	*/

//	triangulate("pcz", &in, &mid, &vorout);
	triangulate("pz", &in, &mid, &vorout);

	// Very important !  Update and save !
	// Save Node and Trgl, to CLoop* lp
	if (allPt.GetSize() == mid.numberofpoints) {
	    for (i = 0; i < allPt.GetSize(); i++) {
		    lp->pNds.Add(allPt.GetAt(i));
	    }

	    for (i = 0; i < mid.numberoftriangles; i++) { // Trgl
            for (j = 0; j < mid.numberofcorners; j++) {
			    nID[j] = mid.trianglelist[i * mid.numberofcorners + j];
            }
		    lp->pTrs.Add(new CTrgl(i, nID[0], nID[1], nID[2]));
	    }

	    for(i = 0; i < lp->pTrs.GetSize(); i++) { // Must copy, otherwise error
		    for (j = 0; j < 3; j++) {
		    	id = lp->pTrs.GetAt(i)->nID[j];
		    	lp->pTrs.GetAt(i)->x[j] = lp->pNds.GetAt(id)->x;
			    lp->pTrs.GetAt(i)->y[j] = lp->pNds.GetAt(id)->y;
			    lp->pTrs.GetAt(i)->z[j] = lp->pNds.GetAt(id)->z;
		    }
    	}

	}

	else {
	//	AfxMessageBox(_T("Warning:  DivideEachTrgl_CDT \nallPt.GetSize() != mid.numberofpoints"));

		// Change it back!
		N = mid.numberofpoints;
	    tx = new double[N];  ty = new double[N];  tz = new double[N];
	    for(i = 0; i < N; i++) { // save temporily
			tx[i] = mid.pointlist[2*i];
		    ty[i] = mid.pointlist[2*i+1];
		    tz[i] = 0.0;
	    }

        // get the original version of new coordinate, local coordinate system
		a = new double[9];
	    a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
	    a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
	    a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;

		// Empty nodes
		for(i = 0; i < N; i++) {
			lp->pNds.Add(new CNode());
		}

	    for(i = 0; i < N; i++) { // rotate
			lp->pNds.GetAt(i)->x = tx[i] * a[0] + ty[i] * a[1] + tz[i] * a[2];
		    lp->pNds.GetAt(i)->y = tx[i] * a[3] + ty[i] * a[4] + tz[i] * a[5];
		    lp->pNds.GetAt(i)->z = tx[i] * a[6] + ty[i] * a[7] + tz[i] * a[8];
	    }

	    for(i = 0; i < N; i++) { // transform
		    lp->pNds.GetAt(i)->x += tCrdt->u;
		    lp->pNds.GetAt(i)->y += tCrdt->v;
		    lp->pNds.GetAt(i)->z += tCrdt->w;
	    }

		for (i = 0; i < mid.numberoftriangles; i++) { // Trgl
            for (j = 0; j < mid.numberofcorners; j++) {
			    nID[j] = mid.trianglelist[i * mid.numberofcorners + j];
            }
		    lp->pTrs.Add(new CTrgl(i, nID[0], nID[1], nID[2]));
	    }

	    for(i = 0; i < lp->pTrs.GetSize(); i++) { // Must copy, otherwise error
		    for (j = 0; j < 3; j++) {
		    	id = lp->pTrs.GetAt(i)->nID[j];
		    	lp->pTrs.GetAt(i)->x[j] = lp->pNds.GetAt(id)->x;
			    lp->pTrs.GetAt(i)->y[j] = lp->pNds.GetAt(id)->y;
			    lp->pTrs.GetAt(i)->z[j] = lp->pNds.GetAt(id)->z;
		    }
    	}

	    delete [] tx;  delete [] ty;  delete [] tz;  delete [] a;


		/*
	// tmp output
	double x, y, z;
	int i, j, n, ID, n0, n1, n2;
	CString fn;

	if(AfxMessageBox(_T("Saving Polygon Surface ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);

		// allPt
		for(i = 0; i < allPt.GetSize(); i++) {
			x = allPt.GetAt(i)->x;
			y = allPt.GetAt(i)->y;
			z = allPt.GetAt(i)->z;
			fin << i <<"   "<<  x <<"   "<< y <<"   "<< z << endl;
		}
		fin << endl;

		// newPt
		for(i = 0; i < newPt.GetSize(); i++) {
			x = newPt.GetAt(i)->x;
			y = newPt.GetAt(i)->y;
			z = newPt.GetAt(i)->z;
			fin << i <<"   "<<  x <<"   "<< y <<"   "<< z << endl;
		}
		fin << endl;

		// mid points
		for(i = 0; i < mid.numberofpoints; i++) { // Node
			x = mid.pointlist[2*i];
			y = mid.pointlist[2*i+1];  
			z = 0.0;
			fin << i <<"   "<<  x <<"   "<< y <<"   "<< z << endl;
		}
		fin << endl;

		for(i = 0; i < mid.numberoftriangles; i++) { // Mesh
			n0 = mid.trianglelist[3*i];
			n1 = mid.trianglelist[3*i+1];
			n2 = mid.trianglelist[3*i+2];
			fin << i <<"   "<< n0 <<"   "<< n1 <<"   "<< n2 << endl;
		}
		fin.close();
	} 
	*/

	}

	// Free all allocated arrays, including those allocated by Triangle.
	free(in.pointlist);
	free(in.pointattributelist);
	free(in.pointmarkerlist);
	free(in.segmentlist);
	free(in.segmentmarkerlist);
	free(mid.pointlist);
	free(mid.trianglelist);

#else
    // to be continue

#endif

}

void CInsect::DivideEachTrgl_CDT_Robust(CTrgl* tr, CLoop* lp)
{
#ifndef EGC
	int i, j, k, id, nid, eid, nID[3];
	double nx, ny, nz, x, y, z;
	
	// Create all Points and Segments
	CArray<CNode*, CNode*> allPt;
	CArray<CNode*, CNode*> newPt;
	CArray<CPair*, CPair*> allSg;
	
	// First 3 Point of triangle
	id= 0;
	for(i = 0; i < 3; i++) {
		nx = tr->x[i];  ny = tr->y[i];  nz = tr->z[i];
		allPt.Add(new CNode(id++, nx, ny, nz));
	}
	
	// Then, the intersection edges
	bool bexist, bsame;
	for(i = 0; i < tr->iRgnIntrLns.GetSize(); i++) {
		eid = tr->iRgnIntrLns.GetAt(i);
		CEdge * eg = m_aInsectEdge.GetAt(eid);
		
		CPair * pr = new CPair();
		
		// First node
		x = eg->FstNd[0];  y = eg->FstNd[1];  z = eg->FstNd[2];
		
		bexist = false;
		for(j = 0; j < allPt.GetSize(); j++) {
			nx = allPt.GetAt(j)->x;
			ny = allPt.GetAt(j)->y;
			nz = allPt.GetAt(j)->z;
			
			if(IsSameNode(nx, ny, nz, x, y, z, myEps) == true) {
				bexist = true;  nid = j;  break;
			}
		}
		
		if(bexist == true) {
			pr->FstID = nid; // Set ID
		}
		else {
			nid = allPt.GetSize(); // Set ID
			allPt.Add(new CNode(id++, x, y, z)); // Add new node
			pr->FstID = nid;
		}
		
		// Second node
		x = eg->SecNd[0];  y = eg->SecNd[1];  z = eg->SecNd[2];
		
		bexist = false;
		for(j = 0; j < allPt.GetSize(); j++) {
			nx = allPt.GetAt(j)->x;
			ny = allPt.GetAt(j)->y;
			nz = allPt.GetAt(j)->z;
			
			if(IsSameNode(nx, ny, nz, x, y, z, myEps) == true) {
				bexist = true;  nid = j;  break;
			}
		}
		
		if(bexist == true) {
			pr->SedID = nid;
		}
		else {
			nid = allPt.GetSize();
			allPt.Add(new CNode(id++, x, y, z));
			pr->SedID = nid;
		}
		
		// Finally, save segment / constrained edge
		allSg.Add(pr);
		
	}
	
	// 3D to 2D
	// Step 1: compute local coordinates system and transfer
	CCordnt * tCrdt = new CCordnt();
	CEarCut *earCut = new CEarCut();
	earCut->GetNewCoordt(tr, tCrdt);
	
	// change to XY plane
	double * tx, * ty, * tz, * a;
	
	int N = allPt.GetSize();
	
	tx = new double[N];  ty = new double[N];  tz = new double[N];
	
	for(i = 0; i < N; i++) {
		tx[i] = allPt.GetAt(i)->x;
		ty[i] = allPt.GetAt(i)->y;
		tz[i] = allPt.GetAt(i)->z;
	}
	
	// very important
	a = new double[9];
	a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
	a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
	a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;
	earCut->Dcinv(a, 3);
	
	// new points, planar
	for(i = 0; i < N; i++) { // transform
		tx[i] -= tCrdt->u;  ty[i] -= tCrdt->v;  tz[i] -= tCrdt->w;
		newPt.Add(new CNode(i, tx[i], ty[i], tz[i]));
	}
	for(i = 0; i < N; i++) {
		newPt.GetAt(i)->x = tx[i] * a[0] + ty[i] * a[1] + tz[i] * a[2];
		newPt.GetAt(i)->y = tx[i] * a[3] + ty[i] * a[4] + tz[i] * a[5];
		newPt.GetAt(i)->z = tx[i] * a[6] + ty[i] * a[7] + tz[i] * a[8];
	}

	// Avoid float-point problem
	for(i = 0; i < N; i++) {
		if (fabs(newPt.GetAt(i)->x) < myEps) {
			newPt.GetAt(i)->x = 0.0;
		}
		if (fabs(newPt.GetAt(i)->y) < myEps) {
			newPt.GetAt(i)->y = 0.0;
		}
		if (fabs(newPt.GetAt(i)->z) < myEps) {
			newPt.GetAt(i)->z = 0.0;
		}
	}
	
	// Create Boundary edges, very important !
	int id1, id2;
	CNode * pt1, * pt2, * endpoint;
	CArray<int, int> boundaryID;
	boundaryID.Add(0); // first
	boundaryID.Add(1); // second
	boundaryID.Add(2); // third
	boundaryID.Add(0); // first again

	double * dist;
	int startID, endID, nEdge, ownEdge, * edgeIDs;
	CNode * startPoint, * endPoint;
	for(i = 0; i < allSg.GetSize(); i++) {
		startID = allSg.GetAt(i)->FstID;
		endID = allSg.GetAt(i)->SedID;

		startPoint = newPt.GetAt(startID);
		endPoint = newPt.GetAt(endID);

		CNode * p0 = startPoint;
		CNode * d0 = new CNode(0, startPoint->x - endPoint->x, 
			                   startPoint->y - endpoint->y, 0.0);

		// First endpoint
		nEdge = boundaryID.GetSize();
		dist = new double[nEdge];

		for(j = 0; j < boundaryID.GetSize() - 1; j++) {
			id1 = boundaryID.GetAt(j);
			id2 = boundaryID.GetAt(j+1);

			pt1 = newPt.GetAt(id1);
			pt2 = newPt.GetAt(id2);

			CNode * p1 = pt1;
			CNode * d1 = new CNode(0, pt2->x - pt1->x, pt2->y - pt1->y, 0.0);

			CNode * i0 = new CNode();
			CNode * i1 = new CNode();

			int iCase = IntrLine2Segment2D(p0, d0, p1, d1, i0, i1, myEps);

			if( iCase == 1) {
				dist[j] = (startPoint->x - i0->x) * (startPoint->x - i0->x) + 
					      (startPoint->y - i0->y) * (startPoint->y - i0->y);
			}
			else {
				dist[j] = 100; // Set a large dist
			}

			delete i0;  delete i1;
		}
		// Closest intersection point
		edgeIDs = new int[nEdge];
		for(int t = 0; t < nEdge; t++) edgeIDs[t];

		prhap(dist, edgeIDs, nEdge);
		ownEdge = edgeIDs[0];  delete [] dist;

		boundaryID.InsertAt(ownEdge + 1, startID);

		
		// Second endpoint
		nEdge = boundaryID.GetSize();
		dist = new double[nEdge];

		for(j = 0; j < boundaryID.GetSize() - 1; j++) {
			id1 = boundaryID.GetAt(j);
			id2 = boundaryID.GetAt(j+1);

			pt1 = newPt.GetAt(id1);
			pt2 = newPt.GetAt(id2);

			CNode * p1 = pt1;
			CNode * d1 = new CNode(0, pt2->x - pt1->x, pt2->y - pt1->y, 0.0);

			CNode * i0 = new CNode();
			CNode * i1 = new CNode();

			int iCase = IntrLine2Segment2D(p0, d0, p1, d1, i0, i1, myEps);

			if( iCase == 1) {
				dist[j] = (endPoint->x - i0->x) * (endPoint->x - i0->x) + 
					      (endPoint->y - i0->y) * (endPoint->y - i0->y);
			}
			else {
				dist[j] = 100; // Set a large dist
			}

			delete i0;  delete i1;
		}
		// Closest intersection point
		edgeIDs = new int[nEdge];
		for(int t = 0; t < nEdge; t++) edgeIDs[t];

		prhap(dist, edgeIDs, nEdge);
		ownEdge = edgeIDs[0];  delete [] dist;

		boundaryID.InsertAt(ownEdge + 1, endID);
	}

	delete [] tx;  delete [] ty;  delete [] tz;  delete [] a;
	

	// Now, use CDT
	triangulateio in, mid, out, vorout;
	
	// Define input points. 
	in.numberofpoints = newPt.GetSize();
	in.numberofpointattributes = 0;
	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
	for(i = 0; i < in.numberofpoints; i++) {
		in.pointlist[2*i] = newPt.GetAt(i)->x;
		in.pointlist[2*i+1] = newPt.GetAt(i)->y;
	}

	// Define constrained edges + boundary edges
	in.numberofsegments = allSg.GetSize() + (boundaryID.GetSize() - 1);
	in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));
	in.segmentmarkerlist = (int *) malloc(in.numberofsegments * sizeof(int));
	for(i = 0; i < allSg.GetSize(); i++) {
		in.segmentlist[2*i] = allSg.GetAt(i)->FstID;
		in.segmentlist[2*i+1] = allSg.GetAt(i)->SedID;

		in.segmentmarkerlist[i] = 0; // Not boundary edge, interior edge
	}
	for(i = 0; i < (boundaryID.GetSize() - 1); i++) {
		in.segmentlist[ 2*(i+allSg.GetSize()) ] = boundaryID.GetAt(i);
		in.segmentlist[2*(i+allSg.GetSize())+1] = boundaryID.GetAt(i+1);

		in.segmentmarkerlist[i+allSg.GetSize()] = 1;
	}

	in.numberofholes = 0;
	in.numberofregions = 0;
	in.regionlist = (REAL *) NULL;

	in.pointattributelist = (REAL *) NULL;
	in.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
	in.triangleattributelist = (REAL *) NULL;
	in.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */

	/* Make necessary initializations so that Triangle can return a */
	/*   triangulation in `mid' and a voronoi diagram in `vorout'.  */
	mid.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
	/* Not needed if -N switch used or number of point attributes is zero: */
	mid.pointattributelist = (REAL *) NULL;
	mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
	mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
	/* Not needed if -E switch used or number of triangle attributes is zero: */
	mid.triangleattributelist = (REAL *) NULL;
	mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
	/* Needed only if segments are output (-p or -c) and -P not used: */
	mid.segmentlist = (int *) NULL;
	/* Needed only if segments are output (-p or -c) and -P and -B not used: */
	mid.segmentmarkerlist = (int *) NULL;
	mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
	mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */
	
	/* Triangulate the points.  Switches are chosen to read and write a  */
	/*   PSLG (p), preserve the convex hull (c), number everything from  */
	/*   zero (z), assign a regional attribute to each element (A), and  */
	/*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
	/*   neighbor list (n). 
	*/

//	triangulate("pcz", &in, &mid, &vorout);
	triangulate("pz", &in, &mid, &vorout);

	// Very important !  Update and save !
	// Save Node and Trgl, to CLoop* lp
	if (allPt.GetSize() == mid.numberofpoints) {
	    for (i = 0; i < allPt.GetSize(); i++) {
		    lp->pNds.Add(allPt.GetAt(i));
	    }

	    for (i = 0; i < mid.numberoftriangles; i++) { // Trgl
            for (j = 0; j < mid.numberofcorners; j++) {
			    nID[j] = mid.trianglelist[i * mid.numberofcorners + j];
            }
		    lp->pTrs.Add(new CTrgl(i, nID[0], nID[1], nID[2]));
	    }

	    for(i = 0; i < lp->pTrs.GetSize(); i++) { // Must copy, otherwise error
		    for (j = 0; j < 3; j++) {
		    	id = lp->pTrs.GetAt(i)->nID[j];
		    	lp->pTrs.GetAt(i)->x[j] = lp->pNds.GetAt(id)->x;
			    lp->pTrs.GetAt(i)->y[j] = lp->pNds.GetAt(id)->y;
			    lp->pTrs.GetAt(i)->z[j] = lp->pNds.GetAt(id)->z;
		    }
    	}

	}

	else {
	//	AfxMessageBox(_T("Warning:  DivideEachTrgl_CDT \nallPt.GetSize() != mid.numberofpoints"));

		// Change it back!
		N = mid.numberofpoints;
	    tx = new double[N];  ty = new double[N];  tz = new double[N];
	    for(i = 0; i < N; i++) { // save temporily
			tx[i] = mid.pointlist[2*i];
		    ty[i] = mid.pointlist[2*i+1];
		    tz[i] = 0.0;
	    }

        // get the original version of new coordinate, local coordinate system
		a = new double[9];
	    a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
	    a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
	    a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;

		// Empty nodes
		for(i = 0; i < N; i++) {
			lp->pNds.Add(new CNode());
		}

	    for(i = 0; i < N; i++) { // rotate
			lp->pNds.GetAt(i)->x = tx[i] * a[0] + ty[i] * a[1] + tz[i] * a[2];
		    lp->pNds.GetAt(i)->y = tx[i] * a[3] + ty[i] * a[4] + tz[i] * a[5];
		    lp->pNds.GetAt(i)->z = tx[i] * a[6] + ty[i] * a[7] + tz[i] * a[8];
	    }

	    for(i = 0; i < N; i++) { // transform
		    lp->pNds.GetAt(i)->x += tCrdt->u;
		    lp->pNds.GetAt(i)->y += tCrdt->v;
		    lp->pNds.GetAt(i)->z += tCrdt->w;
	    }

		for (i = 0; i < mid.numberoftriangles; i++) { // Trgl
            for (j = 0; j < mid.numberofcorners; j++) {
			    nID[j] = mid.trianglelist[i * mid.numberofcorners + j];
            }
		    lp->pTrs.Add(new CTrgl(i, nID[0], nID[1], nID[2]));
	    }

	    for(i = 0; i < lp->pTrs.GetSize(); i++) { // Must copy, otherwise error
		    for (j = 0; j < 3; j++) {
		    	id = lp->pTrs.GetAt(i)->nID[j];
		    	lp->pTrs.GetAt(i)->x[j] = lp->pNds.GetAt(id)->x;
			    lp->pTrs.GetAt(i)->y[j] = lp->pNds.GetAt(id)->y;
			    lp->pTrs.GetAt(i)->z[j] = lp->pNds.GetAt(id)->z;
		    }
    	}

	    delete [] tx;  delete [] ty;  delete [] tz;  delete [] a;


		/*
	// tmp output
	double x, y, z;
	int i, j, n, ID, n0, n1, n2;
	CString fn;

	if(AfxMessageBox(_T("Saving Polygon Surface ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);

		// allPt
		for(i = 0; i < allPt.GetSize(); i++) {
			x = allPt.GetAt(i)->x;
			y = allPt.GetAt(i)->y;
			z = allPt.GetAt(i)->z;
			fin << i <<"   "<<  x <<"   "<< y <<"   "<< z << endl;
		}
		fin << endl;

		// newPt
		for(i = 0; i < newPt.GetSize(); i++) {
			x = newPt.GetAt(i)->x;
			y = newPt.GetAt(i)->y;
			z = newPt.GetAt(i)->z;
			fin << i <<"   "<<  x <<"   "<< y <<"   "<< z << endl;
		}
		fin << endl;

		// mid points
		for(i = 0; i < mid.numberofpoints; i++) { // Node
			x = mid.pointlist[2*i];
			y = mid.pointlist[2*i+1];  
			z = 0.0;
			fin << i <<"   "<<  x <<"   "<< y <<"   "<< z << endl;
		}
		fin << endl;

		for(i = 0; i < mid.numberoftriangles; i++) { // Mesh
			n0 = mid.trianglelist[3*i];
			n1 = mid.trianglelist[3*i+1];
			n2 = mid.trianglelist[3*i+2];
			fin << i <<"   "<< n0 <<"   "<< n1 <<"   "<< n2 << endl;
		}
		fin.close();
	} 
	*/

	}

	// Free all allocated arrays, including those allocated by Triangle.
	free(in.pointlist);
	free(in.pointattributelist);
	free(in.pointmarkerlist);
	free(in.segmentlist);
	free(in.segmentmarkerlist);
	free(mid.pointlist);
	free(mid.trianglelist);

#else
    // to be continue

#endif

}

//To compute the intersecton of line-segment
//CNode * p0, CNode * d0: Line
//CNode * p1, CNode * d1: Segment
//Return value:
//0-do not intersect;
//1-intersect on a point;
//2-intersect on a interval;
int CInsect::IntrLine2Segment2D(CNode * p0, CNode * d0, CNode * p1, CNode * d1, CNode * i0, CNode * i1, double eps)
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
		if(-1*Epsilon < t && t < 1 + Epsilon) {
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
			i0->x = p1->x ;    i0->y = p1->y ;    i0->z = 0.0;
			i1->x = p1->x + d1->x;    i1->y = p1->y + d1->y;    i1->z = 0.0;
			return 2;
		}
	}
	delete E;
}


void CInsect::Merge(CSurf* surf, CBlock* blk)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();

	int i, j, k, id = 0;
	double x, y, z;
	CArray<CNode*, CNode*> pNds;

	/*
	//emerge all the blocks , and then rearrange and update them
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			x = blk->pTrs.GetAt(i)->x[j];
			y = blk->pTrs.GetAt(i)->y[j];
			z = blk->pTrs.GetAt(i)->z[j];
			pNds.Add(new CNode(id, x, y, z));
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			x = surf->pTrs.GetAt(i)->x[j];
			y = surf->pTrs.GetAt(i)->y[j];
			z = surf->pTrs.GetAt(i)->z[j];
			pNds.Add(new CNode(id, x, y, z));
		}
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		x = m_aInsectEdge.GetAt(i)->FstNd[0];
		y = m_aInsectEdge.GetAt(i)->FstNd[1];
		z = m_aInsectEdge.GetAt(i)->FstNd[2];
	    pNds.Add(new CNode(id, x, y, z));

		x = m_aInsectEdge.GetAt(i)->SecNd[0];
		y = m_aInsectEdge.GetAt(i)->SecNd[1];
		z = m_aInsectEdge.GetAt(i)->SecNd[2];
	    pNds.Add(new CNode(id, x, y, z));
	}
	*/
	//emerge all the blocks , and then rearrange and update them
	for(i = 0; i < blk->pNds.GetSize(); i++) { 
		x = blk->pNds.GetAt(i)->x;
		y = blk->pNds.GetAt(i)->y;
		z = blk->pNds.GetAt(i)->z;
		pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < surf->pNds.GetSize(); i++) { 
		x = surf->pNds.GetAt(i)->x;
		y = surf->pNds.GetAt(i)->y;
		z = surf->pNds.GetAt(i)->z;
		pNds.Add(new CNode(id, x, y, z));
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		x = m_aInsectEdge.GetAt(i)->FstNd[0];
		y = m_aInsectEdge.GetAt(i)->FstNd[1];
		z = m_aInsectEdge.GetAt(i)->FstNd[2];
	    pNds.Add(new CNode(id, x, y, z));

		x = m_aInsectEdge.GetAt(i)->SecNd[0];
		y = m_aInsectEdge.GetAt(i)->SecNd[1];
		z = m_aInsectEdge.GetAt(i)->SecNd[2];
	    pNds.Add(new CNode(id, x, y, z));
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
	for(i = cnt_1 - 1; i >= 0; i--){
		pNds.GetAt(i)->Clear(); 
		delete pNds.GetAt(i); 
		pNds.RemoveAt(i);    
	}
	pNds.RemoveAll();  

	//sort nodes
    pView->SortNode(p, cnt_1);

	/*
	// tmp node
	CString fileName;
	if(AfxMessageBox(_T("确定保存Tmp Node文件?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fileName = Dlg.GetPathName(); 
		if(fileName.Right(4)!=".txt")   fileName += "_Node.txt";
		ofstream fin(fileName);
		for(int i=0;i< cnt_1; i++){
			fin << i <<"   "<<  p[i].x <<"   "<< p[i].y<<"   "<< p[i].z<<endl;
		}
		fin.close(); 
		
	}
	exit(0);
	// end tmp
	*/



	
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
			if(IsSameNode(p[i].x, p[i].y, p[i].z, 
				pNds.GetAt(j)->x, pNds.GetAt(j)->y, pNds.GetAt(j)->z, myEps)){ 
				boolexist = true;  break; 
			}
		}
		if(!boolexist) pNds.Add(new CNode(pNds.GetSize(), p[i].x, p[i].y, p[i].z));
	}

	delete [] p;

	delete[] iID;  delete[] nID;
    
	//最直接、最笨的方法合并节点
	CNode* tmpNode;
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			x = blk->pTrs.GetAt(i)->x[j];
			y = blk->pTrs.GetAt(i)->y[j];
			z = blk->pTrs.GetAt(i)->z[j];
			tmpNode = new CNode(id, x, y, z);
			for(k = 0; k < pNds.GetSize(); k++) {
				if(IsSameNode(tmpNode, pNds.GetAt(k), myEps)) {
					blk->pTrs.GetAt(i)->nID[j] = k;  break;
				}
			}
			delete tmpNode;
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			x = surf->pTrs.GetAt(i)->x[j];
			y = surf->pTrs.GetAt(i)->y[j];
			z = surf->pTrs.GetAt(i)->z[j];
			tmpNode = new CNode(id, x, y, z);
			for(k = 0; k < pNds.GetSize(); k++) {
				if(IsSameNode(tmpNode, pNds.GetAt(k), myEps)) {
					surf->pTrs.GetAt(i)->nID[j] = k;  break;
				}
			}
			delete tmpNode;
		}
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		x = m_aInsectEdge.GetAt(i)->FstNd[0];
		y = m_aInsectEdge.GetAt(i)->FstNd[1];
		z = m_aInsectEdge.GetAt(i)->FstNd[2];
		tmpNode = new CNode(id, x, y, z);
		for(k = 0; k < pNds.GetSize(); k++) {
			if(IsSameNode(tmpNode, pNds.GetAt(k), myEps)) {
				m_aInsectEdge.GetAt(i)->Start = k;  break; // first node
			}
		}
		delete tmpNode;

		x = m_aInsectEdge.GetAt(i)->SecNd[0];
		y = m_aInsectEdge.GetAt(i)->SecNd[1];
		z = m_aInsectEdge.GetAt(i)->SecNd[2];
		tmpNode = new CNode(id, x, y, z);
		for(k = 0; k < pNds.GetSize(); k++) {
			if(IsSameNode(tmpNode, pNds.GetAt(k), myEps)) {
				m_aInsectEdge.GetAt(i)->End = k;  break; // second node
			}
		}
		delete tmpNode;
	}

	// 检测，删除退化
	int NID[3];
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = blk->pTrs.GetAt(i)->nID[j];
		}
		if(NID[0] == NID[1] || NID[1] == NID[2] || NID[2] == NID[0]) {
			blk->pTrs.GetAt(i)->bValid = false;
		}
		else {
			blk->pTrs.GetAt(i)->bValid = true;
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = surf->pTrs.GetAt(i)->nID[j];
		}
		if(NID[0] == NID[1] || NID[1] == NID[2] || NID[2] == NID[0]) {
			surf->pTrs.GetAt(i)->bValid = false;
		}
		else {
			surf->pTrs.GetAt(i)->bValid = true;
		}
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		if(m_aInsectEdge.GetAt(i)->Start == m_aInsectEdge.GetAt(i)->End) {
			m_aInsectEdge.GetAt(i)->bValid = false;
		}
		else {
			m_aInsectEdge.GetAt(i)->bValid = true;
		}
	}

	// 三点共线，退化
	CNode * pt1, * pt2, * pt3;
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = blk->pTrs.GetAt(i)->nID[j];
		}
		pt1 = pNds.GetAt(NID[0]); 
		pt2 = pNds.GetAt(NID[1]); 
		pt3 = pNds.GetAt(NID[2]);

		if (Is3PointCollinear(pt1, pt2, pt3, myEps) == true) {
			blk->pTrs.GetAt(i)->bValid = false;
			AfxMessageBox(_T("Find a trgl Collinear ! \n\nDelete this trgl!"));
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = surf->pTrs.GetAt(i)->nID[j];
		}
		pt1 = pNds.GetAt(NID[0]); 
		pt2 = pNds.GetAt(NID[1]); 
		pt3 = pNds.GetAt(NID[2]);

		if (Is3PointCollinear(pt1, pt2, pt3, myEps) == true) {
			surf->pTrs.GetAt(i)->bValid = false;
			AfxMessageBox(_T("Find a trgl Collinear ! \n\nDelete this trgl!"));
		}
	}


	// 删除
	for(i = blk->pTrs.GetUpperBound(); i > -1; i--) {
		if(blk->pTrs.GetAt(i)->bValid == false) {
			delete blk->pTrs.GetAt(i);  blk->pTrs.RemoveAt(i);
		}
	}
	for(i = surf->pTrs.GetUpperBound(); i > -1; i--) {
		if(surf->pTrs.GetAt(i)->bValid == false) {
			delete surf->pTrs.GetAt(i);  surf->pTrs.RemoveAt(i);
		}
	}
	for(i = m_aInsectEdge.GetUpperBound(); i > -1; i--) {
		if(m_aInsectEdge.GetAt(i)->bValid == false) {
			delete m_aInsectEdge.GetAt(i);  m_aInsectEdge.RemoveAt(i);
		}
	}

	//检测，删除重复
	for(i = 0; i < surf->pTrs.GetSize() - 1; i++) {
		CTrgl* ta = surf->pTrs.GetAt(i);
		for(j = i+1; j < surf->pTrs.GetSize(); j++) {
			CTrgl* tb = surf->pTrs.GetAt(j);
			if(IsSameTrgl(ta, tb) == true) {
				tb->bValid = false; // Only change Tb
				break;
			}
		}
	}
	for(i = 0; i < blk->pTrs.GetSize() - 1; i++) {
		CTrgl* ta = blk->pTrs.GetAt(i);
		for(j = i+1; j < blk->pTrs.GetSize(); j++) {
			CTrgl* tb = blk->pTrs.GetAt(j);
			if(IsSameTrgl(ta, tb) == true) {
				tb->bValid = false; // Only change Tb
				break;
			}
		}
	}
	for(i = blk->pTrs.GetUpperBound(); i > -1; i--) {
		if(blk->pTrs.GetAt(i)->bValid == false) {
			delete blk->pTrs.GetAt(i);  blk->pTrs.RemoveAt(i);
		}
	}
	for(i = surf->pTrs.GetUpperBound(); i > -1; i--) {
		if(surf->pTrs.GetAt(i)->bValid == false) {
			delete surf->pTrs.GetAt(i);  surf->pTrs.RemoveAt(i);
		}
	}
	
	// Save
	for(i = 0; i < pNds.GetSize(); i++) {
		m_aUpdateNode.Add(pNds.GetAt(i));
	}
 
}

void CInsect::Merge_Fast(CSurf* surf, CBlock* blk)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();

	int i, j, k, id = 0;
	double x, y, z;
	CArray<CNode*, CNode*> pNds;

	// merge all the blocks , and then rearrange and update them
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			x = blk->pTrs.GetAt(i)->x[j];
			y = blk->pTrs.GetAt(i)->y[j];
			z = blk->pTrs.GetAt(i)->z[j];
			blk->pTrs.GetAt(i)->nID[j] = id;
			pNds.Add(new CNode(id++, x, y, z));
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			x = surf->pTrs.GetAt(i)->x[j];
			y = surf->pTrs.GetAt(i)->y[j];
			z = surf->pTrs.GetAt(i)->z[j];
			surf->pTrs.GetAt(i)->nID[j] = id;
			pNds.Add(new CNode(id++, x, y, z));
		}
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		x = m_aInsectEdge.GetAt(i)->FstNd[0];
		y = m_aInsectEdge.GetAt(i)->FstNd[1];
		z = m_aInsectEdge.GetAt(i)->FstNd[2];
		m_aInsectEdge.GetAt(i)->Start = id;
	    pNds.Add(new CNode(id++, x, y, z));

		x = m_aInsectEdge.GetAt(i)->SecNd[0];
		y = m_aInsectEdge.GetAt(i)->SecNd[1];
		z = m_aInsectEdge.GetAt(i)->SecNd[2];
		m_aInsectEdge.GetAt(i)->End = id;
	    pNds.Add(new CNode(id++, x, y, z));
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
    pView->SortNode(p, cnt_1);

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
	i = 0;
	x = p[i].x;  y = p[i].y;  z = p[i].z;
	m_aUpdateNode.Add(new CNode(i, x, y, z));

	for(i = 1; i < cnt_1; i++) {
		if(newIDs[i] != newIDs[i-1]) {
			x = p[i].x;  y = p[i].y;  z = p[i].z;
	        m_aUpdateNode.Add(new CNode(i, x, y, z));
		}
	}

	/*
	// tmp node
	CString fileName;
	if(AfxMessageBox(_T("确定保存Tmp Node文件?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fileName = Dlg.GetPathName(); 
		if(fileName.Right(4)!=".txt")   fileName += "_Node.txt";
		ofstream fin(fileName);
		for(int i=0;i< cnt_1; i++){
			id = newIDs[i];

			id = p[i].ID;
			fin << id <<"   "<<  p[i].x <<"   "<< p[i].y<<"   "<< p[i].z<<endl;
		}
		fin.close(); 
	}
	exit(0);
	// end tmp
	*/
	
	// Update : 2nd step
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			id = blk->pTrs.GetAt(i)->nID[j];
			blk->pTrs.GetAt(i)->nID[j] = pNds.GetAt(id)->ID;
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			surf->pTrs.GetAt(i)->nID[j] = pNds.GetAt(id)->ID;
		}
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		id = m_aInsectEdge.GetAt(i)->Start;
		m_aInsectEdge.GetAt(i)->Start = pNds.GetAt(id)->ID;

		id = m_aInsectEdge.GetAt(i)->End;
		m_aInsectEdge.GetAt(i)->End = pNds.GetAt(id)->ID;
	}

	// 检测，删除退化
	int NID[3];
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = blk->pTrs.GetAt(i)->nID[j];
		}
		if(NID[0] == NID[1] || NID[1] == NID[2] || NID[2] == NID[0]) {
			blk->pTrs.GetAt(i)->bValid = false;
		}
		else {
			blk->pTrs.GetAt(i)->bValid = true;
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = surf->pTrs.GetAt(i)->nID[j];
		}
		if(NID[0] == NID[1] || NID[1] == NID[2] || NID[2] == NID[0]) {
			surf->pTrs.GetAt(i)->bValid = false;
		}
		else {
			surf->pTrs.GetAt(i)->bValid = true;
		}
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		if(m_aInsectEdge.GetAt(i)->Start == m_aInsectEdge.GetAt(i)->End) {
			m_aInsectEdge.GetAt(i)->bValid = false;
		}
		else {
			m_aInsectEdge.GetAt(i)->bValid = true;
		}
	}

	/*
	// 三点共线，退化
	CNode * pt1, * pt2, * pt3;
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = blk->pTrs.GetAt(i)->nID[j];
		}
		pt1 = m_aUpdateNode.GetAt(NID[0]); 
		pt2 = m_aUpdateNode.GetAt(NID[1]); 
		pt3 = m_aUpdateNode.GetAt(NID[2]);

		if (Is3PointCollinear(pt1, pt2, pt3, myEps) == true) {
			blk->pTrs.GetAt(i)->bValid = false;
			AfxMessageBox(_T("Find a trgl Collinear ! \n\nDelete this trgl!"));
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = surf->pTrs.GetAt(i)->nID[j];
		}
		pt1 = m_aUpdateNode.GetAt(NID[0]); 
		pt2 = m_aUpdateNode.GetAt(NID[1]); 
		pt3 = m_aUpdateNode.GetAt(NID[2]);

		if (Is3PointCollinear(pt1, pt2, pt3, myEps) == true) {
			surf->pTrs.GetAt(i)->bValid = false;
			AfxMessageBox(_T("Find a trgl Collinear ! \n\nDelete this trgl!"));
		}
	}
	*/

	// 删除
	for(i = blk->pTrs.GetUpperBound(); i > -1; i--) {
		if(blk->pTrs.GetAt(i)->bValid == false) {
			delete blk->pTrs.GetAt(i);  blk->pTrs.RemoveAt(i);
		}
	}
	for(i = surf->pTrs.GetUpperBound(); i > -1; i--) {
		if(surf->pTrs.GetAt(i)->bValid == false) {
			delete surf->pTrs.GetAt(i);  surf->pTrs.RemoveAt(i);
		}
	}
	for(i = m_aInsectEdge.GetUpperBound(); i > -1; i--) {
		if(m_aInsectEdge.GetAt(i)->bValid == false) {
			delete m_aInsectEdge.GetAt(i);  m_aInsectEdge.RemoveAt(i);
		}
	}

	//检测，删除重复
	for(i = 0; i < surf->pTrs.GetSize() - 1; i++) {
		CTrgl* ta = surf->pTrs.GetAt(i);
		for(j = i+1; j < surf->pTrs.GetSize(); j++) {
			CTrgl* tb = surf->pTrs.GetAt(j);
			if(IsSameTrgl(ta, tb) == true) {
				tb->bValid = false; // Only change Tb
				break;
			}
		}
	}
	for(i = 0; i < blk->pTrs.GetSize() - 1; i++) {
		CTrgl* ta = blk->pTrs.GetAt(i);
		for(j = i+1; j < blk->pTrs.GetSize(); j++) {
			CTrgl* tb = blk->pTrs.GetAt(j);
			if(IsSameTrgl(ta, tb) == true) {
				tb->bValid = false; // Only change Tb
				break;
			}
		}
	}
	for(i = blk->pTrs.GetUpperBound(); i > -1; i--) {
		if(blk->pTrs.GetAt(i)->bValid == false) {
			delete blk->pTrs.GetAt(i);  blk->pTrs.RemoveAt(i);
		}
	}
	for(i = surf->pTrs.GetUpperBound(); i > -1; i--) {
		if(surf->pTrs.GetAt(i)->bValid == false) {
			delete surf->pTrs.GetAt(i);  surf->pTrs.RemoveAt(i);
		}
	}
 
	delete [] p;
}

void CInsect::Merge_FirstTime(CSurf* surf, CBlock* blk)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();

	int i, j, k, id = 0, Add;
	double x, y, z;
	CArray<CNode*, CNode*> pNds;

	// merge all the blocks , and then rearrange and update them
	for(i = 0; i < blk->pNbs.GetSize(); i++) {
		x = blk->pNds.GetAt(i)->x;
		y = blk->pNds.GetAt(i)->y;
		z = blk->pNds.GetAt(i)->z;
		pNds.Add(new CNode(i, x, y, z));
	}
	
	Add = blk->pNds.GetSize();

	for(i = 0; i < surf->pNds.GetSize(); i++) {
		x = surf->pNds.GetAt(i)->x;
		y = surf->pNds.GetAt(i)->y;
		z = surf->pNds.GetAt(i)->z;
		pNds.Add(new CNode(i+Add, x, y, z));
	}

	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			surf->pTrs.GetAt(i)->nID[j] += Add;
		}
	}

	id = pNds.GetSize();

	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		x = m_aInsectEdge.GetAt(i)->FstNd[0];
		y = m_aInsectEdge.GetAt(i)->FstNd[1];
		z = m_aInsectEdge.GetAt(i)->FstNd[2];
		m_aInsectEdge.GetAt(i)->Start = id;
	    pNds.Add(new CNode(id++, x, y, z));

		x = m_aInsectEdge.GetAt(i)->SecNd[0];
		y = m_aInsectEdge.GetAt(i)->SecNd[1];
		z = m_aInsectEdge.GetAt(i)->SecNd[2];
		m_aInsectEdge.GetAt(i)->End = id;
	    pNds.Add(new CNode(id++, x, y, z));
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
    pView->SortNode(p, cnt_1);

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
	i = 0;
	x = p[i].x;  y = p[i].y;  z = p[i].z;
	m_aUpdateNode.Add(new CNode(i, x, y, z));

	for(i = 1; i < cnt_1; i++) {
		if(newIDs[i] != newIDs[i-1]) {
			x = p[i].x;  y = p[i].y;  z = p[i].z;
	        m_aUpdateNode.Add(new CNode(i, x, y, z));
		}
	}

	// Update : 2nd step
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			id = blk->pTrs.GetAt(i)->nID[j];
			blk->pTrs.GetAt(i)->nID[j] = pNds.GetAt(id)->ID;
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			id = surf->pTrs.GetAt(i)->nID[j];
			surf->pTrs.GetAt(i)->nID[j] = pNds.GetAt(id)->ID;
		}
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		id = m_aInsectEdge.GetAt(i)->Start;
		m_aInsectEdge.GetAt(i)->Start = pNds.GetAt(id)->ID;

		id = m_aInsectEdge.GetAt(i)->End;
		m_aInsectEdge.GetAt(i)->End = pNds.GetAt(id)->ID;
	}

	// 检测，删除退化
	int NID[3];
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = blk->pTrs.GetAt(i)->nID[j];
		}
		if(NID[0] == NID[1] || NID[1] == NID[2] || NID[2] == NID[0]) {
			blk->pTrs.GetAt(i)->bValid = false;
		}
		else {
			blk->pTrs.GetAt(i)->bValid = true;
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = surf->pTrs.GetAt(i)->nID[j];
		}
		if(NID[0] == NID[1] || NID[1] == NID[2] || NID[2] == NID[0]) {
			surf->pTrs.GetAt(i)->bValid = false;
		}
		else {
			surf->pTrs.GetAt(i)->bValid = true;
		}
	}
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		if(m_aInsectEdge.GetAt(i)->Start == m_aInsectEdge.GetAt(i)->End) {
			m_aInsectEdge.GetAt(i)->bValid = false;
		}
		else {
			m_aInsectEdge.GetAt(i)->bValid = true;
		}
	}

	/*
	// 三点共线，退化
	CNode * pt1, * pt2, * pt3;
	for(i = 0; i < blk->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = blk->pTrs.GetAt(i)->nID[j];
		}
		pt1 = m_aUpdateNode.GetAt(NID[0]); 
		pt2 = m_aUpdateNode.GetAt(NID[1]); 
		pt3 = m_aUpdateNode.GetAt(NID[2]);

		if (Is3PointCollinear(pt1, pt2, pt3, myEps) == true) {
			blk->pTrs.GetAt(i)->bValid = false;
			AfxMessageBox(_T("Find a trgl Collinear ! \n\nDelete this trgl!"));
		}
	}
	for(i = 0; i < surf->pTrs.GetSize(); i++) { 
		for(j = 0; j < 3; j++) {
			NID[j] = surf->pTrs.GetAt(i)->nID[j];
		}
		pt1 = m_aUpdateNode.GetAt(NID[0]); 
		pt2 = m_aUpdateNode.GetAt(NID[1]); 
		pt3 = m_aUpdateNode.GetAt(NID[2]);

		if (Is3PointCollinear(pt1, pt2, pt3, myEps) == true) {
			surf->pTrs.GetAt(i)->bValid = false;
			AfxMessageBox(_T("Find a trgl Collinear ! \n\nDelete this trgl!"));
		}
	}
	*/

	// 删除
	for(i = blk->pTrs.GetUpperBound(); i > -1; i--) {
		if(blk->pTrs.GetAt(i)->bValid == false) {
			delete blk->pTrs.GetAt(i);  blk->pTrs.RemoveAt(i);
		}
	}
	for(i = surf->pTrs.GetUpperBound(); i > -1; i--) {
		if(surf->pTrs.GetAt(i)->bValid == false) {
			delete surf->pTrs.GetAt(i);  surf->pTrs.RemoveAt(i);
		}
	}
	for(i = m_aInsectEdge.GetUpperBound(); i > -1; i--) {
		if(m_aInsectEdge.GetAt(i)->bValid == false) {
			delete m_aInsectEdge.GetAt(i);  m_aInsectEdge.RemoveAt(i);
		}
	}

	//检测，删除重复
	for(i = 0; i < surf->pTrs.GetSize() - 1; i++) {
		CTrgl* ta = surf->pTrs.GetAt(i);
		for(j = i+1; j < surf->pTrs.GetSize(); j++) {
			CTrgl* tb = surf->pTrs.GetAt(j);
			if(IsSameTrgl(ta, tb) == true) {
				tb->bValid = false; // Only change Tb
				break;
			}
		}
	}
	for(i = 0; i < blk->pTrs.GetSize() - 1; i++) {
		CTrgl* ta = blk->pTrs.GetAt(i);
		for(j = i+1; j < blk->pTrs.GetSize(); j++) {
			CTrgl* tb = blk->pTrs.GetAt(j);
			if(IsSameTrgl(ta, tb) == true) {
				tb->bValid = false; // Only change Tb
				break;
			}
		}
	}
	for(i = blk->pTrs.GetUpperBound(); i > -1; i--) {
		if(blk->pTrs.GetAt(i)->bValid == false) {
			delete blk->pTrs.GetAt(i);  blk->pTrs.RemoveAt(i);
		}
	}
	for(i = surf->pTrs.GetUpperBound(); i > -1; i--) {
		if(surf->pTrs.GetAt(i)->bValid == false) {
			delete surf->pTrs.GetAt(i);  surf->pTrs.RemoveAt(i);
		}
	}
 
	delete [] p;
}

//Node数的堆排序  according to axis x
void CInsect::SortNode(int n, double * px, double * py, double * pz, int * IDs)
{
	int i, j, k, l, mx, my, mz;
	double* x = new double[n];
	int* c = new int[n];

	//copy the data
	for(i = 0; i < n; i++) {
		x[i] = px[i];  c[i] = IDs[i];
	}

	//sort according to x
	prhap(x, c, n);

	i = 0;
	while(i < n){
		qrbsh(x, n, x[i], x[i], &mx);

		double* y = new double[mx];
		for(l = 0; l < mx; l++) y[l] = py[c[i + l]]; 

		//sort y with same x
		prhap(y, c + i, mx);

		j = 0;
		while(j < mx){
			qrbsh(y, mx, y[j], y[j], &my);
		    double* z = new double[my];
            for(l = 0; l < my; l++) z[l] = pz[c[i + j + l]];

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
	for(i = 0; i < n; i++) IDs[i] = c[i];  
	for(i = 0; i < n; i++) x[i] = px[i];  
	for(i = 0; i < n; i++) px[i] = x[c[i]];  
	for(i = 0; i < n; i++) x[i] = py[i];  
	for(i = 0; i < n; i++) py[i] = x[c[i]];  
	for(i = 0; i < n; i++) x[i] = pz[i];  
	for(i = 0; i < n; i++) pz[i] = x[c[i]]; 

	delete[] x;  delete[] c;

}

bool CInsect::IsSameTrgl(CTrgl* ta, CTrgl*tb)
{
	if( (ta->nID[0] != tb->nID[0]) && 
		(ta->nID[0] != tb->nID[1]) && 
		(ta->nID[0] != tb->nID[2])) {
			return false;
	}
	if( (ta->nID[1] != tb->nID[0]) && 
		(ta->nID[1] != tb->nID[1]) && 
		(ta->nID[1] != tb->nID[2])) {
			return false;
	}
	if( (ta->nID[2] != tb->nID[0]) && 
		(ta->nID[2] != tb->nID[1]) && 
		(ta->nID[2] != tb->nID[2])) {
			return false;
	}

	return true;
}

// Divide a closed loop by a open loop into 2 or 1 closed sub-loops
// 1: one sub-block;   2: two sub-blocks
int CInsect::CreateSubLoop(CLoop* pOrig, CLoop* pEdge, CLoop* pSonA, CLoop* pSonB)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
    CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();

	int i, j, t;
	int position[2] = {-1, -1};
	
	CNode* pHead, *pTail, *tNode;
	int N = pOrig->pNds.GetSize();
	pOrig->pNds.Add(pOrig->pNds.GetAt(0)); // add the first one again
	
	tNode = pEdge->pNds.GetAt(0);
	for(i = 0; i < N; i++) {
		pHead = pOrig->pNds.GetAt(i);
		pTail = pOrig->pNds.GetAt(i+1);
		int nCase = IsPointOnSegment3D(pHead, pTail, tNode, myEps);
		if(nCase > 0) {
			position[0] = i;  break;
		}
	}
	tNode = pEdge->pNds.GetAt(pEdge->pNds.GetUpperBound());
	for(i = 0; i < N; i++) {
		pHead = pOrig->pNds.GetAt(i);
		pTail = pOrig->pNds.GetAt(i+1);
		int nCase = IsPointOnSegment3D(pHead, pTail, tNode, myEps);
		if(nCase > 0) {
			position[1] = i;  break;
		}
	}
	
	// form 2 sub-loops
	if(position[0] == -1 && position[1] == -1) {
		AfxMessageBox(_T("Error: CInsect::CreateSubLoop() \n\nposition[0] == -1 && position[1] == -1"));
		return 0;
		
	}
	if(position[0] == -1 || position[1] == -1) {
		AfxMessageBox(_T("Error: CInsect::CreateSubLoop() \n\nposition[0] == -1 || position[1] == -1"));
		// SaveLoop(pOrig, pEdge);
		return 0;
	}
	if(position[0] < position[1] ) {
		//pSonA
		for(i = 0; i < position[0] + 1; i++) { // +1
			pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		}
		for(i = 0; i < pEdge->pNds.GetSize(); i++) {
			pSonA->pNds.Add(pEdge->pNds.GetAt(i));
		}
		for(i = position[1] + 1; i < pOrig->pNds.GetSize(); i++) { // +1
			pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		}

		//pSonB
		for(i = position[0] + 1; i < position[1] + 1; i++) { //+1
			pSonB->pNds.Add(pOrig->pNds.GetAt(i));
		}
		for(i = pEdge->pNds.GetUpperBound(); i > -1; i--) {
			pSonB->pNds.Add(pEdge->pNds.GetAt(i));
		}
	}
	else if(position[0] > position[1]) {
		//pSonA
		for(i = 0; i < position[1] + 1; i++) {
			pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		}
		for(i = pEdge->pNds.GetUpperBound(); i > -1; i--) {
			pSonA->pNds.Add(pEdge->pNds.GetAt(i));
		}
		for(i = position[0] + 1; i < pOrig->pNds.GetSize(); i++) {
			pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		}

		//pSonB
		for(i = position[1] +1; i < position[0]+1; i++) {
			pSonB->pNds.Add(pOrig->pNds.GetAt(i));
		}
		for(i = 0; i < pEdge->pNds.GetSize(); i++) {
			pSonB->pNds.Add(pEdge->pNds.GetAt(i));
		}

	}
	else if( position[0] == position[1] ){ // 有问题！
		// judge distance
		double dist[2];
		N = pEdge->pNds.GetSize();
		pHead = pEdge->pNds.GetAt(0);
		pTail = pEdge->pNds.GetAt(N-1);
		CNode* tNode = pOrig->pNds.GetAt(position[0]);

		dist[0] = pow(pHead->x - tNode->x, 2) +
			      pow(pHead->y - tNode->y, 2) +
				  pow(pHead->z - tNode->z, 2);
		dist[1] = pow(pTail->x - tNode->x, 2) +
			      pow(pTail->y - tNode->y, 2) +
				  pow(pTail->z - tNode->z, 2);
		dist[0] = sqrt(dist[0]);  dist[1] = sqrt(dist[1]);

		if(dist[0] + myEps < dist[1]) { // position[0] > position[1]
		    //pSonA
		    for(i = 0; i < position[0] + 1; i++) { // +1
			    pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		    }
		    for(i = 0; i < pEdge->pNds.GetSize(); i++) {
			    pSonA->pNds.Add(pEdge->pNds.GetAt(i));
		    }
		    for(i = position[1] + 1; i < pOrig->pNds.GetSize(); i++) { // +1
			    pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		    }

		    //pSonB
		    for(i = pEdge->pNds.GetUpperBound(); i > -1; i--) {
			    pSonB->pNds.Add(pEdge->pNds.GetAt(i));
		    }

		}
		else if(dist[1] + myEps < dist[0]) { // position[0] < position[1]
			//pSonA
		    for(i = 0; i < position[0] + 1; i++) { // +1
			    pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		    }
			for(i = pEdge->pNds.GetUpperBound(); i > -1; i--) {
			    pSonA->pNds.Add(pEdge->pNds.GetAt(i));
		    }
		    for(i = position[1] + 1; i < pOrig->pNds.GetSize(); i++) { // +1
			    pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		    }

		    //pSonB
			for(i = 0; i < pEdge->pNds.GetSize(); i++) {
			    pSonB->pNds.Add(pEdge->pNds.GetAt(i));
		    }
		}
		else {
			// AfxMessageBox(_T("Error: CInsect::CreateSubLoop()"));
			return 0;
		}

		/*
		// tmp
		SaveSubLoop(pSonA);
		SaveSubLoop(pSonB);
		*/

	}

	// Need to check
	CNode* na, * nb;

	N = pSonA->pNds.GetSize();
	na = pSonA->pNds.GetAt(0); // loop a
	nb = pSonA->pNds.GetAt(N-1);
	if(fabs(na->x - nb->x) < myEps && 
	   fabs(na->y - nb->y) < myEps && 
	   fabs(na->z - nb->z) < myEps) {
		   pSonA->pNds.RemoveAt(N-1);
	}

	N = pSonB->pNds.GetSize();
	na = pSonB->pNds.GetAt(0); // loop b
	nb = pSonB->pNds.GetAt(N-1);
	if(fabs(na->x - nb->x) < myEps && 
	   fabs(na->y - nb->y) < myEps && 
	   fabs(na->z - nb->z) < myEps) {
		   pSonB->pNds.RemoveAt(N-1);
	}
	
	/*
	// Check CCW
	CLoop* tmp = new CLoop();
	if(pView->IsLoopCCW(pSonA) == false) {
		for(i = 0; i < pSonA->pNds.GetSize(); i++) {
			tmp->pNds.Add(pSonA->pNds.GetAt(i));
		}
		pSonA->pNds.RemoveAll();
		for(i = tmp->pNds.GetUpperBound(); i > -1; i--) {
			pSonA->pNds.Add(tmp->pNds.GetAt(i));
		}
		tmp->pNds.RemoveAll();
	}
	if(pView->IsLoopCCW(pSonB) == false) {
		for(i = 0; i < pSonB->pNds.GetSize(); i++) {
			tmp->pNds.Add(pSonB->pNds.GetAt(i));
		}
		pSonB->pNds.RemoveAll();
		for(i = tmp->pNds.GetUpperBound(); i > -1; i--) {
			pSonB->pNds.Add(tmp->pNds.GetAt(i));
		}
		tmp->pNds.RemoveAll();
	}
	*/
	
	return 2;
	
}

// New based on CreateSubLoop() 2012.8.3
// Divide a closed loop by a open loop into 2 or 1 closed sub-loops
// 1: one sub-block;   2: two sub-blocks
// New: consider an edge chain inside prev sub-polygon
int CInsect::CreateSubLoop_Hole(CLoop* pOrig, CLoop* pEdge, CLoop* pSonA, CLoop* pSonB)
{
	int i, j, t;
	int position[2] = {-1, -1};
	
	CNode* pHead, *pTail, *tNode;
	int N = pOrig->pNds.GetSize();
	pOrig->pNds.Add(pOrig->pNds.GetAt(0)); // add the first one again
	
	tNode = pEdge->pNds.GetAt(0);
	for(i = 0; i < N; i++) {
		pHead = pOrig->pNds.GetAt(i);
		pTail = pOrig->pNds.GetAt(i+1);
		int nCase = IsPointOnSegment3D(pHead, pTail, tNode, myEps);
		if(nCase > 0) {
			position[0] = i;  break;
		}
	}
	tNode = pEdge->pNds.GetAt(pEdge->pNds.GetUpperBound());
	for(i = 0; i < N; i++) {
		pHead = pOrig->pNds.GetAt(i);
		pTail = pOrig->pNds.GetAt(i+1);
		int nCase = IsPointOnSegment3D(pHead, pTail, tNode, myEps);
		if(nCase > 0) {
			position[1] = i;  break;
		}
	}
	
	// Special case: edge chain pEdge inside pOrig
	// Two sub-polygon: the Hole and polygon cutted by the Hole
	// Whethe a closed edge chain
	int C = pEdge->pNds.GetUpperBound();
	pHead = pEdge->pNds.GetAt(0);
	pTail = pEdge->pNds.GetAt(C);

	if( IsSameNode(pHead, pTail, myEps) == true && // closed edge chain
		position[0] == -1 && position[1] == -1) {
		
		// in further, determine whether inside
		// firstly, transfer 3D to 2D
		
		// Step 1: compute local coordinates system and transfer
		
		// change pIntrTmp and pTrgl to XY plane
		int i, j, n, nID[3];
		double * x, * y, * z, * a, avgx = 0.0, avgy = 0.0, avgz = 0.0;
        CCordnt * tCrdt = new CCordnt();
		CTrgl * pTrgl = new CTrgl();
		
		CEarCut *earCut = new CEarCut();
		
		CLoop * lp = pOrig;
		
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
		earCut->GetNewCoordt(pTrgl, tCrdt);
		
		
		// very important
		a = new double[9];
		a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
		a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
		a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;
        earCut->Dcinv(a, 3);
		
		CLoop * newOrig = new CLoop();
		for(i = 0; i < n; i++) { // transform
			x[i] -= tCrdt->u;  y[i] -= tCrdt->v;  z[i] -= tCrdt->w;
			newOrig->pNds.Add(new CNode(i, x[i], y[i], z[i]));
		}
		for(i = 0; i < n; i++) {
			newOrig->pNds.GetAt(i)->x = x[i] * a[0] + y[i] * a[1] + z[i] * a[2];
			newOrig->pNds.GetAt(i)->y = x[i] * a[3] + y[i] * a[4] + z[i] * a[5];
			newOrig->pNds.GetAt(i)->z = x[i] * a[6] + y[i] * a[7] + z[i] * a[8];
		}
		delete [] x;  delete [] y;  delete [] z;
		
		
		CLoop * newPoly = new CLoop();
		n = pEdge->pNds.GetSize();
		x = new double[n];  y = new double[n];  z = new double[n];
		for(i = 0; i < n; i++) { // transform
			x[i] = pEdge->pNds.GetAt(i)->x;
			y[i] = pEdge->pNds.GetAt(i)->y;
			z[i] = pEdge->pNds.GetAt(i)->z;
			
			x[i] -= tCrdt->u;  y[i] -= tCrdt->v;  z[i] -= tCrdt->w;
			newPoly->pNds.Add(new CNode(i, x[i], y[i], z[i]));
		}
		
		for(i = 0; i < n; i++) {
			newPoly->pNds.GetAt(i)->x = x[i] * a[0] + y[i] * a[1] + z[i] * a[2];
			newPoly->pNds.GetAt(i)->y = x[i] * a[3] + y[i] * a[4] + z[i] * a[5];
			newPoly->pNds.GetAt(i)->z = x[i] * a[6] + y[i] * a[7] + z[i] * a[8];
		}
		delete [] x;  delete [] y;  delete [] z;  delete [] a;
		
		// Check whethe inside
		n = newOrig->pNds.GetSize();
		double * px, * py, * pz;
		px = new double[n];  py = new double[n];  pz = new double[n];
		for(int j = 0; j < n; j++) {
			px[j] = newOrig->pNds.GetAt(j)->x;
			py[j] = newOrig->pNds.GetAt(j)->y;
			pz[j] = 0.0;
		}
		
		bool bInside = true;
		double nx, ny, nz;
		n = newPoly->pNds.GetSize();
		for(int j = 0; j < n; j++) {
			nx = newPoly->pNds.GetAt(j)->x;
			ny = newPoly->pNds.GetAt(j)->y;
			bInside = IsPointInPolygon(n, px, py, nx, ny);

			if(bInside == false)  return 0; // Not in this sub-polygon, may another

		}
		
		// Assuming the hole is inside this sub-polygon
		// Go on computing
		
		// Step 2 : Check
		// create sub-polygons
		n = newPoly->pNds.GetSize();
		CNode * p = new CNode[newPoly->pNds.GetSize()];
		for(i = 0; i < newPoly->pNds.GetSize(); i++) { // Prepare
			p[i].x = newPoly->pNds.GetAt(i)->x;
			p[i].y = newPoly->pNds.GetAt(i)->y;
		}
		if(earCut->IsCCwize(p, newPoly->pNds.GetSize()) == false) { // when it is clock wise
			// Convert Polygon with Hole
			earCut->ConvertPolygonHole(newOrig, newPoly);
			
			double nx, ny, nz;
			
			// Degerent Polygon --> pSonA
			n = newOrig->pNds.GetSize();
			for(int j = 0; j < n; j++) {
				nx = newOrig->pNds.GetAt(j)->x;
				ny = newOrig->pNds.GetAt(j)->y;
				nz = 0.0;
				pSonA->pNds.Add(new CNode(j, nx, ny, nz));
			}
			
			// Hole -> pSonB
			n = newPoly->pNds.GetSize();
			for(int j = 0; j < n; j++) {
				nx = p[n-1-j].x; // Notice
				ny = p[n-1-j].y;
				nz = 0.0;
				pSonB->pNds.Add(new CNode(j, nx, ny, nz));
			}
			
		}
		else { // Create sub-polygon when Couunt-Clock-wise
			
			// firstly, make the hole be opposite
			n = newPoly->pNds.GetSize();
			for(int j = 0; j < n; j++) {
				newPoly->pNds.GetAt(j)->x = p[n-1-j].x; // Notice
				newPoly->pNds.GetAt(j)->y = p[n-1-j].y;
				newPoly->pNds.GetAt(j)->z = 0.0;
			}
			
			// Convert Polygon with 
			earCut->ConvertPolygonHole(newOrig, newPoly);
			
			double nx, ny, nz;
			
			// Degerent Polygon --> pSonA
			n = newOrig->pNds.GetSize();
			for(int j = 0; j < n; j++) {
				nx = newOrig->pNds.GetAt(j)->x;
				ny = newOrig->pNds.GetAt(j)->y;
				nz = 0.0;
				pSonA->pNds.Add(new CNode(j, nx, ny, nz));
			}
			
			// Hole -> pSonB
			n = newPoly->pNds.GetSize();
			for(int j = 0; j < n; j++) {
				nx = p[j].x; // Notice
				ny = p[j].y;
				nz = 0.0;
				pSonB->pNds.Add(new CNode(j, nx, ny, nz));
			}
		 }
		
			// change it back!
	    n = pSonA->pNds.GetSize();
	    x = new double[n];  y = new double[n];  z = new double[n];
	    for(i = 0; i < n; i++) { // save temporily
		    x[i] = pSonA->pNds.GetAt(i)->x;
	    	y[i] = pSonA->pNds.GetAt(i)->y;
		    z[i] = pSonA->pNds.GetAt(i)->z;
	    }
        // get the original version of new coordinate, local coordinate system
	    a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
	    a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
	    a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;

	    for(i = 0; i < n; i++) { // rotate
	    	pSonA->pNds.GetAt(i)->x = x[i] * a[0] + y[i] * a[1] + z[i] * a[2];
		    pSonA->pNds.GetAt(i)->y = x[i] * a[3] + y[i] * a[4] + z[i] * a[5];
	    	pSonA->pNds.GetAt(i)->z = x[i] * a[6] + y[i] * a[7] + z[i] * a[8];
	    }
	    for(i = 0; i < n; i++) { // transform
	    	pSonA->pNds.GetAt(i)->x += tCrdt->u;
	    	pSonA->pNds.GetAt(i)->y += tCrdt->v;
	    	pSonA->pNds.GetAt(i)->z += tCrdt->w;
	    }
	    delete [] x;  delete [] y;  delete [] z;

	    n = pSonB->pNds.GetSize();
	    x = new double[n];  y = new double[n];  z = new double[n];
	    for(i = 0; i < n; i++) { // save temporily
		    x[i] = pSonB->pNds.GetAt(i)->x;
	    	y[i] = pSonB->pNds.GetAt(i)->y;
	    	z[i] = pSonB->pNds.GetAt(i)->z;
	    }
        // get the original version of new coordinate, local coordinate system
	    a[0] = tCrdt->lx;    a[1] = tCrdt->ly;    a[2] = tCrdt->lz;
	    a[3] = tCrdt->mx;    a[4] = tCrdt->my;    a[5] = tCrdt->mz;
	    a[6] = tCrdt->nx;    a[7] = tCrdt->ny;    a[8] = tCrdt->nz;

	    for(i = 0; i < n; i++) { // rotate
	    	pSonB->pNds.GetAt(i)->x = x[i] * a[0] + y[i] * a[1] + z[i] * a[2];
	    	pSonB->pNds.GetAt(i)->y = x[i] * a[3] + y[i] * a[4] + z[i] * a[5];
	    	pSonB->pNds.GetAt(i)->z = x[i] * a[6] + y[i] * a[7] + z[i] * a[8];
	    }
	    for(i = 0; i < n; i++) { // transform
	    	pSonB->pNds.GetAt(i)->x += tCrdt->u;
	    	pSonB->pNds.GetAt(i)->y += tCrdt->v;
	    	pSonB->pNds.GetAt(i)->z += tCrdt->w;
	    }

	    delete [] x;  delete [] y;  delete [] z;  //delete [] a;
	    delete [] p;

		// Tep to save
		SaveSubLoop(pSonA);  SaveSubLoop(pSonB);

	//	exit(0);
		return 2;
	}
	

	// form 2 sub-loops
	if(position[0] == -1 && position[1] == -1) {
		return 0;
		
	}
	if(position[0] == -1 || position[1] == -1) {
		AfxMessageBox(_T("Error: CInsect::CreateSubLoop()"));
		SaveLoop(pOrig, pEdge);
	}
	if(position[0] < position[1] ) {
		//pSonA
		for(i = 0; i < position[0] + 1; i++) { // +1
			pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		}
		for(i = 0; i < pEdge->pNds.GetSize(); i++) {
			pSonA->pNds.Add(pEdge->pNds.GetAt(i));
		}
		for(i = position[1] + 1; i < pOrig->pNds.GetSize(); i++) { // +1
			pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		}
		
		//pSonB
		for(i = position[0] + 1; i < position[1] + 1; i++) { //+1
			pSonB->pNds.Add(pOrig->pNds.GetAt(i));
		}
		for(i = pEdge->pNds.GetUpperBound(); i > -1; i--) {
			pSonB->pNds.Add(pEdge->pNds.GetAt(i));
		}
	}
	else if(position[0] > position[1]) {
		//pSonA
		for(i = 0; i < position[1] + 1; i++) {
			pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		}
		for(i = pEdge->pNds.GetUpperBound(); i > -1; i--) {
			pSonA->pNds.Add(pEdge->pNds.GetAt(i));
		}
		for(i = position[0] + 1; i < pOrig->pNds.GetSize(); i++) {
			pSonA->pNds.Add(pOrig->pNds.GetAt(i));
		}
		
		//pSonB
		for(i = position[1] +1; i < position[0]+1; i++) {
			pSonB->pNds.Add(pOrig->pNds.GetAt(i));
		}
		for(i = 0; i < pEdge->pNds.GetSize(); i++) {
			pSonB->pNds.Add(pEdge->pNds.GetAt(i));
		}
		
	}
	else if( position[0] == position[1] ){ // ???!
		// judge distance
		double dist[2];
		N = pEdge->pNds.GetSize();
		pHead = pEdge->pNds.GetAt(0);
		pTail = pEdge->pNds.GetAt(N-1);
		CNode* tNode = pOrig->pNds.GetAt(position[0]);
		
		dist[0] = pow(pHead->x - tNode->x, 2) +
			pow(pHead->y - tNode->y, 2) +
			pow(pHead->z - tNode->z, 2);
		dist[1] = pow(pTail->x - tNode->x, 2) +
			pow(pTail->y - tNode->y, 2) +
			pow(pTail->z - tNode->z, 2);
		dist[0] = sqrt(dist[0]);  dist[1] = sqrt(dist[1]);
		
		if(dist[0] + myEps < dist[1]) { // position[0] > position[1]
			//pSonA
			for(i = 0; i < position[0] + 1; i++) { // +1
				pSonA->pNds.Add(pOrig->pNds.GetAt(i));
			}
			for(i = 0; i < pEdge->pNds.GetSize(); i++) {
				pSonA->pNds.Add(pEdge->pNds.GetAt(i));
			}
			for(i = position[1] + 1; i < pOrig->pNds.GetSize(); i++) { // +1
				pSonA->pNds.Add(pOrig->pNds.GetAt(i));
			}
			
			//pSonB
			for(i = pEdge->pNds.GetUpperBound(); i > -1; i--) {
				pSonB->pNds.Add(pEdge->pNds.GetAt(i));
			}
			
		}
		else if(dist[1] + myEps < dist[0]) { // position[0] < position[1]
			//pSonA
			for(i = 0; i < position[0] + 1; i++) { // +1
				pSonA->pNds.Add(pOrig->pNds.GetAt(i));
			}
			for(i = pEdge->pNds.GetUpperBound(); i > -1; i--) {
				pSonA->pNds.Add(pEdge->pNds.GetAt(i));
			}
			for(i = position[1] + 1; i < pOrig->pNds.GetSize(); i++) { // +1
				pSonA->pNds.Add(pOrig->pNds.GetAt(i));
			}
			
			//pSonB
			for(i = 0; i < pEdge->pNds.GetSize(); i++) {
				pSonB->pNds.Add(pEdge->pNds.GetAt(i));
			}
		}
		else {
			AfxMessageBox(_T("Error: CInsect::CreateSubLoop()"));
		}
		
		/*
		// tmp
		SaveSubLoop(pSonA);
		SaveSubLoop(pSonB);
		*/
		
	}
	
	// Need to check
	CNode* na, * nb;
	
	N = pSonA->pNds.GetSize();
	na = pSonA->pNds.GetAt(0); // loop a
	nb = pSonA->pNds.GetAt(N-1);
	if(fabs(na->x - nb->x) < myEps && 
		fabs(na->y - nb->y) < myEps && 
		fabs(na->z - nb->z) < myEps) {
		pSonA->pNds.RemoveAt(N-1);
	}
	
	N = pSonB->pNds.GetSize();
	na = pSonB->pNds.GetAt(0); // loop b
	nb = pSonB->pNds.GetAt(N-1);
	if(fabs(na->x - nb->x) < myEps && 
		fabs(na->y - nb->y) < myEps && 
		fabs(na->z - nb->z) < myEps) {
		pSonB->pNds.RemoveAt(N-1);
	}
	
	/*
	// Check CCW
	CLoop* tmp = new CLoop();
	if(pView->IsLoopCCW(pSonA) == false) {
	for(i = 0; i < pSonA->pNds.GetSize(); i++) {
	tmp->pNds.Add(pSonA->pNds.GetAt(i));
	}
	pSonA->pNds.RemoveAll();
	for(i = tmp->pNds.GetUpperBound(); i > -1; i--) {
	pSonA->pNds.Add(tmp->pNds.GetAt(i));
	}
	tmp->pNds.RemoveAll();
	}
	if(pView->IsLoopCCW(pSonB) == false) {
	for(i = 0; i < pSonB->pNds.GetSize(); i++) {
	tmp->pNds.Add(pSonB->pNds.GetAt(i));
	}
	pSonB->pNds.RemoveAll();
	for(i = tmp->pNds.GetUpperBound(); i > -1; i--) {
	pSonB->pNds.Add(tmp->pNds.GetAt(i));
	}
	tmp->pNds.RemoveAll();
	}
	*/
	
	return 2;
	
}

// 2012.1.9 by Mei Gang at University of Freiburg, Germany.
void CInsect::SaveLoop(CLoop* pOrig, CLoop* pEdge)
{
	// Save Property
	int i, j;
	CString fn;
	int NumOfProps;

	double x, y, z;
	int ID = 0;
	if(AfxMessageBox(_T("Saving Properties ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);
		fin << 3 + pEdge->pNds.GetSize() <<"   "<<  1 + pEdge->pEgs.GetSize() << "   "<< endl;
		NumOfProps = pOrig->pNds.GetSize() - 1;
		for(i = 0; i < NumOfProps; i++) { 
			x = pOrig->pNds.GetAt(i)->x;
			y = pOrig->pNds.GetAt(i)->y;
			z = pOrig->pNds.GetAt(i)->z;
	    	fin << ID++ <<"   "<<  x <<"   "<< y <<"   "<< z << "   "<< endl;
		}
		NumOfProps = pEdge->pNds.GetSize();
		for(i = 0; i < NumOfProps; i++) { 
			x = pEdge->pNds.GetAt(i)->x;
			y = pEdge->pNds.GetAt(i)->y;
			z = pEdge->pNds.GetAt(i)->z;
	    	fin << ID++ <<"   "<<  x <<"   "<< y <<"   "<< z << "   "<< endl;
		}

		ID = 0;
		fin << ID++ <<"   "<<  0 <<"   "<< 1 <<"   "<< 2 << "   "<< endl;
		NumOfProps = pEdge->pEgs.GetSize();
		int n0, n1, n2;
		for(i = 0; i < NumOfProps; i++) { 
			n0 = 2*i+3;  n1 = 2*i+3+1;  n2 = 2*i+3+1;
	    	fin << ID++ <<"   "<<  n0 <<"   "<< n1 <<"   "<< n2 << "   "<< endl;
		}
		fin.close();
	}

}

// 2012.2.9 by Mei Gang at University of Freiburg, Germany.
void CInsect::SaveSubLoop(CLoop* pLoop)
{
	// Save Property
	int i, j;
	CString fn;
	int NumOfProps;

	double x, y, z;
	int ID = 0;
	if(AfxMessageBox(_T("Saving Properties ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += ".OFF";
		ofstream fin(fn);
		fin << "OFF  "<< pLoop->pNds.GetSize() <<"  1   0  "<< endl;
		NumOfProps = pLoop->pNds.GetSize();
		for(i = 0; i < NumOfProps; i++) { 
			x = pLoop->pNds.GetAt(i)->x;
			y = pLoop->pNds.GetAt(i)->y;
			z = pLoop->pNds.GetAt(i)->z;
	    	fin <<  x <<"   "<< y <<"   "<< z << "   "<< endl;
		}
		fin << pLoop->pNds.GetSize();
		for(i = 0; i < NumOfProps; i++) {
			fin <<"  "<<i;
		}
		fin.close();
	}

}

// 2012.1.9 by Mei Gang at University of Freiburg, Germany.
void CInsect::SaveEdge()
{
	// Save Property
	int i, j;
	CString fn;
	int NumOfProps;

	double x, y, z;
	int ID = 0;
	if(AfxMessageBox(_T("Saving Properties ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);
		NumOfProps = m_aInsectEdge.GetSize();
		CNode* fst, *lst;
		for(i = 0; i < NumOfProps; i++) { 
			CEdge* eg = m_aInsectEdge.GetAt(i);
			fin << i <<"   "<<  eg->FstTri <<"   "<< eg->SecTri
			    <<"   "<<  eg->FstNd[0] <<"   "<< eg->FstNd[1] <<"   "<< eg->FstNd[2]
			    <<"   "<<  eg->SecNd[0] <<"   "<< eg->SecNd[1] <<"   "<< eg->SecNd[2] << "   "<< endl;
		}
		fin.close();
	}

}

// 2012.7.31 by Mei Gang at University of Freiburg, Germany.
void CInsect::SavePair()
{
	// Save Property
	CString fn;

	if(AfxMessageBox(_T("Saving Pairs ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);
		int NumOfPairs = m_aInsectPair.GetSize();
		int fst, lst;
		for(int i = 0; i < NumOfPairs; i++) { 
			fst = m_aInsectPair.GetAt(i)->FstID;
			lst = m_aInsectPair.GetAt(i)->SedID;
			fin << fst << "  " << lst << "  " << endl;
		}
		fin.close();
	}

}

void CInsect::SaveUpdateEdge()
{
	// Save Property
	int i, j;
	CString fn;
	int NumOfProps;

	double x, y, z;
	int ID = 0;
	if(AfxMessageBox(_T("Saving Properties ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);
		NumOfProps = m_aInsectEdge.GetSize();
		CNode* fst, *lst;
		/*
		for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
			fin << i <<"   "<<  m_aInsectLoop.GetAt(i)->iNds.GetSize() - 1 <<"   "; // -1
		    for(j = 0; j < m_aInsectLoop.GetAt(i)->iNds.GetSize() - 1; j++) { // -1
				fin << m_aInsectLoop.GetAt(i)->iNds.GetAt(j) << "   ";
		    }
			fin << endl;
		}
		*/
		for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
			fin << i <<"   "<<  m_aInsectLoop.GetAt(i)->iNds.GetSize()<< endl; // -1
		    for(j = 0; j < m_aInsectLoop.GetAt(i)->iNds.GetSize(); j++) { // -1
				fin << m_aInsectLoop.GetAt(i)->iNds.GetAt(j) << "   ";
		    }
			fin << endl << endl;
		}

		fin.close();
	}

}

void CInsect::SaveUpdateNode()
{
	// Save Property
	int i, j;
	CString fn;
	int NumOfNodes;

	double x, y, z;
	int ID = 0;
	if(AfxMessageBox(_T("Saving Update Nodes ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);
		NumOfNodes = m_aUpdateNode.GetSize();
		CNode* fst, *lst;
		for(i = 0; i < NumOfNodes; i++) { 
			x = m_aUpdateNode.GetAt(i)->x;
			y = m_aUpdateNode.GetAt(i)->y;
			z = m_aUpdateNode.GetAt(i)->z;
			fin << i <<"  "<<  x <<"   "<< y << "  " << z << endl;
		}
		fin.close();
	}

}

int CInsect::IsPointOnSegment3D(CNode * sp0, CNode * sp1, CNode * point, double eps)
{
	double dist, cross;
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

void CInsect::ConnectEdge(CLoop* lp)
{
	int i, j, k, t, oldNum, newNum;
	
	// Initiate
	for(i = 0; i < lp->pEgs.GetSize(); i++) {
		lp->pEgs.GetAt(i)->bUsed = false;
	}
	
	CEdge* start;
	
	CNode* pHead, *pTail;

	// Find non-used one
	for(i = 0; i < lp->pEgs.GetSize(); i++) {
		if(lp->pEgs.GetAt(i)->bUsed == false) {
			start = lp->pEgs.GetAt(i);  break;
		}
	}
	
	CLoop* insectLoop = new CLoop();
	insectLoop->pEgs.Add(start);  start->bUsed = true;
	insectLoop->pNds.Add(start->fstNode); // Save Node orderly
	insectLoop->pNds.Add(start->lstNode);
	
	bool bStop = false;
	while(bStop != true) { // No more added nodes
		oldNum = insectLoop->pNds.GetSize();

		pHead = insectLoop->pNds.GetAt(0);
		pTail = insectLoop->pNds.GetAt(insectLoop->pNds.GetUpperBound());
		
		for(i = 0; i < lp->pEgs.GetSize(); i++) {
			CEdge* eg = lp->pEgs.GetAt(i);
			if(eg->bUsed == true) {  continue;  }
			
			if( fabs(eg->SecNd[0] - pHead->x) < myEps && // 后面加入
				fabs(eg->SecNd[1] - pHead->y) < myEps &&
				fabs(eg->SecNd[2] - pHead->z) < myEps )
			{
				insectLoop->pNds.InsertAt(0, eg->fstNode); 
				eg->bUsed = true;  break;
			}
			if( fabs(eg->FstNd[0] - pHead->x) < myEps && // 后面加入
				fabs(eg->FstNd[1] - pHead->y) < myEps &&
			    fabs(eg->FstNd[2] - pHead->z) < myEps  )
			{
				insectLoop->pNds.InsertAt(0, eg->lstNode);
				eg->bUsed = true;  break;
			}
		}
		for(i = 0; i < lp->pEgs.GetSize(); i++) {
			CEdge* eg = lp->pEgs.GetAt(i);
			if(eg->bUsed == true) {  continue;  }
			
			if( fabs(eg->FstNd[0] - pTail->x) < myEps && // 前面加入
			    fabs(eg->FstNd[1] - pTail->y) < myEps &&
			    fabs(eg->FstNd[2] - pTail->z) < myEps )
			{
				insectLoop->pNds.Add(eg->lstNode); 
				eg->bUsed = true;  break;
			}
			if( fabs(eg->SecNd[0] - pTail->x) < myEps && // 前面加入
				fabs(eg->SecNd[1] - pTail->y) < myEps &&
				fabs(eg->SecNd[2] - pTail->z) < myEps )
			{
				insectLoop->pNds.Add(eg->fstNode);
				eg->bUsed = true;  break;
			}
		}
		
		newNum = insectLoop->pNds.GetSize();
		if(oldNum == newNum)  bStop = true;
	} //End while
	
	// Get the new one
    lp->pNds.RemoveAll();
	for(i = 0; i < insectLoop->pNds.GetSize(); i++) {
		lp->pNds.Add(insectLoop->pNds.GetAt(i));
	}
	insectLoop->pNds.RemoveAll();
	
}

// Connect intersection loop: based on Head, Tail
void CInsect::CreateInsectLoop(CSurf* surf)
{
	int i, j, k, t, n, oldNum, newNum;
	int first_id, last_id;

	// Initiate
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		m_aInsectEdge.GetAt(i)->bUsed = false;
	}

	bool bFinish = false;
	CEdge* start;
	
	while(bFinish != true) {
		// Find non-used one
		for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
			if(m_aInsectEdge.GetAt(i)->bUsed == false) {
				start = m_aInsectEdge.GetAt(i);  
				start->bUsed = true;
				break;
			}
		}

		CLoop* insectLoop = new CLoop();
		insectLoop->pEgs.Add(start);
		insectLoop->iNds.Add(start->Start);
		insectLoop->iNds.Add(start->End);

		bool bStop = false;
		while(bStop != true) {
			oldNum = insectLoop->pEgs.GetSize();
			n = insectLoop->iNds.GetSize();

			first_id = insectLoop->iNds.GetAt(0);
			last_id = insectLoop->iNds.GetAt(n-1);

			for(i = 0; i < m_aInsectEdge.GetSize(); i++) { // 从前面加入
				CEdge* eg = m_aInsectEdge.GetAt(i);
				if(eg->bUsed == true) {  continue;  }

				if(eg->Start == last_id) {
					insectLoop->iNds.Add(eg->End);
					insectLoop->pEgs.Add(eg);
					eg->bUsed = true;  break;
				}
				if(eg->End == last_id) {
					insectLoop->iNds.Add(eg->Start);
					insectLoop->pEgs.Add(eg);
					eg->bUsed = true;  break;
				}
			}
			for(i = 0; i < m_aInsectEdge.GetSize(); i++) { // 从后面加入
				CEdge* eg = m_aInsectEdge.GetAt(i);
				if(eg->bUsed == true) {  continue;  }

				if(eg->End == first_id) {
					insectLoop->iNds.InsertAt(0, eg->Start);
					insectLoop->pEgs.Add(eg);
					eg->bUsed = true;  break;
				}
				if(eg->Start == first_id) {
					insectLoop->iNds.InsertAt(0, eg->End);
					insectLoop->pEgs.Add(eg);
					eg->bUsed = true;  break;
				}
			}

			newNum = insectLoop->pEgs.GetSize();
			if(oldNum == newNum)  bStop = true;
		} //End while

		m_aInsectLoop.Add(insectLoop);

		// Check
		bFinish = true;
		for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
			if(m_aInsectEdge.GetAt(i)->bUsed == false) {
				bFinish = false;  break;
			}
		}
	}

	// Closed / Open ?
	// int flag; //outer(0) or inner(1) or line(2)
	for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
		CLoop* insectLoop = m_aInsectLoop.GetAt(i);
		n = insectLoop->iNds.GetSize();
		first_id = insectLoop->iNds.GetAt(0);
		last_id = insectLoop->iNds.GetAt(n-1);

		if(first_id == last_id)  insectLoop->flag = 1;
		else insectLoop->flag = 2;
	}

//	SaveUpdateEdge();  exit(0);
//	Check_Edge(surf, blk); exit(0);

}

// 2012.4.20
// for all general cases: assuming >2 edges share a vertex
// Connect intersection loop: based on Head, Tail
void CInsect::CreateInsectLoop_Extend(CSurf* surf)
{
	int i, j, k, t, n, oldNum, newNum;
	int id, first_id, last_id;

	// Step 1: count used times
	// Initiate
	for(i = 0; i < m_aUpdateNode.GetSize(); i++) {
		m_aUpdateNode.GetAt(i)->UseTime = 0;
	}
	
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		CEdge* eg = m_aInsectEdge.GetAt(i);
		id = eg->Start;  m_aUpdateNode.GetAt(id)->UseTime++;
		id = eg->End;  m_aUpdateNode.GetAt(id)->UseTime++;
	}

	// Step 2: form closed , open, or bi-closed loops
	// Initiate
	for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
		m_aInsectEdge.GetAt(i)->bUsed = false;
	}

	bool bFinish = false;
	CEdge* start;
	
	while(bFinish != true) {
		// Find non-used one
		for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
			if(m_aInsectEdge.GetAt(i)->bUsed == false) {
				start = m_aInsectEdge.GetAt(i);  
				start->bUsed = true;
				break;
			}
		}

		CLoop* insectLoop = new CLoop();
		insectLoop->pEgs.Add(start);
		insectLoop->iNds.Add(start->Start);
		insectLoop->iNds.Add(start->End);

		bool bStop = false;
		while(bStop != true) {
			oldNum = insectLoop->pEgs.GetSize();
			n = insectLoop->iNds.GetSize();

			first_id = insectLoop->iNds.GetAt(0);
			last_id = insectLoop->iNds.GetAt(n-1);

			// Check used time
			int usetime;

			usetime = m_aUpdateNode.GetAt(last_id)->UseTime;
			if(usetime < 3) {// 1 or 2
			for(i = 0; i < m_aInsectEdge.GetSize(); i++) { // 从前面加入
				CEdge* eg = m_aInsectEdge.GetAt(i);
				if(eg->bUsed == true) {  continue;  }

				if(eg->Start == last_id) {
					insectLoop->iNds.Add(eg->End);
					insectLoop->pEgs.Add(eg);
					eg->bUsed = true;  break;
				}
				if(eg->End == last_id) {
					insectLoop->iNds.Add(eg->Start);
					insectLoop->pEgs.Add(eg);
					eg->bUsed = true;  break;
				}
			}
			}

			usetime = m_aUpdateNode.GetAt(first_id)->UseTime;
			if(usetime < 3) {// 1 or 2
			for(i = 0; i < m_aInsectEdge.GetSize(); i++) { // 从后面加入
				CEdge* eg = m_aInsectEdge.GetAt(i);
				if(eg->bUsed == true) {  continue;  }

				if(eg->End == first_id) {
					insectLoop->iNds.InsertAt(0, eg->Start);
					insectLoop->pEgs.Add(eg);
					eg->bUsed = true;  break;
				}
				if(eg->Start == first_id) {
					insectLoop->iNds.InsertAt(0, eg->End);
					insectLoop->pEgs.Add(eg);
					eg->bUsed = true;  break;
				}
			}
			}

			newNum = insectLoop->pEgs.GetSize();
			if(oldNum == newNum)  bStop = true;
		} //End while

		m_aInsectLoop.Add(insectLoop);

		// Check
		bFinish = true;
		for(i = 0; i < m_aInsectEdge.GetSize(); i++) {
			if(m_aInsectEdge.GetAt(i)->bUsed == false) {
				bFinish = false;  break;
			}
		}
	}

	// Step 3: set them as the closed in some aspect
	// Closed / Open or others ?
	// int flag; //outer(0) or inner(1) or line(2)
	int first_usetime, last_usetime;
	for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
		CLoop* insectLoop = m_aInsectLoop.GetAt(i);
		n = insectLoop->iNds.GetSize();
		first_id = insectLoop->iNds.GetAt(0);
		last_id = insectLoop->iNds.GetAt(n-1);

		if(first_id == last_id)  insectLoop->flag = 1;
		else insectLoop->flag = 2;

		// Check
		first_usetime = m_aUpdateNode.GetAt(first_id)->UseTime;
		last_usetime = m_aUpdateNode.GetAt(last_id)->UseTime;
		if(first_usetime > 2 && last_usetime > 2) {
			insectLoop->flag = 1;
		}
	}

	/*
	// tmp: special for open surfaces
	for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
		CLoop* insectLoop = m_aInsectLoop.GetAt(i);
		insectLoop->flag = 1; // very important
	}
	*/

	/*
	int usetime;
	for(i = 0; i < m_aUpdateNode.GetSize(); i++) {
		usetime = m_aUpdateNode.GetAt(i)->UseTime;
		if(usetime > 2) {
			CString str;
			str.Format(_T("The Node: %d \n\nUse Time: %d"), i, usetime);
			AfxMessageBox(str);
		}
	}
	*/
//  SaveUpdateEdge();  exit(0);
//	Check_Edge(surf, blk); exit(0);


}

// Form intersection cross surface : on surface
void CInsect::ObtainInsectSurf(CSurf* surf)
{
	int i, j, k, t, head, tail, nID[3], n, oldNum, newNum;
	bool bexist;

	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		surf->pTrs.GetAt(i)->bUsed = false;
	}

	for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
		CLoop* lp = m_aInsectLoop.GetAt(i);
		if(lp->flag == 2)  continue; 	// outer(0) or inner(1) or line(2)

		CSurf* insectSurf = new CSurf();
		CArray<CEdge*, CEdge*> tEdge;
		
		for(j = 0; j < lp->iNds.GetSize() - 1; j++) {
			head = lp->iNds.GetAt(j);
			tail = lp->iNds.GetAt(j+1);
		//	tEdge.Add(new CEdge(head, tail));
			tEdge.Add(new CEdge(tail, head));
		}

		oldNum = 0;  newNum = 1; // 初始化
		while(oldNum != newNum) {
		
			/*
	////////// tmp
	int ti, tj;
	CString fn;
	int NumOfProps;

	double x, y, z;
	int ID = 0;
	if(AfxMessageBox(_T("Saving Properties ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(false,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += ".txt";
		ofstream fin(fn);
		NumOfProps = tEdge.GetSize();

		for(ti = 0; ti < NumOfProps; ti++) { 
			CEdge* eg = tEdge.GetAt(ti);
			fin << ti <<"   "<<  eg->Start <<"   "<< eg->End <<"   "<<  eg->End << endl;
		}
		fin.close();
	}
	*/


	///////////////////////////////////end tmp
			oldNum = insectSurf->pTrs.GetSize();

			n = tEdge.GetSize();
			for(j = 0; j < n; j++) {
				tEdge.GetAt(j)->bUsed = true;
				
				head = tEdge.GetAt(j)->Start;
				tail = tEdge.GetAt(j)->End;

				for(int jj = 0; jj < surf->pTrs.GetSize(); jj++) {
					if(surf->pTrs.GetAt(jj)->bUsed == true)  continue; // has been used!

					for(t = 0; t < 3; t++) {
						nID[t] = surf->pTrs.GetAt(jj)->nID[t];
					}

					if((head == nID[0] && tail == nID[1]) ){ 
						CEdge* eg_a = new CEdge(nID[2], nID[1]);  

						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_a->Start == eg->End && eg_a->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_a->bUsed = false;  tEdge.Add(eg_a);
						}
						else { delete eg_a; }

						CEdge* eg_b = new CEdge(nID[0], nID[2]);  

						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_b->Start == eg->End && eg_b->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_b->bUsed = false;  tEdge.Add(eg_b);
						}
						else { delete eg_b; }

						insectSurf->pTrs.Add(surf->pTrs.GetAt(jj));
						surf->pTrs.GetAt(jj)->bUsed = true;
						break;
					}

					if((head == nID[1] && tail == nID[2]) ){
						CEdge* eg_a = new CEdge(nID[0], nID[2]);  
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_a->Start == eg->End && eg_a->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_a->bUsed = false;  tEdge.Add(eg_a);
						}
						else { delete eg_a; }

						CEdge* eg_b = new CEdge(nID[1], nID[0]);  
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_b->Start == eg->End && eg_b->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_b->bUsed = false;  tEdge.Add(eg_b);
						}
						else { delete eg_b; }

						insectSurf->pTrs.Add(surf->pTrs.GetAt(jj));
						surf->pTrs.GetAt(jj)->bUsed = true;
						break;
					}

					if((head == nID[2] && tail == nID[0]) ){
						CEdge* eg_a = new CEdge(nID[2], nID[1]);  
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_a->Start == eg->End && eg_a->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_a->bUsed = false;  tEdge.Add(eg_a);
						}
						else { delete eg_a; }

						CEdge* eg_b = new CEdge(nID[1], nID[0]); 
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_b->Start == eg->End && eg_b->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_b->bUsed = false;  tEdge.Add(eg_b);
						}
						else { delete eg_b; }

						insectSurf->pTrs.Add(surf->pTrs.GetAt(jj));
						surf->pTrs.GetAt(jj)->bUsed = true;
						break;
					} // end if

				} // end for
				
			} // end for

			for(j = tEdge.GetUpperBound(); j > -1; j--) {
				if(tEdge.GetAt(j)->bUsed == true) {
					delete tEdge.GetAt(j);  tEdge.RemoveAt(j);
				}
			}

			newNum = insectSurf->pTrs.GetSize();

			/*
			// tmp
			CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
            CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	        pView->SaveTrglMesh(insectSurf);
			*/


		} // end while

		CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
        CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	    pView->SaveTrglMesh(insectSurf);

		insectSurf->pLps.Add(lp); // 加入边界环
		m_aInsectSurf.Add(insectSurf);
	}

	// exit(0);

}

// Create sub Blocks : on block
void CInsect::CreateSubBlock(CSurf* surf, CBlock* blk)
{
	int i, j, k, t, n, oldNum, newNum, nID[3];
	int head, tail;
		
	for(i = 0; i < blk->pTrs.GetSize(); i++) {
		blk->pTrs.GetAt(i)->bUsed = false;
		blk->pTrs.GetAt(i)->bValid = true;
	}
	for(i = 0; i < m_aInsectSurf.GetSize(); i++) {
		m_aInsectSurf.GetAt(i)->bUsed = false;
	}

	// 先找出截面周边三角形，设置为 bValid = false;
	for(i = 0; i < m_aInsectSurf.GetSize(); i++) {
		CLoop* lp = m_aInsectSurf.GetAt(i)->pLps.GetAt(0);
		for(j = 0; j < lp->pEgs.GetSize(); j++) {
			CEdge* eg = lp->pEgs.GetAt(j);
			head = eg->Start;  tail = eg->End;
			for(k = 0; k < blk->pTrs.GetSize(); k++) {
				for(t = 0; t < 3; t++) {
					nID[t] = blk->pTrs.GetAt(k)->nID[t];
				}
				if((head == nID[0] && tail == nID[1]) || 
				   (head == nID[1] && tail == nID[2]) || 
				   (head == nID[2] && tail == nID[0]) ){
					   blk->pTrs.GetAt(k)->bValid = false;  break;
				}
			} 
		}
	}

	bool bFinish = false;
	CSurf* start;
	while(bFinish != true) {
        // Find an surface
		for(i = 0; i < m_aInsectSurf.GetSize(); i++) {
			if(m_aInsectSurf.GetAt(i)->bUsed == false) {
				start = m_aInsectSurf.GetAt(i);  
				start->bUsed = true;  break;
			}
		}

		// Create block
		CBlock* subBlock = new CBlock();

		CLoop* lp = start->pLps.GetAt(0);
		CArray<CEdge*, CEdge*> tEdge;
		for(j = 0; j < lp->iNds.GetSize() - 1; j++) {
			head = lp->iNds.GetAt(j);
			tail = lp->iNds.GetAt(j+1);
			tEdge.Add(new CEdge(head, tail));
		}

		oldNum = 0;  newNum = 1;
		while(oldNum != newNum) {
			oldNum = subBlock->pTrs.GetSize();

			n = tEdge.GetSize();
			for(j = 0; j < n; j++) {
		//		tEdge.GetAt(j)->bUsed = false;
				tEdge.GetAt(j)->bUsed = true;
				head = tEdge.GetAt(j)->Start;
				tail = tEdge.GetAt(j)->End;

				for(int jj = 0; jj < blk->pTrs.GetSize(); jj++) {
					if(blk->pTrs.GetAt(jj)->bUsed == true)  continue;

					for(t = 0; t < 3; t++) {
						nID[t] = blk->pTrs.GetAt(jj)->nID[t];
					}

					if((head == nID[0] && tail == nID[1]) ){
						tEdge.GetAt(j)->bUsed = true;
						CEdge* eg_a = new CEdge(nID[2], nID[1]);  eg_a->bUsed = false;  tEdge.Add(eg_a);
						CEdge* eg_b = new CEdge(nID[0], nID[2]);  eg_b->bUsed = false;  tEdge.Add(eg_b);

						subBlock->pTrs.Add(blk->pTrs.GetAt(jj));
						blk->pTrs.GetAt(jj)->bUsed = true;
						break;
					}
					if((head == nID[1] && tail == nID[2]) ){
						tEdge.GetAt(j)->bUsed = true;
						CEdge* eg_a = new CEdge(nID[0], nID[2]);  eg_a->bUsed = false;  tEdge.Add(eg_a);
						CEdge* eg_b = new CEdge(nID[1], nID[0]);  eg_b->bUsed = false;  tEdge.Add(eg_b);

						subBlock->pTrs.Add(blk->pTrs.GetAt(jj));
						blk->pTrs.GetAt(jj)->bUsed = true;
						break;
					}
					if((head == nID[2] && tail == nID[0]) ){
						tEdge.GetAt(j)->bUsed = true;
						CEdge* eg_a = new CEdge(nID[2], nID[1]);  eg_a->bUsed = false;  tEdge.Add(eg_a);
						CEdge* eg_b = new CEdge(nID[1], nID[0]);  eg_b->bUsed = false;  tEdge.Add(eg_b);

						subBlock->pTrs.Add(blk->pTrs.GetAt(jj));
						blk->pTrs.GetAt(jj)->bUsed = true;
						break;
					} // end if
				} // end for
			} // end for

			for(j = tEdge.GetUpperBound(); j > -1; j--) {
				if(tEdge.GetAt(j)->bUsed == true) {
					delete tEdge.GetAt(j);  tEdge.RemoveAt(j);
				}
			}

			newNum = subBlock->pTrs.GetSize();

			// SaveTrglBlock(subBlock);
	
		} // end while

		m_aSubBlock.Add(subBlock); // add new

	    // Check
	    bFinish = true;
	    for(i = 0; i < m_aInsectSurf.GetSize(); i++) {
		    if(m_aInsectSurf.GetAt(i)->bUsed == false) {
			   bFinish = false;  break;
		    }
	    }
	}

}

// Create sub-surfaces
void CInsect::CreateSubSurface(CSurf* surf, CBlock* blk)
{
	int i, j, k, t, n, id, oldNum, newNum, nID[3];
	int head, tail;
	bool bexist;

	CArray<CLoop*, CLoop*> m_aOpposiLoop; // To save opposite loops temporarily
	for(i = 0; i < m_aInsectLoop.GetSize(); i++) { // Create opposite loops
		CLoop* lp = m_aInsectLoop.GetAt(i);
		CLoop* newLp = new CLoop();

		// Set the same ID, very important
		lp->ID = i;  newLp->ID = i;

		// Set direction, very important
		lp->iDirection = Positive;  newLp->iDirection = Negative;

		for(j = lp->iNds.GetUpperBound(); j > -1; j--) {
			id = lp->iNds.GetAt(j);
			newLp->iNds.Add(id);
		}
		for(j = 0; j < newLp->iNds.GetUpperBound(); j++) {
			head = newLp->iNds.GetAt(j);
			tail = newLp->iNds.GetAt(j+1);
			newLp->pEgs.Add(new CEdge(head, tail));
		}
		m_aOpposiLoop.Add(newLp);
	}
	// Add the opposite loops
	for(i = 0; i < m_aOpposiLoop.GetSize(); i++) {
		m_aInsectLoop.Add(m_aOpposiLoop.GetAt(i));
	}

	// For Block
	for(i = 0; i < blk->pTrs.GetSize(); i++) { // initiate
		blk->pTrs.GetAt(i)->bUsed = false;
		blk->pTrs.GetAt(i)->bValid = true;
	}

	// Get Sub-surfaces
	for(i = 0; i < blk->pTrs.GetSize(); i++) {
		blk->pTrs.GetAt(i)->bUsed = false;
	}

	for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
		CLoop* lp = m_aInsectLoop.GetAt(i);
		if(lp->flag == 2)  continue; 	// outer(0) or inner(1) or line(2)

		CSurf* insectSurf = new CSurf();
		CArray<CEdge*, CEdge*> tEdge;
		
		for(j = 0; j < lp->iNds.GetSize() - 1; j++) {
			head = lp->iNds.GetAt(j);
			tail = lp->iNds.GetAt(j+1);
			tEdge.Add(new CEdge(head, tail));
		}

		oldNum = 0;  newNum = 1; // 初始化
		while(oldNum != newNum) {
			oldNum = insectSurf->pTrs.GetSize();

			n = tEdge.GetSize();
			for(j = 0; j < n; j++) {
				tEdge.GetAt(j)->bUsed = true;
				
				head = tEdge.GetAt(j)->Start;
				tail = tEdge.GetAt(j)->End;

				for(int jj = 0; jj < blk->pTrs.GetSize(); jj++) {
					if(blk->pTrs.GetAt(jj)->bUsed == true)  continue; // has been used!

					for(t = 0; t < 3; t++) {
						nID[t] = blk->pTrs.GetAt(jj)->nID[t];
					}

					if((head == nID[0] && tail == nID[1]) ){ 
						CEdge* eg_a = new CEdge(nID[2], nID[1]);  

						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_a->Start == eg->End && eg_a->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_a->bUsed = false;  tEdge.Add(eg_a);
						}
						else { delete eg_a; }

						CEdge* eg_b = new CEdge(nID[0], nID[2]);  

						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_b->Start == eg->End && eg_b->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_b->bUsed = false;  tEdge.Add(eg_b);
						}
						else { delete eg_b; }

						insectSurf->pTrs.Add(blk->pTrs.GetAt(jj));
						blk->pTrs.GetAt(jj)->bUsed = true;
						break;
					}

					if((head == nID[1] && tail == nID[2]) ){

						CEdge* eg_a = new CEdge(nID[0], nID[2]);  
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_a->Start == eg->End && eg_a->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_a->bUsed = false;  tEdge.Add(eg_a);
						}
						else { delete eg_a; }

						CEdge* eg_b = new CEdge(nID[1], nID[0]);  
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_b->Start == eg->End && eg_b->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_b->bUsed = false;  tEdge.Add(eg_b);
						}
						else { delete eg_b; }

						insectSurf->pTrs.Add(blk->pTrs.GetAt(jj));
						blk->pTrs.GetAt(jj)->bUsed = true;
						break;
					}

					if((head == nID[2] && tail == nID[0]) ){

						CEdge* eg_a = new CEdge(nID[2], nID[1]);  
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_a->Start == eg->End && eg_a->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_a->bUsed = false;  tEdge.Add(eg_a);
						}
						else { delete eg_a; }

						CEdge* eg_b = new CEdge(nID[1], nID[0]); 
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_b->Start == eg->End && eg_b->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_b->bUsed = false;  tEdge.Add(eg_b);
						}
						else { delete eg_b; }

						insectSurf->pTrs.Add(blk->pTrs.GetAt(jj));
						blk->pTrs.GetAt(jj)->bUsed = true;
						break;
					} // end if

				} // end for
				
			} // end for

			// Check edge
			for(j = 0; j < tEdge.GetSize(); j++) {
				CEdge* eg = tEdge.GetAt(j);
				if(eg->bUsed == true)  continue;

				for(k = 0; k < m_aInsectLoop.GetSize(); k++) {
					CLoop* tmpLp = m_aInsectLoop.GetAt(k);
					for(t = 0; t < tmpLp->iNds.GetSize() - 1; t++) {
						head = tmpLp->iNds.GetAt(t);
						tail = tmpLp->iNds.GetAt(t+1);
						if( (eg->Start == head && eg->End == tail) ||
							(eg->End == head && eg->Start == tail)) {
								eg->bUsed = true;  break;
						}
					}
					if(eg->bUsed == true)  break;
				}
			}

			for(j = tEdge.GetUpperBound(); j > -1; j--) {
				if(tEdge.GetAt(j)->bUsed == true) {
					delete tEdge.GetAt(j);  tEdge.RemoveAt(j);
				}
			}

			newNum = insectSurf->pTrs.GetSize();
		} // end while

		int trglNum = insectSurf->pTrs.GetSize();
		if(trglNum == 0) {
			CString str;
			str = _T("Loop cannot form Sub-Surface ! \n\n");
			str += _T("Reason: the Sub-Surface has been formed !");
			AfxMessageBox(str);
			continue;
		}

		CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
        CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	    pView->SaveTrglMesh(insectSurf);

		// 2012.4.18 new加入边界环
		// Check and add more sub-Loops
	    for(j = 0; j < m_aInsectLoop.GetSize(); j++) {
			CLoop* lp = m_aInsectLoop.GetAt(j);
			bool bfind = false;

			for(k = 0; k < lp->iNds.GetSize() - 1; k++) {
			    head = lp->iNds.GetAt(k);
			    tail = lp->iNds.GetAt(k+1);
			
				for(t = 0; t < insectSurf->pTrs.GetSize(); t++) {
					CTrgl* tr = insectSurf->pTrs.GetAt(t);
					if( (head == tr->nID[0] && tail == tr->nID[1]) ||
						(head == tr->nID[1] && tail == tr->nID[2]) ||
						(head == tr->nID[2] && tail == tr->nID[0]) ){
							bfind = true;  break;
					}
				}
				if(bfind == true)  break;
		     }

			if(bfind == true)  insectSurf->pLps.Add(lp);
	    }
		CString str;
		str.Format(_T("Sub-Loops: %d\n"), insectSurf->pLps.GetSize());
		AfxMessageBox(str);

		insectSurf->iParent = MESH_MODEL_B; // Set parent
		m_aSubSurf.Add(insectSurf);
	}


	// For Surface
	for(i = 0; i < surf->pTrs.GetSize(); i++) { // initiate
		surf->pTrs.GetAt(i)->bUsed = false;
		surf->pTrs.GetAt(i)->bValid = true;
	}

	// Get Sub-surfaces
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		surf->pTrs.GetAt(i)->bUsed = false;
	}

	for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
		CLoop* lp = m_aInsectLoop.GetAt(i);
		if(lp->flag == 2)  continue; 	// outer(0) or inner(1) or line(2)

		CSurf* insectSurf = new CSurf();
		CArray<CEdge*, CEdge*> tEdge;
		
		for(j = 0; j < lp->iNds.GetSize() - 1; j++) {
			head = lp->iNds.GetAt(j);
			tail = lp->iNds.GetAt(j+1);
			tEdge.Add(new CEdge(head, tail));
		}

		oldNum = 0;  newNum = 1; // 初始化
		while(oldNum != newNum) {
			oldNum = insectSurf->pTrs.GetSize();

			n = tEdge.GetSize();
			for(j = 0; j < n; j++) {
				tEdge.GetAt(j)->bUsed = true;
				
				head = tEdge.GetAt(j)->Start;
				tail = tEdge.GetAt(j)->End;

				for(int jj = 0; jj < surf->pTrs.GetSize(); jj++) {
					if(surf->pTrs.GetAt(jj)->bUsed == true)  continue; // has been used!

					for(t = 0; t < 3; t++) {
						nID[t] = surf->pTrs.GetAt(jj)->nID[t];
					}

					if((head == nID[0] && tail == nID[1]) ){ 
						CEdge* eg_a = new CEdge(nID[2], nID[1]);  

						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_a->Start == eg->End && eg_a->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_a->bUsed = false;  tEdge.Add(eg_a);
						}
						else { delete eg_a; }

						CEdge* eg_b = new CEdge(nID[0], nID[2]);  

						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_b->Start == eg->End && eg_b->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_b->bUsed = false;  tEdge.Add(eg_b);
						}
						else { delete eg_b; }

						insectSurf->pTrs.Add(surf->pTrs.GetAt(jj));
						surf->pTrs.GetAt(jj)->bUsed = true;
						break;
					}

					if((head == nID[1] && tail == nID[2]) ){

						CEdge* eg_a = new CEdge(nID[0], nID[2]);  
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_a->Start == eg->End && eg_a->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_a->bUsed = false;  tEdge.Add(eg_a);
						}
						else { delete eg_a; }

						CEdge* eg_b = new CEdge(nID[1], nID[0]);  
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_b->Start == eg->End && eg_b->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_b->bUsed = false;  tEdge.Add(eg_b);
						}
						else { delete eg_b; }

						insectSurf->pTrs.Add(surf->pTrs.GetAt(jj));
						surf->pTrs.GetAt(jj)->bUsed = true;
						break;
					}

					if((head == nID[2] && tail == nID[0]) ){

						CEdge* eg_a = new CEdge(nID[2], nID[1]);  
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_a->Start == eg->End && eg_a->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_a->bUsed = false;  tEdge.Add(eg_a);
						}
						else { delete eg_a; }

						CEdge* eg_b = new CEdge(nID[1], nID[0]); 
						// must check, very important!
						bexist = false;
						for(int kk = 0; kk < tEdge.GetSize(); kk++) {
							CEdge* eg = tEdge.GetAt(kk);
							if(eg_b->Start == eg->End && eg_b->End == eg->Start) {
								bexist = true;  break;
							}
						}
						if(bexist == false) {
						    eg_b->bUsed = false;  tEdge.Add(eg_b);
						}
						else { delete eg_b; }

						insectSurf->pTrs.Add(surf->pTrs.GetAt(jj));
						surf->pTrs.GetAt(jj)->bUsed = true;
						break;
					} // end if

				} // end for
				
			} // end for

			// Check edge
			for(j = 0; j < tEdge.GetSize(); j++) {
				CEdge* eg = tEdge.GetAt(j);
				if(eg->bUsed == true)  continue;

				for(k = 0; k < m_aInsectLoop.GetSize(); k++) {
					CLoop* tmpLp = m_aInsectLoop.GetAt(k);
					for(t = 0; t < tmpLp->iNds.GetSize() - 1; t++) {
						head = tmpLp->iNds.GetAt(t);
						tail = tmpLp->iNds.GetAt(t+1);
						if( (eg->Start == head && eg->End == tail) ||
							(eg->End == head && eg->Start == tail)) {
								eg->bUsed = true;  break;
						}
					}
					if(eg->bUsed == true)  break;
				}
			}

			for(j = tEdge.GetUpperBound(); j > -1; j--) {
				if(tEdge.GetAt(j)->bUsed == true) {
					delete tEdge.GetAt(j);  tEdge.RemoveAt(j);
				}
			}

			newNum = insectSurf->pTrs.GetSize();
		} // end while

		int trglNum = insectSurf->pTrs.GetSize();
		if(trglNum == 0) {
			CString str;
			str = _T("Loop cannot form Sub-Surface ! \n\n");
			str += _T("Reason: the Sub-Surface has been formed !");
			AfxMessageBox(str);
			continue;
		}

		CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
        CRealModelView* pView =(CRealModelView*) pMain->GetActiveView();
	    pView->SaveTrglMesh(insectSurf);

		// 2012.4.18 new加入边界环
		// Check and add more sub-Loops
	    for(j = 0; j < m_aInsectLoop.GetSize(); j++) {
			CLoop* lp = m_aInsectLoop.GetAt(j);
			bool bfind = false;

			for(k = 0; k < lp->iNds.GetSize() - 1; k++) {
			    head = lp->iNds.GetAt(k);
			    tail = lp->iNds.GetAt(k+1);
			
				for(t = 0; t < insectSurf->pTrs.GetSize(); t++) {
					CTrgl* tr = insectSurf->pTrs.GetAt(t);
					if( (head == tr->nID[0] && tail == tr->nID[1]) ||
						(head == tr->nID[1] && tail == tr->nID[2]) ||
						(head == tr->nID[2] && tail == tr->nID[0]) ){
							bfind = true;  break;
					}
				}
				if(bfind == true)  break;
		     }

			if(bfind == true)  insectSurf->pLps.Add(lp);
	    }
		CString str;
		str.Format(_T("Sub-Loops: %d\n"), insectSurf->pLps.GetSize());
		AfxMessageBox(str);

		insectSurf->iParent = MESH_MODEL_A; // Set parent
		m_aSubSurf.Add(insectSurf);
	}

//	exit(0);

}

// new version, create all cases of blocks 2012.4.9
// unio, intersection, subtraction
void CInsect::CreateSubBlock()
{
	int i, j, k, t, n, nSurf, oldNum, newNum, nID[3];
	int head, tail;
	bool bFinish;
	CSurf* start, *current;
		
	for(i = 0; i < m_aSubSurf.GetSize(); i++) {
		m_aSubSurf.GetAt(i)->ID = i;
		m_aSubSurf.GetAt(i)->bUsed = false;
	}

	// Step 1
	// Create Unio and Intersection: A+B  A#B
	bFinish = false;
	while(bFinish != true) {
        // Find an surface
		for(i = 0; i < m_aSubSurf.GetSize(); i++) {
			if(m_aSubSurf.GetAt(i)->bUsed == false) {
				start = m_aSubSurf.GetAt(i);  
				start->bUsed = true;  break;
			}
		}

		// Create block
		CBlock* subBlock = new CBlock();
		subBlock->pSfs.Add(start);
		for(i = 0; i < start->pLps.GetSize(); i++) {
			start->pLps.GetAt(i)->iCurParent = start->ID; // 2012.4.18
			subBlock->pLps.Add(start->pLps.GetAt(i));
		}

		// 2012.4.18
		// 初始化, very important!
		for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
			m_aInsectLoop.GetAt(i)->bUsed = false;
		}
		oldNum = 0;  newNum = 1;  

		while(oldNum != newNum) { // end when no more sub-surface
			oldNum = subBlock->pSfs.GetSize();

			n = subBlock->pLps.GetSize();
			for(j = 0; j < n; j++) {
				CLoop* lp = subBlock->pLps.GetAt(j);
				current = m_aSubSurf.GetAt(lp->iCurParent); // 2012.4.18

				if(lp->bUsed == true)  continue;

				subBlock->pLps.GetAt(j)->bUsed = true;

				CSurf* sf;
				bool bfind = false;
				for(int jj = 0; jj < m_aSubSurf.GetSize(); jj++) {
					sf = m_aSubSurf.GetAt(jj);

					// Check whether same parent
					if(sf->iParent == current->iParent)  continue; // 2012.4.18

					// Check
					if(sf->bUsed == true)  continue;

					for(int kk = 0; kk < sf->pLps.GetSize(); kk++) {
						/*
						if(sf->pLps.GetAt(kk)->ID == lp->ID &&
						   sf->pLps.GetAt(kk)->iDirection == lp->iDirection) { // Direction
							bfind = true;  break;
						}
						*/
						if(sf->pLps.GetAt(kk)->ID == lp->ID &&
						   sf->pLps.GetAt(kk)->iDirection != lp->iDirection) { // Direction
							bfind = true;  break;
						}
					}
					 if(bfind == true)  break;
 				}

				if(bfind == true) {
					// Check
					bool bexist = false;
					for(int tt = 0; tt < subBlock->pSfs.GetSize(); tt++) {
						if(sf->ID == subBlock->pSfs.GetAt(tt)->ID) {
							bexist = true;  break;
						}
					}

					// Add
					if(bexist == true)  continue;

					// add new sub-surface
					sf->bUsed = true;
					subBlock->pSfs.Add(sf); 

					for(int jj = 0; jj < sf->pLps.GetSize(); jj++) { // add new loops
						sf->pLps.GetAt(jj)->iCurParent = sf->ID; // Set current parent!

						// Check
						bool bhave = false;
						for(int kk = 0; kk < subBlock->pLps.GetSize(); kk++) {
							if(sf->pLps.GetAt(jj)->ID == subBlock->pLps.GetAt(kk)->ID) {
								bhave = true;  break;
							}
						}

						// Add
						if(bhave == false) {
							subBlock->pLps.Add(sf->pLps.GetAt(jj));
						}
					}
				} // end if
			} // end for

			// Check number
			newNum = subBlock->pSfs.GetSize();

		} // end while

		
		// 2012.4.18
		// Check whether create valid sub-block
		int surfNum = subBlock->pSfs.GetSize();
		if(surfNum == 1) { // only the start itself, invalid
			subBlock->pSfs.RemoveAll();
			continue;
		}

		subBlock->iBoolean = A_U_B + A_I_B; // Set iBoolean: Union or Intersection
		m_aSubBlock.Add(subBlock); // add new

		// tmp save
		subBlock->pTrs.RemoveAll();
		for(i = 0; i < subBlock->pSfs.GetSize(); i++) {
			CSurf* sf = subBlock->pSfs.GetAt(i);
			for(j = 0; j < sf->pTrs.GetSize(); j++) {
				CTrgl* tr = sf->pTrs.GetAt(j);
				subBlock->pTrs.Add(tr);
			}
		}
		SaveTrglBlock(subBlock);

	    // Check
	    bFinish = true;
	    for(i = 0; i < m_aSubSurf.GetSize(); i++) {
		    if(m_aSubSurf.GetAt(i)->bUsed == false) {
			   bFinish = false;  break;
		    }
	    }
	}


	// Step 2
	// Create Subtraction: A-B, B-A
	for(i = 0; i < m_aSubSurf.GetSize(); i++) {
		m_aSubSurf.GetAt(i)->ID = i;
		m_aSubSurf.GetAt(i)->bUsed = false;
		for(j = 0; j < m_aSubSurf.GetAt(i)->pLps.GetSize(); j++) {
			m_aSubSurf.GetAt(i)->pLps.GetAt(j)->bUsed = false;
		}
	}

	bFinish = false;
	while(bFinish != true) {
        // Find an surface
		for(i = 0; i < m_aSubSurf.GetSize(); i++) {
			if(m_aSubSurf.GetAt(i)->bUsed == false) {
				start = m_aSubSurf.GetAt(i);  
				start->bUsed = true;  break;
			}
		}

		// Create block
		CBlock* subBlock = new CBlock();
		subBlock->pSfs.Add(start);
		for(i = 0; i < start->pLps.GetSize(); i++) {
			start->pLps.GetAt(i)->iCurParent = start->ID; // 2012.4.18
			subBlock->pLps.Add(start->pLps.GetAt(i));
		}

		// 2012.4.18
		// 初始化, very important!
		for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
			m_aInsectLoop.GetAt(i)->bUsed = false;
		}
		oldNum = 0;  newNum = 1;  

		while(oldNum != newNum) { // end when no more sub-surface
			oldNum = subBlock->pSfs.GetSize();

			n = subBlock->pLps.GetSize();
			for(j = 0; j < n; j++) {
				CLoop* lp = subBlock->pLps.GetAt(j);
				current = m_aSubSurf.GetAt(lp->iCurParent); // 2012.4.18

				if(lp->bUsed == true)  continue;

				subBlock->pLps.GetAt(j)->bUsed = true;

				CSurf* sf;
				bool bfind = false;
				for(int jj = 0; jj < m_aSubSurf.GetSize(); jj++) {
					sf = m_aSubSurf.GetAt(jj);

					// Check whether same parent
					if(sf->iParent == current->iParent)  continue; // 2012.4.18

					// Check
					if(sf->bUsed == true)  continue;

					for(int kk = 0; kk < sf->pLps.GetSize(); kk++) {
						if(sf->pLps.GetAt(kk)->ID == lp->ID &&
						   sf->pLps.GetAt(kk)->iDirection == lp->iDirection) { // Direction
							bfind = true;  break;
						}
						/*
						if(sf->pLps.GetAt(kk)->ID == lp->ID &&
						   sf->pLps.GetAt(kk)->iDirection != lp->iDirection) { // Direction
							bfind = true;  break;
						}
						*/
					}
					 if(bfind == true)  break;
 				}

				if(bfind == true) {
					// Check
					bool bexist = false;
					for(int tt = 0; tt < subBlock->pSfs.GetSize(); tt++) {
						if(sf->ID == subBlock->pSfs.GetAt(tt)->ID) {
							bexist = true;  break;
						}
					}

					// Add
					if(bexist == true)  continue;

					// add new sub-surface
					sf->bUsed = true;
					subBlock->pSfs.Add(sf); 
					for(int jj = 0; jj < sf->pLps.GetSize(); jj++) {
						sf->pLps.GetAt(jj)->iCurParent = sf->ID; // Set current parent!

						// Check
						bool bhave = false;
						for(int kk = 0; kk < subBlock->pLps.GetSize(); kk++) {
							if(sf->pLps.GetAt(jj)->ID == subBlock->pLps.GetAt(kk)->ID) {
								bhave = true;  break;
							}
						}

						// Add
						if(bhave == false) {
							subBlock->pLps.Add(sf->pLps.GetAt(jj));
						}
					}
				} // end if
			} // end for

			// Check number
			newNum = subBlock->pSfs.GetSize();

		} // end while

	    // 2012.4.18
		// Check whether create valid sub-block
		int surfNum = subBlock->pSfs.GetSize();
		if(surfNum == 1) { // only the start itself, invalid
			subBlock->pSfs.RemoveAll();
			continue;
		}

		subBlock->iBoolean = A_S_B + B_S_A; // Set iBoolean: A Subtract B or B Subtract A
		m_aSubBlock.Add(subBlock); // add new

		// tmp save
		subBlock->pTrs.RemoveAll();
		for(i = 0; i < subBlock->pSfs.GetSize(); i++) {
			CSurf* sf = subBlock->pSfs.GetAt(i);
			for(j = 0; j < sf->pTrs.GetSize(); j++) {
				CTrgl* tr = sf->pTrs.GetAt(j);
				subBlock->pTrs.Add(tr);
			}
		}
		SaveTrglBlock(subBlock);

	    // Check
	    bFinish = true;
	    for(i = 0; i < m_aSubSurf.GetSize(); i++) {
		    if(m_aSubSurf.GetAt(i)->bUsed == false) {
			   bFinish = false;  break;
		    }
	    }
	}

}

// 2012.4.11
// determine which part of boolean operation
void CInsect::DetermineSubBlock()
{
	int i, j, k, n, id;
	double *x, *y, *z;
	CNode maxGlobal, minGlobal;
	CNode maxLocal, minLocal;
	
	// Get global max and min coordinates
	n = m_aUpdateNode.GetSize();
	x = new double[n];  y = new double[n];  z = new double[n];
	for(i = 0; i < n; i++) {
		x[i] = m_aUpdateNode.GetAt(i)->x;
		y[i] = m_aUpdateNode.GetAt(i)->y;
		z[i] = m_aUpdateNode.GetAt(i)->z;
	}
	prhap(x, n);  prhap(y, n);  prhap(z, n);
	minGlobal.x = x[0];  maxGlobal.x = x[n-1];
	minGlobal.y = y[0];  maxGlobal.y = y[n-1];
	minGlobal.z = z[0];  maxGlobal.z = z[n-1];
	delete [] x;  delete [] y;  delete [] z;

	// Find the Union, only one
	for(i = 0; i < m_aSubBlock.GetSize(); i++) {
		CBlock* blk = m_aSubBlock.GetAt(i);
		n = blk->pTrs.GetSize();
		x = new double[3*n];
		y = new double[3*n];
		z = new double[3*n];

		for(j = 0; j < n; j++) {
			CTrgl* tr = blk->pTrs.GetAt(j);
			for(k = 0; k < 3; k++) {
				id = tr->nID[k];
				x[3*j+k] = m_aUpdateNode.GetAt(id)->x;
				y[3*j+k] = m_aUpdateNode.GetAt(id)->y;
				z[3*j+k] = m_aUpdateNode.GetAt(id)->z;
			}
		}

		prhap(x, 3*n);  prhap(y, 3*n);  prhap(z, 3*n);
	    minLocal.x = x[0];  maxLocal.x = x[3*n-1];
	    minLocal.y = y[0];  maxLocal.y = y[3*n-1];
	    minLocal.z = z[0];  maxLocal.z = z[3*n-1];
	    delete [] x;  delete [] y;  delete [] z;

		if( IsSameNode(&maxGlobal, &maxLocal, myEps) &&
			IsSameNode(&minGlobal, &minLocal, myEps) ){
				blk->iBoolean = A_U_B;  break;
		}
	}

	// Find intersection
	for(i = 0; i < m_aSubBlock.GetSize(); i++) {
		CBlock* blk = m_aSubBlock.GetAt(i);
		if(blk->iBoolean == A_U_B + A_I_B) {
			blk->iBoolean = A_I_B;
		}
	}

	// Update Sub-surface
	for(i = 0; i < m_aSubBlock.GetSize(); i++) {
		CBlock* blk = m_aSubBlock.GetAt(i);
		if(blk->iBoolean == A_U_B) {
			for(j = 0; j < blk->pSfs.GetSize(); j++) {
				blk->pSfs.GetAt(j)->iOutIn = OUTER;
			}
		}
		if(blk->iBoolean == A_I_B) {
		    for(j = 0; j < blk->pSfs.GetSize(); j++) {
				blk->pSfs.GetAt(j)->iOutIn = INNER;
			}
		}
	}

	// Find A - B or B - A
	for(i = 0; i < m_aSubBlock.GetSize(); i++) {
		CBlock* blk = m_aSubBlock.GetAt(i);
		if(blk->iBoolean == A_S_B + B_S_A) {
			for(j = 0; j < blk->pSfs.GetSize(); j++) {
				CSurf* sf = blk->pSfs.GetAt(j);
				if(sf->iOutIn == OUTER) {
					if(sf->iParent == MESH_MODEL_A) {
						blk->iBoolean = A_S_B;  break;
					}
					if(sf->iParent == MESH_MODEL_B) {
						blk->iBoolean = B_S_A;  break;
					}
				}
				if(sf->iOutIn == INNER) {
					if(sf->iParent == MESH_MODEL_B) {
						blk->iBoolean = A_S_B;  break;
					}
					if(sf->iParent == MESH_MODEL_A) {
						blk->iBoolean = B_S_A;  break;
					}
				}
			}
		}
	}



}

void CInsect::SaveSubBlockInfo()
{
	// Save Sub-Block Info
	if(AfxMessageBox(_T("Save Sub-Block info ?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg.DoModal() == IDOK) {
		CString fn = Dlg.GetPathName(); 
		if(fn.Right(4)!=".txt")   fn += "_Info.txt";
		ofstream fin(fn);
		for(int i = 0; i < m_aSubBlock.GetSize(); i++)	{
			int iBoolean = m_aSubBlock.GetAt(i)->iBoolean;
			switch (iBoolean) 
			{
			case A_U_B: { 
				fin <<"Sub-Block: "<< i+1 <<"   "<<"A_U_B"<< endl;
				break; }
			case A_I_B: { 
				fin <<"Sub-Block: "<< i+1 <<"   "<<"A_I_B" << endl;
				break; }
			case A_S_B: { 
				fin <<"Sub-Block: "<< i+1 <<"   "<<"A_S_B" << endl;
				break; }
			case B_S_A: { 
				fin <<"Sub-Block: "<< i+1 <<"   "<<"B_S_A" << endl;
				break; }
			default: { 
				fin <<"Sub-Block: "<< i+1 <<"   "<<"Error!"<< endl;
			    }
			}
		}
		fin.close(); 
	}

}

void CInsect::SaveTrglBlock(CBlock* blk)
{
	double xx,yy,zz;
	int i, n0, n1, n2, ID;
	int NumOfNodes,NumOfEles;
	CString fnHexMesh,fnHexNode;
	NumOfNodes = blk->pNds.GetSize();    
	NumOfEles = blk->pTrs.GetSize();

	//node
	if(AfxMessageBox(_T("确定保存Surf文件?"), MB_YESNOCANCEL) != 6) return;
	CFileDialog Dlg3(FALSE,NULL,NULL,OFN_HIDEREADONLY, _T("Data File(*.txt)|*.txt"), NULL);
	if(Dlg3.DoModal() == IDOK) {
		
		fnHexNode = Dlg3.GetPathName(); 
		if(fnHexNode.Right(4)!=".txt")   fnHexNode+="_Node.txt";
		ofstream fin3(fnHexNode);
		for(i=0;i<NumOfNodes-1;i++){
			ID = blk->pNds.GetAt(i)->ID;
			xx = blk->pNds.GetAt(i)->x;  
			yy = blk->pNds.GetAt(i)->y;  
			zz = blk->pNds.GetAt(i)->z;
			fin3 << i <<"   "<<  xx <<"   "<< yy<<"   "<< zz<<endl;
		}
		ID = blk->pNds.GetAt(i)->ID;
		xx = blk->pNds.GetAt(i)->x;  
		yy = blk->pNds.GetAt(i)->y;  
		zz = blk->pNds.GetAt(i)->z;
		fin3 << i <<"   "<<  xx <<"   "<< yy<<"   "<< zz;
		fin3.close(); 
		

	//mesh
		fnHexMesh = Dlg3.GetPathName(); 
		if(fnHexMesh.Right(4)!=".txt")   fnHexMesh+="_TMesh.txt";
		ofstream fin2(fnHexMesh);
		for(i=0;i<NumOfEles-1;i++)	{
			n0 = blk->pTrs.GetAt(i)->nID[0];
			n1 = blk->pTrs.GetAt(i)->nID[1];
			n2 = blk->pTrs.GetAt(i)->nID[2];
			fin2 <<i<<"   "<<n0<<"   "<<n1<<"   "<<n2 << endl;
		}
		n0 = blk->pTrs.GetAt(i)->nID[0];
		n1 = blk->pTrs.GetAt(i)->nID[1];
		n2 = blk->pTrs.GetAt(i)->nID[2];
		fin2 << i <<"   "<<n0<<"   "<<n1<<"   "<<n2 ;
		fin2.close(); 
	}

}

void CInsect::InsectOfSurfAndBlock()
{
	// Notice: very important!!! 2012.7.31
	// There are two cases: 
	//// 1: for Closed surface (Block) intersection
	//// 2: for Open surface intersection
	// The code is different!

	/*
	// Case 1: Special for Block intersection
//	SearchInsectPair(surf, blk); 
	SearchInsectPair_OBBTree(surf, blk); 
	AfxMessageBox(_T("SearchInsectPair() is Done!\n"));

//	ObtainInsectEdge(surf, blk); AfxMessageBox(_T("SearchInsectPair() is Done!\n"));
	ObtainInsectEdge_Parallel(surf, blk); 
	AfxMessageBox(_T("ObtainInsectEdge_Parallel() is Done!\n"));

	Update(surf, blk); 
	AfxMessageBox(_T("Update() is Done!\n"));

//	CreateInsectLoop(surf); 
	CreateInsectLoop_Extend(surf);
	AfxMessageBox(_T("CreateInsectLoop() is Done!\n"));

	CreateSubSurface(surf, blk); 
	AfxMessageBox(_T("CreateSubSurface() is Done!\n"));

	CreateSubBlock(); 
	AfxMessageBox(_T("CreateSubBlock() is Done!\n"));

	DetermineSubBlock(); 
	AfxMessageBox(_T("DetermineSubBlock() is Done!\n"));

	SaveSubBlockInfo();
	AfxMessageBox(_T("SaveSubBlockInfo() is Done!\n"));

	//ObtainInsectSurf(surf);
	//CreateSubBlock(surf, blk);
	
	*/
	
	// Case 2: For two Open Surface
//	SearchInsectPair(surf, blk);  AfxMessageBox(_T("SearchInsectPair() is Done!\n"));
	SearchInsectPair_OBBTree(surf, blk);  AfxMessageBox(_T("SearchInsectPair_OBBTree() is Done!\n"));

	ObtainInsectEdge(surf, blk); AfxMessageBox(_T("ObtainInsectEdge() is Done!\n"));
//	ObtainInsectEdge_Parallel(surf, blk);  AfxMessageBox(_T("ObtainInsectEdge_Parallel() is Done!\n"));
	
//	Merge_FirstTime(surf, blk);

	Update(surf, blk); 
//	Update_CDT(surf, blk); 
	AfxMessageBox(_T("Update() is Done!\n"));

	CreateInsectLoop(surf); 
//	CreateInsectLoop_Extend(surf);
	AfxMessageBox(_T("CreateInsectLoop() is Done!\n"));

	CreateSubSurface(surf, blk); 
	AfxMessageBox(_T("CreateSubSurface() is Done!\n"));

	system("pause");
	
}

//2010.4.20 by Mei Gang, University of Freiburg
//Read a triangle surface/plane: grid and nodes. New file format
void CInsect::ReadDataSurface(CSurf* surf)
{
	int i, j, k, m, n0, n1, n2, ndata;
	double x, y, z;
	int nNds, nTrgls;
	ndata = AfxMessageBox(_T("Read a Triangle Surface?"), MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ndata == 6){
//	m_nObjectType = OBJECT_TYPE_SURFACE;

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
void CInsect::ReadDataSurfaceEGC(CSurf* surf)
{
	int i, j, k, m, n0, n1, n2, ndata;
	float x, y, z; // must be float, cannot be double !
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

void CInsect::ReadSurface()
{
	surf = new CSurf();

#ifndef EGC
	ReadDataSurface(surf);
#else 
	ReadDataSurfaceEGC(surf);
#endif

}

void CInsect::ReadBlock()
{
	blk = new CBlock();
	CSurf* pTBlk = new CSurf();
	ReadDataSurface(pTBlk);

	for(int i = 0; i < pTBlk->pNds.GetSize(); i++) {
		blk->pNds.Add(pTBlk->pNds.GetAt(i));
	}
	for(int i = 0; i < pTBlk->pTrs.GetSize(); i++) {
		blk->pTrs.Add(pTBlk->pTrs.GetAt(i));
	}
}

void CInsect::Check(CSurf* surf, CBlock* blk)
{
	int i, j, k, t, tmp[3], nID[3];
	bool bFinish = false;

	CArray<CTrgl*, CTrgl*> have_check;
	CArray<CTrgl*, CTrgl*> will_check;
	CArray<CEdge*, CEdge*> allEdge;

	// for surface
	have_check.RemoveAll();  
	will_check.RemoveAll();  
	allEdge.RemoveAll();

	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		CTrgl* tr = surf->pTrs.GetAt(i);
		if(tr->bNeedCheck == false)  have_check.Add(tr);
		else  will_check.Add(tr);
	}

	// Special Case : have_check is empty
	if(have_check.IsEmpty() == true) {
		CTrgl* tr = will_check.GetAt(0);
		tr->bNeedCheck = false;
		have_check.Add(tr);
		will_check.RemoveAt(0); // Only remove, not delete
	}

	allEdge.RemoveAll();
	for(i = 0; i < have_check.GetSize(); i++) {
		for(t = 0; t < 3; t++) {
			nID[t] = have_check.GetAt(i)->nID[t];
		}
		allEdge.Add(new CEdge(nID[0], nID[1]));
		allEdge.Add(new CEdge(nID[1], nID[2]));
		allEdge.Add(new CEdge(nID[2], nID[0]));
	}
	for(i = 0; i < allEdge.GetSize(); i++) {
		allEdge.GetAt(i)->UseTime = 1;
	}
	for(i = 0; i < allEdge.GetSize() - 1; i++) {
		for(j = i+1; j < allEdge.GetSize(); j++) {
			if( (allEdge.GetAt(i)->Start == allEdge.GetAt(j)->End) && 
				(allEdge.GetAt(i)->End   == allEdge.GetAt(j)->Start)) {
					allEdge.GetAt(i)->UseTime++;
					allEdge.GetAt(j)->UseTime++;
			}
		}
	}
	for(i = allEdge.GetUpperBound(); i > -1; i--) { // Check
		if(allEdge.GetAt(i)->UseTime > 2)  {
			AfxMessageBox(_T("Error: Check()"));
		}

		if(allEdge.GetAt(i)->UseTime == 2) {
			delete allEdge.GetAt(i);  allEdge.RemoveAt(i);
		}
	}

	while(bFinish != true) {
		for(i = 0; i < allEdge.GetSize(); i++) { // Find
			CEdge* eg = allEdge.GetAt(i);
			for(j = 0; j < will_check.GetSize(); j++) {
				CTrgl* tr = will_check.GetAt(j);
				// Get IDs
				for(t = 0; t < 3; t++) {
					nID[t] = tr->nID[t];
				}

				// Find Same Edge: change 
				if( (eg->Start == nID[0] && eg->End == nID[1]) || 
					(eg->Start == nID[1] && eg->End == nID[2]) ||
					(eg->Start == nID[2] && eg->End == nID[0]) ){
				//		AfxMessageBox(_T(" Change and Save !"));
			            for(t = 0; t < 3; t++) {
			                tmp[t] = tr->nID[t];
		                }
			            tr->nID[0] = tmp[0];  
						tr->nID[1] = tmp[2];  
						tr->nID[2] = tmp[1];
			            tr->bNeedCheck = false;  break;
				}

				// Find Opposite Edge
				if( (eg->Start == nID[1] && eg->End == nID[0]) || 
					(eg->Start == nID[2] && eg->End == nID[1]) ||
					(eg->Start == nID[0] && eg->End == nID[2]) ){
					//	AfxMessageBox(_T(" Find Opposite Edge !"));
			            tr->bNeedCheck = false;  break;
				}
			}
		}

		// Update edge
		for(i = will_check.GetUpperBound(); i > -1; i--) { // Add new edges
			CTrgl* tr = will_check.GetAt(i);
			if(tr->bNeedCheck == false) {
				for(t = 0; t < 3; t++) {
			         nID[t] = tr->nID[t];
		        }
		        allEdge.Add(new CEdge(nID[0], nID[1]));
		        allEdge.Add(new CEdge(nID[1], nID[2]));
		        allEdge.Add(new CEdge(nID[2], nID[0]));

				// very important! Only Remove, not delete!
			    will_check.RemoveAt(i);
			    have_check.Add(tr); // add
			}
		}

		// Remove invalid
		for(i = 0; i < allEdge.GetSize(); i++) {
		   allEdge.GetAt(i)->UseTime = 1;
	    }
	    for(i = 0; i < allEdge.GetSize() - 1; i++) {
		     for(j = i+1; j < allEdge.GetSize(); j++) {
			    if( (allEdge.GetAt(i)->Start == allEdge.GetAt(j)->End) && 
				    (allEdge.GetAt(i)->End   == allEdge.GetAt(j)->Start)) {
					allEdge.GetAt(i)->UseTime++;
					allEdge.GetAt(j)->UseTime++;
			    }
		     }
	    }
	    for(i = allEdge.GetUpperBound(); i > -1; i--) { // Check
		    if(allEdge.GetAt(i)->UseTime > 2)  {
			    AfxMessageBox(_T("Error: Check()"));
		    }
		    if(allEdge.GetAt(i)->UseTime == 2) {
			    delete allEdge.GetAt(i);  allEdge.RemoveAt(i);
		    }
		}

		/*
		// Empty
		if(will_check.IsEmpty()) {
			bFinish = true;
		}
		else {
			bFinish = false;
		}
		*/
		
		// Empty
		if(will_check.GetSize() < 2) {
			bFinish = true;
		}
		else {
			bFinish = false;
		}
		
		
	}

	// for block
	have_check.RemoveAll();  
	will_check.RemoveAll();  
	allEdge.RemoveAll();

	for(i = 0; i < blk->pTrs.GetSize(); i++) {
		CTrgl* tr = blk->pTrs.GetAt(i);
		if(tr->bNeedCheck == false)  have_check.Add(tr);
		else  will_check.Add(tr);
	}

	// Special Case : have_check is empty
	if(have_check.IsEmpty() == true) {
		CTrgl* tr = will_check.GetAt(0);
		tr->bNeedCheck = false;
		have_check.Add(tr);
		will_check.RemoveAt(0); // Only remove, not delete
	}

	allEdge.RemoveAll();
	for(i = 0; i < have_check.GetSize(); i++) {
		for(t = 0; t < 3; t++) {
			nID[t] = have_check.GetAt(i)->nID[t];
		}
		allEdge.Add(new CEdge(nID[0], nID[1]));
		allEdge.Add(new CEdge(nID[1], nID[2]));
		allEdge.Add(new CEdge(nID[2], nID[0]));
	}
	for(i = 0; i < allEdge.GetSize(); i++) {
		allEdge.GetAt(i)->UseTime = 1;
	}
	for(i = 0; i < allEdge.GetSize() - 1; i++) {
		for(j = i+1; j < allEdge.GetSize(); j++) {
			if( (allEdge.GetAt(i)->Start == allEdge.GetAt(j)->End) && 
				(allEdge.GetAt(i)->End   == allEdge.GetAt(j)->Start)) {
					allEdge.GetAt(i)->UseTime++;
					allEdge.GetAt(j)->UseTime++;
			}
		}
	}
	for(i = allEdge.GetUpperBound(); i > -1; i--) { // Check
		if(allEdge.GetAt(i)->UseTime > 2)  {
			AfxMessageBox(_T("Error: Check()"));
		}

		if(allEdge.GetAt(i)->UseTime == 2) {
			delete allEdge.GetAt(i);  allEdge.RemoveAt(i);
		}
	}

	while(bFinish != true) {
		for(i = 0; i < allEdge.GetSize(); i++) { // Find
			CEdge* eg = allEdge.GetAt(i);
			for(j = 0; j < will_check.GetSize(); j++) {
				CTrgl* tr = will_check.GetAt(j);
				// Get IDs
				for(t = 0; t < 3; t++) {
					nID[t] = tr->nID[t];
				}

				// Find Same Edge: change 
				if( (eg->Start == nID[0] && eg->End == nID[1]) || 
					(eg->Start == nID[1] && eg->End == nID[2]) ||
					(eg->Start == nID[2] && eg->End == nID[0]) ){
				//		AfxMessageBox(_T(" Change and Save !"));
			            for(t = 0; t < 3; t++) {
			                tmp[t] = tr->nID[t];
		                }
			            tr->nID[0] = tmp[0];  
						tr->nID[1] = tmp[2];  
						tr->nID[2] = tmp[1];
			            tr->bNeedCheck = false;  break;
				}

				// Find Opposite Edge
				if( (eg->Start == nID[1] && eg->End == nID[0]) || 
					(eg->Start == nID[2] && eg->End == nID[1]) ||
					(eg->Start == nID[0] && eg->End == nID[2]) ){
					//	AfxMessageBox(_T(" Find Opposite Edge !"));
			            tr->bNeedCheck = false;  break;
				}
			}
		}

		// Update edge
		for(i = will_check.GetUpperBound(); i > -1; i--) { // Add new edges
			CTrgl* tr = will_check.GetAt(i);
			if(tr->bNeedCheck == false) {
				for(t = 0; t < 3; t++) {
			         nID[t] = tr->nID[t];
		        }
		        allEdge.Add(new CEdge(nID[0], nID[1]));
		        allEdge.Add(new CEdge(nID[1], nID[2]));
		        allEdge.Add(new CEdge(nID[2], nID[0]));

				// very important! Only Remove, not delete!
			    will_check.RemoveAt(i);
			    have_check.Add(tr); // add
			}
		}

		// Remove invalid
		for(i = 0; i < allEdge.GetSize(); i++) {
		   allEdge.GetAt(i)->UseTime = 1;
	    }
	    for(i = 0; i < allEdge.GetSize() - 1; i++) {
		     for(j = i+1; j < allEdge.GetSize(); j++) {
			    if( (allEdge.GetAt(i)->Start == allEdge.GetAt(j)->End) && 
				    (allEdge.GetAt(i)->End   == allEdge.GetAt(j)->Start)) {
					allEdge.GetAt(i)->UseTime++;
					allEdge.GetAt(j)->UseTime++;
			    }
		     }
	    }
	    for(i = allEdge.GetUpperBound(); i > -1; i--) { // Check
		    if(allEdge.GetAt(i)->UseTime > 2)  {
			    AfxMessageBox(_T("Error: Check()"));
		    }
		    if(allEdge.GetAt(i)->UseTime == 2) {
			    delete allEdge.GetAt(i);  allEdge.RemoveAt(i);
		    }
		}

		// Empty
		if(will_check.IsEmpty()) {
			bFinish = true;
		}
		else {
			bFinish = false;
		}
	}


}
	
void CInsect::Check_Loop(CSurf* surf, CBlock* blk)
{
	// for surface
	surf->pNds.RemoveAll();
	surf->pLps.RemoveAll();

	int i, j, k, t, tmp[3], nID[3];
	bool bFinish = false;

	CArray<CTrgl*, CTrgl*> have_check;
	CArray<CTrgl*, CTrgl*> will_check;
	CArray<CEdge*, CEdge*> allEdge;

	// for surface
	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		CTrgl* tr = surf->pTrs.GetAt(i);
		if(tr->bNeedCheck == false)  have_check.Add(tr);
		else  will_check.Add(tr);
	}

	allEdge.RemoveAll();
	for(i = 0; i < have_check.GetSize(); i++) {
		for(t = 0; t < 3; t++) {
			nID[t] = have_check.GetAt(i)->nID[t];
		}
		allEdge.Add(new CEdge(nID[0], nID[1]));
		allEdge.Add(new CEdge(nID[1], nID[2]));
		allEdge.Add(new CEdge(nID[2], nID[0]));
	}
	for(i = 0; i < allEdge.GetSize(); i++) {
		allEdge.GetAt(i)->UseTime = 1;
	}
	for(i = 0; i < allEdge.GetSize() - 1; i++) {
		for(j = i+1; j < allEdge.GetSize(); j++) {
			if( (allEdge.GetAt(i)->Start == allEdge.GetAt(j)->End) && 
				(allEdge.GetAt(i)->End   == allEdge.GetAt(j)->Start)) {
					allEdge.GetAt(i)->UseTime++;
					allEdge.GetAt(j)->UseTime++;
			}
		}
	}
	for(i = allEdge.GetUpperBound(); i > -1; i--) { // Check
		if(allEdge.GetAt(i)->UseTime > 2)  {
			AfxMessageBox(_T("Error: Check()"));
		}

		if(allEdge.GetAt(i)->UseTime == 2) {
			delete allEdge.GetAt(i);  allEdge.RemoveAt(i);
		}
	}

	while(bFinish != true) {
		for(i = 0; i < allEdge.GetSize(); i++) { // Find
			CEdge* eg = allEdge.GetAt(i);
			for(j = 0; j < will_check.GetSize(); j++) {
				CTrgl* tr = will_check.GetAt(j);
				// Get IDs
				for(t = 0; t < 3; t++) {
					nID[t] = tr->nID[t];
				}

				// Find Same Edge: change 
				if( (eg->Start == nID[0] && eg->End == nID[1]) || 
					(eg->Start == nID[1] && eg->End == nID[2]) ||
					(eg->Start == nID[2] && eg->End == nID[0]) ){
						AfxMessageBox(_T(" Change and Save !"));
			            for(t = 0; t < 3; t++) {
			                tmp[t] = tr->nID[t];
		                }
			            tr->nID[0] = tmp[0];  
						tr->nID[1] = tmp[2];  
						tr->nID[2] = tmp[1];
			            tr->bNeedCheck = false;  break;
				}

				// Find Opposite Edge
				if( (eg->Start == nID[1] && eg->End == nID[0]) || 
					(eg->Start == nID[2] && eg->End == nID[1]) ||
					(eg->Start == nID[0] && eg->End == nID[2]) ){
					//	AfxMessageBox(_T(" Find Opposite Edge !"));
			            tr->bNeedCheck = false;  break;
				}
			}
		}

		// Update edge
		for(i = will_check.GetUpperBound(); i > -1; i--) { // Add new edges
			CTrgl* tr = will_check.GetAt(i);
			if(tr->bNeedCheck == false) {
				for(t = 0; t < 3; t++) {
			         nID[t] = tr->nID[t];
		        }
		        allEdge.Add(new CEdge(nID[0], nID[1]));
		        allEdge.Add(new CEdge(nID[1], nID[2]));
		        allEdge.Add(new CEdge(nID[2], nID[0]));

				// very important! Only Remove, not delete!
			    will_check.RemoveAt(i);
			    have_check.Add(tr); // add
			}
		}

		// Remove invalid
		for(i = 0; i < allEdge.GetSize(); i++) {
		   allEdge.GetAt(i)->UseTime = 1;
	    }
	    for(i = 0; i < allEdge.GetSize() - 1; i++) {
		     for(j = i+1; j < allEdge.GetSize(); j++) {
			    if( (allEdge.GetAt(i)->Start == allEdge.GetAt(j)->End) && 
				    (allEdge.GetAt(i)->End   == allEdge.GetAt(j)->Start)) {
					allEdge.GetAt(i)->UseTime++;
					allEdge.GetAt(j)->UseTime++;
			    }
		     }
	    }
	    for(i = allEdge.GetUpperBound(); i > -1; i--) { // Check
		    if(allEdge.GetAt(i)->UseTime > 2)  {
			    AfxMessageBox(_T("Error: Check()"));
		    }
		    if(allEdge.GetAt(i)->UseTime == 2) {
			    delete allEdge.GetAt(i);  allEdge.RemoveAt(i);
		    }
		}

		// Empty
		if(will_check.IsEmpty()) {
			bFinish = true;
		}
		else {
			bFinish = false;
		}
	}


}

void CInsect::Check_Edge(CSurf* surf, CBlock* blk)
{
	int i, j, k, t, head, tail, nID[3], n, oldNum, newNum;

	for(i = 0; i < surf->pTrs.GetSize(); i++) {
		surf->pTrs.GetAt(i)->bUsed = false;
	}

	for(i = 0; i < m_aInsectLoop.GetSize(); i++) {
		CLoop* lp = m_aInsectLoop.GetAt(i);
		if(lp->flag == 2)  continue; 	// outer(0) or inner(1) or line(2)

		CSurf* insectSurf = new CSurf();
		CArray<CEdge*, CEdge*> tEdge;
		
		for(j = 0; j < lp->iNds.GetSize() - 1; j++) {
			head = lp->iNds.GetAt(j);
			tail = lp->iNds.GetAt(j+1);
			tEdge.Add(new CEdge(head, tail));
		}



			n = tEdge.GetSize();
			for(j = 0; j < n; j++) {
				tEdge.GetAt(j)->sPsdTrgl.RemoveAll();

				head = tEdge.GetAt(j)->Start;
				tail = tEdge.GetAt(j)->End;

				for(int jj = 0; jj < surf->pTrs.GetSize(); jj++) {
	
					for(t = 0; t < 3; t++) {
						nID[t] = surf->pTrs.GetAt(jj)->nID[t];
					}

					if((head == nID[0] && tail == nID[1]) ){
	
						tEdge.GetAt(j)->sPsdTrgl.Add(jj);
					}

					if((head == nID[1] && tail == nID[2]) ){
	                    tEdge.GetAt(j)->sPsdTrgl.Add(jj);
					}

					if((head == nID[2] && tail == nID[0]) ){
                        tEdge.GetAt(j)->sPsdTrgl.Add(jj);
					} // end if

				} // end for

				if(tEdge.GetAt(j)->sPsdTrgl.GetSize() == 2) {
					CString str;
					str.Format(_T("InsectLoop = %d \nEdge = %d"), i, j);
					AfxMessageBox(str);
				}
				
			} // end for

	}
	AfxMessageBox(_T("Check_Edge() is finished !"));


	
}

// Last Modified 2011.11.23
//judge if two nodes are same
// return TRUE if same, otherwise FALSE
// spere is smaller than cube and in it
bool CInsect::IsSameNode(CNode *nd1, CNode *nd2, double eps)
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
		AfxMessageBox(_T("Warning: IsSameNode()\n"));
		return true;
	}

	// Case 3 : not in cube
	if(bCube == false)  return false;
	
}

bool CInsect::IsSameNode(double ax, double ay, double az, double bx, double by, double bz, double eps)
{
	// Check error
	bool bCube = fabs(ax - bx) < eps && fabs(ay - by) < eps && fabs(az - bz) < eps ;
	bool bSphere = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by) + (az - bz) * (az - bz) ) < eps;
		                 
	// Case 1 : in sphere
	if(bSphere == true)  return true;

	// Case 2 : in cube while not in spere
	if(bSphere == false && bCube == true) {
		AfxMessageBox(_T("Warning: IsSameNode()\n"));
		return true;
	}

	// Case 3 : not in cube
	if(bCube == false)  return false;

}

// Determine whether a point in a arbitrary polygon
//  Globals which should be set before calling this function:
//
//  int    polySides  =  how many corners the polygon has
//  float  polyX[]    =  horizontal coordinates of corners
//  float  polyY[]    =  vertical coordinates of corners
//  float  x, y       =  point to be tested
//
//  (Globals are used in this example for purposes of speed.  Change as
//  desired.)
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.
bool CInsect::IsPointInPolygon(int polySides, double * polyX, double * polyY, double x, double y) 
{

  int i, j = polySides - 1 ;
  bool  oddNodes = false;

  for (i=0; i<polySides; i++) {
    if ((polyY[i]< y && polyY[j]>=y
    ||   polyY[j]< y && polyY[i]>=y)
    &&  (polyX[i]<=x || polyX[j]<=x)) {
      if (polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x) {
        oddNodes=!oddNodes; }
	}
    j=i;
  }

  return oddNodes; 
}