all: prog

run: prog
	./prog 
	
valgrind: prog
	gcc -o prog -g -Wall -ggdb3 *.o -g
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./prog < "Casos Abertos"/2.in

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
	rm -rf *.o prog trab.zip

zip:
	zip trab.zip *.c *.h Makefile

