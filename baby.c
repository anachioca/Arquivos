#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "baby.h"

struct info{
  char * cidadeMae;
  char * cidadeBebe;
  int idNascimento;
  int idadeMae;
  char dataNascimento[10];
  int sexoBebe;
  char estadoMae[2];
  char estadoBebe[2];
};

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

void destroyBaby(baby ** Baby){
  baby * b = *Baby;
  if(b -> cidadeMae) free(b -> cidadeMae);
  if(b -> cidadeBebe) free(b -> cidadeBebe);
  free(*Baby);
  b -> cidadeMae = NULL;
  b -> cidadeBebe = NULL;
  *Baby = NULL;
}

void setcidadeMae(baby *Baby, char *string){
  Baby->cidadeMae = string;
}

void setcidadeBebe(baby *Baby, char *string){
  Baby->cidadeBebe = string;
}

void setidNascimento(baby *Baby, int ID){
  Baby->idNascimento = ID;
}

void setidadeMae(baby *Baby, int idade){
  Baby->idadeMae = idade;
}

void setDataNascimento(baby *Baby, char* string){
   strncpy(Baby->dataNascimento, string, 10);
}

void setsexoBebe(baby *Baby, int sexo){
  Baby->sexoBebe = sexo;
}

void setestadoMae(baby *Baby, char* string){
  strncpy(Baby->estadoMae, string, 2);
}

void setestadoBebe(baby *Baby, char* string){
  strncpy(Baby->estadoBebe, string, 2);
}

void printBaby(baby * Baby){
  if(!Baby) return;
  printf("Mother city: %s\n", Baby->cidadeMae);
  printf("Baby city: %s\n", Baby->cidadeBebe);
  printf("Birth Id: %d\n", Baby->idNascimento);
  printf("Mother age: %d\n", Baby->idadeMae);
  printf("Birth date: %.10s\n", Baby->dataNascimento);
  printf("Baby Sex: %d\n", Baby->sexoBebe);
  printf("Mother state: %.2s\n", Baby->estadoMae);
  printf("Baby state: %.2s\n", Baby->estadoBebe);
  printf("\n");
}

baby * readCsvRow(FILE * fp){
  baby * Baby = newBaby();

  Baby->cidadeMae = csvNextLine(fp);
  Baby->cidadeBebe = csvNextLine(fp);
  Baby->idNascimento = csvNextInt(fp);
  Baby->idadeMae = csvNextInt(fp);

  char * line = csvNextLine(fp);
  strncpy(Baby -> dataNascimento, line, 10);
  free(line);

  Baby->sexoBebe = csvNextInt(fp);

  line = csvNextLine(fp);
  strncpy(Baby -> estadoMae, line, 2);
  free(line);

  line = csvUntilNextLine(fp);
  strncpy(Baby -> estadoBebe, line, 2);
  free(line);

  //char enter[2];
  //fread(enter, sizeof(char), 1, fp);

  return Baby;
}

char * readCsvHeader(FILE * fp){ //funcionando

  char *string = malloc(256*sizeof(char));
  char enter[2];
  fscanf(fp, "%[^\n]", string);
  fread(enter, sizeof(char), 1, fp);
  int size = strlen(string);
  if (size == 0) {
   printf("String vazia\n");
  }
  return string;
}

char * csvUntilNextLine(FILE * fp){
  char *string = malloc(256*sizeof(char));
  char virgula[1];
  fscanf(fp, "%[^\n]", string);
  fread(virgula, sizeof(char), 1, fp);
  int size = strlen(string);
  if (size == 0) {
   printf("String vazia\n");
  }
  return string;
}

char * csvNextLine(FILE * fp){ //funcionando
  char *string = malloc(256*sizeof(char));
  char virgula[1];
  fscanf(fp, "%[^,]", string);
  fread(virgula, sizeof(char), 1, fp);
  int size = strlen(string);
  if (size == 0) {
   printf("String vazia\n");
  }
  return string;
}


int csvNextInt(FILE * fp){
  char string[256] = {"test"};
  fscanf(fp, "%[^,]", string);
  char virgula[1];
  fread(virgula, sizeof(char), 1, fp);
  int size = strlen(string);

  if (size == 0) {
    printf("Campo vazio");
  }
  
  int num = atoi(string);
  return num;
}