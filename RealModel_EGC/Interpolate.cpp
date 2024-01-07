#include "StdAfx.h"
#include "Interpolate.h"



CInterpolate::CInterpolate(void)
{
}


CInterpolate::~CInterpolate(void)
{
}

// Matrix inversion
int CInterpolate::Dcinv(float *a, int n)
{
	int *is,*js,i,j,k,l,u,v;
    float d,p;
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
            return(0);
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
    return(1);

}

int cmp( const void *a , const void *b )  
{  
    return *(float *)a > *(float *)b ? 1 : -1;  
}  

// Universal Kriging
// dp: data points
// ip: interpolation points
void CInterpolate::UniversalKriging(float *dpx, float *dpy, float *dpz, int ndp, float *ipx, float *ipy, float *ipz, int nip)
{
    int i, j, k;
	int LM = 10; // Group
	float DR1, DZ;
	float a[3]; //´æ·ÅÇòÄ£ÐÍµÄÈý¸öÏµÊý
	
	float *p, *A, *B, *C; //¾ØÕó
	int *LD = new int[LM]; 
	float *AR = new float[LM];
	float *AZ = new float[LM];
	float *DR = new float[LM + 1];
	
	p = new float[(ndp - 1) * ndp / 2]; //¾àÀëÁÐÕó
	A = new float[(ndp + 3) * (ndp + 3)]; //ÏµÊý¾ØÕó 
	for(i = 0; i < (ndp + 3) * (ndp + 3); i++) { A[i] = 0.0; }
	
	//¼ÆËã¾àÀë
	int NumOfDis = 0;
	for(i = 0; i < ndp - 1; i++) {
		for(j = i + 1; j < ndp; j++) {
			p[NumOfDis] = sqrt((dpx[i] - dpx[j]) * (dpx[i] - dpx[j]) +
				               (dpy[i] - dpy[j]) * (dpy[i] - dpy[j]));
			A[i * (ndp + 3) + j] = p[NumOfDis]; //¼ÇÂ¼¾àÀë¾ØÕóµÄÉÏ°ë²¿·Ö
			A[j * (ndp + 3) + i] = p[NumOfDis]; //¼ÇÂ¼¾àÀë¾ØÕóµÄÏÂ°ë²¿·Ö
			NumOfDis++;
		}
	}
	
	// Sort
	qsort(p, NumOfDis, sizeof(float), cmp);	 
	
	//°´¾àÀë·Ö×é
	DR[0] = 0.0;
	for(i = 1; i <= LM; i++) {
		k = int(i * NumOfDis / LM) - 1;
		DR[i] = p[k];
	}
	
	//¼ÆËã·½²î
	for(i = 0; i < LM; i++) {
		LD[i] = 0;  AR[i] = 0.0;  AZ[i] = 0.0;
	}
	for(i = 0; i < ndp - 1; i++) {
		for(j = i + 1; j < ndp; j++) {
			DR1 = sqrt( pow((dpx[i] - dpx[j]), 2) + pow((dpy[i] - dpy[j]), 2) ); //¾àÀë
			DZ = pow((dpz[i] - dpz[j]), 2); //Æ½·½ÔöÁ¿
			for(k = 1; k <= LM; k++) {
				if(DR1 > DR[k - 1] && DR1 <= DR[k]) {  
					LD[k-1] += 1; //µã¸öÊý
					AR[k-1] += DR1; //Ã¿×é¾àÀëºÍ
					AZ[k-1] += DZ;  //Ã¿×éÆ½·½ÔöÁ¿ºÍ
				}
			}
		}
	}
	
    //¶þÔªÏßÐÔ»Ø¹éÖÐ·¨·½³ÌµÄÏµÊý£¬±äÁ¿
	//L11 * X1 + L12 * X2 = L10
	//L21 * X1 + L22 * X2 = L20
	float X1 = 0.0;    float X2=0.0;       float Y = 0.0;	 
	float L11 = 0.0;   float L12 = 0.0;    float L21 = 0.0;  
	float L22 = 0.0;   float L10 = 0.0;    float L20 = 0.0;
	for(i = 0; i < LM; i++) {
		AR[i] = (AR[i] / LD[i]); //¾àÀëÆ½¾ùÖµ
		AZ[i] = (AZ[i] / LD[i]/2); //Æ½·½ÔöÁ¿ºÍ
		X1 += AR[i] / LM; 
        X2 += pow(AR[i],3) / LM;
		Y += AZ[i] / LM; 
	}	
	for(i = 0; i < LM; i++) {
		L10 += (AR[i] - X1) * (AZ[i] - Y);
		L11 += (AR[i] - X1) * (AR[i] - X1);
		L12 += (AR[i] - X1) *(pow(AR[i],3) -X2 );
		L20 += (pow(AR[i],3) -X2 ) * (AZ[i] - Y);
		L21 += (pow(AR[i],3) -X2 ) * (AR[i] - X1);
		L22 += (pow(AR[i],3) -X2 ) * (pow(AR[i],3) -X2 );
	}

	//Çò×´Ä£ÐÍÏµÊý
	float B1 = ( L10*L22-L20*L12) / (L11*L22-L12*L12);
	float B2 = ( L20*L11-L10*L21) / (L11*L22-L12*L12);
	float B0 = Y - B1 * X1 - B2 * X2;
	if(B0 > 0) {
		a[0] = B0;
		a[1] = sqrt(-1 * B1/3/B2);
		a[2] = 2 * B1 / 3 * sqrt(-1 * B1 / 3 / B2);
	}
	delete LD;    delete AR;   delete AZ;    delete DR; 

	//¼ÆËã±ä²îº¯ÊýÏµÊý¾ØÕó
	for(i = 0; i < ndp; i++) {
		for(j = 0; j < ndp; j++) {
			if(i == j) A[i * (ndp + 3) + j] = 0.0;
			else {
				if(A[i * (ndp + 3) + j] < a[1]) { //Ð¡ÓÚ±ä³Ì
					A[i * (ndp + 3) + j] = a[0] + a[2] * (1.5 * A[i * (ndp + 3) + j]
						                  /a[1] - 0.5 * pow(A[i * (ndp + 3) + j] / a[1], 3));
				}
				else   A[i * (ndp + 3) + j] = a[0] + a[2]; //´óÓÚ»òµÈÓÚ±ä³Ì
			}
		}	
	}
	for(i = 0; i < ndp; i++) { //¶ÔÃ¿¸öÊý¾ÝµãÓÉÆ¯ÒÆ´øÀ´µÄ3¸öÏî
		A[(i + 1) * (ndp + 3) - 3] = 1.0;
		A[(i + 1) * (ndp + 3) - 2] = dpx[i];
		A[(i + 1) * (ndp + 3) - 1] = dpy[i];
        A[(ndp) * (ndp + 3) + i] = 1.0; 
		A[(ndp + 1) * (ndp + 3) + i] = dpx[i]; 
		A[(ndp + 2) * (ndp + 3) + i] = dpy[i];
	}	
	
	Dcinv(A, ndp + 3); //ÏµÊý¾ØÕóÇóÄæ
	
	B = new float[ndp + 3]; //´æ·Å¾ØÕó·½³Ì×éµÈºÅÓÒ±ßÁÐÏòÁ¿
	C = new float[ndp + 3]; //´æ·Å½á¹ûÏòÁ¿

	//Î´ÖªµãÑ­»·
	for(i = 0; i < nip; i++) {
		for(j = 0; j < ndp; j++) {
			DR1 = sqrt( pow((ipx[i] - dpx[j]),2) + pow((ipy[i] - dpy[j]),2) );
			if(DR1 < 0.00000000001) {
				B[j] = 0.0; 
			}               
			else {
				if(DR1 < a[1])
					B[j] = a[0]+a[2]*(1.5*DR1/a[1]-0.5*pow(DR1/a[1],3));
				else   B[j] = a[0]+a[2];
			}
		}
		
		B[ndp] = 1.0; //ÓÉÆ¯ÒÆ´øÀ´µÄ3¸öÏî
		B[ndp+1] = ipx[i];
		B[ndp+2] = ipy[i];
		
		for(k = 0; k < ndp + 3; k++) { //²åÖµ·½³Ì×éµÄ½â
			C[k] = 0.0;
			for(j = 0; j < ndp + 3; j++) {
				C[k] += A[k * (ndp + 3) + j] * B[j];
			}
		}
		
		ipz[i] = 0.0;
		for(j = 0; j < ndp; j++) {
			ipz[i] += C[j] * dpz[j]; //·º¿ËÁ¢¸ñ¹ÀÖµ
		}
	}
	
	delete [] p;  delete [] A;  delete [] B;  delete [] C;

}


