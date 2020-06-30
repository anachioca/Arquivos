#ifndef INDICE_MANAGER_H
#define INDICE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "baby.h"

#define MODO_DE_ABERTURA char *
#define ARQUIVO_EXISTENTE "rb+"
#define ARQUIVO_NOVO "wb+"

typedef struct Indice_ Indice;

Indice * initIndice(char * nomeDoBinario, MODO_DE_ABERTURA modo);

void closeIndice(Indice ** indice);

char getStatusIndice(Indice * indice);

int getNoRaiz(Indice * indice);

void inserir(Indice * indice, int rrn, int chave);

int pesquisaIndice(Indice * indice, int chave, int * count);

#endif