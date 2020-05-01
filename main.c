#include "baby.h"
#include "binManager.h"
#include <stdio.h>
#include <stdlib.h>

int main(){

  //abrindo os arquivos .csv e .bin
  FILE *fp, *fpb, *fpb2;
  fp = fopen("arq02.csv", "r");
  fpb = fopen("registro_bebes.bin", "w+b");
 
  readCsvHeader(fp);
  InitHeader(fpb);
  baby *a = newBaby();
  baby *b = newBaby();

  //lendo do arquivo .csv, guardando em uma struct baby e escrevendo no .bin
  for (int i = 0; i < 501; i++){
    a = readCsvRow(fp);
    WriteReg(fpb, a);
  }

  //fecha e abre o .bin novamente, para poder ler seu conteúdo
  fclose(fpb);
  fpb2 = fopen("registro_bebes.bin", "rb");

  //lê as infos do cabeçalho
  header *h = malloc(1*sizeof(header));
  readHeader(fpb2, h);
  int RRN = h->RRNproxRegistro;

  //lê os registros e guarda em uma struct baby, imprimindo-a
  for (int i = 0; i < RRN; i++){
    readReg(fpb2, b, i);
    //printBaby(b);
  }

  destroyBaby(&a);
  destroyBaby(&b);
  destroyHeader(&h);
  fclose(fp);
  fclose(fpb2);

  binarioNaTela("registro_bebes.bin");

  return 0;
}