// Universal Kriging STL
// vector<Point> &dP : data points
// vector<Point> &iP : interpolation points
void CInterpolate::UniversalKriging_STL(vector<Point> &dP, vector<Point> &iP)
{
	int ndp = dP.size();
	int nip = iP.size();

    int i, j, k;

	int G = 10; // Groups for dividing distances
	int *LD = new int[G]; 
	float DR1, DZ;
	float a[3]; // Coefficients of Spherical model
	float *AR = new float[G];
	float *AZ = new float[G];
	float *DR = new float[G + 1];
	
	// Matrices
	float *p, *A, *B, *C; 
	p = new float[(ndp - 1) * ndp / 2]; // Distance matrix
	A = new float[(ndp + 3) * (ndp + 3)]; // Coefficient matrix
	B = new float[(ndp + 3)];
	C = new float[(ndp + 3)]; // Weights
	for(i = 0; i < (ndp + 3) * (ndp + 3); i++) { A[i] = 0.0; }

	// Distance
	int NumOfDis = 0;
	for(i = 0; i < ndp - 1; i++) {
		for(j = i + 1; j < ndp; j++) {
			p[NumOfDis] = sqrt( pow(dP[i].x - dP[j].x, 2) +
				                pow(dP[i].y - dP[j].y, 2) );
			A[i * (ndp + 3) + j] = p[NumOfDis]; // Upper part
			A[j * (ndp + 3) + i] = p[NumOfDis]; // Lower part
			NumOfDis++;
		}
	}
	
	// Sort
	qsort(p, NumOfDis, sizeof(float), cmp);

	// Divide all distances
	for(i = 1; i <= G; i++) {
		k = int(i * NumOfDis / G) - 1;
		DR[i] = p[k];
	}

	// Calculate sample semivariogram
	for(i = 0; i < G; i++) {
		LD[i] = 0;  AR[i] = 0.0;  AZ[i] = 0.0;
	}
	for(i = 0; i < ndp - 1; i++) {
		for(j = i + 1; j < ndp; j++) {
			DR1 = sqrt( pow((dP[i].x - dP[j].x), 2) + 
			        	pow((dP[i].y - dP[j].y), 2) );
			DZ = pow((dP[i].z - dP[j].z), 2); 
			for(k = 1; k <= G; k++) {
				if(DR1 > DR[k - 1] && DR1 <= DR[k]) {  
					LD[k-1] += 1; 
					AR[k-1] += DR1; 
					AZ[k-1] += DZ;  
				}
			}
		}
	}
	
    // Binary linear regression
	// L11 * X1 + L12 * X2 = L10
	// L21 * X1 + L22 * X2 = L20
	float X1 = 0.0;    float X2=0.0;       float Y = 0.0;	 
	float L11 = 0.0;   float L12 = 0.0;    float L21 = 0.0;  
	float L22 = 0.0;   float L10 = 0.0;    float L20 = 0.0;

	for(i = 0; i < G; i++) {
		AR[i] = (AR[i] / LD[i]); 
		AZ[i] = (AZ[i] / LD[i]/2); 
		X1 += AR[i] / G; 
        X2 += pow(AR[i],3) / G;
		Y += AZ[i] / G; 
	}	
	for(i = 0; i < G; i++) {
		L10 += (AR[i] - X1) * (AZ[i] - Y);
		L11 += (AR[i] - X1) * (AR[i] - X1);
		L12 += (AR[i] - X1) *(pow(AR[i],3) -X2 );
		L20 += (pow(AR[i],3) -X2 ) * (AZ[i] - Y);
		L21 += (pow(AR[i],3) -X2 ) * (AR[i] - X1);
		L22 += (pow(AR[i],3) -X2 ) * (pow(AR[i],3) -X2 );
	}

	// Coefficients of Spherical model
	float B1 = ( L10*L22-L20*L12) / (L11*L22-L12*L12);
	float B2 = ( L20*L11-L10*L21) / (L11*L22-L12*L12);
	float B0 = Y - B1 * X1 - B2 * X2;
	if(B0 > 0) {
		a[0] = B0;
		a[1] = sqrt(-1 * B1/3/B2);
		a[2] = 2 * B1 / 3 * sqrt(-1 * B1 / 3 / B2);
	}
	delete [] LD;  delete [] AR;  delete [] AZ;  delete [] DR;

	// Coefficient matrix of semivariogram
	for(i = 0; i < ndp; i++) {
		for(j = 0; j < ndp; j++) {
			if(i == j) A[i * (ndp + 3) + j] = 0.0;
			else {
				if(A[i * (ndp + 3) + j] < a[1]) { // Within the Range
					A[i * (ndp + 3) + j] = a[0] + a[2] * (1.5 * A[i * (ndp + 3) + j]
						                  /a[1] - 0.5 * pow(A[i * (ndp + 3) + j] / a[1], 3));
				}
				else A[i * (ndp + 3) + j] = a[0] + a[2]; // Outside
			}
		}	
	}
	for(i = 0; i < ndp; i++) { 
		A[(i + 1) * (ndp + 3) - 3] = 1.0;
		A[(i + 1) * (ndp + 3) - 2] = dP[i].x;
		A[(i + 1) * (ndp + 3) - 1] = dP[i].y;
        A[(ndp) * (ndp + 3) + i] = 1.0; 
		A[(ndp + 1) * (ndp + 3) + i] = dP[i].x; 
		A[(ndp + 2) * (ndp + 3) + i] = dP[i].y;
	}	
	
	Dcinv(A, ndp + 3); // Matrix inversion
	
	// Estimate each unknown point
	for(i = 0; i < nip; i++) {
		for(j = 0; j < ndp; j++) {
			DR1 = sqrt( pow((iP[i].x - dP[j].x),2) + 
				        pow((iP[i].y - dP[j].y),2) );
			if(DR1 < 1.0e-8) {
				B[j] = 0.0; 
			}               
			else {
				if(DR1 < a[1])
					B[j] = a[0]+a[2]*(1.5*DR1/a[1]-0.5*pow(DR1/a[1],3));
				else   B[j] = a[0]+a[2];
			}
		}
		
		B[ndp] = 1.0; // Drift / trend
		B[ndp+1] = iP[i].x;
		B[ndp+2] = iP[i].y;
		
		// All weights
		for(k = 0; k < ndp + 3; k++) { 
			C[k] = 0.0;
			for(j = 0; j < ndp + 3; j++) {
				C[k] += A[k * (ndp + 3) + j] * B[j];
			}
		}
		
		iP[i].z = 0.0;
		for(j = 0; j < ndp; j++) { // Linear weighted sum
			iP[i].z += C[j] * dP[j].z; 
		}
	}
	
	delete [] p;  delete [] A;  delete [] B;  delete [] C;

}


