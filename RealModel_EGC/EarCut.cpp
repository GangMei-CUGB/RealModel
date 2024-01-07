#include "StdAfx.h"
#include "EarCut.h"
#define sqrEpsilon 0.000000001
#define Epsilon 0.00001
#define myEps 0.00001

CEarCut::CEarCut(void)
{
}


CEarCut::~CEarCut(void)
{
}

//三角形中，一边（x0,x1）所对的内角的余弦值
double CEarCut::AngleToASideInTrgl(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2)
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
double CEarCut::AngleOfVecters(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2)
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

//组堆
void CEarCut::rsift(double* p, int* ID, int i, int n)
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

void CEarCut::prhap(double* p, int* ID, int n)
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

// Last Modified 2011.11.23
//judge if two nodes are same
// return TRUE if same, otherwise FALSE
// spere is smaller than cube and in it
bool CEarCut::IsSameNode(CNode *nd1, CNode *nd2, double eps)
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
		AfxMessageBox(_T("Error: IsSameNode(CNode *nd1, CNode *nd2, double eps)"));
		return true;
	}

	// Case 3 : not in cube
	if(bCube == false)  return false;
	
}

bool CEarCut::IsSameNode(double ax, double ay, double az, double bx, double by, double bz, double eps)
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
		AfxMessageBox(_T("Error: IsSameNode(CNode *nd1, CNode *nd2, double eps)"));
		return true;
	}

	// Case 3 : not in cube
	if(bCube == false)  return false;
	
}

