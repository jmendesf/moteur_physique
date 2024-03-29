/*******************************************************************************************/
#include <g2x.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define g 9.8

#define MASS 1
#define PFIX 1
#define RESSORT 0
#define FREIN 1
#define EPSILON 0.002

#define NB_LINK 9
#define NB_MASS 10

typedef struct _PM_ 
{
  G2Xpoint  P; /*pos*/
  G2Xvector V; /*vit*/
  G2Xvector F; /*accumulation force */
  double m;
  double ray;
  void (*draw)(struct _PM_*);
  void (*setup)(struct _PM_*,double);
  int type;
}PMat;

typedef struct _LN_ 
{
  double k; /*raideur ressort*/
  double l0; /*longueur initiale*/
  double z; /*coeff d'amortissement cinétique*/
  /*S1,S2,S3 seuils...*/
  PMat*  M1;
  PMat*  M2;
  void (*algo)(struct _LN_ *);
  void (*draw)(struct _LN_*);	
  int type;
}Link; 

//int nbMass = 10;
//int NB_LINK = nbMass - 1;

PMat tabM[NB_MASS];
Link tabL[NB_LINK];

/* simulation time step */
double h =0.001;
double m = 1;
double k = 100000;


/* limites de la zone reelle associee a la fenetre */
double wxmin=-5.,wymin=-5.,wxmax=+5.,wymax=+5.;


void draw_mass(PMat * M)
{

  g2x_FillCircle(M->P.x,M->P.y,M->ray,G2Xr);

}

void draw_pfix(PMat * M)
{

  g2x_FillCircle(M->P.x,M->P.y,M->ray,G2Xb);

}

void draw_ressort(Link * L)
{
  
  G2Xpoint  A = L->M1->P;
  G2Xpoint  B = L->M2->P;
  
  g2x_Line(A.x, A.y,B.x,B.y,G2Xr,1);
  
}

void AlgoRessort(Link *L)
{

  double e; /*elongation*/
  double d; /*distance*/
  double f; /*intensité de la force*/

  d = g2x_Distance(L->M1->P,L->M2->P);

  /*sécurité division par 0*/
  if(d< EPSILON){
    e = 0;
  }
  else
    e = (1-(L->l0/d));

  
  f = L->k * e * (L->M2->P.x - L->M1->P.x) ;

  L->M1->F.x += f;
  L->M2->F.x -= f;

  f = L->k * e * (L->M2->P.y - L->M1->P.y) ;
  
  L->M1->F.y += f;
  L->M2->F.y -= f;

}

void AlgoRessortFreint(Link *L)
{

  double e; /*elongation*/
  double d; /*distance*/
  double f; /*intensité de la force*/

  d = g2x_Distance(L->M1->P,L->M2->P);

  /*sécurité division par 0*/
  if(d< EPSILON){
    e = 0;
  }
  else
    e = (1-(L->l0/d));

  
  f = L->k * e * (L->M2->P.x - L->M1->P.x) + L->z*(L->M2->V.x - L->M1->V.x);

  L->M1->F.x += f;
  L->M2->F.x -= f;

  f = L->k * e * (L->M2->P.y - L->M1->P.y) + L->z*(L->M2->V.y - L->M1->V.y) ;
  
  L->M1->F.y += f;
  L->M2->F.y -= f;
  
}

/*c'est un integrateur parmis d'autres*/
void leapfrog(PMat * M,double h)
{

  /*1ere integration* F->v*/
  M->V.x += (h/M->m)*M->F.x;
  M->V.y += (h/M->m)*M->F.y -g;

  /*2eme integration* v->p*/
  M->P.x += h*M->V.x;
  M->P.y += h*M->V.y;

  /*vidange*/
  M->F.x = 0.;
  M->F.y = 0.;

} 

void pointfixe(PMat * M, double r)
{

  /*M->V.x = M->V.y = 0.;*/
  /*vidange*/
  M->F.x = 0.;
  M->F.y = 0.;

} 

void InitMass(PMat* M, G2Xpoint  P0, G2Xvector V0, double m, double r)
{

  M->type = MASS;
  M->m = m;
  M->ray = r;
  M->P = P0;
  M->V = V0;
  M->F = (G2Xvector){0.,0.};

  M->draw = draw_mass;
  M->setup = leapfrog;


}

void InitPFix(PMat* M, G2Xpoint  P0, double r)
{

  M->type = PFIX;
  M->m = 0.;
  M->ray = r;
  M->P = P0;
  M->V = (G2Xvector){0.,0.};
  M->F = (G2Xvector){0.,0.};

  M->draw = draw_pfix;
  M->setup = pointfixe;


}

void InitRessort(Link * L, PMat *M1, PMat *M2, double k,double z)
{

  L->M1 =M1;
  L->M2 =M2;
  L->k =k;
  L->z = z;
  L->l0 = g2x_Distance(M1->P,M2->P);
  L->algo = AlgoRessortFreint;
  L->type = RESSORT;
  L->draw = draw_ressort;

}

/* la fonction d'initialisation */
static void init(void)
{
  int i;
  PMat *M = tabM;
  InitPFix(M,(G2Xpoint){-5,0}, 0.1);
  M++;

  for (i = 1; i< NB_LINK; i++){
    InitMass(M,(G2Xpoint){-5 + i,0}, (G2Xvector){0,0},1, 0.1); /*masse puis ray à la fin*/
    M++;
  }
  InitPFix(M,(G2Xpoint){+5,0}, 0.1);

  /*Topologie: liaisons*/
  M=tabM;
  Link* L =tabL;
  while(L<tabL+NB_LINK){

    InitRessort(L,M,M+1,k,100);
    L++;
    M++;
  }

  
  tabM[1].P.y-=0.0;

  /*ICI RUPTURE EQUILIBRE*/

}

static void draw()
{ 
  
  PMat *M = tabM;
  Link *L = tabL;
  while(M<tabM + NB_MASS){
    M->draw(M);
    M++;
  }
  
  while(L<tabL + NB_LINK){
    L->draw(L);
    L++;
  }
}

static void anim(void)
{ 
  PMat *M = tabM;
  Link *L = tabL;

  

  while(L<tabL + NB_LINK){
    L->algo(L);
    L++;
  }

  while(M<tabM + NB_MASS){
    M->setup(M,h);
    M++;
  }

}


/***************************************************************************/
/*                                                                         */
/***************************************************************************/
int main(int argc, char **argv)
{
  /* creation de la fenetre - titre et tailles (pixels) */
  g2x_InitWindow(*argv,1024,1024);
  /* zone graphique reelle associee a la fenetre */	
  g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);

  g2x_SetInitFunction(init); /* fonction d'initialisation */
  g2x_SetDrawFunction(draw); /* fonction de dessin       */
  g2x_SetAnimFunction(anim); /* fonction d'animation      */
	
  /* lancement de la boucle principale */
  return g2x_MainStart();
  /* RIEN APRES CA */
  return 0;
}
