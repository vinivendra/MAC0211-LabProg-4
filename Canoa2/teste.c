#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "Output.h"
#include "grade.h"
#include "util.h"
#include "pixel.h"
#include "rotinasTeste.h"

int main(int argc, char* argv[]){

  pixel **grade;
  float velMin = 100000000, velMedia = 0, velMax = 0, margDirMedia = 0, margEsqMedia = 0;
  int margEsqMin = 10000000, margEsqMax = 0;
  int margDirMin = 10000000, margDirMax = 0;
  int indice = 0;
  int j;
  int size;
  float velocidadeDoBarco = 1;
  int larguraDoRio = 16;
  int fluxoDesejado = 1;
  int dIlha = 15;
  float pIlha = 0.1;
  float limiteMargens = 0.2;
  int alturaDaGrade = 30;
  int verbose = 0;
  int seed = (int)time(NULL);
  int linhasCorretas = 0;

  getArgs(argc, argv, &velocidadeDoBarco, &larguraDoRio, &seed, &fluxoDesejado,
	  &verbose, &dIlha, &pIlha, &limiteMargens, &size);

  srand(seed);

  if (verbose) {
    printf ("\t \t Opcoes disponiveis: \n"
	    "-b = %f  - Velocidade do barco\n"
	    "-l = %d  - Largura do Rio\n"
	    "-s = %d  - Semente para o gerador aleatorio\n"
	    "-f = %d  - Fluxo da agua\n"
	    "-v = %d  - Verbose\n"
	    "-pI = %f - Probabilidade de haver obstaculos\n"
	    "-dI = %d - Distancia minima entre obstaculos\n"
	    "-lM = %f - Limite das margens\n"
	    "Pressione Enter para continuar...\n", velocidadeDoBarco, larguraDoRio, seed, fluxoDesejado, verbose, pIlha, dIlha, limiteMargens);
    getchar();
  }


  grade = initGrade(alturaDaGrade, larguraDoRio);
  
  criaPrimeiroFrame(grade, alturaDaGrade, larguraDoRio, limiteMargens, fluxoDesejado, dIlha, pIlha);
    
  linhasCorretas += calculaVariacoes(grade, larguraDoRio, alturaDaGrade, fluxoDesejado,
				     &velMin, &velMedia,&velMax,
				     &margEsqMin,&margEsqMedia,&margEsqMax,
				     &margDirMin,&margDirMedia,&margDirMax);

  for(j = 0; j < 49; j++){
    indice = (indice - 1+alturaDaGrade) % alturaDaGrade;
    
    criaProximoFrame(grade, alturaDaGrade, larguraDoRio, limiteMargens, fluxoDesejado, indice, dIlha, pIlha);
    
    linhasCorretas += calculaVariacoes(grade, larguraDoRio, alturaDaGrade, fluxoDesejado,
				       &velMin, &velMedia, &velMax,
				       &margEsqMin, &margEsqMedia, &margEsqMax,
				       &margDirMin, &margDirMedia, &margDirMax);
    
  }

  margDirMedia /= 50;
  margEsqMedia /= 50;
  velMedia /= 50;

  printf("\n\nPara uma largura de %d, com fluxo Desejado %d, prob. de ilha %f e uma dist. minima entre "
	 " ilhas de %d, em um grid de altura %d, cada margem  ocupando no maximo %f do grid temos:\n\n"
	 "Houve %d linhas com fluxo correto.\n\n"
	 "Velocidade Maxima = %f\nVelocidade Minima = %f\nVelocidade Media = %f\n\n"
	 "Comprimento maximo da margem direita = %d\nComprimento minimo da margem direita = %d\nComprimento medio da margem direita = %f\n\n"
	 "Comprimento maximo da margem esquerda = %d\nComprimento minimo da margem esquerda = %d\nComprimento medio da margem esquerda = %f\n\n",
	 larguraDoRio, fluxoDesejado, pIlha, dIlha, alturaDaGrade, limiteMargens, linhasCorretas, velMax, velMin, velMedia, margDirMax, margDirMin, margDirMedia,
	 margEsqMax, margEsqMin, margEsqMedia);
  
  velMin = 100000000; velMedia = 0; velMax = 0;
  margEsqMin = 10000000; margEsqMedia = 0; margEsqMax = 0;
  margDirMin = 10000000; margDirMedia = 0; margDirMax = 0;
  
    
  return 0;
}
