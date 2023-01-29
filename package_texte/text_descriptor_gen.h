#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>

#define MAX_WORDS 10000
#define MAX_WORD_LENGTH 50
#define MAX_DOCUMENTS 1000
#define MAX_DOCUMENT_SIZE 100000
#define BUFFER_SIZE 256
#define MAX_FILE_PATH_LENGTH 1000
#define MAX_COMMAND_LENGTH 1000
#define MAX_FILE_NAME_LENGTH 100


typedef struct
{
    char word[MAX_WORD_LENGTH];
    int count;
} Term;

typedef struct
{
    int document_id;
    char file_path[MAX_WORD_LENGTH];
    Term terms[MAX_WORDS];
    int total_terms;
    int document_size;
} Descriptor;


void add_term(Descriptor *descriptor, char *word);

int file_exists(char *file_path);

void clean_xml_files();

int descriptor_exists(int document_id);

void update_liste_emplacement_texte();

void process_text(int document_id, char *file_path, char *document, Descriptor *descriptor);

void indexerText();

