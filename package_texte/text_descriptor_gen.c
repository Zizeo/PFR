#include "text_descriptor_gen.h"

void add_term(Descriptor *descriptor, char *word)
{
    // Recherche du terme dans la liste des termes du descripteur
    int i;
    for (i = 0; i < descriptor->total_terms; i++)
    {
        if (strcmp(descriptor->terms[i].word, word) == 0)
        {
            // Si le terme est déjà présent, on incrémente simplement son compteur
            descriptor->terms[i].count++;
            return;
        }
    }

    // Si le terme n'a pas été trouvé, on l'ajoute à la liste des termes du descripteur
    if (descriptor->total_terms < MAX_WORDS && strlen(word) < MAX_WORD_LENGTH)
    {
        strcpy(descriptor->terms[descriptor->total_terms].word, word);
        descriptor->terms[descriptor->total_terms].count = 1;
        descriptor->total_terms++;
    }
}

// Nettoye les fichier .xml en .tok
void clean_xml_files()
{
    system("chmod +x ./package_texte/clean.sh"); // give execute permission to the script
    DIR *d;
    struct dirent *dir;
    d = opendir("./package_texte/Textes");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            { // only regular files
                char *file_name = dir->d_name;
                if (strstr(file_name, ".xml"))
                { // only xml files
                    char filename[100];
                    sprintf(filename, "./package_texte/TOK/%s.tok", file_name);
                    FILE *file = fopen(filename, "r");
                    if (file == NULL)
                    {
                        char command[100];
                        sprintf(command, "./package_texte/clean.sh ./package_texte/Textes/%s", file_name);
                        system(command);
                    }
                    else
                    {
                        fclose(file);
                    }
                }
            }
        }
        closedir(d);
    }
}

int file_exists(char *file_path)
{
    FILE *fp;
    if ((fp = fopen(file_path, "r")))
    {
        fclose(fp);
        return 1;
    }
    return 0;
}

int descriptor_exists(int document_id)
{
    char buffer[BUFFER_SIZE];
    int found_id = 0;

    FILE *descriptor_file = fopen("./package_texte/base_descripteur_texte.txt", "r");
    if (descriptor_file == NULL)
    {
        return 0;
    }

    // Lire chaque ligne
    while (fgets(buffer, BUFFER_SIZE, descriptor_file) != NULL)
    {
        // Vérifie si il existe bien un document_id sur la ligne
        if (sscanf(buffer, "document_id=%d", &found_id) == 1)
        {

            if (found_id == document_id)
            {
                fclose(descriptor_file);
                return 1;
            }
        }
    }
    fclose(descriptor_file);
    return 0;
}

// Met à jour la liste des empalcements des textes
void update_liste_emplacement_texte()
{
    // Créer un nouveau liste_emplacement_texte.txt
    FILE *fp = fopen("./package_texte/liste_emplacement_texte.txt", "w");
    if (fp == NULL)
    {
        printf("Could not open file liste_emplacement_texte.txt\n");
        return;
    }
    fclose(fp);

    system("ls ./package_texte/TOK/*.tok > ./package_texte/liste_emplacement_texte.txt");

    // Créer un nouveau base_descripteur_texte.txt
    fp = fopen("./package_texte/base_descripteur_texte.txt", "w");
    if (fp == NULL)
    {
        printf("Could not open file base_descripteur_texte.txt\n");
        return;
    }
    fclose(fp);
    system("echo \" > ./package_texte/base_descripteur_texte.txt");

    // Créer un nouveau base_descripteur_texte.txt
    fp = fopen("./package_texte/liste_base_texte.txt", "w");
    if (fp == NULL)
    {
        printf("Could not open file liste_base_texte.txt\n");
        return;
    }
    fclose(fp);
    system("echo \" > ./package_texte/liste_base_texte.txt");
}

void process_text(int document_id, char *file_path, char *document, Descriptor *descriptor)
{

    // Vérifie si le descripteur existe déjà
    if (descriptor_exists(document_id))
    {
        printf("Le descripteur du document %d existe déjà\n", document_id);
        return;
    }

    // Initialisation du descripteur
    descriptor->document_id = document_id;
    strcpy(descriptor->file_path, file_path);
    descriptor->total_terms = 0;
    descriptor->document_size = 0;

    if (strlen(document) > MAX_DOCUMENT_SIZE)
    {
        return;
    }

    // Traitement du document
    char word[MAX_WORD_LENGTH];
    int i, j;
    for (i = 0, j = 0; document[i] != '\0'; i++)
    {
        // Si on trouve autre chose qu'un espace, on l'ajoute au terme en cours de construction
        if (!isspace(document[i]))
        {
            word[j++] = document[i];
        }
        // Si on trouve un caractère de fin de terme, on ajoute le terme à la liste des termes du descripteur
        else if (j > 0 && descriptor->document_size < MAX_DOCUMENT_SIZE)
        {
            word[j] = '\0';
            add_term(descriptor, word);
            descriptor->document_size++;
            j = 0;
        }
    }

    // Si le dernier terme n'a pas été ajouté à la liste des termes (caractère de fin de mot manquant), on l'ajoute maintenant
    if (j > 0)
    {
        word[j] = '\0';
        add_term(descriptor, word);
        descriptor->document_size++;
    }

    // Sauvegarde le descripteur dans base_descriptor_texte
    FILE *descriptor_file = fopen("./package_texte/base_descripteur_texte.txt", "a");
    fprintf(descriptor_file, "%d\n", descriptor->document_id);
    // Nombre total de token
    fprintf(descriptor_file, "%d\n", descriptor->total_terms);
    // Taille du document
    fprintf(descriptor_file, "%d\n", descriptor->document_size);
    for (i = 0; i < descriptor->total_terms; i++)
    {
        fprintf(descriptor_file, "%s:%d\n", descriptor->terms[i].word, descriptor->terms[i].count);
    }

    fclose(descriptor_file);

    // Sauvegarde le lien entre le descripteur et le document dans liste_base_texte
    FILE *document_list_file = fopen("./package_texte/liste_base_texte.txt", "a");
    fprintf(document_list_file, "%d %s\n", descriptor->document_id, file_path);
    fclose(document_list_file);
}

void indexerText()
{
    clean_xml_files();

    update_liste_emplacement_texte();

    // Ouvre le fichier contenant l'emplacement des fichiers à indexer
    FILE *liste_emplacement_texte = fopen("./package_texte/liste_emplacement_texte.txt", "r");
    if (liste_emplacement_texte == NULL)
    {
        printf("Impossible d'ouvrir le fichier liste_emplacement_texte\n");
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

    // Remplacer dernier \n dans la base descripteur texte par un token nul
    FILE *descriptor_file = fopen("./package_texte/base_descripteur_texte.txt", "a");
    fprintf(descriptor_file, "end:0");
    fclose(descriptor_file);
}