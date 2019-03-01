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
	bool *flag;
} G3Xswitch;

static G3Xswitch *_SWITCH_=NULL,*TheSwitch=NULL;
static int       switchnum=0;

static int _switch_id_on_=0,_switch_id_off_=0,_info_id_on_=0,_info_id_off_=0;



/*=  Attribue un numéro et un texte au bouton  =*/
extern bool g3x_CreateSwitch(char *name, bool *flag, char *info)
{
  G3Xswitch *swt;
  char   *c;

  if (!_SWITCH_)
  {
    if (!(_SWITCH_=(G3Xswitch*)calloc(BLOCSIZE,sizeof(G3Xswitch)))) return false;
  }

  swt = _SWITCH_+switchnum;

  strncpy(swt->name,name,MAXBUTLEN);
  if (info) strncpy(swt->info,info,127);
  swt->len = 0;
  c=swt->name;
  while (*c) { swt->len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c); c++; }
  
  Ydialwidth  = MAX(Ydialwidth,swt->len+22);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;
  
  swt->num    = switchnum;
  swt->flag   = flag;
	swt->x      = Xbutpos;
  swt->y      = Ybutpos; Ybutpos+=22; 
  switchnum++;
  return true;
}

/*=  Renvoie le numéro du bouton sélectionné =*/
extern int g3x_GetSwitch(void) { return TheSwitch?(int)TheSwitch->num:-1; }


/*= Si un bouton est sélectionné, son numero =*/
/*= est mis dans la var. glob. <TheSwitch>   =*/
static __inline__ bool g3x_SelectSwitch(int x, int y)
{
  G3Xswitch *swt=_SWITCH_;
  while (swt<_SWITCH_+switchnum)
  {
    if (abs(swt->x-x)<2*Xbutw && abs(swt->y-y)<10)
    { 
      pushmode=GLUT_DOWN;
		 *swt->flag=!*swt->flag;
      return true; 
    }
    swt++;
  }
  return false;
}

/*= libere les bouttons        =*/
static __inline__ void g3x_FreeSwitches()
{ 
  if (_SWITCH_) 
  { 
    free(_SWITCH_); 
    TheSwitch=_SWITCH_=NULL; 
  }
}



static __inline__ void g3x_InitSwitchOn()
{
	int   r  = 12;
	int   rr = r*r;
	float irr= 1./rr;
	int   x,y,yy;
	float d;

	_switch_id_on_ = glGenLists(1);
  glNewList(_switch_id_on_, GL_COMPILE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glBegin(GL_POINTS);
  	for (y=-r; y<=+r; y++)
		{
			yy=y*y;
    	for (x=-r; x<=+r; x++)
    	{
 			 	if ((d=(float)(x*x+yy))>rr) continue;
			 	d=(1.-d*irr);
			 	d=d*d;
			 	glColor4f(0.,d,0,d);
			 	glVertex2i(x,y);
    	}
		}	
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  	glEndList();
}

static __inline__ void g3x_InitSwitchOff()
{
	int   r  = 12;
	int   rr = r*r;
	float irr= 1./rr;
	int   x,y,yy;
	float d;
	
	_switch_id_off_ = glGenLists(1);
  glNewList(_switch_id_off_, GL_COMPILE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glBegin(GL_POINTS);
  	for (y=-r; y<=+r; y++)
		{
			yy=y*y;
    	for (x=-r; x<=+r; x++)
    	{
 			 	if ((d=(float)(x*x+yy))>rr) continue;
			 	d=(1.-d*irr);
			 	d=d*d;
			 	glColor4f(d,0.,0,d);
			 	glVertex2i(x,y);
    	}
		}	
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glEnd();
  	glEndList();
}


static __inline__ void g3x_DrawSwitches(void)
{
  G3Xswitch  *swt=_SWITCH_;
	char *c;
  while (swt<_SWITCH_+switchnum)
	{
		glPushMatrix();
      glTranslatef(+12,curheight-swt->y,0.);
			glCallList(*swt->flag?_switch_id_on_:_switch_id_off_);
    	glColor4f(0.,0.,0.,1.);
    	glRasterPos2i(12,-4); 
    	for (c=swt->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    glPopMatrix();		
		swt++;
	}
}

static void g3x_InitCornSwitchOn()
{
	_info_id_on_ = glGenLists(1);
  glNewList(_info_id_on_, GL_COMPILE);
	/*
		glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);  
      glColor4fv((float*)G3Xk_a);
      glVertex2i(0,0);
      glColor4fv((float*)G3Xw_a);
      glVertex2i(-8,-8);
      glVertex2i(-8,+8);
      glVertex2i(+8,+8);
      glVertex2i(+8,-8);
      glVertex2i(-8,-8);
    glEnd();
	*/	
		int x=-glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,'?')/2;
	  glColor4f(0.33,0.33,0.33,1.);
    glRasterPos2i(x+1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
	  glColor4f(1.,1.,1.,1.);
    glRasterPos2i(x-1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
  glEndList();
}
static void g3x_InitCornSwitchOff()
{
	_info_id_off_ = glGenLists(1);
  glNewList(_info_id_off_, GL_COMPILE);
	/*
		glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);  
      glColor4fv((float*)G3Xw_a);
      glVertex2i(0,0);
      glColor4fv((float*)G3Xk_a);
      glVertex2i(-8,-8);
      glVertex2i(-8,+8);
      glVertex2i(+8,+8);
      glVertex2i(+8,-8);
      glVertex2i(-8,-8);
    glEnd();
	*/	
		int x=-glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,'?')/2;
	  glColor4f(0.33,0.33,0.33,1.);
    glRasterPos2i(x+1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
	  glColor4f(0.,0.,0.,0.);
    glRasterPos2i(x-1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
  glEndList();
}

static bool _info_switch_=false;
static void g3x_DrawInfoSwitch(void)
{
	int x;
	glPushMatrix();
    glTranslatef(Ydialwidth/2,Xdialheight/2,0.);
		glCallList(_info_switch_?_info_id_on_:_info_id_off_);
  glPopMatrix();		
}

