#include "baby.h"

int main(){

  FILE *fp;
  fp = fopen("SCC0215012020arquivoCSV.csv", "r");
  baby *b = newBaby();
  baby *c = newBaby();
  baby *d = newBaby();
  char *string = malloc(50*sizeof(char));
  string = readCsvHeader(fp);

  b = readCsvRow(fp);
  c = readCsvRow(fp);
  d = readCsvRow(fp);
  printf("\nBaby B:\n");
  printBaby(b);
  printf("\nBaby C:\n");
  printBaby(c);
  printf("\nBaby D:\n");
  printBaby(d);

  return 0;
}