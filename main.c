#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include"Def.h"

Ville  *premiere[MAX_10PREMIERE_VILLES+1];
Trajet premier[MAX_50PREMIER_TRAJETS+1];

int main_T() {

    

    char line[MAX_LINE_SIZE];
    char *NomVilleCouranteDebut,*NomVilleCouranteFin;
    char *token;
   
    int h = 0;
    int stepIdCourant = 0, routeIdCourante = 0;
    pArbre a = NULL;
    clock_t debut, fin;
    double temps;

    debut = clock();
    
    // Ouverture du fichier cvs en mode lecture 
    FILE *fichier = fopen("data.csv", "r");
    if (fichier == NULL) {
        exit(1);
    }
   
    //lecture de la première ligne du fichier cvs
    fgets(line, sizeof(line), fichier);
    
	//boucle pour lire la totalité des lignes du fichiers cvs 
    while (fgets(line, sizeof(line), fichier) ) {
        //décodage des différentes valeurs contenues dans la lignes 
        token = strtok(line, ";");
        routeIdCourante = atoi (token);
        token = strtok(NULL, ";");
        stepIdCourant = atoi(token);
        token = strtok(NULL, ";");
        
        if (stepIdCourant == 1){
           NomVilleCouranteDebut = malloc(strlen(token) + 1);
           if (NomVilleCouranteDebut == NULL){
              exit(1);
           }
           strcpy(NomVilleCouranteDebut, token);
        }
        

        token = strtok(NULL, ";");
        NomVilleCouranteFin = malloc(strlen(token) + 1);
        if (NomVilleCouranteFin == NULL){
           exit(1);
        }
        strcpy(NomVilleCouranteFin, token);
        
   
        //insertion de la ville de fin de l'étape dans l'arbre
      	a = insertVille(a, NomVilleCouranteFin, routeIdCourante,stepIdCourant, false,&h);
      	

      	/*printf("retour au main\n");
      	if (a!=NULL) {
      	printf("equilibre0\n");
      	      printf("equilibre %d\n",a->equilibre);
      	/*if (a->elmt != NULL){
      	     	printf("essai2\n");
      	Ville *ptrv = (struct Ville*)a->elmt;
      	printf("a->elmt->nomVille %s\n",ptrv->nomVille);
      	}
      	}*/
     
      	
       if(stepIdCourant==1){
    
               
        //insertion de la ville de debut de l'étape dans l'arbre dans les cas ou l'étape correspond à la première tape du trajet 
            a = insertVille(a, NomVilleCouranteDebut, routeIdCourante,stepIdCourant,true, &h);
              


        }

        
    }
    

    
     // Initialisation des pointeurs sur les 10 premières villes
     int i;
     for (i=1;i<=10;i++){
            premiere[i] = NULL;
     }

    // recherche des 10 premières villes traversées par le plus de trajets 
    rechercheVille(a);
    
    fin = clock();
    temps = ((double)fin - debut) / CLOCKS_PER_SEC;
    
    // Affichage des 10 premières villes traversées par le plus de trajets 
    AfficherResultat_T();
    
    freeArbre_T(a);
    
   // fermeture du fichier cvs
    fclose(fichier);

    printf("Temps d'execution : %f secondes\n", temps);

    return 0;
}



int main_S() {
    

    
    
    char line[MAX_LINE_SIZE];
    char *NomVilleCouranteDebut,*NomVilleCouranteFin;
    char *token;

    int h = 0;
    int stepIdCourant = 0, routeIdCourante = 0; 
    float distanceCourante = 0;
    pArbre a = NULL;
    clock_t debut, fin;
    double temps;

    debut = clock();
    // Ouverture du fichier cvs en mode lecture 
    FILE *fichier = fopen("data.csv", "r");
    if (fichier == NULL) {
        exit(1);
    }
   

    //lecture de la première ligne du fichier cvs
        fgets(line, sizeof(line), fichier);

    
    // Initialisation du tableau des 50 p^remiers trajets
    for (int i=0;i<=50;i++){
      premier[i].IdTrajet = 0;
      premier[i].ecart = -1;
    }

    //boucle pour lire la totalité des lignes du fichiers cvs 
    while (fgets(line, sizeof(line), fichier) ) {

         //décodage des différentes valeurs contenues dans la lignes 
        token = strtok(line, ";");
        routeIdCourante = atof (token);
        token = strtok(NULL, ";");
        stepIdCourant = atof(token);
        token = strtok(NULL, ";");
        NomVilleCouranteDebut = malloc(strlen(token) + 1);
        strcpy(NomVilleCouranteDebut, token);
        token = strtok(NULL, ";");
        NomVilleCouranteFin = malloc(strlen(token) + 1);
        strcpy(NomVilleCouranteFin, token);
        token = strtok(NULL, ";");
        distanceCourante = atof (token);
 

         //insertion du trajet dans l'arbre
         a = insertRoute(a, routeIdCourante, stepIdCourant, distanceCourante, &h);
      	
      	
        free(NomVilleCouranteDebut);
        free(NomVilleCouranteFin);
        
    }


    
    // recherche des 50 premiers trajets dont la distance_max - la distance_min est la plus grande
    rechercheRoute(a);
    
    fin = clock();
    temps = ((double)fin - debut) / CLOCKS_PER_SEC;
    // affichage des 50 premiers trajets dont la distance_max - la distance_min est la plus grande
    AfficherResultat_S();
    freeArbre_S(a);
   // fermeture du fichier cvs
   fclose(fichier);


    printf("Temps d'execution : %f secondes\n", temps);

    return 0;
}

int main(int argc, char **argv) {
//printf("debut %s\n",argv[1]);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <option>\n", argv[0]);
        return 1;
        }
        else {
    
        if (strcmp(argv[1], "-s") == 0) {
        
         main_S();
         }
         else if (strcmp(argv[1], "-t") == 0) {
         
          
           main_T();
         }
         else {
         fprintf(stderr, "Usage: %s <option>\n", argv[0]);
         }
         }
     
     }
