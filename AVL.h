#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

pArbre creationArbre_T(char* nomVilleCourante, int IdrouteCourante,int stepIdcourant, bool premiereEtape);

pArbre creationArbre_S(int IdrouteCourante,int stepIdcourant,  float distance);pArbre rotationGauche(pArbre a);

pArbre rotationGauche(pArbre a);

pArbre rotationDroite(pArbre a);

pArbre doubleRotationGauche(pArbre a);

pArbre doubleRotationDroite(pArbre a);

pArbre equilibreAVL(pArbre a);

pArbre insertVille(pArbre a, char* nomVilleCourante, int RouteIdCourante,int stepIdcourant ,bool premiereEtape, int *h);

pArbre insertRoute(pArbre a,int RouteIdCourante,int stepIdcourant, float distance, int *h);
