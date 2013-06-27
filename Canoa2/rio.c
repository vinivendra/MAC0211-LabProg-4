#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rio.h"
#include "Output.h"
#include "pixel.h"

#define limiteDasIlhas 0.5

/*
 Variável gloabal
 */

static int  distanciaAtualEntreIlhas = 0;

/*
 Protótipos
 */

float velocidadeDaAguaPrimeiraLinha (float velocidadePontoAnterior);
int tamanhoDaPrimeiraMargem(int largura, float limiteMargens);
void normaliza(pixel *linha, int largura, int fluxoDesejado);
void aleatorizaMargem(pixel *linhaAnterior, pixel *linha, float limiteMargens, int largura);
int insereIlha(pixel *linha, int distanciaMinimaEntreIlhas, float probIlha, int margemEsquerda, int margemDireita, int largura);
void velocidadeProximaLinha (pixel *linha, pixel *linhaAnterior, int fluxoDesejado, int largura);
void suavizaVelocidades (pixel *linha, int largura);
void aleatorizaPrimeiraMargem (pixel *linha, int largura, float limiteMargens);

/*
 Implementações
 */

void proximaLinha (pixel *linhaAnterior, pixel *linha, int largura, float limiteMargens, int fluxoDesejado, int distanciaEntreIlhas, float probIlha) {
    /* Calcula como deve ser a nova linha do frame */
    
    int tamanhoDaMargemEsquerda;
    int tamanhoDaMargemDireita;
    
    aleatorizaMargem(linhaAnterior, linha, limiteMargens, largura);  /* Insere as novas margens aleatorizadas */
    
    tamanhoDaMargemEsquerda = margemEsquerda(linha);
    tamanhoDaMargemDireita = margemDireita(linha, largura);
    
    insereIlha(linha, distanciaEntreIlhas, probIlha, tamanhoDaMargemEsquerda, tamanhoDaMargemDireita, largura);
    
    velocidadeProximaLinha(linha, linhaAnterior, fluxoDesejado, largura);
    
    suavizaVelocidades(linha, largura);
    
    normaliza(linha, largura, fluxoDesejado);
}

int margemEsquerda (pixel *linha) {     /* Retorna o tamanho da margem esquerda da linha */
    int n = 0;
    
    while ( tipo(&linha[n]) == TERRA)
        n++;
    
    return n;
}

int margemDireita (pixel *linha, int largura) {     /* Retorna o tamanho da margem direita da linha de tamanho 'largura' */
    int n = largura-1;
    
    while ( tipo(&linha[n]) == TERRA)
        n--;
    
    n++;
    
    return largura-n;
}

void primeiraLinha(pixel *linha, int largura, float limiteMargens, int fluxoDesejado, int distanciaEntreIlhas, float probIlha) {
    /* Insere os valores da primeira linha do programa */
    
    int tamanhoDaMargemEsquerda;
    int tamanhoDaMargemDireita;
    
    int i = 0;
    float v;
    
    aleatorizaPrimeiraMargem (linha, largura, limiteMargens);
    
    tamanhoDaMargemEsquerda = margemEsquerda(linha);
    tamanhoDaMargemDireita = margemDireita(linha, largura);
    
    insereIlha(linha, distanciaEntreIlhas, probIlha, tamanhoDaMargemEsquerda, tamanhoDaMargemDireita, largura);
    
    for (i = tamanhoDaMargemEsquerda; i < largura - tamanhoDaMargemDireita; i++) {  /* Insere a velocidade água */
        if (linha[i-1].tipo == TERRA || tipo(&linha[i]) == TERRA || linha[i+1].tipo == TERRA)
            setaVelocidade(&linha[i],0);
        else{
            v = velocidadeDaAguaPrimeiraLinha (linha[i-1].velocidade);
            setaVelocidade(&linha[i], v);
        }
    }
    
    suavizaVelocidades(linha, largura);
    
    /* Consideramos a probabilidade de a primeira linha ter ilhas */
    normaliza(linha, largura, fluxoDesejado);
}

int tamanhoDaPrimeiraMargem(int largura, float limiteMargens) {  /* Calcula um valor aleatório para o tamanho das margens da primeira linha */
    int resultado = (limiteMargens*largura - 1)*rand()/RAND_MAX + 1;     /* Valor entre 1 e o limite das margens */
    
    return resultado;
}

