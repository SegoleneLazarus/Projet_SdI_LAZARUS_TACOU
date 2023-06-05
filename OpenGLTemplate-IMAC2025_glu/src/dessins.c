#include "dessins.h"


void dessinersectionmur()
{
	// rect Haut
		glPushMatrix();
			glColor3f(100/255,10/255,10/255);
			glTranslatef(0,0,4.9);
			glScalef(1./2.,15,1);
			drawSquare();
		glPopMatrix();
		// rect Bas
		glPushMatrix();
			glColor3f(100/255,10/255,10/255);
			glTranslatef(0,0,-5+0.1);
			glScalef(1./2.,15,1);
			drawSquare();
		glPopMatrix();		

		// rect Droit
		glPushMatrix();
			glColor3f(100/255,10/255,10/255);
			glTranslatef(0,15./2.-0.1,0);
			glRotatef(90.0,1.,0.,0.);			

			glScalef(1./2.,10,1);
			
			drawSquare();
		glPopMatrix();	

		// rect Gauche
		glPushMatrix();
			glColor3f(100/255,10/255,10/255);
			glTranslatef(0,-15./2.+0.1,0);
			glRotatef(90.0,1.,0.,0.);
			glScalef(1./2.,10,1);
			drawSquare();
		glPopMatrix();
}

void dessinerRaquette(){
		glPushMatrix();
			glRotatef(90.0,0.,1.,0.);
			glTranslatef(-4./3.,0,0);
			glScalef(4,4,2);
			drawSquare();
		glPopMatrix();
}

void dessinerballe(Balle balle)
{
	glColor3f(100/255,10/255,10/255);
	glRotatef(90.,0,1,0);
	// glScalef(balle.rayon,balle.rayon,balle.rayon);
	glTranslatef(balle.zpos,balle.ypos,balle.xpos-1);// parfois la logique elle même cesse de régenter; 
	drawCarre();
}

void dessinVie(){
	glColor3f(0/255,0/255,0/255);
	glPushMatrix();
			glRotatef(90.0,0.,1.,0.);
			glTranslatef(-4,9,-20);
			glScalef(0.8,1.5,0.7);
			drawCarre();
	glPopMatrix();
}

void drawBonusVie(Objet bonus){
	glColor3f(200/255,200/255,200/255);
	glRotatef(90.0,0.,1.,0.);
	glPushMatrix();
		glRotatef(teta, 0, 0, 1);
		glTranslatef(bonus.zpos,bonus.ypos,bonus.xpos);
		glScalef(1.2,1.2,1.2);
		drawPyramide();
	glPopMatrix();
}

void drawBonusColle(Objet bonus){
	glColor3f(200/255,200/255,200/255);
	glRotatef(90.0,0.,1.,0.);
	glPushMatrix();
		glRotatef(teta, 0, 0, 1);
		glTranslatef(bonus.zpos,bonus.ypos,bonus.xpos);
		glScalef(1.2,1.2,1.2);
		drawOctoedre();
	glPopMatrix();
}

void drawEcranAccueil(){

	glColor3f(100/255,100/255,100/255);
	glPushMatrix();
		glRotatef(90.0,0.,1.,0.);
		glTranslatef(0.,0,-20.);
		glScalef(3.,8,2);
		drawCarre();
	glPopMatrix();
}

void drawEcranFin(){

	glColor3f(100/255,100/255,100/255);
	glPushMatrix();
		glRotatef(90.0,0.,1.,0.);
		glTranslatef(0.,0,-20.);
		glScalef(4.,8,2);
		drawCarre();
	glPopMatrix();
}