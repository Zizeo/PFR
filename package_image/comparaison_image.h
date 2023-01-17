#ifndef COMPARAISON_IMAGE
#define COMPARAISON_IMAGE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))


struct info_comparaison
{
    char id_image[200];
    int difference[64];
    float somme;
    int valeur_token_indexe[64];
    char couleur_indexe[10];
    float taux_de_similarite;
};

void comparaison_descripteur();
void index_recherche(char *id_image);

#endif