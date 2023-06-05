#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include "3D_tools.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;




/* Window properties */
static const unsigned int WINDOW_WIDTH = 1100;
static const unsigned int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "La Route des Vacances";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* IHM flag */
static int flag_animate_rot_scale = 0;
static int flag_animate_rot_arm = 0;

/* Error handling function */
void onError(int error, const char* description)
{
	fprintf(stderr, "GLFW Error: %s\n", description);
}

void onWindowResized(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,aspectRatio,Z_NEAR,Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}

float teta = 0;
float rectOpacity = 0.5f;
int nombredobstacle=8;
Objet objettab[100];
int nombredemur=32;
double lastMouseX = 0.0;
double lastMouseY = 0.0;
// Position du rectangle
float rectPositionZ = 0.0f;
float rectPositionY = 0.0f;
float xsectionmur = 0;
bool clic=false;
float avancement_depuis_dernier_clic=10;
int bouton; 
int MEGA_DRAPEAU=0; 
float nombre_de_vies=20;
const int nombre_reserve_obstacle=10;
Objet reserve_obstacles[nombre_reserve_obstacle];
float avancement_total=0;

float valeur_absolue(float nombre);

// Sensibilité du mouvement de la souris
const float SENSITIVITY = 0.02f;

Balle attraperballe (Balle balle, float rectPositionY,float rectPositionZ);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
     double deltaY = ypos - lastMouseY;
	 double deltaX = xpos - lastMouseX;

    // Met à jour la position du rectangle en fonction du mouvement de la souris sur l'axe Z
    rectPositionZ += deltaX * SENSITIVITY;
	rectPositionY += deltaY * SENSITIVITY;

    // Met à jour la position précédente de la souris
    lastMouseX = xpos;
    lastMouseY = ypos;
}


void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_A :
			case GLFW_KEY_ESCAPE :
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			case GLFW_KEY_L :
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				break;
			case GLFW_KEY_P :
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
			case GLFW_KEY_R :
				flag_animate_rot_arm = 1-flag_animate_rot_arm;
				break;
			case GLFW_KEY_T :
				flag_animate_rot_scale = 1-flag_animate_rot_scale;
				break;
			case GLFW_KEY_KP_9 :
				if(dist_zoom<100.0f) dist_zoom*=1.1;
				printf("Zoom is %f\n",dist_zoom);
				break;
			case GLFW_KEY_KP_3 :
				if(dist_zoom>1.0f) dist_zoom*=0.9;
				printf("Zoom is %f\n",dist_zoom);
				break;
			case GLFW_KEY_UP :
				if (phy>2) phy -= 2;
				printf("Phy %f\n",phy);
				break;
			case GLFW_KEY_DOWN :
				if (phy<=88.) phy += 2;
				printf("Phy %f\n",phy);
				break;
			case GLFW_KEY_LEFT :
				theta -= 5;
				break;
			case GLFW_KEY_RIGHT :
				theta += 5;
				break;
			default: fprintf(stdout,"Touche non gérée (%d)\n",key);
		}
	}
}

void light(Objet objettab[],int nombredemur,int nombredobstacle,Balle balle)
{
	
	for(int i=0;i<nombredemur+nombredobstacle;i++)
	{
		//caméra source lumineuse
		objettab[i].lumiere=(objettab[i].xpos+25)*3/255;

		//balle source lumineuse

		objettab[i].lumiere+=valeur_absolue(objettab[i].xpos-balle.xpos)*1.5/255;
		if(i>=nombredemur && balle.xpos>objettab[i].xpos) objettab[i].lumiere+=30./255;

		if(valeur_absolue(objettab[i].xpos-balle.xpos)>10) objettab[i].lumiere+=20./255;
		else 
		{
			objettab[i].lumiere+=valeur_absolue(objettab[i].ypos-balle.ypos)/255;
			objettab[i].lumiere+=valeur_absolue(objettab[i].zpos-balle.zpos)/255;
		} 
		
		
	}
}

