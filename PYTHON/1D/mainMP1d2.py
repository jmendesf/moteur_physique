# -*- coding: utf-8 -*-
from math       import sqrt
from tkGraphPad import MainWindow
from PMat1d     import *
from Link1d2    import *

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
  PMat.append(PointFixe(0.,-5.,r,"blue"))
  # masses mobiles #
  for i in range(1,n-1) :
    PMat.append(Particule(0.,1.,1./Fe,-5.+i*dx,r,"red")) 
  # point fixe #
  PMat.append(PointFixe(0.,-5.+(n-1)*dx,r,"blue")) 

  # Creation et initialisation des RessortFrein
  for i in range(n-1) :
   Link.append(RessortFrein(PMat[i],PMat[i+1],k,z))
   
  # Creation et initialisation des Gravités
  grav = -10
  for i in range(1,n-1) :
   Link.append(ConstantForce(PMat[i],grav))

  # Perturbation de l'equilibre : Force
  PMat[1].frc=100.*Fe

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
  win=MainWindow("Corde 1D",900,450,"lightgrey")
  win.SetDrawZone(-6,-3,+6,+3)
     
  Fe  = 100
  k   = 1000
  z   = 1
  PMat,Link = Corde(25,Fe,k,z)
  
  # scrollbars
  kscale=win.CreateScalev(label='k',inf=100,sup=2000,step=100.0)
  kscale.set(k)
  zscale=win.CreateScalev(label='z',inf=  0,sup=  10,step=  0.1)
  zscale.set(z)
  Fscale=win.CreateScalev(label='F',inf= 10,sup=1000,step= 10.0)
  Fscale.set(Fe)

  win.anim=anim  
  win.draw=draw
  win.startmainloop()

