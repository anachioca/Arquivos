// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531

#include "baby.h"
#include "binManager.h"
#include <stdio.h>
#include <stdlib.h>

void leCsv(){
  char arquivoEntrada[128];
  char arquivoGerado[128];

  scanf("%s", arquivoEntrada);
  scanf("%s", arquivoGerado);

  FILE * csv, * binario;

  // checa se os arquivos a serem abertos existem
  // caso não seja possível abri-los, imprime mensagem de erro
  csv = fopen(arquivoEntrada, "r");
  binario = fopen(arquivoGerado, "w+b");

  if (csv == NULL || binario == NULL){
    printf("Falha no processamento do arquivo.");
    return;
  }

  readCsvHeader(csv); //colocar condição caso o arquivo seja vazio

  Baby * baby;
  Header * header = initHeader(); // inializa o cabeçalho 
  writeHeader(header, binario); // escreve o cabeçalho no arquivo binário

  long posicao = ftell(csv); // Salva posição atual
  fseek(csv, 0, SEEK_END); // vai até o fim do arquivo
  long fimDoArquivo = ftell(csv); // salva a posição atual (fim do arquivo)
  fseek(csv, posicao, SEEK_SET); // volta para a posição inicial
  
  //enquanto posição atual não for o fim do arquivo
  //lê linha do arquivo .csv e escreve no .bin
  //após fazer isso, apaga as informações 
  while (ftell(csv) != fimDoArquivo){ 
      baby = readCsvRow(csv);
      writeRegistros(header, binario, baby); 
      destroyBaby(&baby);
      updateHeader(header, 1);
  }

  setStatusConsistente(header); // seta o status do arquivo binário
  writeHeader(header, binario); // Escreve o cabeçalho no arquivo binário
  destroyHeader(&header);
  fclose(binario);
  fclose(csv);

  binarioNaTela(arquivoGerado);
}

void imprimeBin(){
  char arquivoGerado[128];

  scanf("%s", arquivoGerado);
  Baby * baby;
  FILE * binario;
  
  // checa se o arquivo a ser aberto existe
  // caso não seja possível abri-lo, imprime mensagem de erro
  binario = fopen(arquivoGerado, "rb");
  if (binario == NULL){
    printf("Falha no processamento do arquivo.");
    return;
  }

  //lê as informações do cabeçalho
  Header * header = malloc(1*sizeof(Header));
  readHeader(binario, header);
  int RRN = header->RRNproxRegistro;

  //caso o status seja 0, imprime mensagem de erro
  if(header->status[0] == '0'){
    printf("Falha no processamento do arquivo.");
    return;
  }

  // caso não existam registros no arquivo
  if (RRN == 0){
    printf("Registro inexistente.");
    return;
  }

  //lê os registros e guarda em uma struct baby, imprimindo-a
  for (int i = 0; i < RRN; i++){
    baby = readRegistros(binario, i);
    printBaby(baby);
    destroyBaby(&baby);
  }

  destroyHeader(&header);
  fclose(binario);
}

int main(){

  int opcao;

  scanf("%d", &opcao);

  if (opcao == 1){
    leCsv();
  }

  if (opcao == 2){
    imprimeBin();
  }

  return 0;
}












