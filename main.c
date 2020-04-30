#include "baby.h"
#include "binManager.h"
#include <stdio.h>
#include <stdlib.h>

int main(){

  FILE *fp, *fpb;
  fp = fopen("SCC0215012020arquivoCSV.csv", "r");
  fpb = fopen("registro_bebes.bin", "w+b");
 
  readCsvHeader(fp);
  InitHeader(fpb);

  baby *a = newBaby();
  a = readCsvRow(fp);
  printf("\nBaby 1:\n");
  printBaby(a);
  WriteReg(fpb, a);

  baby *b = newBaby();
  b = readCsvRow(fp);
  printf("\nBaby 2:\n");
  printBaby(b);
  WriteReg(fpb, b);

  baby *c = newBaby();
  c = readCsvRow(fp);
  printf("\nBaby 2:\n");
  printBaby(c);
  WriteReg(fpb, c);

  fclose(fp);
  fclose(fpb);

  return 0;
}













// baby *b = newBaby();
  // b = readCsvRow(fp);
  // printf("\nBaby 2:\n");
  // printBaby(b);

  // baby *c = newBaby();
  // c = readCsvRow(fp);
  // printf("\nBaby 3:\n");
  // printBaby(c);

  // baby *d = newBaby();
  // d = readCsvRow(fp);
  // printf("\nBaby 4:\n");
  // printBaby(d);

  // baby *e = newBaby();
  // e = readCsvRow(fp);
  // printf("\nBaby 5:\n");
  // printBaby(e);

  // baby *f = newBaby();
  // f = readCsvRow(fp);
  // printf("\nBaby 6:\n");
  // printBaby(f);

  // baby *g = newBaby();
  // g = readCsvRow(fp);
  // printf("\nBaby 7:\n");
  // printBaby(g);

  // baby *h = newBaby();
  // h = readCsvRow(fp);
  // printf("\nBaby 8:\n");
  // printBaby(h);

  // baby *i = newBaby();
  // i = readCsvRow(fp);
  // printf("\nBaby 9:\n");
  // printBaby(i);

  // baby *j = newBaby();
  // j = readCsvRow(fp);
  // printf("\nBaby 10:\n");
  // printBaby(j);

  // baby *k = newBaby();
  // k = readCsvRow(fp);
  // printf("\nBaby 11:\n");
  // printBaby(k);
  