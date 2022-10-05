#include <GL/freeglut.h>
#include "Jogo.h"
#include <stdio.h>
#include <windows.h>  
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "MMSystem.h"
#include <math.h>

mudaPosicao Dir; //direção
mudaPosicao posicaoCobra[TAMANHOMAXIMO] = { mudaPosicao(0,0,1) }; //cobra

Cor vermelho = Cor(1, 0, 0, 1);
Cor cinza = Cor(.5f, .5f, .5f, 1);
Cor ciano = Cor(0, 1, 1, 1);
Cor preto = Cor(.0f, .0f, .0f, 1);
Cor laranja = Cor(.6f, .4f, .0f, 1);

int tamanhoCobra = 1;
int pontos;


float cam_x;
float cam_y;

bool brotouComida = false; //ja tem comida no campo
mudaPosicao posicaoComida; 
float anguloComida;//comida rodando

void desenharCubo(float x, float y, float z, float w, float h, float d, float size, Cor cor) {//cubo da cobra,mapa e comida

    glPushMatrix();
    glTranslatef(x, y, z);
    glColor4f(cor.r, cor.g, cor.b, cor.a);
    glScalef(w, h, d);
    glutSolidCube(size);
    glPopMatrix();
}

void desenharPista() {

    glPushMatrix();

    //paredes da ponte
    glRotatef(90, 0, 0, 1);
    desenharCubo(0, TAMANHOPISTA, 0, TAMANHOPISTA * 2, 1, 2, 1, cinza);
    glRotatef(180, 0, 0, 1);
    desenharCubo(0, TAMANHOPISTA, 0, TAMANHOPISTA * 2, 1, 2, 1, cinza);
    
    glPopMatrix();
    glPushMatrix();

    //pista
    for (int x = -TAMANHOPISTA; x < TAMANHOPISTA; x++) {
        for (int y = -TAMANHOPISTA; y < TAMANHOPISTA; y++) {
            if ((x > -3 && x < 0) || (x < 3 && x>0)) {
                desenharCubo(x, y, -1, 1, 1, 1, 1, laranja);
            }
            desenharCubo(x, y, -1, 1, 1, 1, 1, preto);
        }
    }
}

void desenharCobra() {
    glPushMatrix();

    for (int i = 0; i < tamanhoCobra; i++) {
        desenharCubo(posicaoCobra[i].x, posicaoCobra[i].y, 0, 0.8, 0.8, .5f, 1, vermelho);
    }
    glPopMatrix();
    cobraBateu();
}

void atualizaPosicaoCobra(mudaPosicao dir) {
    Dir = dir;
    posicaoCobra[0].move = dir.move;

}

void aumentaCobra() {
    if (tamanhoCobra < TAMANHOMAXIMO)
        tamanhoCobra++;
}

void comida() {
    if (!brotouComida) {
        posicaoComida = posicaoAleatoria();
        desenharCubo(posicaoComida.x, posicaoComida.y, 0, 1, 1, 1, .7f, ciano);
        brotouComida = true;
        
    }
    else
    {
        comeu();
        glPushMatrix();
        glRotatef(anguloComida, 1, 1, 0);
        desenharCubo(posicaoComida.x, posicaoComida.y, 0, 1, 1, 1, .7f, ciano);

        anguloComida++;
        glPopMatrix();
    }
}





void comeu() { //verifica se a cobra encostou na comida

    if (fabs(posicaoCobra[0].x - posicaoComida.x) < FOODCOLLECTOFFSET 
        && fabs(posicaoCobra[0].x - posicaoComida.x) > -FOODCOLLECTOFFSET
        && fabs(posicaoCobra[0].y - posicaoComida.y) < FOODCOLLECTOFFSET
        && fabs(posicaoCobra[0].y - posicaoComida.y) > -FOODCOLLECTOFFSET) {
            pontos++;
            aumentaCobra();
            brotouComida = false;
            PlaySound(TEXT("comeu.wav"), NULL, NULL);   
    }
}

mudaPosicao posicaoAleatoria() {//gera uma posição aleatória no mapa

    mudaPosicao posicao;

    srand(time(NULL));
    posicao.x = rand() % ((TAMANHOPISTA - 1) + 1 - (-TAMANHOPISTA + 1)) + (-TAMANHOPISTA + 1);
    srand(time(NULL));
    posicao.y = rand() % ((TAMANHOPISTA - 1) + 1 - (-TAMANHOPISTA + 1)) + (-TAMANHOPISTA + 1);

    return posicao;
}

void colisaoParede() {//configura as colisões

    //saiu de um lado aparece em outro

    if (posicaoCobra[0].y >= TAMANHOPISTA) {

        posicaoCobra[0].y = -TAMANHOPISTA + 1;
    }

    if (posicaoCobra[0].y <= -TAMANHOPISTA) {
        posicaoCobra[0].y = TAMANHOPISTA - 1;
    }

    //paredes da ponte

    if (posicaoCobra[0].x >= TAMANHOPISTA || posicaoCobra[0].x <= -TAMANHOPISTA) {
        gameOver((char*)"Eita caiu da ponte!");
    }

}

void moveCobra() {//movimentação

    for (int i = tamanhoCobra - 1; i > 0; i--) {
        posicaoCobra[i].x = posicaoCobra[i - 1].x;
        posicaoCobra[i].y = posicaoCobra[i - 1].y;
        posicaoCobra[i].move = posicaoCobra[i - 1].move;
    }

    posicaoCobra[0].x = round(posicaoCobra[0].x + (Dir.x));
    posicaoCobra[0].y = round(posicaoCobra[0].y + (Dir.y));

    colisaoParede();
}


void cobraBateu() {// vê se a cobra tropeçou em si mesmo
    for (int i = tamanhoCobra - 1; i > 0; i--) {
        if ((int)posicaoCobra[0].x == (int)posicaoCobra[i].x && 
            (int)posicaoCobra[0].y == (int)posicaoCobra[i].y) {

            gameOver((char*)"Tropeçando em si mesmo fica difícil!");
        }
    }
}

void gameOver(char text[]) {
    char buffer[33];

    _itoa_s(pontos, buffer, 10);
    MessageBoxA(NULL, text, "Fim de jogo", 0);
    MessageBoxA(NULL, buffer, "Pontos", 0);

    exit(0);
}
