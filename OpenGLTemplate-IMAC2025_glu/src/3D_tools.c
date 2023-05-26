#include "3D_tools.h"

/* Camera parameters and functions */
float theta = 45.0f; // Angle between x axis and viewpoint
float phy = 60.0f; // Angle between z axis and viewpoint
float dist_zoom = 30.0f; // Distance between origin and viewpoint

void setCamera() {
	gluLookAt(-35.,0.,0.,
			  0.0,0.0,0.0,
			  0.0,0.0,1.0);
}

/* Convert degree to radians */
float toRad(float deg) {
	return deg*M_PI/180.0f;
}
//gl color , vertex3fglnormal dans le glbegin,gltextcoord

void drawMur(Objet mur){
	glBegin(GL_TRIANGLE_FAN);
		float r=mur.r-mur.lumiere; 
		if(r>1)r=1;
		if(r<0)r=0;
		float v=mur.v-mur.lumiere;
		if(v>1)v=1;
		if(v<0)v=0;
		float b=mur.b-mur.lumiere;
		if(b>1)b=1;
		if(b<0)b=0;
		glColor3f(r,v,b);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,-0.5,0.0);
		glVertex3f(0.5,0.5,0.0);
		glVertex3f(-0.5,0.5,0.0);
	glEnd();
}

void drawSquare() {
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,-0.5,0.0);
		glVertex3f(0.5,0.5,0.0);
		glVertex3f(-0.5,0.5,0.0);
	glEnd();
}

void drawCircle() {
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0,0.0,0.0);
		float step_rad = 2*M_PI/(float)NB_SEG_CIRCLE;
		for(int i=0;i<=NB_SEG_CIRCLE;i++) {
			glVertex3f(cos(i*step_rad),sin(i*step_rad),0.0f);
		}
	glEnd();
}

void drawCone() {
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0,0.0,1.0);
		float step_rad = 2*M_PI/(float)NB_SEG_CIRCLE;
		for(int i=0;i<=NB_SEG_CIRCLE;i++) {
			glVertex3f(cos(i*step_rad),sin(i*step_rad),0.0f);
		}
	glEnd();
}

void drawSphere() {
	gluSphere(gluNewQuadric(),1.0,NB_SEG_CIRCLE,NB_SEG_CIRCLE);
}
