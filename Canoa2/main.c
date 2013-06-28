#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
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
ALLEGRO_EVENT_QUEUE *event_queue = NULL, *fila_contador = NULL;    /* A event queue, usada para manejar eventos */
ALLEGRO_TIMER *timer = NULL, *contador = 0 ;/* O timer do programa */
ALLEGRO_BITMAP *boat = NULL;
ALLEGRO_FONT *fonte = NULL, *fonteScore = NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_SAMPLE *sample = NULL;
int min, seg;
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
BOOL STinitAllegro (int larguraDoRio, int size, int fps);
void pointCounter ();
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
    
  int invulnerabilidade = 60;

  int maxTime = 0;
    
  Vector_2D *velBarco = v_initZero();
    
  int seed = 1;           /* seed pro random */
  int verbose = 0;        /* flag do modo verbose */
  int indice = 0;         /* usado para imprimir a grade */
  pixel **grade;          /* A grade em que se guarda as informacoes sobre o rio */
    
  al_init_font_addon(); 
  al_init_ttf_addon();
    

  /*
    Leitura de argumentos
  */
    
  getArgs(argc, argv, &FPSInicial, &larguraDoRio, &seed, &fluxoDesejado, &verbose, &dIlha, &pIlha, &limiteMargens, &tamPixel);
  corrigeArgs(argc, argv, &FPSInicial, &larguraDoRio, &seed, &fluxoDesejado, &verbose, &dIlha, &pIlha, &limiteMargens, &tamPixel);
    
  if(boatSize > 30) boatSize = 30;
    
  v_setXY(velBarco, 0, (tamPixel*larguraDoRio*0.006 + 2)/1.2);
    
  if (verbose) {
    printf ("\t \t Opcoes disponiveis: \n"
	    "-b = %f  - FPS inicial\n"
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
  al_start_timer(contador);
  al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
  al_set_audio_stream_playing(musica, YES);
  pointCounter ();

    
  outputArray(grade, alturaDaGrade, larguraDoRio, indice, player_x, player_y, tamPixel, boat, angle,min,seg,fonte);
    
  while (!doexit) {
        
    ALLEGRO_EVENT ev;       /* Variável para guardar qualquer evento que aconteça */
    al_wait_for_event(event_queue, &ev);    /* Faz o allegro esperar até que exista um evento na fila */
        
    if (invulnerabilidade > 0) {
      invulnerabilidade --;        
    }
        
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
	al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

      }
      if(key[KEY_DOWN]) {
	player_x -= v_getX(velBarco);
	player_y += v_getY(velBarco);
	al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

      }
      if(key[KEY_LEFT]) {
	int rodou = v_rotate_SC_up(velBarco, sen, cos);
	if (!key[KEY_DOWN] && ! key[KEY_UP]) {
	  player_x += v_getX(velBarco)/2;
	  player_y -= v_getY(velBarco)/2;
	  al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

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
	  al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

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

	if( min*60 + seg > maxTime) maxTime = min*60 + seg;
		
	min = 0;
	seg = 0;
      }
            
      indice = (indice - 1+alturaDaGrade) % alturaDaGrade;    /* Move a grade uma linha para cima */
                        
      /* Popula a grade com as informações do próximo frame */
      criaProximoFrame(grade, alturaDaGrade, larguraDoRio, limiteMargens, fluxoDesejado, indice, dIlha, pIlha);
            
      /* Imprime a grade na tela */
      pointCounter ();
      outputArray(grade, alturaDaGrade, larguraDoRio, indice, player_x, player_y, tamPixel, boat, angle,min,seg,fonte);
            
            
    }
        
  }
    
    
  /*
    Frees
  */
    

  outputHighScore(maxTime, fonteScore, larguraDoRio, alturaDaGrade, tamPixel);
    
    al_destroy_timer(timer);
    timer = NULL;
    
    doexit = NO;
    
    while (!doexit) {
        
        ALLEGRO_EVENT ev;       /* Variável para guardar qualquer evento que aconteça */
        al_wait_for_event(event_queue, &ev);    /* Faz o allegro esperar até que exista um evento na fila */
        
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)     /* Caso o usuário aperte o botão 'x', saímos do programa */
            doexit = YES;
        
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                doexit = YES;
            }
        }
    }
    

  freeOutput(display, event_queue, timer);        /* Dá free em qualquer coisa que o allegro tenha allocado */
    
  freeGrade(grade, alturaDaGrade, larguraDoRio);  /* Dá free na matriz da grade */
    
  return 0;
}



BOOL STinitAllegro (int larguraDoRio, int size, int fps){
    
  if(!al_init()){        /* Inicializa o allegro. Se falhar, imprime o erro e sai. */
    al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    return NO;
  }
  printf ("passou11\n");

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
    
  event_queue = al_create_event_queue();
  if(!event_queue) {
    al_show_native_message_box(display, "Error", "Error", "Failed to create event queue!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    freeOutput();
    return NO;
  }
    
  timer = al_create_timer(1.0/framesPerSecond);
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
    
  fonte = al_load_font("pirulen.ttf", 50, 0);
  fonteScore = al_load_font("pirulen.ttf", 30, 0);
  if (!fonte || !fonteScore)
    {
      fprintf(stderr, "Falha ao carregar fonte.\n");
      freeOutput();
      return NO;
    }
 
  contador = al_create_timer(1.0);
  if (!contador)
    {
      fprintf(stderr, "Falha ao criar timer.\n");
      freeOutput();
      return NO;
    }
 
  fila_contador = al_create_event_queue();
  if (!fila_contador)
    {
      fprintf(stderr, "Falha ao criar fila do contador.\n");
      freeOutput();
      return NO;
    }
    
  if (!al_install_audio()){
    fprintf(stderr, "Falha ao inicializar áudio.\n");
    return NO;
  }

  if (!al_init_acodec_addon()){
    fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
    return NO;
  }		
	
  if (!al_reserve_samples(1)){
    fprintf(stderr, "Falha ao alocar canais de áudio.\n");
    return NO;
  }

  sample = al_load_sample("sfx.wav");
  if (!sample)
    {
      fprintf(stderr, "Falha ao carregar sample.\n");
      return NO;
    }

  musica = al_load_audio_stream("sound.ogg", 4, 1024);
  if (!musica)
    {
      fprintf(stderr, "Falha ao carregar audio.\n");
      return NO;
    }
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(fila_contador, al_get_timer_event_source(contador));
        
  return YES;
}

void freeOutput() { /* Dá free em qualquer coisa que o allegro tenha allocado */
  if (display != NULL)
    al_destroy_display(display);            /* Dá free no display */
  if (event_queue != NULL)
    al_destroy_event_queue(event_queue);    /* Dá free na event queue */
  if (timer != NULL)
    al_destroy_timer(timer);                /* Dá free no timer */
  if (boat != NULL) 
    al_destroy_bitmap(boat);
  if (sample != NULL) 
    al_destroy_sample(sample);
  if (musica != NULL) 
    al_destroy_audio_stream(musica);
}


void pointCounter (){
  if (!al_is_event_queue_empty(fila_contador))
    {
      ALLEGRO_EVENT evento;
      al_wait_for_event(fila_contador, &evento);
 
      if (evento.type == ALLEGRO_EVENT_TIMER)
        {
	  seg++;
	  if (seg == 60)
            {
	      min++;
	      seg = 0;
            }
        }
    }
}


