#define JOGO_H

#define MAX_X 900 //escala
#define MAX_Y 900 //escala
#define TAMANHOMAXIMO  100 //tamanho maximo da cobra
#define TAMANHOPISTA 15 //tamanho da pista
#define ESC 27 //sair
#define FPS 20 //fps
#define FOODCOLLECTOFFSET 0.5f //colisao da comida
#define FOODSPAWNDELAY 0 //delay pra comida surgir

extern bool AlreadyUpdate;
extern float cam_x;
extern float cam_y;

struct Cor //estrutura pras cores
{
    float r;
    float g;
    float b;
    float a;
    Cor(float red, float green, float blue, float alfa) {
        r = red;
        g = green;
        b = blue;
        a = alfa;
    }
};

struct mudaPosicao //vetor de posição
{
    float x;
    float y;
    int move;

    void Set(float x1, float y1, int move1) {
        x = x1;
        y = y1;
        move = move1; //evitar que o sentido seja alterado e a cobra volte pra trás
    }

    mudaPosicao() {
        x = 0;
        y = 0;
        move = 1;
    }

    mudaPosicao(float x1, float y1, int move1) {
        x = x1;
        y = x1;
        move = move1;
    }


    
};
extern mudaPosicao posicaoCobra[TAMANHOMAXIMO];
void desenharCubo(float x, float y, float z, float w, float h, float d, float size, Cor cor);
void desenharPista();
void desenharCobra();
void cobraBateu();
void atualizaPosicaoCobra(mudaPosicao dir);
void moveCobra();
mudaPosicao posicaoAleatoria();
void comida();
void gameOver(char text[]);
void comeu();

