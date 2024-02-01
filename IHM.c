#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include"Def.h"



void AfficherResultat_T()
{
printf("Affichage des resultats\n");
        for (int i=1; i<11;i++){
           if(premiere[i]!=NULL){
        
               printf("[%s,%d,%d]-> ", premiere[i]->nomVille, premiere[i]->nbrTrajet, premiere[i]->nbrTrajetDebut);
            }
        
        }
        printf("\n");

}


void AfficherResultat_S()
{
printf("Affichage des resultats\n");
        for (int i=0; i<50;i++){
        
        printf("[%d,%.3f,%.3f,%.3f,%.3f]->\n ", premier[i].IdTrajet,premier[i].dist_min,premier[i].dist_moy,premier[i].dist_max,premier[i].ecart );
       
        }
        printf("\n");

}

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

