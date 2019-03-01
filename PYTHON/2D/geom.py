# -*- coding: utf-8 -*-
from math		 import sqrt

#____________________classe Point	____________________#
class Point():
		"""deux coordonnées (x,y)"""
		def __init__(self,x,y) : 
			self.x=x
			self.y=y

		def sqrdist(self,p)  : return (self.x-p.x)*(self.x-p.x)+(self.y-p.y)*(self.y-p.y)
		def dist(self,p)		 : return sqrt((self.x-p.x)*(self.x-p.x)+(self.y-p.y)*(self.y-p.y))
		def __add__(self,V)  : return Point(self.x+V.x,self.y+V.y)
		def __sub__(self,V)  : return Point(self.x-V.x,self.y-V.y)
		def __rmul__(self,a) : return Point(a*self.x,a*self.y)

#____________________distance entre 2 points		____________________#
def distance(A,B) : return A.dist(B)

#____________________classe Vecteur	____________________#
class Vecteur:
		"""deux coordonnées (x,y)"""
		def __init__(self,u,v):
				if (type(u)==Point and type(v)==Point) :
						self.x=v.x-u.x
						self.y=v.y-u.y
				else :		
						self.x=u
						self.y=v
		#-------------------------------- (-u)					 
		def __neg__(self)		: return Vecteur(-self.x,-self.y)
		#-------------------------------- (u+v)					 
		def __add__(self,v)	: return Vecteur(self.x+v.x,self.y+v.y)
		#-------------------------------- (u-v)
		def __sub__(self,v)	: return Vecteur(self.x-v.x,self.y-v.y)
		#-------------------------------- (a*u)
		def __rmul__(self,a) : return Vecteur(a*self.x,a*self.y)
		#-------------------------------- (u*v) (produit scalaire)
		def __mul__(self,v)	: return (self.x*v.x+self.y*v.y)
		#-------------------------------- (u^v) (produit vectoriel 2D)
		def __xor__(self,v)	: return (self.x*v.y-self.y*v.x)
		#-------------------------------- ||u||²
		def sqrnorm(self)		: return self*self
		#-------------------------------- ||u||
		def norm(self)			 : return sqrt(self.sqrnorm())
		#-------------------------------- vecteur normalisé
		def normalized(self) :
			n=self.norm()
			if (n>0.) : return (1./n)*self
		#-------------------------------- vecteur orthogonal
		def ortho(self,sign) : 
			if (sign=='+') : return Vecteur(-self.y,+self.x)
			else					 : return Vecteur(+self.y,-self.x)
		#--------------------------------
		def draw(self,origin,col,e):
			return win.line(origin,origin+self,col,e)

#____________________classe Vecteur	____________________#

