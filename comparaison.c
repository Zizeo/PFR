#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 


#define MAX_TOKEN 2000
#define MAX_TOKEN_LENGTH 50
#define MAX_PARAMETER_LENGTH 10
#define MAX_DESCRIPTEUR 100
#define TOKEN_DELIMITER ":"
#define SEUIL_SIMILARITE 50

typedef struct {
  int nb_occurence;
  char keyword[MAX_TOKEN_LENGTH]; // It should be less than MAX_TOKEN_LENGTH
}
Token;

typedef struct {
  int id;
  int nb_token;
  int nb_total_token;
  Token * tokens;
}
Descripteur;

typedef struct {
  int id;
  int nb_occurence;
}
SearchComparableItem;

Descripteur * parse_base_descripteur(FILE * fp, int * descripteur_nb) {
  Descripteur * descripteurs = malloc(sizeof(Descripteur) * MAX_DESCRIPTEUR);

  // Check if file exists
  if (fp == NULL) {
    printf("Erreur: base descripteur vide");
    exit(EXIT_FAILURE);
  }

  * descripteur_nb = 0;
  while (feof(fp) == 0 && * descripteur_nb < MAX_DESCRIPTEUR) {
    // Access & init a new Descripteur 
    Descripteur * descripteur = & descripteurs[ * descripteur_nb];
    char line[MAX_PARAMETER_LENGTH];

    // Read 3 first lines
    // Line 0: id
    fgets(line, MAX_PARAMETER_LENGTH, fp);
    sscanf(line, "%d", & descripteur -> id);

    // Line 1: nb_token
    fgets(line, MAX_PARAMETER_LENGTH, fp);
    sscanf(line, "%d", & descripteur -> nb_token);

    // Line 2: nb_total_token
    fgets(line, MAX_PARAMETER_LENGTH, fp);
    sscanf(line, "%d", & descripteur -> nb_total_token);

    if (descripteur -> nb_token > MAX_TOKEN) {
      printf("Erreur: le nombre de token est supérieur à la limite MAX_TOKEN %d", descripteur -> nb_token);
      exit(EXIT_FAILURE);
    }

    descripteur -> tokens = (Token * ) malloc(sizeof(Token) * descripteur -> nb_token);

    // For nb_token, parse Token
    for (int t = 0; t < descripteur -> nb_token; t++) {
      char token_line[MAX_TOKEN_LENGTH];
      Token * token = & (descripteur -> tokens[t]);

      fgets(token_line, MAX_TOKEN_LENGTH, fp);
      char * tokenized_line = strtok(token_line, TOKEN_DELIMITER);

      sscanf(tokenized_line, "%s", & token -> keyword);
      sscanf(strtok(NULL, tokenized_line), "%d", & token -> nb_occurence);

    }
    ++ * descripteur_nb;
  }

  fclose(fp);
  return descripteurs;
}

void search_by_keyword(char * mot_cle, Descripteur * descripteurs, int descripteurs_length) {
  
  SearchComparableItem search_comparables[MAX_DESCRIPTEUR];

  int nb_apparition = 0;
  for (int j = 0; j < descripteurs_length; j++) {
    for (int k = 0; k < descripteurs[j].nb_token; k++) {
      if (strcmp(mot_cle, descripteurs[j].tokens[k].keyword) == 0) {
        nb_apparition += 1;
        search_comparables[j].id = descripteurs[j].id;
        search_comparables[j].nb_occurence = descripteurs[j].tokens[k].nb_occurence;
        break;
      }
    }

  }

  for (int i = 0; i < nb_apparition - 1; i++) {
    for (int j = 0; j < nb_apparition - i - 1; j++) {
      /* Pour un ordre décroissant utiliser < */
      //int similarite_max = ((search_comparables[j].nb_occurence * 100)/temp);
      //printf("%d\n", similarite_max);
      if (search_comparables[j].nb_occurence < search_comparables[j + 1].nb_occurence) {

        SearchComparableItem tmp = search_comparables[j];

        search_comparables[j] = search_comparables[j + 1];
        search_comparables[j + 1] = tmp;

      }

    }
  }
  int max_occ = nb_apparition > 0 ? search_comparables[0].nb_occurence : 0;
  printf("Occ max %d\n", max_occ);

  for (int i = 0; i < nb_apparition; i++) {
    printf("ID: %d, Occurences: %d\n", search_comparables[i].id, search_comparables[i].nb_occurence);
    // TODO : attention div par 0; int similarite_max = ((search_comparables[i].nb_occurence * 100) / max_occ);
  }

  /*
  int temp= search_comparables[0].nb_occurence;
  printf("%d\n",temp);
  for(int i= 0; i<nb_apparition; i++){
      printf("%d\n",search_comparables[0].nb_occurence);
      int similarite_max = ((search_comparables[i].nb_occurence * 100)/temp); //16
      printf("%d\n", similarite_max);
     // if (similarite_max >SEUIL_SIMILARITE){}


         printf("ID: %d, Occurences: %d\n", search_comparables[i].id, search_comparables[i].nb_occurence );

     
  }*/

}

