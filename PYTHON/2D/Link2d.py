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
def detection_collision(A,B,dAB,N,pos,vit) :
	''' '''
	PA	= Vecteur(pos,A)
	scal= N*vit
	# si scal>0, c'est qu'on est dans la mauvaise direction : c'est fini 
	# si scal=0, c'est que la vitesse est parallèle au plan : c'est fini 
	if scal>=0. : return (False,pos)

	CORRECTEUR=0.5  # sans lui, ça passe à travers....

	scal=1./scal;
	t = (-vit.y*PA.x + vit.x*PA.y)*scal;
	# attention : la normale au plan est normée : il faut réintroduire
	# la longueur du segment, stockée dans le champ ->m							 
	if (t<-CORRECTEUR or t>dAB+CORRECTEUR) : return (False,pos) 

	t = scal*(N*PA);
	if (t<-CORRECTEUR or t>1.+CORRECTEUR)	: return (False,pos)

	# position : point d'impact : on triche un peu pour rester au dessus 
	t*=(1.-CORRECTEUR);
	# position du point d'impact
	pos += t*vit

	return (True,pos)



#___________________________________________________________#
#_______________________classe Liaison______________________#
class Liaison:
	""""""
	def __init__(self,M1,M2,col):
		self.M1 = M1
		self.M2 = M2
		self.frc= Vecteur(0.,0.)
		self.col= col
		if M1==None or M2==None :
			return
		self.l = distPM(M1,M2)
	#--------------------------------
	def setup(self):
		if self.M1 : self.M1.frc += self.frc
		if self.M2 : self.M2.frc -= self.frc

	#--------------------------------
	# méthodes de dessin
	def draw(self) : 
		if self.col==None : return
		line(self.M1.pos,self.M2.pos,self.col,1)


#___________________________________________________________#
#___________________classe ConstantForce____________________#
class RebondInvKin(Liaison) :

	def __init__(self,M1,M2,k,z,s):
		Liaison.__init__(self,M1,M2,None)
		self.k = k
		self.z = z
		self.s = s


	# 2°) TRAITEMENT DE LA COLLISION																			
	#		 Il faut que M1 soit une masse libre et M2 un plan				 
	def setup(self) :
		''' '''
		corr= self.M1.m*self.s;
		p1	= self.M1.pos
		f1	= self.M1.frc
		n2	= self.M2.frc # ATTENTION : ici c'est la normale au segment !!!		
	
		# 1° Détection de la collision : géométrique												
		# les paramètres du segment formant l'obstacle 
		# pour compenser le 'diamètre de la particule, indéxé sur sa masse	
		# on avance un peu le segment, virtuellement												
		A=self.M2.pos+self.M1.m*n2
		B=self.M2.vit+self.M1.m*n2
		l=self.M2.ray+self.M1.m*2

	
		# On détermine l'intersection éventuelle entre le vecteur vitesse	 
		# de la masse et le plan (segment)																	
		# Si il n'y a pas d'intersection, c'est fini...										 
		# Pour limiter les fuites lors des contacts prolongés, si le test	 
		# avec la position courante échoue, on essaie avec la pos. retardée 

		# ATTENTION : on utilise le déplacement -- PAS LA VITESSE !!!			 
		d1 = self.M1.h*self.M1.vit

		flag,p1 = detection_collision(A,B,l,n2,p1,d1)
		if (not flag) : return;
	 
		# Si il y a eu collision, la position p1 en sortie est sur l'obstacle 
		# Ca sera la nouvelle position de la particule												
	
		# 2° Traitement de la collision ; cinétique/dynamique inverse		 
		# On détermine la réaction par cinétique/dynamique inverse				

		# ATTENTION : ici, on récupère la vitesse -- PAS LE DEPLACEMENT 
		v1 = self.M1.vit;

		# on décompose vitesse et force en composantes normale et tangentielle	
		vn = -(n2*v1)*n2
		vt = v1+vn
		fn = -(n2*f1)*n2
		ft = f1+fn
	
		# on recompose vitesse et force après impact (il faut bien sur 0<k<1 et 0<z<1)						
		v1 = self.k*vn + self.z*vt
		f1 = self.k*fn + self.z*ft

		# on ajuste l'état de la masse libre après impact						 
		self.M1.pos = p1
		self.M1.vit = v1
		self.M1.frc = f1

