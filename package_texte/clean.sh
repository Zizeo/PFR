#!/bin/bash

echo "This is $0"

ARG=$1

stopwords="./package_texte/stopword.txt"

#convertir le fichier d'entrée en UTF-8 et on enregistre le résultat dans un nouveau fichier
iconv -f ISO-8859-1 -t UTF-8 "$ARG" > "$ARG.utf8"

# On utilise awk pour supprimer les balises XML du fichier converti en UTF-8
# On lit chaque ligne du fichier de stopwords et on ajoute chaque mot à un tableau stopwords_set en minuscule
awk '{gsub(/<[^>]*>/, ""); print $0}' "$ARG.utf8" | awk -v stopwords="$stopwords" '
BEGIN {
    FS = "[^a-zA-ZÀ-ÿ0-9]+"
    while (getline < stopwords) {
        stopwords_set[tolower($1)] = 1
    }
}
{
    for (i = 1; i <= NF; i++) {
        if (!stopwords_set[tolower($i)]) {
            printf("%s ", $i)
        }
    }
    printf("\n")
}
' > "$ARG.tok"

mv "$ARG.tok" ./package_texte/TOK

rm "$ARG.utf8"

echo "This is $0"
