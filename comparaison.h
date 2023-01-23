#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <math.h>

#include <limits.h>

#include "./package_texte/text_descriptor_gen.h"


#define MAX_TOKEN 2000
#define MAX_TOKEN_LENGTH 50
#define MAX_PARAMETER_LENGTH 10
#define MAX_DESCRIPTEUR 100
#define TOKEN_DELIMITER ":"
#define SEUIL_SIMILARITE 0.5
#define MENU__CHOIX_RECHERCHE_CHEMIN 1
#define MENU__CHOIX_RECHERCHE_ID 2
#define BASE_DESCRIPTEUR "./package_texte/base_descripteur_texte.txt"

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

typedef struct {
  int id;
  float similarite;
}
SimilariteTexte; 
Descripteur * parse_base_descripteur(FILE * fp, int * descripteur_nb);

void search_by_keyword(char * mot_cle, Descripteur * descripteurs, int descripteurs_length);

float cosine_similarity(Descripteur desc1, Descripteur desc2);

void comparaison_par_fichier(Descripteur descripteur);

int get_new_document_id();

int indexation_fichier(char * path_texte, int id);

Descripteur * get_descripteur_par_id(int id);

char * get_path_by_id(int id);

int MENU__main();

void MENU__research_by_path();

void MENU__research_by_id();

int research_by_keyword();

int research_by_file();

