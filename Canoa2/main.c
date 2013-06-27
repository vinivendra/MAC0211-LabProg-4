#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_primitives.h>
#include "Output.h"
#include "grade.h"
#include "util.h"
#include "pixel.h"
#include "Vector_2D.h"

/*
 Defines de valores iniciais
 */
#define framesPerSecond 50
#define larguraDoRioInicial 200
#define fluxoDesejadoInicial 125
#define alturaDaGrade 200
#define distanciaEntreIlhasInicial 40
#define probabilidadeDeObstaculosInicial 0.1
#define limiteDasMargens 0.25
#define tamanhoInicial 4
#define rotacao 0.0872664626

/*
 BOOL
 */

#define YES 1
#define NO 0
typedef int BOOL;
/*
 Variáveis do allegro
 */

ALLEGRO_DISPLAY *display = NULL;    /* Display, ou seja, a janela criada pelo allegro */
ALLEGRO_EVENT_QUEUE *event_queue = NULL;    /* A event queue, usada para manejar eventos */
ALLEGRO_TIMER *timer = NULL; /* O timer do programa */
ALLEGRO_BITMAP *boat = NULL;

/*
 Teclas das setas
 */

enum KEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

/*
 Protótipos
 */

void freeOutput();
BOOL STinitAllegro (int larguraDoRio, int size, float velocidadeDoBarco);

/*
 main
 */

int main (int argc, char *argv[]) {
    
    /*
     Declaração de variáveis
     */
    
    float FPSInicial = framesPerSecond;
    int larguraDoRio = larguraDoRioInicial;
    int fluxoDesejado = fluxoDesejadoInicial;
    int dIlha = distanciaEntreIlhasInicial;
    float pIlha = probabilidadeDeObstaculosInicial;
    float limiteMargens = limiteDasMargens;
    int tamPixel = tamanhoInicial;
    
    float sen = sinf(rotacao);
    float cos = cosf(rotacao);
    
    bool doexit = NO;       /* Para saber quando sair do programa */
    
    bool key[4] = {NO, NO, NO, NO};     /* Vetor que guarda se cada tecla está apertada */
    
    int player_x = larguraDoRio*tamPixel/2; /* Posiçao da canoa */
    int player_y = alturaDaGrade*tamPixel - 40;
    int boatSize = tamPixel + larguraDoRio*0.1;
    float angle = 0;
    
    int invulnerabilidade = 100;
    
    Vector_2D *velBarco = v_initZero();
    
    int seed = 1;           /* seed pro random */
    int verbose = 0;        /* flag do modo verbose */
    int indice = 0;         /* usado para imprimir a grade */
    pixel **grade;          /* A grade em que se guarda as informacoes sobre o rio */
    
    /*
     Leitura de argumentos
     */
    
    getArgs(argc, argv, &FPSInicial, &larguraDoRio, &seed, &fluxoDesejado, &verbose, &dIlha, &pIlha, &limiteMargens, &tamPixel);
    corrigeArgs(argc, argv, &FPSInicial, &larguraDoRio, &seed, &fluxoDesejado, &verbose, &dIlha, &pIlha, &limiteMargens, &tamPixel);
    
    if(boatSize > 30) boatSize = 30;
    
    v_setXY(velBarco, 0, (tamPixel*larguraDoRio*0.006 + 2)/1.2);
#warning corrigir a velocidade do barco de acordo
    
    if (verbose) {
        printf ("\t \t Opcoes disponiveis: \n"
                "-b = %f  - Velocidade do barco\n"
                "-l = %d  - Largura do Rio\n"
                "-s = %d  - Semente para o gerador aleatorio\n"
                "-f = %d  - Fluxo da agua\n"
                "-v = %d  - Verbose\n"
                "-pI = %f - Probabilidade de haver obstaculos\n"
                "-dI = %d - Distancia minima entre obstaculos\n"
                "-lM = %f - Limite de tamanho das margens (de 0 a 1)\n"
                "-D = %d - Tamanho de cada pixel\n"
                "Pressione Enter para continuar...\n", FPSInicial, larguraDoRio, seed, fluxoDesejado, verbose, pIlha, dIlha, limiteMargens, tamPixel);
        getchar();
    }
    
    /*
     Seed
     */
    
    if (seed == 0)
        seed = (int)time(NULL);
    
    srand(seed);
    
    /*
     Criação do primeiro frame
     */
    
    
    
    /* Inicializacao da grade */
    grade = initGrade(alturaDaGrade, larguraDoRio);
    
    /* Criação do primeiro frame */
    criaPrimeiroFrame(grade, alturaDaGrade, larguraDoRio, limiteMargens, fluxoDesejado, dIlha, pIlha);
    
    if (!STinitAllegro(larguraDoRio, tamPixel, FPSInicial)){
        exit (-1);
    }
    
    
    /* Registrar quaisquer fontes de eventos */
    
    
    /*
     Frames subsequentes
     */
    
    
    al_start_timer(timer);
    
    outputArray(grade, alturaDaGrade, larguraDoRio, indice, player_x, player_y, tamPixel, boat, angle);
    
    while (!doexit) {
        
        ALLEGRO_EVENT ev;       /* Variável para guardar qualquer evento que aconteça */
        al_wait_for_event(event_queue, &ev);    /* Faz o allegro esperar até que exista um evento na fila */
        
        if (invulnerabilidade > 0)
            invulnerabilidade --;
        
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)     /* Caso o usuário aperte o botão 'x', saímos do programa */
            doexit = YES;
        
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {    /* Se uma tecla foi apertada, guardamos isso no vetor */
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = YES;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = YES;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = YES;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = YES;
                    break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP) {      /* Se uma tecla foi solta, registramos isso também */
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = NO;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = NO;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = NO;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = NO;
                    break;
                case ALLEGRO_KEY_ESCAPE:                /* Permitimos que o usuário aperte ESC para sair */
                    doexit = YES;
                    break;
            }
        }
        
        else if (ev.type == ALLEGRO_EVENT_TIMER && al_is_event_queue_empty(event_queue)) {  /* Se o timer já chegou no próximo frame */
            
            if(key[KEY_UP]) {      /* Se o usuário está apertando alguma tecla, faz a canoa se mexer */
                player_x += v_getX(velBarco);
                player_y -= v_getY(velBarco);
            }
            if(key[KEY_DOWN]) {
                player_x -= v_getX(velBarco);
                player_y += v_getY(velBarco);
            }
            if(key[KEY_LEFT]) {
                int rodou = v_rotate_SC_up(velBarco, sen, cos);
                if (!key[KEY_DOWN] && ! key[KEY_UP]) {
                    player_x += v_getX(velBarco)/2;
                    player_y -= v_getY(velBarco)/2;
                }
                if (rodou)
                    angle -= rotacao;
                else angle = -3.141592653589/2;
            }
            if(key[KEY_RIGHT]) {
                int rodou = v_rotate_SC_up(velBarco, -sen, cos);
                if (!key[KEY_DOWN] && ! key[KEY_UP]) {
                    player_x += v_getX(velBarco)/2;
                    player_y -= v_getY(velBarco)/2;
                }
                if (rodou)
                    angle += rotacao;
                else angle = 3.141592653589/2;
            }
            
            player_y += velocidade(&grade[((int)(player_y/tamPixel) + indice - 1)%alturaDaGrade][(int)(player_x/tamPixel)%(larguraDoRio)]) * tamPixel;;
            
            if (player_y < 50)
                player_y = 50;
            if (player_y > alturaDaGrade * tamPixel - 25)
                player_y = alturaDaGrade * tamPixel - 25;
            if (player_x < 25)
                player_x = 25;
            if (player_x > larguraDoRio * tamPixel - 50)
                player_x = larguraDoRio * tamPixel - 50;
                        
            if (tipo(&grade[((int)((player_y)/tamPixel) + indice - 1)%alturaDaGrade][(int)(player_x/tamPixel)%(larguraDoRio)]) == TERRA && invulnerabilidade == 0) {
                player_x = larguraDoRio*tamPixel/2;
                player_y = alturaDaGrade*tamPixel - 40;
                angle = 0;
                v_setXY(velBarco, 0, (tamPixel*larguraDoRio*0.006 + 2)/1.2);
                invulnerabilidade = 60;
            }
            
            indice = (indice - 1+alturaDaGrade) % alturaDaGrade;    /* Move a grade uma linha para cima */
                        
            /* Popula a grade com as informações do próximo frame */
            criaProximoFrame(grade, alturaDaGrade, larguraDoRio, limiteMargens, fluxoDesejado, indice, dIlha, pIlha);
            
            /* Imprime a grade na tela */
            outputArray(grade, alturaDaGrade, larguraDoRio, indice, player_x, player_y, tamPixel, boat, angle);
            
            
        }
        
    }
    
    
    /*
     Frees
     */
    
    freeOutput(display, event_queue, timer);        /* Dá free em qualquer coisa que o allegro tenha allocado */
    
    freeGrade(grade, alturaDaGrade, larguraDoRio);  /* Dá free na matriz da grade */
    
    return 0;
}



