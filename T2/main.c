// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531
// Otto Cruz Fernandes || NUSP: 11275130

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
    printf("Falha no carregamento do arquivo.");
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
      writeRegistros(header, binario, baby, -1); 
      destroyBaby(&baby);
      updateHeader(header, 1);
  }

  closeHeaderEBinario(&header, &binario);
  fclose(csv);

  binarioNaTela(arquivoGerado);
}

bool leBinarioEHeader(FILE ** binario, Header ** header, char ** nomeDoArquivo){
  char arquivoGerado[128];
  scanf("%s", arquivoGerado);

  if(nomeDoArquivo != NULL){
    *nomeDoArquivo = malloc(128);
    strncpy(*nomeDoArquivo, arquivoGerado, 128);
  }

  *binario = fopen(arquivoGerado, "rb+");
  if (*binario == NULL){
    free(*nomeDoArquivo);
    printf("Falha no processamento do arquivo.");
    return FAIL;
  }

  //lê as informações do cabeçalho
  *header = malloc(1*sizeof(Header));
  readHeader(*binario, *header);

  if((*header) -> status[0] == '0'){
    free(*nomeDoArquivo);
    closeHeaderEBinario(header, binario);
    printf("Falha no processamento do arquivo.");
    return FAIL;
  }

  setStatusInconsistente(*header, *binario);

  return SUCCESS;
}

void imprimeBin(){
  FILE * binario;
  Header * header;
  if(leBinarioEHeader(&binario, &header, NULL) == FAIL)
    return;

  Baby * baby;
  int rrnMaximo = header->RRNproxRegistro;

  // caso não existam registros no arquivo
  if (rrnMaximo == 0){
    printf("Registro Inexistente.");
    return;
  }

  //lê os registros e guarda em uma struct baby, imprimindo-a
  for (int i = 0; i < rrnMaximo; i++){
    baby = readRegistros(binario, i);
    printBaby(baby);
    destroyBaby(&baby);
  }

  closeHeaderEBinario(&header, &binario);
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

bool babyEstaNoFiltro(Baby * baby, char filtros[8][128]){
  if(!baby) return FALSE;

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

void leRegistroEImprimeBebe(Header * header, FILE * binario, int rrn){
  Baby * baby = readRegistros(binario, rrn);
  printBaby(baby);
  destroyBaby(&baby);
}

bool pesquisaBin(Header * header, FILE * binario, 
    void (*executaNoBinario) (Header *,FILE *, int)){

  int numeroDeFiltros;
  scanf("%d", &numeroDeFiltros);

  char filtros[8][128]; // cada linha dessa matriz representa o filtro para um campo
  char nomeDoCampo[128];

  for(int i = 0; i < 8; i++)
    filtros[i][0] = '\0'; // para campos não selecionados, o vetor terá '\0'

  for(int i = 0; i < numeroDeFiltros; i++){
    scanf("%s", nomeDoCampo);
    scan_quote_string(filtros[getIndiceDoCampo(nomeDoCampo)]);
  }

  Baby * baby;
  bool babyDeuMatch = FALSE;
  bool algumBabyDeuMatch = FALSE;
  int rrnMaximo = header->RRNproxRegistro;
  
  for (int i = 0; i < rrnMaximo; i++){
    baby = readRegistros(binario, i);
    babyDeuMatch = babyEstaNoFiltro(baby, filtros);
    if(babyDeuMatch)
      executaNoBinario(header, binario, i);
    
    algumBabyDeuMatch = algumBabyDeuMatch || babyDeuMatch;
    destroyBaby(&baby);
  }

  return algumBabyDeuMatch;
}

void imprimeComFiltro(){
  FILE * binario;
  Header * header;
  if(leBinarioEHeader(&binario, &header, NULL) == FAIL)
    return;

  bool algumBabyFoiImpresso = pesquisaBin(header, binario, leRegistroEImprimeBebe);
  if(!algumBabyFoiImpresso)
    printf("Registro Inexistente.");
    
  closeHeaderEBinario(&header, &binario);
}


void pesquisaPorRRN(){
  FILE * binario;
  Header * header;
  Baby * baby;
  if(leBinarioEHeader(&binario, &header, NULL) == FAIL)
    return;

  int rrn;
  int rrnMaximo = header->RRNproxRegistro;
  scanf("%d", &rrn);

  baby = readRegistros(binario, rrn);
  // caso o rrn pedido seja válido e o bebê não esteja logicamente removido
  if(rrn <= rrnMaximo && baby != NULL)
    printBaby(baby);
  else 
    printf("Registro Inexistente.");

  destroyBaby(&baby);

  closeHeaderEBinario(&header, &binario);
}

void remocaoLogica(){
  FILE * binario;
  Header * header;
  char * nomeDoArquivo;
  if(leBinarioEHeader(&binario, &header, &nomeDoArquivo) == FAIL)
    return;
  
  // printHeader(header);

  int numeroDeRemocoes;
  scanf("%d", &numeroDeRemocoes);

  for(int i = 0; i < numeroDeRemocoes; i++)
    pesquisaBin(header, binario, removeRegistro);

  closeHeaderEBinario(&header, &binario);

  binarioNaTela(nomeDoArquivo);
  free(nomeDoArquivo);
}

void insereRegistro(){
  FILE * binario;
  Header * header;
  char * nomeDoArquivo;
  if(leBinarioEHeader(&binario, &header, &nomeDoArquivo) == FAIL)
    return;

  Baby * baby;
  int numeroDeInsercoes;
  scanf("%d", &numeroDeInsercoes);

  for (int i = 0; i < numeroDeInsercoes; i++){
    baby = readInputBaby(); // lê o input do teclado e coloca em uma struct Baby
    fseek(binario, 0, SEEK_END); // vai até o fim do arquivo
    
    writeRegistros(header, binario, baby, -1); 
    updateHeader(header, 1); // incrementa no header a quantidade de arquivos inseridos e RRNproxreg
    destroyBaby(&baby);
  }

  closeHeaderEBinario(&header, &binario);
  binarioNaTela(nomeDoArquivo);
  free(nomeDoArquivo);
}

void atualizaReg(){
  FILE * binario;
  Header * header;
  char * nomeDoArquivo;
  if(leBinarioEHeader(&binario, &header, &nomeDoArquivo) == FAIL)
    return;

  Baby * baby;
  int numeroDeAtualizacoes;
  int rrn;
  scanf("%d", &numeroDeAtualizacoes);

  for (int i = 0; i < numeroDeAtualizacoes; i++){
    scanf("%d", &rrn);
    if (rrn > header -> RRNproxRegistro) break;
    if (atualizaRegistros(binario, rrn, header) == 0) {
      updateHeader(header, 2); // incrementa no header a quantidade de arquivos atualizados
    }
  }

  closeHeaderEBinario(&header, &binario);
  binarioNaTela(nomeDoArquivo);
  free(nomeDoArquivo);
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
      imprimeComFiltro();
      break;

    case 4:
      pesquisaPorRRN();
      break;

    case 5:
      remocaoLogica();
      break;
      
    case 6:
      insereRegistro();
      break;
    
    case 7:
      atualizaReg();
      break;
  }  

  return 0;
}

