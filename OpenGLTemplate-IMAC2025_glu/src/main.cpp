#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "3D_tools.h"
#include "draw_scene.h"
using namespace std;


int nombredobstacle=5;
Objet objettab[100];
int nombredemur=32;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 900;
static const unsigned int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "TD04 Ex01";
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

float rectOpacity = 0.5f;
// Texture du rectangle
unsigned int textureRaqu;
// Charge une texture depuis un fichier
// unsigned int loadTexture(const char* filename)
// {
//     unsigned int textureID;
//     glGenTextures(1, &textureID);

//     int width, height, nrChannels;
//     unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
//     if (data)
//     {
//         GLenum format;
//         if (nrChannels == 1)
//             format = GL_RED;
//         else if (nrChannels == 3)
//             format = GL_RGB;
//         else if (nrChannels == 4)
//             format = GL_RGBA;

//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);

//         stbi_image_free(data);
//     }
//     else
//     {
//         // std::cerr << "Erreur lors du chargement de la texture" << std::endl;
//         stbi_image_free(data);
//         return 0;
//     }

//     return textureID;
// }

double lastMouseX = 0.0;
double lastMouseY = 0.0;
// Position du rectangle
float rectPositionZ = 0.0f;
float rectPositionY = 0.0f;
float xsectionmur = 0;
bool clic=false;
float avancement_depuis_dernier_clic=0;
int bouton; 

float valeur_absolue(float nombre);



// Sensibilité du mouvement de la souris
const float SENSITIVITY = 0.02f;

Balle attraperballe (GLFWwindow * 	window,double*xposadresse,double*yposadresse,Balle balle, float rectPositionY,float rectPositionZ);

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
		if(i>=nombredemur && balle.xpos>objettab[i].xpos)
		{
			objettab[i].lumiere+=30./255;
		}
		
		
	}
}

float valeur_absolue(float nombre)
{
	return sqrt(nombre*nombre);
}

Balle balle=Balle(0,-5,-2,-0.4,0.2,0.2,false,1);

