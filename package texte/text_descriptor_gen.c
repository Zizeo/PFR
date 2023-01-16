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
void clean_files()
{
    system("chmod +x clean.sh"); // Permission d'éxécuter
    DIR *d;
    struct dirent *dir;
    d = opendir("Textes");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                char *file_name = dir->d_name;
                if (strstr(file_name, ".xml"))
                {
                    char command[100];
                    sprintf(command, "./clean.sh Textes/%s", file_name);
                    system(command);
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

    FILE *descriptor_file = fopen("./base_descripteur_texte.txt", "r");
    if (descriptor_file == NULL)
    {
        return 0;
    }

    // Lire chaque ligne
    while (fgets(buffer, BUFFER_SIZE, descriptor_file) != NULL)
    {
        // Vérifie si il existe bien un document_id sur la ligne
        if (sscanf(buffer, "%d", &found_id) == 1)
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
    system("ls ./TOK/*.tok > liste_emplacement_texte.txt");
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
    FILE *descriptor_file = fopen("./base_descripteur_texte.txt", "a");
    fprintf(descriptor_file, "%d ", descriptor->document_id);
    for (i = 0; i < descriptor->total_terms; i++)
    {
        fprintf(descriptor_file, "%s:%d ", descriptor->terms[i].word, descriptor->terms[i].count);
    }
    // Nombre total de termes
    // fprintf(descriptor_file, "%d;", descriptor->total_terms);
    // Taille du document en token
    fprintf(descriptor_file, "%d\n", descriptor->document_size);

    fclose(descriptor_file);

    // Sauvegarde le lien entre le descripteur et le document dans liste_base_texte
    FILE *document_list_file = fopen("./liste_base_texte.txt", "a");
    fprintf(document_list_file, "%d %s\n", descriptor->document_id, file_path);
    fclose(document_list_file);
}
