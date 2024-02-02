#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include"Def.h"


// creation d'un nouveau noeud dans l'arbre 
pArbre creationArbre_T(char* nomVilleCourante, int IdrouteCourante,int stepIdcourant, bool premiereEtape) {
	pArbre nouveau = malloc(sizeof(Arbre));
    	if (nouveau == NULL) {
        	exit(1);
    	}
	nouveau->elmtVille = malloc(sizeof(Ville));
    	if (nouveau->elmtVille == NULL) {
        	exit(1);
    	}
    	//affectation du nom de la ville au noeud 
    	nouveau->elmtVille->nomVille = nomVilleCourante;
    	nouveau->elmtVille->nbrTrajet = 1;
    	if (premiereEtape==true)  
        	nouveau->elmtVille->nbrTrajetDebut = 1;
    	else nouveau->elmtVille->nbrTrajetDebut = 0;
    	nouveau->elmtVille->route=malloc(sizeof(Route));
     	if (nouveau->elmtVille->route == NULL) {
       		exit(1);
    	}
    	//affectation du nouveau trajet au noeud 
    	nouveau->elmtVille->route->IdTrajet=IdrouteCourante;
    	nouveau->elmtVille->route->etape=malloc(sizeof(Etape));
     	if (nouveau->elmtVille->route->etape == NULL) {
        	exit(1);
    	}
    	//affectation de l'étape au trajet précédent 
    	nouveau->elmtVille->route->etape->step_ID=stepIdcourant;
     	nouveau->elmtVille->route->etape->suivant=NULL;
    	nouveau->elmtVille->route->suivant=NULL;
     	nouveau->fg = NULL;
    	nouveau->fd = NULL;
    	nouveau->equilibre = 0;
 	return nouveau;
}


// creation d'un nouveau noeud dans l'arbre 
pArbre creationArbre_S(int IdrouteCourante,int stepIdcourant,  float distance) {
	pArbre nouveau = malloc(sizeof(Arbre));
    	if (nouveau == NULL) {
        	exit(1);
    	}
	//affectation du numéro du trajet au noeud 
    	nouveau->elmtTrajet = malloc(sizeof(Trajet));
    	if (nouveau->elmtTrajet == NULL) {
       		exit(1);
    	}
    	nouveau->elmtTrajet->IdTrajet = IdrouteCourante;
    	//Initialisation des statistiques du nouveau noeud
    	nouveau->elmtTrajet->dist_min = distance;
    	nouveau->elmtTrajet->dist_max = distance;
    	nouveau->elmtTrajet->ecart = 0;
    	nouveau->elmtTrajet->somme_dist = distance;
    	//Initialisation du nombre d'étape du trajet à 1
    	nouveau->elmtTrajet->nbstep= 1;
    	//Initialisation de la première étape du trajet
    	Etape *newstep = malloc(sizeof(Etape));
    	nouveau->elmtTrajet->etape = newstep;
    	newstep->step_ID = stepIdcourant;
    	newstep->suivant = NULL;
    	nouveau->fg = NULL;
    	nouveau->fd = NULL;
    	nouveau->equilibre = 0;
	return nouveau;
}


//rotation gauche lorsque l'equilibre est supérieur ou égal a 2
pArbre rotationGauche(pArbre a){
	pArbre pivot;
	int eq_a,eq_p;
	pivot=a->fd;
	a->fd=pivot->fg;
	pivot->fg=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre=eq_a - maxRotGauche(eq_p , 0) -1;
	pivot->equilibre=minRotGauche(eq_a-2,eq_a+eq_p-2,eq_p-1);
	a=pivot;
	return a;
}


// rotation droite lorsque l'equilibre est inférieur ou égale a -2
pArbre rotationDroite(pArbre a){
	pArbre pivot;
	int eq_a,eq_p;
	pivot=a->fg;
	a->fg=pivot->fd;
	pivot->fd=a;
	eq_a=a->equilibre;
	eq_p=pivot->equilibre;
	a->equilibre=eq_a - minRotDroite(eq_p , 0) +1;
	pivot->equilibre=maxRotDroite(eq_a+2,eq_a+eq_p+2,eq_p+1);
	a=pivot;
	return a;
}