#___________________________________________________________#
#_______________________classe Ressort______________________#
class Ressort(Liaison) :

	def __init__(self,M1,M2,k):
		Liaison.__init__(self,M1,M2,"blue")
		self.k = k

	#--------------------------------
	def setup(self):
		global EPSILON
		d = max(EPSILON,distPM(self.M1,self.M2)) # distance inter-masses 
		e = (1.-self.l/d)				# élongation
		
		# force de rappel
		self.frc = self.k*e*(self.M1|self.M2)
		Liaison.setup(self)

#___________________________________________________________#
#________________________classe Frein_______________________#
class Frein(Liaison) :

	def __init__(self,M1,M2,z):
		Liaison.__init__(self,M1,M2,"red")
		self.z = z

	#--------------------------------
	def setup(self):
		# force de freinage cinétique
		self.frc =self.z*(self.M2.vit-self.M1.vit)
		Liaison.setup(self)


#___________________________________________________________#
#____________________classe RessortFrein____________________#
class RessortFrein(Liaison) :

	def __init__(self,M1,M2,k,z):
		Liaison.__init__(self,M1,M2,"darkviolet")
		self.k = k
		self.z = z

	#--------------------------------
	def setup(self):
		global EPSILON
		d = max(EPSILON,distPM(self.M1,self.M2)) # distance inter-masses 
		e = (1.-self.l/d)												# élongation linéaire	
		
		# force de rappel
		self.frc = self.k*e*(self.M1|self.M2)
		self.frc+= self.z*(self.M2.vit-self.M1.vit)
		Liaison.setup(self)

#___________________________________________________________#
#____________________classe RessortFrein____________________#
class RessortFreinNL(Liaison) :

	def __init__(self,M1,M2,k,z):
		Liaison.__init__(self,M1,M2,"darkviolet")
		self.k = k
		self.z = z

	#--------------------------------
	def setup(self):
		global EPSILON
		d = max(EPSILON,distPM(self.M1,self.M2)) # distance inter-masses 
		d = self.l/d														 # élongation linéaire
		
		# force de rappel NON linéaire
		self.frc = pow(self.k,d)*(1.-d)*(self.M1|self.M2)
		self.frc+= self.z*(self.M2.vit-self.M1.vit)
		Liaison.setup(self)

#___________________________________________________________#
#__________________classe RessortFreinCond__________________#
class RessortFreinCond(Liaison) :

	def __init__(self,M1,M2,k,z,s):
		Liaison.__init__(self,M1,M2,"lightgreen")
		self.k = k
		self.z = z
		self.s = s

	#--------------------------------
	def setup(self):
		if self.col==None : pass # lien brisé
		d = distPM(self.M1,self.M2)
		if d>self.s*self.l :
			self.col=None # on casse le lien
			return
		global EPSILON
		e = (1.-self.l/max(EPSILON,d))					
		self.frc = self.k*e*(self.M1|self.M2)
		self.frc+= self.z*(self.M2.vit-self.M1.vit)
		Liaison.setup(self)

#___________________________________________________________#
#_______________classe RessortFreinInterPart________________#
class RessortFreinInterPart(Liaison) :

	def __init__(self,M1,M2,k,z,s):
		Liaison.__init__(self,M1,M2,None)
		self.k = k
		self.z = z
		self.s = s
		self.l = M1.ray+M2.ray

	#--------------------------------
	def setup(self):
		d = distPM(self.M1,self.M2)
		if d>self.s*self.l : return
		global EPSILON
		e = (1.-self.l/max(EPSILON,d))					
		self.frc = self.k*e*(self.M1|self.M2)
		self.frc+= self.z*(self.M2.vit-self.M1.vit)
		Liaison.setup(self)


#___________________________________________________________#
#___________________classe ConstantForce____________________#
class ConstantForce(Liaison) :

	def __init__(self,M,frc):
		Liaison.__init__(self,M,None,None)
		self.frc = frc
	 
	#--------------------------------
	def setup(self):
		Liaison.setup(self)


