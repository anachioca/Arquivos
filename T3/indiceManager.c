// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531
// Otto Cruz Fernandes || NUSP: 11275130

#include "indiceManager.h"
#include "baby.h"

#define ORDEM 6
#define MAX_CHAVES 5

#define NAO_ENCONTRADO -1
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
    int Descendentes[MAX_CHAVES + 1];
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
    Pagina * Pagina;
    Pagina = malloc(sizeof(Pagina));
    Pagina -> numeroDeChaves = 0;

    for(int i = 0; i < MAX_CHAVES; i++){
        Pagina -> chaves[i].chave = -1;
        Pagina -> chaves[i].rrn = -1;
    }

    for(int i = 0; i < MAX_CHAVES + 1; i++)
        Pagina -> Descendentes[i] = -1;
}

void destroyPagina(Pagina ** Pagina){
    if(*Pagina == NULL) return;
    free(*Pagina);
    *Pagina = NULL;
}

void carregaChave(Indice * indice, Pagina * pagina, int i){
    fread(&(pagina -> chaves[i].chave), sizeof(int), 1, indice -> arquivoDeIndice);
    fread(&(pagina -> chaves[i].rrn), sizeof(int), 1, indice -> arquivoDeIndice);
}

// escreve um RegistroBaby na página
void escreveChaveNaPagina(Pagina * pagina, int i, RegistroBaby * RegistroBaby){
    pagina -> chaves[i].chave = RegistroBaby -> chave;
    pagina -> chaves[i].rrn = RegistroBaby -> rrn;
}

// escreve um RegistroBaby no arquivo de indice
void salvaChave(Indice * indice, RegistroBaby RegistroBaby){
    fwrite(&RegistroBaby.chave, sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(&RegistroBaby.rrn, sizeof(int), 1, indice -> arquivoDeIndice);
}

// escreve no arquivo de indice uma página
void closePagina(Indice * indice, Pagina ** pagina, int rrnPagina){
    long posicaoInicial = ftell(indice -> arquivoDeIndice);
    long posicaoFinal = rrnPagina * TAM_PAGINA + TAM_CABECALHO;
    if(posicaoInicial != posicaoFinal);
        fseek(indice -> arquivoDeIndice, posicaoFinal, SEEK_SET);

    fwrite(&((*pagina) -> nivel), sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(&((*pagina) -> numeroDeChaves), sizeof(int), 1, indice -> arquivoDeIndice);

    // if chave == -1 escrever lixo!!!
    for(int i = 0; i < (*pagina) -> numeroDeChaves; i++)
        salvaChave(indice, (*pagina) -> chaves[i]);

    fwrite((*pagina) -> Descendentes, sizeof(int), (*pagina) -> numeroDeChaves + 1, indice -> arquivoDeIndice);
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
    fread(&(pagina -> Descendentes), sizeof(int), MAX_CHAVES + 1
        , indice -> arquivoDeIndice);

    fseek(indice -> arquivoDeIndice, posicao, SEEK_SET);

    return pagina;
}

// verifica se a chave se encontra na página atual
// caso esteja, retorna o RRN do registro no arquivo de dados
// caso contrário retorna NAO_ENCONTRADO
int buscaPelaPagina(Pagina * paginaDaChave, int chave){
    for(int i = 0; i < MAX_CHAVES; i++){
        if(paginaDaChave -> chaves[i].chave == chave)
            return i;
    }
    return NAO_ENCONTRADO;
}

// procura, entre os descendentes da página atual, qual
// será a próxima página a ser pesquisada
int pesquisaProximaPagina(Pagina * paginaDaChave, int chave){
    int i;
    for(i = 0; i < paginaDaChave -> numeroDeChaves; i++){
        if(paginaDaChave -> chaves[i].chave > chave)
            return paginaDaChave -> Descendentes[i];
    }
    return paginaDaChave -> Descendentes[i];

}

// retorna a posição exata do baby no arquivo de dados
// ou seja, o RRN no binário de dados
PosicaoDoRegistro * pesquisaRecursiva(Indice * indice, int rrn, int chave, Pagina * paginaAtual, int rrnPaginaAnterior, int * count){

    PosicaoDoRegistro * PR = malloc(sizeof(PosicaoDoRegistro));
    PR ->posicaoNaPagina = -1;
    int rrnProximaPagina;
    count[0]++;

    if(rrn == RRN_PAGINA_VAZIA){
        destroyPagina(&paginaAtual);
        PR->rrnPagina = rrnPaginaAnterior;
        return PR;
    }

    else{

        PR->rrnPagina = rrn;
        PR->posicaoNaPagina = buscaPelaPagina(paginaAtual, chave);
        if(PR->posicaoNaPagina != NAO_ENCONTRADO){ // a chave foi encontrada
            destroyPagina(&paginaAtual);
            return PR;
        }
        else{ // caso contrário continua para a próxima página
            rrnProximaPagina = pesquisaProximaPagina(paginaAtual, chave);
            destroyPagina(&paginaAtual);
            paginaAtual = carregaPagina(indice, rrnProximaPagina);
            return pesquisaRecursiva(indice, rrnProximaPagina, chave, paginaAtual, rrn, count);
        }
    }
}

// retorna o RRN do baby na posição PR
int getRRNIndice(Indice * indice, PosicaoDoRegistro * PR){
    Pagina * p = carregaPagina(indice, PR->rrnPagina);
    return p->chaves[PR->posicaoNaPagina].rrn;
}

// Pesquisa no arquivo de indice por uma chave
// e retorna a posição do exata do registro no arquivo de indice
int pesquisaIndice_(Indice * indice, int chave, int * count){
    if (indice->noRaiz == -1) return -1;
    Pagina * paginaRaiz = carregaPagina(indice, indice->noRaiz);
    PosicaoDoRegistro * PR = pesquisaRecursiva(indice, indice->noRaiz, chave, paginaRaiz, -1, count);
    if (PR->rrnPagina == -1) return NAO_ENCONTRADO;

    // tendo em mãos a posição exata do registro no indice, retorna o RRN 
    // do registro no arquivo de dados
    return getRRNIndice(indice, PR);
}

// insere um novo registroBaby ao arquivo de indice
// void inserir(Indice * indice, int rrn, int chave){
//     if(indice->nroChaves == 0){
//         Pagina * raiz = initPagina();
//         // setar como raiz?
//     }
    
//     PosicaoDoRegistro * posicaoDoRegistro = pesquisaIndice_(indice, chave);
//     Pagina * pagina = carregaPagina(indice, posicaoDoRegistro -> rrnPagina);
//     if(pagina->numeroDeChaves < 5){ // se ainda tem espaço no nó, insere a chave
//         RegistroBaby * RegistroBaby = criaRegistroBaby(chave, rrn);
//         escreveChaveNaPagina(pagina, posicaoDoRegistro -> posicaoNaPagina, RegistroBaby);
//         closePagina(indice, &pagina, posicaoDoRegistro -> rrnPagina);
//         destroyRegistroBaby(&RegistroBaby);
//     }
//     else {
//         // spliiiit e promotion
//     }
// }