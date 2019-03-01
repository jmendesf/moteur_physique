# -*- coding: utf-8 -*-
from tkGraphPad import MainWindow
from PMat2d     import *
from Link2d     import *

#____________________FONCTIONS       ____________________#

#==========================
# Modeleur : Construction de la corde
def Corde(n,Fe,k,z) :
  ''' '''
  dx=10./n # distance entre les masses
  r =dx/4
  PMat = []
  Link = []
  # point fixe #
  PMat.append(PointFixe(Point(-5., 0.),r,"blue"))
  # masses mobiles #
  for i in range(1,n-1) :
    PMat.append(Particule(Point(-5.+i*dx, 0.),1.,1./Fe,r,"red")) 
  # point fixe #
  PMat.append(PointFixe(Point(-5.+(n-1)*dx, 0.),r,"blue")) 

  # Creation et initialisation des liens
  for i in range(n-1) :
    if i==n//2 : Link.append(RessortFreinCond(PMat[i],PMat[i+1],k,z,1.1))
    else       : Link.append(RessortFreinNL(PMat[i],PMat[i+1],k,z))

  # Creation et initialisation des Gravités
  grav = Vecteur(0.,-1.)
  for i in range(1,n-1) :
   Link.append(ConstantForce(PMat[i],grav))

  # Perturbation de l'equilibre : Force
  PMat[1].frc.y=5.*Fe

  return PMat,Link
    
#==========================
# fonction animatrice
def anim():
  """fonction animatrice"""
  for L in Link : L.setup()
  for M in PMat : M.setup()
        
#==========================
# fonction de dessin
def draw():
  """fonction de dessin"""
  win.clear() # nettoyage
  for L in Link : 
    L.draw()    
    L.k=kscale.get() 
    L.z=zscale.get()
  for M in PMat : 
    M.draw()    
    M.h=1./Fscale.get()

#____________________PRINCIPAL       ____________________#
if __name__ == '__main__':
#==========================

  # Démarrage du réceptionnaire d'evenements :
  win=MainWindow("snakes",900,450,"lightgrey")
  win.SetDrawZone(-6,-5,+6,+1)
     
  Fe  = 100
  k   = 5000
  z   = 1
  PMat,Link = Corde(25,Fe,k,z)
  
  # scrollbars
  kscale=win.CreateScalev(label='k',inf=100,sup=20000,step=100.0)
  kscale.set(k)
  zscale=win.CreateScalev(label='z',inf=  0,sup=  10,step=  0.1)
  zscale.set(z)
  Fscale=win.CreateScalev(label='F',inf= 10,sup=1000,step= 10.0)
  Fscale.set(Fe)

  win.anim=anim  
  win.draw=draw
  win.startmainloop()

