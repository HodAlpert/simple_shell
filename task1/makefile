all: task1.o LineParser.o
	gcc -g -Wall -o myshell LineParser.o task1.o
LineParser.o: LineParser.c LineParser.h
	gcc -g -Wall -c -o LineParser.o LineParser.c
task1.o: task1.c LineParser.h
	gcc -g -Wall -c -o task1.o task1.c
.PHONY: clean
clean:
	rm -f *.o myshell