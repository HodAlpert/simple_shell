all: task2.o
	gcc -g -Wall -o mypipe task2.o
task2.o: task2.c
	gcc -g -Wall -c -o task2.o task2.c
.PHONY: clean
clean:
	rm -f *.o mypipe