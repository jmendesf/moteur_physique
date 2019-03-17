#include <time.h>
#include <g3x.h>

#define g 9.8

#define MASS 1
#define PFIX 1
#define RESSORT 0
#define FREIN 1
#define EPSILON 0.002


// number of masses in a line
#define NB_MASS_X 45
// number of horizontal masses
#define NB_MASS_Y 42

// total number of links 
#define NB_LINK 10867

typedef struct _PM_ 
{
    G3Xpoint  P; /*pos*/
    G3Xvector V; /*vit*/
    G3Xvector F; /*accumulation force */
    double m;
    double ray;
    void (*draw)(struct _PM_ *);
    void (*setup)(struct _PM_ *, double);
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
    void (*draw)(struct _LN_ *);  
    int type;
}Link; 

typedef struct _WD_
{
	G3Xvector F; // strength of the wind
	G3Xvector incStep; // progressive incrementation of windforce 
	double maxX; // maximum value for x component
	double maxZ; // maximum value for z component
	int nbFrameMax; // number of frames the wind stays at max force
	int nbFrameOver; // number of frames the wind stays inactive
	int maxReached;
	int isOver;
	void (*algoWind)(struct _WD_ *);
}Wind;

PMat tabM[NB_MASS_Y][NB_MASS_X];
Link tabL[NB_LINK];
Wind *w;
Wind *w2;

/* simulation time step */
double h = 0.0005;
double m = 1;
double k = 1350000;


/* limites de la zone reelle associee a la fenetre */
double wxmin =- 5., wymin =- 5., wxmax =+ 5., wymax =+ 5.;

void InitWind(Wind *W, G3Xvector incStep, double maxX, double maxZ, int nbFrameMax, int nbFrameOver);

 void draw_mass(PMat *M)
 {
    glColor3d(1, 0, 0); 
    glPushMatrix();
        glTranslated(M->P[0], M->P[1], M->P[2]);
        glutSolidSphere(0.05, 20, 5);
    glPopMatrix();

 }

 void draw_pfix(PMat * M)
 {
    glColor3d(0,1,0); 
    glPushMatrix();
        glTranslated(M->P[0], M->P[1], M->P[2]);
        glutSolidSphere(0.15, 20, 5);
    glPopMatrix();
 }

void draw_ressort(Link * L)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        glColor4f(1, 0, 0, 1);
        glVertex3d(L->M1->P[0], L->M1->P[1], L->M1->P[2]);
        glVertex3d(L->M2->P[0], L->M2->P[1], L->M2->P[2]);
    glEnd();
    glEnable(GL_LIGHTING);
}

void algoWind(Wind *W)
{
	// incremental phase
	if(W->maxReached != 1){
		// if X component is not maximal, increment it
		if(W->F[0] < W->maxX)
			W->F[0] += W->incStep[0];
			
		// if Z component is not maximal, increment it
		if(W->F[2] < W->maxZ)
			W->F[2] += W->incStep[2];
			
		// if max is reached for each component, set maxReached to 1
		if((W->F[0] >= W->maxX) && (W->F[2] >= W->maxZ))
			W->maxReached = 1;
	}
	
	// decremental phase
	if((W->maxReached == 1) && (W->isOver != 1)){
		// if Wind has not been maximal for enough frames, decrement nbFrameMax
		if(W->nbFrameMax > 0){
			W->nbFrameMax--;
		} else {
			// if X component is not minimal, decrement it
			if(W->F[0] > 0)
				W->F[0] -= (W->incStep[0] * 2);
			
			// if X component is inferior to 0, set it to 0
			if(W->F[0] < 0)
				W->F[0] = 0.;
					
			// if Z component is not minimal, decrement it
			if(W->F[2] > 0)
				W->F[2] -= (W->incStep[2]* 2);
				
			// if Z component is inferior to 0, set it to 0
			if(W->F[2] < 0)
				W->F[0] = 0.;
			
			// if Z and X component are equal to 0, set isOver to 1
			if((W->F[0] <= 0) && (W->F[2] <= 0))
				W->isOver = 1;	
		}
	}
	
	if((W->isOver == 1) && W->nbFrameOver != 0){
		W->nbFrameOver--;
	} else if(W->isOver == 1){
		double randX = (rand() % 500) + 300;
		double randZ = (rand() % 500) + 300;
		
		InitWind(W, (G3Xvector){randX , 0., randZ}, 
		(rand() % 10000) + 10000, (rand() % 10000) + 10000, (rand() % 300) + 600, (rand() % 200) + 400);
	}
}

