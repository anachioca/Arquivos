all: prog

run: prog
	./prog <"Casos Abertos"/5.in
	
valgrind: prog
	gcc -o prog -g -Wall -ggdb3 *.o -g
	valgrind --leak-check=full --show-leak-kinds=all ./prog <"Casos Abertos"/9.in

prog: baby.o binManager.o indiceManager.o main.o
	gcc -o prog -g -Wall -ggdb3 *.o -g

main.o: main.c
	gcc -c main.c

binManager.o: binManager.c
	gcc -c binManager.c
	
indiceManager.o: indiceManager.c
	gcc -c indiceManager.c

baby.o: baby.c
	gcc -c baby.c

clean: 
	rm -rf *.o prog

zip:
	zip trab.zip *.c *.h Makefile

