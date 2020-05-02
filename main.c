#include "baby.h"
#include "binManager.h"
#include <stdio.h>
#include <stdlib.h>

int main(){

  char arquivoGerado[100];
  char arquivoEntrada[100];
  int opt;

  scanf("%d", &opt);

  if (opt == 1){
    scanf("%s", arquivoEntrada);
    scanf("%s", arquivoGerado);
  
    FILE *fp, *fpb;
    fp = fopen(arquivoEntrada, "r");
    fpb = fopen(arquivoGerado, "w+b");

    readCsvHeader(fp); //colocar condição caso o arquivo seja vazio
    InitHeader(fpb); 
    baby *a = newBaby();

    // for (int i = 0; i < 5; i++){ // colocar pra ler até o fim do arquivo
    //   a = readCsvRow(fp);
    //   WriteReg(fpb, a);
    // }

    long position = ftell(fp); // Saving current position
    fseek(fp, 0, SEEK_END); // Going to end of file
    long end = ftell(fp); // Saving current position
    fseek(fp, position, SEEK_SET); // Voltando para posição inicial
    
    while (ftell(fp) != end){
        a = readCsvRow(fp);
        WriteReg(fpb, a); 
    }

    setStatus(fpb);
    destroyBaby(&a);
    fclose(fpb);
    fclose(fp);

    binarioNaTela(arquivoGerado);
  }

  if (opt == 2){
    scanf("%s", arquivoGerado);
    baby *b = newBaby();
    FILE *fpb;
    fpb = fopen(arquivoGerado, "rb");

    //lê as infos do cabeçalho
    header *h = malloc(1*sizeof(header));
    readHeader(fpb, h);
    int RRN = h->RRNproxRegistro;

    //lê os registros e guarda em uma struct baby, imprimindo-a
    for (int i = 0; i < RRN; i++){
      readReg(fpb, b, i);
      printBaby(b);
    }

    destroyBaby(&b);
    destroyHeader(&h);
    fclose(fpb);

    binarioNaTela(arquivoGerado);
  }

  return 0;
}












