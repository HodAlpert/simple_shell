all: task3.o LineParser.o
	gcc -g -Wall -o myshell LineParser.o task3.o
LineParser.o: LineParser.c LineParser.h
	gcc -g -Wall -c -o LineParser.o LineParser.c
task3.o: task3.c LineParser.h
	gcc -g -Wall -c -o task3.o task3.c
.PHONY: clean
clean:
	rm -f *.o myshell