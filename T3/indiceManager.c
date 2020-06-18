// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531
// Otto Cruz Fernandes || NUSP: 11275130

#include "indiceManager.h"

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

typedef struct ChaveRrn_{
    int chave;
    int rrn;
} ChaveRrn;

typedef struct Pagina_{
    int nivel;
    int numeroDeChaves;
    ChaveRrn chaves[MAX_CHAVES];
    int rrnDescendentes[MAX_CHAVES + 1];
} Pagina;

typedef struct PosicaoDoRegistro_{
    int rrnPagina;
    int posicaoNaPagina;
} PosicaoDoRegistro;

void salvaIndice(Indice * indice);

Indice * initIndice(char * nomeDoBinario, MODO_DE_ABERTURA modo){
	Indice * indice = malloc(sizeof(Indice));
	indice -> status = '0';
  	indice -> noRaiz = -1;
  	indice -> nroNiveis = 0;
	indice -> proxRRN = 0;
	indice -> nroChaves = 0;

    indice -> arquivoDeIndice = fopen(nomeDoBinario, modo);
    if(indice -> arquivoDeIndice == NULL){
        free(indice);
        return NULL;
    }

	return indice;
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
        Pagina -> rrnDescendentes[i] = -1;
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

void escreveChaveNaPagina(Pagina * pagina, int i, ChaveRrn * chaveRrn){
    pagina -> chaves[i].chave = chaveRrn -> chave;
    pagina -> chaves[i].rrn = chaveRrn -> rrn;
}

void salvaChave(Indice * indice, ChaveRrn chaveRrn){
    fwrite(chaveRrn.chave, sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(chaveRrn.rrn, sizeof(int), 1, indice -> arquivoDeIndice);
}

void closePagina(Indice * indice, Pagina ** pagina, int rrnPagina){
    long posicaoInicial = ftell(indice -> arquivoDeIndice);
    long posicaoFinal = rrnPagina * TAM_PAGINA + TAM_CABECALHO;
    if(posicaoInicial != posicaoFinal);
        fseek(indice -> arquivoDeIndice, posicaoFinal, SEEK_SET);

    fwrite(&((*pagina) -> nivel), sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(&((*pagina) -> numeroDeChaves), sizeof(int), 1, indice -> arquivoDeIndice);
    for(int i = 0; i < (*pagina) -> numeroDeChaves; i++)
        salvaChave(indice, (*pagina) -> chaves[i]);

    fwrite((*pagina) -> rrnDescendentes, sizeof(int)
        , (*pagina) -> numeroDeChaves + 1, indice -> arquivoDeIndice);
}

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
    fread(&(pagina -> rrnDescendentes), sizeof(int), MAX_CHAVES + 1
        , indice -> arquivoDeIndice);

    fseek(indice -> arquivoDeIndice, posicao, SEEK_SET);

    return pagina;
}

int buscaPelaPagina(Pagina * paginaDaChave, int chave){
    for(int i = 0; i < MAX_CHAVES; i++){
        if(paginaDaChave -> chaves[i].chave == chave)
            return paginaDaChave -> chaves[i].rrn;
    }
    return NAO_ENCONTRADO;
}

int pesquisaProximaPagina(Pagina * paginaDaChave, int chave){
    int i;
    for(i = 0; i < paginaDaChave -> numeroDeChaves; i++){
        if(paginaDaChave -> chaves[i].chave > chave)
            return paginaDaChave -> rrnDescendentes[i];
    }
    return paginaDaChave -> rrnDescendentes[i];

}

int pesquisaRecursiva(Indice * indice, int rrn, int chave
        , Pagina * paginaAtual, int posicaoDaChave){

    int rrnDoBinario;
    int rrnProximaPagina;
    if(rrn == RRN_PAGINA_VAZIA){
        destroyPagina(&paginaAtual);
        return NAO_ENCONTRADO;
    }
    else{
        rrnDoBinario = buscaPelaPagina(paginaAtual, chave);
        if(rrnDoBinario != NAO_ENCONTRADO){
            destroyPagina(&paginaAtual);
            return rrnDoBinario;
        }
        else{
            rrnProximaPagina = pesquisaProximaPagina(paginaAtual, chave);
            destroyPagina(&paginaAtual);
            paginaAtual = carregaPagina(indice, rrnProximaPagina);
            pesquisaRecursiva(indice, rrnProximaPagina, chave,
                paginaAtual, posicaoDaChave);
        }
    }
}

int pesquisaIndice(Indice * indice, int chave){
    // return pesquisaRecursiva(indice -> noRaiz, chave, )
    // TODO: pesquisa que vai ser colocada no .h e inserção
}

void inserir(Indice * indice, int rrn, int chave){
    if(isIndiceVazio(indice)){
        criaRaiz(indice);
    }
    PosicaoDoRegistro * posicaoDoRegistro = pesquisaIndice_(chave);
    Pagina * pagina = carregaPagina(indice, posicaoDoRegistro -> rrnPagina);
    if(paginaTemEspaco(pagina)){
        ChaveRrn * chaveRrn = criaChaveRrn(chave, rrn);
        escreveChaveNaPagina(pagina, posicaoDoRegistro -> posicaoNaPagina, chaveRrn);
        closePagina(indice, &pagina, posicaoDoRegistro -> rrnPagina);
        destroyChaveRrn(&chaveRrn);
    }

    
}

// void inserir_(Indice * indice){

// }