pArbre doubleRotationGauche(pArbre a){
	a->fd=rotationDroite(a->fd);
	return rotationGauche(a);
}


pArbre doubleRotationDroite(pArbre a){
	a->fg=rotationGauche(a->fg);
	return rotationDroite(a);
}


// on vérifie si l'arbre esr équilibré, s'il ne l'est pas on appelle les fonctions de rotation nécéssaires
pArbre equilibreAVL(pArbre a){
	if(a->equilibre >= 2){
		if(a->fd->equilibre >=0){
			return rotationGauche(a);
		}
		else{
			return doubleRotationGauche(a);
		}
		
	}
	else if(a->equilibre <=-2){
		if(a->fg->equilibre <= 0){
			return rotationDroite(a);
		}
		else{
			return doubleRotationDroite(a);
		}
	}
	return a;
}


//insertion de la ligne courante du fichier cvs dans l'arbre 
pArbre insertVille(pArbre a, char* nomVilleCourante, int RouteIdCourante,int stepIdcourant ,bool premiereEtape, int *h) {
	if (a == NULL) {
        	*h = 1;
        	//si l'arbre est vide création d'un nouveau noeud 
        	return creationArbre_T(nomVilleCourante, RouteIdCourante,stepIdcourant, premiereEtape);
    	}
    	else if (strcmp(nomVilleCourante, a->elmtVille->nomVille) < 0) {
      		//si le nom de la ville de la ligne courante est inféririeur au nom de la ville de l'arbre 
      		//ajout d'un nouveau noeud a gauche de l'arbre 
        	a->fg = insertVille(a->fg, nomVilleCourante, RouteIdCourante,stepIdcourant, premiereEtape,h);
        	*h = -*h;
    	}
    	else if (strcmp(nomVilleCourante, a->elmtVille->nomVille) > 0) {
    		//si le nom de la ville de la ligne courante est supérieur au nom de la ville de l'arbre 
      		//ajout d'un nouveau noeud a droite de l'arbre 
        	a->fd = insertVille(a->fd, nomVilleCourante, RouteIdCourante,stepIdcourant, premiereEtape, h);
    	}
    	else if (strcmp(nomVilleCourante, a->elmtVille->nomVille) == 0) {
      		// si le nom de la ville  de la ligne courante existe deja dans l'arbre 
      		//recherche de la route de la ligne dans la liste des routes deja existantes pour cette ville 
         	Route *ptrcourantR,*ptrprecR;
         	ptrcourantR =a->elmtVille->route;
         	ptrprecR =a->elmtVille->route;
         	Etape *ptrcourantE,*ptrprecE;
         	while (ptrcourantR!=NULL && ptrcourantR->IdTrajet != RouteIdCourante ) {
       		ptrprecR = ptrcourantR;
             	ptrcourantR=ptrcourantR->suivant;
 	}
        if (ptrcourantR== NULL) {
    		//si la route de la ligne n'existe pas dans cette liste création d'une nouvelle route dans la liste 
            	a->elmtVille->nbrTrajet++;
            	Route *newroute;
            	newroute=malloc(sizeof(Route));
            	if (newroute == NULL) {
        		exit(1);
   	    	}
            	ptrprecR->suivant=newroute;
            	newroute->IdTrajet=RouteIdCourante;
            	newroute->suivant=NULL;
            	//creation de l'etape associé a la route 
            	Etape *newetape=malloc(sizeof(Etape));
            	if (newetape == NULL) {
       			exit(1);
    	    	}
            	newroute->etape=newetape;
            	newetape->step_ID=stepIdcourant;
            	newetape->suivant=NULL;
            	if(premiereEtape){
           		a->elmtVille->nbrTrajetDebut++;
           	}
         }
         else{
         	//si la route de la ligne existe dans cette liste 
         	//recherche de l'etape dans la liste des etapes de cette route 
         	ptrcourantE=ptrcourantR->etape;
         	ptrprecE=ptrcourantR->etape;
         	while (ptrcourantE!= NULL && ptrcourantE->step_ID != stepIdcourant ){
     			ptrprecE = ptrcourantE;
             		ptrcourantE=ptrcourantE->suivant;
             	}
             	if(ptrcourantE==NULL){
             	        //si l'etape n'existe pas création d'une nouvelle étape dans cette liste      
             		Etape *newetape=malloc(sizeof(Etape));
             		if (newetape == NULL) {
        			exit(1);
    			}
            		ptrprecE->suivant=newetape;
            		newetape->step_ID=stepIdcourant;
            		newetape->suivant=NULL;	
            		if(premiereEtape){
           			a->elmtVille->nbrTrajetDebut++;
           		}
         	}
         	else {
         	//la route et l'étape ont été trouvées dans la liste des routes de la ville 
         		if(premiereEtape){
           			a->elmtVille->nbrTrajetDebut++;
           		}
           	}
         }
       	*h = 0;
 
	}
    	if (*h != 0) {
        	a->equilibre = a->equilibre + *h;
            	a=equilibreAVL(a);
        	if (a->equilibre == 0) {
            		*h = 0;
        	}
        	else {
            		*h = 1;
        	}
     	}
    	return a;
}