void AlgoRessort(Link *L)
{
    double e; /*elongation*/
    double d; /*distance*/
    double f; /*intensité de la force*/

    d = G3Xdist(L->M1->P, L->M2->P);

    /*sécurité division par 0*/
    if(d < EPSILON){
        e = 0;
    }
    else
        e = (1 - (L->l0 / d));

    
    f = L->k * e * (L->M2->P[0] - L->M1->P[0]) ;
    L->M1->F[0] += f;
    L->M2->F[0] -= f;

    f = L->k * e * (L->M2->P[1] - L->M1->P[1]) ;
    L->M1->F[1] += f;
    L->M2->F[1] -= f;

    f = L->k * e * (L->M2->P[2] - L->M1->P[2]) ;
    L->M1->F[2] += f;
    L->M2->F[2] -= f;
}

void AlgoRessortFreint(Link *L)
{

    double e; /*elongation*/
    double d; /*distance*/
    double f; /*intensité de la force*/

    d = G3Xdist(L->M1->P, L->M2->P);

    /*sécurité division par 0*/
    if(d < EPSILON){
        e = 0;
    }
    else
        e = (1 - (L->l0/d));

    
    f = L->k * e * (L->M2->P[0] - L->M1->P[0]) + L->z * (L->M2->V[0] - L->M1->V[0]);

    L->M1->F[0] += f;
    L->M2->F[0] -= f;

    f = L->k * e * (L->M2->P[1]- L->M1->P[1]) + L->z * (L->M2->V[1] - L->M1->V[1]) ;
    
    L->M2->F[1] -= f;
    L->M1->F[1] += f;
    
    f = L->k * e * (L->M2->P[2]- L->M1->P[2]) + L->z * (L->M2->V[2] - L->M1->V[2]) ;
    L->M2->F[2] -= f;
    L->M1->F[2] += f;
}

/*c'est un integrateur parmis d'autres*/
void leapfrog(PMat *M,double h)
{
    /*1ere integration* F->v*/
    M->V[0] += (h/M->m) * M->F[0];
    M->V[1] += (h/M->m) * M->F[1] - g;
    M->V[2] += (h/M->m) * M->F[2];
    
    /*2eme integration* v->p*/
    M->P[0] += h * M->V[0];
    M->P[1] += h * M->V[1];
    M->P[2] += h * M->V[2];
    
    /*vidange*/
    M->F[0] = 0.;
    M->F[1] = 0.;
    M->F[2] = 0.;
    } 

void pointfixe(PMat *M, double r)
{
    /*vidange*/
    M->F[0] = 0.;
    M->F[1] = 0.;
    M->F[2] = 0.;
} 

void InitMass(PMat *M, G3Xpoint  P0, G3Xvector V0, double m, double r)
{
    M->type = MASS;
    M->m = m;
    M->ray = r;

    M->P[0] = P0[0];
    M->P[1] = P0[1];
    M->P[2] = P0[2];

    M->V[0] = V0[0];
    M->V[1] = V0[1];
    M->V[2] = V0[2];

    M->F[0] = 0.;
    M->F[1] = 0.;
    M->F[2] = 0.;

    M->draw = draw_mass;
    M->setup = leapfrog;
}

void InitPFix(PMat *M, G3Xpoint  P0, double r)
{
    M->type = PFIX;
    M->m = 0.;
    M->ray = r;

    M->P[0] = P0[0];
    M->P[1] = P0[1];
    M->P[2] = P0[2];

    M->V[0] = 0.;
    M->V[1] = 0.;
    M->V[2] = 0.;

    M->F[0] = 0.;
    M->F[1] = 0.;
    M->F[2] = 0.;

    M->draw = draw_pfix;
    M->setup = pointfixe;
}

void InitRessort(Link *L, PMat *M1, PMat *M2, double k, double z)
{
    L->M1 = M1;
    L->M2 = M2;
    L->k = k;
    L->z = z;
    L->l0 = G3Xdist(M1->P, M2->P);
    L->algo = AlgoRessortFreint;
    L->type = RESSORT;
    L->draw = draw_ressort;
}

void InitWind(Wind * W, G3Xvector incStep, double maxX, double maxZ, int nbFrameMax, int nbFrameOver)
{
	W->F[0] = 0.;
	W->F[1] = 0.;
	W->F[2] = 0.;
	
	W->incStep[0] = incStep[0];
	W->incStep[1] = incStep[1];
	W->incStep[2] = incStep[2];
	
	W->maxX = maxX;
	W->maxZ = maxZ;

	W->nbFrameMax = nbFrameMax;
	W->nbFrameOver = nbFrameOver;
	W->maxReached = 0;
	W->isOver = 0;
	W->algoWind = algoWind;
}

