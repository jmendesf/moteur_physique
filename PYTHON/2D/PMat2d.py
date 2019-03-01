# -*- coding: utf-8 -*-
from geom				import *
from tkGraphPad	import *

def distPM(M1,M2) :
	return distance(M1.pos,M2.pos)

#____________________classe Point Fixe____________________#
class PointFixe :
		"""initialisatuion d'un point fixe : position, rayon, couleur"""
		def __init__(self,pos,ray,col):
			self.typ=0
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
			fillcircle(self.pos,self.ray,self.col)

		#--------------------------------
		# moteur : rien
		def setup(self) : 
			pass
		#-------------------------------- (A|B) (vecteur AB)
		def __or__(self,M) : return Vecteur(self.pos,M.pos)



#____________________classe Particule____________________#
class FixLine(PointFixe) :
		"""initialisation d'un "bord" AB : pos=A, vit=B, frc=N, ray=d(AB) """
		def __init__(self,A,B,col) :
			# hérite du PointFixe
			AB	= Vecteur(A,B)
			dAB = AB.norm()
			PointFixe.__init__(self,A,dAB,col)
			self.typ = 1
			self.vit = B
			self.frc = (1./dAB)*AB.ortho('+')

		#--------------------------------
		# méthodes de dessin
		def draw(self) :
			M=0.5*(self.pos+self.vit)
			line(M,M+0.01*self.ray*self.frc,"orange",1)
			line(self.pos,self.vit,self.col,1)
			

#____________________classe Particule____________________#
class Particule(PointFixe) :
		"""initialisation d'une particule : position, masse, pas, rayon, couleur"""
		def __init__(self,pos,m,h,ray,col) :
			# hérite du PointFixe
			PointFixe.__init__(self,pos,ray,col)
			self.typ = 2
		# paramètres physiques
			self.h	 = h
			self.m	 = m 

		#--------------------------------
		# moteur : LEAPFROG
		def setup(self) :
			self.vit += self.h/self.m*self.frc
			self.pos += self.h*self.vit
			self.frc	= Vecteur(0,0)
			
