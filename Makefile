all: prog

run: prog
	./prog
	
prog: baby.o main.o
	gcc -o prog baby.o main.o -g

main.o: main.c
	gcc -c main.c

baby.o: baby.c
	gcc -c baby.c

clean: 
	rm -rf *o prog