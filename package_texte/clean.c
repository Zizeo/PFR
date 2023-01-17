#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main()
{
    system("chmod +x clean.sh"); // give execute permission to the script
    DIR *d;
    struct dirent *dir;
    d = opendir("Textes");
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
                    sprintf(filename, "TOK/%s.tok", file_name);
                    FILE *file = fopen(filename, "r");
                    if (file == NULL)
                    {
                        char command[100];
                        sprintf(command, "./clean.sh Textes/%s", file_name);
                        system(command);
                    }
                    else{
                        fclose(file);
                    }
                }
            }
        }
        closedir(d);
    }
    return 0;
}
