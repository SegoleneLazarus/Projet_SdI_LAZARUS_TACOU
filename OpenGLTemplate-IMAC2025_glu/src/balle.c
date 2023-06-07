#include "balle.h"



Balle attraperballe (Balle balle, float rectPositionY,float rectPositionZ);

Balle attraperballe (Balle balle, float rectPositionY,float rectPositionZ)
{
	balle.vitx=0;
	balle.vity=0;
	balle.vitz=0;
	balle.xpos=-17;
	balle.ypos=rectPositionY;
	balle.zpos=-rectPositionZ;
	return balle;
}

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

	if (balle.xpos<=-19+balle.rayon)
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
		{
			if((balle.xpos+rayon>=obstacle.xpos && balle.xpos<=obstacle.xpos) || (balle.xpos-rayon<=obstacle.xpos && balle.xpos>=obstacle.xpos)) // la balle touche ou pénètre le mur de par un côté où de l'autre
			{
				if (obstacle.bonus==0)
				{
					balle.vitx=-balle.vitx;
					balle.xpos+=balle.vitx;
				}
				
			}
		}

		//retiré car marche trop mal 

		// Vérification si la balle est au même niveau que l'obstacle selon l'axe x
		// if (balle.xpos-balle.rayon<objettab[i].xpos && balle.xpos+balle.rayon>objettab[i].xpos && obstacle.bonus==0)
		// {
		// 	// Vérification si la balle touche le bord vertical de l'obstacle
		// 	if ((balle.ypos + balle.rayon >= obstacle.ypos - obstacle.sizey / 2 && balle.ypos - balle.rayon <= obstacle.ypos - obstacle.sizey / 2 )||( balle.ypos - balle.rayon <= obstacle.ypos + obstacle.sizey / 2 && balle.ypos + balle.rayon >= obstacle.ypos + obstacle.sizey / 2))
		// 	{
		// 		balle.vity = -balle.vity; // Inversion de la vitesse verticale
		// 	}

		// 	// Vérification si la balle touche le bord horizontal de l'obstacle
		// 	if ((balle.zpos + balle.rayon >= obstacle.zpos - obstacle.sizez / 2 && balle.zpos - balle.rayon <= obstacle.zpos - obstacle.sizez / 2 )||( balle.zpos - balle.rayon <= obstacle.zpos + obstacle.sizez / 2 && balle.zpos + balle.rayon >= obstacle.zpos + obstacle.sizez / 2))
		// 	{
		// 		balle.vitz = -balle.vitz; // Inversion de la vitesse horizontale
		// 	}
		// }

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
					if (obstacle.zpos < (rectPositionY + 3.5) && obstacle.zpos > (rectPositionY - 3.5) && obstacle.ypos < (-rectPositionZ + 3.5) && obstacle.ypos > (-rectPositionZ - 3.5) && obstacle.xpos<=-19.5 && obstacle.xpos>=-20)
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
			if (balle.xpos>=-18+balle.rayon && not flag_obstacle)balle.xpos-=0.4f;
		}
	
		*clic=false;
	}
	return balle;
	
}