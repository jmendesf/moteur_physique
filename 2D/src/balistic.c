#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <g2x.h>

#define g 10.0

/* window dimensions */
double xmin=-5.,ymin=-5.,xmax=+201.,ymax=+101.;


double m=1.0; /* ball mass */
double r=2.5; /* ball ray  */
/* 'ground' parameters for inverse kinetics collision */
double kc=0.75,zc=0.95; 
double za=0.0; /* air viscosity */

/* initial position and speed */
G2Xpoint  P0=(G2Xpoint) { 0.,24.};
G2Xvector V0=(G2Xvector){8.,32.};
/* current position and speed */
G2Xpoint  P,Q;
/* speed, resulting force, gravity */
G2Xvector V,F,G;
/* sampling rate for images */
double dt=0.1;
/* simulation time step */
double h =0.001;
/* ration FAFF=(int)(dt/h) */
int    FAFF;


/*=================================================================*/
/*=================================================================*/
void Analytic(G2Xpoint* p0, G2Xvector* v0, double *t0)
{
	G2Xpoint  p,q,v;	
	double t  = *t0;
	double ti;
	g2x_Circle(p0->x,p0->y,r,G2Xwc,1);
	q=*p0;
	do
	{
		if (G2Xzero(za)) 
		{ 
			p.x = p0->x + v0->x*t;
			p.y = p0->y + v0->y*t- 0.5*g*t*t;		
			v.x = v0->x;
			v.y = v0->y - g*t;		
		}
		else
		{
			double w=m/za,gw=g*w;
			double e   = exp(-t/w);
			p.x = p0->x + w*(   v0->x)*(1.-e)       ;
			p.y = p0->y + w*(gw+v0->y)*(1.-e) - gw*t;		
			v.x = v0->x*e;
			v.y = (gw+v0->y)*e-gw;
		}
		g2x_Line(q.x,q.y,p.x,p.y,G2Xwa,1);
		g2x_Plot(p.x,p.y,G2Xwb,3);
		q=p;
		t+=dt;
	} while (p.y+v.y*dt>r);
  p0->x = p.x+v.x*dt;
	p0->y = r;
	g2x_Line(q.x,q.y,p0->x,p0->y,G2Xwa,1);

	v0->x =  zc*v.x;
	v0->y = -kc*v.y;
}

/*=================================================================*/
/*=================================================================*/
void Background(void)
{
	double u=0;
	G2Xpoint  p=P0;	
	G2Xvector v=V0;		
	g2x_Axes();
	do 
		Analytic(&p,&v,&u);
	while (!G2Xzero(v.x) && p.x<xmax);
}

void Init(void)
{
	P=P0;
	V=V0;
	G.x=0;
	G.y=-g;
	FAFF=(int)(dt/h);
	/* produit un affichage (draw) tous les 'FAFF' pas de simulation */
  g2x_SetRefreshFreq(FAFF);
}

void reset(void)
{	
	V = V0;
	P = P0;
}

/*=================================================================*/
/*=================================================================*/
void Anim(void)
{  
	FAFF=(int)(dt/h);
	/* produit un affichage (draw) tous les 'FAFF' pas de simulation */
  g2x_SetRefreshFreq(FAFF);
  
	F.x=G.x-(za/m)*V.x;
	F.y=G.y-(za/m)*V.y;
	
	V.x += h*F.x; 
	V.y += h*F.y; 

	P.x += h*V.x;
	P.y += h*V.y;


	if (P.y<=r) 	/* collision detection */
	{
		V.x *=  zc;  
		V.y *= -kc; /* collision treatment : inverse kinetics */
		P.y  = r;
	}
	if (P.x>=0.95*xmax) reset();
}

void Draw(void)
{
	Background();
	g2x_FillCircle(P.x,P.y,r,G2Xr);
	g2x_StaticPrint(10,10,G2Xr,'l',"dt=%.1e  h=%.1e  Faff=%d",dt,h,FAFF);
}


/*===========================================================================*/
/*=                                                                         =*/
/*===========================================================================*/
int main(int argc, char* argv[])
{
  uint   pixwidth=1340,pixheight=680;
	  
  /* Boucle de récupération des arguments dans n'importe quel ordre */  
  g2x_InitWindow("Tir Balistique",pixwidth,pixheight);
  g2x_SetWindowCoord(xmin,ymin,xmax,ymax);

	/*g2x_SetBkgFunction(Background);*/
  g2x_SetInitFunction(Init);
	g2x_SetDrawFunction(Draw);  
	g2x_SetAnimFunction(Anim);  


	g2x_CreateScrollv_d("kc",&kc,0.,1.,.5,"kc");
	g2x_CreateScrollv_d("zc",&zc,0.,1.,.5,"zc");
	g2x_CreateScrollv_d("za",&za,0.,1.,2.,"za");
	g2x_CreateScrollh_d("dt",&dt,0.01,0.5,1.,"dt");
	g2x_CreateScrollh_d(" h",&h ,0.00001,1.,1.,"h");
	g2x_CreatePopUp("reset",reset,"reset");

	g2x_MainStart();
	
  return 0;
}
