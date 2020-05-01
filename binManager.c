#include <stdlib.h>
#include <stdio.h>
#include "binManager.h"
#include "baby.h"

#define reg_size 128

void InitHeader(FILE *fp){
	header *h = malloc(1*sizeof(header));
	char *stat = malloc(1*sizeof(char));
  	stat = "0";

	// escreve no header os valores iniciais
  	strncpy(h->status, stat, 1);
  	h->RRNproxRegistro = 0;
  	h->numeroRegistrosInseridos = 0;
	h->numeroRegistrosRemovidos = 0;
	h->numeroRegistrosAtualizados = 0;

    fwrite(&(h->status), sizeof(char), 1, fp);
    fwrite(&(h->RRNproxRegistro), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosInseridos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosRemovidos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosAtualizados), sizeof(int), 1, fp);
    
	free(h);
    WriteTrash(fp, 111);
}

void destroyHeader(header ** h){
  header * head = *h;
  free(*h);
  *h = NULL;
}

void WriteTrash(FILE *fp, int qt){
	char trash = '$';
    for(int i = 0; i < qt; i++) // preenche espaço que sobra
        fwrite(&trash, 1, 1, fp);
}

// opt = 1 se um registro foi inserido
// opt = 0 se um registro foi removido
// opt = 2 se um registro foi alterado
void UpdateHeader(FILE *fp, int opt){
	long position = ftell(fp);
  	fseek(fp, 1, SEEK_SET);
	header *h = malloc(1*sizeof(header));

	fread(&(h->RRNproxRegistro), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosInseridos), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosRemovidos), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosAtualizados), sizeof(int), 1, fp);
  	
	if (opt == 1){
		h->RRNproxRegistro++;
		h->numeroRegistrosInseridos++;
	} 

	if (opt == 0){
		h->numeroRegistrosRemovidos++;
	}

	if (opt == 2){
		h->numeroRegistrosAtualizados++;
	}

	fseek(fp, 1, SEEK_SET);
    fwrite(&(h->RRNproxRegistro), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosInseridos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosRemovidos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosAtualizados), sizeof(int), 1, fp);

	free(h);

	fseek(fp, position, SEEK_SET);
}

int getRRN(FILE *fp){
	int RRN;
	fseek(fp, 1, SEEK_SET);
	fread(&RRN, sizeof(int), 1, fp);
	return RRN+1;
}

void WriteReg(FILE *fp, baby *b){

	int RRN = getRRN(fp);
	fseek(fp, RRN*reg_size, SEEK_SET);

	int a = strlen(b->cidadeMae);
	int c = strlen(b->cidadeBebe);
	int zero = 0;
	char zero_char = '0';

	if (!strcmp(b->cidadeMae, "$$$$$$$$$$")) fwrite(&zero, sizeof(int), 1, fp);
	else  fwrite(&a, sizeof(int), 1, fp);
	if (!strcmp(b->cidadeBebe, "$$$$$$$$$$")) fwrite(&zero, sizeof(int), 1, fp);
	else fwrite(&c, sizeof(int), 1, fp);

	fwrite(b->cidadeMae, sizeof(char), a, fp);
	fwrite(b->cidadeBebe, sizeof(char), c, fp);
	int lixo =  105 - a - c - 8;
	WriteTrash(fp, lixo);

	if (b->idNascimento == -1) WriteTrash(fp, 4);
	else if (b->idNascimento != -1) fwrite(&b->idNascimento, sizeof(int), 1, fp);
	
	if (b->idadeMae == -1) WriteTrash(fp, 4);
	else if (b->idadeMae != -1) fwrite(&b->idadeMae, sizeof(int), 1, fp);
	
	fwrite(&b->dataNascimento, sizeof(char), 10, fp);

	if (b->sexoBebe[0] == '$') fwrite(&zero_char, sizeof(char), 1, fp);
	else fwrite(&b->sexoBebe, sizeof(char), 1, fp);

	fwrite(&b->estadoMae, sizeof(char), 2, fp);
	fwrite(&b->estadoBebe, sizeof(char), 2, fp);

	UpdateHeader(fp, 1);
}

