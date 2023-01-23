#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "./package_texte/text_descriptor_gen.h"

#define MAX_TOKEN 50
#define MAX_TOKEN_LENGTH 50
#define MAX_PARAMETER_LENGTH 10 
#define MAX_DESCRIPTEUR 100
#define TOKEN_DELIMITER ":"
#define SEUIL_SIMILARITE 50

typedef struct {
    int nb_occurence;
    char keyword[MAX_TOKEN_LENGTH]; // It should be less than MAX_TOKEN_LENGTH
} Token;

typedef struct {
    int id;
    int nb_token;
    int nb_total_token;
    Token *tokens;
} Descripteur;

typedef struct {
    int id;
    int nb_occurence;
} SearchComparableItem; 

Descripteur* parse_base_descripteur(FILE* fp, int* descripteur_nb);

void search_by_keyword(char* mot_cle, Descripteur *descripteurs, int descripteurs_length);

void comparaison_fichier( int descripteur_length );

double compare_descriptors(Descriptor *descriptor1, Descriptor *descriptor2);

void recherche_text_mot();
