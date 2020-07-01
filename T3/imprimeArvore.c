

void printaArvoreB(char* binarioIndex)
{
    FILE* arquivoDeIndice = fopen(binarioIndex, "rb");

    printf("Leitura resultante do arquivo: %s\n", binarioIndex);
    
    CabecalhoIndice cabecalho;
    leCabecalhoDeIndice(&cabecalho, arquivoDeIndice);

    printf("status cabecalho: %c\n", cabecalho.status);
    printf("no raiz: %d\n", cabecalho.noRaiz);
    printf("num niveis: %d\n", cabecalho.nroNiveis);
    printf("prox RRN cabecalho: %d\n", cabecalho.proxRRN);
    printf("num chaves: %d\n\n", cabecalho.nroChaves);

    for(int i = 0; i < cabecalho.proxRRN; i++)
    {
        printf("\n****nó da árvore %d do arquivo de índice: ****\n", i);
        
        Pagina no;
        leNoDaArvoreB(i, &no, arquivoDeIndice);

        printf("nivel do nó: %d\n", no.nivel);
        printf("número de chaves: %d\n", no.numeroDeChaves);
        
            for (int k = 0; k < 5; k++)
            {
                printf("\tchave[%d]: %d\trrn: %d\n", k, no.chaves[k], no.rrnDoArquivoDeDados[k]);
            }

            printf("\n\tDescendentes:  ");

            for ( int k = 0; k < 6; k++)
            {
                printf("%d ", no.descendentes[k]);
            }

            printf("\n\n");
    }

    fclose(arquivoDeIndice);
}