float valeur_absolue(float nombre)
{
	return sqrt(nombre*nombre);
}

Balle balle=Balle(-19,-5,-2,-0.4,0.2,0.2,true,1);

Balle deplacementballe(Balle balle,float rectPositionY,float rectPositionZ,GLFWwindow* window,double* xposmousebuttoncallback,double*yposmousebuttoncallback)
{
	if(balle.attrapee)
	{
		bouton=-1;
		balle=attraperballe(balle,-rectPositionZ,-rectPositionY);
			
		bouton = glfwGetMouseButton	(window,GLFW_MOUSE_BUTTON_RIGHT); //
		if (bouton==GLFW_PRESS)
		{
			bouton=-1;
			balle.attrapee=false;
			balle.vitx=0.4f;
			balle.xpos=-19+balle.rayon;
			balle.xpos+=balle.vitx;
		}
		return balle;
	}

	if (balle.ypos>=15./2-balle.rayon || balle.ypos<=-15./2+balle.rayon)
	{
		balle.vity=-balle.vity;
		balle.ypos+=balle.vity;
	}
	if (balle.zpos>=5-balle.rayon || balle.zpos<=-5+balle.rayon)
	{
		balle.vitz=-balle.vitz;
		balle.zpos+=balle.vitz;
	}

	if (balle.xpos<=-20+balle.rayon)
	{
		if (balle.zpos < (rectPositionY + 3.5) && balle.zpos > (rectPositionY - 3.5) && balle.ypos < (-rectPositionZ + 3.5) && balle.ypos > (-rectPositionZ - 3.5))
		{
			balle.vitx = -balle.vitx;
			balle.xpos += balle.vitx;

			float distanceFromCenter = balle.ypos + rectPositionZ; // Calcul de la distance entre la position de la balle et le centre de la raquette
			float distanceFromCenterz= balle.zpos + rectPositionY;
			balle.vity += distanceFromCenter * 0.03f;
			balle.vitz -= distanceFromCenterz * 0.03f;
		}	
		else 
		{	
			balle=attraperballe(balle,-rectPositionZ,-rectPositionY);
			balle.attrapee=true;
			nombre_de_vies-=4;
			if (nombre_de_vies==0)
			{
				MEGA_DRAPEAU=0;
				avancement_total=0;
				nombre_de_vies=20;
				xsectionmur+=40;
				balle.attrapee=true;
			}

		}	
	}

	// mtn avec les obstacles : on va considérer que la balle rebondit sur les obstacles comme si elle était un carré (sans compter les rebonds bizarres sur les coins) 
	for (int i=nombredemur;i<nombredemur+nombredobstacle;i++)
	{
		Objet obstacle=objettab[i];
		float hauteur=obstacle.sizex;
		float largeur=obstacle.sizey;
		float centrey=obstacle.ypos;
		float centrez=obstacle.zpos;
		float posy=balle.ypos;
		float posz=-balle.zpos;
		float rayon=balle.rayon;
		if (posy-rayon<centrey+largeur/2 && posy+rayon>centrey-largeur/2 && posz+rayon>centrez-hauteur/2 && posz-rayon<centrez+hauteur/2)// ca veut dire que la balle est devant ou derrière le mur ; 
			if((balle.xpos+rayon>=obstacle.xpos && balle.xpos<=obstacle.xpos) || (balle.xpos-rayon<=obstacle.xpos && balle.xpos>=obstacle.xpos)) // la balle touche ou pénètre le mur de par un côté où de l'autre
			{
				if (obstacle.bonus==0)
				{
					balle.vitx=-balle.vitx;
					balle.xpos+=balle.vitx;
				}
				
			}
	}
	
	if (balle.xpos>=19)balle.vitx=-balle.vitx;
	if (balle.attrapee==false)
	{
	balle.xpos+=balle.vitx;
	balle.ypos+=balle.vity;
	balle.zpos+=balle.vitz;
	}

	if (balle.vity>0.4)balle.vity-=0.01;
	if (balle.vity<-0.4)balle.vity+=0.01;
	if (balle.vitz>0.4)balle.vitz-=0.01;
	if (balle.vitz<-0.4)balle.vitz+=0.01;

	return balle;

	
}

