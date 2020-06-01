// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531
// Otto Cruz Fernandes || NUSP: 11275130

#ifndef baby_H
#define baby_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct info Baby;

struct info{
    char * cidadeMae; //campo de tamanho variável
    char * cidadeBebe;  //campo de tamanho variável
    int idNascimento;
    int idadeMae;
    char dataNascimento[11];
    char sexoBebe[2];
    char estadoMae[3];
    char estadoBebe[3];
};

Baby * newBaby();

void destroyBaby(Baby ** Baby);

void printBaby(Baby * Baby);

Baby * readCsvRow(FILE * fp);

void readCsvHeader(FILE * fp);

char * csvUntilNextLine(FILE * fp);

char * csvNextLine(FILE * fp);

int csvNextInt(FILE * fp);

#endif
