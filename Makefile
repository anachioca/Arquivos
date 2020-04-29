all: prog

run: prog
	./prog
	
prog: baby.o binManager.o main.o
	gcc -o prog baby.o binManager.o main.o -g

main.o: main.c
	gcc -c main.c

binManager.o: binManager.c
	gcc -c binManager.c

baby.o: baby.c
	gcc -c baby.c

clean: 
	rm -rf *o prog