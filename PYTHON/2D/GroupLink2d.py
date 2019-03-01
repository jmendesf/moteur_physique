# -*- coding: utf-8 -*-
from tkGraphPad	import line
from geom				import *
from PMat2d			import *

EPSILON = 0.00000001
# Algo. Choc sur obstacle plan (ici segment) par dynamique inverse !
# 1°) DETECTION DE LA COLLISION																				
#	 <AB,dAB,N> : le segment (extrêmités, long., normale)																						 
#			 <pos>	: la position courante de la particule à tester					 
#								si une collision est détectée, la postion est					 
#								renvoyée sur le point d'interseion avec le segment			 
#			 <vit>	: vecteur déplacement de la particule hors contraintes		
#																																				
#	C'est l'algo classique d'intersection de segment, optimisé						
#	et un peu bricolé pour limiter les fuites....												 
def detection_collision(A,B,dAB,N,P,V) :
	''' '''
	scal= N*V
	# si scal>0, c'est qu'on est dans la mauvaise direction : c'est fini 
	# si scal=0, c'est que la vitesse est parallèle au plan : c'est fini 
	if scal>=0. : return (False,P)

	CORR=0.1	# sans lui, ça peut passe à travers....

	scal=1./scal;
	PA	= Vecteur(P,A)
	# V^PA = N(v)*PA : on transforme un prod.scal. en prod.vect ....
	t   = scal*(V^PA);
	# attention : la normale au plan est normée : il faut réintroduire
	# la longueur du segment, stockée dans le champ ->m							 
	if (t<-CORR or t>dAB+CORR) : return (False,P) 

	t = -scal*(N*PA);
	if (t<-CORR or t>1.+CORR)	:  return (False,P)

	# position : point d'impact : on triche un peu pour rester au dessus 
	t*=(1.-CORR);
	# position du point d'impact
	return (True,(P+t*V) )


def traitement_collision(M1,M2,k,z) :
	''' '''
	p1	= M1.pos
	f1	= M1.frc
	n2	= M2.frc # ATTENTION : ici c'est la normale au segment !!!		
	
	# 1° Détection de la collision : géométrique												
	# les paramètres du segment formant l'obstacle 
	# pour compenser le 'diamètre de la particule, indéxé sur sa masse	
	# on avance un peu le segment, virtuellement												
	A=M2.pos+M1.m*n2
	B=M2.vit+M1.m*n2
	l=M2.ray+M1.m*2

	# On détermine l'intersection éventuelle entre le vecteur vitesse	 
	# de la masse et le plan (segment)																	
	# Si il n'y a pas d'intersection, c'est fini...										 
	# Pour limiter les fuites lors des contacts prolongés, si le test	 
	# avec la position courante échoue, on essaie avec la pos. retardée 

	# ATTENTION : on utilise le déplacement -- PAS LA VITESSE !!!			 
	v1 = M1.h*M1.vit
	flag,p1 = detection_collision(A,B,l,n2,p1,v1)
	# si pas de collision
	if (not flag) : return M1;
	 
	# Si il y a eu collision, la position p1 en sortie est sur l'obstacle 
	# Ca sera la nouvelle position de la particule												
	
	# 2° Traitement de la collision ; cinétique/dynamique inverse		 
	# On détermine la réaction par cinétique/dynamique inverse				

	# ATTENTION : ici, on récupère la vitesse -- PAS LE DEPLACEMENT 
	v1 = M1.vit;
	# on décompose vitesse et force en composantes normale et tangentielle	
	vn = -(n2*v1)*n2
	vt = v1+vn
	fn = -(n2*f1)*n2
	ft = f1+fn
	# on recompose vitesse et force après impact (il faut bien sur 0<k<1 et 0<z<1)						
	v1 = k*vn + z*vt
	f1 = k*fn + z*ft
	# on ajuste l'état de la masse libre après impact						 
	M1.pos = p1
	M1.vit = v1
	M1.frc = f1
	return M1

#___________________________________________________________#
#_______________________classe Liaison______________________#
class Liaison:
	""""""
	def __init__(self,ML,col):
		self.ML	 = ML
		self.frc = Vecteur(0.,0.)
		self.col = col
		if ML==None : return
	#--------------------------------
	def setup(self): pass
	#--------------------------------
	def draw(self) : pass

#___________________________________________________________#
#___________________classe ConstantForce____________________#
class RebondInvKin(Liaison) :

	def __init__(self,ML,k,z,s):
		Liaison.__init__(self,ML,None)
		self.k = k
		self.z = z
		self.s = s
		
	# 2°) TRAITEMENT DE LA COLLISION																			
	#		 Il faut que M1 soit une masse libre et M2 un plan				 
	def setup(self) :
		''' '''
		n = len(self.ML)
		for i in range(0,n-1) :
			if self.ML[i].typ!=2 : continue
			M1=self.ML[i]
			for j in range(i+1,n) :
				if self.ML[j].typ!=1 : continue
				M2=self.ML[j]
				self.ML[i]=traitement_collision(M1,M2,self.k,self.z)

#___________________________________________________________#
#_______________classe RessortFreinInterPart________________#
class RessortFreinInterPart(Liaison) :

	def __init__(self,ML,k,z,s):
		''' '''
		Liaison.__init__(self,ML,None)
		self.k = k
		self.z = z
		self.s = s

	#--------------------------------
	def setup(self):
		''' '''
		global EPSILON
		n = len(self.ML)
		for i in range(0,n-1) :
			if self.ML[i].typ!=2 : continue
			M1=self.ML[i]
			for j in range(i+1,n) :
				if self.ML[j].typ!=2 : continue
				M2=self.ML[j]
				self.l = M1.ray+M2.ray
				d = distPM(M1,M2)
				#a=1
				#if (M1.col==M2.col) : 
				a=self.s
				if d>a*self.l : continue
				e = (1.-self.l/max(EPSILON,d))					
				self.frc = self.k*e*(M1|M2)
				self.frc+= self.z*(M2.vit-M1.vit)
				self.ML[i].frc += self.frc
				self.ML[j].frc -= self.frc

#___________________________________________________________#
#___________________classe ConstantForce____________________#
class ConstantForce(Liaison) :

	def __init__(self,ML,frc):
		Liaison.__init__(self,ML,None)
		self.frc = frc
	 
	#--------------------------------
	def setup(self):
		for M in self.ML :
			if M.typ!=2 : continue
			M.frc += self.frc
