// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531

#ifndef baby_H
#define baby_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

    typedef struct info baby;

    struct info{
        char * cidadeMae; //campo de tamanho variável
        char * cidadeBebe;  //campo de tamanho variável
        int idNascimento;
        int idadeMae;
        char dataNascimento[10];
        char sexoBebe[1];
        char estadoMae[2];
        char estadoBebe[2];
    };

    baby * newBaby();
    void destroyBaby(baby ** Baby);
    void printBaby(baby * Baby);
    baby * readCsvRow(FILE * fp);
    void readCsvHeader(FILE * fp);
    char * csvUntilNextLine(FILE * fp);
    char * csvNextLine(FILE * fp);
    int csvNextInt(FILE * fp);

#endif
