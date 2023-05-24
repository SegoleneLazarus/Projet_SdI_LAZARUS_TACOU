#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Needed before including GL/glu.h 
// to avoid a compilation error because of CALLBACK pre-processor definition.
// See https://stackoverflow.com/questions/19198244/how-do-i-get-rid-of-these-compiler-errors-in-glu-h#comment28407913_19198244
// It's usually defined in a system header to define the default calling convention for callbacks, 
// but if it is defined incorrectly this sort of thing happens.
// windef.h usually defines it is __stdcall
#if defined(_WIN64) || defined(_WIN32)
    #include <windef.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

// important to add this line to use M_PI
#define _USE_MATH_DEFINES
#include <math.h>

#define NB_SEG_CIRCLE 64

typedef struct Balle{
	float xpos;
	float ypos;
	float zpos;
	float vitx;
	float vity;
	float vitz;
	bool attrapee;
	Balle() {};
	Balle(float xpos, float ypos, float zpos,float vitx,float vity,float vitz,bool attrapee) 
	{
        this->xpos = xpos;
        this->ypos = ypos;
		this->zpos = zpos; 
		this->vitx = vitx;
		this->vity = vity;
		this->vitz = vitz;
		this->attrapee=attrapee;
	}
} Balle;
typedef struct Objet{
	// char nom;
	float xpos;
	float ypos;
	float zpos;
	float r;
	float v;
	float b;
	float sizex;
	float sizey;
	float sizez;
	float anglerotate;
	float rotatex;
	float rotatey;
	float rotatez;
	float lumiere;

	Objet() {} // là un constructeur vide

    Objet(float xpos, float ypos, float zpos, float r, float v, float b, float sizex, float sizey, float sizez, float anglerotate, float rotatex, float rotatey, float rotatez,float lumiere) {
        this->xpos = xpos;
        this->ypos = ypos;
        this->zpos = zpos;
		this->r = r;
        this->v = v;
        this->b = b;
		this->sizex = sizex;
        this->sizey = sizey;
        this->sizez = sizez;
		this->anglerotate = anglerotate;
        this->rotatex = rotatex;
        this->rotatey = rotatey;
		this->rotatez = rotatez;
		this->lumiere = lumiere;
    }
}Objet;

/* Camera parameters and functions */
static const float Z_NEAR = 0.1f;
static const float Z_FAR = 100.f;
extern float theta;      // Angle between x axis and viewpoint
extern float phy  ;      // Angle between z axis and viewpoint
extern float dist_zoom ; // Distance between origin and viewpoint

void setCamera();

/* Draw cannonic objet functions */
void drawSquare();

void drawCircle();

void drawCone();

void drawSphere();

void drawMur(Objet mur);

/* Small tools */
float toRad(float deg);
