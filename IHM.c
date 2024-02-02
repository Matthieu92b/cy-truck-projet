#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include"Def.h"



// fonction qui va afficher les 10 premieres villes ou on passe le plus souvent 
void AfficherResultat_T() {
   	printf("Affichage des resultats\n");
    	for (int i = 0; i < MAX_10PREMIERE_VILLES+1; i++) {
        	if (premiere[i] != NULL) {
            		printf("[%s,%d,%d]-> ", premiere[i]->nomVille, premiere[i]->nbrTrajet, premiere[i]->nbrTrajetDebut);
        	}
    	}
    	printf("\n");
}

// fonction qui va afficher les distances_max min et moyenne de villes
void AfficherResultat_S(){
	printf("Affichage des resultats\n");
        for (int i=0; i<50;i++){
        	printf("[%d,%.3f,%.3f,%.3f,%.3f]->\n ", premier[i].IdTrajet,premier[i].dist_min,premier[i].dist_moy,premier[i].dist_max,premier[i].ecart );
       	}
        printf("\n");
}
//liberation memoire arbre pour traitement t
void freeArbre_T(pArbre a) {
	Route *ptrroute,*ptrR;
	Etape *ptretape,*ptrE;
    	if (a != NULL) {
        	freeArbre_T(a->fg);
        	freeArbre_T(a->fd);
        	free(a->elmtVille->nomVille);
        	ptrroute=a->elmtVille->route;
        	while (ptrroute!=NULL){
        		ptretape=ptrroute->etape;
          		while (ptretape!=NULL){
             			ptrE=ptretape;
             			ptretape = ptretape->suivant;
             			free(ptrE);
          		}
          		ptrR = ptrroute;
          		ptrroute = ptrroute->suivant;
          		free(ptrR);
          	}
        	free(a->elmtVille);
        	free(a);
    	}
}
//liberation memoire arbre pour traitement s
void freeArbre_S(pArbre a) {
	Etape *ptrE, *ptretape;
    	if (a != NULL) {
        	freeArbre_S(a->fg);
        	freeArbre_S(a->fd);
        	ptretape=a->elmtTrajet->etape;
        	while (ptretape!=NULL){
             		ptrE=ptretape;
             		ptretape = ptretape->suivant;
             		free(ptrE);
        	}
        	free(a->elmtTrajet);
        	free(a);
    	}
}
