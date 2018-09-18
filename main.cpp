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
//#include "../headers/model.hpp"
#include "modelo.hpp"

using namespace std::chrono;
uint64_t get_now_ms() {
      return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main()
{	
      
      char derrota[] = "********************PERDEU******************";
      
      Comida *food = new Comida();
      Cobra_corpo *snake = new Cobra_corpo(1, 10, 0, 0, 0);
      
      
      // cria classe Movimento
      Movimento *m = new Movimento(snake,food);

      // cria classe tela
      Tela *tela = new Tela(snake, food );
      tela->init();
      // cria classe Choque
      Choque *choque = new Choque(snake);
      
      // cria classe teclado
      Teclado *teclado = new Teclado();
      teclado->init();
      
      uint64_t t0;
      uint64_t t1;
      uint64_t deltaT;
      uint64_t T;
      T = get_now_ms();
      t1 = T;
      
      
      
      
      // considero o jogo aqui
      while(1){
	    t0 = t1;
	    t1 = get_now_ms();
	    deltaT = t1-t0;
	    
	    char c = teclado->getchar(); // consigo ler do teclado ja
	    if (c=='q') 
		  break;
	    if(c == 's'){
		  snake->update_vel(0,10);
	    }
	    if(c == 'w')
		  snake->update_vel(0,-10);
	    if(c == 'd')
		  snake->update_vel(10,0);
	    if(c == 'a')
		  snake->update_vel(-10,0);
		  
	  
	    m->update(deltaT);
	    tela->update();
	    if(choque->colisao(snake)){
		  mvprintw(15,15,"%s",derrota);
		  printw("TECLE S PARA SAIR");
		  refresh();
		  std::this_thread::sleep_for (std::chrono::milliseconds(1000));
		  getch();
		  break;
	    }
	    std::this_thread::sleep_for (std::chrono::milliseconds(100));
		  
	 //   if ( (t1-T) > 10000 ) break;
      }
      
      
      
      tela->stop();
      teclado->stop();
      
      return 0;
}