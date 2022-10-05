#include <GL/freeglut.h>
#include "game.h"
#include <stdio.h>
#include <windows.h>  
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "MMSystem.h"
#include <math.h>

Vector2 Dir;
Vector2 posicaoCobra[MAXLENGTH] = { Vector2(0, 0, 1) };

bool AlreadyUpdate;
Color CobraColor = Color(1, 0, 0, 1); // cor amarela
Color WallColor = Color(.5f, .5f, .5f, 1); // cor azul
Color ComidaColor = Color(0, 1, 1, 1); // cor branca

Color Blue = Color(.1f, .1f, .2f, 1); //azul escuro portal

Color GreenCampo = Color(.0f, .0f, .0f, 1); //cor campo
Color Laranja = Color(.6f, .4f, .0f, 1);
Color verdeEscuro = Color(.4f, .0f, .0f, 1);// verde escuro para corpo da cobra

int CobraSize = 1;

float cam_x;
float cam_z;
float z_offset = 0.01f; // ajuda na movimentacao da camera
float x_offset = 0.01f;// ajuda na movimentacao da camera

int Points;

bool ComidaSpawned = false;
Vector2 ComidaPos;
float ComidaAngle;
float ComidaSpawnDelay;



void desenharCubo(float x, float y, float z, float w, float h, float d, float size, Color color) { //funcao base para criacao da estrutura geometrica do game, que no nosso caso eh um cubo
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor4f(color.r, color.g, color.b, color.a);
    glScalef(w, h, d);
    glutSolidCube(size);
    glPopMatrix();
}

void desenharArena()
{
    glPushMatrix();


    glRotatef(90, 0, 0, 1);
    desenharCubo(0, ARENA_MAX_SIZE, 0, ARENA_MAX_SIZE * 2, 1, 2, 1, WallColor);
    glRotatef(180, 0, 0, 1);    
    desenharCubo(0, ARENA_MAX_SIZE, 0, ARENA_MAX_SIZE * 2, 1, 2, 1, WallColor);
    glPopMatrix();

    glPushMatrix();

    for (int x = -ARENA_MAX_SIZE; x < ARENA_MAX_SIZE; x++) {
        for (int y = -ARENA_MAX_SIZE; y < ARENA_MAX_SIZE; y++) {
            if ((x > -3 && x < 0)||(x<3 && x>0)) {
                desenharCubo(x, y, -1, 1, 1, 1, 1, Laranja);
            }
            desenharCubo(x, y, -1, 1, 1, 1, 1, GreenCampo); // reinderizar o campo e a cor 
        }
    }
}

void desenharCobra() {
    glPushMatrix();

    for (int i = 0; i < CobraSize; i++) {
        desenharCubo(CobraPos[i].x, CobraPos[i].y, 0, 0.8, 0.8, .5f, 1, CobraColor);
    }

    glPopMatrix();

 
    checkTailCollision();

}

void setCobraInput(Vector2 dir) { //atualiza a posição da cobra
    Dir = dir;
    CobraPos[0].move = dir.move;
}

void lidarComComida() { //Controla as informações da comida, como o respawn, colisão, rotação...
    if (!ComidaSpawned) {
        if (ComidaSpawnDelay <= 0) {
            ComidaPos = getRandomPosInArena();
            desenharCubo(ComidaPos.x, ComidaPos.y, 0, 1, 1, 1, .7f, ComidaColor);
            ComidaSpawned = true;
        }
        else
        {
            ComidaSpawnDelay -= DELTATIME;
        }
    }
    else
    {
        detectarColisaoDaComida();
        glPushMatrix();
        glRotatef(ComidaAngle, 1, 1, 0);
        desenharCubo(ComidaPos.x, ComidaPos.y, 0, 1, 1, 1, .7f, ComidaColor);

        ComidaAngle++;
        glPopMatrix();
    }
}

void incrementCobra() { // atualizar o tamanho da cobra
    if (CobraSize < MAXLENGTH) {
        CobraSize++;
    }
}

