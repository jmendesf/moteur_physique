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
#include <math.h>
#include <g3x_transfo.h>

extern void g3x_MakeIdentity(G3Xhmat A) 
{
	memset(A,0,sizeof(G3Xhmat));
  A[a00] = A[a11] = A[a22] = A[a33] = 1.0;
}

extern void g3x_MakeTranslationV(G3Xhmat A, G3Xvector t) 
{
  G3Xloadidentity(A);
  A[a03] = t[0];
  A[a13] = t[1];
  A[a23] = t[2];
}

extern void g3x_MakeTranslationXYZ(G3Xhmat A, double tx, double ty, double tz)
{
  G3Xloadidentity(A);
  A[a03] = tx;
  A[a13] = ty;
  A[a23] = tz;
}

extern void g3x_MakeHomothetieV(G3Xhmat A, G3Xvector h) 
{
  G3Xloadidentity(A);
  A[a00] = h[0];
  A[a11] = h[1];
  A[a22] = h[2];
}

extern void g3x_MakeHomothetieXYZ(G3Xhmat A, double hx, double hy, double hz) 
{
  G3Xloadidentity(A);
  A[a00] = hx;
  A[a11] = hy;
  A[a22] = hz;
}

extern void g3x_MakeRotationX(G3Xhmat A, double alpha) 
{
  double c=cos(alpha), s=sin(alpha);
  G3Xloadidentity(A);
  A[a11] = +c; A[a12] = -s;
  A[a21] = +s; A[a22] = +c;
}

extern void g3x_MakeRotationY(G3Xhmat A, double alpha) 
{
  double c=cos(alpha), s=sin(alpha);
  G3Xloadidentity(A);
  A[a00] = +c; A[a02] = +s;
  A[a20] = -s; A[a22] = +c;
}

extern void g3x_MakeRotationZ(G3Xhmat A, double alpha) 
{
  double c=cos(alpha), s=sin(alpha);
  G3Xloadidentity(A);
  A[a00] = +c; A[a01] = -s;
  A[a10] = +s; A[a11] = +c;
}

extern void g3x_MakeRotation(G3Xhmat A, G3Xvector R) 
{
  G3Xloadidentity(A);
  double c=cos(R[0]), s=sin(R[0]);
  A[a00] = +c; A[a01] = -s;
  A[a10] = +s; A[a11] = +c;

}

extern void g3x_ProdHMatVector(G3Xhmat A,G3Xvector V, G3Xvector W) 
{	
	static G3Xpoint U;
  U[0] = A[a00]*V[0] + A[a01]*V[1] + A[a02]*V[2];
  U[1] = A[a10]*V[0] + A[a11]*V[1] + A[a12]*V[2];
  U[2] = A[a20]*V[0] + A[a21]*V[1] + A[a22]*V[2];
	G3Xcopy(W,U);
}

extern void g3x_ProdHMatPoint(G3Xhmat A, G3Xpoint P, G3Xpoint Q)
{
	static G3Xpoint M;
  M[0] = A[a00]*P[0] + A[a01]*P[1] + A[a02]*P[2] + A[a03];
  M[1] = A[a10]*P[0] + A[a11]*P[1] + A[a12]*P[2] + A[a13];
	M[2] = A[a20]*P[0] + A[a21]*P[1] + A[a22]*P[2] + A[a23];
	G3Xcopy(Q,M);
}

extern void g3x_ProdHMatXYZ(G3Xhmat A, double x, double y, double z, G3Xcoord Q)
{
	static G3Xcoord M;
  M[0] = A[a00]*x + A[a01]*y + A[a02]*z + A[a03];
  M[1] = A[a10]*x + A[a11]*y + A[a12]*z + A[a13];
	M[2] = A[a20]*x + A[a21]*y + A[a22]*z + A[a23];
	G3Xcopy(Q,M);
}

extern void g3x_ProdHMat(G3Xhmat A,G3Xhmat B, G3Xhmat C) 
{
	static G3Xhmat D;
  D[a00] = A[a00]*B[a00] + A[a01]*B[a10] + A[a02]*B[a20] + A[a03]*B[a30];
  D[a10] = A[a10]*B[a00] + A[a11]*B[a10] + A[a12]*B[a20] + A[a13]*B[a30];
  D[a20] = A[a20]*B[a00] + A[a21]*B[a10] + A[a22]*B[a20] + A[a23]*B[a30];
  D[a30] = A[a30]*B[a00] + A[a31]*B[a10] + A[a32]*B[a20] + A[a33]*B[a30];
  D[a01] = A[a00]*B[a01] + A[a01]*B[a11] + A[a02]*B[a21] + A[a03]*B[a31];
  D[a11] = A[a10]*B[a01] + A[a11]*B[a11] + A[a12]*B[a21] + A[a13]*B[a31];
  D[a21] = A[a20]*B[a01] + A[a21]*B[a11] + A[a22]*B[a21] + A[a23]*B[a31];
  D[a31] = A[a30]*B[a01] + A[a31]*B[a11] + A[a32]*B[a21] + A[a33]*B[a31];
  D[a02] = A[a00]*B[a02] + A[a01]*B[a12] + A[a02]*B[a22] + A[a03]*B[a32];
  D[a12] = A[a10]*B[a02] + A[a11]*B[a12] + A[a12]*B[a22] + A[a13]*B[a32];
  D[a22] = A[a20]*B[a02] + A[a21]*B[a12] + A[a22]*B[a22] + A[a23]*B[a32];
  D[a32] = A[a30]*B[a02] + A[a31]*B[a12] + A[a32]*B[a22] + A[a33]*B[a32];
  D[a03] = A[a00]*B[a03] + A[a01]*B[a13] + A[a02]*B[a23] + A[a03]*B[a33];
  D[a13] = A[a10]*B[a03] + A[a11]*B[a13] + A[a12]*B[a23] + A[a13]*B[a33];
  D[a23] = A[a20]*B[a03] + A[a21]*B[a13] + A[a22]*B[a23] + A[a23]*B[a33];
  D[a33] = A[a30]*B[a03] + A[a31]*B[a13] + A[a32]*B[a23] + A[a33]*B[a33];
	memcpy(C,D,sizeof(G3Xhmat));
}

extern  void g3x_FormatGLmat(G3Xhmat dst, G3Xhmat src)
{
 dst[a00]=src[a00]; dst[a10]=src[a01]; dst[a20]=src[a02]; dst[a30]=src[a03];
 dst[a01]=src[a10]; dst[a11]=src[a11]; dst[a21]=src[a12]; dst[a31]=src[a13];
 dst[a02]=src[a20]; dst[a12]=src[a21]; dst[a22]=src[a22]; dst[a32]=src[a23];
 dst[a03]=src[a30]; dst[a13]=src[a31]; dst[a23]=src[a32]; dst[a33]=src[a33];
}


extern void g3x_PrintHMat(G3Xhmat A) 
{
  fprintf(stderr,"Matrice :\n");
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A[a00],A[a01],A[a02],A[a03]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A[a10],A[a11],A[a12],A[a13]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A[a20],A[a21],A[a22],A[a23]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A[a30],A[a31],A[a32],A[a33]);
  fprintf(stderr,"\n");
}

#ifdef __cplusplus
	}
#endif
/*=============================================================================*/

