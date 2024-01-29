#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<time.h>

#define MAX_LINE_SIZE 1024
#define MAX_10PREMIERE_VILLES 10
typedef struct etape{
	int step_ID;
	struct etape* suivant;
}Etape;


typedef struct route{
	int IdTrajet;
	Etape* etape;
	struct route* suivant;
}Route;


typedef struct Ville {
    char *nomVille;
    int nbrTrajet;
    int nbrTrajetDebut;
    Route *route;
    
} Ville;

typedef struct Arbre {
    Ville *elmt;
    struct Arbre *fg;
    struct Arbre *fd;
    int equilibre;
} Arbre;

Ville  *premiere[MAX_10PREMIERE_VILLES+1];


typedef Arbre* pArbre;
// creation d'un nouveau noeud dans l'arbre 
pArbre creationArbre(char* nomVilleCourante, int IdrouteCourante,int stepIdcourant, bool premiereEtape) {
    pArbre nouveau = malloc(sizeof(Arbre));
    if (nouveau == NULL) {
        exit(1);
    }

    nouveau->elmt = malloc(sizeof(Ville));
    if (nouveau->elmt == NULL) {
        exit(1);
    }
    //affectation du nom de la ville au noeud 
    //strcpy(nouveau->elmt->nomVille, nomVilleCourante);
    nouveau->elmt->nomVille = nomVilleCourante;
    //printf("%s\n",nouveau->elmt->nomVille);
    nouveau->elmt->nbrTrajet = 1;
    if (premiereEtape==true)  
          nouveau->elmt->nbrTrajetDebut = 1;
    else nouveau->elmt->nbrTrajetDebut = 0;
    
  
    nouveau->elmt->route=malloc(sizeof(Route));
     if (nouveau->elmt->route == NULL) {
        exit(1);
    }
    //affectation du nouveau trajet au noeud 
    nouveau->elmt->route->IdTrajet=IdrouteCourante;
    nouveau->elmt->route->etape=malloc(sizeof(Etape));
     if (nouveau->elmt->route->etape == NULL) {
        exit(1);
    }
    //affectation de l'étape au trajet précédent 
    nouveau->elmt->route->etape->step_ID=stepIdcourant;
     nouveau->elmt->route->etape->suivant=NULL;
    nouveau->elmt->route->suivant=NULL;
     
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


//insertion de la ligne courante du fichier cvs dans l'arbre 
pArbre insertAVL(pArbre a, char* nomVilleCourante, int RouteIdCourante,int stepIdcourant ,bool premiereEtape, int *h) {

    if (a == NULL) {
        *h = 1;
        //si l'arbre est vide création d'un nouveau noeud 
        return creationArbre(nomVilleCourante, RouteIdCourante,stepIdcourant, premiereEtape);
    }
    else if (strcmp(nomVilleCourante, a->elmt->nomVille) < 0) {
      //si le nom de la ville de la ligne courante est inféririeur au nom de la ville de l'arbre 
      //ajout d'un nouveau noeud a gauche de l'arbre 
        a->fg = insertAVL(a->fg, nomVilleCourante, RouteIdCourante,stepIdcourant, premiereEtape,h);
        *h = -*h;
    }
    else if (strcmp(nomVilleCourante, a->elmt->nomVille) > 0) {
    //si le nom de la ville de la ligne courante est supérieur au nom de la ville de l'arbre 
      //ajout d'un nouveau noeud a droite de l'arbre 
        a->fd = insertAVL(a->fd, nomVilleCourante, RouteIdCourante,stepIdcourant, premiereEtape, h);
    }
    else if (strcmp(nomVilleCourante, a->elmt->nomVille) == 0) {
      // si le nom de la ville  de la ligne courante existe deja dans l'arbre 
      //recherche de la route de la ligne dans la liste des routes deja existantes pour cette ville 
         Route *ptrcourantR,*ptrprecR;
         ptrcourantR =a->elmt->route;
         ptrprecR =a->elmt->route;
         Etape *ptrcourantE,*ptrprecE;
         while (ptrcourantR!=NULL && ptrcourantR->IdTrajet != RouteIdCourante ) {
       
             ptrprecR = ptrcourantR;
             ptrcourantR=ptrcourantR->suivant;
 
        
        }
        if (ptrcourantR== NULL) {
    	//si la route de la ligne n'existe pas dans cette liste création d'une nouvelle route dans la liste 
            a->elmt->nbrTrajet++;
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
           	a->elmt->nbrTrajetDebut++;
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
           			a->elmt->nbrTrajetDebut++;
           		}
         	}
         	else {
         	//la route et l'étape ont été trouvées dans la liste des routes de la ville 
         	
            		if(premiereEtape){
           		a->elmt->nbrTrajetDebut++;
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

void AfficherResultat()
{
printf("Affichage des resultats\n");
        for (int i=1; i<11;i++){
           if(premiere[i]!=NULL){
        
               printf("[%s,%d,%d]-> ", premiere[i]->nomVille, premiere[i]->nbrTrajet, premiere[i]->nbrTrajetDebut);
            }
        
        }
        printf("\n");

}

//recherche des 10 premières villes traversées pas le plus grand nombre de trajets 
void infixe(pArbre a) {


    if (a != NULL) {
        //recherche dans la partie gauche de l'arbre 
        infixe(a->fg);
        //pour le noeud courant on recherche si la ville doit etre ajoutée aux 10 premières villes en fonction de son nombre de trajets
	int i = 1;
        while (i<=10 && premiere[i] != NULL && a->elmt->nbrTrajet <= premiere[i]->nbrTrajet){
           i++;
        }
        if (premiere[i]==NULL){
           premiere[i] = a->elmt;
        }
        else if (a->elmt->nbrTrajet > premiere[i]->nbrTrajet)
         {
         //ajout de la ville au tableau des 10 premieres villes 

               for (int j=10;j>i;j--)
               {
                   premiere[j] = premiere[j-1];
               }
              premiere[i] = a->elmt;

         }
         


   //recherche de la partie droite de l'arbre      
        infixe(a->fd);

    }

}

void freeArbre(pArbre a) {
Route *ptrroute,*ptrR;
Etape *ptretape,*ptrE;
    if (a != NULL) {
        freeArbre(a->fg);
        freeArbre(a->fd);
        free(a->elmt->nomVille);
        ptrroute=a->elmt->route;
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
      	a = insertAVL(a, NomVilleCouranteFin, routeIdCourante,stepIdCourant, false,&h);
      	
     
      	
       if(stepIdCourant==1){
    
               
        //insertion de la ville de debut de l'étape dans l'arbre dans les cas ou l'étape correspond à la première tape du trajet 
            a = insertAVL(a, NomVilleCouranteDebut, routeIdCourante,stepIdCourant,true, &h);
              


        }

        
    }
    

    
     // Initialisation des pointeurs sur les 10 premières villes
     int i;
     for (i=1;i<=10;i++){
            premiere[i] = NULL;
     }

    // recherche des 10 premières villes traversées par le plus de trajets 
    infixe(a);
    
    fin = clock();
    temps = ((double)fin - debut) / CLOCKS_PER_SEC;
    
    // Affichage des 10 premières villes traversées par le plus de trajets 
    AfficherResultat();
    
    freeArbre(a);
    
   // fermeture du fichier cvs
    fclose(fichier);

    printf("Temps d'execution : %f secondes\n", temps);

    return 0;
}


    
	
	

