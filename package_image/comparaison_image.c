
/*Ensemble de fonction qui permet la comparaison et la recherche des image.
Auteur : Elio Genson
Date de début : 1ère semaine de janvier 2023.
Dernière modification : */

#include "module_image.h"


int compare(const void *a, const void *b)                         //fonction qui est appellé dans la fonction qsort. 
{                                                                 //Permet de choisir comment est trier le tableau
    struct info_comparaison *im1 = (struct info_comparaison *)b;  
    struct info_comparaison *im2 = (struct info_comparaison *)a;
    return im1->taux_de_similarite > im2->taux_de_similarite ? 1 :-1;
}



void comparaison_descripteur(){
    char descripteur_recherche[1000];
    char descripteur_indexe[1000];
    FILE *descripteurs=NULL;
    FILE *file_nb_descripteurs=NULL;
    FILE *file_descripteur_recherche=NULL;
    char *token_recherche;
    char *token_indexe;
    char id_recherche[50];

    char couleur_recherche[10];                          
    float seuil_similarite=60;
    float somme_recherche=0;
    int indice_descripteur=0;
    int valeur_token_recherche[64];
    unsigned long min=4000000;
    int nb_descripteurs=0;
    int nb_valeur = 64;
    char closest_id[100] = "";

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
    for (int i = 0; i < nb_descripteurs; i++)
    {
        if(strcmp(couleur_recherche,list_info[i].couleur_indexe) != 0){
            list_info[i].somme=400000;
            if(strcmp(list_info[i].couleur_indexe,"RGB")==0){                // cette partie permet de traiter les images noir et blanc
                                                                            //et RGB indépendament.
                list_info[i].somme=-(list_info[i].somme);
            }
//---------------------------------------------------------------------------------------------------------------------------------
        }else{
            list_info[i].somme=0;
            for (int j = 0; j < nb_valeur; j++)
            {
                list_info[i].difference[j] = MIN(list_info[i].valeur_token_indexe[j], valeur_token_recherche[j]);

                list_info[i].somme += list_info[i].difference[j];
            }
                
        }

    

        if (strcmp(list_info[i].couleur_indexe,"RGB") == 0)
        {
            list_info[i].taux_de_similarite = (list_info[i].somme/somme_recherche)*100;
        }
        else if (strcmp(list_info[i].couleur_indexe,"NB") == 0)
        {
            list_info[i].taux_de_similarite = ((list_info[i].somme/somme_recherche))*100;
        }
    }

    qsort(list_info, nb_descripteurs, sizeof(struct info_comparaison), compare);

    for (int i = 0; i < nb_descripteurs; i++)
    {
        printf("taux image %s = %f\n", list_info[i].id_image, list_info[i].taux_de_similarite);
    }

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
    
    system(commande);
    fclose(descripteurs);
    fclose(file_nb_descripteurs);
    fclose(file_descripteur_recherche);
    return;
}








int index_recherche(char *id_image)
{

    char path_image[1000];
    FILE *image_recherche=NULL;
    char couleur[100];
    FILE *descripteur_recherche=NULL;
    FILE *descripteur_indexe=NULL;
    char descripteur[1000];

    sprintf(path_image, "./TEST_RGB/%s.txt", id_image);
    image_recherche = fopen(path_image, "r");
    strcpy(couleur,"RGB");
    if (image_recherche == NULL)
    {
        sprintf(path_image, "./TEST_NB/%s.txt", id_image);
        image_recherche = fopen(path_image,"r");
        strcpy(couleur, "NB");
    }
    
    descripteur_recherche = fopen("./descripteur_recherche.txt", "w+");

    if (descripteur_recherche == NULL)
    {
        printf("Error opening file");
    }

    if (image_recherche == NULL)
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

    Indexer(image_recherche, id_image, couleur, descripteur_recherche);

    if (check_doublon(id_image)==0)
    {
        fgets(descripteur, 1000, descripteur_recherche);
        fprintf(descripteur_indexe,"%s",descripteur);
    }

    fclose(descripteur_indexe);
    fclose(descripteur_recherche);
    image_recherche = NULL;
    return 1;
}