void detectarColisaoDaComida() { // funcao secundaria que detecta a colisao da cobra com a comida
    if (fabs(CobraPos[0].x - ComidaPos.x) < FOODCOLLECTOFFSET && fabs(CobraPos[0].x - ComidaPos.x) > -FOODCOLLECTOFFSET) {
        if (fabs(CobraPos[0].y - ComidaPos.y) < FOODCOLLECTOFFSET && fabs(CobraPos[0].y - ComidaPos.y) > -FOODCOLLECTOFFSET) {
            Points++;
            incrementCobra();
            ComidaSpawnDelay = FOODSPAWNDELAY;
            ComidaSpawned = false;
            PlaySound(TEXT("comeu.wav"),NULL,NULL);
            
        }
    }
}

Vector2 getRandomPosInArena() { //funcao secundaria que serve pra achar um pixel aleatorio na tela
    Vector2 pos;

    srand(time(NULL));
    pos.x = rand() % ((ARENA_MAX_SIZE - 1) + 1 - (-ARENA_MAX_SIZE + 1)) + (-ARENA_MAX_SIZE + 1);
    srand(time(NULL));
    pos.y = rand() % ((ARENA_MAX_SIZE - 1) + 1 - (-ARENA_MAX_SIZE + 1)) + (-ARENA_MAX_SIZE + 1);

    return pos;
}

void gameOver(char text[]) { //Finaliza o jogo e imprime uma mensagem na tela
    char buffer[33];
    _itoa_s(Points, buffer, 10);
    MessageBoxA(NULL, text, "Você perdeu", 0);
    MessageBoxA(NULL, buffer, "Pontos", 0);

    exit(0);
    
}


void checkOutOfBounds() { //checa as colisoes das paredes.. criado para fazer os gols, e adicionar colisao nas bordas do campo


    //colisões cima e baixo
    
    if (CobraPos[0].y >= ARENA_MAX_SIZE) {
        CobraPos[0].y = -ARENA_MAX_SIZE+1;
    }

    if (CobraPos[0].y <= -ARENA_MAX_SIZE) {
        CobraPos[0].y = ARENA_MAX_SIZE-1;
    }
    
    //colisões laterais
    if (CobraPos[0].x >= ARENA_MAX_SIZE && abs((long)CobraPos[0].y) < ARENA_MAX_SIZE / 3) {
        gameOver((char*)"Eita caiu da ponte!");
    }
    else if (CobraPos[0].x >= ARENA_MAX_SIZE && abs((long)CobraPos[0].y) >= ARENA_MAX_SIZE / 3) {
        gameOver((char*)"Eita caiu da ponte!");
    }

    if (CobraPos[0].x <= -ARENA_MAX_SIZE  && abs((long)CobraPos[0].y) < ARENA_MAX_SIZE / 3) {
        gameOver((char*)"Eita caiu da ponte!");
    }
    else if (CobraPos[0].x <= -ARENA_MAX_SIZE && abs((long)CobraPos[0].y) >= ARENA_MAX_SIZE / 3) {
        gameOver((char*)"Eita caiu da ponte!");
    }

}

void checkTailCollision() {   //checa se a cabeca da cobra bate na calda
    for (int i = CobraSize - 1; i > 0; i--) {
        if ((int)CobraPos[0].x == (int)CobraPos[i].x && (int)CobraPos[0].y == (int)CobraPos[i].y) {
            gameOver((char*)"Tropeçando em si mesmo fica difícil!");
        }
    }
}


void moveCobra() { // serve para efeito cascata. ou seja, todas as partes da cobra passarao pelo pixel que a cabeca


    for (int i = CobraSize - 1; i > 0; i--) {
        CobraPos[i].x = CobraPos[i - 1].x;
        CobraPos[i].y = CobraPos[i - 1].y;
        CobraPos[i].move = CobraPos[i - 1].move;

    }

    CobraPos[0].x = round(CobraPos[0].x + (Dir.x));
    CobraPos[0].y = round(CobraPos[0].y + (Dir.y));

    checkOutOfBounds();


}