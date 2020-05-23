// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "binManager.h"
#include "baby.h"

#define reg_size 128

// inicializa uma struct header
header *InitHeader(){
	header *h = malloc(1*sizeof(header));
	char stat[1];
  	stat[0] = '0';

	// escreve no header os valores iniciais
	// inicializa o status como 0
  	strncpy(h->status, stat, 1);
  	h->RRNproxRegistro = 0;
  	h->numeroRegistrosInseridos = 0;
	h->numeroRegistrosRemovidos = 0;
	h->numeroRegistrosAtualizados = 0;

	return h;
}

void writeHeader(header *h, FILE *fp){
	fseek(fp, 0, SEEK_SET);

    fwrite(&(h->status), sizeof(char), 1, fp);
    fwrite(&(h->RRNproxRegistro), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosInseridos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosRemovidos), sizeof(int), 1, fp);
    fwrite(&(h->numeroRegistrosAtualizados), sizeof(int), 1, fp);
    
    WriteTrash(fp, 111);
}

// apaga uma struct header
void destroyHeader(header ** head){
  header * h = *head;
  free(*head);
  *head = NULL;
}

// escreve o caracter '$' em 'qt' bytes do aqruivo binário
void WriteTrash(FILE *fp, int qt){
	char trash = '$';
    for(int i = 0; i < qt; i++) // preenche espaço que sobra
        fwrite(&trash, 1, 1, fp);
}

// opt = 1 se um registro foi inserido
// opt = 0 se um registro foi removido
// opt = 2 se um registro foi alterado
void UpdateHeader(header *h, int opt){

	if (opt == 1){
		h->RRNproxRegistro++;
		h->numeroRegistrosInseridos++;
	} 

	if (opt == 0){
		h->numeroRegistrosRemovidos++;
		h->numeroRegistrosInseridos--;
	}

	if (opt == 2){
		h->numeroRegistrosAtualizados++;
	}

}

// status = 1
void setStatus(header *h){
	char stat = '1';
	strncpy(h->status, &stat, 1);
}

// status = 0
void resetStatus(header *h){
	char stat = '0';
	strncpy(h->status, &stat, 1);
}

// retorna o RRNproxRegistro
int getRRN(header *h){
	int RRN;
	RRN = h->numeroRegistrosInseridos;
	return RRN+1;
}

// escreve um registro no arquivo binário
// recebe o registro que é uma struct baby
void WriteReg(header *h, FILE *fp, baby *b){

	//int RRN = getRRN(h);
	//fseek(fp, RRN*reg_size, SEEK_SET);

	// comprimento das strings cidadeBebe e cidadeMae
	int a = strlen(b->cidadeMae);
	int c = strlen(b->cidadeBebe);
	int zero = 0;
	char zero_char = '0';
	char strvazia = '\0';

	// se cidadeMae for uma string vazia, escreve o int 0 no binário
	if (b->cidadeMae[0] == '$') {
		fwrite(&zero, sizeof(int), 1, fp);
		a = 0;
	} // caso contrário escreve o comprimento da string
	else  fwrite(&a, sizeof(int), 1, fp);

	// se cidadeBebe for uma string vazia, escreve o int 0 no binário
	if (b->cidadeBebe[0] == '$') {
		fwrite(&zero, sizeof(int), 1, fp);
		c = 0;
	} // caso contrário escreve o comprimento da string
	else fwrite(&c, sizeof(int), 1, fp);

	//escreve a cidadeMae e cidadeBebe no binário
	fwrite(b->cidadeMae, sizeof(char), a, fp);
	fwrite(b->cidadeBebe, sizeof(char), c, fp);

	// escreve '$', ou seja, lixo, nos bytes que restaram, até o 105 byte do registro (espaço reservado para os campos variáveis)
	int lixo =  105 - a - c - 8;
	WriteTrash(fp, lixo);

	//escreve no binário o idNascimento e a idadeMae
	fwrite(&b->idNascimento, sizeof(int), 1, fp);
	fwrite(&b->idadeMae, sizeof(int), 1, fp);
	
	// se cidadeBebe for uma string vazia, escreve \0 no binário e preenche com $
	if (b->dataNascimento[0] == '$'){
		fwrite(&strvazia, sizeof(char), 1, fp);
		WriteTrash(fp, 9);
	} // caso contrário escreve a string dataNascimento
	else if (b->dataNascimento[0] != '$') fwrite(&b->dataNascimento, sizeof(char), 10, fp);

	// se sexoBebe for uma string vazia, escreve o char '0', sexo ignorado
	if (b->sexoBebe[0] == '$') fwrite(&zero_char, sizeof(char), 1, fp);
	// caso contrário escreve a string sexoBebe
	else fwrite(&b->sexoBebe, sizeof(char), 1, fp);

	// se estadoMae for uma string vazia escreve '/0' e preenche com $ 
	if (b->estadoMae[0] == '$'){
		fwrite(&strvazia, sizeof(char), 1, fp);
		WriteTrash(fp, 1);
	} // caso contrário escreve a string estadoMae
	else if (b->estadoMae[0] != '$') fwrite(&b->estadoMae, sizeof(char), 2, fp);
	
	// se estadoBebe for uma string vazia escreve '/0' e preenche com $ 
	if (b->estadoBebe[0] == '$'){
		fwrite(&strvazia, sizeof(char), 1, fp);
		WriteTrash(fp, 1);
	} // caso contrário escreve a string estadoBebe
	else if (b->estadoBebe[0] != '$') fwrite(&b->estadoBebe, sizeof(char), 2, fp);

}

// lê o header de um arquivo binário e retorna uma struct header
void readHeader(FILE *fp, header *h){
	fseek(fp, 0, SEEK_SET);
	fread(&(h->status), sizeof(char), 1, fp);
	fread(&(h->RRNproxRegistro), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosInseridos), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosRemovidos), sizeof(int), 1, fp);
	fread(&(h->numeroRegistrosAtualizados), sizeof(int), 1, fp);
}

// lê um registro do arquivo binário e coloca as informações em uma struct baby
baby * readReg(FILE *fp, int RRN){
	baby *b = newBaby();
	int byteoffset = (RRN+1)*reg_size;
	fseek(fp, byteoffset, SEEK_SET);

	//size of cidadeMae and cidadeBebe
	int a = 0;
	int c = 0;

	fread(&(a), sizeof(int), 1, fp);

	// verifica se o registro foi removido
	if (a == -1) {
		printf("Registro Removido");
		return b; 
	}
	
	fread(&(c), sizeof(int), 1, fp);
	b->cidadeMae = malloc((a+1)*sizeof(char));
	b->cidadeBebe = malloc((c+1)*sizeof(char));

	fread(b->cidadeMae, sizeof(char), a, fp);
	fread(b->cidadeBebe, sizeof(char), c, fp);

	b->cidadeMae[a] = '\0';
	b->cidadeBebe[c] = '\0';

	// pula os bytes reservados para os campos variáveis
	fseek(fp, byteoffset+105, SEEK_SET);

	fread(&(b->idNascimento), sizeof(int), 1, fp);
	fread(&(b->idadeMae), sizeof(int), 1, fp);
	fread(b->dataNascimento, sizeof(char), 10, fp);
	fread(b->sexoBebe, sizeof(char), 1, fp);
	fread(b->estadoMae, sizeof(char), 2, fp);
	fread(b->estadoBebe, sizeof(char), 2, fp);
	return b;
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

