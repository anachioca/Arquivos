// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531
// Otto Cruz Fernandes || NUSP: 11275130

#ifndef binManager_H
#define binManager_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "baby.h"

#define bool int
#define TRUE 1
#define FALSE 0
#define FAIL -1
#define SUCCESS 2

typedef struct head{
    char status[1];
    int RRNproxRegistro;
    int numeroRegistrosInseridos;
    int numeroRegistrosRemovidos;
    int numeroRegistrosAtualizados;
} Header;

Header * initHeader();

void writeHeader(Header * header, FILE *fp);

void printHeader(Header * header);

void closeHeaderEBinario(Header ** header, FILE ** binario);

void destroyHeader(Header ** header);

void writeTrash(FILE *fp, int qt);

void updateHeader(Header * header, int opt);

void setStatusConsistente(Header * header);

void setStatusInconsistente(Header * header, FILE * binario);

int getRRN(Header * header);

int atualizaRegistros(FILE * FP, int RRN, Header * h);

void writeRegistros(Header * header, FILE *fp, Baby * baby, int RRN);

void readHeader(FILE *fp, Header * header);

Baby * readInputBaby();

Baby * readRegistros(FILE *fp, int RRN);

void removeRegistro(Header * header, FILE * binario, int rrn);

void binarioNaTela(char *nomeArquivoBinario);

void trim(char * string);

void scan_quote_string(char * string);

#endif