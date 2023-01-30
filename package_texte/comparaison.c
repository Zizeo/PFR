#include "./comparaison.h"

Descripteur *parse_base_descripteur(FILE *fp, int *descripteur_nb)
{
  Descripteur *descripteurs = malloc(sizeof(Descripteur) * MAX_DESCRIPTEUR);

  // Verifier si fichier existe
  if (fp == NULL)
  {
    printf("Erreur: base descripteur vide");
    exit(EXIT_FAILURE);
  }

  *descripteur_nb = 0;
  while (feof(fp) == 0 && *descripteur_nb < MAX_DESCRIPTEUR)
  {

    Descripteur *descripteur = &descripteurs[*descripteur_nb];
    char line[MAX_PARAMETER_LENGTH];

    fgets(line, MAX_PARAMETER_LENGTH, fp);
    sscanf(line, "%d", &descripteur->id);

    fgets(line, MAX_PARAMETER_LENGTH, fp);
    sscanf(line, "%d", &descripteur->nb_token);

    fgets(line, MAX_PARAMETER_LENGTH, fp);
    sscanf(line, "%d", &descripteur->nb_total_token);

    if (descripteur->nb_token > MAX_TOKEN)
    {
      printf("Erreur: le nombre de token est supérieur à la limite MAX_TOKEN %d", descripteur->nb_token);
      exit(EXIT_FAILURE);
    }

    descripteur->tokens = (Token *)malloc(sizeof(Token) * descripteur->nb_token);

    for (int t = 0; t < descripteur->nb_token; t++)
    {
      char token_line[MAX_TOKEN_LENGTH];
      Token *token = &(descripteur->tokens[t]);

      fgets(token_line, MAX_TOKEN_LENGTH, fp);
      char *tokenized_line = strtok(token_line, TOKEN_DELIMITER);

      sscanf(tokenized_line, "%s", (char *)&token->keyword);
      sscanf(strtok(NULL, tokenized_line), "%d", &token->nb_occurence);
    }
    ++*descripteur_nb;
  }

  fclose(fp);
  return descripteurs;
}

// recherche par mot clé
void search_by_keyword(char *mot_cle, Descripteur *descripteurs, int descripteurs_length)
{
  int nb_apparition = 0;
  SearchComparableItem search_comparables[MAX_DESCRIPTEUR]; //, sizeof *search_comparables);

  for (int i = 0; i < descripteurs_length; i++)
  {
    for (int j = 0; j < descripteurs[i].nb_token; j++)
    {
      if (strcmp(mot_cle, descripteurs[i].tokens[j].keyword) == 0)
      {
        search_comparables[nb_apparition].id = descripteurs[i].id;
        search_comparables[nb_apparition].nb_occurence = descripteurs[i].tokens[j].nb_occurence;
        nb_apparition++;
      }
    }
  }

  // tri à bulle
  for (int i = 0; i < nb_apparition - 1; i++)
  {
    for (int j = 0; j < nb_apparition - i - 1; j++)
    {
      if (search_comparables[j].nb_occurence < search_comparables[j + 1].nb_occurence)
      {
        SearchComparableItem tmp = search_comparables[j];
        search_comparables[j] = search_comparables[j + 1];
        search_comparables[j + 1] = tmp;
      }
    }
  }

  int max_occ = 0;
  if (nb_apparition > 0)
  {
    max_occ = search_comparables[0].nb_occurence;
    printf("Occ max %d\n", max_occ);
  }

  for (int i = 0; i < nb_apparition; i++)
  {
    printf("ID: %d Fichier: %s Occurences: %d\n", search_comparables[i].id, get_path_by_id(search_comparables[i].id), search_comparables[i].nb_occurence);
  }
}
// calcule de similiarité entre deux descripteur par la similarité de Cosinus
float cosine_similarity(Descripteur desc1, Descripteur desc2)
{
  float dot_product = 0.0, desc1_norm = 0.0, desc2_norm = 0.0;

  for (int i = 0; i < desc1.nb_token; i++)
  {
    for (int j = 0; j < desc2.nb_token; j++)
    {
      if (strcmp(desc1.tokens[i].keyword, desc2.tokens[j].keyword) == 0)
      {
        dot_product += desc1.tokens[i].nb_occurence * desc2.tokens[j].nb_occurence;
        break;
      }
    }
    desc1_norm += pow(desc1.tokens[i].nb_occurence, 2);
  }

  for (int i = 0; i < desc2.nb_token; i++)
  {
    desc2_norm += pow(desc2.tokens[i].nb_occurence, 2);
  }

  return dot_product / (sqrt(desc1_norm) * sqrt(desc2_norm));
}
// comparaison par fichier
void comparaison_par_fichier(Descripteur descripteur)
{
  int descriptors_length = 0;
  FILE *base_descripteur = fopen(BASE_DESCRIPTEUR, "r");

  Descripteur *descripteurs = parse_base_descripteur(base_descripteur, &descriptors_length);
  SimilariteTexte similarites[descriptors_length];

  for (int i = 0; i < descriptors_length; i++)
  {
    // On ne le compare pas à lui même
    if (descripteur.id == descripteurs[i].id)
    {
      continue;
    }

    similarites[i].id = descripteurs[i].id;
    similarites[i].similarite = cosine_similarity(descripteur, descripteurs[i]);
  }

  // Tri bulle
  for (int i = 0; i < descriptors_length; i++)
  {
    for (int j = 0; j < descriptors_length - i; j++)
    {
      if (similarites[j].similarite < similarites[j + 1].similarite)
      {
        SimilariteTexte tmp = similarites[j];

        similarites[j] = similarites[j + 1];
        similarites[j + 1] = tmp;
      }
    }
  }

  for (int i = 0; i < descriptors_length; i++)
  {
    if (similarites[i].similarite > 0.5)
    {
      printf("ID: %d Fichier %s similaire à %.0f%%\n", similarites[i].id, get_path_by_id(similarites[i].id), similarites[i].similarite * 100);
    }
  }
}
// Trouver l'identifiant d'un fichier
int get_new_document_id()
{
  int biggest_id = INT_MIN;
  char line[MAX_FILE_PATH_LENGTH];
  FILE *liste_base_texte = fopen("./package_texte/liste_base_texte.txt", "rb");

  while (fgets(line, MAX_FILE_PATH_LENGTH, liste_base_texte) != NULL)
  {
    int id;
    sscanf(line, "%d ", &id);
    if (id > biggest_id)
    {
      biggest_id = id;
    }
  }

  return biggest_id + 1;
}

