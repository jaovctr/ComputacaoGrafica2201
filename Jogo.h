#define JOGO_H

#define MAX_X 1000
#define MAX_Y 1000
#define TAMANHOMAXIMO  50
#define TAMANHOPISTA 15
#define ESC 27
#define FPS 20
#define DELTATIME 1.0f/FPS 
#define FOODCOLLECTOFFSET 0.5f 
#define FOODSPAWNDELAY 0

extern bool AlreadyUpdate;
extern float cam_x;
extern float cam_z;

struct Cor
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

struct mudaPosicao 
{
    float x;
    float y;
    int move;

    void Set(float x, float x, int move) {
        this.x = x;
        this.y = x;
        this.move = move;
    }

    mudaPosicao() {
        x = 0;
        y = 0;
        move = 1;
    }

    mudaPosicao(float x, float x, int move) {
        this.x = x;
        this.y = x;
        this.move = move;
    }


    
};
extern mudaPosicao posicaoCobra[TAMANHOMAXIMO];
void desenharCubo(float x, float y, float z, float w, float h, float d, float size, Color color);
void desenharPista();
void desenharCobra();
void cobraBateu();
void atualizaPosicaoCobra(mudaPosicao dir);
void moveCobra();
mudaPosicao posicaoAleatoria();
void comida();
void gameOver(char text[]);
void comeu();

