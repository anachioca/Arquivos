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
    baby *a;

    long position = ftell(fp); // Salva posição atual
    fseek(fp, 0, SEEK_END); // vai até o fim do arquivo
    long end = ftell(fp); // salva a posição atual (fim do arquivo)
    fseek(fp, position, SEEK_SET); // volta para a posição inicial
    
    //enquanto posição atual não for o fim do arquivo
    //lê linha do arquivo .csv e escreve no .bin
    //após fazer isso, apaga as informações 
    while (ftell(fp) != end){ 
        a = readCsvRow(fp);
        WriteReg(fpb, a); 
        destroyBaby(&a);
    }

    setStatus(fpb);
    fclose(fpb);
    fclose(fp);

    binarioNaTela(arquivoGerado);
  }

  if (opt == 2){
    scanf("%s", arquivoGerado);
    baby *b = newBaby();
    FILE *fpb;
    
    // checa se o arquivo a ser aberto existe
    //caso não seja possível abri-lo, imprime mensagem de erro
    if (fopen(arquivoGerado, "rb") == NULL){
      printf("Falha no processamento do arquivo.");
      return 0;
    }

    fpb = fopen(arquivoGerado, "rb");

    //lê as informações do cabeçalho
    header *h = malloc(1*sizeof(header));
    readHeader(fpb, h);
    int RRN = h->RRNproxRegistro;

    //caso o status seja 0, imprime mensagem de erro
    if(h->status[0] == '0'){
      printf("Falha no processamento do arquivo.");
      return 0;
    }

    //lê os registros e guarda em uma struct baby, imprimindo-a
    for (int i = 0; i < RRN; i++){
      readReg(fpb, b, i);
      printBaby(b);
    }

    destroyBaby(&b);
    destroyHeader(&h);
    fclose(fpb);
  }

  return 0;
}