pArbre insertRoute(pArbre a,int RouteIdCourante,int stepIdcourant, float distance, int *h) {
	if (a == NULL) {
    		//si l'arbre est vide création d'un nouveau noeud 
        	*h = 1;
        	return creationArbre_S(RouteIdCourante, stepIdcourant,distance);
    	}
    	else if ( RouteIdCourante < a->elmtTrajet->IdTrajet) {
      		//si le numéro du trajet de la ligne courante est inféririeur au numéro du trajet de l'arbre 
      		//ajout d'un nouveau noeud a gauche de l'arbre 
        	a->fg = insertRoute(a->fg,  RouteIdCourante,stepIdcourant, distance,h);
        	*h = -*h;
    	}
    	else if (RouteIdCourante > a->elmtTrajet->IdTrajet) {
      		//si le numéro du trajet de la ligne courante est supérieur au numéro du trajet de l'arbre 
      		//ajout d'un nouveau noeud a gauche de l'arbre 
        	a->fd = insertRoute(a->fd, RouteIdCourante,stepIdcourant, distance,h);
    	}
    	else if (RouteIdCourante == a->elmtTrajet->IdTrajet) {
		// si le numéro du trajet de la ligne courante existe deja dans l'arbre 
     		// recherche de l'étape de la ligne dans la liste des étapes deja existantes pour ce trajet 
      		Etape *etape, *etapeprec;
        	etape = a->elmtTrajet->etape;
        	etapeprec = a->elmtTrajet->etape;
        	while (etape!=NULL && etape->step_ID != stepIdcourant ){
             		etapeprec = etape;
             		etape = etape->suivant;
        	}
		if (etape==NULL) {
			//si l'étape de la ligne n'existe pas dans cette liste d'étapes création d'une nouvelle étape dans la liste                    
           		etape = malloc(sizeof(Etape));
           		if (etape == NULL) {
              			exit(1);
            		}
           		etapeprec->suivant = etape;
           		etape->step_ID= stepIdcourant;
           		// mise à jour des statistiques de la route
           		if(a->elmtTrajet->dist_max<distance)
           			a->elmtTrajet->dist_max=distance;
           			if(a->elmtTrajet->dist_min> distance)
           				a->elmtTrajet->dist_min= distance;
           				a->elmtTrajet->ecart=a->elmtTrajet->dist_max - a->elmtTrajet->dist_min;
           				a->elmtTrajet->somme_dist=a->elmtTrajet->somme_dist+distance;
           				//mise à jour du nombre d'étape du trajet
           				++a->elmtTrajet->nbstep;
  		}
  		*h = 0;
  	}
    	if (*h != 0) {
        	a->equilibre = a->equilibre + *h;
        	a=equilibreAVL(a);
        	if (a->equilibre == 0) {
            		*h = 0;
        	}
        	else {
            		*h = 1;
        	}
    	}
 	return a;
}
