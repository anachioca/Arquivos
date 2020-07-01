#ifndef INDICE_MANAGER_H
#define INDICE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "baby.h"

#define MODO_DE_ABERTURA char *
#define ARQUIVO_EXISTENTE "rb+"
#define ARQUIVO_NOVO "wb+"

typedef struct Indice_ Indice;
typedef struct Pagina_ Pagina;

Indice * initIndice(char * nomeDoBinario, MODO_DE_ABERTURA modo);

void printHeaderIndice(Indice * indice);

void closeIndice(Indice ** indice);

void destroyPagina(Pagina ** pagina);

char getStatusIndice(Indice * indice);

int getNoRaiz(Indice * indice);

int getProxRRN(Indice * indice);

void printPagina(Pagina * pagina);

Pagina * carregaPagina(Indice * indice, int rrn);

void inserir(Indice * indice, int rrn, int chave);

int pesquisaIndice(Indice * indice, int chave, int * count);

#endif