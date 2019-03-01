# -*- coding: utf-8 -*-
from geom       import Point
from tkGraphPad import line

#____________________classe Liaison____________________#
class Liaison:
    """"""
    def __init__(self,k,z,l,s,c):
        self.k = k
        self.z = z
        self.l = l
        self.s = s
        self.c = c
    
    def connect(self,M1,M2) :
        if M1==None or M2==None : return
        self.M1=M1
        self.M2=M2
        self.l =M2.pos-M1.pos

    #--------------------------------
    # méthodes de dessin
    def draw(self) :
        if self.M1==None or self.M2==None : return
        line(Point(self.M1.loc,self.M1.pos),Point(self.M2.loc,self.M2.pos),self.c,2)

#____________________classe Ressort____________________#
class Ressort(Liaison) :

    def __init__(self,M1,M2,k):
      self.connect(M1,M2)
      Liaison.__init__(self,k,0.,0.,0.,"red")
    #--------------------------------
    def setup(self):
      d = self.M2.pos-self.M1.pos
      # force de rappel
      f = self.k*(d-self.l)
      self.M1.frc += f
      self.M2.frc -= f

#____________________classe Frein____________________#
class Frein(Liaison) :

    def __init__(self,M1,M2,z):
      self.connect(M1,M2)
      Liaison.__init__(self,0.,z,0.,0.,"blue")
    #--------------------------------
    def setup(self):
      # composante en x de la force
      f =self.z*(self.M2.vit-self.M1.vit)
      self.M1.frc += f
      self.M2.frc -= f


#____________________classe RessortFrein____________________#
class RessortFrein(Liaison) :

    def __init__(self,M1,M2,k,z):
      self.connect(M1,M2)
      Liaison.__init__(self,k,z,0.,0.,"purple")
    #--------------------------------
    def setup(self):
      d = self.M2.pos-self.M1.pos
      # force de rappel
      f = self.k*(d-self.l)+self.z*(self.M2.vit-self.M1.vit)
      self.M1.frc += f
      self.M2.frc -= f

#____________________classe Global Force____________________#
class ExternalForce(Liaison) :

    def __init__(self,M,f):
      self.M1=M
      self.M2=None
      Liaison.__init__(self,0.,0.,0.,f,"purple")
      
    #--------------------------------
    def setup(self):
      self.M1.frc += self.s

