# -*- coding: utf-8 -*-
from tkGraphPad  import MainWindow
from PMat2d		   import *
from GroupLink2d import *
from random      import uniform as uni


xmin,ymin,xmax,ymax = 0,0,+200,+100
Fe  = 100
g	  =	100.
kb,zb	= .9,.9
kp,zp,sp = 7500,10.,1.
r = m = 1.
nbpart= 100
col = ["red","orange","yellow","lightgreen","green","cyan","lightblue","blue","magenta"]
show = 2
t = 0

#____________________FONCTIONS			 ____________________#
#==========================
# Modeleur : Construction de la corde
def Model() :
	''' '''
	PMat = []
	Link = []
	i=0
	# Les particules
	for i in range(0,nbpart) :
		m=r=uni(1.,2.)
		PMat.append(Particule(Point(uni(xmin+r,xmax-r),uni(ymin+r,ymax-r)),m,1./Fe,r,col[i%len(col)]))
		PMat[i].vit	= Vecteur(uni(-75,+75),uni(-75,+75))
		i+=1

	# Perturbation de l'equilibre : vitesse initiale
	# Les bords
	PMat.append(FixLine(Point(	1.,	 1.),Point(199.,	1.),"red"))
	PMat.append(FixLine(Point(199.,	 1.),Point(199., 99.),"red"))
	PMat.append(FixLine(Point(199., 99.),Point(	 1., 99.),"red"))
	PMat.append(FixLine(Point(	1., 99.),Point(	 1.,	1.),"red"))

	# la force de gravité
	Link.append(ConstantForce(PMat,Vecteur(0.,-g)))
	# la liaison inter-particules
	Link.append(RessortFreinInterPart(PMat,kp,zp,sp))
	# la liaison de "choc-plan"
	Link.append(RebondInvKin(PMat,kb,zb,1.)) 

	return PMat,Link
		
#==========================
# fonction animatrice
def anim():
	"""fonction animatrice"""
	for M in PMat : M.setup()
	for L in Link : L.setup()
	# remise à zéro
	#if PMat[0].pos.y<=0 : reset()
		
				
#==========================
# fonction de dessin
def draw() :
	"""fonction de dessin"""
	global t,show
	t+=1
	if t%show : return
	win.clear() # nettoyage
	for M in PMat : M.draw()		
	for L in Link : L.draw()

#____________________PRINCIPAL			 ____________________#
if __name__ == '__main__':
#==========================

	# Démarrage du réceptionnaire d'evenements :
	win=MainWindow("Balistic",1360,680,"lightgrey")
	win.SetDrawZone(xmin,ymin,xmax,ymax)
		 
	PMat,Link = Model()
	
	# scrollbars
	#Fscale=win.CreateScalev(label='F',inf= 10,sup=1000,step= 10.0)
	#Fscale.set(Fe)

	win.anim=anim	
	win.draw=draw
	win.startmainloop()