void CEarCut::PolygonPartion(CLoop * lp)
{
	int i, j, k, n;
	int * id, min, nID[3];
	double * angle;
	double x0, y0, z0, x1, y1, z1, x2, y2, z2;

	n = lp->pNds.GetSize();
	if(n < 3) { // Can't form a polygon
		AfxMessageBox(_T("Can't deposite polygon for points < 3! \n Function : PolygonPartion()."), MB_OK|MB_ICONSTOP);
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

void CEarCut::PolygonPartionExtended(CLoop * lp)
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

// 2011.5.25 by MeiGang
// to check whether a point is on a segment
bool CEarCut::IsPointOnSegment(double p0x, double p0y, double p1x, double p1y, double ptx, double pty, double eps)
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

// Test whether a Node is in a triangle or not. By Mei Gang 2009.10.9
// (xn, yn)  the Node to be tested
// x0, y0, x1, y1, x2, y2 the vertexes of triangle, they are supposed to lay on clockwise
// return value: 
// 0-Node is in triangle;  1-Node is on vertex;  2-Node is on edge;  3-Node is outside of the triangle
int CEarCut::IsNdInTrgl(double xn, double yn, double x0, double y0, double x1, double y1, double x2, double y2)
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

//矩阵求逆
void CEarCut::Dcinv(double *a, int n)
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

//2009.12.17 accepted from "RockModel"
//the surface equation of a trgl: Ax + By + Cz + D = 0
void CEarCut::EquaOfTrgl(double *x, double *y, double *z, double *Para)
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

void CEarCut::EquaOfTrgl(float *x, float *y, float *z, float *Para)
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

double CEarCut::VolOfTetra(double *x, double *y, double *z)
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

float CEarCut::VolOfTetra(float *x, float *y, float *z)
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

void CEarCut::NodeOnNormal(double *x, double *y, double *z, double eps)
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

void CEarCut::NodeOnNormal(float *x, float *y, float *z, float eps)
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

void CEarCut::GetNewCoordt(CTrgl *tr, CCordnt *NewCordt)
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

//2009.11.18 by Mei Gang
// 矢量叉积判断多边形顶点排列方向的方法
bool CEarCut::IsCCwize(CNode* p, int vcount) 
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

void CEarCut::PolygonPartion3D(CLoop * lp)
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

// Convert a polygon with a hole into a degerent polygon
// Original polygon: pOirg    Original Hole : pHole
// Resulted polygon: pOrig
void CEarCut::ConvertPolygonHole(CLoop* pOrig, CLoop* pHole) 
{
	int i, j, k, ndata;
	int nNds, nLps, flag;
	double x, y, z;
	int id_nds; 
	
	CArray<CLoop*, CLoop*>m_aLps;
	CArray<CPair*, CPair*>m_aDist;

	m_aLps.Add(pOrig);  m_aLps.Add(pHole);

	CLoop * polygon = new CLoop();
	for(i = 0; i < m_aLps.GetAt(0)->pNds.GetSize(); i++) {
		polygon->pNds.Add(m_aLps.GetAt(0)->pNds.GetAt(i));
	}
	
	for(i = 1; i < m_aLps.GetSize(); i++) {
		CLoop * hole = m_aLps.GetAt(i);

		// disttance
		double x1, y1, x2, y2;
		double * dist = new double[polygon->pNds.GetSize() * hole->pNds.GetSize()];
		for(j = 0; j < polygon->pNds.GetSize(); j++) {
			for(k = 0; k < hole->pNds.GetSize(); k++) {
				CPair * pr = new CPair();
				pr->FstID = j;  pr->SedID = k;
				if(polygon->pNds.GetAt(j)->flag == true && hole->pNds.GetAt(k)->flag == true) {
					pr->Flag = true;
				}
				else {
					pr->Flag = false;
				}
                m_aDist.Add(pr);

				x1 = polygon->pNds.GetAt(j)->x;
				y1 = polygon->pNds.GetAt(j)->y;
				x2 = hole->pNds.GetAt(k)->x;
				y2 = hole->pNds.GetAt(k)->y;

				dist[j*hole->pNds.GetSize()+k] = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
			}
		}

		// sort distance, find shortest
		int * ids = new int[m_aDist.GetSize()];
		for(j = 0; j < m_aDist.GetSize(); j++) {
			ids[j] = j;
		}
		prhap(dist, ids, m_aDist.GetSize());
		delete [] dist;

		// Create Bridge
		int index = 0;
		bool bfind = false, bintersect = false;
		int id_p0, id_d0, id_p1, id_d1;
		while(bfind != true) {
			if( m_aDist.GetAt(ids[index])->Flag == false) { 
				bfind = false;
				index++;
				continue;
			}

			id_p0 = m_aDist.GetAt(ids[index])->FstID;
			id_d0 = m_aDist.GetAt(ids[index])->SedID;
			x = polygon->pNds.GetAt(id_p0)->x;
			y = polygon->pNds.GetAt(id_p0)->y;
			z = polygon->pNds.GetAt(id_p0)->z;
			CNode * p0 = new CNode(id_p0, x, y, z);

			x = hole->pNds.GetAt(id_d0)->x - polygon->pNds.GetAt(id_p0)->x;
			y = hole->pNds.GetAt(id_d0)->y - polygon->pNds.GetAt(id_p0)->y;
			z = hole->pNds.GetAt(id_d0)->z - polygon->pNds.GetAt(id_p0)->z;
			CNode * d0 = new CNode(id_d0, x, y, z);
			
			bintersect = false;

			// for the boundary
			for(j = 0; j < polygon->pNds.GetSize(); j++) {
				if(j == polygon->pNds.GetSize()-1) {
					id_p1 = polygon->pNds.GetSize() - 1;
					id_d1 = 0;
				}
				else {
					id_p1 = j;
					id_d1 = j+1;
				}

				if(id_p0 == id_p1 || id_p0 == id_d1)  continue; // noticeable
				
				x = polygon->pNds.GetAt(id_p1)->x;
				y = polygon->pNds.GetAt(id_p1)->y;
				z = polygon->pNds.GetAt(id_p1)->z;
				CNode * p1 = new CNode(id_p1, x, y, z);
				
				x = polygon->pNds.GetAt(id_d1)->x - polygon->pNds.GetAt(id_p1)->x;
				y = polygon->pNds.GetAt(id_d1)->y - polygon->pNds.GetAt(id_p1)->y;
				z = polygon->pNds.GetAt(id_d1)->z - polygon->pNds.GetAt(id_p1)->z;
				CNode * d1 = new CNode(id_d1, x, y, z);
				
				CNode * i0 = new CNode();
				CNode * i1 = new CNode();
				
				if(IntrSegment2Segment2D(p0, d0, p1, d1, i0, i1) != 0 ) {
					bintersect = true;
					break;
				}
			}

			if(bintersect == true) {
				bfind = false;
				index++;
				continue;
			}

			// for the hole
			for(j = 0; j < hole->pNds.GetSize(); j++) {
				if(j == hole->pNds.GetSize()-1) {
					id_p1 = hole->pNds.GetSize() - 1;
					id_d1 = 0;
				}
				else {
					id_p1 = j;
					id_d1 = j+1;
				}

				if(id_d0 == id_p1 || id_d0 == id_d1)  continue; // noticeable
				
				x = hole->pNds.GetAt(id_p1)->x;
				y = hole->pNds.GetAt(id_p1)->y;
				z = hole->pNds.GetAt(id_p1)->z;
				CNode * p1 = new CNode(id_p1, x, y, z);
				
				x = hole->pNds.GetAt(id_d1)->x - hole->pNds.GetAt(id_p1)->x;
				y = hole->pNds.GetAt(id_d1)->y - hole->pNds.GetAt(id_p1)->y;
				z = hole->pNds.GetAt(id_d1)->z - hole->pNds.GetAt(id_p1)->z;
				CNode * d1 = new CNode(id_d1, x, y, z);
				
				CNode * i0 = new CNode();
				CNode * i1 = new CNode();
				
				if(IntrSegment2Segment2D(p0, d0, p1, d1, i0, i1) != 0 ) {
					bintersect = true;
					break;
				}
			}
			if(bintersect == true) {
				bfind = false;
				index++;
				continue;
			}

			bfind = true;
		}

		// Combine
		CLoop * tmp = new CLoop();
		int fst, lst;
		fst = m_aDist.GetAt(ids[index])->FstID;
		lst = m_aDist.GetAt(ids[index])->SedID;

		polygon->pNds.GetAt(fst)->flag = false;
		hole->pNds.GetAt(lst)->flag = false;

		//// common case
		for(k = 0; k <= fst; k++) { // 1-boundary
			tmp->pNds.Add(polygon->pNds.GetAt(k));
		}
		for(k = lst; k < hole->pNds.GetSize(); k++) { // 2-hole
			tmp->pNds.Add(hole->pNds.GetAt(k));
		}

		// Check whether Head == Tail, two cases!
		int N = hole->pNds.GetSize();
		if( (fabs(hole->pNds.GetAt(0)->x - hole->pNds.GetAt(N-1)->x) < myEps) &&
			(fabs(hole->pNds.GetAt(0)->y - hole->pNds.GetAt(N-1)->y) < myEps) ) {
			for(k = 1; k <= lst; k++) { // 3-hole, discard 1st Node
		        tmp->pNds.Add(hole->pNds.GetAt(k));
		    }
		}
		else {
		    for(k = 0; k <= lst; k++) { // 3-hole, keep 1st Node
			    tmp->pNds.Add(hole->pNds.GetAt(k));
	    	}
		}

		for(k = fst; k < polygon->pNds.GetSize(); k++) { // 4-boundary
			tmp->pNds.Add(polygon->pNds.GetAt(k));
		}

		// update
		pOrig->pNds.RemoveAll();
		for(k = 0; k < tmp->pNds.GetSize(); k++) {
			pOrig->pNds.Add(tmp->pNds.GetAt(k));
		}
		tmp->pNds.RemoveAll(); // Only remove, not delete
		polygon->pNds.RemoveAll(); // Only remove, not delete

		delete [] ids;
		for(k = m_aDist.GetSize()-1; k > -1; k--) {
			delete m_aDist.GetAt(k);  m_aDist.RemoveAt(k);
		}
		m_aDist.RemoveAll();
	}
	
}

//To compute the intersecton of segment-segment
//Return value:
//0-Segments do not intersect;
//1-Segments intersect on a point;
//2-Segments intersect on a interval;
int CEarCut::IntrSegment2Segment2D(CNode * p0, CNode * d0, CNode * p1, CNode * d1, CNode * i0, CNode * i1)
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
			int imax = FindIntersection(0.0, 1.0, smin, smax, w);
			
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
int CEarCut::FindIntersection(double u0, double u1, double v0, double v1, double w[2])
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