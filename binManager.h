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
    void destroyHeader(header ** h);
    void WriteTrash(FILE *fp, int qt);
    void UpdateHeader(FILE *fp, int opt);
    void setStatus(FILE *fp);
    void resetStatus(FILE *fp);
    int getRRN(FILE *fp);
    void WriteReg(FILE *fp, baby *b);
    void readHeader(FILE *fp, header *h);
    void readReg(FILE *fp, baby *b, int RRN);
    void binarioNaTela(char *nomeArquivoBinario);
    void trim(char *str);
    void scan_quote_string(char *str);

#endif