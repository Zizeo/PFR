text_descriptor_gen.out: text_descriptor_gen.o text_main.o
	gcc -o text_descriptor_gen.out text_descriptor_gen.o indexerText.o

text_descriptor_gen.o: text_descriptor_gen.c 
	gcc -c text_descriptor_gen.c -o text_descriptor_gen.o


clean: 
	rm -rf *.o