// Trabalho 2 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531
// Otto Cruz Fernandes || NUSP: 11275130

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "binManager.h"
#include "baby.h"

#define reg_size 128


// inicializa uma struct Header
Header * initHeader(){
	Header * header = malloc(1*sizeof(Header));
	char stat[1];
  	stat[0] = '0';

	// escreve no Header os valores iniciais
	// inicializa o status como 0
  	strncpy(header -> status, stat, 1);
  	header -> RRNproxRegistro = 0;
  	header -> numeroRegistrosInseridos = 0;
	header -> numeroRegistrosRemovidos = 0;
	header -> numeroRegistrosAtualizados = 0;

	return header;
}

void writeHeader(Header * header, FILE * fp){
	fseek(fp, 0, SEEK_SET);

    fwrite(&(header -> status), sizeof(char), 1, fp);
    fwrite(&(header -> RRNproxRegistro), sizeof(int), 1, fp);
    fwrite(&(header -> numeroRegistrosInseridos), sizeof(int), 1, fp);
    fwrite(&(header -> numeroRegistrosRemovidos), sizeof(int), 1, fp);
    fwrite(&(header -> numeroRegistrosAtualizados), sizeof(int), 1, fp);
    
    writeTrash(fp, 111);
}

// apaga uma struct Header
void destroyHeader(Header ** head){
  Header * h = *head;
  free(*head);
  *head = NULL;
}

// escreve o caracter '$' em 'quantidade' bytes do aqruivo binário
void writeTrash(FILE *fp, int quantidade){
	char trash = '$';
    for(int i = 0; i < quantidade; i++) // preenche espaço que sobra
        fwrite(&trash, 1, 1, fp);
}

// opt = 1 se um registro foi inserido
// opt = 0 se um registro foi removido
// opt = 2 se um registro foi alterado
void updateHeader(Header * header, int opt){

	if (opt == 1){
		header-> RRNproxRegistro++;
		header-> numeroRegistrosInseridos++;
	} 

	if (opt == 0){
		header-> numeroRegistrosRemovidos++;
		header-> numeroRegistrosInseridos--;
	}

	if (opt == 2){
		header-> numeroRegistrosAtualizados++;
	}

}

// status = 1
void setStatusConsistente(Header * header){
	char stat = '1';
	strncpy(header -> status, &stat, 1);
}

// status = 0
void setStatusInconsistente(Header * header){
	char stat = '0';
	strncpy(header -> status, &stat, 1);
}

// retorna o RRNproxRegistro
int getRRN(Header * header){
	int RRN;
	RRN = header -> numeroRegistrosInseridos;
	return RRN+1;
}

// escreve um registro no arquivo binário
// recebe o registro que é uma struct baby
void writeRegistros(Header * header, FILE * fp, Baby * baby){

	//int RRN = getRRN(h);
	//fseek(fp, RRN*reg_size, SEEK_SET);

	// comprimento das strings cidadeBebe e cidadeMae
	int a = strlen(baby -> cidadeMae);
	int c = strlen(baby -> cidadeBebe);
	int zero = 0;
	char zero_char = '0';
	char strvazia = '\0';

	// se cidadeMae for uma string vazia, escreve o int 0 no binário
	if (baby -> cidadeMae[0] == '$') {
		fwrite(&zero, sizeof(int), 1, fp);
		a = 0;
	} // caso contrário escreve o comprimento da string
	else  fwrite(&a, sizeof(int), 1, fp);

	// se cidadeBebe for uma string vazia, escreve o int 0 no binário
	if (baby -> cidadeBebe[0] == '$') {
		fwrite(&zero, sizeof(int), 1, fp);
		c = 0;
	} // caso contrário escreve o comprimento da string
	else fwrite(&c, sizeof(int), 1, fp);

	//escreve a cidadeMae e cidadeBebe no binário
	fwrite(baby -> cidadeMae, sizeof(char), a, fp);
	fwrite(baby -> cidadeBebe, sizeof(char), c, fp);

	// escreve '$', ou seja, lixo, nos bytes que restaram, até o 105 byte do registro (espaço reservado para os campos variáveis)
	int lixo =  105 - a - c - 8;
	writeTrash(fp, lixo);

	//escreve no binário o idNascimento e a idadeMae
	fwrite(&baby -> idNascimento, sizeof(int), 1, fp);
	fwrite(&baby -> idadeMae, sizeof(int), 1, fp);
	
	// se cidadeBebe for uma string vazia, escreve \0 no binário e preenche com $
	if (baby -> dataNascimento[0] == '$'){
		fwrite(&strvazia, sizeof(char), 1, fp);
		writeTrash(fp, 9);
	} // caso contrário escreve a string dataNascimento
	else if (baby -> dataNascimento[0] != '$') fwrite(&baby -> dataNascimento, sizeof(char), 10, fp);

	// se sexoBebe for uma string vazia, escreve o char '0', sexo ignorado
	if (baby -> sexoBebe[0] == '$') fwrite(&zero_char, sizeof(char), 1, fp);
	// caso contrário escreve a string sexoBebe
	else fwrite(&baby -> sexoBebe, sizeof(char), 1, fp);

	// se estadoMae for uma string vazia escreve '/0' e preenche com $ 
	if (baby -> estadoMae[0] == '$'){
		fwrite(&strvazia, sizeof(char), 1, fp);
		writeTrash(fp, 1);
	} // caso contrário escreve a string estadoMae
	else if (baby -> estadoMae[0] != '$') fwrite(&baby -> estadoMae, sizeof(char), 2, fp);
	
	// se estadoBebe for uma string vazia escreve '/0' e preenche com $ 
	if (baby -> estadoBebe[0] == '$'){
		fwrite(&strvazia, sizeof(char), 1, fp);
		writeTrash(fp, 1);
	} // caso contrário escreve a string estadoBebe
	else if (baby -> estadoBebe[0] != '$') fwrite(&baby -> estadoBebe, sizeof(char), 2, fp);

}

