#include "comparaison_image.h"


int compare(const void *a, const void *b)
{
    struct info_comparaison *im1 = (struct info_comparaison *)a;
    struct info_comparaison *im2 = (struct info_comparaison *)b;
    return im1->somme - im2->somme;
}


void comparaison_descripteur(){
char descripteur_recherche[1000];
char descripteur_indexe[1000];
FILE *descripteurs;
FILE *file_nb_descripteurs;
FILE *file_descripteur_recherche;
char *token_recherche;
char *token_indexe;
char id_recherche[50];

char couleur_recherche[10];

int indice_descripteur=0;
int valeur_token_recherche[64];
unsigned long min=4000000;
int nb_descripteurs=0;
int nb_pixel = 64;
char closest_id[100] = "";

system("wc -l < ./descripteurs/base_descripteur_image.csv > nb_descripteurs.txt");

file_descripteur_recherche = fopen("descripteur_recherche.txt", "r");
if (file_descripteur_recherche == NULL)
{
    printf("Error opening descripteur_recherche.txt");
    return;
}

file_nb_descripteurs = fopen("nb_descripteurs.txt","r");
if(file_nb_descripteurs==NULL){
    printf("Error opening file_nb_descripteur");
    return;
}
fscanf(file_nb_descripteurs,"%d", &nb_descripteurs);

struct info_comparaison list_info[nb_descripteurs];

descripteurs = fopen("./descripteurs/base_descripteur_image.csv","r");

if(descripteurs==NULL){
    printf("Error opening file base_descripteur_image.csv");
    return;
}


fscanf(file_descripteur_recherche,"%s", id_recherche);

fscanf(file_descripteur_recherche,"%s", couleur_recherche);


for(int i=0; i<nb_pixel; i++){
    fscanf(file_descripteur_recherche,"%d", &valeur_token_recherche[i]);
}


while (fgets(descripteur_indexe, 10000, descripteurs) != NULL)
{
    token_indexe = strtok(descripteur_indexe, " ");
    strcpy(list_info[indice_descripteur].id_image, token_indexe);
    token_indexe = strtok(NULL, " ");
    strcpy(list_info[indice_descripteur].couleur_indexe, token_indexe);

    int indice_token = 0;

    while (token_indexe != NULL)
    {
        token_indexe = strtok(NULL, " ");
        if (token_indexe == NULL)
            break;
        int nombre_indexe = atoi(token_indexe);
        list_info[indice_descripteur].valeur_token_indexe[indice_token] = nombre_indexe;

        indice_token++;
    }

    indice_descripteur++;
    }

    for (int i = 0; i < nb_descripteurs; i++)
    {
        if(strcmp(couleur_recherche,list_info[i].couleur_indexe) != 0){
            list_info[i].somme=400000;

        }else{
            
            list_info[i].somme=0;
            for (int j = 0; j < nb_pixel; j++)
            {

                list_info[i].difference[j] = abs(list_info[i].valeur_token_indexe[j] - valeur_token_recherche[j]);

                list_info[i].somme += list_info[i].difference[j];
            }
                
            if (list_info[i].somme < min)
            {
                min = list_info[i].somme;
                strcpy(closest_id, list_info[i].id_image);
            }
        }
            
    }
    int closest_num =min;

    for (int i = 0; i < nb_descripteurs; i++)
    {
        printf("somme  =%lu\n", list_info[i].somme);
    }
    printf("closest = %d\n", closest_num);

    qsort(list_info, nb_descripteurs, sizeof(struct info_comparaison), compare);

    printf("l'image recherché est la %s\n", id_recherche);

    printf("les images les plus proches sont : \n");

    for(int i=0;i<4;i++){

        printf("%s.jpg ",list_info[i].id_image);
    }
    printf("\n");

    char commande[1000];
    if(strcmp(list_info[0].couleur_indexe,"RGB")==0){
        sprintf(commande, "viewnior ./TEST_RGB/%s.jpg",list_info[0].id_image);
    }else{
        sprintf(commande, "viewnior ./TEST_NB/%s.bmp",list_info[0].id_image);
    }
    
    system(commande);

    fclose(descripteurs);
    fclose(file_nb_descripteurs);
    fclose(file_descripteur_recherche);
}





void index_recherche(char *id_image)
{

    char path_image[1000];
    FILE *image;
    char couleur[10];
    FILE *descripteur_recherche;
    FILE *descripteur_indexe;
    char descripteur[1000];

    sprintf(path_image, "./TEST_RGB/%s.txt", id_image);
    image = fopen(path_image, "r");
    strcpy(couleur, "RGB");
    if (image == NULL)
    {
        sprintf(path_image, "./TEST_NB/%s.txt", id_image);
        image = fopen(path_image, "r");
        strcpy(couleur, "NB");
    }

    descripteur_recherche = fopen("./descripteur_recherche.txt", "w");

    if (descripteur_recherche == NULL)
    {
        printf("Error opening file");
    }

    if (image == NULL)
    {
        printf("Error opening file : cette image n'existe pas dans les répertoires d'image\n");
        return;
    }

    descripteur_indexe = fopen("./descripteurs/base_descripteur_image.csv", "a");

    if (descripteur_indexe == NULL)
    {
        printf("Error opening file");
    }

    Indexer(image, id_image, couleur, descripteur_recherche);

    if (!check_doublon(id_image))
    {
        fgets(descripteur, 1000, descripteur_recherche);
        fprintf(descripteur_indexe,"%s",descripteur);
    }

    fclose(descripteur_indexe);
    fclose(descripteur_recherche);
    return;
}