//void search_by_similarity(int descripteur_id, Descripteur *descripteurs, int descripteurs_length) {
// Find the descriptor from the ID
// For each tokens of a given descripteur_id
// Compare to all tokens in all les decripteurs excepted the concerned one
// tab avec id et de taux de similarité, make them both des ints to avoid UNE STRUCTURE 
// on recupère un dossier, check si le fichier existe dans la base des descripteurs 
// si oui je recupère le descripteur sinon j'indexe 
//}

// compare deux descripteurs dans bdd 
//compare deux descipteurs pas dans bdd 

/*double compare_descriptors(Descriptor *descriptor1, Descriptor *descriptor2)
{
    int total_common_terms = 0;
    for (int i = 0; i < descriptor1->total_terms; i++)
    {
        for (int j = 0; j < descriptor2->total_terms; j++)
        {
            if (strcmp(descriptor1->terms[i].word, descriptor2->terms[j].word) == 0)
            {
                total_common_terms += descriptor1->terms[i].count * descriptor2->terms[j].count;
                break;
            }
        }
    }
    return total_common_terms / (sqrt(descriptor1->total_terms) * sqrt(descriptor2->total_terms));
}

*/
double compare_descripteurs(Descripteur descripteur1, Descripteur descripteur2){
    int total_termes_commun= 0;
    for( int i=0; i<descripteur1.nb_token; i++) {
        for(int j =0; j<descripteur2.nb_token; j++){
            if (strcmp((char*)descripteur1.tokens[i].keyword, (char*)descripteur2.tokens[j].keyword) == 0){
                total_termes_commun += descripteur1.tokens[i].nb_occurence* descripteur2.tokens[j].nb_occurence;
                break;
            }
        }
    }
    return total_termes_commun / (sqrt(descripteur1.nb_token) * sqrt(descripteur2.nb_token)); 
}
int main() {
    /*
  //recherche par mot clé dans la bdd
  FILE * base_descripteur_texte;
  base_descripteur_texte = fopen("/home/sri-admin-etud/Documents/PFR/package_texte/base_descripteur_texte.txt", "r");

  int descriptors_length = 0;
  Descripteur * descriptors;
  descriptors = parse_base_descripteur(base_descripteur_texte, &descriptors_length);

    for (int i = 0; i < descriptors_length; i++) {
        printf("N°%d, ID: %d, nb_token: %d, max token: %d\n", i, descriptors[i].id, descriptors[i].nb_token, descriptors[i].nb_total_token);
    }

  char * search_keyword;
  printf("Saisir le mot clé : ");
  scanf("%s", search_keyword);

  search_by_keyword(search_keyword, descriptors, descriptors_length);

  //comparaison_fichier(&descriptors_length);

  //fclose(fp);
  //////////////////////////////////////

  
      //recherche par fihier 
      FILE *base_descripteur_texte = fopen("./package_t exte/base_descripteur_texte.txt", "r");//à remover 
      if (base_descripteur_texte == NULL)
      {
          printf("Impossible d'ouvrir le fichier base_descripteur_texte, il n'y a aucun fichier à comparer\n");
      }
      int descriptor_nb; 
      // Compare the text to all others in the list
      char path_texte[MAX_FILE_NAME_LENGTH];
      printf("Pour une recherche par chemin Saisir 1, pour une recherche par Identifiant saisir 2");  
      int TYPE_RECHERCHE; 
      scanf("%d",  TYPE_RECHERCHE); 
      switch(TYPE_RECHERCHE){
      case 1:
          printf("Entrer le chemin du texte source: ");
          scanf("%s", path_texte);
          //get identifiant 
          break; 

       
      case 2:
          printf("Entrer le numéro de l'identifiant: "); 
          int num_id; 
          scanf("%d", num_id);
          int existe=descriptor_exists(num_id); 
          if (existe==1){
              //texte déjà indexé 

          }
          if (existe==0){
              //le texte est à indexer 
             Descriptor *descipteur; 
             long file_size = ftell(text);
             rewind(text);
             char *document = malloc(file_size + 1); 
             //process_text(num_id, path_texte, descipteur ); 

          }
          break; 
          default: 
          printf("Saisir un chiffre entre 0 et 1");

       

          
      }*/
      FILE * base_descripteur1;
      base_descripteur1 = fopen("/home/sri-admin-etud/Documents/PFR/package_texte/descr1.txt", "r");
      
      int descriptors_length = 0;
      Descripteur * descr1;
      descr1 = parse_base_descripteur(base_descripteur1, &descriptors_length);
     
      FILE * base_descripteur2;
      base_descripteur2 = fopen("/home/sri-admin-etud/Documents/PFR/package_texte/descr2.txt", "r");
      
      int descriptors_length2 = 0;
      Descripteur * descr2;
      descr2 = parse_base_descripteur(base_descripteur2, &descriptors_length2);
     
      double val= compare_descripteurs(descr1[0], descr2[0]);
      
      printf("%.0f\n", val);

    return 0;
}