// vector<Point> &dP : data points
// vector<Point> &iP : interpolation points
// float p : power
void CInterpolate::IDW_STL(vector<Point> &dP, vector<Point> &iP, float p)
{
#define eps 1.0e-7
	int i, j, mark;
	vector<float> w(dP.size(), 0.0);  // weights

	for ( i = 0; i < iP.size(); i++ ) {
		if ( fabs(p) < eps ) { // Average
			for ( j = 0; j < dP.size(); j++ ) {
				w[j] = 1.0 / dP.size();
			}
		}
		else {
			mark = -1;
			for ( j = 0; j < dP.size(); j++ ) {
				// Distance
				w[j] = sqrt( pow(iP[i].x - dP[j].x, 2.0f) +
					         pow(iP[i].y - dP[j].y, 2.0f) );
				if ( fabs(w[j]) < eps ) {
					mark = j;  break;
				}
			}

			if ( mark != -1 ) {
				for ( j = 0; j < dP.size(); j++ ) {
					w[j] = 0.0;
				}
				w[mark] = 1.0;
			}
			else {
				float sum = 0.0;
				for ( j = 0; j < dP.size(); j++ ) { // Sum
					w[j] = 1.0 / pow ( w[j], p ); // Inverse distance
					sum += w[j];
				}
				for ( j = 0; j < dP.size(); j++ ) { // inverse distance weights
					w[j] = w[j] / sum;
				}
			}
		}

		// Value
		iP[i].z = 0.0;
		for(j = 0; j < dP.size(); j++) {
			iP[i].z += w[j] * dP[j].z;
		}
	}

	w.clear();

}

