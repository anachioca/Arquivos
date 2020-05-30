// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531

#ifndef binManager_H
#define binManager_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "baby.h"

typedef struct head{
    char status[1];
    int RRNproxRegistro;
    int numeroRegistrosInseridos;
    int numeroRegistrosRemovidos;
    int numeroRegistrosAtualizados;
} Header;

Header * initHeader();

void writeHeader(Header * header, FILE *fp);

void printHeader(Header * h);

void destroyHeader(Header ** header);

void writeTrash(FILE *fp, int qt);

void updateHeader(Header * header, int opt);

void setStatusConsistente(Header * header);

void setStatusInconsistente(Header * header);

int getRRN(Header * header);

int atualizaRegistros(FILE * FP, int RRN, Header * h);

void writeRegistros(Header * header, FILE *fp, Baby * baby, int RRN);

void readHeader(FILE *fp, Header * header);

Baby * readRegistros(FILE *fp, int RRN);

void binarioNaTela(char *nomeArquivoBinario);

void trim(char * string);

void scan_quote_string(char * string);

#endif