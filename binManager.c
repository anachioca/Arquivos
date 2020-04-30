#include <stdlib.h>
#include <stdio.h>
#include "binManager.h"
#include "baby.h"

// #define reg_size 128

void InitHeader(FILE *fp){
	header *h = malloc(1*sizeof(header));
	char *stat = malloc(1*sizeof(char));
  	stat = "0";

	// escreve no header os valores iniciais
  	strncpy(h->status, stat, 1);
  	h->RRNproxRegistro = 0;
  	h->numeroRegistrosInseridos = 0;
	h->numeroRegistrosRemovidos = 0;
	h->numeroRegistrosAtualizados = 0;

    fwrite(&(h->status), sizeof(char), 1, fp);
    fwrite(&(h->RRNproxRegistro), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosInseridos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosRemovidos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosAtualizados), sizeof(int), 1, fp);
    
	free(h);
    WriteTrash(fp, 111);
}

void WriteTrash(FILE *fp, int qt){
	char trash = '$';
    for(int i = 0; i < qt; i++) // preenche espaço que sobra
        fwrite(&trash, 1, 1, fp);
}

// opt = 1 se um registro foi inserido
// opt = 0 se um registro foi removido
// opt = 2 se um registro foi alterado
void UpdateHeader(FILE *fp, int opt){
	long position = ftell(fp);
  	fseek(fp, 1, SEEK_SET);
	header *h = malloc(1*sizeof(header));

	fread(&(h->RRNproxRegistro), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosInseridos), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosRemovidos), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosAtualizados), sizeof(int), 1, fp);
  	
	if (opt == 1){
		h->RRNproxRegistro++;
		h->numeroRegistrosInseridos++;
	} 

	if (opt == 0){
		h->numeroRegistrosRemovidos++;
	}

	if (opt == 2){
		h->numeroRegistrosAtualizados++;
	}

	fseek(fp, 1, SEEK_SET);
    fwrite(&(h->RRNproxRegistro), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosInseridos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosRemovidos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosAtualizados), sizeof(int), 1, fp);

	free(h);

	fseek(fp, position, SEEK_SET);
}

int getRRN(FILE *fp){
	int RRN;
	fseek(fp, 1, SEEK_SET);
	fread(&RRN, sizeof(int), 1, fp);
	printf("\nRRNproxReg = %d\n", RRN);
	return RRN+1;
}

void WriteReg(FILE *fp, baby *b){

	int RRN = getRRN(fp);
	fseek(fp, RRN*128, SEEK_SET);

	int a = strlen(b->cidadeMae);
	int c = strlen(b->cidadeBebe);
	fwrite(&b->idNascimento, sizeof(int), 1, fp);
	fwrite(&b->idadeMae, sizeof(int), 1, fp);
	fwrite(&b->dataNascimento, sizeof(char), 10, fp);
	fwrite(&b->sexoBebe, sizeof(char), 1, fp);
	fwrite(&b->estadoMae, sizeof(char), 2, fp);
	fwrite(&b->estadoBebe, sizeof(char), 2, fp);

	fwrite(&b->cidadeMae, sizeof(char), a, fp);
	fwrite(&b->cidadeBebe, sizeof(char), c, fp);

	printf("%d %d", a, c);
	int lixo =  105 - a - c;
	WriteTrash(fp, lixo);
	UpdateHeader(fp, 1);
}

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