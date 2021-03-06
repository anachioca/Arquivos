// Trabalho 1 || SCC0215 - Organização de Arquivos
// Ana Laura Chioca Vieira || NUSP: 9866531
// Otto Cruz Fernandes || NUSP: 11275130

#include "indiceManager.h"
#include "baby.h"

#define bool int
#define TRUE 1
#define FALSE 0

#define ORDEM 6
#define MAX_CHAVES 5
#define MIN_CHAVES 2

#define NAO_ENCONTRADO -1
#define ENCONTRADO 1
#define RRN_PAGINA_VAZIA -1

#define TAM_PAGINA 72
#define TAM_CABECALHO 72

#define IS_PROMOTION int
#define PROMOTION 1
#define NO_PROMOTION 2
#define ERROR 3

#define SUCESSO 1
#define ERRO 0

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
    int chave;
    int rrn; // rrn do baby no arquivo de dados
} RegistroBaby;

typedef struct Pagina_{
    int nivel;
    int numeroDeChaves;
    RegistroBaby * chaves[MAX_CHAVES];
    int descendentes[MAX_CHAVES + 1];
} Pagina;

// representa a posição do baby no arquivo de indice
typedef struct PosicaoDoRegistro_{
    int rrnPagina;
    int posicaoNaPagina;
} PosicaoDoRegistro;

void salvaIndice(Indice * indice);

RegistroBaby * initRegistroBaby();

void destroyRegistroBaby(RegistroBaby ** RB);

void printChave(RegistroBaby * registroBaby, int i);

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
        if(ftell(indice -> arquivoDeIndice) != 0)
            fseek(indice->arquivoDeIndice, 0, SEEK_SET);
        else{
            fread(&indice->status, sizeof(char), 1, indice->arquivoDeIndice);
            fread(&indice->noRaiz, sizeof(int), 1, indice->arquivoDeIndice);
            fread(&indice->nroNiveis, sizeof(int), 1, indice->arquivoDeIndice);
            fread(&indice->proxRRN, sizeof(int), 1, indice->arquivoDeIndice);
            fread(&indice->nroChaves, sizeof(int), 1, indice->arquivoDeIndice);
        }
    }   

	return indice;
}

char getStatusIndice(Indice * indice){
    return indice->status;
}

int getNoRaiz(Indice * indice){
    return indice->noRaiz;
}

int getProxRRN(Indice * indice){
    return indice -> proxRRN;
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

    for(int i = 0; i < MAX_CHAVES; i++)
        pagina -> chaves[i] = NULL;

    for(int i = 0; i < MAX_CHAVES + 1; i++)
        pagina -> descendentes[i] = -1;

    return pagina;
}

void destroyPagina(Pagina ** pagina){
    if(*pagina == NULL) return;

    for(int i = 0; i < (*pagina) -> numeroDeChaves; i++){
        destroyRegistroBaby(&((*pagina) -> chaves[i]));
    }

    free(*pagina);
    *pagina = NULL;
}

RegistroBaby * initRegistroBaby(){
    RegistroBaby * registroBaby = malloc(sizeof(RegistroBaby));
    if(!registroBaby) return NULL;

    registroBaby -> chave = -1;
    registroBaby -> rrn = -1;
    return registroBaby;
}

void destroyRegistroBaby(RegistroBaby ** RB){
    if(*RB == NULL) return;
    free(*RB);
    *RB = NULL;
}

void carregaChave(Indice * indice, Pagina * pagina, int i){
    RegistroBaby * registro = initRegistroBaby();

    fread(&(registro -> chave), sizeof(int), 1, indice -> arquivoDeIndice);
    fread(&(registro -> rrn), sizeof(int), 1, indice -> arquivoDeIndice);

    if(registro -> chave == -1){
        pagina -> chaves[i] = NULL;
        destroyRegistroBaby(&registro);
    }
    else
        pagina -> chaves[i] = registro;
}

// escreve um RegistroBaby na página
void escreveChaveNaPagina(Pagina * pagina, int i, RegistroBaby * RegistroBaby){
    pagina -> chaves[i] -> chave = RegistroBaby -> chave;
    pagina -> chaves[i] -> rrn = RegistroBaby -> rrn;
}

