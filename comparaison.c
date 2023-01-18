#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MAX_TOKEN 50
#define MAX_TOKEN_LENGTH 50
#define MAX_PARAMETER_LENGTH 10
#define MAX_DESCRIPTEUR 100
#define TOKEN_DELIMITER ":"

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

Descripteur* parse_base_descripteur(FILE* fp, int* descripteur_nb) {
    Descripteur *descripteurs = malloc(sizeof(Descripteur) * MAX_DESCRIPTEUR);

    // Check if file exists
    if (fp == NULL) {
        printf("Erreur: base descripteur vide");
        exit(EXIT_FAILURE); 
    }

    *descripteur_nb = 0;
    while(feof(fp) == 0 && *descripteur_nb < MAX_DESCRIPTEUR) {
        // Access & init a new Descripteur 
        Descripteur* descripteur = &descripteurs[*descripteur_nb];
        char line[MAX_PARAMETER_LENGTH];

        // Read 3 first lines
        // Line 0: id
        fgets(line, MAX_PARAMETER_LENGTH, fp);
        sscanf(line, "%d", &descripteur->id);

        // Line 1: nb_token
        fgets(line, MAX_PARAMETER_LENGTH, fp);
        sscanf(line, "%d", &descripteur->nb_token);

        // Line 2: nb_total_token
        fgets(line, MAX_PARAMETER_LENGTH, fp);
        sscanf(line, "%d", &descripteur->nb_total_token);

        if (descripteur->nb_token > MAX_TOKEN){
            printf("Erreur: le nombre de token est supérieur à la limite MAX_TOKEN %d", descripteur->nb_token);
            exit(EXIT_FAILURE); 
        }

        descripteur->tokens =(Token*)malloc(sizeof(Token) * descripteur->nb_token);

        // For nb_token, parse Token
        for (int t=0; t < descripteur->nb_token; t++) {
            char token_line[MAX_TOKEN_LENGTH];
            Token* token = &(descripteur->tokens[t]);

            fgets(token_line, MAX_TOKEN_LENGTH, fp);
            char* tokenized_line = strtok(token_line, TOKEN_DELIMITER);

            sscanf(tokenized_line, "%s", &token->keyword);
            sscanf(strtok(NULL, tokenized_line), "%d", &token->nb_occurence);

        }
        ++*descripteur_nb;
    }

    fclose(fp);
    return descripteurs;
}

void search_by_keyword(char* mot_cle, Descripteur *descripteurs, int descripteurs_length) {
    SearchComparableItem search_comparables[MAX_DESCRIPTEUR];
       
    int nb_apparition = 0;
    for(int j=0; j<descripteurs_length; j++) {
        for(int k=0; k<descripteurs[j].nb_token; k++) {
            if (strcmp(mot_cle, descripteurs[j].tokens[k].keyword) == 0) {
                nb_apparition += 1;
                search_comparables[j].id= descripteurs[j].id;
                search_comparables[j].nb_occurence= descripteurs[j].tokens[k].nb_occurence;
                break;
            }
        }          
       
    }

    for (int i=0 ; i < nb_apparition-1; i++)   {
        for (int j=0 ; j < nb_apparition-i-1; j++) {
      /* Pour un ordre décroissant utiliser < */
      if (search_comparables[j].nb_occurence < search_comparables[j+1].nb_occurence) {
        SearchComparableItem tmp = search_comparables[j];
        search_comparables[j]= search_comparables[j+1];
        search_comparables[j+1] = tmp;
      }
    }
  }

    for(int i= 0; i<nb_apparition; i++){
        printf("ID: %d, Occurences: %d\n", search_comparables[i].id, search_comparables[i].nb_occurence );

    }

}

/*void comparaison_fichier(Descripteur *descripteurs_comparer, int descripteur_length){
    FILE *fptr_base_descr;
    fptr_base_descr= fopen(/home/sri-admin-etud/Documents/Projet_fil_rouge/TEXTE/base_descripteur_texte.txt, "a");
    int nb_apparitions=0;
    for(int i=0; i<nb_apparitions; i++) {
        for(int j=0; j<descripteur_length; j++) {
            char * search_keyword= descripteurs_comparer[j].tokens[j].keyword; 
            search_by_keyword(search_keyword, descripteurs_comparer, descripteur_length);
            


    }
        }
    
    fclose(fptr_base_descr); 
}*/



//void search_by_similarity(int descripteur_id, Descripteur *descripteurs, int descripteurs_length) {
    // Find the descriptor from the ID
    // For each tokens of a given descripteur_id
    // Compare to all tokens in all les decripteurs excepted the concerned one
    // tab avec id et de taux de similarité, make them both des ints to avoid UNE STRUCTURE 
    // on recupère un dossier, check si le fichier existe dans la base des descripteurs 
    // si oui je recupère le descripteur sinon j'indexe 
//}

int main (int argc, char* argv[]){
    if (argc == 0) {
        printf("Usage: ./comparaison <chemin_base_descripteur>");
        exit(1);
    }

    char* location = "/home/sri-admin-etud/Documents/Projet_fil_rouge/base_descripteurs.txt";
    
    FILE *fp;
    fp = fopen(location,"rb");
    
    int descriptors_length = 0;
    Descripteur *descriptors;
    descriptors = parse_base_descripteur(fp, &descriptors_length);

    
    char * search_keyword; 
    printf("Saisir le mot clé : ");
    scanf("%s", search_keyword);

    search_by_keyword(search_keyword, descriptors, descriptors_length);
    
    return 0;
}
 