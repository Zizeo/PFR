
/*Ensemble de fonction qui permet la comparaison et la recherche des image.
Auteur : Elio Genson
Date de début : 1ère semaine de janvier 2023.
Dernière modification : */

#include "module_image.h"


int compare(const void *a, const void *b)                         
{ 
    //fonction qui est appellé dans la fonction qsort.
    //Permet de choisir comment est trier le tableau                                                                
    struct info_comparaison *im1 = (struct info_comparaison *)b;  
    struct info_comparaison *im2 = (struct info_comparaison *)a;
    return im1->taux_de_similarite > im2->taux_de_similarite ? 1 :-1;
}



void comparaison_descripteur(){
    char descripteur_indexe[1000];
    FILE *descripteurs=NULL;
    FILE *file_nb_descripteurs=NULL;
    FILE *file_descripteur_recherche=NULL;
    char *token_indexe;
    char id_recherche[50];

    char couleur_recherche[10];                          
    float seuil_similarite=60;
    float somme_recherche=0;
    int indice_descripteur=0;
    int valeur_token_recherche[64];
    int nb_descripteurs=0;
    int nb_valeur = 64;

    system("wc -l < ./package_image/descripteurs/base_descripteur_image.csv > nb_descripteurs.txt");

    file_descripteur_recherche = fopen("descripteur_recherche.txt", "r");
    if (file_descripteur_recherche == NULL)
    {
        printf("Error opening descripteur_recherche.txt");
        return;
    }

    file_nb_descripteurs = fopen("nb_descripteurs.txt","r");
    if(file_nb_descripteurs==NULL){
        printf("Error opening file_nb_descripteur");                    //dans cette partie, les fichiers sont ouverts et on vérifie qu'il sont bien ouverts.
        return;                                                         //et on déclare les variables.
    }               
    fscanf(file_nb_descripteurs,"%d", &nb_descripteurs);

    struct info_comparaison list_info[nb_descripteurs];

    descripteurs = fopen("./package_image/descripteurs/base_descripteur_image.csv","r");

    if(descripteurs==NULL){
        printf("Error opening file base_descripteur_image.csv");
        return;
    }
//--------------------------------------------------------------------------------------------------------------------------------

    fscanf(file_descripteur_recherche,"%s", id_recherche);
    if (strcmp(id_recherche,"\0")==0) return;

    fscanf(file_descripteur_recherche,"%s", couleur_recherche);          //dans cette partie, le programme lis les variable correspondant au descripteur de l'image recherché
    if (strcmp(couleur_recherche, "\0") == 0) return;
    

    for(int i=0; i<nb_valeur; i++){
        fscanf(file_descripteur_recherche,"%d", &valeur_token_recherche[i]);
    }
//--------------------------------------------------------------------------------------------------------------------------------

    while (fgets(descripteur_indexe, 10000, descripteurs) != NULL)
    {                                                                   //cette partie permet de lire chaque descripteur des immage indexé
                                                                        //et d'extraire l'identifiant, la composante couleur et les valeur de l'histogramme.
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
//-------------------------------------------------------------------------------------------------------------------------------------
    for (int i = 0; i < nb_valeur; i++)          //on fait la somme du descripteur recherché pour ensuite calculer le taux de similarité
    {
        somme_recherche+=valeur_token_recherche[i];   
    }
//--------------------------------------------------------------------------------------------------------------------------------
     // cette partie permet de traiter les images noir et blanc
     //et RGB indépendament.
    for (int i = 0; i < nb_descripteurs; i++)
    {
        if(strcmp(couleur_recherche,list_info[i].couleur_indexe) != 0){
            list_info[i].somme=-400000;
        
                                                                           
//---------------------------------------------------------------------------------------------------------------------------------
//ici on calcule l'intersection des descripteur, puis la somme des l'intersection et le taux de similarité
        }else{
            list_info[i].somme=0;
            for (int j = 0; j < nb_valeur; j++)
            {
            list_info[i].intersection[j] = MIN(list_info[i].valeur_token_indexe[j], valeur_token_recherche[j]);

            list_info[i].somme += list_info[i].intersection[j];
            }
                
        }
        list_info[i].taux_de_similarite = (list_info[i].somme/somme_recherche)*100;
    }
//--------------------------------------------------------------------------------------------------------------------------------
        //on trie la liste dans l'ordre décroissant avec qsort et on afficher les image les plus ressemblantes

    qsort(list_info, nb_descripteurs, sizeof(struct info_comparaison), compare);

    printf("l'image recherché est la %s\n", id_recherche);

    printf("les images les plus proches sont : \n");

    int nb_image_trouvee = 0;
    for(int i=0;i<5;i++){
        
        if(strcmp(list_info[i].id_image,id_recherche)==0) continue;

        if(list_info[i].taux_de_similarite > seuil_similarite){
            printf("%s.jpg ",list_info[i].id_image);
            nb_image_trouvee++;
        }else if(list_info[i].taux_de_similarite>0 && nb_image_trouvee==0){
            printf("%s.jpg ", list_info[i].id_image);
        }
    }
//--------------------------------------------------------------------------------------------------------------------------------
//on ouvre l'image la plus proches avec viewnior
//on a 2 cas : si l'image recherché est dans la base de descripteurs ou si elle n'y est pas.
    char commande[1000];
    if(strcmp(list_info[0].couleur_indexe,"RGB")==0){
        if(strcmp(list_info[0].id_image, id_recherche) == 0){
            sprintf(commande, "viewnior ./TEST_RGB/%s.jpg&", list_info[1].id_image);
        }else{
        sprintf(commande, "viewnior ./TEST_RGB/%s.jpg&", list_info[0].id_image);
        }
    }else{
        if (strcmp(list_info[0].id_image, id_recherche) == 0){
            sprintf(commande, "viewnior ./TEST_NB/%s.bmp&", list_info[1].id_image);
        }else{
        sprintf(commande, "viewnior ./TEST_NB/%s.bmp&", list_info[0].id_image);
        }
    }
//---------------------------------------------------------------------------------------------------------------------------------
    system(commande);
    fclose(descripteurs);
    fclose(file_nb_descripteurs);
    fclose(file_descripteur_recherche);
    return;
}




int index_recherche(char *id_image)
{
//--------------------------------------------------------------------------------------------------------------------------------
//déclaration des variable, ouverture des fichiers et vérifications
    char path_image[1000];
    FILE *image_rechercheNB=NULL;
    FILE *image_rechercheRGB = NULL;
    FILE *descripteur_recherche=NULL;
    FILE *descripteur_indexe=NULL;
    char descripteur[10000];

    sprintf(path_image, "./TEST_RGB/%s.txt", id_image);
    image_rechercheRGB = fopen(path_image, "r");

    sprintf(path_image, "./TEST_NB/%s.txt", id_image);
    image_rechercheNB = fopen(path_image,"r");

    descripteur_recherche = fopen("./descripteur_recherche.txt", "w+");

    if (descripteur_recherche == NULL)
    {
        printf("Error opening file");
    }

    if (image_rechercheRGB == NULL && image_rechercheNB==NULL)
    {
        printf("Error opening file : cette image n'existe pas dans les répertoires d'image\n");
        return -1;
    }

    descripteur_indexe = fopen("./package_image/descripteurs/base_descripteur_image.csv", "a");

    if (descripteur_indexe == NULL)
    {
        printf("\nError opening file descripteur_indexe\n");
        return -1;
    }
//---------------------------------------------------------------------------------------------------------------------
//gestion des doublons, on choisis quel descripteur à écrire dans le fichier descripteur_recherche.
    if (image_rechercheRGB != NULL && image_rechercheNB != NULL){
        printf("\nDoublon d'image détecté\n");
        printf("================================\n");
        printf("choisir une option :\n");
        printf("1. image RGB\n");
        printf("2. image noir et blanc\n");
        int choix_image;
        scanf("%d", &choix_image);
        switch (choix_image)
        {
        case 1:
        Indexer(image_rechercheRGB, id_image, "RGB", descripteur_recherche);
        break;
        
        case 2:
        Indexer(image_rechercheNB, id_image, "NB", descripteur_recherche);
        break;
        }

    }else if (image_rechercheRGB != NULL){
        Indexer(image_rechercheRGB, id_image, "RGB", descripteur_recherche);
    }else{
        Indexer(image_rechercheNB, id_image, "NB", descripteur_recherche);
    } 
//-----------------------------------------------------------------------------------------------------------
//si le descripteur de l'image recherché n'est pas dans la base de données on l'y ajoute.
    if (check_doublon(id_image, "RGB")==0 && check_doublon(id_image, "NB")==0)
    {
        rewind(descripteur_recherche);
        fgets(descripteur, 10000, descripteur_recherche);
        fprintf(descripteur_indexe,"%s",descripteur);
    }

    fclose(descripteur_indexe);
    fclose(descripteur_recherche);
    if(image_rechercheRGB != NULL) image_rechercheRGB=NULL;
    if(image_rechercheNB != NULL) image_rechercheNB=NULL;
    return 1;
}