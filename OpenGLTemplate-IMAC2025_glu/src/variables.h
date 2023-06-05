#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
// #include "balle.h"
#include "3D_tools.h"

extern int bouton; 
extern int MEGA_DRAPEAU; 
extern float nombre_de_vies;
constexpr int nombre_reserve_obstacle=10;
// extern Objet reserve_obstacles[nombre_reserve_obstacle];
extern float avancement_total;
extern float teta;
extern float rectPositionZ;
extern float rectPositionY;
extern int nombredobstacle;
extern Objet objettab[100];
extern int nombredemur;


extern float xsectionmur;

extern float avancement_depuis_dernier_clic;