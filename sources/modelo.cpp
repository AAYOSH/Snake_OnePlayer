




//#include "../headers/model.hpp"
#include "../headers/modelo.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <ncurses.h>
#include<time.h> // para gerar comidas aleatorias
#include<stdlib.h>
#include "portaudio.h"
#include "../headers/01-playback.hpp"

using namespace std::chrono;

/////////////////*******classe choque********************//////////////////////
// montador
Choque::Choque(ListaDeCorpos *lista){
      this->lista = lista;
}
// verifica colisao da cabeca ou com os extremos ou com proprio corpo, retorna verdadeiro caso haja colisao
bool Choque::colisao(ListaDeCorpos *lista){
      int pos_x,pos_y;
      int max_x,max_y;
      std::vector<Cobra_corpo *> *c = this->lista->get_corpos();
      
      getmaxyx(stdscr,max_y,max_x); // obtenho extremos da minha tela.
      
      // obtenho posicao da cabeca
      pos_x = (*c)[0]->get_posicaoX();
      pos_y = (*c)[0]->get_posicaoY();
      
      // colisao com os extremos
      if(pos_x >= max_x || pos_x < 0) 
	    return true;
      if(pos_y >= max_y|| pos_y < 0)
	    return true;
      
      // agora avalio caso haja alguma colisao contra o proprio corpo
      for(int i = 1; i < (*c).size(); i++){  
	    if(pos_x == (*c)[i]->get_posicaoX() && pos_y == (*c)[i]->get_posicaoY())
		  return true;
      }
      
      
      return false;
      
}

////////******************** classe movimento****************////////////////
// construtor
Movimento::Movimento(ListaDeCorpos *l, Comida *food){
      this->food = food;
      this->lista  = l;
  
}
// atualiza as posicoes de cada parte da cobra, retorna 1 caso comeu a comida
int Movimento::update(void){
      std::vector<Cobra_corpo *> *c = this->lista->get_corpos();
      
      // aqui faco o movimento para a cabeca, separado pois é a unica que possui velocidade
      int velx = (*c)[0]->get_velocidadeX();// obtem velocidade
      int vely = (*c)[0]->get_velocidadeY();
      (*c)[0]->update_old((*c)[0]->get_posicaoX(),(*c)[0]->get_posicaoY()); // guarda posicao antiga antes de atualizar
      int new_posx = (*c)[0]->get_posicaoX() + velx; //significa in/decrementar 1;
      int new_posy = (*c)[0]->get_posicaoY() + vely;
      
      // aqui eu evito movimentos nao permitidos,ex: a cobra vai para direita e jogador pressiona tecla esquerda -> IMPOSSIVEL caso ela possua tamanho maior que 1
      if((*c).size() > 1 ){
	    // só faco a comparacao com o corpo logo atras da cabeca, que ja é suficiente
	    int pos_x = (*c)[1]->get_posicaoX();
	    int pos_y = (*c)[1]->get_posicaoY();
      
	    if(new_posx == pos_x){ // caso a nova posicao da cobra seja igual a da parte anterior
		  new_posx = pos_x - 2*velx; 
	    }
	    if(new_posy == pos_y)
		  new_posy = pos_y - 2*vely;
	    }
	    
      (*c)[0]->update(new_posx,new_posy); // atualizado finalmente posicao da cabeca
      
      // faco o restante dos corpos seguirem a cabeca
      for(int i = 1; i < (*c).size(); i++){  
	    (*c)[i]->update_old((*c)[i]->get_posicaoX(),(*c)[i]->get_posicaoY());
	    int new_posx = (*c)[i-1]->get_oldX();
	    int new_posy = (*c)[i-1]->get_oldY();
	    (*c)[i]->update(new_posx,new_posy);
	    
      }
   
      
      int foodx = food->getcx();
      int foody = food->getcy();
	// verificando caso a cobra pegue a comida, no caso somente a cabeca
      if( foodx == (*c)[0]->get_posicaoY() && foody == (*c)[0]->get_posicaoX() ){
	    int aux = (*c).size();
	    mvprintw(foodx,foody ," ");
	    food->update();
	    Cobra_corpo *nova = new Cobra_corpo( 1, 0,(*c)[aux-1]->get_oldX(), (*c)[aux-1]->get_oldY() ); // adiciona nova parte ao corpo da cobra
	    this->lista->add_corpo(nova);
	    return 1;
      }
      return 0;
}
     
/////*************************** CLASS Comida **********************88//////////////////
// construtor
Comida::Comida(){
      int max_x,max_y;
      getmaxyx(stdscr,max_x,max_y); // obtenho extremos da minha tela.
      
      srand(time(NULL)); // gero a comida em posicoa aleatoria
      this->posicaoY =  random()%(max_y) +10 ;
      this->posicaoX = random()%(max_x) +10 ;
      
}

// funcao que da update na posicao da comida apos ela ser consumida
void Comida::update(){
      
      int max_x,max_y;
      getmaxyx(stdscr,max_x,max_y); // obtenho extremos da minha tela.
      
      
      srand(time(NULL)); // posiciona comida em novo lugar random
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
// construtor
ListaDeCorpos::ListaDeCorpos() {
      this->corpos = new std::vector<Cobra_corpo *>(0);
}


void ListaDeCorpos::add_corpo(Cobra_corpo *c) {
      (this->corpos)->push_back(c);
}

std::vector<Cobra_corpo *> *ListaDeCorpos::get_corpos() {
      return (this->corpos);
}

////*****************************CLASS Cobra_corpo******************************////////////////////

Cobra_corpo::Cobra_corpo( int velocidadex, int velocidadey, int posicaoX, int posicaoY){
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

//construtor
Tela::Tela(ListaDeCorpos *ldc, Comida *food) {
      this->lista = ldc;
      this->food = food;
      
}
void Tela::update() {
      int foodx,foody;
      std::vector<Cobra_corpo *> *corpos = this->lista->get_corpos();
      // laco que atualiza os corpos na tela
      for (int k=0; k<corpos->size(); k++)
      {		
	    int xo = (*corpos)[k]->get_oldX();
	    int yo = (*corpos)[k]->get_oldY();
	    move(yo, xo);   /* Move cursor to position */
	    echochar(' ');  /* Prints character, advances a position */
	    
	    
	    int x = (*corpos)[k]->get_posicaoX();
	    int y = (*corpos)[k]->get_posicaoY();
	    mvprintw(y, x,"s"); // similar às duas instrucoes acima
	    
	    
      }
      // printa comida
      foodx = food->getcx();
      foody = food->getcy();
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






















