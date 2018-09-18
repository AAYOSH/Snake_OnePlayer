




//#include "../headers/model.hpp"
#include "modelo.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <ncurses.h>
#include<time.h> // para gerar comidas aleatorias
#include<stdlib.h>


using namespace std::chrono;

int score = 0;
/////////////////*******classe choque********************//////////////////////
Choque::Choque(Cobra_corpo *cobra){
      this->cobra = cobra;
}

bool Choque::colisao(Cobra_corpo *cobra){// caso a cobra se choque com alguma das extremidades
      int pos_x,pos_y;
      int max_x,max_y;
      getmaxyx(stdscr,max_y,max_x); // obtenho extremos da minha tela.
      
      pos_x = cobra->get_posicaoX();
      pos_y = cobra->get_posicaoY();
      
      
      if(pos_x >= max_x || pos_x < 0)
	    return true;
      if(pos_y >= max_y|| pos_y < 0)
	    return true;
      
      return false;
      
}

////////******************** classe movimento****************////////////////
Movimento::Movimento(Cobra_corpo *cobra, Comida *food){
      this->food = food;
      this->cobra  = cobra;
}

void Movimento::update(float deltaT){
      int velx = cobra->get_velocidadeX();
      int vely = cobra->get_velocidadeY();
      cobra->update_old(cobra->get_posicaoX(),cobra->get_posicaoY()); // guarda posicao antiga antes de atualizar
      float new_posx = cobra->get_posicaoX() + (float)deltaT * velx/1000;
      float new_posy = cobra->get_posicaoY() + (float)deltaT * vely/1000;
      cobra->update(new_posx,new_posy);  
 //     
      int foodx = food->getcx();
      int foody = food->getcy();
      mvprintw(1,1 ,"foodx = %d, foody = %d, pos_x = %d, posy = %d ",foodx,foody,cobra->get_posicaoX(),cobra->get_posicaoY());
      if( foodx == cobra->get_posicaoY() && foody == cobra->get_posicaoX() ){
	    mvprintw(foodx,foody ," ");
	    food->update();
	    //score++;
      }
}
/////*************************** CLASS Comida **********************88//////////////////

Comida::Comida(){
      int max_x,max_y;
      getmaxyx(stdscr,max_x,max_y); // obtenho extremos da minha tela.
      
      
      srand(time(NULL));
      this->posicaoY =  random()%(max_y) ;
      this->posicaoX = random()%(max_x) ;
      
}

// funcao que da update na posicao da comida apos ela ser consumida
void Comida::update(){
      
      int max_x,max_y;
      getmaxyx(stdscr,max_x,max_y); // obtenho extremos da minha tela.
      
      
      srand(time(NULL));
      
      this->posicaoY =  random()%(max_y);
      this->posicaoX = random()%(max_x);
}

int Comida::getcx(){
      return this->posicaoX;
}
int Comida::getcy(){
      return this->posicaoY;
}



////*****************************CLASS Cobra_corpo******************************////////////////////

Cobra_corpo::Cobra_corpo(int comprimento, int velocidadex, int velocidadey, int posicaoX, int posicaoY){
      this->comprimento = comprimento;
      this->velocidadeX = velocidadex;
      this->velocidadeY = velocidadey;
      this->posicaoX = posicaoX;
      this->posicaoY = posicaoY;
}

void Cobra_corpo::update(int nova_posicaoX, int nova_posicaoY) {
      this->posicaoX = nova_posicaoX;
      this->posicaoY = nova_posicaoY;
}

void Cobra_corpo::update_old(int posicaoX, int posicaoY) {
      this->oldX = posicaoX;
      this->oldY = posicaoY;
}

void Cobra_corpo::update_vel(int nova_velX, int nova_velY) {
      this->velocidadeX = nova_velX;
      this->velocidadeY = nova_velY;
}

int Cobra_corpo::get_comprimento() {
      return this->comprimento;
}
int Cobra_corpo::get_velocidadeX() {
      return this->velocidadeX;
}
int Cobra_corpo::get_velocidadeY() {
      return this->velocidadeY;
}
int Cobra_corpo::get_posicaoX() {
      return this->posicaoX;
}

int Cobra_corpo::get_posicaoY() {
      return this->posicaoY;
}
int Cobra_corpo::get_oldX() {
      return this->oldX;
}
int Cobra_corpo::get_oldY() {
      return this->oldY;
}


////************************************* CLASS TELA**********************//////////////////////////////
Tela::Tela(Cobra_corpo *cobra, Comida *food) {
      this->cobra = cobra;
      this->food = food;
      
}

void Tela::update() {
      int x,y;
      int foodx,foody;
      // Apaga corpos na tela
     // x e y sao as posicoes da cobra atualizada apos cada movimento
      
      int xo = cobra->get_oldX();
      int yo = cobra->get_oldY();
      move(yo, xo);   /* Move cursor to position */
      echochar(' ');
      
      x = cobra->get_posicaoX();
      y = cobra->get_posicaoY();
      mvprintw(y, x,"s");   /* mesma instrucao q anterios, porem simplif */
      
      
      foodx = food->getcx();// print n posicao da commida*/
      foody = food->getcy();
   //   mvprintw(1,1,"foodx =%d, foody = %d",foodx,foody);
      mvprintw(foodx,foody,"*");
      // Atualiza tela
      refresh();
      
}



void Tela::init() {
      initscr();			       /* Start curses mode 		*/
      raw();				         /* Line buffering disabled	*/
      curs_set(0);           /* Do not display cursor */
}

void Tela::stop() {
      endwin();
}


Tela::~Tela() {
      this->stop();;
}
/////////////////////////////*****************CLASS TECLADO*******************************//////////////////
// acho que completo tambem
void threadfun (char *keybuffer, int *control)
{
      char c;
      while ((*control) == 1) {
	    c = getch();
	    if (c!=ERR) (*keybuffer) = c;
	    else (*keybuffer) = 0;
	    std::this_thread::sleep_for (std::chrono::milliseconds(10));
      }
      return;
}

Teclado::Teclado() {
}

Teclado::~Teclado() {
}

void Teclado::init() {
      // Inicializa ncurses
      raw();				         /* Line buffering disabled	*/
      keypad(stdscr, TRUE);	 /* We get F1, F2 etc..		*/
      noecho();			         /* Don't echo() while we do getch */
      curs_set(0);           /* Do not display cursor */
      
      this->rodando = 1;
      std::thread newthread(threadfun, &(this->ultima_captura), &(this->rodando));
      (this->kb_thread).swap(newthread);
}

void Teclado::stop() {
      this->rodando = 0;
      (this->kb_thread).join();
}

char Teclado::getchar() {
      char c = this->ultima_captura;
      this->ultima_captura = 0;
      return c;
}






















