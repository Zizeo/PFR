#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <text_descriptor_gen.h>


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
    //int seuil= 50; 
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
    int temp= search_comparables[0].nb_occurence;
    for (int i=0 ; i < nb_apparition-1; i++)   {
        for (int j=0 ; j < nb_apparition-i-1; j++) {
      /* Pour un ordre décroissant utiliser < */
      int similarite_max = ((search_comparables[i].nb_occurence * 100)/temp);
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
    /*int temp= search_comparables[0].nb_occurence;
    printf("%d\n",temp);
    for(int i= 0; i<nb_apparition; i++){
        //printf("%d\n",search_comparables[0].nb_occurence);
        int similarite_max = ((search_comparables[i].nb_occurence * 100)/temp); //16
        printf("%d\n", similarite_max);
        if (similarite_max >SEUIL_SIMILARITE)


           //printf("ID: %d, Occurences: %d\n", search_comparables[j].id, search_comparables[j].nb_occurence );

       // }
        

    }*/
    

}

void comparaison_fichier( int descripteur_length ){
    FILE *f;
    char location_a_comparer[]="/home/sri-admin-etud/Documents/Projet_fil_rouge/4"; //insert base à comparer 
    f=fopen(location_a_comparer, "a");

    FILE *fptr_base_descr;
    fptr_base_descr= fopen("/home/sri-admin-etud/Documents/Projet_fil_rouge/TEXTE/base_descripteur_texte.txt", "a");
   
    Descripteur *descripteur_comparer; 
    descripteur_comparer = parse_base_descripteur(f, &descripteur_length);

    Descripteur *descripteur;
    descripteur= parse_base_descripteur(fptr_base_descr, &descripteur_length); 

    int nb_apparitions=0;
    for(int i=0; i<nb_apparitions; i++) {
        for(int j=0; j<descripteur_length; j++) {
            char * search_keyword= descripteur_comparer[i].tokens[j].keyword; 
            search_by_keyword(search_keyword, descripteur_comparer, descripteur_length); 
     }
    }
   // printf("%d",nb_apparition  )
    fclose(f);
    fclose(fptr_base_descr); 


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

double compare_descriptors(Descriptor *descriptor1, Descriptor *descriptor2)
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




int comparaison_texte(){
    FILE *fp;
    fp = fopen("/home/sri-admin-etud/Documents/Projet_fil_rouge/base_descripteurs.txt","rb");
    
    int descriptors_length = 0;
    Descripteur *descriptors;
    descriptors = parse_base_descripteur(fp, &descriptors_length);

    
    char * search_keyword; 
    printf("Saisir le mot clé : ");
    scanf("%s", search_keyword);

    search_by_keyword(search_keyword, descriptors, descriptors_length);

    comparaison_fichier(&descriptors_length);
    
    return 0;
}

int main (){
    
    FILE *base_descripteur_texte;
    base_descripteur_texte= fopen("/home/sri-admin-etud/Documents/Projet_fil_rouge/base_descripteurs.txt","rb");
    
    int descriptors_length = 0;
    Descripteur *descriptors;
    descriptors = parse_base_descripteur(base_descripteur_texte, &descriptors_length);

    
    char * search_keyword; 
    printf("Saisir le mot clé : ");
    scanf("%s", search_keyword);

    search_by_keyword(search_keyword, descriptors, descriptors_length);

    //comparaison_fichier(&descriptors_length);
    
    //fclose(fp);


    //FILE *base_descripteur_texte = fopen("./package_texte/base_descripteur_texte.txt", "r");
    if (base_descripteur_texte == NULL)
    {
        printf("Impossible d'ouvrir le fichier base_descripteur_texte, il n'y a aucun fichier à comparer\n");
    }
    int descriptor_nb; 
    // Compare the text to all others in the list
    char path_texte[MAX_FILE_NAME_LENGTH];
    printf("Pour une recherche par chemin Saisir 1, pour une recherche par Identifiant saisir 2");  
    int type_recherche; 
    scanf("%d",  type_recherche); 
    if(type_recherche==1 ){
        printf("Entrer le chemin du texte source: ");
        scanf("%s", path_texte);
        //get identifiant 

    } 
    if (type_recherche==2){
        printf("Entrer le numéro de l'identifiant: "); 
        int num_id; 
        scanf("%d", num_id);

        descriptor_exists( num_id); 

        
    }

    
   
    



*/

    return 0;

}
 

 //descripteur à comp if pas indexer, indexer 
 //focntion comparer le nombre en commun 