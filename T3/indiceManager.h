#ifndef INDICE_MANAGER_H
#define INDICE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>

#define MODO_DE_ABERTURA char *
#define ARQUIVO_EXISTENTE "rb+"
#define ARQUIVO_NOVO "wb+"

typedef struct Indice_ Indice;

Indice * initIndice(char * nomeDoBinario, MODO_DE_ABERTURA modo);

void closeIndice(Indice ** indice);

#endif