// Trouver un descripteur à partir de l'identifiant
Descripteur *get_descripteur_par_id(int id)
{
  FILE *base_descripteur_texte = fopen(BASE_DESCRIPTEUR, "rb");
  int descripteur_length = 0;
  Descripteur *descripteurs = parse_base_descripteur(base_descripteur_texte, &descripteur_length);

  for (int i = 0; i < descripteur_length; i++)
  {
    if (descripteurs[i].id == id)
    {
      return &descripteurs[i];
    }
  }

  return (Descripteur *)NULL;
  fclose(base_descripteur_texte);
}

// Trouver chemin grace à l'identifiant
char *get_path_by_id(int id)
{
  char line[MAX_FILE_PATH_LENGTH];
  FILE *liste_base_texte = fopen("./package_texte/liste_base_texte.txt", "rb");
  while (fgets(line, MAX_FILE_PATH_LENGTH, liste_base_texte) != NULL)
  {
    int rid;
    char *path = malloc(MAX_FILE_PATH_LENGTH);
    sscanf(line, "%d %s", &rid, path);
    if (rid == id)
    {
      fclose(liste_base_texte);
      return path;
    }
  }
  fclose(liste_base_texte);
  return NULL;
}

int MENU__search_by_file()
{
  int choix;
  printf("Pour une recherche par :\n  %d. Chemin\n  %d. Identifiant\n\nEntrer votre choix (%d/%d): ",
         MENU__CHOIX_RECHERCHE_CHEMIN, MENU__CHOIX_RECHERCHE_ID, MENU__CHOIX_RECHERCHE_CHEMIN, MENU__CHOIX_RECHERCHE_ID);
  scanf("%d", &choix);
  return choix;
}

void MENU__research_by_path()
{
  int id;
  char *text_path = malloc(MAX_FILE_NAME_LENGTH);
  printf("Entrer le chemin du texte source: ");
  // scanf("%d %s", & id, text_path);
  scanf("%d", &id);
  printf("%d", id);

  char *path_texte = get_path_by_id(id);
  if (path_texte == NULL)
  {
    printf("Erreur: Le chemin est invalide");
    return;
  }

  Descripteur *descripteur = get_descripteur_par_id(id);
  comparaison_par_fichier(*descripteur);
}

// verifie si le descripteur existe
int descripteur_exists(int id)
{
  int descriptors_length = 0;
  FILE *base_descripteur_texte = fopen(BASE_DESCRIPTEUR, "r");
  Descripteur *descriptors = parse_base_descripteur(base_descripteur_texte, &descriptors_length);

  for (int i = 0; i < descriptors_length; i++)
  {
    if (descriptors[i].id == id)
    {
      return 1;
    }
  }
  return 0;
}

void MENU__research_by_id()
{
  int id;

  printf("Entrer le numéro de l'identifiant: ");
  scanf("%d", &id);
  if (descripteur_exists(id) == 1)
  {
    // char * path_texte = get_path_by_id(id);
    // if (path_texte == NULL) {
    // printf("Erreur: Le chemin est invalide.");
    // return;
    Descripteur *descripteur = get_descripteur_par_id(id);

    comparaison_par_fichier(*descripteur);
  }
}
// Recherche par mot clé
int research_by_keyword()
{
  int descriptors_length = 0;
  char search_keyword[MAX_TOKEN_LENGTH];
  FILE *base_descripteur_texte = fopen(BASE_DESCRIPTEUR, "r");
  Descripteur *descriptors = parse_base_descripteur(base_descripteur_texte, &descriptors_length);

  printf("\nVeuillez saisir un mot clé pour votre recherche : ");
  scanf("%s", (char *)search_keyword);

  search_by_keyword(search_keyword, descriptors, descriptors_length);
}

// Recherche par fichier
int research_by_file()
{
  FILE *base_descripteur_texte;
  base_descripteur_texte = fopen(BASE_DESCRIPTEUR, "r");

  if (base_descripteur_texte == NULL)
  {
    printf("Impossible d'ouvrir le fichier base_descripteur_texte, il n'y a aucun fichier à comparer\n");
    exit(1);
  }

  int descriptor_nb;
  int TYPE_RECHERCHE;

  while (TYPE_RECHERCHE != MENU__CHOIX_RECHERCHE_CHEMIN && TYPE_RECHERCHE != MENU__CHOIX_RECHERCHE_ID)
  {
    TYPE_RECHERCHE = MENU__search_by_file();
    switch (TYPE_RECHERCHE)
    {
    case MENU__CHOIX_RECHERCHE_CHEMIN:
      MENU__research_by_path();
      break;

    case MENU__CHOIX_RECHERCHE_ID:
      MENU__research_by_id();
      break;

    default:
      printf("Saisir un chiffre entre 1 et 2");
    }
  }

  return 0;
}
