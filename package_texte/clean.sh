#!/bin/bash

echo "This is $0"

ARG=$1

stopwords="./package_texte/stopword.txt"

#convertir le fichier d'entrée en UTF-8 et on enregistre le résultat dans un nouveau fichier .utf8
iconv -f ISO-8859-1 -t UTF-8 "$ARG" > "$ARG.utf8"

# On utilise awk pour supprimer les balises XML du fichier converti en UTF-8 
# Les mots du texte qui n'existent pas dans stopword.txt sont ensuite séparés par des espaces et enregistrés dans un nouveau fichier .tok
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

# Le nouveau fichier nettoyé .tok est déplacé dans le répertoire "package_texte/TOK" et le fichier temporaire converti en UTF-8 est supprimé.
mv "$ARG.tok" ./package_texte/TOK
rm "$ARG.utf8"

echo "This is $0"