// escreve um RegistroBaby no arquivo de indice
void salvaChave(Indice * indice, RegistroBaby * registroBaby){
    int chave;
    int rrn;

    if(!registroBaby){
        chave = -1;
        rrn = -1;
    }
    else{
        chave = registroBaby -> chave;
        rrn = registroBaby -> rrn;
    }

    fwrite(&chave, sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(&rrn, sizeof(int), 1, indice -> arquivoDeIndice);
}

void printChave(RegistroBaby * registroBaby, int i){
    printf("Chave %d: %d, %d\n", i, registroBaby -> chave, registroBaby -> rrn);
}

void printPagina(Pagina * pagina){

    printf("Nível: %d\n", pagina -> nivel);
    printf("Número de Chaves: %d\n", pagina -> numeroDeChaves);

    for (int i = 0; i < pagina -> numeroDeChaves; i++)
        printChave(pagina -> chaves[i], i);

    for (int i = 0; i < MAX_CHAVES + 1; i++)
        printf("Descendente %d: %d\n", i, pagina->descendentes[i]);
}

// escreve no arquivo de indice uma página
void salvaPagina(Indice * indice, Pagina ** pagina, int rrnPagina){

    long posicaoInicial = ftell(indice -> arquivoDeIndice);
    long posicaoPagina = rrnPagina * TAM_PAGINA + TAM_CABECALHO;
    if(posicaoInicial != posicaoPagina);
        fseek(indice -> arquivoDeIndice, posicaoPagina, SEEK_SET);

    fwrite(&((*pagina) -> nivel), sizeof(int), 1, indice -> arquivoDeIndice);
    fwrite(&((*pagina) -> numeroDeChaves), sizeof(int), 1, indice -> arquivoDeIndice);

    for(int i = 0; i < MAX_CHAVES; i++)
        salvaChave(indice, (*pagina) -> chaves[i]);

    fwrite((*pagina) -> descendentes, sizeof(int), MAX_CHAVES + 1, indice -> arquivoDeIndice);
}

// função que traz uma página do arquivo de indice para a RAM
Pagina * carregaPagina(Indice * indice, int rrn){

    if(rrn == RRN_PAGINA_VAZIA)
        return NULL;

    long posicaoInicial = ftell(indice -> arquivoDeIndice);
    long posicaoPagina = rrn * TAM_PAGINA + TAM_CABECALHO;
    if(posicaoInicial != posicaoPagina)
        fseek(indice -> arquivoDeIndice, posicaoPagina, SEEK_SET);

    Pagina * pagina = initPagina();

    fread(&(pagina -> nivel), sizeof(int), 1, indice -> arquivoDeIndice);
    fread(&(pagina -> numeroDeChaves), sizeof(int), 1, indice -> arquivoDeIndice);
    for(int i = 0; i < MAX_CHAVES; i++)
        carregaChave(indice, pagina, i);

    fread(&(pagina -> descendentes), sizeof(int), MAX_CHAVES + 1
        , indice -> arquivoDeIndice);

    fseek(indice -> arquivoDeIndice, posicaoInicial, SEEK_SET);

    return pagina;
}

// verifica se a chave se encontra na página atual
// caso esteja, retorna o RRN do registro no arquivo de dados
// caso contrário retorna NAO_ENCONTRADO
int buscaPelaChaveNaPagina(Pagina * pagina, int chave){
    for(int i = 0; i < pagina ->numeroDeChaves; i++){
        if( pagina -> chaves[i] -> chave == chave)
            return i;
    }
    return NAO_ENCONTRADO;
}

void buscaPelaPosicaoNaPagina(Pagina * paginaDaChave, int chaveInserida, int * i){
    int chaveAtual;

    for(*i = 0; *i < paginaDaChave -> numeroDeChaves; (*i)++){
        chaveAtual = paginaDaChave -> chaves[*i] -> chave;
        if(chaveInserida < chaveAtual)
            break;
    }
    return;
}

// procura, entre os descendentes da página atual, qual
// será a próxima página a ser pesquisada
int pesquisaProximaPagina(Pagina * paginaDaChave, int chave){
    int i;
    for(i = 0; i < paginaDaChave -> numeroDeChaves; i++){
        if(paginaDaChave -> chaves[i] -> chave > chave)
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
        PR->posicaoNaPagina = buscaPelaChaveNaPagina(paginaAtual, chave);
        if(PR->posicaoNaPagina != NAO_ENCONTRADO){ // a chave foi encontrada
            destroyPagina(&paginaAtual);
            return PR;
        }
        else{ // caso contrário continua para a próxima página
            free(PR);
            rrnProximaPagina = pesquisaProximaPagina(paginaAtual, chave);
            destroyPagina(&paginaAtual);
            paginaAtual = carregaPagina(indice, rrnProximaPagina);
            return pesquisaRecursiva(indice, rrnProximaPagina, chave, paginaAtual, rrn, count);
        }
    }
}

// retorna o RRN do baby na posição PR
int getRRNIndice(Indice * indice, PosicaoDoRegistro * PR){
    if(!PR || PR -> posicaoNaPagina == -1)
        return -1;
    
    Pagina * pagina = carregaPagina(indice, PR->rrnPagina);
    int posicao = PR -> posicaoNaPagina;
    int rrn = pagina -> chaves[posicao] -> rrn;
    destroyPagina(&pagina);
    return rrn;
}

// Pesquisa no arquivo de indice por uma chave
// e retorna a posição do exata do registro no arquivo de indice
int pesquisaIndice(Indice * indice, int chave, int * count){
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

bool paginaTemEspaco(Pagina * pagina){
    return pagina -> numeroDeChaves < MAX_CHAVES;
}

void swapRegistros(RegistroBaby ** xp, RegistroBaby ** yp) { 
    RegistroBaby * temp = *xp; 
    *xp = *yp;
    *yp = temp; 
}

void swapInt(int * a, int * b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void shiftaChavesEDescendentes(Pagina * pagina, int posicaoASerInserida){
    for(int i = pagina -> numeroDeChaves; i > posicaoASerInserida; i--){
        swapRegistros(&(pagina -> chaves[i]), &(pagina -> chaves[i-1]));
        swapInt(&(pagina -> descendentes[i+1]), &(pagina -> descendentes[i]));
    }
}

int insereOrdenadoChaveNaPagina(Indice * indice, Pagina * pagina, RegistroBaby * 
    chavePromovida, int rrnPromovido){

    if(!paginaTemEspaco(pagina)) return ERRO;
  
    int posicaoASerInserida;
    buscaPelaPosicaoNaPagina(pagina, chavePromovida -> chave, &posicaoASerInserida);
    shiftaChavesEDescendentes(pagina, posicaoASerInserida);

    pagina -> chaves[posicaoASerInserida] = chavePromovida;
    pagina -> descendentes[posicaoASerInserida + 1] = rrnPromovido;

    pagina -> numeroDeChaves++;
    indice -> nroChaves++;

}

void limpaPagina(Pagina * pagina){
    for (int i = 0; i < MAX_CHAVES; i++){
        pagina->chaves[i] = NULL;
        pagina->descendentes[i] = -1;
    }
    pagina->descendentes[MAX_CHAVES] = -1;
    pagina->numeroDeChaves = 0;
}


RegistroBaby * split(Indice * indice, Pagina * paginaAntiga, RegistroBaby *
    chavePromovida, int * filhoDaChavePromovida, Pagina * novaPagina){

    int i, meio;
    RegistroBaby * elementos[MAX_CHAVES + 1];
    int children[MAX_CHAVES + 2];

    for (i = 0; i < MAX_CHAVES; i++){
        elementos[i] = paginaAntiga -> chaves[i];
        children[i] = paginaAntiga -> descendentes[i];
    }
    children[i] = paginaAntiga -> descendentes[i];

    // inserindo a nova chave
    elementos[MAX_CHAVES] = elementos[MAX_CHAVES - 1];
    int posicaoAInserir;
    buscaPelaPosicaoNaPagina(paginaAntiga, chavePromovida -> chave, &posicaoAInserir);

    for (i = MAX_CHAVES; i != posicaoAInserir; i--){
        elementos[i] = elementos[i-1];
        children[i+1] = children[i];
    }
    elementos[i] = chavePromovida;
    children[i+1] = *filhoDaChavePromovida;

    *filhoDaChavePromovida = indice -> proxRRN;
    indice -> proxRRN++;

    limpaPagina(paginaAntiga);

    for (int i = 0; i < (MAX_CHAVES+1)/2; i++)
        paginaAntiga -> chaves[i] = elementos[i];

    for(int i = ((MAX_CHAVES+1)/2) + 1, j = 0; i < MAX_CHAVES + 1; i++, j++)
        novaPagina -> chaves[j] = elementos[i];

    for (int i = 0; i <= (MAX_CHAVES+2)/2; i++)
        paginaAntiga -> descendentes[i] = children[i];

    for(int i = ((MAX_CHAVES+2)/2) + 1, j = 0; i < MAX_CHAVES + 2; i++, j++){
        novaPagina -> descendentes[j] = children[i];
    }


    novaPagina -> numeroDeChaves = MIN_CHAVES;
    paginaAntiga -> numeroDeChaves = MIN_CHAVES + 1;

    return elementos[(MAX_CHAVES+1)/2];
}


void criaNovoNoRaiz(Indice * indice, int rrnPaginaEsquerda, int rrnPaginaDireita, 
    RegistroBaby * chavePromovida, int nivel){

    Pagina * novoNoRaiz = initPagina();
    insereOrdenadoChaveNaPagina(indice, novoNoRaiz, chavePromovida, rrnPaginaDireita);
    novoNoRaiz -> descendentes[0] = rrnPaginaEsquerda;
    novoNoRaiz -> nivel = nivel + 1;

    indice -> noRaiz = indice -> proxRRN;
    indice -> nroNiveis++;
    indice -> proxRRN++;

    salvaPagina(indice, &novoNoRaiz, indice -> noRaiz);
    destroyPagina(&novoNoRaiz);

    return;
}

bool chaveEstaNaPagina(Pagina * pagina, RegistroBaby * chaveInserida
    , int posicaoEncontrada){
    
    if(posicaoEncontrada == 0)
        return FALSE;
    else
        return(
            pagina -> chaves[posicaoEncontrada - 1] -> chave ==
            chaveInserida -> chave
        );
}

RegistroBaby * insercaoRecursiva(Indice * indice, int rrnPaginaAtual,
    RegistroBaby * chaveInserida, RegistroBaby ** chavePromovida,
    int * filhoDaChaveDeCima){
    
    Pagina * pagina;
    int posicaoNaPagina;
    RegistroBaby * retorno;
    Pagina * novaPagina;


    if(rrnPaginaAtual == RRN_PAGINA_VAZIA){
        *chavePromovida = chaveInserida;
        *filhoDaChaveDeCima = -1;
        return *chavePromovida;
    }
    else{
        pagina = carregaPagina(indice, rrnPaginaAtual);
        buscaPelaPosicaoNaPagina(pagina, chaveInserida -> chave, &posicaoNaPagina);
        if(chaveEstaNaPagina(pagina, chaveInserida, posicaoNaPagina))
            return NULL;

        retorno = insercaoRecursiva(indice, pagina -> descendentes[posicaoNaPagina],
            chaveInserida, chavePromovida, filhoDaChaveDeCima);

        if(retorno == NULL){
            destroyPagina(&pagina);
            return NULL;
        }

        else if(paginaTemEspaco(pagina)){
            insereOrdenadoChaveNaPagina(indice, pagina, retorno
            , *filhoDaChaveDeCima);
            salvaPagina(indice, &pagina, rrnPaginaAtual);
            destroyPagina(&pagina);
            
            return NULL;
        }
        else{
    
            novaPagina = initPagina();
            novaPagina -> nivel = pagina -> nivel;
            *chavePromovida = split(indice, pagina, *chavePromovida, filhoDaChaveDeCima, novaPagina);

            if (rrnPaginaAtual == indice->noRaiz){
                criaNovoNoRaiz(indice, rrnPaginaAtual, *filhoDaChaveDeCima
                , *chavePromovida, novaPagina->nivel);
            }

            salvaPagina(indice, &pagina, rrnPaginaAtual);
            salvaPagina(indice, &novaPagina, *filhoDaChaveDeCima);

            destroyPagina(&pagina);
            destroyPagina(&novaPagina);

            return *chavePromovida;
        }
    }
}


void inserir(Indice * indice, int chave, int rrnNoBinario){

    RegistroBaby * chaveInserida = initRegistroBaby();
    chaveInserida -> chave = chave;
    chaveInserida -> rrn = rrnNoBinario;

    RegistroBaby * chavePromovida;
    int filhoDaChaveDeCima;

    if(indice -> noRaiz == -1){
        criaNovoNoRaiz(indice, -1, -1, chaveInserida, 0);
    }
        
    else{
        insercaoRecursiva(indice, indice -> noRaiz, chaveInserida, &chavePromovida,
        &filhoDaChaveDeCima);
    }
}
