#ifndef TOUTINDEXER_H
#define TOUTINDEXER_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>


void Indexer(FILE *image, char *id_image, char *couleur, FILE *fichier_descripteur);

void toutIndexer();

int check_doublon(char identifiant[]);

#endif