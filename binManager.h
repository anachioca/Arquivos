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

    void InitHeader(FILE *fp);
    void WriteTrash(FILE *fp, int qt);
    void UpdateHeader(FILE *fp, int opt);
    int getRRN(FILE *fp);
    void WriteReg(FILE *fp, baby *b);
    void binarioNaTela(char *nomeArquivoBinario);

#endif