// lê o Header de um arquivo binário e retorna uma struct Header
void readHeader(FILE *fp, Header * header){
	fseek(fp, 0, SEEK_SET);
	fread(&(header -> status), sizeof(char), 1, fp);
	fread(&(header -> RRNproxRegistro), sizeof(int), 1, fp);
	fread(&(header -> numeroRegistrosInseridos), sizeof(int), 1, fp);
	fread(&(header -> numeroRegistrosRemovidos), sizeof(int), 1, fp);
	fread(&(header -> numeroRegistrosAtualizados), sizeof(int), 1, fp);
}

// lê um registro do arquivo binário e coloca as informações em uma struct baby
Baby * readRegistros(FILE *fp, int RRN){
	Baby * baby = newBaby();
	int byteoffset = (RRN+1)*reg_size;
	fseek(fp, byteoffset, SEEK_SET);

	//size of cidadeMae and cidadeBebe
	int a = 0;
	int c = 0;

	fread(&(a), sizeof(int), 1, fp);

	// verifica se o registro foi removido
	if (a == -1) {
		return NULL; 
	}
	
	fread(&(c), sizeof(int), 1, fp);
	baby -> cidadeMae = malloc((a+1)*sizeof(char));
	baby -> cidadeBebe = malloc((c+1)*sizeof(char));

	fread(baby -> cidadeMae, sizeof(char), a, fp);
	fread(baby -> cidadeBebe, sizeof(char), c, fp);

	baby -> cidadeMae[a] = '\0';
	baby -> cidadeBebe[c] = '\0';

	// pula os bytes reservados para os campos variáveis
	fseek(fp, byteoffset+105, SEEK_SET);

	fread(&(baby -> idNascimento), sizeof(int), 1, fp);
	fread(&(baby -> idadeMae), sizeof(int), 1, fp);
	fread(baby -> dataNascimento, sizeof(char), 10, fp);
	baby -> dataNascimento[10] = '\0';
	fread(baby -> sexoBebe, sizeof(char), 1, fp);
	baby -> sexoBebe[1] = '\0';
	fread(baby -> estadoMae, sizeof(char), 2, fp);
	baby -> estadoMae[2] = '\0';
	fread(baby -> estadoBebe, sizeof(char), 2, fp);
	baby -> estadoBebe[2] = '\0';
	return baby;
}

void removeRegistro(FILE * binario, int rrn){
	int byteoffset = (RRN+1)*reg_size;
	fseek(fp, byteoffset, SEEK_SET);

	//size of cidadeMae and cidadeBebe
	int a = -1;

	fwrite(&(a), sizeof(int), 1, fp);
}

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

