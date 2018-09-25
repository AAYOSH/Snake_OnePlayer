/*
 * 
 * Nome : André de Morais Yoshida
 * RA: 166241
 * 
 * Primeiro projeto da disciplina EA872
 * 
 * 
 * 
 * ************SNAKE GAME***************
 */


#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <ncurses.h>
#include "portaudio.h"
#include "../headers/01-playback.hpp"
//#include "../headers/model.hpp"
#include "../headers/modelo.hpp"

using namespace std::chrono;
uint64_t get_now_ms() {
      return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main()
{	
      
      // cria classe comida
      Comida *food = new Comida();
      
      //cria classe snake
      Cobra_corpo *snake = new Cobra_corpo(1, 0, 0, 0);// cria cabeca da cobra
      
      // cria classe lista de corpos vazia 
      ListaDeCorpos *l = new ListaDeCorpos();
      l->add_corpo(snake);// adiciona cabeca na lista de corpos
      
      
      // inicializa sons
      Audio::Sample *asample;
      asample = new Audio::Sample();
      asample->load("Audios/You-Lose-Sound-Effect.dat");
      
      Audio::Sample *asample1;
      asample1 = new Audio::Sample();
      asample1->load("Audios/blip.dat");
      
      Audio::Sample *asample2;
      asample2 = new Audio::Sample();
      asample2->load("Audios/Bite.dat");
      
      Audio::Player *player;
      player = new Audio::Player();
      player->init();
      
      // cria classe Movimento
      Movimento *m = new Movimento(l,food);
      
    
      std::this_thread::sleep_for (std::chrono::milliseconds(1100)); // evitar que erros do som aparecam na tela
      // cria classe tela
      Tela *tela = new Tela(l, food );
      tela->init();
      // cria classe Choque
      Choque *choque = new Choque(l);
      
      // cria classe teclado
      Teclado *teclado = new Teclado();
      teclado->init();
      
      int max_x;
      int max_y;
      getmaxyx(stdscr,max_y,max_x); 
      
      
      // jogo snake_game
      while(1){
	
	    char c = teclado->getchar(); // leitura do teclado em thread separada
	    if (c=='q') 
		  break;
	    if(c == 's'){
		  asample1->set_position(0);
		  snake->update_vel(0,1);
		  player->play(asample1);
	    }
	    if(c == 'w'){
		  snake->update_vel(0,-1);
		  asample1->set_position(0);
		  player->play(asample1);
	    }
	    if(c == 'd'){
		  snake->update_vel(1,0);
		  asample1->set_position(0);
		  player->play(asample1);
	    }
	    if(c == 'a'){
		  snake->update_vel(-1,0);
		  asample1->set_position(0);
		  player->play(asample1);
	    }
	  
	    if(m->update()){// atualizado movimento do corpo
		  asample2->set_position(0);
		  player->play(asample2);
	    }
	    tela->update(); // atualiza tela
	    
	    if(choque->colisao(l)){ // caso haja colisao, programa é encerrado
		  asample->set_position(0);
		  player->play(asample);
		  mvprintw(max_y/2-1,max_x/2-1,"DIGITE QUALQUER TECLA PARA SAIR");
		  refresh();
		  std::this_thread::sleep_for (std::chrono::milliseconds(400));
		  getch();
		  break;
	    }
	    
	    std::this_thread::sleep_for (std::chrono::milliseconds(100));
      }
      
      
      player->stop();
      tela->stop();
      teclado->stop();
      
      return 0;
}