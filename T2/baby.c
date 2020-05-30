// Trabalho 2 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531
// Otto Cruz Fernades

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "baby.h"

// cria um uma struct baby
Baby * newBaby(){
  Baby * baby;
  baby = malloc(sizeof(Baby));
  if(!baby){
      destroyBaby(&baby);
    return NULL;
  }
  baby -> cidadeMae = NULL;
  baby -> cidadeBebe = NULL;
  return baby;
}

// apaga uma struct baby
void destroyBaby(Baby ** baby){
  Baby * b = *baby;
  if(b -> cidadeMae) free(b -> cidadeMae);
  if(b -> cidadeBebe) free(b -> cidadeBebe);
  free(*baby);
  *baby = NULL;
}

// função que imprime uma struct Baby
void printBaby(Baby * baby){
  if(!baby) return;
  char estado[2];
  char nascimento[10];

  // verifica se o estadoBebe está preenchido, caso contrário imprimirá um '-'
  if (strlen(baby->estadoBebe) == 0 || baby->estadoBebe[0] == '$') {
    estado[0] = '-';
    estado[1] = '\0';
  }
  else {
    strncpy(estado, baby->estadoBebe, 2);
  }

  // verifica se o dataNascimento está preenchido, caso contrário imprimirá um '-'
  if (strlen(baby->dataNascimento) == 0 || baby->dataNascimento[0] == '$') {
    nascimento[0] = '-';
    nascimento[1] = '\0';
  }
  else {
    strncpy(nascimento, baby->dataNascimento, 10);
  }

  // verifica se o cidadeBebe está preenchido, caso contrário imprimirá um '-'
  if (strlen(baby->cidadeBebe) == 0 || baby->cidadeBebe[0] == '$') {
    if (baby->sexoBebe[0] == '0')
      printf("Nasceu em -/%.2s, em %s, um bebê de sexo IGNORADO.\n",
        estado, nascimento);
    if (baby->sexoBebe[0] == '1')
      printf("Nasceu em -/%.2s, em %s, um bebê de sexo MASCULINO.\n",
        estado, nascimento);
    if (baby->sexoBebe[0] == '2')
      printf("Nasceu em -/%.2s, em %s, um bebê de sexo FEMININO.\n",
        estado, nascimento);
  }

  else {
    if (baby->sexoBebe[0] == '0')
      printf("Nasceu em %s/%.2s, em %s, um bebê de sexo IGNORADO.\n",
        baby->cidadeBebe, estado, nascimento);
    if (baby->sexoBebe[0] == '1')
      printf("Nasceu em %s/%.2s, em %s, um bebê de sexo MASCULINO.\n",
        baby->cidadeBebe, estado, nascimento);
    if (baby->sexoBebe[0] == '2')
      printf("Nasceu em %s/%.2s, em %s, um bebê de sexo FEMININO.\n",
        baby->cidadeBebe, estado, nascimento);
  }
}

void printBabyFull(Baby * b){
  printf("\ncidadeMae: %s\n", b->cidadeMae);
  printf("cidadeBebe: %s\n", b->cidadeBebe);
  printf("idNascimento: %d\n", b->idNascimento);
  printf("idadeMae: %d\n", b->idadeMae);
  printf("dataNascimento: %.10s\n", b->dataNascimento);
  printf("sexoBebe: %.1s\n", b->sexoBebe);
  printf("estadoMae: %.2s\n", b->estadoMae);
  printf("estadoBebe: %.2s\n", b->estadoBebe);
}

// Le uma linha inteira do .csv e coloca cada informação em um lugar da struct Baby criada
Baby * readCsvRow(FILE * fp){
  Baby * baby = newBaby();
  
  baby->cidadeMae = csvNextLine(fp);
  
  baby->cidadeBebe = csvNextLine(fp);
  baby->idNascimento = csvNextInt(fp);
  baby->idadeMae = csvNextInt(fp);

  char * line = csvNextLine(fp);
  strncpy(baby -> dataNascimento, line, 10);
  free(line);

  line = csvNextLine(fp);
  strncpy(baby -> sexoBebe, line, 1);
  free(line);

  line = csvNextLine(fp);
  strncpy(baby -> estadoMae, line, 2);
  free(line);

  // usa a função csvUntilNextLine pois o final do estadoBebe é marcado por um \n, e não por uma vírgula
  line = csvUntilNextLine(fp);
  strncpy(baby -> estadoBebe, line, 2);
  free(line);

  return baby;
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