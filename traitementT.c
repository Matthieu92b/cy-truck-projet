#include"stdio.h"
#include"stdlib.h"
#include"string.h"

#define MAX_LINE_SIZE 1024
#define MAX_FIELD_SIZE 6



typedef struct Ville{
	char nomVille[1000];
	int nbrTrajet;
}Ville;

typedef struct Arbre{
	Ville *elmt;
	struct Arbre *fg;
	struct Arbre *fd;
	int equilibre;
}Arbre;

typedef Arbre* pArbre;


pArbre creationArbre(char* nomVilleCourante){
  pArbre nouveau=malloc(sizeof(Arbre));
  if(nouveau==NULL){
    exit(1);
  }

  nouveau->elmt = malloc(sizeof(Ville));
  strcpy(nouveau->elmt->nomVille,nomVilleCourante);
  nouveau->elmt->nbrTrajet=0;
  nouveau->fg=NULL;
  nouveau->fd=NULL;
  nouveau->equilibre=0;
  printf ("Création noeud %s %d\n\n\n",nouveau->elmt->nomVille,nouveau->elmt->nbrTrajet);
  return nouveau;
}

pArbre insertABR(pArbre a, char* nomVilleCourante){
  if(a==NULL){
  printf("a=NULL\n");
    return creationArbre(nomVilleCourante);
    }
  else if(strcmp(nomVilleCourante,a->elmt->nomVille)<0){
    printf("gauche %s  %s\n",nomVilleCourante,a->elmt->nomVille);
    a->fg=insertABR(a->fg,nomVilleCourante);
      
  }
  else if(strcmp(nomVilleCourante,a->elmt->nomVille)>0){
        printf("droite %s  %s\n",nomVilleCourante,a->elmt->nomVille);
    a->fd=insertABR(a->fd,nomVilleCourante);
    }
  else if(strcmp(nomVilleCourante,a->elmt->nomVille)==0){
        printf("identique %s  %s\n",nomVilleCourante,a->elmt->nomVille);
        a->elmt->nbrTrajet++;
  }
  return a;
}


int main(){
	char line[MAX_LINE_SIZE];
	char *NomVilleCourante;
	char *token;
	int toto;
	pArbre a=NULL;
	FILE *fichier=fopen("data.csv","r");
	if(fichier==NULL){
		exit(1);
	}
	
        toto=0;
	fgets(line, sizeof(line), fichier);


	while (fgets(line, sizeof(line), fichier) && toto < 10) {
        // Séparation des champs par le délimiteur ';'
        toto++;
        printf("ligne %s",line);

        token = strtok(line, ";");

        token = strtok(NULL, ";");

        token= strtok(NULL, ";");

        strcpy(NomVilleCourante, token);
        
           
            a=insertABR(a,NomVilleCourante); 
            

 
        }
        
        
        fclose(fichier);

    }

    
