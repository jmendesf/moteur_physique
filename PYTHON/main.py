# -*- coding: utf-8 -*-
from math       import *
from geom       import *
from tkGraphPad import *

#____________________classe Point Fixe____________________#
class PointFixe :
    """initialisatuion d'un point fixe : position, rayon, couleur"""
    def __init__(self,pos,ray,col):
      # paramètres physiques
      self.pos=pos
      self.vit=Vecteur(0,0)
      self.frc=Vecteur(0,0)
      # paramètres d'affichage
      self.col=col # couleur
      self.ray=ray # rayon

    #--------------------------------
    # méthodes de dessin
    def draw(self) : 
      circle(self.pos,self.ray,self.col,1)

    #--------------------------------
    # moteur : rien
    def setup(self) : 
      pass
    #-------------------------------- (A|B) (vecteur AB)
    def __or__(self,M) : return Vecteur(self.pos,M.pos)


#____________________classe Particule____________________#
class Particule(PointFixe) :
    """initialisation d'une particule : position, masse, pas, rayon, couleur"""
    def __init__(self,pos,m,ray,col) :
      # paramètres physiques
      self.m  = m
      # hérite du PointFixe
      PointFixe.__init__(self,pos,ray,col)

    #--------------------------------
    # moteur : LEAPFROG
    def Leapfrog(self) :
      self.vit = self.vit + h*(-G-(z/self.m)*self.vit)
      self.pos = self.pos + h*self.vit

    def EulerExp(self) :
      self.pos = self.pos + h*self.vit
      self.vit = self.vit + h*(-G-(z/self.m)*self.vit)

    def EulerImp(self) :
      self.vit = (self.m/(self.m+h*z))*(self.vit-h*G)
      self.pos = self.pos + h*self.vit
      

P0=Point(0,.5)
V0=Vecteur(5,9)
m =1
z =0.01
g =-10
dt=0.05
h =0.001
G=Vecteur(0,-g)
A=Particule(P0,m,0.05,"red");
A.vit=V0
B=Particule(P0,m,0.05,"blue");
B.vit=V0
C=Particule(P0,m,0.05,"green");
C.vit=V0
 

      
#____________________FONCTIONS       ____________________#

#==========================
# Modeleur : Construction -- "statique"
def Modeleur() :
  ''' '''
  pass
    
#==========================
# fonction animatrice
def anim():
  """fonction animatrice"""
  h= hscale.get() 
  A.EulerExp()
  B.Leapfrog()
  C.EulerImp()


def analytic(P0,V0) :
  w=m/z
  t=0
  P=P0
  while (P.y>=0):
    e = exp(-t/w)  
    P=Point(P0.x+w*(     V0.x)*(1.-e),
            P0.y+w*(-g*w+V0.y)*(1.-e)+g*w*t)
    V=Vecteur(V0.x*e,
              (-g*w+V0.y)*e+g*w)
    circle(P,.02,"black",1)
    t+=dt
        
#==========================
# fonction de dessin
def draw():
  """fonction de dessin"""
  win.clear() # nettoyage 
  dt=dtscale.get() 
  z= zscale.get() 
  analytic(P0,V0) 
  A.draw()
  B.draw()
  C.draw()

#____________________PRINCIPAL       ____________________#
if __name__ == '__main__':
#==========================

  # Démarrage du réceptionnaire d'evenements :
  win=MainWindow("Corde 1D",900,450,"lightgrey")
  win.SetDrawZone(-0.1,-0.1,+10.1,+5.1)
      
  Modeleur()    
  
  # scrollbars
  dtscale=win.CreateScalev(label='dt',inf=0.01,sup=0.1,step=0.002)
  dtscale.set(dt)
  zscale=win.CreateScalev(label='z',inf=0.01,sup=0.2,step=0.002)
  zscale.set(z)
  hscale=win.CreateScalev(label='h',inf=0.01,sup=0.1,step=0.001)
  hscale.set(h)

  win.anim=anim  
  win.draw=draw
  win.startmainloop()

