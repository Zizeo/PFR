/*Ensemble de fonction qui permet l'indexation des image.
Auteur : Elio Genson
Date de début : 1ère semaine de décembre 2022.
Dernière modification : */

#include "module_image.h"

void Indexer(FILE *image, char *id_image, char *couleur, FILE *fichier_descripteur)
{
    int li, co;
    int pix;
    int bit_quantification;
    unsigned int pixMasque;
    FILE *config;

    config = fopen("./config.txt","r");

    if(config == NULL){
        printf("Error oppening config file");
        return;
    }

    fscanf(config, "bits de quantification= %d", &bit_quantification);

    if(bit_quantification<0 || bit_quantification>8){
        printf("nombre de bit de quantification invalide : > 8 ou < 0");
        return;
    }
    int taille_histogramme;
    int masque = (unsigned char)((1 << bit_quantification) - 1) << (8 - bit_quantification);

    int c = 0;
    int composante=0;
    int histogramme[64] = {0};
    int nombre_lu=0;

    fscanf(image, "%d", &li);
    fscanf(image, "%d", &co);
    fscanf(image, "%d", &composante);

    unsigned int R[li][co], G[li][co], B[li][co], binary_pix[li][co];


    for (c = 0; c < composante; c++)
    {
        for (int i = 0; i < li; i++)
        {

            for (int j = 0; j < co; j++)
            {

                fscanf(image, "%d", &pix);

                pixMasque = pix & masque;

                if (c == 0)
                {
                    R[i][j] = (pixMasque >> 2);
                }
                else if (c == 1)
                {
                    G[i][j] = (pixMasque >> 4);
                }
                else if (c == 2)
                {
                    B[i][j] = (pixMasque >> 6);
                }
            }
        }
    }

    for (int i = 0; i < li; i++)
    {
        for (int j = 0; j < co; j++)
        {
            if(composante==1){
                binary_pix[i][j] = R[i][j];
            }else{
            binary_pix[i][j] = R[i][j] | G[i][j] | B[i][j];
            }
        }
    }

    for (int i = 0; i < li; i++)
    {
        for (int j = 0; j < co; j++)
        {
            nombre_lu = binary_pix[i][j];
            histogramme[nombre_lu] += 1;
        }
    }

    fprintf(fichier_descripteur, "%s", id_image);
    fprintf(fichier_descripteur, " %s", couleur);

    for (int k = 0; k < 64; k++)
    {

        fprintf(fichier_descripteur, " %d", histogramme[k]);
    }

    fprintf(fichier_descripteur, "\n");
    fclose(image);
    return;
}



