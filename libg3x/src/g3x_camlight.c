static GLint    id_repere;

static G3Xpoint  cam_default_pos={5.,5.,5.};
static G3Xpoint  cam_default_tar={0.,0.,0.};
static GLdouble  thetaOeilInit=+0.25*PI,
                 phiOeilInit  =+0.25*PI,
                 distOeilInit = 5.;

static G3Xcamera cam;

extern G3Xcamera* g3x_GetCamera(void) { return &cam; }

extern void g3x_SetCameraTracking(G3Xpoint *position, G3Xpoint *target)
{
	if (position) cam.pos=position;
	if (target  ) cam.tar=target;
}

extern void g3x_SetCameraSpheric(double theta, double phi, double dist, G3Xpoint target)  
{ 
	cam.theta=theta; 
	cam.phi=phi; 
	cam.dist=dist; 
	G3Xcopy(cam_default_tar,target);
	cam_default_pos[0] = cam.dist*cos(cam.theta)*cos(cam.phi);
  cam_default_pos[1] = cam.dist*sin(cam.theta)*cos(cam.phi);
  cam_default_pos[2] = cam.dist*sin(cam.phi);
  cam.upv[2] = cos(cam.phi)>0.?1.0:-1.0;
}

extern void g3x_SetCameraCartesian(G3Xpoint position, G3Xpoint target)  
{ 
	G3Xcopy(cam_default_pos,position);
	G3Xcopy(cam_default_tar,target);
	cam.dist  = G3Xnorm(position);
	cam.phi   = (G3Xiszero(cam.dist   )?0.    :asin(position[2]/cam.dist   ));
	cam.theta = (G3Xiszero(position[1])?0.5*PI:atan(position[0]/position[1]));
}

extern void g3x_SetCameraTheta(double theta) { cam.theta=theta; }
extern void g3x_SetCameraPhi  (double phi  ) { cam.phi=phi;}
extern void g3x_SetCameraDist (double dist ) { cam.dist=dist; }

static GLfloat   _near=20.0,
                 _far =100.0,
								 _open=0.1;

extern void g3x_SetPerspective(float near, float far, float open) 
{ 
	_near=near; 
	_far=far; 
	_open=open;
}


static G3Xlight spot;

static void g3x_DefaultLight(void)
{
	G3Xset(spot.ambi,1.,1.,1.);
  G3Xset(spot.diff,1.,1.,1.);
  G3Xset(spot.spec,1.,1.,1.);
  G3Xset(spot.pos ,10.,10.,10.);
  G3Xset(spot.dir ,0.,0.,0.);
}

extern G3Xlight* g3x_GetLight(void) { return &spot; }

extern void g3x_SetLightSpheric(double theta, double phi, double dist, G3Xpoint target)  
{ 
	spot.theta=theta; 
	spot.phi=phi; 
	spot.dist=dist; 
	G3Xcopy(spot.dir,target);
  spot.pos[0] = spot.dist*cos(spot.theta)*cos(spot.phi);
  spot.pos[1] = spot.dist*sin(spot.theta)*cos(spot.phi);
  spot.pos[2] = spot.dist*sin(spot.phi);
}

extern void g3x_SetLightCartesian(G3Xpoint position, G3Xpoint target)  
{ 
	G3Xvector V;
	G3Xsetvect(V,position,target);
	G3Xcopy(spot.pos,position);
	G3Xcopy(spot.dir,target);
	spot.dist  =  G3Xnorm(V);
	spot.phi   = (G3Xiszero(spot.dist)?0.:asin(spot.pos[2]/spot.dist));
	spot.theta = (G3Xiszero(spot.pos[1])?0.5*PI:atan(spot.pos[0]/spot.pos[1]));
}

extern void g3x_SetSpotSteps(GLfloat dx, GLfloat dy, GLfloat dz)
{
	if (dx!=0.) spot.dx=dx;
	if (dy!=0.) spot.dy=dy;
	if (dz!=0.) spot.dz=dz;
}

extern void g3x_SetLightAmbient  (float r, float g, float b){ G3Xset(spot.ambi,r,g,b); }
extern void g3x_SetLightDiffuse  (float r, float g, float b){ G3Xset(spot.diff,r,g,b); }
extern void g3x_SetLightSpecular (float r, float g, float b){ G3Xset(spot.spec,r,g,b); }
extern void g3x_SetLightPosition (float x, float y, float z){ G3Xset(spot.pos ,x,y,z); }
extern void g3x_SetLightDirection(float x, float y, float z){ G3Xset(spot.dir ,x,y,z); }

extern void g3x_Arrow(GLfloat r, GLfloat h, G3Xcolor col)
{
	static GLUquadricObj *qobj=NULL;
  float  black[4]={0.1,0.1,0.1,0.66};
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glPushMatrix();
    if (!qobj) qobj=gluNewQuadric();
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,black);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,col);
    glMaterialf(GL_FRONT_AND_BACK,GL_SPECULAR,.5);
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluCylinder(qobj,r,r,h,8,2);
    glTranslatef(0.,0.,h);
    glutSolidCone(4.*r,8.*r,8,2);
  glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK,_FILL_?GL_FILL:GL_LINE);
}

extern void g3x_ArrowVect(G3Xpoint O, G3Xvector V, GLfloat r, GLfloat h, G3Xcolor col)
{
	static GLUquadricObj *qobj=NULL;
  float  black[4]={0.1,0.1,0.1,0.66};
  if (!qobj) qobj=gluNewQuadric();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	double l,theta,phi,sinphi;
	l=G3Xnorm(V);
	phi=acos(V[2]/l);
	sinphi=sin(phi);
	theta=(G3Xiszero(sinphi)?0.:(V[1]>0?+1.:-1.)*acos(V[0]/(l*sinphi)));
  glPushMatrix();
		glTranslatef(O[0],O[1],O[2]);
  	glPushMatrix();
			glRotated(RadToDeg*theta,0.,0.,1.);
	  	glPushMatrix();
				glRotated(RadToDeg*phi,0.,1.,0.);
    		glPushMatrix();
					glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,black);
    			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,col);
    			glMaterialf(GL_FRONT_AND_BACK,GL_SPECULAR,0.5);
    			gluQuadricDrawStyle(qobj,GLU_FILL);
    			gluCylinder(qobj,r,r,h*l,8,2);
    			glTranslatef(0.,0.,h*l);
    			glutSolidCone(4.*r,8.*r,8,2);  			
				glPopMatrix();	
  		glPopMatrix();	
  	glPopMatrix();
  glPopMatrix();
}

extern void g3x_Axis(void)
{
	glCallList(id_repere);
}


