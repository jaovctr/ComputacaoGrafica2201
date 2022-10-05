#include <GL/freeglut.h>
#include "Jogo.h"
#include <stdio.h>
#include <windows.h>  
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "MMSystem.h"
#include <math.h>

mudaPosicao Dir;
mudaPosicao posicaoCobra[TAMANHOMAXIMO] = { mudaPosicao(0,0,1) };
bool JaAtualizado;

Cor vermelho = Cor(1, 0, 0, 1);
Cor cinza = Cor(.5f, .5f, .5f, 1);
Cor ciano = Cor(0, 1, 1, 1);
Cor preto = Cor(.0f, .0f, .0f, 1);
Cor laranja = Cor(.6f, .4f, .0f, 1);

int tamanhoCobra = 1;
int pontos;


float cam_x;
float cam_z;
float x_offset = 0.01f;
float z_offset = 0.01f;

bool brotouComida = false;
mudaPosicao posicaoComida;
float anguloComida;
float delayBrotarComida;

void desenharCubo(float x, float y, float z, float w, float h, float d, float size, Cor cor) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor4f(cor.r, cor.g, cor.b, cor.a);
    glScalef(w, h, d);
    glutSolidCube(size);
    glPopMatrix();
}

void desenharPista() {

    glPushMatrix();

    glRotatef(90, 0, 0, 1);
    desenharCubo(0, TAMANHOPISTA, 0, TAMANHOPISTA * 2, 1, 2, 1, cinza);
    glRotatef(180, 0, 0, 1);
    desenharCubo(0, TAMANHOPISTA, 0, TAMANHOPISTA * 2, 1, 2, 1, cinza);
    glPopMatrix();

    glPushMatrix();
}

void desenharCobra() {
    glPushMatrix();

    for (int i = 0; i < CobraSize; i++) {
        desenharCubo(CobraPos[i].x, CobraPos[i].y, 0, 0.8, 0.8, .5f, 1, vermelho);
    }
    glPopMatrix();
    cobraBateu();
}

void atualizaPosicaoCobra(mudaPosicao dir) {
    Dir = dir;
    posicaoCobra[0].move = dir.move;
}

void moveCobra() {
    for (int i = CobraSize - 1; i > 0; i--) {
        posicaoCobra[i].x = posicaoCobra[i - 1].x;
        posicaoCobra[i].y = posicaoCobra[i - 1].y;
        posicaoCobra[i].move = posicaoCobra[i - 1].move;

    }

    posicaoCobra[0].x = round(posicaoCobra[0].x + (Dir.x));
    posicaoCobra[0].y = round(posicaoCobra[0].y + (Dir.y));

    colisaoParede();
}

void aumentaCobra() {
    if (tamanhoCobra < TAMANHOMAXIMO)
        tamanhoCobra++;
}

void comida() {
    if (fabs(posicaoCobra[0].x - posicaoComida.x) < FOODCOLLECTOFFSET 
        && fabs(posicaoCobra[0].x - posicaoComida.x) > -FOODCOLLECTOFFSET
        && fabs(posicaoCobra[0].y - posicaoComida.y) < FOODCOLLECTOFFSET
        && fabs(posicaoCobra[0].y - posicaoComida.y) > -FOODCOLLECTOFFSET) {
        pontos++;
        aumentaCobra();
        delayBrotarComida = FOODSPAWNDELAY;
        brotouComida = false;
        PlaySound(TEXT("comeu.wav"), NULL, NULL);        
    }
}

void comeu() {
    if (fabs(posicaoCobra[0].x - posicaoComida.x) < FOODCOLLECTOFFSET 
        && fabs(posicaoCobra[0].x - posicaoComida.x) > -FOODCOLLECTOFFSET
        && fabs(posicaoCobra[0].y - posicaoComida.y) < FOODCOLLECTOFFSET
        && fabs(posicaoCobra[0].y - posicaoComida.y) > -FOODCOLLECTOFFSET) {
            Points++;
            incrementCobra();
            ComidaSpawnDelay = FOODSPAWNDELAY;
            ComidaSpawned = false;
            PlaySound(TEXT("comeu.wav"), NULL, NULL);   
    }
}

mudaPosicao posicaoAleatoria() {
    mudaPosicao posicao;

    srand(time(NULL));
    posicao.x = rand() % ((TAMANHOPISTA - 1) + 1 - (-TAMANHOPISTA + 1)) + (-TAMANHOPISTA + 1);
    srand(time(NULL));
    posicao.y = rand() % ((TAMANHOPISTA - 1) + 1 - (-TAMANHOPISTA + 1)) + (-TAMANHOPISTA + 1);

    return posicao
}

void colisaoParede() {

    //saiu de um lado aparece em outro

    if (CobraPos[0].y >= ARENA_MAX_SIZE) { 
        CobraPos[0].y = -ARENA_MAX_SIZE + 1;
    }

    if (CobraPos[0].y <= -ARENA_MAX_SIZE) {
        CobraPos[0].y = ARENA_MAX_SIZE - 1;
    }

    //paredes da ponte
    if (posicaoCobra[0].x >= TAMANHOPISTA && abs((long)posicaoCobra[0].y) < TAMANHOPISTA / 3) {
        gameOver((char*)"Eita caiu da ponte!");
    }
    else if (posicaoCobra[0].x >= TAMANHOPISTA && abs((long)posicaoCobra[0].y) >= TAMANHOPISTA / 3) {
        gameOver((char*)"Eita caiu da ponte!");
    }

    if (posicaoCobra[0].x <= -TAMANHOPISTA && abs((long)posicaoCobra[0].y) < TAMANHOPISTA / 3) {
        gameOver((char*)"Eita caiu da ponte!");
    }
    else if (posicaoCobra[0].x <= -TAMANHOPISTA && abs((long)posicaoCobra[0].y) >= TAMANHOPISTA / 3) {
        gameOver((char*)"Eita caiu da ponte!");
    }


}

void cobraBateu() {
    for (int i = tamanhoCobra - 1; i > 0; i--) {
        if ((int)posicaoCobra[0].x == (int)posicaoCobra[i].x
            && (int)posicaoCobra[0].y == (int)posicaoCobra[i].y) {

            gameOver((char*)"Tropeçando em si mesmo fica difícil!");
        }
    }
}

void gameOver(char text[]) {
    char buffer[33];

    _itoa_s(Points, buffer, 10);
    MessageBoxA(NULL, text, "Fim de jogo", 0);
    MessageBoxA(NULL, buffer, "Pontos", 0);

    exit(0);
}
