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

  char teste[5] = "$";

  if (strcmp((Baby->cidadeMae), teste)) printf("Mother city: %s\n", Baby->cidadeMae);
  else printf("Mother city: INDEFINIDO\n");

  if (strcmp((Baby->cidadeBebe), teste)) printf("Baby city: %s\n", Baby->cidadeBebe);
  else printf("Baby city: INDEFINIDO\n");
  
  if (Baby->idNascimento != 0) printf("Birth Id: %d\n", Baby->idNascimento);
  else printf("Birth Id: INDEFINIDO\n");

  if(Baby->idadeMae != 0) printf("Mother age: %d\n", Baby->idadeMae);
  else printf("Mother age: INDEFINIDO\n");
 
  if (strcmp((Baby->dataNascimento), teste)) printf("Birth date: %.10s\n", Baby->dataNascimento);
  else printf("Birth date: INDEFINIDO\n");
  
  printf("Baby Sex: %d\n", Baby->sexoBebe);

  if (strcmp((Baby->estadoMae), teste)) printf("Mother state: %.2s\n", Baby->estadoMae);
  else printf("Mother State: INDEFINIDO\n");
  
  if (strcmp((Baby->estadoBebe), teste)) printf("Baby state: %.2s\n", Baby->estadoBebe);
  else printf("Baby State: INDEFINIDO\n");
  
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

  return Baby;
}

// baby * readCsvRow(FILE * fp){
//   baby * Baby = newBaby();
//   char *none = malloc(4*sizeof(char));

//   char * line = csvNextLine(fp);
//   if (strlen(line) == 0) Baby->cidadeMae = none;
//   else if (strlen(line) > 0) Baby->cidadeMae = line;
//   free(line);

//   line = csvNextLine(fp);
//   if (strlen(line) == 0) Baby->cidadeBebe = none;
//   else if (strlen(line) > 0) Baby->cidadeBebe = line;
//   free(line);

//   int aux = csvNextInt(fp);
//   if (aux == 0) Baby->idNascimento = -1;
//   else if (aux > 0) Baby->idNascimento = aux;
//   aux = 0;

//   aux = csvNextInt(fp);
//   if (aux == 0) Baby->idadeMae = -1;
//   else if (aux > 0) Baby->idadeMae = aux;
//   aux = 0;

//   line = csvNextLine(fp);
//   if (strlen(line) == 0) strncpy(Baby -> dataNascimento, none, 10);
//   else if (strlen(line) > 0) strncpy(Baby -> dataNascimento, line, 10);
//   free(line);

//   aux = csvNextInt(fp);
//   if (aux == 0) Baby->sexoBebe = -1;
//   else if (aux > 0) Baby->sexoBebe = aux;
//   aux = 0;

//   line = csvNextLine(fp);
//   if (strlen(line) == 0) strncpy(Baby -> estadoMae, none, 10);
//   else if (strlen(line) > 0) strncpy(Baby -> estadoMae, line, 10);
//   free(line);

//   line = csvUntilNextLine(fp);
//   if (strlen(line) == 0) strncpy(Baby -> estadoBebe, none, 10);
//   else if (strlen(line) > 0) strncpy(Baby -> estadoBebe, line, 10);
//   free(line);

//   return Baby;
// }

char * readCsvHeader(FILE * fp){ //funcionando

  char string[256] = "$";
  char enter[2];
  fscanf(fp, "%[^\n]", string);
  fread(enter, sizeof(char), 1, fp);
  int size = strlen(string);
  char *str = malloc((size+1)*sizeof(char));
  strncpy(str, string, size);
  return str;
}

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

char * csvNextLine(FILE * fp){ //funcionando
  char string[256] = "$";
  char virgula[1];
  fscanf(fp, "%[^,]", string);
  fread(virgula, sizeof(char), 1, fp);

  int size = strlen(string);
  char *str = malloc((size+1)*sizeof(char));
  
  strncpy(str, string, size);
  return str;
}


int csvNextInt(FILE * fp){
  char string[256] = "$";
  fscanf(fp, "%[^,]", string);
  char virgula[1];
  fread(virgula, sizeof(char), 1, fp);
  int num = atoi(string);
  return num;
}