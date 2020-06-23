// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531
// Otto Cruz Fernandes || NUSP: 11275130

#include "indiceManager.h"
#include "baby.h"

#define ORDEM 6
#define MAX_CHAVES 5

#define NAO_ENCONTRADO -1
#define ENCONTRADO 1
#define RRN_PAGINA_VAZIA -1

#define TAM_PAGINA 72
#define TAM_CABECALHO 72

typedef struct Indice_{
    char status;
    int noRaiz;
    int nroNiveis;
    int proxRRN;
    int nroChaves;
    FILE * arquivoDeIndice;
} Indice;

// dupla que representa um baby no arquivo de indice
typedef struct RegistroBaby_{
    int chave; // era pra ser char aaaaaaa !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    int rrn; // rrn do baby no arquivo de dados
} RegistroBaby;

typedef struct Pagina_{
    int nivel;
    int numeroDeChaves;
    RegistroBaby chaves[MAX_CHAVES];
    int descendentes[MAX_CHAVES + 1];
} Pagina;

// representa a posição do baby no arquivo de indice
typedef struct PosicaoDoRegistro_{
    int rrnPagina;
    int posicaoNaPagina;
} PosicaoDoRegistro;

void salvaIndice(Indice * indice);

Indice * initIndice(char * nomeDoBinario, MODO_DE_ABERTURA modo){
	Indice * indice = malloc(sizeof(Indice));
    indice -> arquivoDeIndice = fopen(nomeDoBinario, modo);

    if(indice -> arquivoDeIndice == NULL){
        free(indice);
        return NULL;
    } else if (!strcmp(modo, ARQUIVO_NOVO)){
        indice -> status = '0';
        indice -> noRaiz = -1;
        indice -> nroNiveis = 0;
        indice -> proxRRN = 0;
        indice -> nroChaves = 0;
    } else if (!strcmp(modo, ARQUIVO_EXISTENTE)){
        fseek(indice->arquivoDeIndice, 0, SEEK_SET);
        fread(&indice->status, sizeof(char), 1, indice->arquivoDeIndice);
        fread(&indice->noRaiz, sizeof(int), 1, indice->arquivoDeIndice);
        fread(&indice->nroNiveis, sizeof(int), 1, indice->arquivoDeIndice);
        fread(&indice->proxRRN, sizeof(int), 1, indice->arquivoDeIndice);
        fread(&indice->nroChaves, sizeof(int), 1, indice->arquivoDeIndice);
    }   

	return indice;
}

char getStatusIndice(Indice * indice){
    return indice->status;
}

int getNoRaiz(Indice * indice){
    return indice->noRaiz;
}

// função para debugar
void printHeaderIndice(Indice * indice){
    printf("\nstatus: %c\n", indice->status);
    printf("noRaiz: %d\n", indice->noRaiz);
    printf("nroNiveis: %d\n", indice->nroNiveis);
    printf("proxRRN: %d\n", indice->proxRRN);
    printf("nroChaves: %d\n", indice->nroChaves);
}

void closeIndice(Indice ** indice){
    salvaIndice(*indice);
    fclose((*indice) -> arquivoDeIndice);
    free(*indice);
    *indice = NULL;
}

void fwriteLixo(FILE * pasta, size_t quantidade){
    char lixo = '$';
    for(int i = 0; i < quantidade; i++){
        fwrite(&lixo, 1, 1, pasta);
    }
}

