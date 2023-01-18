#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "./package_image/module_image.h"

int main()
{
  int quitM1 = 0;
  int quitM2 = 0;
  int quitM3 = 0;
  int mode = 0;
  int type_search = 0;
  char mot_cle[1000];
  char id_image[100];

  // Demande à l'utilisateur de choisir le mode d'utilisation

  toutIndexer();

  while (quitM1 == 0)
  {
    mode = 0;
    quitM2 = 0;
    printf("\n\n======== LUNARSEARCH ========\n\n");
    printf("______Menu Principal______\n");
    printf("\nChoisissez le mode d'utilisation :\n");
    printf("1. Mode utilisateur\n");
    printf("2. Mode administrateur\n");
    printf("3. Quitter\n\n");
    printf("Entrez le numéro de l'option que vous souhaitez : \n");
    scanf("%d", &mode);
    if (mode < 1 || mode > 3)
    {
      printf(" ⚠ Valeur non valide, veuillez réessayer. ⚠ \n");
      scanf("%s");
      sleep(1);
    }
    else
    {
      if (mode == 1)
      {
        // Demande à l'utilisateur de choisir une action à réaliser
        while (quitM2 == 0 && mode == 1)
        {
          type_search = 0;
          quitM3 = 0;
          // Affiche le menu principal
          printf("\n\n______Mode utilisateur :______\n\n");
          printf("1. Recherche de Texte grâce à un mot clé.\n");
          printf("2. Recherche de Texte à partir d'un fichier texte source.\n");
          printf("3. Recherche d'image à partir d'une image source.\n");
          printf("4. Recherche d'image par couleur dominante.\n");
          printf("5. Recherche d'audio à partir d'un fichier audio source.\n");
          printf("6. Retour au menu principal.\n\n");
          printf("Entrez le numéro de l'action que vous souhaitez réaliser : \n");

          scanf("%d", &type_search);

          if (type_search < 1 || type_search > 6)
          {
            printf("⚠ Valeur non valide, veuillez réessayer. ⚠ \n");
            scanf("%s");
            sleep(1);
          }
          else
          {
            switch (type_search)
            {
            case 1:
              printf("\nVeuillez saisir un mot clé pour votre recherche : ");
              scanf(" %s", mot_cle);

              printf("Recherche de texte avec le mot clé ");
              printf(" '%s' ", mot_cle);
              printf(" en cours...\n");
              sleep(2);

              break;

            case 2:
              printf("\nRecherche de Texte à partir d'un fichier texte source en cours...\n");
              sleep(2);
              break;

            case 3:
              printf("\nRentrez l'identifiant de l'image source : ");
              scanf("%s", id_image);
              index_recherche(id_image);
              printf("\nRecherche d'image à partir d'une image source en cours...\n");
              sleep(2);
              comparaison_descripteur(id_image);
              break;

            case 4:
              while (quitM3 == 0)
              {
                type_search = 0;
                // Affiche le menu principal
                printf("\n\n______Recherche d'image par couleur domiante :______\n\n");
                printf("1. Rouge\n");
                printf("2. Vert\n");
                printf("3. Bleu\n");
                printf("4. Bleu Ciel\n");
                printf("5. Magenta\n");
                printf("6. Jaune\n");
                printf("7. Retour au menu de sélection de recherche.\n\n");
                printf("Entrez le numéro de l'action que vous souhaitez réaliser : \n");

                scanf("%d", &type_search);

                if (type_search < 1 || type_search > 7)
                {
                  printf(" ⚠ Valeur non valide, veuillez réessayer. ⚠ \n");
                  scanf("%s");
                  sleep(1);
                }
                else
                {
                  switch (type_search)
                  {
                  case 1:
                    printf("Recherche de d'image par couleur dominante rouge en cours...\n");
                    sleep(1);
                    index_recherche("rouge");
                    comparaison_descripteur();

                    break;

                  case 2:
                    printf("Recherche de d'image par couleur dominante verte en cours...\n");
                    sleep(1);
                    index_recherche("vert");
                    comparaison_descripteur();
                    break;

                  case 3:
                    printf("Recherche de d'image par couleur dominante bleue en cours...\n");
                    sleep(1);
                    index_recherche("bleu");
                    comparaison_descripteur();
                    break;

                  case 4:
                    printf("Recherche de d'image par couleur dominante bleue ciel en cours...\n");
                    sleep(1);
                    index_recherche("bleu_ciel");
                    comparaison_descripteur();
                    break;

                  case 5:
                    printf("Recherche de d'image par couleur dominante majenta en cours...\n");
                    sleep(1);
                    index_recherche("majenta");
                    comparaison_descripteur();
                    break;

                  case 6:
                    printf("Recherche de d'image par couleur dominante jaune en cours...\n");
                    sleep(1);
                    index_recherche("jaune");
                    comparaison_descripteur();
                    break;

                  case 7:
                    printf("\nRetour au Menu Mode Utilisateur en cours . . .\n\n");
                    sleep(2);
                    quitM3 = 1;
                    break;
                  }
                }
              }
              break;
            case 5:
              printf("\n ⚠ Recherche d'audio à partir d'un fichier audio source Non-Implémenté ⚠ \n");
              sleep(1);
              break;

            case 6:
              printf("\nRetour au Menu Principal en cours . . .\n\n");
              sleep(1);
              quitM2 = 1;
              break;
            }
          }
        }
      }
      else if (mode == 2)
      {
        printf("\n ⚠ Mode Administrateur Non-Implémenté ⚠ \n");
        sleep(1);
      }
      else if (mode == 3)
      {
        printf("\nArrêt de LUNARSEARCH en cours . . .\n");
        sleep(1);
        quitM1 = 1;
        break;
      }
    }
  }
  return 0;
}