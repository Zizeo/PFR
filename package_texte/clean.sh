#!/bin/bash 

#script pour creer les fichiers clean et tok avant descr.

echo "This is $0"

ARG=$1
 
array=$(cat ./stopword.txt) 
iconv -f ISO-8859-1 -t UTF-8 "$ARG" | sed 's/<\/\?[^>]\+>//g' |  tr -d '[:punct:]' | tr -s '[:space:]' | tr '[:upper:]' '[:lower:]' | tail -n +4 > "$ARG.pre"

sed 's/-/ /g' "$ARG.pre" | sed 's/\./ /g' |  tr "\n" " " > "$ARG.clean"
#cat "$ARG.clean"


#nous avons creer un ficchier clean contenant sur une meme ligne 
#le fichier sans les balises, sans ponctuation 
#et les majusccule transformés en minuscule

#ensuite on enlève les stopword contenu dans le fichier ayant le meme nom



#array=( mimer un le la pour par les ou )
for i in ${array[@]}
do
    sed -i.bak -e  "s/\<$i\>//g" "$ARG.clean" 

done >> "$ARG.tok"

#cat "$ARG.clean"
#cat "$ARG.tok"
rm "$ARG.pre" "$ARG.clean"

mv "$ARG.clean.bak" "$ARG.tok"

mv "$ARG.tok" TOK


echo "This is $0"