void toutIndexer(){
    system("ls ./TEST_RGB/*.txt > list_imageRGB.txt");
    system("wc -l < list_imageRGB.txt > nb_imageRGB.txt");
    system("ls ./TEST_NB/*.txt > list_imageNB.txt");
    system("wc -l < list_imageNB.txt > nb_imageNB.txt");
    system("ls ./TEST_RGB/*.txt | cut -d / -f 3 | cut -d . -f 1 > list_nom_imageRGB.txt");

    FILE *image;
    FILE *list_image;
    FILE *nb_image;
    FILE *list_nom_imageRGB;
    FILE *list_nom_imageNB;
    FILE *fichier_descripteur;

    char path_image[100];
    char nom_image[100];


    fichier_descripteur = fopen("package_image/descripteurs/base_descripteur_image.csv", "a+");

    list_image = fopen("list_imageRGB.txt", "r");
    list_nom_imageRGB = fopen("list_nom_imageRGB.txt", "r");
    nb_image = fopen("nb_imageNB.txt", "r");
    int i = 0;
    fscanf(nb_image, "%d", &i);
    int buffer_id_total = i;
    nb_image = fopen("nb_imageRGB.txt", "r");
    i = 0;
    fscanf(nb_image, "%d", &i);
    buffer_id_total += i;
    int buffer_idRGB = i;
    printf("%d\n",buffer_id_total);
    char list_nom_id[buffer_id_total][1][100];

    if (fichier_descripteur==NULL)
    {
        perror("Error opening file base_descripteur_image.csv\n");
        return;
    }
    

    if (list_image == NULL)
    {
        perror("Error opening file list_image.txt\n");
        return;
    }

    for(int i =0; i < buffer_id_total; i++){
        char str_i[10];
        sprintf(str_i,"%d",i);
        strcpy(list_nom_id[i][1],str_i);
    }

    for (int j = 0; j < i; j++){
                     
        fgets(nom_image, 100, list_nom_imageRGB);
        fgets(path_image, 100, list_image);

        strcpy(list_nom_id[j][0], nom_image);

        int check = check_doublon(list_nom_id[j][1]);

        if (check ==0 ){
                    
        printf("%s\n", path_image);
        path_image[strlen(path_image) - 1] = '\0';

        image = fopen(path_image, "r");
        if (image == NULL){
                            
        printf("Error opening file path_image\n");
        return;
        }
        char couleur[4] = "RGB";
        Indexer(image, list_nom_id[j][1], couleur, fichier_descripteur);

        printf("fichier %s a bien été indexé\n",nom_image);


        }else printf("fichier %s dejà indexé\n", nom_image);
    }

    system("ls ./TEST_NB/*.txt | cut -d / -f 3 | cut -d . -f 1 > list_nom_imageNB.txt");

    list_image = fopen("list_imageNB.txt", "r");
    nb_image = fopen("nb_imageNB.txt", "r");
    list_nom_imageNB = fopen("list_nom_imageNB.txt", "r");

    if (list_image == NULL)
    {
        perror("Error opening file");
        return;
    }

    i = 0;

    fscanf(nb_image, "%d", &i);

    for (int j = 0; j < i; j++){

        fgets(nom_image, 100, list_nom_imageNB);
        fgets(path_image, 100, list_image);
        strcpy(list_nom_id[buffer_idRGB+j][0], nom_image);

        if (!check_doublon(list_nom_id[buffer_idRGB + j][1]))
        {

        printf("%s\n", path_image);

        path_image[strlen(path_image) - 1] = '\0';

        image = fopen(path_image, "r");

        if (image == NULL)
        {

        printf("Error opening file\n");

        return;
        }
        char couleur[4] = "NB";

        Indexer(image, list_nom_id[buffer_idRGB + j][1], couleur, fichier_descripteur);

        printf("fichier %s a bien été indexé\n", nom_image);
        }
        else
        printf("fichier %s déjà indexé\n", nom_image);
    }

    for (int i = 0; i < buffer_id_total; i++)
    {
        printf("nom : %s ; id : %s \n", list_nom_id[i][0], list_nom_id[i][1]);
    }

    fclose(nb_image);
    fclose(list_image);
    fclose(list_nom_imageNB);
    fclose(list_nom_imageRGB);
    fclose(fichier_descripteur);

    system("rm list_nom_imageNB.txt; rm list_nom_imageRGB.txt; rm nb_imageNB.txt; rm list_imageNB.txt; rm nb_imageRGB.txt; rm list_imageRGB.txt");
    printf("\nIndexation terminé\n");

    return;
}


int check_doublon(char identifiant[]){
FILE *liste_id_indexee;

liste_id_indexee = fopen("./package_image/descripteurs/base_descripteur_image.csv", "r");
    if (liste_id_indexee == NULL)
    {
        perror("Error opening file list_image.txt\n");
        return -1;
    }

char buffer[1000];

char *id_descripteur;

if(identifiant[strlen(identifiant)-1] == '\n'){
identifiant[strlen(identifiant) - 1] = '\0';
}

if (liste_id_indexee)
{
        while (fgets(buffer, 1000, liste_id_indexee) != NULL){

            id_descripteur = strtok(buffer, " ");

            if (strcmp(id_descripteur, identifiant) == 0)
            {
            return 1;
            }
        }
        fclose(liste_id_indexee);
}
return 0;
}
