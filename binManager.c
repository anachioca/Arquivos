#include <stdlib.h>
#include <stdio.h>
#include "binManager.h"
#include "baby.h"

// #define reg_size 128

void WriteHeader(FILE *fp, header *h){
    fwrite(&(h->status), sizeof(char), 1, fp);
    fwrite(&(h->RRNproxRegistro), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosInseridos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosRemovidos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosAtualizados), sizeof(int), 1, fp);
    
    char trash = '$';
    for(int i = 0; i < 111; i++) // preenche espaço que sobra
        fwrite(&trash, 1, 1, fp);
}

// void WriteRegister(FILE *fp, baby *b){

// }

void binarioNaTela(char *nomeArquivoBinario) {

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}