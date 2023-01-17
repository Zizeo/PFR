#include "toutIndexer.h"

void Indexer(FILE *image, char *id_image, char *couleur, FILE *fichier_descripteur)
{
    int li, co;
    int pix;
    unsigned int pixMasque;
    int c = 0;
    int composante;
    int masque = 0b11000000;
    int histogramme[64] = {0};
    int nombre_lu;

    fscanf(image, "%d", &li);
    fscanf(image, "%d", &co);
    fscanf(image, "%d", &composante);

    unsigned int R[li][co], G[li][co], B[li][co], binary_pix[li][co];

    printf("nb ligne = %d\n", li);
    printf("nb colone = %d\n", co);
    printf("nb composante = %d\n\n", composante);

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
            binary_pix[i][j] = R[i][j] | G[i][j] | B[i][j];
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

    return;
}

void toutIndexer(){
    system("ls ./TEST_RGB/*.txt > list_image.txt");
    system("wc -l < list_image.txt > nb_image.txt");
    system("ls ./TEST_RGB/*.txt | cut -d / -f 3 | cut -d . -f 1 > list_id_imageRGB.txt");

    FILE *image;
    FILE *list_image;
    FILE *nb_image;
    FILE *list_id_imageRGB;
    FILE *list_id_imageNB;
    FILE *fichier_descripteur;

    
    char path_image[100];
    char id_image[100];

    fichier_descripteur = fopen("./package_image/descripteurs/base_descripteur_image.csv", "a");

    list_image = fopen("list_image.txt", "r");
    list_id_imageRGB = fopen("list_id_imageRGB.txt", "r");
    nb_image = fopen("nb_image.txt", "r");

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

        int i = 0;
        fscanf(nb_image, "%d", &i);
        printf("i = %d\n", i);

        for (int j = 0; j < i; j++){
                     
        fgets(id_image, 100, list_id_imageRGB);
        fgets(path_image, 100, list_image);

        int check = check_doublon(id_image);

        if (check ==0 ){
                    
        printf("%s\n", path_image);
        path_image[strlen(path_image) - 1] = '\0';

        image = fopen(path_image, "r");
        if (image == NULL){
                            
        printf("Error opening file path_image\n");
        return;
        }
        char couleur[4] = "RGB";
        Indexer(image, id_image, couleur, fichier_descripteur);

        printf("fichier %s a bien été indexé\n",id_image);
        fclose(image);

        }else printf("fichier %s dejà indexé\n", id_image);
    }

    system("ls ./TEST_NB/*.txt > list_image.txt");
    system("wc -l < list_image.txt > nb_image.txt");
    system("ls ./TEST_NB/*.txt | cut -d / -f 3 | cut -d . -f 1 > list_id_imageNB.txt");

    list_image = fopen("list_image.txt", "r");
    nb_image = fopen("nb_image.txt", "r");
    list_id_imageNB = fopen("list_id_imageNB.txt", "r");

    if (list_image == NULL)
    {
        perror("Error opening file");
        return;
    }

    i = 0;

    fscanf(nb_image, "%d", &i);

    printf("i = %d\n", i);

    for (int j = 0; j < i; j++){

        fgets(id_image, 100, list_id_imageNB);
        fgets(path_image, 100, list_image);


        if (!check_doublon(id_image)){

            printf("%s\n", path_image);

            path_image[strlen(path_image) - 1] = '\0';

            image = fopen(path_image, "r");

            if (image == NULL){
            

                printf("Error opening file\n");

                return;
            }
            char couleur[4]="NB";
            Indexer(image, id_image, couleur, fichier_descripteur);

            printf("fichier %s a bien été indexé\n",id_image);

            fclose(image);
            

            }else printf("fichier %s déjà indexé\n",id_image);                
    }

    fclose(nb_image);
    fclose(list_image);
    fclose(list_id_imageNB);
    fclose(list_id_imageRGB);
    fclose(fichier_descripteur);
    system("rm list_id_imageNB.txt; rm list_id_imageRGB.txt; rm nb_image.txt; rm list_image.txt");

    return;
}


int check_doublon(char identifiant[]){
FILE *liste_id_indexee;


liste_id_indexee = fopen("./descripteurs/base_descripteur_image.csv", "r");
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