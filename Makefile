toutIndexer.out: toutIndexer.o test_index.o 
	gcc -o toutIndexer.out toutIndexer.o test_index.o Indexer.o comparaison_image.o  -lm

indexer.o: Indexer.c Indexer.h
	gcc -c -g Indexer.c -o Indexer.o -W -Wall -ansi -pedantic -std=c99

toutIndexer.o: toutIndexer.c toutIndexer.h
	gcc -c -g toutIndexer.c -o toutIndexer.o -W -Wall -ansi -pedantic -std=c99

comparaison_image: comparaison_image.c comparaison_image.h
	gcc -c -g comparaison_image.c -o comparaison_image.o -W -Wall -ansi -pedantic -std=99

test_index.o: test_index.c toutIndexer.h
	gcc -o test_index.o -c -g test_index.c -W -Wall -ansi -pedantic -std=c99
	

clean: 
	rm -rf *.o

interface: interface.c ./package_image/toutIndexer.c ./package_image/comparaison_image.c
	gcc -c -g interface.c
	gcc -c -g ./package_image/toutIndexer.c
	gcc -c -g ./package_image/comparaison_image.c
	gcc -o interface.out interface.o toutIndexer.o comparaison_image.o
	./interface.out
	


test_toutindex : Indexer.c Indexer.h toutIndexer.c toutIndexer.h test_index.c comparaison_image.c comparaison_image.h
	gcc -c -g Indexer.c
	gcc -c -g toutIndexer.c
	gcc -c -g test_index.c
	gcc -c -g comparaison_image.c
	gcc -o test_index.out Indexer.o toutIndexer.o test_index.o comparaison_image.o
	./test_index.out











