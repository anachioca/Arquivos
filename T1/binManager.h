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
    } header;

    // void InitHeader(FILE *fp);
    header * InitHeader();
    void writeHeader(header *h, FILE *fp);
    void destroyHeader(header ** h);
    void WriteTrash(FILE *fp, int qt);
    void UpdateHeader(header *h, int opt);
    void setStatus(header *h);
    void resetStatus(header *h);
    int getRRN(header *h);
    void WriteReg(header *h, FILE *fp, baby *b);
    void readHeader(FILE *fp, header *h);
    baby * readReg(FILE *fp);
    void binarioNaTela(char *nomeArquivoBinario);
    void trim(char *str);
    void scan_quote_string(char *str);

#endif