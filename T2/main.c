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

// int findEndOfString(char *str){
//   for (int i = 1; i < 150; i++){
//     if (str[i] == '"') return i;
//   }
// }

Baby * readInputBaby(){
  Baby *b = newBaby();
  char strCidadeMae[105]; 
  char strCidadeBebe[105]; 
  char c;
  getchar();

  // Cidade Mãe:
  // se o primeiro caracter for '"', le até o próximo '"'
  // se for 'N', b->cidadeMae é NULO
  scanf("%c", &c);
  if (c == '"'){
    scanf("%[^\"]", strCidadeMae);
    b->cidadeMae = malloc(strlen(strCidadeMae) * sizeof(char));
    strncpy(b->cidadeMae, strCidadeMae, strlen(strCidadeMae));
  }

  else{
    char nulo[1] = "$";
    b->cidadeMae = malloc(1 * sizeof(char));
    strncpy(b->cidadeMae, nulo, 1);
    getchar();
    getchar();
  }

  // pula o '"' final da string cidadeMae e o espaço antes da próxima string 
  getchar();
  getchar();

  // Cidade Bebe:
  // se o primeiro caracter for '"', le até o próximo '"'
  // se for 'N', b->cidadeBebe é NULO
  scanf("%c", &c);
  if (c == '"'){
    scanf("%[^\"]", strCidadeBebe);
    getchar();
    getchar();
    b->cidadeBebe = malloc(strlen(strCidadeBebe) * sizeof(char));
    strncpy(b->cidadeBebe, strCidadeBebe, strlen(strCidadeBebe));
  }

  else{
    char nulo[1] = "$";
    b->cidadeBebe = malloc(1 * sizeof(char));
    strncpy(b->cidadeMae, nulo, 1);
    getchar();
    getchar();
  }

  getchar();

  scanf("%d %d %s %s %s %s", &b->idNascimento, &b->idadeMae, b->dataNascimento, b->sexoBebe, b->estadoMae, b->estadoBebe);
  return b;
}

void insereReg(){
    char arquivoGerado[100];
    scanf("%s", arquivoGerado);
    int n; // número de registros a serem inseridos
    scanf("%d", &n);
    FILE *fpb;
    
    // checa se o arquivo a ser aberto existe
    // caso não seja possível abri-lo, imprime mensagem de erro
    fpb = fopen(arquivoGerado, "r+b");
    if (fpb == NULL){
      printf("Falha no processamento do arquivo.");
      return;
    }

    //lê as informações do cabeçalho
    Header *h = malloc(1*sizeof(Header));
    readHeader(fpb, h);

    //caso o status seja 0, imprime mensagem de erro
    if(h->status[0] == '0'){
      printf("Falha no processamento do arquivo.");
      return;
    }

    setStatusInconsistente(h);

    for (int i = 0; i < n; i++){
      Baby *b;
      b = readInputBaby(); // lê o input do teclado e coloca em uma struct Baby
      printBaby(b);
    
      fseek(fpb, 0, SEEK_END); // vai até o fim do arquivo
      
      writeRegistros(h, fpb, b); // escreve a struct baby no arquivo binário
      updateHeader(h, 1); // incrementa no header a quantidade de arquivos inseridos e RRNproxreg
      destroyBaby(&b);
    }

    setStatusConsistente(h); // seta o status do arquivo binário
    writeHeader(h, fpb); // Escreve o cabeçalho no arquivo binário
    destroyHeader(&h);
    fclose(fpb);
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

  if (opcao == 6){
    insereReg();
  }

  return 0;
}