// vector<Point> &dP : data points
// vector<Point> &iP : interpolation points
// float p : power
void CInterpolate::IDW_STL(vector<CNode*> &dP, vector<CNode*> &iP, float p)
{
#ifndef EGC

#define eps 1.0e-7
	int i, j, mark;
	vector<float> w(dP.size(), 0.0);  // weights

	for ( i = 0; i < iP.size(); i++ ) {
		if ( fabs(p) < eps ) { // Average
			for ( j = 0; j < dP.size(); j++ ) {
				w[j] = 1.0 / dP.size();
			}
		}
		else {
			mark = -1;
			for ( j = 0; j < dP.size(); j++ ) {
				// Distance
				w[j] = sqrt( pow(iP[i]->x - dP[j]->x, 2.0) +
					         pow(iP[i]->y - dP[j]->y, 2.0) );
				if ( fabs(w[j]) < eps ) {
					mark = j;  break;
				}
			}
			
			if ( mark != -1 ) {
				for ( j = 0; j < dP.size(); j++ ) {
					w[j] = 0.0;
				}
				w[mark] = 1.0;
			}
			else {
				float sum = 0.0;
				for ( j = 0; j < dP.size(); j++ ) { // Sum
					w[j] = 1.0 / pow ( w[j], p ); // Inverse distance
					sum += w[j];
				}
				for ( j = 0; j < dP.size(); j++ ) { // inverse distance weights
					w[j] = w[j] / sum;
				}
			}
		}

		// Value
		iP[i]->z = 0.0;
		for(j = 0; j < dP.size(); j++) {
			iP[i]->z += w[j] * dP[j]->z;
		}
	}

	w.clear();

#else

#endif

}

