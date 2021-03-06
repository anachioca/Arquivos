// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "baby.h"

// cria um uma struct baby
baby * newBaby(){
  baby * Baby;
  Baby = malloc(sizeof(baby));
  if(!Baby){
      destroyBaby(&Baby);
    return NULL;
  }
  Baby -> cidadeMae = NULL;
  Baby -> cidadeBebe = NULL;
  return Baby;
}

// apaga uma struct baby
void destroyBaby(baby ** Baby){
  baby * b = *Baby;
  if(b -> cidadeMae) free(b -> cidadeMae);
  if(b -> cidadeBebe) free(b -> cidadeBebe);
  free(*Baby);
  *Baby = NULL;
}

// função que imprime uma struct baby
void printBaby(baby * Baby){
  if(!Baby) return;
  char estado[2];
  char nascimento[10];

  // verifica se o estadoBebe está preenchido, caso contrário imprimirá um '-'
  if (strlen(Baby->estadoBebe) == 0) {
    estado[0] = '-';
    estado[1] = '\0';
  }
  else {
    strncpy(estado, Baby->estadoBebe, 2);
  }

  // verifica se o dataNascimento está preenchido, caso contrário imprimirá um '-'
  if (strlen(Baby->dataNascimento) == 0) {
    nascimento[0] = '-';
    nascimento[1] = '\0';
  }
  else {
    strncpy(nascimento, Baby->dataNascimento, 10);
  }

  // verifica se o cidadeBebe está preenchido, caso contrário imprimirá um '-'
  if (strlen(Baby->cidadeBebe) == 0) {
    if (Baby->sexoBebe[0] == '0') printf("Nasceu em -/%.2s, em %s, um bebê de sexo IGNORADO.\n", estado, nascimento);
    if (Baby->sexoBebe[0] == '1') printf("Nasceu em -/%.2s, em %s, um bebê de sexo MASCULINO.\n", estado, nascimento);
    if (Baby->sexoBebe[0] == '2') printf("Nasceu em -/%.2s, em %s, um bebê de sexo FEMININO.\n", estado, nascimento);
  }

  else {
    if (Baby->sexoBebe[0] == '0') printf("Nasceu em %s/%.2s, em %s, um bebê de sexo IGNORADO.\n", Baby->cidadeBebe, estado, nascimento);
    if (Baby->sexoBebe[0] == '1') printf("Nasceu em %s/%.2s, em %s, um bebê de sexo MASCULINO.\n", Baby->cidadeBebe, estado, nascimento);
    if (Baby->sexoBebe[0] == '2') printf("Nasceu em %s/%.2s, em %s, um bebê de sexo FEMININO.\n", Baby->cidadeBebe, estado, nascimento);
  }
}

// Le uma linha inteira do .csv e coloca cada informação em um lugar da struct baby criada
baby * readCsvRow(FILE * fp){
  baby * Baby = newBaby();
  
  Baby->cidadeMae = csvNextLine(fp);
  
  Baby->cidadeBebe = csvNextLine(fp);
  Baby->idNascimento = csvNextInt(fp);
  Baby->idadeMae = csvNextInt(fp);

  char * line = csvNextLine(fp);
  strncpy(Baby -> dataNascimento, line, 10);
  free(line);

  line = csvNextLine(fp);
  strncpy(Baby -> sexoBebe, line, 1);
  free(line);

  line = csvNextLine(fp);
  strncpy(Baby -> estadoMae, line, 2);
  free(line);

  // usa a função csvUntilNextLine pois o final do estadoBebe é marcado por um \n, e não por uma vírgula
  line = csvUntilNextLine(fp);
  strncpy(Baby -> estadoBebe, line, 2);
  free(line);

  return Baby;
}

// Le a primeira linha do .csv, até chegar em um \n
void readCsvHeader(FILE * fp){ 
  char string[256] = "$";
  char enter[2];
  fscanf(fp, "%[^\n]", string);
  fread(enter, sizeof(char), 1, fp);
}

// Le a proxima string até chegar em um \n
// caso o campo esteja vazio, a string será "$"
char * csvUntilNextLine(FILE * fp){
  char string[256] = "$";
  char virgula[1];
  fscanf(fp, "%[^\n]", string);
  fread(virgula, sizeof(char), 1, fp);

  int size = strlen(string);
  char *str = malloc((size+1)*sizeof(char));
  strncpy(str, string, size);
  return str;
}

// Le a proxima string até chegar em uma vírgula
// caso o campo esteja vazio, a string será "$"
char * csvNextLine(FILE * fp){
  char string[256] = "$";
  char virgula[1];
  fscanf(fp, "%[^,]", string);
  fread(virgula, sizeof(char), 1, fp);

  int size = strlen(string);
  char *str = malloc((size+1)*sizeof(char));

  strncpy(str, string, size+1);
  return str;
}

// Le o próximo int até chegar em uma vírgula
// caso o espaço esteja vazio, retorna -1
int csvNextInt(FILE * fp){
  char string[256] = "$";
  int num;
  fscanf(fp, "%[^,]", string);
  char virgula[1];
  fread(virgula, sizeof(char), 1, fp);
  if (!strcmp(string, "$")) num = -1;
  else num = atoi(string);
  return num;
}