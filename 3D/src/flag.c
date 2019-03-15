/*******************************************************************************************/
#include <g3x.h>

#define g 9.8

#define MASS 1
#define PFIX 1
#define RESSORT 0
#define FREIN 1
#define EPSILON 0.002

#define NB_LINK_X 3
#define NB_LINK_Y 1
#define NB_MASS_X 4
#define NB_MASS_Y 1

typedef struct _PM_ 
{
	G3Xpoint	P; /*pos*/
	G3Xvector V; /*vit*/
	G3Xvector F; /*accumulation force */
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
	PMat*	M1;
	PMat*	M2;
	void (*algo)(struct _LN_ *);
	void (*draw)(struct _LN_*);	
	int type;
}Link; 

PMat tabM[NB_MASS_Y][NB_MASS_X];
Link tabL[NB_LINK_Y][NB_LINK_X];

/* simulation time step */
double h = 0.0005;
double m = 1;
double k = 300000;


/* limites de la zone reelle associee a la fenetre */
double wxmin=-5., wymin=-5., wxmax=+5., wymax=+5.;


 void draw_mass(PMat * M)
 {
	glColor3d(1,0,0); 
	glPushMatrix();
		glTranslated(M->P[0], M->P[1], M->P[2]);
		glutSolidSphere(0.1,20,50);
	glPopMatrix();

 }

 void draw_pfix(PMat * M)
 {
	glColor3d(0,1,0); 
	glPushMatrix();
		glTranslated(M->P[0], M->P[1], M->P[2]);
		glutSolidSphere(0.2,20,50);
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

void AlgoRessort(Link *L)
{

	double e; /*elongation*/
	double d; /*distance*/
	double f; /*intensité de la force*/

	d = G3Xdist(L->M1->P, L->M2->P);

	/*sécurité division par 0*/
	if(d< EPSILON){
		e = 0;
	}
	else
		e = (1-(L->l0/d));

	
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

	d = G3Xdist(L->M1->P,L->M2->P);

	/*sécurité division par 0*/
	if(d< EPSILON){
		e = 0;
	}
	else
		e = (1-(L->l0/d));

	
	f = L->k * e * (L->M2->P[0] - L->M1->P[0]) + L->z*(L->M2->V[0] - L->M1->V[0]);

	L->M1->F[0] += f;
	L->M2->F[0] -= f;

	f = L->k * e * (L->M2->P[1]- L->M1->P[1]) + L->z*(L->M2->V[1] - L->M1->V[1]) ;
	
	L->M2->F[1] -= f;
	L->M1->F[1] += f;
	
	f = L->k * e * (L->M2->P[2]- L->M1->P[2]) + L->z*(L->M2->V[2] - L->M1->V[2]) ;
	L->M2->F[2] -= f;
	L->M1->F[2] += f;
}

/*c'est un integrateur parmis d'autres*/
void leapfrog(PMat * M,double h)
{

	/*1ere integration* F->v*/
	M->V[0] += (h/M->m)*M->F[0];
	M->V[1] += (h/M->m)*M->F[1] - g;
	M->V[2] += (h/M->m)*M->F[2];
	
	/*2eme integration* v->p*/
	M->P[0] += h*M->V[0];
	M->P[1] += h*M->V[1];
	M->P[2] += h*M->V[2];
	
	/*vidange*/
	M->F[0] = 0.;
	M->F[1] = 0.;
	M->F[2] = 0.;
} 

void pointfixe(PMat * M, double r)
{

	/*M->V.x = M->V.y = 0.;*/
	/*vidange*/
	M->F[0] = 0.;
	M->F[1] = 0.;
	M->F[2] = 0.;
} 

void InitMass(PMat* M, G3Xpoint	P0, G3Xvector V0, double m, double r)
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

void InitPFix(PMat* M, G3Xpoint	P0, double r)
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

void InitRessort(Link * L, PMat *M1, PMat *M2, double k,double z)
{

	L->M1 =M1;
	L->M2 =M2;
	L->k =k;
	L->z = z;
	L->l0 = G3Xdist(M1->P,M2->P);
	L->algo = AlgoRessortFreint;
	L->type = RESSORT;
	L->draw = draw_ressort;

}

/* la fonction d'initialisation */
static void init(void)
{
	int i;
	PMat *M = tabM[0];
	InitPFix(M,(G3Xpoint){0,0,0}, 0.1);
	M++;

	for (i = 1; i< NB_MASS_X; i++){
		InitMass(M,(G3Xpoint){i,0,0}, (G3Xvector){0,0,0},1, 0.1); /*masse puis ray à la fin*/
		M++;
	}
	//InitPFix(M,(G2Xpoint){+5,0}, 0.1);
	/*Topologie: liaisons*/
	M=tabM[0];
	Link *L = tabL[0];
	/*while(L<tabL+NB_LINK_Y){
		InitRessort(L,M,M+1,k,90);
		L++;
		M++;
	}*/
	for(i = 0; i < NB_LINK_X; i++){
                        

                InitRessort(L,M,M+1,k,90);
                M++;
                L++;
        }
	
	//tabM[1].P[1] -= 0.0;

	/*ICI RUPTURE EQUILIBRE*/
}

static void draw()
{ 
	int i;
	PMat *M = tabM[0];
	Link *L = tabL[0];
        
        for(i = 0; i < NB_MASS_X; i++){
                M->draw(M);
                M++;
        }
        
	
        for(i = 0; i < NB_LINK_X; i++){

                L->draw(L);
                L++;
        }
        
        /*
	while(M<tabM + NB_MASS){
		M->draw(M);
		M++;
	}*/
        
        /*
	while(L<tabL + NB_LINK){
		L->draw(L);
		L++;
	}*/
}

static void anim(void)
{ 
        int i;
	PMat *M = tabM[0];
	Link *L = tabL[0];

	for(i = 0; i < NB_LINK_X; i++){
                L->algo(L);
                L++;
        }
        
        for(i = 0; i < NB_MASS_X; i++){
                M->setup(M,h);
                M++;
        }	
        /*
	while(L<tabL + NB_LINK){
		L->algo(L);
		L++;
	}

	 while(M<tabM + NB_MASS){
		 M->setup(M,h);
		 M++;
	 }*/

}


/***************************************************************************/
/*																																				 */
/***************************************************************************/
int main(int argc, char **argv)
{
	/* creation de la fenetre - titre et tailles (pixels) */
	g3x_InitWindow(*argv,1024,1024);
	/* param. géométrique de la caméra. cf. gluLookAt(...) */
	g3x_SetPerspective(40.,100.,1.);
	/* position, orientation de la caméra */
	g3x_SetCameraSpheric(0.7*PI,-.75*PI,25.,(G3Xpoint){0.,0.,0.});
		
	g3x_SetInitFunction(init); /* fonction d'initialisation */
	g3x_SetDrawFunction(draw); // fonction de dessin			 */
	g3x_SetAnimFunction(anim); /* fonction d'animation			*/
	/* lancement de la boucle principale */
	return g3x_MainStart();
	return 0;
	/* RIEN APRES CA */
}
