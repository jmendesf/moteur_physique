# -*- coding: utf-8 -*-
from geom       import Point
from tkGraphPad import line

#____________________classe Liaison____________________#
class Liaison:
  """"""
  def __init__(self,M1,M2,col):
    self.M1 = M1
    self.M2 = M2
    self.frc= 0.
    self.col= col
    if M1 and M2 : self.len =M2.pos-M1.pos

  #--------------------------------
  def setup(self):
    if self.M1 : self.M1.frc += self.frc
    if self.M2 : self.M2.frc -= self.frc
  
  #--------------------------------
  # méthodes de dessin
  def draw(self) :
    if self.col==None : return
    line(Point(self.M1.loc,self.M1.pos),Point(self.M2.loc,self.M2.pos),self.col,2)

#____________________classe Ressort____________________#
class Ressort(Liaison) :

  def __init__(self,M1,M2,k):
    Liaison.__init__(self,M1,M2,"blue")
    self.k = k

  #--------------------------------
  def setup(self):
    # distance
    d = self.M2.pos-self.M1.pos
    # force de rappel
    self.frc = self.k*(d-self.len)
    Liaison.setup(self)

#____________________classe Frein____________________#
class Frein(Liaison) :

  def __init__(self,M1,M2,z):
    Liaison.__init__(self,M1,M2,"red")
    self.z = z

  #--------------------------------
  def setup(self):
    # force de freinage cinétique
    self.frc =self.z*(self.M2.vit-self.M1.vit)
    Liaison.setup(self)


#____________________classe RessortFrein____________________#
class RessortFrein(Liaison) :

  def __init__(self,M1,M2,k,z):
    Liaison.__init__(self,M1,M2,"darkviolet")
    self.k = k
    self.z = z

  #--------------------------------
  def setup(self):
    # distance
    d = self.M2.pos-self.M1.pos
    # force de rappel et de freinage cinétique
    self.frc = self.k*(d-self.len)
    self.frc+= self.z*(self.M2.vit-self.M1.vit)
    Liaison.setup(self)

#____________________classe RessortFreinCond____________________#
class RessortFreinCond(Liaison) :

  def __init__(self,M1,M2,k,z,s):
    Liaison.__init__(self,M1,M2,"lightgreen")
    self.k = k
    self.z = z
    self.s = s

  #--------------------------------
  def setup(self):
    if self.col==None : return
    # distance
    d = self.M2.pos-self.M1.pos
    if d>self.s*self.len :
      self.col=None
      return
    # force de rappel et de freinage cinétique
    self.frc = self.k*(d-self.len) + self.z*(self.M2.vit-self.M1.vit)
    Liaison.setup(self)

#____________________classe ConstantForce____________________#
class ConstantForce(Liaison) :

  def __init__(self,M,frc):
    Liaison.__init__(self,M,None,None)
    self.frc = frc
    
  #--------------------------------
  def setup(self):
    Liaison.setup(self)

