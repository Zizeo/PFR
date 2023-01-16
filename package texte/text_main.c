#include "text_descriptor_gen.h"

int main()
{
    clean_files();

    update_liste_emplacement_texte();

    // Ouvre le fichier contenant l'emplacement des fichiers à indexer
    FILE *liste_emplacement_texte = fopen("liste_emplacement_texte.txt", "r");
    if (liste_emplacement_texte == NULL)
    {
        printf("Impossible d'ouvrir le fichier liste_emplacement_texte\n");
        return 1;
    }

    // Initialise le descripteur
    Descriptor descriptor;
    int document_id = 0;

    // Lit chaque ligne du fichier contenant l'emplacement des fichiers à indexer
    char path_texte[100];
    while (fgets(path_texte, 100, liste_emplacement_texte) != NULL)
    {

        printf("%s\n", path_texte);
        path_texte[strlen(path_texte) - 1] = '\0';

        // Ouvre le fichier texte
        FILE *text = fopen(path_texte, "r");
        if (text == NULL)
        {
            printf("Impossible d'ouvrir le fichier %s\n", path_texte);
            continue;
        }

        // Lit le contenu du fichier texte
        fseek(text, 0, SEEK_END);
        long file_size = ftell(text);
        rewind(text);
        char *document = malloc(file_size + 1);
        fread(document, file_size, 1, text);
        document[file_size] = '\0';

        // Ferme le fichier texte
        fclose(text);

        // Appelle la fonction process_text en passant en paramètre l'id du document,
        // l'emplacement du fichier et le contenu du fichier
        process_text(document_id, path_texte, document, &descriptor);

        // Libère la mémoire allouée pour le document
        free(document);

        // Incrémente l'id du document
        document_id++;
    }

    // Ferme le fichier contenant l'emplacement des fichiers à indexer
    fclose(liste_emplacement_texte);

    return 0;
}