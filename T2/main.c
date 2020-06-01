// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531

#include "baby.h"
#include "binManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define TRUE 1
#define FALSE 0

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

int getIndiceDoCampo(char * nomeDoCampo){
  char * campos[] = {"cidadeMae", "cidadeBebe", "idNascimento", "idadeMae",
    "dataNascimento", "sexoBebe", "estadoMae", "estadoBebe"};

  for(int i = 0; i < 8; i++){
    if(strcmp(nomeDoCampo, campos[i]) == 0)
      return i;
  }
  printf("Erro!\n");
  return -1;
}

char * getCampoStringDoBaby(Baby * baby, int i){
  switch(i){
    case 0:
      return baby -> cidadeMae;
    case 1:
      return baby -> cidadeBebe;
    case 4:
      return baby -> dataNascimento;
    case 5:
      return baby -> sexoBebe;
    case 6:
      return baby -> estadoMae;
    case 7:
      return baby -> estadoBebe;

    default:
      return NULL;
  }
}

int getCampoIntDoBaby(Baby * baby, int i){
  switch(i){
    case 2:
      return baby -> idNascimento;
    case 3:
      return baby -> idadeMae;
  }
}

bool babyEstaNoFiltro(Baby * baby, char filtros[8][20]){
  for(int i = 0; i < 8; i++){
    if(filtros[i][0] != '\0'){
      if(i == 2 || i == 3){
        if(getCampoIntDoBaby(baby, i) != atoi(filtros[i]))
          return FALSE;
      }
      else if(strcmp(getCampoStringDoBaby(baby, i), filtros[i]) != 0)
        return FALSE;
    }
  }

  return TRUE;
}

void pesquisaBin(){
  char arquivoGerado[128];
  scanf("%s", arquivoGerado);

  int m;
  scanf("%d", &m);

  char filtros[8][20]; // cada linha dessa matriz representa o filtro para um campo
  char nomeDoCampo[128];

  for(int i = 0; i < 8; i++){
    filtros[i][0] = '\0'; // para campos não selecionados, o vetor terá '\0'
  }

  for(int i = 0; i < m; i++){
    scanf("%s", nomeDoCampo);
    scan_quote_string(filtros[getIndiceDoCampo(nomeDoCampo)]);
  }

  Baby * baby;
  FILE * binario;

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

  bool encontrouResultado = FALSE;

  //lê os registros e guarda em uma struct baby, imprimindo-a
  for (int i = 0; i < RRN; i++){
    baby = readRegistros(binario, i);
    if(babyEstaNoFiltro(baby, filtros)){
      printBaby(baby);
      encontrouResultado = TRUE;
    }
    destroyBaby(&baby);
  }

  // caso nenhum registro tenha sido encontrado
  if(!encontrouResultado)
    printf("Registro Inexistente.");

  destroyHeader(&header);
  fclose(binario);
}

int main(){

  int opcao;

  scanf("%d", &opcao);

  switch(opcao){
    case 1:
      leCsv();
      break;

    case 2: 
      imprimeBin();
      break;

    case 3:
      pesquisaBin();
      break;

  }
  

  return 0;
}