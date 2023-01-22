#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 50
#define MAX_FILENAME_LENGTH 100
#define MAX_LINE_LENGTH 200

typedef struct
{
    char word[MAX_WORD_LENGTH];
    int count;
} Token;

typedef struct
{
    int id;
    int total_terms;
    Token terms[MAX_WORDS];
} Descriptor;

double compare_descriptors(Descriptor *descriptor1, Descriptor *descriptor2)
{
    int total_common_terms = 0;
    for (int i = 0; i < descriptor1->total_terms; i++)
    {
        for (int j = 0; j < descriptor2->total_terms; j++)
        {
            if (strcmp(descriptor1->terms[i].word, descriptor2->terms[j].word) == 0)
            {
                total_common_terms += descriptor1->terms[i].count * descriptor2->terms[j].count;
                break;
            }
        }
    }
    return total_common_terms / (sqrt(descriptor1->total_terms) * sqrt(descriptor2->total_terms));
}

int main()
{
    // Load descriptor base
    FILE *descriptor_file = fopen("./package_texte/base_descripteur_texte.txt", "r");
    int descriptor_nb;
    Descriptor *descriptors = parse_base_descriptor(descriptor_file, &descriptor_nb);
    // Compare the text to all others in the list
    char input_text_file_path[MAX_FILENAME_LENGTH];
    printf("Enter file path of the input text: ");
    scanf("%s", input_text_file_path);

    Descriptor input_descriptor;
    int input_id = -1;
    FILE *input_text_file = fopen(input_text_file_path, "r");
    if (input_text_file != NULL)
    {
        input_descriptor = generate_descriptor(input_text_file, &input_id);
        fclose(input_text_file);
    }
    else
    {
        printf("Error: input text file not found\n");
        return 1;
    }

    double max_similarity = 0;
    int most_similar_id = -1;
    for (int i = 0; i < descriptor_nb; i++)
    {
        double similarity = compare_descriptors(&input_descriptor, &descriptors[i]);
        if (similarity > max_similarity)
        {
            max_similarity = similarity;
            most_similar_id = descriptors[i].id;
        }
    }

    printf("The most similar text to the input is text %d with a similarity of %f", most_similar_id, max_similarity);

    return 0;
}