void aleatorizaPrimeiraMargem (pixel *linha, int largura, float limiteMargens) {
    
    int i;
    
    int tamanhoDaMargemEsquerda = (limiteMargens*largura - 1)*rand()/RAND_MAX + 1;
    int tamanhoDaMargemDireita = (limiteMargens*largura - 1)*rand()/RAND_MAX + 1;
    
    if (tamanhoDaMargemEsquerda <= 1) tamanhoDaMargemEsquerda = 2;  /* Evita que as novas margens sejam 0 ou passem do limite */
    else if (tamanhoDaMargemEsquerda > limiteMargens * largura || tamanhoDaMargemEsquerda >= largura - tamanhoDaMargemDireita - 10) tamanhoDaMargemEsquerda = (largura - 10)/2;
    
    if (tamanhoDaMargemDireita <= 1) tamanhoDaMargemDireita = 2;
    else if (tamanhoDaMargemDireita > limiteMargens * largura || tamanhoDaMargemDireita >= largura - tamanhoDaMargemEsquerda - 10) tamanhoDaMargemDireita = (largura - 10)/2;
    
    for (i = 0; i < tamanhoDaMargemEsquerda; i++){  /* Insere nova margem esquerda */
        setaTipo(&linha[i], TERRA);
        setaVelocidade(&linha[i], 0);
    }
    for (i = tamanhoDaMargemEsquerda; i < largura - tamanhoDaMargemDireita; i++) {  /* Insere água no meio, para evitar erros*/
        setaVelocidade(&linha[i], 1);
        setaTipo(&linha[i], AGUA);
    }
    for (i = largura - tamanhoDaMargemDireita; i < largura; i++) {  /* Insere nova margem direita */
        setaTipo(&linha[i], TERRA);
        setaVelocidade(&linha[i], 0);
    }
    
}


float realRandomico(float min, float max){
    float d;
    d = (float) rand() / ((float) RAND_MAX + 1);
    return (min + d * (max - min));
}

void velocidadeProximaLinha (pixel *linha, pixel *linhaAnterior, int fluxoDesejado, int largura) {
    int i = 0;
    
    int tamanhoDaMargemEsquerda = margemEsquerda(linha);
    int tamanhoDaMargemDireita = margemDireita(linha, largura);
    
    for (i = tamanhoDaMargemEsquerda; i < largura-tamanhoDaMargemDireita; i++) {
        if (tipo(&linha[i-1]) == TERRA || tipo(&linha[i]) == TERRA || tipo(&linha[i+1]) == TERRA) {
            setaVelocidade(&linha[i], 0);
        }
        else {
            float velocidadeAnterior = velocidade(&linhaAnterior[i]);
            float aleatorio = realRandomico(0.9, 1.1);
            float velocidadeNova = velocidadeAnterior*aleatorio;
            if (velocidadeNova == 0) {
                velocidadeNova = aleatorio*fluxoDesejado/(largura*8);
            }
            setaVelocidade(&linha[i], velocidadeNova);
        }
    }
    
}

float velocidadeDaAguaPrimeiraLinha (float velocidadePontoAnterior) {
    /*Adotamos que apenas nas margens do rio e das ilhas a velocidade admite ser zero*/
    /* Calcula um valor aleatório para ser a velocidade da água */
    
    float aleatorio = 1.0*rand()/RAND_MAX - 0.5;    /*Gera um número aleatório entre [-0.5,0.5]*/
    float v;
    
    if (velocidadePontoAnterior == 0)
        return 1.0*rand()/RAND_MAX;
    
    v = aleatorio + velocidadePontoAnterior;
    if( v < 0 ) v = -v;
    if(v == 0) v = 1.0*rand()/RAND_MAX;
    
    return v;
}


void suavizaVelocidades (pixel *linha, int largura) {
    int i, j;

    int tamanhoDaMargemEsquerda = margemEsquerda(linha);
    int tamanhoDaMargemDireita = margemDireita(linha, largura);
    
    for (j = 0; j < 6; j ++) {
        
        float velocidadeAnterior = 0;
        
        for (i = tamanhoDaMargemEsquerda; i < largura - tamanhoDaMargemDireita; i++) {
            if (velocidade(&linha[i]) != 0) {
                float velocidadeNova = (velocidadeAnterior + velocidade(&linha[i]) + velocidade(&linha[i+1]))/3;
                velocidadeAnterior = velocidade(&linha[i]);
                setaVelocidade(&linha[i], velocidadeNova);
            }
            else {
                velocidadeAnterior = 0;
            }
        }
    }
}


void normaliza(pixel *linha, int largura, int fluxoDesejado) {  /* Normaliza a linha para ter o fluxo desejado */
    int i = 0;
    float fluxoObtido = 0;
    float novaVel, vel;
    
    for (i = 0; i < largura; i++)       /* O fluxo obtido é a soma de todas as velocidades */
        if (tipo(& linha[i] ) != TERRA)
            fluxoObtido += velocidade(&linha[i]);
    
    for (i = 0; i < largura; i++) {     /* Transforma a linha numa que tenha o fluxo desejado */
        if (tipo(&linha[i]) != TERRA){
            vel = velocidade(&linha[i]);
            novaVel = 1.0*vel*fluxoDesejado/fluxoObtido;
            setaVelocidade(&linha[i], novaVel);
        }
    }
}

