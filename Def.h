



//tratitement t
#define MAX_LINE_SIZE 1024
#define MAX_10PREMIERE_VILLES 10
#define MAX_50PREMIER_TRAJETS 50


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
	Ville *elmtVille;
	Trajet *elmtTrajet;
	struct Arbre *fg;
	struct Arbre *fd;
	int equilibre;
} Arbre;



typedef Arbre* pArbre;
//on utilise extern sinon cela nous fait une erreur de commande multiple
extern Ville  *premiere[];
extern Trajet premier[];


int minRotGauche(int a, int b, int c);

int maxRotGauche(int a, int b);

int minRotDroite(int a, int b);

int maxRotDroite(int a, int b, int c);

void rechercheVille(pArbre a);

void rechercheRoute(pArbre a);

pArbre creationArbre_T(char* nomVilleCourante, int IdrouteCourante,int stepIdcourant, bool premiereEtape);

pArbre creationArbre_S(int IdrouteCourante,int stepIdcourant,  float distance);pArbre rotationGauche(pArbre a);

pArbre rotationGauche(pArbre a);

pArbre rotationDroite(pArbre a);

pArbre doubleRotationGauche(pArbre a);

pArbre doubleRotationDroite(pArbre a);

pArbre equilibreAVL(pArbre a);

pArbre insertVille(pArbre a, char* nomVilleCourante, int RouteIdCourante,int stepIdcourant ,bool premiereEtape, int *h);

pArbre insertRoute(pArbre a,int RouteIdCourante,int stepIdcourant, float distance, int *h);

void AfficherResultat_T();

void AfficherResultat_S();

void freeArbre_T(pArbre a) ;

void freeArbre_S(pArbre a) ;
