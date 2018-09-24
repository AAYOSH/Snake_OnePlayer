




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
Choque::Choque(ListaDeCorpos *lista){
      this->lista = lista;
}

bool Choque::colisao(ListaDeCorpos *lista){// caso a cobra se choque com alguma das extremidades
      int pos_x,pos_y;
      int max_x,max_y;
      std::vector<Cobra_corpo *> *c = this->lista->get_corpos();
      getmaxyx(stdscr,max_y,max_x); // obtenho extremos da minha tela.
      
      // obtenho posicao da cabeca
      pos_x = (*c)[0]->get_posicaoX();
      pos_y = (*c)[0]->get_posicaoY();
      
      
      if(pos_x >= max_x || pos_x < 0)
	    return true;
      if(pos_y >= max_y|| pos_y < 0)
	    return true;
      
      // agora avalio caso haja alguma colisao contra o proprio corpo
      
      for(int i = 1; i < (*c).size(); i++){  // comeca em 1 pois cabeca possui a posicoa determinada pela velocid.
	    if(pos_x == (*c)[i]->get_posicaoX() && pos_y == (*c)[i]->get_posicaoY())
		  return true;
      }
      
      
      return false;
      
}

////////******************** classe movimento****************////////////////
Movimento::Movimento(ListaDeCorpos *l, Comida *food){
      this->food = food;
      this->lista  = l;
}

void Movimento::update(float deltaT){
      std::vector<Cobra_corpo *> *c = this->lista->get_corpos();
      
      
      // aqui faco o movimento para a cabeca
      int velx = (*c)[0]->get_velocidadeX();
      int vely = (*c)[0]->get_velocidadeY();
      (*c)[0]->update_old((*c)[0]->get_posicaoX(),(*c)[0]->get_posicaoY()); // guarda posicao antiga antes de atualizar
      int new_posx = (*c)[0]->get_posicaoX() + velx; //significa in/decrementar 1;
      int new_posy = (*c)[0]->get_posicaoY() + vely;
      if((*c).size() > 1 ){
	    int pos_x = (*c)[1]->get_posicaoX();
	    int pos_y = (*c)[1]->get_posicaoY();
      
	    mvprintw(20, 1,"new_posx: %d   pos_x: %d" ,new_posx,pos_x);
	    if(new_posx == pos_x){
		  new_posx = pos_x - 2*velx;
		  mvprintw(1, 1,"velx = %d",velx);
	    }
	    if(new_posy == pos_y)
		  new_posy = pos_y - 2*vely;
      
	    }
      (*c)[0]->update(new_posx,new_posy);  
      for(int i = 1; i < (*c).size(); i++){  // comeca em 1 pois cabeca possui a posicoa determinada pela velocid.
	    (*c)[i]->update_old((*c)[i]->get_posicaoX(),(*c)[i]->get_posicaoY());
	    int new_posx = (*c)[i-1]->get_oldX();
	    int new_posy = (*c)[i-1]->get_oldY();
	    (*c)[i]->update(new_posx,new_posy);
	    
      }
   
      
      int foodx = food->getcx();
      int foody = food->getcy();
	// verificando caso a cabeca bata, adicionando novo corpo
      if( foodx == (*c)[0]->get_posicaoY() && foody == (*c)[0]->get_posicaoX() ){
	    int aux = (*c).size();
	    mvprintw(foodx,foody ," ");
	    food->update();
	    Cobra_corpo *nova = new Cobra_corpo(1, 1, 0,(*c)[aux-1]->get_oldX(), (*c)[aux-1]->get_oldY() );
	    this->lista->add_corpo(nova);
	    aux = (*c).size();
	    score++;
//	    mvprintw(1, 1,"Score: %d" ,score);
      }
}
     
/////*************************** CLASS Comida **********************88//////////////////

Comida::Comida(){
      int max_x,max_y;
      getmaxyx(stdscr,max_x,max_y); // obtenho extremos da minha tela.
      
      
      srand(time(NULL));
      this->posicaoY =  random()%(max_y) +10 ;
      this->posicaoX = random()%(max_x) +10 ;
      
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

/////***************** Class Lista de Corpos*********************//////////////////////
ListaDeCorpos::ListaDeCorpos() {
      this->corpos = new std::vector<Cobra_corpo *>(0);
      this->comprimento = 0;
}

void ListaDeCorpos::hard_copy(ListaDeCorpos *ldc) {
      std::vector<Cobra_corpo *> *corpos = ldc->get_corpos();
      
      for (int k=0; k<corpos->size(); k++) {
	    Cobra_corpo *c = new Cobra_corpo( (*corpos)[k]->get_comprimento(),\
	    (*corpos)[k]->get_velocidadeX(),\
	    (*corpos)[k]->get_velocidadeY(),\
	    (*corpos)[k]->get_posicaoX(),\
	    (*corpos)[k]->get_posicaoY()
	    );
	    this->add_corpo(c);
      }
      
}


void ListaDeCorpos::add_corpo(Cobra_corpo *c) {
      (this->corpos)->push_back(c);
}

std::vector<Cobra_corpo *> *ListaDeCorpos::get_corpos() {
      return (this->corpos);
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
Tela::Tela(ListaDeCorpos *ldc, Comida *food) {
      this->lista = ldc;
      this->food = food;
      this->lista_anterior = new ListaDeCorpos();
      this->lista_anterior->hard_copy(this->lista);
      
}

void Tela::update() {
      int foodx,foody;
      // Apaga corpos na tela
     // x e y sao as posicoes da cobra atualizada apos cada movimento
      
      // esse laco faz com que se tenha a sensacao de movimento
      std::vector<Cobra_corpo *> *corpos = this->lista->get_corpos();
      for (int k=0; k<corpos->size(); k++)
      {		
	    int xo = (*corpos)[k]->get_oldX();
	    int yo = (*corpos)[k]->get_oldY();
	    move(yo, xo);   /* Move cursor to position */
	    echochar(' ');  /* Prints character, advances a position */
	    
	    
	    int x = (*corpos)[k]->get_posicaoX();
	    int y = (*corpos)[k]->get_posicaoY();
	    mvprintw(y, x,"s");
	    
	    
      }
      
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






















