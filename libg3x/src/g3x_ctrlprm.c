/* ===============================================================
  E.Incerti - Universit� de Marne-la-Vall�e - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'acc�s public -           
	                  version 5.0 - Dec.2012                                           
  ============================================================= */

/*=========================================*/
/*=  GESTION DU CONTROLE DE PARAMETRES    =*/
/*=========================================*/

/*= Il  y a 2 types de param�tres contr�lable : entier et r�el  =*/
typedef struct {double  r; int  i;} G3Xparam;
typedef struct {double* r; int* i;} G3Xadres;
typedef struct
{
  char    ope;         /* arithm.('+') ou g�om.('*')   */
  char    nom[16];     /* nom du param�tre (pour menu) */
  G3Xadres var;         /* adresse du param�tre         */
  G3Xparam pas,min,max; /* pas et bornes pour param�tre */
	G3Xparam org,val,old; /* valeurs originale, courante et pr�c�dente */
} G3Xprm;

static int     prmnum=0;
static int     prmblocnumb=0;
static G3Xprm *_PRM_=NULL,*ThePrm=NULL;

/*= Ajoute un nouveau param�tre de contr�le de type <double> =*/
extern bool g3x_SetControlParameter_d(double* prm, char* nom, double pas, double min, double max, char mode)
{ 
	static G3Xprm *newptr;
	if (prmblocnumb==MAXBLOC) 
	{ fprintf(stderr,"\e[1;31m<g3x_SetControlParameter>\e[0;31m %d param�tres : maximum atteint\e[0m\n",MAXBLOC*BLOCSIZE); return false;}
  if (prmnum%BLOCSIZE==0)
  { /* si toutes les G3Xprm ont �t� attribu�es on ajoute un bloc */	
		if (!(_PRM_=(G3Xprm*)realloc(_PRM_,(prmblocnumb+1)*BLOCSIZE*sizeof(G3Xprm)))) return false;
    memset(_PRM_+prmblocnumb*BLOCSIZE,0,BLOCSIZE*sizeof(G3Xprm));
    prmblocnumb++;
  }
	newptr = _PRM_+prmnum;
  strncpy(newptr->nom,nom,15); newptr->nom[15]=0x0;
  newptr->ope   = mode; 
  newptr->var.r = prm;
  newptr->pas.r = pas;
  newptr->min.r = min;
  newptr->max.r = max;
  newptr->org.r =*prm;
  newptr->val.r =*prm;
  newptr->old.r =*prm;
	prmnum++;
  return true;
}
/*= Ajoute un nouveau param�tre de contr�le de type <int> =*/
extern bool g3x_SetControlParameter_i(int   * prm, char* nom, int    pas, int    min, int    max, char mode)
{ 	static G3Xprm *newptr;
	if (prmblocnumb==MAXBLOC) 
	{ fprintf(stderr,"<g3x_SetControlParameter> %d param�tres : maximum atteint\n",MAXBLOC*BLOCSIZE); return false;}
  if (prmnum%BLOCSIZE==0)
  { /* si toutes les G3Xprm ont �t� attribu�es on ajoute un bloc */	
		if (!(_PRM_=(G3Xprm*)realloc(_PRM_,(prmblocnumb+1)*BLOCSIZE*sizeof(G3Xprm)))) return false;
    memset(_PRM_+prmblocnumb*BLOCSIZE,0,BLOCSIZE*sizeof(G3Xprm));
    prmblocnumb++;
  }
	newptr = _PRM_+prmnum;
  strncpy(newptr->nom,nom,15); newptr->nom[15]=0x0;
  newptr->ope   = mode; 
  newptr->var.i = prm;
  newptr->pas.i = pas;
  newptr->min.i = min;
  newptr->max.i = max;
  newptr->org.i =*prm;
  newptr->val.i =*prm;
  newptr->old.i =*prm;
	prmnum++;
  return true;
}


extern bool g3x_ChangeControlParameter(void *param)
{
	if (!ThePrm) return false;
	if (param==(void*)ThePrm->var.r && ThePrm->old.r!=ThePrm->val.r)
	{
		ThePrm->old.r=ThePrm->val.r;
		return true;
	}	
	if (param==(void*)ThePrm->var.i && ThePrm->old.i!=ThePrm->val.i)
	{
		ThePrm->old.i=ThePrm->val.i;
		return true;
	}	
	return false;
}

/*= Lib�re les Param�tres de contr�le =*/
static __inline__ void g3x_FreeCtrlPrm(void)
{
  if (_PRM_) free(_PRM_); 
  _PRM_=NULL;
  prmblocnumb=0;
  prmnum=0;
}



static void g3x_PrintParam(void)
{
	if (_INFO_) return;
	if (ThePrm!=_PRM_+prmnum && ThePrm->var.r)
	fprintf(stderr,"\e[1;33mParam�tre : \e[0;35m\"%s\" : \e[0;31m %+3.3f<\e[0;41m%+3.3f<\e[0;31m%+3.3f                    \r"
	              ,ThePrm->nom,ThePrm->min.r,*(ThePrm->var.r),ThePrm->max.r);
	else if (ThePrm!=_PRM_+prmnum && ThePrm->var.i)
	fprintf(stderr,"\e[1;33mParam�tre : \e[0;35m\"%s\" : \e[0;31m %+d<\e[0;41m%+d\e[0;31m<%+d                         \r"
	              ,ThePrm->nom,ThePrm->min.i,*(ThePrm->var.i),ThePrm->max.i);
	else
	fprintf(stderr,"\e[1;33mParam�tre : \e[0;35mvis�e  : \e[0;31m[%+3.3f,%+3.3f,%+3.3f]      \r"
	              ,(*cam.tar)[0],(*cam.tar)[1],(*cam.tar)[2]);
}
