#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "balle.h"
#include "3D_tools.h"
#include "variables.h"

// float teta=0;

void dessinerRaquette();
void dessinersectionmur();
void dessinerballe(Balle balle);
void dessinVie();
void drawBonusVie(Objet bonus);
void drawBonusColle(Objet bonus);
void drawEcranAccueil();
void drawEcranFin();
