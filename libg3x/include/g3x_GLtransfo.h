/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'accès public -           
                    version 5.0 - Dec.2012                                           
                    version 5.1 - Dec.2018 
  Matrices de transfo façon GL : transposée par rapport à G3Xhmat  
  ============================================================= */

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G3X_GLTRANSFO_H
  #define _G3X_GLTRANSFO_H

  #include <g3x_macros.h>

  /*= MATRICE EN COORDONNEES HOMOGENE VERION OpenGl =*/
  /* 1° ligne */
  #define _m00  0
  #define _m10  1
  #define _m20  2
  #define _m30  3
  /* 2° colonne */
  #define _m01  4
  #define _m11  5
  #define _m21  6  
  #define _m31  7
  /* 3° colonne */
  #define _m02  8
  #define _m12  9
  #define _m22 10
  #define _m32 11
  /* 4° colonne */
  #define _m03 12
  #define _m13 13
  #define _m23 14
  #define _m33 15

  /* copie la matrice src dans la matrice dst */
  #define G3XcopyGLmat(dest,src) /**/memcpy((dest),(src),sizeof(G3Xglmat))

  /* matrice identité Id */
  #define G3XGLidentity /**/(G3Xglmat){1.,0.,0.,0.,\
                                     0.,1.,0.,0.,\
                                     0.,0.,1.,0.,\
                                     0.,0.,0.,1.}

  /* initialise la matrice A à Id */
  #define G3XloadGLidentity(A)/**/(memset((A),0,sizeof(G3Xglmat)),\
                                (A)[_m00]=(A)[_m11]=(A)[_m22]=(A)[_m33]=1)

  /* type matrice en coordonnées homogènes */   
  typedef double (G3Xglmat)[16];

  /* Creation de la matrice 3x3 Identite */
  void g3x_MakeGLIdentity(G3Xglmat A);
    

  /*=****************************************=*/
  /*=   MATRICES DE TRANSFORMATION STANDARD  =*/
  /*=****************************************=*/
  void g3x_MakeGLTranslationV(G3Xglmat A, G3Xvector t);
  void g3x_MakeGLTranslationXYZ(G3Xglmat A, double tx, double ty, double tz);
  void g3x_MakeGLHomothetieV(G3Xglmat A, G3Xvector h);
  void g3x_MakeGLHomothetieXYZ(G3Xglmat A, double hx, double hy, double hz);
  void g3x_MakeGLRotationX(G3Xglmat A, double alpha);
  void g3x_MakeGLRotationY(G3Xglmat A, double alpha);
  void g3x_MakeGLRotationZ(G3Xglmat A, double alpha);

  /*=****************************************=*/
  /*=  Fonctions utiltaires sur les matrices =*/
  /*=****************************************=*/
  /* Produit Matrice(A)*Vecteur(V) -> Vecteur(W) */
  void g3x_ProdGLMatVector(G3Xglmat A, G3Xvector V, G3Xvector W);
  /* Produit Matrice(A)*Point(P) -> Point(Q) */
  void g3x_ProdGLMatPoint(G3Xglmat A, G3Xpoint P, G3Xpoint Q);
  /* Produit Matrice(A)*Point(x,y,z) -> Coord(Q) */
  void g3x_ProdGLMatXYZ(G3Xglmat A, double x, double y, double z, G3Xcoord Q);
  /* Produit Matrice(A)*Matrice(B)->Matrice(C) */
  void g3x_ProdGLMat(G3Xglmat A,G3Xglmat B, G3Xglmat C);
  /* Affichage de la matrice */
  void g3x_PrintGLMat(G3Xglmat A);

#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
