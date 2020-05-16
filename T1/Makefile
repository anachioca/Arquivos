all: prog

run: prog
	./prog
	
valgrind: prog
	valgrind --leak-check=full --show-leak-kinds=all ./prog

prog: baby.o binManager.o main.o
	gcc -o prog -g -Wall -ggdb3 baby.o binManager.o main.o -g

main.o: main.c
	gcc -c main.c

binManager.o: binManager.c
	gcc -c binManager.c

baby.o: baby.c
	gcc -c baby.c

clean: 
	rm -rf *o prog