/* la fonction d'initialisation */
static void init(void)
{
  PMat *M; // current mass
  PMat *MSup; // mass directly above
  PMat *MInf; // mass directly below
  PMat *M2Inf; // mass below MInf
  Link *L = tabL; // link array
  int nbLink = 0;
  
  w = malloc(sizeof(Wind));
  // w2 = malloc(sizeof(Wind));
  
  InitWind(w, (G3Xvector){300., 0, 500.}, 10000., 10000., 300., 200);
  // InitWind(w2, (G3Xvector){250., 0, 400.}, 10000., 100000., 1000., 100);
  
  int i, j;
  
  for(j = 0; j < NB_MASS_Y; j++){
    M = tabM[j];
    InitPFix(M, (G3Xpoint){0, -j, 0}, 0.1);
    M++;
    
    // allows rigidity to be superior on the springs closer to the base
    // than those at the end of the flag
		double kCoeff = 1.; 
    for (i = 1; i< NB_MASS_X; i++){
      InitMass(M, (G3Xpoint){i,-j,0}, (G3Xvector){0,0,0},1, 0.1); /*masse puis ray à la fin*/
      M++;
    }

    M = tabM[j];
    if(j > 0) MSup = tabM[j - 1];
    if(j < NB_MASS_Y - 1) MInf = tabM[j + 1];
    if(j < NB_MASS_Y - 2) M2Inf = tabM[j + 2];
    
    for(i = 0; i < NB_MASS_X - 1; i++){
			// horizontal springs
      InitRessort(L,M,M+1,k / kCoeff,90);
      k += 25.;
      L++;  
      nbLink++;
      
      // vertical springs         
      if(j > 0){
				InitRessort(L, M, MSup, k / kCoeff, 15);
				L++;
				nbLink++;
			}
			
			// diagonal springs
			if(i < NB_MASS_X - 1){
				if(j > 0){
					InitRessort(L, M, MSup + 1, k/500, 15);
					L++;
					nbLink++;
				}
				if(j < NB_MASS_Y - 1){
					InitRessort(L, M, MInf + 1, k/500, 15);
					L++;
					nbLink++;
				}
			}
			// last vertical springs
			if((i == NB_MASS_X - 2) && (j > 0)){
				InitRessort(L, M + 1, MSup +1, k, 15);
				L++;
				nbLink++;
			}
			
			// horizontal tensor springs
			if(i < NB_MASS_X - 2){
				InitRessort(L, M, M + 2, k/100, 15);
				L++;
				nbLink++;
			}
			
			// vertical tensor springs
			if(j < NB_MASS_Y - 2){
				InitRessort(L, M, M2Inf, k/100, 15);
				L++;
				nbLink++;
			}
		
			// printf("%d\n", nbLink);
			MSup++;
			MInf++;
			M2Inf++;
      M++;
    }               
  }  
  /*ICI RUPTURE EQUILIBRE*/
}

static void draw()
{ 
    int i, j;
    Link *L = tabL;
     /*   
    for(j = 0; j < NB_MASS_Y; j++){
        PMat *M = tabM[j];
                        
        for(i = 0; i < NB_MASS_X; i++){
            M->draw(M);
            M++;
        }
    }     
    */   
    for(i = 0; i < NB_LINK; i++){
        L->draw(L);
        L++;
    }
}

static void anim(void)
{ 
    int i, j;
  
 	Link *L = tabL;
    for(i = 0; i < NB_LINK; i++){
			L->algo(L);
			L++;          
	}  
	w->algoWind(w);
	// w->algoWind(w2);
	// printf("x : %f, z : %f, nbFrameMax : %d, nbFrameOver : %d \n", w->F[0], w->F[2], w->nbFrameMax, w->nbFrameOver);
	
	for(j = 0; j < NB_MASS_Y; j++){
		PMat *M = tabM[j];
		for(i = 0; i < NB_MASS_X; i++){
			M->F[0] += w->F[0];
			M->F[2] += w->F[2];
			
			// M->F[0] += w2->F[0];
			// M->F[2] += w2->F[2];
			M->setup(M, h);
			M++;
		}   
 	}                   
}

int main(int argc, char **argv)
{
	srand(time(NULL));
    /* creation de la fenetre - titre et tailles (pixels) */
    g3x_InitWindow(*argv,1024,1024);
    /* param. géométrique de la caméra. cf. gluLookAt(...) */
    g3x_SetPerspective(40.,500.,1.);
    /* position, orientation de la caméra */
    g3x_SetCameraSpheric(0.7*PI,-.75*PI,120.,(G3Xpoint){0.,0.,0.});
        
    g3x_SetInitFunction(init); /* fonction d'initialisation */
    g3x_SetDrawFunction(draw); // fonction de dessin       */
    g3x_SetAnimFunction(anim); /* fonction d'animation      */
    /* lancement de la boucle principale */
    return g3x_MainStart();
}
