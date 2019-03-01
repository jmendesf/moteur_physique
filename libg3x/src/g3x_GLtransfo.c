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
#include <g3x_GLtransfo.h>

extern void g3x_MakeGLIdentity(G3Xglmat A) 
{
	memset(A,0,sizeof(G3Xglmat));
  A[_m00] = A[_m11] = A[_m22] = A[_m33] = 1.0;
}

extern void g3x_MakeGLTranslationV(G3Xglmat A, G3Xvector t) 
{
  G3XloadGLidentity(A);
  A[_m03] = t[0];
  A[_m13] = t[1];
  A[_m23] = t[2];
}

extern void g3x_MakeGLTranslationXYZ(G3Xglmat A, double tx, double ty, double tz)
{
  G3XloadGLidentity(A);
  A[_m03] = tx;
  A[_m13] = ty;
  A[_m23] = tz;
}

extern void g3x_MakeGLHomothetieV(G3Xglmat A, G3Xvector h) 
{
  G3XloadGLidentity(A);
  A[_m00] = h[0];
  A[_m11] = h[1];
  A[_m22] = h[2];
}

extern void g3x_MakeGLHomothetieXYZ(G3Xglmat A, double hx, double hy, double hz) 
{
  G3XloadGLidentity(A);
  A[_m00] = hx;
  A[_m11] = hy;
  A[_m22] = hz;
}

extern void g3x_MakeGLRotationX(G3Xglmat A, double alpha) 
{
  double c=cos(alpha), s=sin(alpha);
  G3XloadGLidentity(A);
  A[_m11] = +c; A[_m12] = -s;
  A[_m21] = +s; A[_m22] = +c;
}

extern void g3x_MakeGLRotationY(G3Xglmat A, double alpha) 
{
  double c=cos(alpha), s=sin(alpha);
  G3XloadGLidentity(A);
  A[_m00] = +c; A[_m02] = +s;
  A[_m20] = -s; A[_m22] = +c;
}

extern void g3x_MakeGLRotationZ(G3Xglmat A, double alpha) 
{
  double c=cos(alpha), s=sin(alpha);
  G3XloadGLidentity(A);
  A[_m00] = +c; A[_m01] = -s;
  A[_m10] = +s; A[_m11] = +c;
}

extern void g3x_MakeGLRotation(G3Xglmat A, G3Xvector R) 
{
  G3XloadGLidentity(A);
  double c=cos(R[0]), s=sin(R[0]);
  A[_m00] = +c; A[_m01] = -s;
  A[_m10] = +s; A[_m11] = +c;

}

extern void g3x_ProdGLMatVector(G3Xglmat A, G3Xvector V, G3Xvector W) 
{	
	static G3Xpoint U;
  U[0] = A[_m00]*V[0] + A[_m01]*V[1] + A[_m02]*V[2];
  U[1] = A[_m10]*V[0] + A[_m11]*V[1] + A[_m12]*V[2];
  U[2] = A[_m20]*V[0] + A[_m21]*V[1] + A[_m22]*V[2];
	G3Xcopy(W,U);
}

extern void g3x_ProdGLMatPoint(G3Xglmat A, G3Xpoint P, G3Xpoint Q)
{
	static G3Xpoint M;
  M[0] = A[_m00]*P[0] + A[_m01]*P[1] + A[_m02]*P[2] + A[_m03];
  M[1] = A[_m10]*P[0] + A[_m11]*P[1] + A[_m12]*P[2] + A[_m13];
	M[2] = A[_m20]*P[0] + A[_m21]*P[1] + A[_m22]*P[2] + A[_m23];
	G3Xcopy(Q,M);
}

extern void g3x_ProdGLMatXYZ(G3Xglmat A, double x, double y, double z, G3Xcoord Q)
{
	static G3Xcoord M;
  M[0] = A[_m00]*x + A[_m01]*y + A[_m02]*z + A[_m03];
  M[1] = A[_m10]*x + A[_m11]*y + A[_m12]*z + A[_m13];
	M[2] = A[_m20]*x + A[_m21]*y + A[_m22]*z + A[_m23];
	G3Xcopy(Q,M);
}

extern void g3x_ProdGLMat(G3Xglmat A,G3Xglmat B, G3Xglmat C) 
{
	static G3Xglmat D;
  D[_m00] = A[_m00]*B[_m00] + A[_m01]*B[_m10] + A[_m02]*B[_m20] + A[_m03]*B[_m30];
  D[_m10] = A[_m10]*B[_m00] + A[_m11]*B[_m10] + A[_m12]*B[_m20] + A[_m13]*B[_m30];
  D[_m20] = A[_m20]*B[_m00] + A[_m21]*B[_m10] + A[_m22]*B[_m20] + A[_m23]*B[_m30];
  D[_m30] = A[_m30]*B[_m00] + A[_m31]*B[_m10] + A[_m32]*B[_m20] + A[_m33]*B[_m30];
  D[_m01] = A[_m00]*B[_m01] + A[_m01]*B[_m11] + A[_m02]*B[_m21] + A[_m03]*B[_m31];
  D[_m11] = A[_m10]*B[_m01] + A[_m11]*B[_m11] + A[_m12]*B[_m21] + A[_m13]*B[_m31];
  D[_m21] = A[_m20]*B[_m01] + A[_m21]*B[_m11] + A[_m22]*B[_m21] + A[_m23]*B[_m31];
  D[_m31] = A[_m30]*B[_m01] + A[_m31]*B[_m11] + A[_m32]*B[_m21] + A[_m33]*B[_m31];
  D[_m02] = A[_m00]*B[_m02] + A[_m01]*B[_m12] + A[_m02]*B[_m22] + A[_m03]*B[_m32];
  D[_m12] = A[_m10]*B[_m02] + A[_m11]*B[_m12] + A[_m12]*B[_m22] + A[_m13]*B[_m32];
  D[_m22] = A[_m20]*B[_m02] + A[_m21]*B[_m12] + A[_m22]*B[_m22] + A[_m23]*B[_m32];
  D[_m32] = A[_m30]*B[_m02] + A[_m31]*B[_m12] + A[_m32]*B[_m22] + A[_m33]*B[_m32];
  D[_m03] = A[_m00]*B[_m03] + A[_m01]*B[_m13] + A[_m02]*B[_m23] + A[_m03]*B[_m33];
  D[_m13] = A[_m10]*B[_m03] + A[_m11]*B[_m13] + A[_m12]*B[_m23] + A[_m13]*B[_m33];
  D[_m23] = A[_m20]*B[_m03] + A[_m21]*B[_m13] + A[_m22]*B[_m23] + A[_m23]*B[_m33];
  D[_m33] = A[_m30]*B[_m03] + A[_m31]*B[_m13] + A[_m32]*B[_m23] + A[_m33]*B[_m33];
	memcpy(C,D,sizeof(G3Xglmat));
}


extern void g3x_PrintGLMat(G3Xglmat A) 
{
  fprintf(stderr,"Matrice :\n");
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A[_m00],A[_m10],A[_m20],A[_m30]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A[_m01],A[_m11],A[_m21],A[_m31]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A[_m02],A[_m12],A[_m22],A[_m32]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A[_m03],A[_m13],A[_m23],A[_m33]);
  fprintf(stderr,"\n");
}

#ifdef __cplusplus
	}
#endif
/*=============================================================================*/

