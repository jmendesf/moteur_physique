/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'accès public -           
	                  version 5.0 - Dec.2012                                           
  ============================================================= */

/*===============================================*/
/*=  ASSOCIER DES FONCTIONALITES A DES TOUCHES  =*/
/*===============================================*/
typedef struct
{
  void (*action)();
  char key;
	char info[64];
} G3Xkey;

static int    keynumb=0;
static int    keyblocnumb=0;
static G3Xkey* _KEY_=NULL;

/*= crée une G3Xkey =*/
extern bool g3x_SetKeyAction(char key, void (*action)(void), char *info)
{ 
	if (keyblocnumb==MAXBLOC) 
	{ fprintf(stderr,"\e[1;31m<glx_SetKeyAction>\e[0;31m %d actions : maximum atteint\e[0;31m\n",MAXBLOC*BLOCSIZE); return false;}
  if (keynumb%BLOCSIZE==0)
  { /* si toutes les GLXkey ont été attribuées on ajoute un bloc */	
		if (!(_KEY_=(G3Xkey*)realloc(_KEY_,(keyblocnumb+1)*BLOCSIZE*sizeof(G3Xkey)))) return false;
    memset(_KEY_+keyblocnumb*BLOCSIZE,0,BLOCSIZE*sizeof(G3Xkey));
    keyblocnumb++;
  }  
  _KEY_[keynumb].key    = key; 
  _KEY_[keynumb].action = action; 
	if (info) snprintf(_KEY_[keynumb].info,32,"%s",info);
  keynumb++;
  return true;
}

/*= Exécute l'action associée au car. c =*/
static __inline__ bool g3x_ExecuteKeyAction(char c)
{
  int k=0;
  while (k<keynumb) 
  {
    if (c==_KEY_[k].key) { _KEY_[k].action(); return true; }
    k++;
  }
	return false;
}

/*= Libère les KeyActions =*/
static __inline__ void g3x_FreeKeyAction(void)
{
  if (_KEY_) free(_KEY_); 
  _KEY_=NULL;
  keyblocnumb=0;
  keynumb=0;
}
