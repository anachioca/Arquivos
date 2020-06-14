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

typedef struct RegistroDeDados_{
    int nivel;
    int numeroDeChaves;
    ChaveRrn chaves[MAX_CHAVES];
    int rrnDescendentes[MAX_CHAVES + 1];
} RegistroDeDados;

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

RegistroDeDados * initRegistroDeDados(){
    RegistroDeDados * registroDeDados;
    registroDeDados = malloc(sizeof(RegistroDeDados));
    registroDeDados -> numeroDeChaves = 0;

    for(int i = 0; i < MAX_CHAVES; i++){
        registroDeDados -> chaves[i].chave = -1;
        registroDeDados -> chaves[i].rrn = -1;
    }

    for(int i = 0; i < MAX_CHAVES + 1; i++)
        registroDeDados -> rrnDescendentes[i] = -1;
}

void destroyRegistroDeDados(RegistroDeDados ** registroDeDados){
    if(*registroDeDados == NULL) return;

    free(*registroDeDados);
    *registroDeDados = NULL;
}

void carregaChave(Indice * indice, RegistroDeDados * pagina, int i){
    fread(&(pagina -> chaves[i].chave), sizeof(int), 1, indice -> arquivoDeIndice);
    fread(&(pagina -> chaves[i].rrn), sizeof(int), 1, indice -> arquivoDeIndice);
}

RegistroDeDados * carregaPagina(Indice * indice, int rrn){

    if(rrn == RRN_PAGINA_VAZIA)
        return NULL;

    long posicao = ftell(indice -> arquivoDeIndice);
    fseek(indice -> arquivoDeIndice, rrn * TAM_PAGINA + TAM_CABECALHO, SEEK_SET);

    RegistroDeDados * pagina = initRegistroDeDados();

    fread(&(pagina -> nivel), sizeof(int), 1, indice -> arquivoDeIndice);
    fread(&(pagina -> numeroDeChaves), sizeof(int), 1, indice -> arquivoDeIndice);
    for(int i = 0; i < MAX_CHAVES; i++)
        carregaChave(indice, pagina, i);
    fread(&(pagina -> rrnDescendentes), sizeof(int), MAX_CHAVES + 1
        , indice -> arquivoDeIndice);

    fseek(indice -> arquivoDeIndice, posicao, SEEK_SET);

    return pagina;
}

int buscaPelaPagina(RegistroDeDados * paginaDaChave, int chave){
    for(int i = 0; i < MAX_CHAVES; i++){
        if(paginaDaChave -> chaves[i].chave == chave)
            return paginaDaChave -> chaves[i].rrn;
    }
    return NAO_ENCONTRADO;
}

int pesquisaProximaPagina(RegistroDeDados * paginaDaChave, int chave){
    int i;
    for(i = 0; i < paginaDaChave -> numeroDeChaves; i++){
        if(paginaDaChave -> chaves[i].chave > chave)
            return paginaDaChave -> rrnDescendentes[i];
    }
    return paginaDaChave -> rrnDescendentes[i];

}

int pesquisaRecursiva(Indice * indice, int rrn, int chave
        , RegistroDeDados * paginaAtual, int posicaoDaChave){

    int rrnDoBinario;
    int rrnProximaPagina;
    if(rrn == RRN_PAGINA_VAZIA){
        destroyRegistroDeDados(&paginaAtual);
        return NAO_ENCONTRADO;
    }
    else{
        rrnDoBinario = buscaPelaPagina(paginaAtual, chave);
        if(rrnDoBinario != NAO_ENCONTRADO){
            destroyRegistroDeDados(&paginaAtual);
            return rrnDoBinario;
        }
        else{
            rrnProximaPagina = pesquisaProximaPagina(paginaAtual, chave);
            destroyRegistroDeDados(&paginaAtual);
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

void inserir(){

}