void aleatorizaMargem(pixel *linhaAnterior, pixel *linha, float limiteMargens, int largura){
    /* Calcula o novo tamanho das margens, baseado na linha anterior */
    
    int tamanhoDaMargemEsquerda = margemEsquerda(linhaAnterior);
    int tamanhoDaMargemDireita = margemDireita(linhaAnterior, largura);
    int variacaoEsquerda = (rand()%3) - 1;  /* O tamanho da margem nova vai variar da antiga de -1, 0 ou 1 */
    int variacaoDireita = (rand()%3) - 1;
    int i;
    
    tamanhoDaMargemEsquerda = tamanhoDaMargemEsquerda + variacaoEsquerda;
    tamanhoDaMargemDireita = tamanhoDaMargemDireita + variacaoDireita;
    
    if (tamanhoDaMargemEsquerda <= 1) tamanhoDaMargemEsquerda = 2;  /* Evita que as novas margens sejam 0 ou passem do limite */
    else if (tamanhoDaMargemEsquerda > limiteMargens * largura || tamanhoDaMargemEsquerda >= largura - tamanhoDaMargemDireita - 10) tamanhoDaMargemEsquerda = margemEsquerda(linhaAnterior);
    
    if (tamanhoDaMargemDireita <= 1) tamanhoDaMargemDireita = 2;
    else if (tamanhoDaMargemDireita > limiteMargens * largura || tamanhoDaMargemDireita >= largura - tamanhoDaMargemEsquerda - 10) tamanhoDaMargemDireita = margemDireita(linhaAnterior, largura);
    
    for (i = 0; i < tamanhoDaMargemEsquerda; i++){  /* Insere nova margem esquerda */
        setaTipo(&linha[i], TERRA);
        setaVelocidade(&linha[i], 0);
    }
    for (i = tamanhoDaMargemEsquerda; i < largura - tamanhoDaMargemDireita; i++) {  /* Insere água no meio, para evitar erros*/
        setaVelocidade(&linha[i], 1);
        setaTipo(&linha[i], AGUA);
    }
    for (i = largura - tamanhoDaMargemDireita; i < largura; i++) {  /* Insere nova margem direita */
        setaTipo(&linha[i], TERRA);
        setaVelocidade(&linha[i], 0);
    }
}

int insereIlha(pixel *linha, int distanciaMinimaEntreIlhas, float probIlha, int tmargemEsquerda, int tmargemDireita, int largura){
    int q, i;
    int sorteio;
    int comecoIlha, finalIlha;
    int aux, aux2;
    
    if( distanciaAtualEntreIlhas < distanciaMinimaEntreIlhas - 1){
        distanciaAtualEntreIlhas++;
        return 0;
    }
    
    q = (int) ( probIlha*1000000 - 1 );
    sorteio = rand()%1000000;
    
    if(sorteio > q){    /*Com probabilidade 1-p, sorteio sera maior que q*/
        distanciaAtualEntreIlhas++;
        return 0;
    }
    
    if(sorteio <= q){   /*Sorteio sera menor ou igual a  q com probabilidade probIlha*/
        int tamanhoMaximoDaIlha = (largura - tmargemEsquerda - tmargemDireita) * limiteDasIlhas;
        int comecoMaximoDaIlha = tmargemEsquerda + (((1-limiteDasIlhas)/2)*(largura - tmargemEsquerda - tmargemDireita));
        
        distanciaAtualEntreIlhas = 0;
        
        aux = rand()%tamanhoMaximoDaIlha;
        aux = comecoMaximoDaIlha + aux;
        aux2 = rand()%tamanhoMaximoDaIlha;
        aux2 = comecoMaximoDaIlha + aux2;
        
        comecoIlha = aux < aux2 ? aux : aux2;
        finalIlha = aux < aux2 ? aux2 : aux;
        
        if(comecoIlha <= tmargemEsquerda || comecoIlha <= tmargemEsquerda + 1) comecoIlha = tmargemEsquerda + 3;
        if(finalIlha >= largura - tmargemDireita || finalIlha >= largura - tmargemDireita - 2) finalIlha = largura - tmargemDireita - 3;
        
        if(comecoIlha > finalIlha){
            aux = comecoIlha;
            comecoIlha = finalIlha;
            finalIlha = aux;
        }
        
        for(i = comecoIlha; i <= finalIlha; i++){
            setaTipo(&linha[i], TERRA);
            setaVelocidade(&linha[i], 0);
        }
    }
    return 1;
}





