LunarSearch.exe: toutIndexer.o comparaison.o comparaison_image.o text_descriptor_gen.o interface.o
	gcc -o LunarSearch.exe toutIndexer.o text_descriptor_gen.o interface.o comparaison.o comparaison_image.o  -lm -pedantic

toutIndexer.o: ./package_image/toutIndexer.c ./package_image/module_image.h
	gcc -c -g ./package_image/toutIndexer.c -o toutIndexer.o -lm -pedantic

comparaison_image.o: ./package_image/comparaison_image.c ./package_image/module_image.h
	gcc -c -g ./package_image/comparaison_image.c -o comparaison_image.o -lm -pedantic

comparaison.o: ./package_texte/comparaison.c
	gcc -c  ./package_texte/comparaison.c -o comparaison.o -lm -pedantic

text_descriptor_gen.o: ./package_texte/text_descriptor_gen.c 
	gcc -c ./package_texte/text_descriptor_gen.c -o text_descriptor_gen.o -lm -pedantic

interface.o: interface.c ./package_image/module_image.h ./package_texte/text_descriptor_gen.h ./package_texte/comparaison.h
	gcc -o interface.o -c interface.c -lm -pedantic
clean: 
	rm -rf *.o