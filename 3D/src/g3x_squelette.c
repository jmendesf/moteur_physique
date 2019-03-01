/* ===============================================================
  E.Incerti - Universit� de Marne-la-Vall�e - incerti@univ-mlv.fr 
  ============================================================= */
#include <g3x.h>

int n=50;

void Init(void)
{
}

/*= FONCTION DE DESSIN PRINCIPALE =*/
static void Draw(void)
{
  g3x_Material(G3Xr,0.25,0.5,0.5,0.5,0.);
  glutSolidSphere(1.,n,n);
}


int main(int argc, char** argv)
{   
  /* initialisation de la fen�tre graphique et param�trage Gl */
  g3x_InitWindow(*argv,768,512);
  /* param. g�om�trique de la cam�ra. cf. gluLookAt(...) */
  g3x_SetPerspective(40.,100.,1.);
  /* position, orientation de la cam�ra */
  g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.,(G3Xpoint){0.,0.,0.});

  /* d�finition des fonctions */
  g3x_SetInitFunction(Init);     /* la fonction d'initialisation */
  g3x_SetDrawFunction(Draw);     /* la fonction de dessin        */
  g3x_SetAnimFunction(NULL);		 /* pas de fonction d'animation  */
  g3x_SetExitFunction(NULL);     /* pas de fonction de sortie    */

  g3x_SetScrollWidth(2);	
  g3x_CreateScrollv_i("n",&n,3,100,1.,"r�solution");
  g3x_CreateScrollh_i("n",&n,3,100,1.,"r�solution");
	
  /* boucle d'ex�cution principale */
  return g3x_MainStart();
  /* rien apr�s �a */
}
