


#ifndef model_hpp
#define model_hpp

#include <thread>
#include <vector>
// acho que completo
class Cobra_corpo{
      private:
	    int velocidadeX;
	    int velocidadeY;
	    int posicaoX;
	    int posicaoY;
	    int oldX;
	    int oldY;
	    
      public:
	    Cobra_corpo(int velocidadex,int velocidadey, int posicaoX, int posicaoY);
	    void update(int nova_posicaoX, int nova_posicaoY);
	    void update_vel(int nova_velX, int nova_velY);
	    void update_old(int posicaoX, int posicaoY);
	    int get_velocidadeX();
	    int get_velocidadeY();
	    int get_posicaoX();
	    int get_posicaoY();
	    int get_oldX();
	    int get_oldY();
};

// classe que agrega varios Cobra_corpos
class ListaDeCorpos {
private:
      std::vector<Cobra_corpo *> *corpos;
      
public:
      ListaDeCorpos();
      void add_corpo(Cobra_corpo *c);
      std::vector<Cobra_corpo *> *get_corpos();

};

// classe que verifica se a cobra em questao se colidiu
class Choque {
      private:
	    ListaDeCorpos *lista;
      
      public:
	    Choque(ListaDeCorpos *lista);
	    bool colisao(ListaDeCorpos *lista);
      
      
};

// classe que dispoem comida na tela
class Comida{
      private:
	    int posicaoX;
	    int posicaoY;
      public:
	    Comida();
	    int getcx();
	    int getcy();
	    void update();
};

// classe responsavel pelo movimento dos corpos
class Movimento {
      private:
	    ListaDeCorpos *lista;
	    Comida *food;

      public:
	    Movimento(ListaDeCorpos *l, Comida *food);
	    int update(void);
};





void threadfun (char *keybuffer, int *control);

class Teclado{
      private:
	    char ultima_captura;
	    int rodando;
	    std::thread kb_thread;
      public:
	    Teclado();
	    ~Teclado();
	    void stop();
	    void init();
	    char getchar();
};

class Tela{
      private:
	    ListaDeCorpos *lista;
	    Comida *food;
      public:
	    Tela(ListaDeCorpos *ldc,Comida *food);
	    ~Tela();
	    void stop();
	    void init();
	    void update();
};













#endif