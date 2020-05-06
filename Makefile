all: prog

run: prog
	./prog
	
valgrind: prog
	valgrind --leak-check=full ./prog

prog: baby.o binManager.o main.o
	gcc -o prog -ggdb3 baby.o binManager.o main.o -g

main.o: main.c
	gcc -c main.c

binManager.o: binManager.c
	gcc -c binManager.c

baby.o: baby.c
	gcc -c baby.c

clean: 
	rm -rf *o prog