void salvaIndice(Indice * indice){
    indice -> status = '1';

    fseek(indice -> arquivoDeIndice, 0, SEEK_SET);
    fwrite(&(indice -> status), 1, 1, indice -> arquivoDeIndice);
    fwrite(&(indice -> noRaiz), sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(&(indice -> nroNiveis), sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(&(indice -> proxRRN), sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(&(indice -> nroChaves), sizeof(int), 1, indice -> arquivoDeIndice);

    fwriteLixo(indice -> arquivoDeIndice, 55);
}

Pagina * initPagina(){
    Pagina * pagina;
    pagina = malloc(sizeof(Pagina));
    pagina -> numeroDeChaves = 0;

    for(int i = 0; i < MAX_CHAVES; i++){
        pagina -> chaves[i].chave = -1;
        pagina -> chaves[i].rrn = -1;
    }

    for(int i = 0; i < MAX_CHAVES + 1; i++)
        pagina -> descendentes[i] = -1;

    return pagina;
}

void destroyPagina(Pagina ** pagina){
    if(*pagina == NULL) return;
    free(*pagina);
    *pagina = NULL;
}

void destroyRegistroBaby(RegistroBaby ** RB){
    if(*RB == NULL) return;
    free(*RB);
    *RB = NULL;
}

void carregaChave(Indice * indice, Pagina * pagina, int i){
    //printf("%d ", i);
    fread(&(pagina -> chaves[i].chave), sizeof(int), 1, indice -> arquivoDeIndice);
    fread(&(pagina -> chaves[i].rrn), sizeof(int), 1, indice -> arquivoDeIndice);
}

// escreve um RegistroBaby na página
void escreveChaveNaPagina(Pagina * pagina, int i, RegistroBaby * RegistroBaby){
    pagina -> chaves[i].chave = RegistroBaby -> chave;
    pagina -> chaves[i].rrn = RegistroBaby -> rrn;
}

// escreve um RegistroBaby no arquivo de indice
void salvaChave(Indice * indice, RegistroBaby registroBaby){
    if (registroBaby.chave == -1) fwriteLixo(indice->arquivoDeIndice, sizeof(int));
    else fwrite(&registroBaby.chave, sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(&registroBaby.rrn, sizeof(int), 1, indice -> arquivoDeIndice);
}

void printPagina(Pagina * pagina){
    printf("\nNível: %d\n", pagina->nivel);
    printf("Número de Chaves: %d\n", pagina->numeroDeChaves);
    for (int i = 0; i < MAX_CHAVES; i++){
        printf("Chave %d: %d, %d\n", i, pagina->chaves[i].chave, pagina->chaves[i].rrn);
    }
    for (int i = 0; i < MAX_CHAVES + 1; i++){
        printf("Descendente %d: %d\n", i, pagina->descendentes[i]);
    }
}

// escreve no arquivo de indice uma página
void closePagina(Indice * indice, Pagina ** pagina, int rrnPagina){
    long posicaoInicial = ftell(indice -> arquivoDeIndice);
    long posicaoPagina = rrnPagina * TAM_PAGINA + TAM_CABECALHO;
    if(posicaoInicial != posicaoPagina);
        fseek(indice -> arquivoDeIndice, posicaoPagina, SEEK_SET);

    // printf("numero de chaves: %d\n", (*pagina)->numeroDeChaves);
    // printf("nível: %d\n", (*pagina)->nivel);
    fwrite(&((*pagina) -> nivel), sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(&((*pagina) -> numeroDeChaves), sizeof(int), 1, indice -> arquivoDeIndice);

    // if chave == -1 escrever lixo
    for(int i = 0; i < MAX_CHAVES; i++)
        salvaChave(indice, (*pagina) -> chaves[i]);

    fwrite((*pagina) -> descendentes, sizeof(int), MAX_CHAVES + 1, indice -> arquivoDeIndice);
}

// função que traz uma página do arquivo de indice para a RAM
Pagina * carregaPagina(Indice * indice, int rrn){

    if(rrn == RRN_PAGINA_VAZIA)
        return NULL;

    long posicao = ftell(indice -> arquivoDeIndice);
    fseek(indice -> arquivoDeIndice, rrn * TAM_PAGINA + TAM_CABECALHO, SEEK_SET);

    Pagina * pagina = initPagina();

    fread(&(pagina -> nivel), sizeof(int), 1, indice -> arquivoDeIndice);
    fread(&(pagina -> numeroDeChaves), sizeof(int), 1, indice -> arquivoDeIndice);
    for(int i = 0; i < MAX_CHAVES; i++)
        carregaChave(indice, pagina, i);
    fread(&(pagina -> descendentes), sizeof(int), MAX_CHAVES + 1
        , indice -> arquivoDeIndice);

    fseek(indice -> arquivoDeIndice, posicao, SEEK_SET);
    //printPagina(pagina);

    return pagina;
}

// verifica se a chave se encontra na página atual
// caso esteja, retorna o RRN do registro no arquivo de dados
// caso contrário retorna NAO_ENCONTRADO
int buscaPelaPagina(Pagina * paginaDaChave, int chave, int * i){
    for(*i = 0; *i < MAX_CHAVES; (*i)++){
        if(paginaDaChave -> chaves[*i].chave >= chave)
            return ENCONTRADO;
    }
    return NAO_ENCONTRADO;
}

// procura, entre os descendentes da página atual, qual
// será a próxima página a ser pesquisada
int pesquisaProximaPagina(Pagina * paginaDaChave, int chave){
    int i;
    for(i = 0; i < paginaDaChave -> numeroDeChaves; i++){
        if(paginaDaChave -> chaves[i].chave > chave)
            return paginaDaChave -> descendentes[i];
    }
    return paginaDaChave -> descendentes[i];

}

// retorna a posição exata do baby no arquivo de dados
// ou seja, o RRN no binário de dados
PosicaoDoRegistro * pesquisaRecursiva(Indice * indice, int rrn, int chave, Pagina * paginaAtual, int rrnPaginaAnterior, int * count){

    PosicaoDoRegistro * PR = malloc(sizeof(PosicaoDoRegistro));
    PR -> posicaoNaPagina = -1;
    int rrnProximaPagina;
    (*count)++;

    if(rrn == RRN_PAGINA_VAZIA){
        destroyPagina(&paginaAtual);
        PR->rrnPagina = rrnPaginaAnterior;
        return PR;
    }

    else{

        PR->rrnPagina = rrn;
        if(buscaPelaPagina(paginaAtual, chave, &(PR->posicaoNaPagina)) != NAO_ENCONTRADO){ // a chave foi encontrada
            destroyPagina(&paginaAtual);
            return PR;
        }
        else{ // caso contrário continua para a próxima página
            rrnProximaPagina = pesquisaProximaPagina(paginaAtual, chave);
            destroyPagina(&paginaAtual);
            paginaAtual = carregaPagina(indice, rrnProximaPagina);
            free(PR);
            return pesquisaRecursiva(indice, rrnProximaPagina, chave, paginaAtual, rrn, count);
        }
    }
}

// retorna o RRN do baby na posição PR
int getRRNIndice(Indice * indice, PosicaoDoRegistro * PR){
    Pagina * p = carregaPagina(indice, PR->rrnPagina);
    int rrn = p->chaves[PR->posicaoNaPagina].rrn;
    free(p);
    return rrn;
}

// Pesquisa no arquivo de indice por uma chave
// e retorna a posição do exata do registro no arquivo de indice
int pesquisaIndice_(Indice * indice, int chave, int * count){
    if (indice->noRaiz == -1) return -1;
    Pagina * paginaRaiz = carregaPagina(indice, indice->noRaiz);
    PosicaoDoRegistro * PR = pesquisaRecursiva(indice, indice->noRaiz, chave, paginaRaiz, -1, count);
    if (PR->rrnPagina == -1){
        free(PR);
        return NAO_ENCONTRADO;
    }

    // tendo em mãos a posição exata do registro no indice, retorna o RRN 
    // do registro no arquivo de dados
    int rrnIndice = getRRNIndice(indice, PR);
    free(PR);
    return rrnIndice;
}

RegistroBaby * criaRegistroBaby(int chave, int rrn){
    RegistroBaby * registroBaby = malloc(sizeof(RegistroBaby));
    registroBaby->chave = chave;
    registroBaby->rrn = rrn;
    return registroBaby;
}

void insereChaveSemOverflow(Indice * indice, int rrn, int chave, int nivel){
    Pagina * raiz = initPagina();
    int nroChaves = raiz -> numeroDeChaves;
    raiz -> chaves[nroChaves].chave = chave;
    raiz -> chaves[nroChaves].rrn = rrn;
    raiz -> nivel = nivel;
    raiz -> numeroDeChaves++;

    indice -> noRaiz = 0;
    indice -> proxRRN = 1;
    indice -> nroChaves = 1;
}

// insere um novo registroBaby ao arquivo de indice
void inserir(Indice * indice, int rrn, int chave){
    if(indice->nroChaves == 0){
        Pagina * raiz = initPagina();
        raiz -> chaves[0].chave = chave;
        raiz -> chaves[0].rrn = rrn;
        raiz -> nivel = 1;
        raiz -> numeroDeChaves = 1;

        indice -> noRaiz = 0;
        indice -> nroNiveis = 1;
        indice -> proxRRN = 1;
        indice -> nroChaves = 1;

        closePagina(indice, &raiz, 0);
        return;
    }

    int * count = malloc(sizeof(int));
    *count = 0;
    
    PosicaoDoRegistro * posicaoDoRegistro = pesquisaRecursiva(
        indice, indice->noRaiz, chave, carregaPagina(indice, indice->noRaiz)
        , -1, count);

    Pagina * pagina = carregaPagina(indice, posicaoDoRegistro -> rrnPagina);

    if(pagina->numeroDeChaves < 5){ // se ainda tem espaço no nó, insere a chave
        // procurar a posição exata dentro da página:
        buscaPelaPagina(pagina, chave, &(posicaoDoRegistro->posicaoNaPagina));
        RegistroBaby * registroBaby = criaRegistroBaby(chave, rrn);
        escreveChaveNaPagina(pagina, posicaoDoRegistro -> posicaoNaPagina, registroBaby);
        pagina->numeroDeChaves++;
        closePagina(indice, &pagina, posicaoDoRegistro -> rrnPagina);
        destroyRegistroBaby(&registroBaby);
    }
    else {
        // spliiiit e promotion
    }

    free(count);
}