Balle deplacementballe(Balle balle,float rectPositionY,float rectPositionZ,GLFWwindow* window,double* xposmousebuttoncallback,double*yposmousebuttoncallback)
{
	//mur haut bas gauche droite
	if (balle.ypos>=15./2-balle.rayon || balle.ypos<=-15./2+balle.rayon)
	{
		balle.vity=-balle.vity;
	}
	if ( balle.zpos<=-5+balle.rayon)
	{
		balle.vitz=sqrt(balle.vitz*balle.vitz);
		}
	if (balle.zpos>=5-balle.rayon)
	{
		balle.vitz=-sqrt(balle.vitz*balle.vitz);
	}
	if (balle.xpos<=-19.5)// diamètre de la balle est de 1 ? 
	{
		if(balle.zpos<(-rectPositionY)+4 && balle.zpos>(-rectPositionY)-4 && balle.ypos<(-rectPositionZ)+4 && balle.ypos>(-rectPositionZ)-4)// test si la balle touche la raquette 
		{
			balle.vitx=-balle.vitx;
		}
		else 
		{	
			balle.vitx=-balle.vitx;
			// balle.attrapee=true;
			// balle=attraperballe(window,xposmousebuttoncallback,yposmousebuttoncallback,balle,-rectPositionZ,-rectPositionY);
			
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
		float rayon=balle.rayon;
		if (posy-rayon<centrey+largeur/2 && posy+rayon>centrey-largeur/2 && posy+rayon>centrez-hauteur/2 && posy-rayon<centrez+hauteur/2)// ca veut dire que la balle est devant ou derrière le mur ; S
			if((balle.xpos+rayon>=obstacle.xpos && balle.xpos<=obstacle.xpos) || (balle.xpos-rayon<=obstacle.xpos && balle.xpos>=obstacle.xpos)) // la balle touche ou pénètre le mur de par un côté où de l'autre
			{
				balle.vitx=-balle.vitx;
			}
	}
	
	if (balle.xpos>=19)balle.vitx=-balle.vitx;
	if (balle.attrapee==false)
	{
	balle.xpos+=balle.vitx;
	balle.ypos+=balle.vity;
	balle.zpos+=balle.vitz;
	}

	return balle;
}

void dessinerballe(Balle balle)
{
	glColor3f(100/255,10/255,10/255);
	glRotatef(90.,0,1,0);
	glScalef(balle.rayon,balle.rayon,balle.rayon);
	glTranslatef(balle.zpos,balle.ypos,balle.xpos);// parfois la logique elle même cesse de régenter; 
	drawCircle();
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
// tu me preferes flo ???? pas segolene !!!!
// il est jaloux que je bosse le projet pendant mes vacances
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

		glColor4f(10/255,10/255,10/255,rectOpacity);
		glPushMatrix();
			
			glRotatef(90.0,0.,1.,0.);
			glTranslatef(-4./3.,0,0);
			glScalef(4.,4,2);
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


Balle attraperballe (GLFWwindow * 	window,double*xposadresse,double*yposadresse,Balle balle, float rectPositionY,float rectPositionZ)
{
	balle.vitx=0;
	balle.vity=0;
	balle.vitz=0;
	balle.ypos=rectPositionY;
	balle.zpos=rectPositionZ;
	double balisex =*xposadresse;
	double balisey =*yposadresse;
	glfwGetCursorPos(window,xposadresse,yposadresse);
	if (balisex!=*xposadresse || balisey!=*yposadresse)
	{
		balle.vitx=-1;
		balle.attrapee=false;
	}
}

void deplacementobstacles(float xsectionmur,int nombredemur)
{
	Objet obstacle=objettab[nombredemur];

	obstacle.xpos=60-xsectionmur;
	objettab[nombredemur]=obstacle;

	obstacle=objettab[nombredemur+1];
	if (xsectionmur<=70.) obstacle.xpos=50-xsectionmur;
	else obstacle.xpos=130-xsectionmur;
	objettab[nombredemur+1]=obstacle;
		
	obstacle=objettab[nombredemur+2];
	if (xsectionmur<=60) obstacle.xpos=40-xsectionmur;
	else obstacle.xpos=120-xsectionmur;
	objettab[nombredemur+2]=obstacle;
	
	obstacle=objettab[nombredemur+3];
	if (xsectionmur<=50) obstacle.xpos=30-xsectionmur;
	else obstacle.xpos=110-xsectionmur;
	objettab[nombredemur+3]=obstacle;

	obstacle=objettab[nombredemur+4];
	if (xsectionmur<=40) obstacle.xpos=20-xsectionmur;
	else obstacle.xpos=100-xsectionmur;
	objettab[nombredemur+4]=obstacle;
	
}

Balle avancer(float *xsectionmur,bool *clic,float *avancement_depuis_dernier_clic,Balle balle)
{
	//quand le joueur clic gauche, le jeu avance de 5 unité mais en combien de temps? lors du clic on passe un bool à vrai, on créer une variable avancement qui lorsqu'elle atteint 5 est remise à 0 et passe le bool à faux alors xsectionmur arrête d'avancer, si on clic pendant l'avancement cela ne fait rien  
	if (*clic){
		*avancement_depuis_dernier_clic=0;
	}
	if(*avancement_depuis_dernier_clic<10)
	{
		*avancement_depuis_dernier_clic+=0.4f;
		*xsectionmur+=0.4f;
		if (balle.xpos>=-19+balle.rayon)balle.xpos-=0.4f;
	}
	*clic=false;
	return balle;
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

	float teta = 0;

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
		objettab[i]=mur;

		// rect Bas
		mur.r=100.0/255.;
		mur.b=100.0/255.;
		mur.v=255.0/255.;
		mur.zpos=-5.;
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
		objettab[i+2]=mur;

		// rect Gauche
		mur.ypos=-15./2.;
		objettab[i+3]=mur;

		xpos-=10.;
	}

	//créer obstacles 

	Objet obstacle;
	//moitié gauche
	obstacle.r=250./255;
	obstacle.v=110./255;
	obstacle.b=110./255;
	obstacle.lumiere=0.;
	obstacle.sizex=10;
	obstacle.sizey=15./2;
	obstacle.sizez=1;
	obstacle.xpos=0;
	obstacle.ypos=-15./4;
	obstacle.zpos=0;
	obstacle.anglerotate=90.0;
	obstacle.rotatex=0;
	obstacle.rotatey=1.0;
	obstacle.rotatez=0;
	objettab[nombredemur]=obstacle;

	//moitié droite
	obstacle.ypos=-15./4;
	objettab[nombredemur+1]=obstacle;

	//moitié basse
	obstacle.ypos=0;
	obstacle.zpos=-5./2;
	obstacle.sizex=5;
	obstacle.sizey=15;
	objettab[nombredemur+2]=obstacle;

	//moitié haute 
	obstacle.zpos=5./2;
	objettab[nombredemur+3]=obstacle;
	
	//moitié gauche décalée au centre
	obstacle.zpos=0;
	obstacle.sizex=10;
	obstacle.sizey=15./2;
	objettab[nombredemur+4]=obstacle;


	
	
	// Activate transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
		
			


		/* Scene rendering */

		//déplacement et affichage
		light(objettab,nombredemur,nombredobstacle,balle);

		deplacementobstacles(xsectionmur,nombredemur);

		for (int i=0;i<nombredemur+nombredobstacle;i++)
			{

				mur=objettab[i];//plus un mur mais mur ou obstacle
				glPushMatrix();
					glTranslatef(mur.xpos,mur.ypos,mur.zpos);
					glRotatef(mur.anglerotate,mur.rotatex,mur.rotatey,mur.rotatez);
					glScalef(mur.sizex,mur.sizey,mur.sizez);
					drawMur(mur);
				glPopMatrix();
			}

		//sectionmur 
		bouton = glfwGetMouseButton	(window,GLFW_MOUSE_BUTTON_LEFT); //
		if (bouton==GLFW_PRESS)
		{
			clic=true;
		}
		bouton=-1;
		balle = avancer(&xsectionmur,&clic,&avancement_depuis_dernier_clic,balle);
		
		if (xsectionmur>=80.f)xsectionmur-=80.f;
		glPushMatrix();
			glTranslatef(60-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur<=70.) glTranslatef(50-xsectionmur,0,0);
			else glTranslatef(130-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur<=60.) glTranslatef(40-xsectionmur,0,0);
			else glTranslatef(120-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur<=50.) glTranslatef(30-xsectionmur,0,0);
			else glTranslatef(110-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur<=40.) glTranslatef(20-xsectionmur,0,0);
			else glTranslatef(100-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur<=30.) glTranslatef(10-xsectionmur,0,0);
			else glTranslatef(90-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur<=20) glTranslatef(-xsectionmur,0,0);
			else glTranslatef(80-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur<=10) glTranslatef(-10-xsectionmur,0,0);
			else glTranslatef(70-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();

		//balle 
		glPushMatrix();
			dessinerballe(balle);
		glPopMatrix();

		balle=deplacementballe(balle,rectPositionY,rectPositionZ,window,&xposmousebuttoncallback,&yposmousebuttoncallback);


		//raquette 
		glPushMatrix();
			glTranslatef(-20,-rectPositionZ,-rectPositionY);
			// glTranslatef(0, xpos,ypos);
			dessinerRaquette();
		glPopMatrix();

		

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
		teta+=0.03;
		//teta+=0.1;
		double tempspasse = glfwGetTime() - startTime;

		while(tempspasse < FRAMERATE_IN_SECONDS){
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
			tempspasse = glfwGetTime() - startTime;
		}
	}

	//draw
	glDisable(GL_BLEND);
	

	glfwTerminate();
	return 0;
}
