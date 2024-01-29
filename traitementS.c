#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#define MAX_LINE_SIZE 1024

typedef struct etape{
	int step_ID;
	struct etape* suivant;
}Etape;

typedef struct Trajet {
	int IdTrajet;
	float dist_min;
        float dist_max;
        float somme_dist;
        float ecart;
        Etape *etape;
        int nbstep;
        float dist_moy;
} Trajet;

typedef struct Arbre {
    Trajet *elmt;
    struct Arbre *fg;
    struct Arbre *fd;
    int equilibre;
} Arbre;

Trajet premier[51];


typedef Arbre* pArbre;

// creation d'un nouveau noeud dans l'arbre 
pArbre creationArbre(int IdrouteCourante,int stepIdcourant,  float distance) {
    pArbre nouveau = malloc(sizeof(Arbre));
    if (nouveau == NULL) {
        exit(1);
    }

    //affectation du numéro du trajet au noeud 
    nouveau->elmt = malloc(sizeof(Trajet));
    if (nouveau->elmt == NULL) {
        exit(1);
    }
    nouveau->elmt->IdTrajet = IdrouteCourante;
    
    //Initialisation des statistiques du nouveau noeud
    nouveau->elmt->dist_min = distance;
    nouveau->elmt->dist_max = distance;
    nouveau->elmt->ecart = 0;
    nouveau->elmt->somme_dist = distance;
    
    //Initialisation du nombre d'étape du trajet à 1
    nouveau->elmt->nbstep= 1;
    
    //Initialisation de la première étape du trajet
    Etape *newstep = malloc(sizeof(Etape));
    nouveau->elmt->etape = newstep;
    newstep->step_ID = stepIdcourant;
    newstep->suivant = NULL;
    
    nouveau->fg = NULL;
    nouveau->fd = NULL;
    nouveau->equilibre = 0;

    return nouveau;
}



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



pArbre insertAVL(pArbre a,int RouteIdCourante,int stepIdcourant, float distance, int *h) {
    if (a == NULL) {
    //si l'arbre est vide création d'un nouveau noeud 
        *h = 1;
        return creationArbre(RouteIdCourante, stepIdcourant,distance);
    }
    else if ( RouteIdCourante < a->elmt->IdTrajet) {
      //si le numéro du trajet de la ligne courante est inféririeur au numéro du trajet de l'arbre 
      //ajout d'un nouveau noeud a gauche de l'arbre 
        a->fg = insertAVL(a->fg,  RouteIdCourante,stepIdcourant, distance,h);
        *h = -*h;
    }
    else if (RouteIdCourante > a->elmt->IdTrajet) {
      //si le numéro du trajet de la ligne courante est supérieur au numéro du trajet de l'arbre 
      //ajout d'un nouveau noeud a gauche de l'arbre 
              a->fd = insertAVL(a->fd, RouteIdCourante,stepIdcourant, distance,h);
    }
    else if (RouteIdCourante == a->elmt->IdTrajet) {

     // si le numéro du trajet de la ligne courante existe deja dans l'arbre 
     // recherche de l'étape de la ligne dans la liste des étapes deja existantes pour ce trajet 
      
        Etape *etape, *etapeprec;
        etape = a->elmt->etape;
        etapeprec = a->elmt->etape;
        while (etape!=NULL && etape->step_ID != stepIdcourant )
        {
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
           if(a->elmt->dist_max<distance)
           	a->elmt->dist_max=distance;
           if(a->elmt->dist_min> distance)
           	a->elmt->dist_min= distance;
           a->elmt->ecart=a->elmt->dist_max - a->elmt->dist_min;
           a->elmt->somme_dist=a->elmt->somme_dist+distance;
           
           //mise à jour du nombre d'étape du trajet
           ++a->elmt->nbstep;
  
           
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

void AfficherResultat()
{
printf("Affichage des resultats\n");
        for (int i=0; i<50;i++){
        
        printf("[%d,%.3f,%.3f,%.3f,%.3f]->\n ", premier[i].IdTrajet,premier[i].dist_min,premier[i].dist_moy,premier[i].dist_max,premier[i].ecart );
       
        }
        printf("\n");

}

//recherche des 50 premiers trajets dont la distance max - la distance min sont les plus grandes 
void infixe(pArbre a) {

    if (a != NULL) {

        //recherche dans la partie gauche de l'arbre
        infixe(a->fd);
        int i = 0;
         
        
        //pour le noeud courant on recherche si le trajet doit etre ajouté aux 50 premiers trajets en fonction de son nombre de sa distance_max -distance_min
        while (i<=50  &&  premier[i].IdTrajet != 0 ){
           i++;          
        }
       if ( premier[i].IdTrajet == 0)
         {

              premier[i].IdTrajet =a->elmt->IdTrajet;

              premier[i].dist_max =a->elmt->dist_max;

              premier[i].dist_min =a->elmt->dist_min;

              premier[i].ecart =a->elmt->ecart;

              premier[i].dist_moy =a->elmt->somme_dist / a->elmt->nbstep;

         }
         else {
               i = 0;
               while (i<=50  && premier[i].ecart > a->elmt->ecart  ){
                   i++;

           
                 }


       
         	if (a->elmt->ecart > premier[i].ecart)
        	 {
        	    //ajout du trajet au tableau des 50 premiers trajets 


              	 for (int j=50;j>i;j--)
              	 {

                   premier[j].IdTrajet =premier[j-1].IdTrajet; 
                   premier[j].dist_max =premier[j-1].dist_max;
                   premier[j].dist_min =premier[j-1].dist_min;
                   premier[j].ecart =premier[j-1].ecart;
                   premier[j].dist_moy =premier[j-1].dist_moy;
   
              	 }

              premier[i].IdTrajet =a->elmt->IdTrajet;
              premier[i].dist_max =a->elmt->dist_max;
              premier[i].dist_min =a->elmt->dist_min;
              premier[i].ecart =a->elmt->ecart;
              premier[i].dist_moy =a->elmt->somme_dist / a->elmt->nbstep;

              }
         }
         

        //recherche dans la partie gauche de l'arbre
          infixe(a->fg);

    }

}



void freeArbre(pArbre a) {
Etape *ptrE, *ptretape;
    if (a != NULL) {
        freeArbre(a->fg);
        freeArbre(a->fd);
        ptretape=a->elmt->etape;
        while (ptretape!=NULL){
             ptrE=ptretape;
             ptretape = ptretape->suivant;
             free(ptrE);
        }
        free(a->elmt);
        free(a);
    }
}

int main() {
    
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
         a = insertAVL(a, routeIdCourante, stepIdCourant, distanceCourante, &h);
      	
      	
        free(NomVilleCouranteDebut);
        free(NomVilleCouranteFin);
        
    }


    
    // recherche des 50 premiers trajets dont la distance_max - la distance_min est la plus grande
    infixe(a);
    
    fin = clock();
    temps = ((double)fin - debut) / CLOCKS_PER_SEC;
    // affichage des 50 premiers trajets dont la distance_max - la distance_min est la plus grande
    AfficherResultat();
    freeArbre(a);
   // fermeture du fichier cvs
   fclose(fichier);


    printf("Temps d'execution : %f secondes\n", temps);

    return 0;
}


    
