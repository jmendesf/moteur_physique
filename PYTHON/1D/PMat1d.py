# -*- coding: utf-8 -*-
from geom       import Point
from tkGraphPad import circle

#____________________classe Point Fixe____________________#
class PointFixe :
    """initialisation d'un point fixe : position, localisation, rayon, couleur"""
    def __init__(self,pos,loc,ray,col):
      # paramètres physiques
      self.pos=pos
      self.vit=0.
      self.frc=0.
      # paramètres d'affichage
      self.col=col # couleur
      self.ray=ray # rayon
      self.loc=loc # pour localiser

    #--------------------------------
    # méthodes de dessin
    def draw(self) : 
      circle(Point(self.loc,self.pos),self.ray,self.col,1)

    #--------------------------------
    # moteur : rien
    def setup(self) : 
      pass

#____________________classe Particule____________________#
#_____________hérite de la classe PointFixe______________#
class Particule(PointFixe) :
    """initialisation d'une particule : position, masse, pas, localisation, rayon, couleur"""
    def __init__(self,pos,m,h,loc,ray,col) :
      # paramètres physiques
      self.m  = 1./m
      # hérite du PointFixe
      PointFixe.__init__(self,pos,loc,ray,col)

    #--------------------------------
    # moteur : LEAPFROG
    def setup(self) :
      global h
      self.vit += self.h*self.frc*self.m
      self.pos += self.h*self.vit
      self.frc  = 0.