void dessinerballe(Balle balle)
{
	glColor3f(100/255,10/255,10/255);
	glRotatef(90.,0,1,0);
	// glScalef(balle.rayon,balle.rayon,balle.rayon);
	glTranslatef(balle.zpos,balle.ypos,balle.xpos-1);// parfois la logique elle même cesse de régenter; 
	drawCarre();
}

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



static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &xpos, &ypos);
}


GLFWmousebuttonfun glfwSetMouseButtonCallback	(	GLFWwindow * 	window,
GLFWmousebuttonfun 	cbfun )	;
void glfwGetCursorPos	(	GLFWwindow * 	window,
double * 	xpos,
double * 	ypos 
);


int glfwGetMouseButton	(	GLFWwindow * 	window,
int 	button 
);	


Balle attraperballe (Balle balle, float rectPositionY,float rectPositionZ)
{
	balle.vitx=0;
	balle.vity=0;
	balle.vitz=0;
	balle.xpos=-18.5;
	balle.ypos=-rectPositionZ;
	balle.zpos=rectPositionY;
	return balle;
}

void deplacementobstacles(float xsectionmur,int nombredemur)
{
	Objet obstacle;
	for(int i=0;i<nombredobstacle;i++)
	{
		obstacle=objettab[nombredemur+i];
		if (xsectionmur<=80.-10.*i) obstacle.xpos=60-10*i-xsectionmur;
		else obstacle.xpos=140-10*i-xsectionmur;
		if(obstacle.xpos>=58)
		{
			objettab[nombredemur+i]=reserve_obstacles[(int)rand()%nombre_reserve_obstacle];
			if(objettab[nombredemur+i].bonus==1 || objettab[nombredemur+i].bonus==2)
			{
				objettab[nombredemur+i].ypos=6-rand()%15;
				objettab[nombredemur+i].zpos=4-rand()%8;
			}
			objettab[nombredemur+i].xpos=obstacle.xpos;
			objettab[nombredemur+i].lumiere=obstacle.lumiere;
		}
		else objettab[nombredemur+i]=obstacle;
	}
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


Balle avancer(float *xsectionmur,bool *clic,float *avancement_depuis_dernier_clic,Balle balle)
{
	//quand le joueur clic gauche, le jeu avance de 5 unité mais en combien de temps? lors du clic on passe un bool à vrai, on créer une variable avancement qui lorsqu'elle atteint 5 est remise à 0 et passe le bool à faux alors xsectionmur arrête d'avancer, si on clic pendant l'avancement cela ne fait rien 
	if (not balle.attrapee)
	{
		if (*xsectionmur>=80.f)*xsectionmur-=80.f;
		if (*clic){
		*avancement_depuis_dernier_clic=0;
		}
		if(*avancement_depuis_dernier_clic<10)
		{
			*avancement_depuis_dernier_clic+=0.4f;
			*xsectionmur+=0.4f;
			avancement_total+=0.4f;
			if (avancement_total>=250)
			{
				*xsectionmur+=40;
				avancement_total=0;
				nombre_de_vies=20;
				balle.attrapee=true;
				MEGA_DRAPEAU=2;
			}
			bool flag_obstacle=false;
			for (int i=nombredemur;i<nombredemur+nombredobstacle;i++)
			{
				Objet obstacle=objettab[i];
				float hauteur=obstacle.sizex;
				float largeur=obstacle.sizey;
				float centrey=obstacle.ypos;
				float centrez=obstacle.zpos;
				float posy=balle.ypos;
				float posz=balle.zpos;
				float rayon=balle.rayon;
				if (obstacle.bonus!=0)
				{
					if (posy-rayon<centrey+largeur/2 && posy+rayon>centrey-largeur/2 && posz+rayon>centrez-hauteur/2 && posz-rayon<centrez+hauteur/2 && obstacle.xpos<=19.4)
					{
						if(obstacle.bonus==1)
						{
							if(nombre_de_vies<20)nombre_de_vies+=4;
						}
						else balle.attrapee=true;
					}
				}
				else
				{
					if (posy-rayon<centrey+largeur/2 && posy+rayon>centrey-largeur/2 && posz+rayon>centrez-hauteur/2 && posz-rayon<centrez+hauteur/2)// ca veut dire que la balle est devant ou derrière le mur ; 
					{
						if((balle.xpos+rayon>=obstacle.xpos && balle.xpos<=obstacle.xpos) || (balle.xpos-rayon<=obstacle.xpos && balle.xpos>=obstacle.xpos)) // la balle touche ou pénètre le mur de par un côté où de l'autre
						{
							flag_obstacle=true;
						}
					}
				}
				
			}
			if (balle.xpos>=-19+balle.rayon && not flag_obstacle)balle.xpos-=0.4f;
		}
	
		*clic=false;
	}
	else balle=attraperballe(balle,rectPositionY,rectPositionZ);
	return balle;
	
}

GLuint genTexture(char* filePath){
		// stbi_set_flip_vertically_on_load(true);
		int x, y, n; //largeur, hauteur et nb de canaux par pixel
		unsigned char* image = stbi_load(filePath,&x,&y,&n,STBI_rgb_alpha); //contient les valeurs des pixels

		GLuint texture;
		glGenTextures(1, &texture); // genere de l'espace pour 1 texture

		glBindTexture(GL_TEXTURE_2D, texture); //openGl fonctionne par etat: on lui dit qu'on est actuellement en train de Wer avec telle texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // configure le comportement de la texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);// donne concretement les pixels a la texture
		glBindTexture(GL_TEXTURE_2D, 0); //bonne habitude de code: detacher a chaque fois la texture de son point de bind
		
		stbi_image_free(image);

		return texture;
	}

int main(int argc, char** argv)/////////////////////////////////////////////////////////////
{
	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);

	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);

	glPointSize(5.0);
	glEnable(GL_DEPTH_TEST);

	

	

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);

	// créer les objets 
	double xposmousebuttoncallback,yposmousebuttoncallback; 

	//créer mur 
	Objet mur;
	// mur.nom="mur";
	mur.sizex=10.;
	mur.sizez=1.;
	mur.rotatey=0;
	mur.rotatez=0;
	float xpos=55.;

	for (int i=0; i<nombredemur;i+=4)
	{
		mur.sizey=15.;
		mur.anglerotate=0;
		mur.rotatex=0;
		mur.xpos=xpos;
		mur.ypos=0;

		//rect haut
		mur.r=100.0/255.;
		mur.v=100.0/255.;
		mur.b=255.0/255.;
		mur.zpos=5.;
		mur.position=0;
		objettab[i]=mur;

		// rect Bas
		mur.r=100.0/255.;
		mur.b=100.0/255.;
		mur.v=255.0/255.;
		mur.zpos=-5.;
		mur.position=2;
		objettab[i+1]=mur;


		// rect Droit
		mur.sizey=10.;
		mur.r=250.0/255;
		mur.b=100.0/255;
		mur.v=100.0/255;
		mur.zpos=0;
		mur.anglerotate=90.0;
		mur.rotatex=1.;

		mur.ypos=15./2.;
		mur.position=3;
		objettab[i+2]=mur;

		// rect Gauche
		mur.ypos=-15./2.;
		mur.position=4;
		objettab[i+3]=mur;

		xpos-=10.;
	}

	//créer obstacles 

	Objet obstacle;
	//moitié gauche
	obstacle.r=255./255;
	obstacle.v=255./255;
	obstacle.b=255./255;
	obstacle.lumiere=0.;
	obstacle.sizex=10;
	obstacle.sizey=15./2;
	obstacle.sizez=1;
	obstacle.xpos=0;
	obstacle.ypos=15./4;
	obstacle.zpos=0;
	obstacle.anglerotate=90.0;
	obstacle.rotatex=0;
	obstacle.rotatey=1.0;
	obstacle.rotatez=0;
	reserve_obstacles[0]=obstacle;

	//moitié droite
	obstacle.ypos=-15./4;
	reserve_obstacles[1]=obstacle;

	//moitié basse
	obstacle.ypos=0;
	obstacle.zpos=-5./2;
	obstacle.sizex=5;
	obstacle.sizey=15;
	reserve_obstacles[2]=obstacle;

	//moitié haute 
	obstacle.zpos=5./2;
	reserve_obstacles[3]=obstacle;

	//moitié haute décalée au centre
	obstacle.zpos=0;
	obstacle.sizex=3;
	reserve_obstacles[4]=obstacle;
	reserve_obstacles[9]=obstacle;
	
	//moitié gauche décalée au centre
	obstacle.zpos=0;
	obstacle.sizex=10;
	obstacle.sizey=15./2-2;
	reserve_obstacles[5]=obstacle;

	Objet bonus;
	bonus.bonus=1;
	/*bonus*/
	reserve_obstacles[6]=bonus;

	bonus.bonus=2;
	/*bonus*/
	reserve_obstacles[7]=bonus;

	/*pas d'obstacle*/
	Objet pas_obstacle;
	reserve_obstacles[8]=pas_obstacle;

	for (int i=0;i<nombredobstacle;i++) objettab[nombredemur+i]=reserve_obstacles[(int)rand()%nombre_reserve_obstacle];
	

	
	
	
	
	// Activate transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// TEXTUREs

	/*preparation des textures */
	GLuint textures[15];
	char filePath[]="../doc/nerd.png";
	char filePath2[]="../doc/menu_debut.png";
	char filePath3[]="../doc/menu_debut_souris.png";

	char filePath4[]="../doc/2020.png";
	char filePath5[]="../doc/1620.png";
	char filePath6[]="../doc/1220.png";
	char filePath7[]="../doc/0820.png";
	char filePath8[]="../doc/0420.png";
	char filePath9[]="../doc/0020.png";

	char filePath10[]="../doc/souris.png";

	char filePath11[]="../doc/menu_fin.png";
	char filePath12[]="../doc/menu_fin_souris.png";

	char filePath13[]="../doc/ciel.png";
	char filePath14[]="../doc/route.png";
	char filePath15[]="../doc/obstacles.png";

	textures[0]=genTexture(filePath);
	textures[1]=genTexture(filePath2);
	textures[2]=genTexture(filePath3);

	textures[3]=genTexture(filePath4);
	textures[4]=genTexture(filePath5);
	textures[5]=genTexture(filePath6);
	textures[6]=genTexture(filePath7);
	textures[7]=genTexture(filePath8);
	textures[8]=genTexture(filePath9);

	textures[9]=genTexture(filePath10);

	textures[10]=genTexture(filePath11);
	textures[11]=genTexture(filePath12);

	textures[12]=genTexture(filePath13);
	textures[13]=genTexture(filePath14);
	textures[14]=genTexture(filePath15);
	

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.0,0.0,0.0,0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();

		
		/* Initial scenery setup */
		
			

		if(MEGA_DRAPEAU==0)
		{
			/*menu*/
			//ecran accueil

			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,textures[9]);
					glPushMatrix();
						glTranslatef(-20,-rectPositionZ,-rectPositionY);
						glScalef(0.1,0.1,0.1);
						dessinerRaquette();
					glPopMatrix();

				glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);

			float Zsouris=-rectPositionY;
			float Ysouris=-rectPositionZ;
			if (Ysouris<8 && Ysouris>-8 && Zsouris>-2.5 && Zsouris<2.5)
			{
				/*change sprite pcq c pas bon le sprite*/
				glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D,textures[2]);

					glPushMatrix();
						drawEcranAccueil();
					glPopMatrix();

					glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);

				bouton = glfwGetMouseButton	(window,GLFW_MOUSE_BUTTON_LEFT);
				if (bouton==GLFW_PRESS)
				{
					bouton=-1;
					MEGA_DRAPEAU=1;
					
				}
				
			}

			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,textures[1]);

				glPushMatrix();
					drawEcranAccueil();
				glPopMatrix();

				glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);

			
			
			
			//printf("Y %f\n",-rectPositionZ);
			//printf("Z %f\n",-rectPositionY);

		}
		else if(MEGA_DRAPEAU==1)
		{
			
			/*jeu*/
		
			/* Scene rendering */

			//déplacement et affichage
			light(objettab,nombredemur,nombredobstacle,balle);

			deplacementobstacles(xsectionmur,nombredemur);

			//dessiner_objets
			for (int i=0;i<nombredemur+nombredobstacle;i++)
				{
					if(i<nombredemur){
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D,textures[13]);
							mur=objettab[i];//plus un mur mais mur ou obstacle
							glPushMatrix();
								glTranslatef(mur.xpos,mur.ypos,mur.zpos);
								glRotatef(mur.anglerotate,mur.rotatex,mur.rotatey,mur.rotatez);
								glScalef(mur.sizex,mur.sizey,mur.sizez);
								drawMur(mur);
							glPopMatrix();

						glBindTexture(GL_TEXTURE_2D, 0);
						glDisable(GL_TEXTURE_2D);
					}
					else{
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D,textures[14]);
							mur=objettab[i];//plus un mur mais mur ou obstacle
							glPushMatrix();
								glTranslatef(mur.xpos,mur.ypos,mur.zpos);
								glRotatef(mur.anglerotate,mur.rotatex,mur.rotatey,mur.rotatez);
								glScalef(mur.sizex,mur.sizey,mur.sizez);
								drawMur(mur);
							glPopMatrix();
						glBindTexture(GL_TEXTURE_2D, 0);
						glDisable(GL_TEXTURE_2D);
					}

					

					
					if(mur.bonus==2)
					{
						glPushMatrix();
							drawBonusColle(mur);
						glPopMatrix();
					}
					if(mur.bonus==1)
					{
						glPushMatrix();
							drawBonusVie(mur);
						glPopMatrix();
					}
				}

			//sectionmur 
			bouton = glfwGetMouseButton	(window,GLFW_MOUSE_BUTTON_LEFT); //
			if (bouton==GLFW_PRESS)
			{
				clic=true;
			}
			bouton=-1;
			balle = avancer(&xsectionmur,&clic,&avancement_depuis_dernier_clic,balle);
			
			for(int i=0;i<nombredobstacle;i++)
			{
				glPushMatrix();
				if (xsectionmur<=80.-10.*i) glTranslatef(60-10*i-xsectionmur,0,0);
				else glTranslatef(140-10*i-xsectionmur,0,0);
				dessinersectionmur();
				glPopMatrix();
			}

			// score
			switch (static_cast<int>(nombre_de_vies))
			{
			case 20:
				glEnable(GL_TEXTURE_2D);

					glBindTexture(GL_TEXTURE_2D,textures[3]);
						glPushMatrix();
							dessinVie();
						glPopMatrix();
					glBindTexture(GL_TEXTURE_2D, 0);

				glDisable(GL_TEXTURE_2D);
				break;

			case 16:
				glEnable(GL_TEXTURE_2D);

					glBindTexture(GL_TEXTURE_2D,textures[4]);
						glPushMatrix();
							dessinVie();
						glPopMatrix();
					glBindTexture(GL_TEXTURE_2D, 0);

				glDisable(GL_TEXTURE_2D);
				break;

			case 12:
				glEnable(GL_TEXTURE_2D);

					glBindTexture(GL_TEXTURE_2D,textures[5]);
						glPushMatrix();
							dessinVie();
						glPopMatrix();
					glBindTexture(GL_TEXTURE_2D, 0);

				glDisable(GL_TEXTURE_2D);
				break;

			case 8:
				glEnable(GL_TEXTURE_2D);

					glBindTexture(GL_TEXTURE_2D,textures[6]);
						glPushMatrix();
							dessinVie();
						glPopMatrix();
					glBindTexture(GL_TEXTURE_2D, 0);

				glDisable(GL_TEXTURE_2D);
				break;

			case 4:
				glEnable(GL_TEXTURE_2D);

					glBindTexture(GL_TEXTURE_2D,textures[7]);
						glPushMatrix();
							dessinVie();
						glPopMatrix();
					glBindTexture(GL_TEXTURE_2D, 0);

				glDisable(GL_TEXTURE_2D);
				break;

			case 0:
				glEnable(GL_TEXTURE_2D);

					glBindTexture(GL_TEXTURE_2D,textures[8]);
						glPushMatrix();
							dessinVie();
						glPopMatrix();
					glBindTexture(GL_TEXTURE_2D, 0);

				glDisable(GL_TEXTURE_2D);
				break;
			
			default:
				break;
			}

			// bonus test
			// glPushMatrix();
			// 	drawBonusVie(-xsectionmur);
			// glPopMatrix();
			

			//balle 
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,textures[0]);
			
				
				glPushMatrix();
					dessinerballe(balle);
				glPopMatrix();

				glBindTexture(GL_TEXTURE_2D, 0);

			glDisable(GL_TEXTURE_2D);

			

			balle=deplacementballe(balle,rectPositionY,rectPositionZ,window,&xposmousebuttoncallback,&yposmousebuttoncallback);

			//raquette 
			glPushMatrix();
				glColor4f(10/255,10/255,10/255,rectOpacity);
				glTranslatef(-20,-rectPositionZ,-rectPositionY);
				// glTranslatef(0, xpos,ypos);
				dessinerRaquette();
			glPopMatrix();

		}
		else if(MEGA_DRAPEAU==2){
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,textures[9]);
					glPushMatrix();
						// glColor4f(200./255.,200./255.,200./255.,1);
						glTranslatef(-20,-rectPositionZ,-rectPositionY);
						glScalef(0.1,0.1,0.1);
						// glTranslatef(0, xpos,ypos);
						dessinerRaquette();
					glPopMatrix();

				glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);

			float Zsouris=-rectPositionY;
			float Ysouris=-rectPositionZ;
			if (Ysouris<8 && Ysouris>-8 && Zsouris>-2.5 && Zsouris<0.)
			{
				/*change sprite pcq c pas bon le sprite*/
				glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D,textures[11]);

					glPushMatrix();
						drawEcranFin();
					glPopMatrix();

					glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);

				bouton = glfwGetMouseButton	(window,GLFW_MOUSE_BUTTON_LEFT);
				if (bouton==GLFW_PRESS)
				{
					bouton=-1;
					MEGA_DRAPEAU=1;
					
				}
				
			}

			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,textures[10]);

				glPushMatrix();
					drawEcranFin();
				glPopMatrix();

				glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			

		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}

		/* Animate scenery */
		teta+=1.;
		// teta+=0.1;
		double tempspasse = glfwGetTime() - startTime;

		while(tempspasse < FRAMERATE_IN_SECONDS){
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
			tempspasse = glfwGetTime() - startTime;
		}
	}

	//draw
	// stbi_image_free(imageBalle);
	// stbi_image_free(imageMenu);
	// glDeleteTextures(1, &tex1);
	// glDeleteTextures(2, &tex2);

	glDisable(GL_BLEND);
	
	

	glfwTerminate();
	return 0;
}
