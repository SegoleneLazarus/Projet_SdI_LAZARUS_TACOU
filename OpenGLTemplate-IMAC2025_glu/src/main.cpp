#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "3D_tools.h"
#include "draw_scene.h"
/* Structure */
struct objet{
	char nom;
	float xpos;
	float ypos;
<<<<<<< HEAD
	int r;
	int v;
	int b;
	int sizex;
	int sizey;
	int sizez;
	int transformx;
	int transformy;
	int transformz;


=======
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
>>>>>>> 131770a8bb03abcbcb2b3ed559e31a653c95da4f
};



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
void light()
{
	glMatrixMode(GL_MODELVIEW);
	GLfloat pos[] = {5,5,5,1};
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glLoadIdentity();
   	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
}
void dessinerballe()
{
	glColor3f(100/255,10/255,10/255);
	glRotatef(90.,0,1,0);
	//glScalef(2,2,2);
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
void dessinerraquette(){
// rect Haut
		glPushMatrix();
			glColor3f(10/255,10/255,10/255);
			glRotatef(90.0,0.,1.,0.);
			glTranslatef(-4./3.,0,0);
			glScalef(1./20.,4,2);
			drawSquare();
		glPopMatrix();
// tu me preferes flo ???? pas segolene !!!!
// il est jaloux que je bosse le projet pendant mes vacances
		// rect Bas
		glPushMatrix();
			glColor3f(10/255,10/255,10/255);
			glRotatef(90.0,0.,1.,0.);
			glTranslatef(4./3.,0,0);
			glScalef(1./20.,4,2);
			drawSquare();
		glPopMatrix();		

		// rect Droit
		glPushMatrix();
			glColor3f(10/255,10/255,10/255);
			glTranslatef(0,2,0);
			glRotatef(90.0,1.,0.,0.);			
			glRotatef(90.0,0.,1.,0.);

			glScalef(1./20.,4.*2./3.,2);
			
			drawSquare();
		glPopMatrix();	

		// rect Gauche
		glPushMatrix();
			glColor3f(10/255,10/255,10/255);
			glTranslatef(0,-2,0);
			glRotatef(90.0,1.,0.,0.);
			glRotatef(90.0,0.,1.,0.);
			glScalef(1./20.,4.*2./3.,2);
			drawSquare();
		glPopMatrix();
}

//déplcament balle /////////////////////////////////////////////////////////////
float vitx=0.1;
float vity=0.;
float posxballe=-20.;
float posyballe=0.;
void moveball(float vitx,float vity,float *posxballe,float *posyballe)
{
	*posxballe+=vitx;
	*posyballe+=vity; 
}
double xpos, ypos;
// define prend le rôle d'une constante mais il va tout remplacer avant la compilation par sa valeur
#define NBTAB 1500
float tabX[NBTAB];
float tabY[NBTAB];
int nbPoint = 0;
int entered;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &xpos, &ypos);
}
int xsectionmur;

<<<<<<< HEAD
=======


>>>>>>> 131770a8bb03abcbcb2b3ed559e31a653c95da4f
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

	glfwSetCursorPosCallback(window, cursor_position_callback);

<<<<<<< HEAD
=======
	// créer les objets 

	//créer mur 
	objet mur;
	// mur.nom="mur";
	mur.sizex=10;
	mur.sizez=1;
	mur.rotatey=0;
	mur.rotatez=0;
	objet murtab[16];
	int xpos=15;
	for (int i=0; i>16;i+=4)
	{
		mur.sizey=15;
		mur.r=100.0/255.;
		mur.b=100.0/255.;
		mur.v=255.0/255.;
		mur.anglerotate=0;
		mur.rotatex=0;
		mur.xpos=xpos;
		mur.ypos=0;

		//rect haut
		mur.zpos=5;
		murtab[i]=mur;

		// rect Bas
		mur.zpos=-5;
		murtab[i+1]=mur;

		mur.sizey=10;
		mur.r=200.0;
		mur.b=200.0/255;
		mur.v=200.0/255;
		mur.zpos=0;
		mur.anglerotate=90.0;
		mur.rotatex=1.;
		// rect Droit
		mur.ypos=15./2.;
		murtab[i+2]=mur;

		// rect Gauche
		mur.ypos=-15./2.;
		murtab[i+3]=mur;

		xpos-=10;
	}
>>>>>>> 131770a8bb03abcbcb2b3ed559e31a653c95da4f
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.5,0.5,0.5,0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();

		/* Initial scenery setup */
		// light();


		/* Scene rendering */
		// rect Haut
		glPushMatrix();
			glTranslatef(20-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur%40<=30) glTranslatef(10-xsectionmur,0,0);
			else glTranslatef(50-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur%40<=20) glTranslatef(-xsectionmur,0,0);
			else glTranslatef(40-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur%40<=10) glTranslatef(-10-xsectionmur,0,0);
			else glTranslatef(30-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();

		//balle 
		glPushMatrix();
<<<<<<< HEAD
			glScalef(40,15,1);
			glTranslatef(0,0,-5);
			drawSquare();
		glPopMatrix();		

		// rect Droit
		glPushMatrix();
			glColor3f(200.0/255,200.0/255,200.0/255);
			glTranslatef(0,15./2.,0);
			glRotatef(90.0,1.,0.,0.);
			glScalef(40,10,1);
			drawSquare();
		glPopMatrix();

		// rect Gauche
		glPushMatrix();
			glColor3f(200.0/255,200.0/255,200.0/255);
			glTranslatef(0,-15./2.,0);
			glRotatef(90.0,1.,0.,0.);
			glScalef(40,10,1);
			drawSquare();
		glPopMatrix();

	//////////////////////////////////////////////////////////////////////////////sectionmur 
		xsectionmur=(xsectionmur+1)%40;
		glPushMatrix();
			glTranslatef(20-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur%40<=30) glTranslatef(10-xsectionmur,0,0);
			else glTranslatef(50-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur%40<=20) glTranslatef(-xsectionmur,0,0);
			else glTranslatef(40-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();
		glPushMatrix();
			if (xsectionmur%40<=10) glTranslatef(-10-xsectionmur,0,0);
			else glTranslatef(30-xsectionmur,0,0);
			dessinersectionmur();
		glPopMatrix();

		//balle 
		glPushMatrix();
			glTranslatef(0,-5,-2);
			dessinerballe();
		glPopMatrix();

=======
			glTranslatef(0,-5,-2);
			dessinerballe();
		glPopMatrix();

>>>>>>> 131770a8bb03abcbcb2b3ed559e31a653c95da4f
		//dessin raquette
		glPushMatrix();
			glTranslatef(-20,-((10.0/WINDOW_WIDTH)*xpos-5)*aspectRatio,(-10.0/WINDOW_HEIGHT)*ypos+5);
			// glTranslatef(0, xpos,ypos);
			dessinerraquette();
		glPopMatrix();



		
		

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
	}

	glfwTerminate();
	return 0;
}
