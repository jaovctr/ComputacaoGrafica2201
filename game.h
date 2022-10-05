#define GAME_H_INCLUDED

#define MAX_X 1000
#define MAX_Y 1000
#define MAXLENGTH  50
#define ARENA_MAX_SIZE 15
#define ESC 27 // sair do game
#define FPS 15
//#define SNAKESPEED 1
#define DELTATIME 1.0f/FPS 
#define FOODCOLLECTOFFSET 0.5f 
#define FOODSPAWNDELAY 0

extern bool AlreadyUpdate;
extern float cam_x;
extern float cam_z;
struct Color
{
	float r;
	float g;
	float b;
	float a;
	Color(float red, float green, float blue, float alfa){
		r = red;
		g = green;
		b = blue;
		a = alfa;
	}
};

struct Vector2 //Serve para mudar posição e velocidade do objeto
{
	float x;
	float y;
	int move;
	
	void Set(float _x, float _y, int _move){
		x = _x;
		y = _y;
		move = _move;
	}
	
	Vector2(){
		x = 0;
		y = 0;
		move = 1;
	}
	
	Vector2(float _x, float _y, int _move){
		x = _x;
		y = _y;
		move = _move;
	}
};

extern Vector2 CobraPos[MAXLENGTH];

//definicões de métodos a serem usados

void desenharCubo(float x, float y, float z, float w, float h, float d, float size, Color color);
void desenharArena();
void desenharCobra();
void checkTailCollision();
void setCobraInput(Vector2 dir);
void moveCobra();
Vector2 getRandomPosInArena();
void lidarComComida();
void gameOver(char text[]);
void detectarColisaoDaComida();

