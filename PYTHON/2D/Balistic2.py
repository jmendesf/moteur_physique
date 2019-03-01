# -*- coding: utf-8 -*-
from tkGraphPad import MainWindow
from PMat2d		 import *
from Link2d		 import *

Fe = 200
g	 =  20
P0 = Point(1.,0.5)
V0 = Vecteur(+70.,100.)
P1 = Point(110.,2.1)
V1 = Vecteur(0.,0.)
k,z  = 0.9,0.99


#____________________FONCTIONS			 ____________________#
#==========================
# Modeleur : Construction de la corde
def Model() :
	''' '''
	PMat = []
	Link = []
	# Les projectiles
	PMat.append(Particule(P0,2.,1./Fe,2.,"green"))
	PMat.append(Particule(P1,2.,1./Fe,2.,"blue"))
	# Perturbation de l'equilibre : vitesse initiale
	PMat[0].vit	= V0
	PMat[1].vit	= V1
	# Les bords
	PMat.append(FixLine(Point(  0.,  0.),Point(200.,  0.),"red"))
	PMat.append(FixLine(Point(200.,  0.),Point(200.,100.),"red"))
	PMat.append(FixLine(Point(200.,100.),Point(  0.,100.),"red"))
	PMat.append(FixLine(Point(  0.,100.),Point(  0.,  0.),"red"))
	# la force de gravité
	Link.append(ConstantForce(PMat[0],Vecteur(0.,-g)))
	Link.append(ConstantForce(PMat[1],Vecteur(0.,-g)))
	# la liaison inter-particules
	Link.append(RessortFreinInterPart(PMat[0],PMat[1],2000,10,1.))
	# la liaison de "choc-plan"
	Link.append(RebondInvKin(PMat[0],PMat[2],k,z,1.)) 
	Link.append(RebondInvKin(PMat[0],PMat[3],k,z,1.)) 
	Link.append(RebondInvKin(PMat[0],PMat[4],k,z,1.)) 
	Link.append(RebondInvKin(PMat[0],PMat[5],k,z,1.)) 
	Link.append(RebondInvKin(PMat[1],PMat[2],k,z,1.)) 
	Link.append(RebondInvKin(PMat[1],PMat[3],k,z,1.)) 
	Link.append(RebondInvKin(PMat[1],PMat[4],k,z,1.)) 
	Link.append(RebondInvKin(PMat[1],PMat[5],k,z,1.)) 
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
	win.clear() # nettoyage
	for M in PMat : M.draw()		
	for L in Link : L.draw()

#____________________PRINCIPAL			 ____________________#
if __name__ == '__main__':
#==========================

	# Démarrage du réceptionnaire d'evenements :
	win=MainWindow("Balistic",1340,680,"lightgrey")
	win.SetDrawZone(-5,-5,+201,+101)
		 
	PMat,Link = Model()
	
	# scrollbars
	#Fscale=win.CreateScalev(label='F',inf= 10,sup=1000,step= 10.0)
	#Fscale.set(Fe)

	win.anim=anim	
	win.draw=draw
	win.startmainloop()

