# -*- coding: utf-8 -*-
from tkGraphPad import MainWindow
from PMat2d		 import *
from Link2d		 import *

Fe = 100
g	 =  20
P0 = Point(0.1,0.1)
V0 = Vecteur(50.,100.)
k,z  = 0.6,0.6

#____________________FONCTIONS			 ____________________#
#==========================
# Modeleur : Construction de la corde
def Model() :
	''' '''
	PMat = []
	Link = []
	# Le projectile
	PMat.append(Particule(P0,2.,1./Fe,2.,"green"))
	# Perturbation de l'equilibre : vitesse initiale
	PMat[0].vit	= V0
	# Les bords
	PMat.append(FixLine(Point(  0.,  0.),Point(200.,  0.),"red"))
	PMat.append(FixLine(Point(200.,  0.),Point(200.,100.),"red"))
	PMat.append(FixLine(Point(200.,100.),Point(  0.,100.),"red"))
	PMat.append(FixLine(Point(  0.,100.),Point(  0.,  0.),"red"))
	# la force de gravité
	Link.append(ConstantForce(PMat[0],Vecteur(0.,-g)))
	# la liaison de "choc-plan"
	Link.append(RebondInvKin(PMat[0],PMat[1],k,z,1.)) 
	Link.append(RebondInvKin(PMat[0],PMat[2],k,z,1.)) 
	Link.append(RebondInvKin(PMat[0],PMat[3],k,z,1.)) 
	Link.append(RebondInvKin(PMat[0],PMat[4],k,z,1.)) 
	return PMat,Link
		
#==========================
# remise à zéro
def reset():
	PMat[0].pos = P0 
	PMat[0].vit = V0	

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

