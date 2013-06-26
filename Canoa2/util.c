#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#define maxCaracter 30

/*
 Implementações
 */

/*Faz a leitura dos parâmetros e permite a usuário corrigir alguma entrada incorreta*/
int getArgs(int argc,char *argv[],float *velocidadeDoBarco, int *larguraDoRio, int *seed, int *fluxoDesejado, int *verbose, int *dIlha, float *pIlha, float *limiteMargens, int *tamPixel) {
    int read = 0;
    
    while(--argc){ /* Le todos os parâmetros */
        if (sscanf(argv[argc], "-b%f", velocidadeDoBarco)) read++;
        else if (sscanf(argv[argc], "-l%d", larguraDoRio)) read++;
        else if (sscanf(argv[argc], "-s%d", seed)) read++;
        else if (sscanf(argv[argc], "-f%d", fluxoDesejado)) read++;
        else if (sscanf(argv[argc], "-pI%f", pIlha)) read++;
        else if (sscanf(argv[argc], "-dI%d", dIlha)) read++;
        else if (sscanf(argv[argc], "-lM%f", limiteMargens)) read++;
	else if (sscanf(argv[argc], "-D%d", tamPixel)) read++;
        else if (strcmp(argv[argc],"-v") == 0) *verbose = 1;
    }
    return read;
}

void corrigeArgs (int argc,char *argv[],float *velocidadeDoBarco, int *larguraDoRio, int *seed, int *fluxoDesejado, int *verbose, int *dIlha, float *pIlha, float *limiteMargens, int *tamPixel) {
    /* Corrige os argumentos passados pelo usuário para atenderem os padrões necessários para o bom funcionamento do programa */
    
    if (*larguraDoRio < 12) {
        *larguraDoRio = 12;
        printf("A largura do rio deve ser um int maior ou igual a 12.\n"
               "Pressione Enter para continuar...\n");
        getchar();
    }
    
    if (*velocidadeDoBarco < 0.11) {
        *velocidadeDoBarco = 0.11;
        printf("A velocidade do barco deve ser um float maior ou igual a 0.11.\n"
               "Pressione Enter para continuar...\n");
        getchar();
    }
    
    if (*fluxoDesejado <= 0) {
        *fluxoDesejado = 1;
        printf("O fluxo desejado deve ser um int maior que 0.\n"
               "Pressione Enter para continuar...\n");
        getchar();
    }
    
    if (*pIlha < 0) {
        *pIlha = 0;
        printf("A probabilidade de ilhas deve ser um float entre 0 e 1.\n"
               "Pressione Enter para continuar...\n");
        getchar();
    }
    else if (*pIlha > 1) {
        *pIlha = 1;
        printf("A probabilidade de ilhas deve ser um float entre 0 e 1.\n"
               "Pressione Enter para continuar...\n");
        getchar();
    }
    
    else if (*dIlha < 1) {
        printf("%d\n", *dIlha);
        *dIlha = 1;
        printf("A distância entre ilhas deve ser um int maior ou igual a 1.\n"
               "Pressione Enter para continuar...\n");
        getchar();
    }
    
    else if (*limiteMargens <= 0) {
        *limiteMargens = 0.1;
        printf("O limite das margens deve ser um float maior que 0 e menor que 1.\n"
               "Pressione Enter para continuar...\n");
        getchar();
    }
    
    else if (*limiteMargens >= 1) {
        *limiteMargens = 0.9;
        printf("O limite das margens deve ser um float maior que 0 e menor que 1.\n"
               "Pressione Enter para continuar...\n");
        getchar();
    }

    else if(*tamPixel < 3)
      *tamPixel = 3;
}
