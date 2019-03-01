/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'accès public -           
	                  version 5.0 - Dec.2012                                           
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#else
	#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <g3x_quaternions.h>

extern void g3x_QuatIdentity(G3Xquat A) 
{
	memset(&A,0,sizeof(G3Xquat));
  A.r = 1.0;
}

extern G3Xquat g3x_QuatSet(double r, G3Xvector v) 
{
  G3Xquat A;
	A.r = r;
	G3Xcopy(A.v,v);
	return A;
}

extern G3Xquat g3x_QuatSet4(double r, double x, double y, double z) 
{
  G3Xquat A;
  A.r = r;
	G3Xset(A.v,x,y,z);
	return A;
}

extern G3Xquat g3x_QuatAdd(G3Xquat A, G3Xquat B) 
{
	A.r += B.r;
	G3Xaddvect(A.v,B.v);
	return A;
}

extern G3Xquat g3x_QuatProd(G3Xquat A, G3Xquat B) 
{
  G3Xquat C;
	C.r = A.r*B.r - G3Xprodscal(A.v,B.v);
	G3Xvector w;
	G3Xprodvect(w,A.v,B.v);
	C.v[0] = A.r*B.v[0] + B.r*A.v[0] + w[0];
	C.v[1] = A.r*B.v[1] + B.r*A.v[1] + w[1];
	C.v[2] = A.r*B.v[2] + B.r*A.v[2] + w[2];
	return C;
}

extern G3Xquat g3x_QuatConj(G3Xquat A) 
{
	A.v[0]=-A.v[0];
	A.v[1]=-A.v[1];
	A.v[2]=-A.v[2];
	return A;
}

extern double g3x_QuatSqrNorm(G3Xquat A) 
{
	return SQR(A.r)+G3Xsqrnorm(A.v);
}

extern double g3x_QuatNorm(G3Xquat A) 
{
	return sqrt(SQR(A.r)+G3Xsqrnorm(A.v));
}

extern G3Xquat g3x_QuatNormalize(G3Xquat A) 
{
	double  n=g3x_QuatSqrNorm(A);
	if (G3Xiszero(n)) { memset(&A,0,sizeof(G3Xquat)); return A; }
	n = 1./sqrt(n);
	A.r   *=n;
	G3Xmulvect(A.v,n);
	return A;
}

extern G3Xquat g3x_QuatScalMap(G3Xquat A, double a) 
{
	A.r   *=a;
	G3Xmulvect(A.v,a);
	return A;
}


extern G3Xquat g3x_QuatInv(G3Xquat A) 
{
	double n = g3x_QuatNorm(A);
	if (G3Xiszero(n)) { memset(&A,0,sizeof(G3Xquat)); return A; }
	A.v[0]=-A.v[0];
	A.v[1]=-A.v[1];
	A.v[2]=-A.v[2];
	g3x_QuatScalMap(A,1./n);
	return A;
}


extern void g3x_QuatToHmat(G3Xquat A, G3Xhmat M)
{
	M[a00]= A.r; 	  M[a01]=-A.v[0]; M[a02]=-A.v[1]; M[a03]=-A.v[2];
	M[a10]=+A.v[0]; M[a11]= A.r; 	  M[a12]=-A.v[2]; M[a13]=+A.v[1];
	M[a20]=+A.v[1]; M[a21]=+A.v[2]; M[a22]= A.r;    M[a23]=-A.v[0];
	M[a30]=+A.v[2]; M[a33]=-A.v[1]; M[a32]=+A.v[0]; M[a33]= A.r;
}

extern void g3x_QuatPrint(G3Xquat A) 
{
  fprintf(stderr,"Quaternion : [%f|(%f,%f,%f)] ||Q||=%lf\t\r",A.r,A.v[0],A.v[1],A.v[2],g3x_QuatNorm(A));
}


extern void g3x_QuatRot(G3Xquat Qrot, G3Xcoord src, G3Xcoord dest)
{
	G3Xquat Qsrc = g3x_QuatSet(0,src);
	G3Xquat Qdst;
	
	Qdst = g3x_QuatProd(Qrot,Qsrc);
	G3Xmulvect(Qrot.v,-1.);
	Qdst = g3x_QuatProd(Qdst,Qrot);
	G3Xcopy(dest,Qdst.v);	
}

extern void g3x_AxeRadRot(G3Xvector v, double rad, G3Xcoord src, G3Xcoord dest)
{
	G3Xvector u;
	G3Xcopy(u,v);
	G3Xnormalize(u);/* direction normée */
	double c=cos(0.5*rad);
	double s=sin(0.5*rad);
	G3Xmulvect(u,s);
	G3Xquat Qrot = g3x_QuatSet(c,u);
	G3Xquat Qsrc = g3x_QuatSet(0,src);
	G3Xquat Qdst;
	
	Qdst = g3x_QuatProd(Qrot,Qsrc);
	G3Xmulvect(u,-1.);
	Qrot = g3x_QuatSet(c,u);
	Qdst = g3x_QuatProd(Qdst,Qrot);
	G3Xcopy(dest,Qdst.v);	
}



/* Quaternion d'alignement du vecteur v sur le vecteur cible */
extern G3Xquat g3x_QuatAlign(G3Xvector v, G3Xvector const cible)
{ 
	G3Xvector v1,c1,w;
	G3Xquat   Q;
	
	/* c1 : cible normée */
	G3Xcopy(c1,cible);
	G3Xnormalize(c1);
	
	G3Xcopy(v1,v); 
	G3Xnormalize(v1);
	/* v1 transformé en (v1+c1)/2 pour avoir l'angle moitié */
	/* angle(v1,c1)=0.5*angle(v,c) et v1 reste normé        */ 
	G3Xaddvect(v1,c1);
	G3Xnormalize(v1);

	/* direction de rotation : v1^c1 de norme sin(a) */	
	G3Xprodvect(w,v1,c1);

	/* et cos(a)=v1*c1 */
	return g3x_QuatSet(G3Xprodscal(v1,c1),w);
}


#ifdef __cplusplus
	}
#endif
/*=============================================================================*/
