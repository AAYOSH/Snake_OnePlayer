


#ifndef model_hpp
#define model_hpp

#include <thread>
// acho que completo
class Cobra_corpo{
      private:
	    int comprimento;
	    int velocidadeX;
	    int velocidadeY;
	    int posicaoX;
	    int posicaoY;
	    int oldX;
	    int oldY;
	    
      public:
	    Cobra_corpo(int comprimento, int velocidadex,int velocidadey, int posicaoX, int posicaoY);
	    void update(int nova_posicaoX, int nova_posicaoY);
	    void update_vel(int nova_velX, int nova_velY);
	    void update_old(int posicaoX, int posicaoY);
	    int get_comprimento();
	    int get_velocidadeX();
	    int get_velocidadeY();
	    int get_posicaoX();
	    int get_posicaoY();
	    int get_oldX();
	    int get_oldY();
};

// classe que verifica se a cobra em questao se colidiu
class Choque {
      private:
	    Cobra_corpo *cobra;
      
      public:
	    Choque(Cobra_corpo *cobra);
	    bool colisao(Cobra_corpo *cobra);
      
      
};

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


class Movimento {
      private:
	    Cobra_corpo *cobra;
	    Comida *food;
      public:
	    Movimento(Cobra_corpo *cobra, Comida *food);
	    void update(float deltaT);
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
// acho que completo
class Tela{
      private:
	    Cobra_corpo *cobra;
	    Comida *food;
      public:
	    Tela(Cobra_corpo *cobra,Comida *food);
	    ~Tela();
	    void stop();
	    void init();
	    void update();
};

namespace Audio{
      
};












#endif