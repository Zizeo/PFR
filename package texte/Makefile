text_descriptor_gen.out: text_descriptor_gen.o text_main.o
	gcc -o text_descriptor_gen.out text_descriptor_gen.o text_main.o

text_descriptor_gen.o: text_descriptor_gen.c 
	gcc -c text_descriptor_gen.c -o text_descriptor_gen.o

text_main.o: text_main.c text_descriptor_gen.h
	gcc -o text_main.o -c text_main.c

clean: 
	rm -rf *.o