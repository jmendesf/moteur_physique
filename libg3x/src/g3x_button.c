/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'accès public -           
                    version 5.0 - Dec.2012                                           
  ============================================================= */

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/ 

#define MAXBUTLEN  15

typedef struct
{
  int  num; /* numéro attribué au bouton      */
  int  x,y; /* coordonnées du bouton          */
  int  len;
  char name[MAXBUTLEN+1];
	char info[128];
  bool on;
} G3Xbut;

static G3Xbut    *_BUTTON_=NULL,*TheButton=NULL;
static int       butnum=0;

/*=  Attribue un numéro et un texte au bouton  =*/
extern int g3x_CreateButton(char *name, char *info)
{
  G3Xbut *but;
  char   *c;

  if (!_BUTTON_)
  {
    if (!(_BUTTON_=(G3Xbut*)calloc(BLOCSIZE,sizeof(G3Xbut)))) return false;
  }

  but = _BUTTON_+butnum;
  

  strncpy(but->name,name,MAXBUTLEN);
  if (info) strncpy(but->info,info,127);
  but->len = 0;
  c=but->name;
  while (*c) { but->len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c); c++; }
  
  Ydialwidth  = MAX(Ydialwidth,but->len+8);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;
  
  but->num  = butnum++;
  but->x    = Xbutpos;
  but->y    = Ybutpos; Ybutpos+=16; 
  but->on   = false;
  return true;
}

/*=  Renvoie le numéro du bouton sélectionné =*/
extern int g3x_GetButton(void) { return TheButton?(int)TheButton->num:-1; }

/*=  Débranche tous les boutons =*/
extern void g3x_ReleaseButton(void) 
{ 
  if (!TheButton) return; 
  TheButton->on=false; 
  TheButton=NULL; 
}


/*= Si un bouton est sélectionné, son numero =*/
/*= est mis dans la var. glob. <TheButton>   =*/
static __inline__ bool g3x_SelectButton(int x, int y)
{
  G3Xbut *but=_BUTTON_;
  while (but<_BUTTON_+butnum)
  {
    if (abs(but->x-x)<2*Xbutw && abs(but->y-y)<10)
    { 
      pushmode=GLUT_DOWN;
      if (TheButton!=NULL) {                TheButton->on=false; }
      if (TheButton!=but ) { TheButton=but; TheButton->on=true;  }
      else TheButton=NULL;
      return true; 
    }
    but++;
  }
  return false;
}

/*= libere les bouttons        =*/
static __inline__ void g3x_FreeButtons()
{ 
  if (_BUTTON_) 
  { 
    free(_BUTTON_); 
    TheButton=_BUTTON_=NULL; 
  }
}

/*=  dessinne tous les boutons  =*/
static __inline__ void g3x_DrawButtons(void)
{
  G3Xbut  *but=_BUTTON_;
	char *c;
  float a;
  while (but<_BUTTON_+butnum)
	{
		glPushMatrix();
      glTranslatef(Xbutpos,curheight-but->y,0.);
			glCallList(but->on?_popup_id_on_:_popup_id_off_);
    	glColor4f(0.,0.,0.,1.);
    	glRasterPos2i(12,-4); 
			a=(but->on?1.:0.);
    	glColor4f(a,a,a,1.);
    	glRasterPos2i(-but->len/2,-4); 
    	for (c=but->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    	glRasterPos2i(1-but->len/2,-4); 
    	for (c=but->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    glPopMatrix();		
		but++;
	}
}