BOOL STinitAllegro (int larguraDoRio, int size, float velocidadeDoBarco){
    
    if(!al_init()){        /* Inicializa o allegro. Se falhar, imprime o erro e sai. */
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return NO;
    }
    
    al_init_primitives_addon();
    
    display = al_create_display(larguraDoRio*size, (alturaDaGrade-1)*size);      /* Cria o display */
    
    if(!display) {          /* Caso haja erro na criação, imprime e sai. */
        al_show_native_message_box(display, "Error", "Error", "Failed to create display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        freeOutput();
        return NO;
    }
    
    if(!al_init_image_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        freeOutput();
        return 0;
    }
    al_init_primitives_addon();
    
    event_queue = al_create_event_queue();
    if(!event_queue) {
        al_show_native_message_box(display, "Error", "Error", "Failed to create event queue!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        freeOutput();
        return NO;
    }
    
    timer = al_create_timer(1.0/velocidadeDoBarco);
    if(!timer) {
        al_show_native_message_box(display, "Error", "Error", "Failed to create timer!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        freeOutput();
        return NO;
    }
    
    if(!al_install_keyboard()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize keyboard!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        freeOutput();
        return NO;
    }
    
    boat = al_load_bitmap("boat.png");
    if(!boat) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize bitmap!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        freeOutput();
        return -1;
    }
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    return YES;
}

void freeOutput() { /* Dá free em qualquer coisa que o allegro tenha allocado */
    if (display != NULL)
        al_destroy_display(display);            /* Dá free no display */
    if (event_queue != NULL)
        al_destroy_event_queue(event_queue);    /* Dá free na event queue */
    if (timer != NULL) {
        al_destroy_timer(timer);                /* Dá free no timer */
    }
    if (boat != NULL) {
        al_destroy_bitmap(boat);
    }
    
}


