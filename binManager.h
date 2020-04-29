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
    } header;

    void WriteHeader(FILE *fp, header *h);
    void binarioNaTela(char *nomeArquivoBinario);

#endif