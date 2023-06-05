#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "balle.h"
#include "3D_tools.h"
#include "variables.h"

// int bouton; 
// int MEGA_DRAPEAU=0; 
// float nombre_de_vies=20;
// const int nombre_reserve_obstacle=10;
// Objet reserve_obstacles[nombre_reserve_obstacle];
// float avancement_total=0;
// float teta = 0;
// float rectPositionZ = 0.0f;
// float rectPositionY = 0.0f;
// int nombredobstacle=8;
// Objet objettab[100];
// int nombredemur=32;

// float xsectionmur = 0;
// bool clic=false;
// float avancement_depuis_dernier_clic=10;

Balle avancer(float *xsectionmur,bool *clic,float *avancement_depuis_dernier_clic,Balle balle);

Balle attraperballe (Balle balle, float rectPositionY,float rectPositionZ);

Balle deplacementballe(Balle balle,float rectPositionY,float rectPositionZ,GLFWwindow* window,double* xposmousebuttoncallback,double*yposmousebuttoncallback);