// newly by Mei Gang. 2010. 7. 6
// dp: data point - dp
// ip: interpolated point - ip
void CInterpolate::OrdinaryKriging(float *dpx, float *dpy, float *dpz, int ndp, float *ipx, float *ipy, float *ipz, int nip)
{
    int i, j, k;
	int LM = 10; //¾àÀë·Ö×éÊ±µÄ×éÊý
	float DR1, DZ;
	float a[3]; //´æ·ÅÇòÄ£ÐÍµÄÈý¸öÏµÊý
	
	float *p, *A, *B, *C; //¾ØÕó
	int *LD = new int[LM]; 
	float *AR = new float[LM];
	float *AZ = new float[LM];
	float *DR = new float[LM + 1];

	p = new float[(ndp - 1) * ndp / 2]; //¾àÀëÁÐÕó
    A = new float[(ndp + 1) * (ndp + 1)]; //ÏµÊý¾ØÕó 
	for(i = 0; i < (ndp + 1) * (ndp + 1); i++) { A[i] = 0.0; }
	
	// Distance
	int NumOfDis = 0;
	for(i = 0; i < ndp - 1; i++) {
		for(j = i + 1; j < ndp; j++) {
			p[NumOfDis] = sqrt((dpx[i] - dpx[j]) * (dpx[i] - dpx[j]) +
				             (dpy[i] - dpy[j]) * (dpy[i] - dpy[j]));
			A[i * (ndp + 1) + j] = p[NumOfDis]; //¼ÇÂ¼¾àÀë¾ØÕóµÄÉÏ°ë²¿·Ö
			A[j * (ndp + 1) + i] = p[NumOfDis]; //¼ÇÂ¼¾àÀë¾ØÕóµÄÏÂ°ë²¿·Ö
			NumOfDis++;
		}
	}
	
	// Sort
	qsort(p, NumOfDis, sizeof(float), cmp);	 
	
	//°´¾àÀë·Ö×é
	DR[0] = 0.0;
	for(i = 1; i <= LM; i++) {
		k = int(i * NumOfDis / LM) - 1;
		DR[i] = p[k];
	}
	
	//¼ÆËã·½²î
	for(i = 0; i < LM; i++) { LD[i] = 0;  AR[i] = 0.0;  AZ[i] = 0.0; }
	for(i = 0; i < ndp - 1; i++) {
		for(j = i + 1; j < ndp; j++) {
			DR1 = sqrt( pow((dpx[i] - dpx[j]), 2) + pow((dpy[i] - dpy[j]), 2) ); //¾àÀë
			DZ = pow((dpz[i] - dpz[j]), 2); //Æ½·½ÔöÁ¿
			for(k = 1; k <= LM; k++) {
				if(DR1 > DR[k - 1] && DR1 <= DR[k]) {  
					LD[k-1] += 1; //µã¸öÊý
					AR[k-1] += DR1; //Ã¿×é¾àÀëºÍ
					AZ[k-1] += DZ;  //Ã¿×éÆ½·½ÔöÁ¿ºÍ
				}
			}
		}
	}
	
    //¶þÔªÏßÐÔ»Ø¹éÖÐ·¨·½³ÌµÄÏµÊý£¬±äÁ¿
	//L11 * X1 + L12 * X2 = L10
	//L21 * X1 + L22 * X2 = L20
	float X1 = 0.0;    float X2=0.0;      float Y = 0.0;	 
	float L11 = 0.0;   float L12 = 0.0;    float L21 = 0.0;  
	float L22 = 0.0;   float L10 = 0.0;    float L20 = 0.0;
	for(i = 0; i < LM; i++) {
		AR[i] = (AR[i] / LD[i]); //¾àÀëÆ½¾ùÖµ
		AZ[i] = (AZ[i] / LD[i]/2); //Æ½·½ÔöÁ¿ºÍ
		X1 += AR[i] / LM;   X2 += pow(AR[i],3) / LM;  Y += AZ[i] / LM;
	}	
	for(i = 0; i < LM; i++) {
		L10 += (AR[i] - X1) * (AZ[i] - Y);
		L11 += (AR[i] - X1) * (AR[i] - X1);
		L12 += (AR[i] - X1) *(pow(AR[i],3) -X2 );
		L20 += (pow(AR[i],3) -X2 ) * (AZ[i] - Y);
		L21 += (pow(AR[i],3) -X2 ) * (AR[i] - X1);
		L22 += (pow(AR[i],3) -X2 ) * (pow(AR[i],3) -X2 );
	}

	//Çò×´Ä£ÐÍÏµÊý
	float B1 = ( L10*L22-L20*L12) / (L11*L22-L12*L12);
	float B2 = ( L20*L11-L10*L21) / (L11*L22-L12*L12);
	float B0 = Y - B1 * X1 - B2 * X2;
	if(B0 > 0) {
		a[0] = B0;  a[1] = sqrt(-1 * B1/3/B2);  a[2] = 2 * B1 / 3 * sqrt(-1 * B1 / 3 / B2);
	}
	delete LD;    delete AR;   delete AZ;    delete DR; 

	//¼ÆËã±ä²îº¯ÊýÏµÊý¾ØÕó
	for(i = 0; i < ndp; i++) {
		for(j = 0; j < ndp; j++) {
			if(i == j) A[i * (ndp + 1) + j] = 0.0;
			else {
				if(A[i * (ndp + 1) + j] < a[1]) { //Ð¡ÓÚ±ä³Ì
					A[i * (ndp + 1) + j] = a[0] + a[2] * (1.5 * A[i * (ndp + 1) + j]
						             /a[1] - 0.5 * pow(A[i * (ndp + 1) + j] / a[1], 3));
				}
				else   A[i * (ndp + 1) + j] = a[0] + a[2]; //´óÓÚ»òµÈÓÚ±ä³Ì
			}
		}	
	}
	for(i = 0; i < ndp; i++) { //¶ÔÃ¿¸öÊý¾Ýµã¼ÓÈëÖµÎª1µÄÏî
		A[(i + 1) * (ndp + 1) - 1] = 1.0;  A[(ndp) * (ndp + 1) + i] = 1.0;
	}	

	Dcinv(A, ndp + 1); //ÏµÊý¾ØÕóÇóÄæ

	B = new float[ndp + 1]; //´æ·Å¾ØÕó·½³Ì×éµÈºÅÓÒ±ßÁÐÏòÁ¿
	C = new float[ndp + 1]; //´æ·Å½á¹ûÏòÁ¿

	//Î´ÖªµãÑ­»·
	for(i = 0; i < nip; i++) {
		for(j = 0; j < ndp; j++) {
			DR1 = sqrt( pow((ipx[i] - dpx[j]),2) + pow((ipy[i] - dpy[j]),2) );
			if(DR1 < 0.00000000001) { B[j] = 0.0; }
			else {
				if(DR1 < a[1])  B[j] = a[0]+a[2]*(1.5*DR1/a[1]-0.5*pow(DR1/a[1],3));
				else  B[j] = a[0]+a[2];
			}
		}
		
		B[ndp] = 1.0; //¼ÓÈëÖµÎª1µÄÏî

		for(k = 0; k < ndp + 1; k++) { //²åÖµ·½³Ì×éµÄ½â
			C[k] = 0.0;
			for(j = 0; j < ndp + 1; j++) { C[k] += A[k * (ndp + 1) + j] * B[j]; }
		}
		ipz[i] = 0.0;
		for(j = 0; j < ndp; j++) {
			ipz[i] += C[j] * dpz[j]; //ÆÕÍ¨¿ËÁ¢¸ñ¹ÀÖµ
		}
	}
	delete [] p;  delete [] A;  delete [] B;  delete [] C;
}