void readHeader(FILE *fp, header *h){
	fseek(fp, 0, SEEK_SET);
	fread(&(h->status), sizeof(char), 1, fp);
	fread(&(h->RRNproxRegistro), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosInseridos), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosRemovidos), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosAtualizados), sizeof(int), 1, fp);
}

void readReg(FILE *fp, baby *b, int RRN){
	int byteoffset = (RRN+1)*reg_size;
	fseek(fp, byteoffset, SEEK_SET);

	int a, c; //size of cidadeMae and cidadeBebe

	fread(&(a), sizeof(int), 1, fp);
	fread(&(c), sizeof(int), 1, fp);
	b->cidadeMae = malloc((a+1)*sizeof(char));
	b->cidadeBebe = malloc((c+1)*sizeof(char));

	fread(b->cidadeMae, sizeof(char), a, fp);
	fread(b->cidadeBebe, sizeof(char), c, fp);

	strcat(b->cidadeMae, "\0");
	strcat(b->cidadeMae, "\0");

	fseek(fp, byteoffset+105, SEEK_SET);

	fread(&(b->idNascimento), sizeof(int), 1, fp);
	fread(&(b->idadeMae), sizeof(int), 1, fp);
	fread(b->dataNascimento, sizeof(char), 10, fp);
	fread(b->sexoBebe, sizeof(char), 1, fp);
	fread(b->estadoMae, sizeof(char), 2, fp);
	fread(b->estadoBebe, sizeof(char), 2, fp);

}


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>



/*
* Abaixo seguem funções que fazem a escrita do binário em "stdout" (tela) pra poder ser comparado no run.codes.
*
* Funciona assim: você faz tudo o que tiver que fazer na funcionalidade no arquivo em disco, assim como ensinado nas aulas da disciplina.
* Ao fim da funcionalidade, use a função "binarioNaTela" e a função já cuida de tudo para você. É só chamar a função.
*
* Note que ao usar a binarioNaTela, o fclose no arquivo binário já deve ter sido feito anteriormente. Você passa o nome do arquivo binário ("arquivoTrabX.bin") pra ela e ela vai ler tudo e escrever na tela.
*
* Você pode colocar isso num módulo .h separado, ou incluir as funções no próprio código .c: como preferir.
* VOCÊ NÃO PRECISA ENTENDER ESSAS FUNÇÕES. É só usar elas da forma certa depois de acabar a funcionalidade.
* NÃO MODIFIQUE ESSAS FUNÇÕES. Da forma como elas estão aqui, já estão funcionando para o propósito delas.
*
* Tá tudo testado e funcionando, mas qualquer dúvida acerca dessas funções, falar com o monitor Matheus (mcarvalhor@usp.br).
*/

// Abaixo vai em algum .c


void binarioNaTela(char *nomeArquivoBinario) {

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}




void trim(char *str) {

	/*
	*	Essa função arruma uma string de entrada "str".
	*	Manda pra ela uma string que tem '\r' e ela retorna sem.
	*	Ela remove do início e do fim da string todo tipo de espaçamento (\r, \n, \t, espaço, ...).
	*	Por exemplo:
	*
	*	char minhaString[] = "    \t TESTE  DE STRING COM BARRA R     \t  \r\n ";
	*	trim(minhaString);
	*	printf("[%s]", minhaString); // vai imprimir "[TESTE  DE STRING COM BARRA R]"
	*
	*/

	size_t len;
	char *p;

	// remove espaçamentos do fim
	for(len = strlen(str); len > 0 && isspace(str[len - 1]); len--);
	str[len] = '\0';
	
	// remove espaçamentos do começo
	for(p = str; *p != '\0' && isspace(*p); p++);
	len = strlen(p);
	
	memmove(str, p, sizeof(char) * (len + 1));
}




void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

