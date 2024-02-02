#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include"Def.h"


//recherche du minimum entre les 3 paramètres
int minRotGauche(int a, int b, int c) {
	int min_value = a;
	if (b < min_value) {
        	min_value = b;
    	}
    	if (c < min_value) {
        	min_value = c;
    	}
    	return min_value;
}


//recherche du maximum entre les 2 paramètres
int maxRotGauche(int a, int b) {
	int max_value = a;
    	if (b > max_value) {
        	max_value = b;
    	}
    	return max_value;
}


//recherche du minimum entre les 2 paramètres
int minRotDroite(int a, int b) {
	int min_value = a;
    	if (b < min_value) {
        	min_value = b;
    	}
    	return min_value;
}

//recherche du maximum entre les 3 paramètres
int maxRotDroite(int a, int b, int c) {
	int max_value = a;
    	if (b > max_value) {
        max_value = b;
    	}
    	if (c > max_value) {
        max_value = c;
    	}
    	return max_value;
}


//recherche des 10 premières villes traversées pas le plus grand nombre de trajets 
void rechercheVille(pArbre a){
	if (a != NULL) {
        	//recherche dans la partie gauche de l'arbre 
        	rechercheVille(a->fg);
        	//pour le noeud courant on recherche si la ville doit etre ajoutée aux 10 premières villes en fonction de son nombre de trajets
		int i = 1;
        	while (i<=10 && premiere[i] != NULL && a->elmtVille->nbrTrajet <= premiere[i]->nbrTrajet){
           		i++;
        	}
        	if (premiere[i]==NULL){
           		premiere[i] = a->elmtVille;
        	}
        	else if (a->elmtVille->nbrTrajet > premiere[i]->nbrTrajet){
        	//ajout de la ville au tableau des 10 premieres villes 
			for (int j=10;j>i;j--){
                		premiere[j] = premiere[j-1];
               		}
              		premiere[i] = a->elmtVille;

        	}
        	//recherche de la partie droite de l'arbre      
        	rechercheVille(a->fd);
        }

}


//recherche des 50 premiers trajets dont la distance max - la distance min sont les plus grandes 
void rechercheRoute(pArbre a) {
	if (a != NULL) {
		//recherche dans la partie gauche de l'arbre
        	rechercheRoute(a->fd);
        	int i = 0;
         	//pour le noeud courant on recherche si le trajet doit etre ajouté aux 50 premiers trajets en fonction de son nombre de sa distance_max -distance_min
        	while (i<=50  &&  premier[i].IdTrajet != 0 ){
           		i++;          
        	}
       		if ( premier[i].IdTrajet == 0){
			premier[i].IdTrajet =a->elmtTrajet->IdTrajet;
			premier[i].dist_max =a->elmtTrajet->dist_max;
			premier[i].dist_min =a->elmtTrajet->dist_min;
			premier[i].ecart =a->elmtTrajet->ecart;
			premier[i].dist_moy =a->elmtTrajet->somme_dist / a->elmtTrajet->nbstep;
		}
         	else {
               		i = 0;
               		while (i<=50  && premier[i].ecart > a->elmtTrajet->ecart  ){
                   		i++;
			}

			if (a->elmtTrajet->ecart > premier[i].ecart){
        	    		//ajout du trajet au tableau des 50 premiers trajets 
				for (int j=50;j>i;j--){
					premier[j].IdTrajet =premier[j-1].IdTrajet; 
                   			premier[j].dist_max =premier[j-1].dist_max;
                   			premier[j].dist_min =premier[j-1].dist_min;
                   			premier[j].ecart =premier[j-1].ecart;
                   			premier[j].dist_moy =premier[j-1].dist_moy;
   
              	 		}
				premier[i].IdTrajet =a->elmtTrajet->IdTrajet;
              			premier[i].dist_max =a->elmtTrajet->dist_max;
              			premier[i].dist_min =a->elmtTrajet->dist_min;
              			premier[i].ecart =a->elmtTrajet->ecart;
              			premier[i].dist_moy =a->elmtTrajet->somme_dist / a->elmtTrajet->nbstep;
			}
         	}
         	//recherche dans la partie gauche de l'arbre
          	rechercheRoute(a->fg);
	}

}
