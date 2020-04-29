#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "baby.h"

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

  char teste[5] = "$";

  //if (strcmp((Baby->cidadeMae), teste)) 
  printf("Mother city: %s\n", Baby->cidadeMae);
  //else printf("Mother city: INDEFINIDO\n");

  //if (strcmp((Baby->cidadeBebe), teste)) 
  printf("Baby city: %s\n", Baby->cidadeBebe);
  //else printf("Baby city: INDEFINIDO\n");
  
  //if (Baby->idNascimento != 0) 
  printf("Birth Id: %d\n", Baby->idNascimento);
  //else printf("Birth Id: INDEFINIDO\n");

  //if(Baby->idadeMae != 0) 
  printf("Mother age: %d\n", Baby->idadeMae);
  //else printf("Mother age: INDEFINIDO\n");
 
  //if (strcmp((Baby->dataNascimento), teste)) 
  printf("Birth date: %.10s\n", Baby->dataNascimento);
  //else printf("Birth date: INDEFINIDO\n");
  
  printf("Baby Sex: %d\n", Baby->sexoBebe);

  //if (strcmp((Baby->estadoMae), teste)) 
  printf("Mother state: %.2s\n", Baby->estadoMae);
  //else printf("Mother State: INDEFINIDO\n");
  
  //if (strcmp((Baby->estadoBebe), teste)) 
  printf("Baby state: %.2s\n", Baby->estadoBebe);
  //else printf("Baby State: INDEFINIDO\n");
  
  printf("\n");
}

// Le uma linha inteira do .csv e coloca cada informação em um lugar da struct baby
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

  return Baby;
}


// Le a primeira linha do .csv, até chegar em um \n
char * readCsvHeader(FILE * fp){ 

  char string[256] = "$";
  char enter[2];
  fscanf(fp, "%[^\n]", string);
  fread(enter, sizeof(char), 1, fp);
  int size = strlen(string);
  char *str = malloc((size+1)*sizeof(char));
  strncpy(str, string, size);
  return str;
}

// Le a proxima string até chegar em um \n
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
char * csvNextLine(FILE * fp){
  char string[256] = "$";
  char virgula[1];
  fscanf(fp, "%[^,]", string);
  fread(virgula, sizeof(char), 1, fp);

  int size = strlen(string);
  char *str = malloc((size+1)*sizeof(char));
  
  strncpy(str, string, size);
  return str;
}

// Le o próximo int até chegar em uma vírgula
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