#include <GL/freeglut.h>
#include "Jogo.h"
#include <stdio.h>
#include <windows.h>  
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "MMSystem.h"
#include <math.h>

int move = 0;
void display_callback();
void timer_callback(int);
void reshape_callback(int, int);


void keyboard(unsigned char key, int x, int y) {
    
    mudaPosicao dir;
    switch (key)
    {
    case 'w':
        if (move == 2) {
            break;
        }
        dir.Set(0, 1, 1);
        move = 1;
        atualizaPosicaoCobra(dir);
        break;

    case 'a':
        if (move == 4) {
            break;
        }
        dir.Set(-1, 0, 3);
        move = 3;
        atualizaPosicaoCobra(dir);
        break;

    case 's':
        if (move == 1) {
            break;
        }
        dir.Set(0, -1, 2);
        move = 2;
        atualizaPosicaoCobra(dir);
        break;

    case 'd':
        if (move == 3) {
            break;
        }
        dir.Set(1, 0, 4);
        move = 4;
        atualizaPosicaoCobra(dir);
        break;

    case ESC:
        exit(0);
        break;

    default:
        break;
    }
}

void init() {//funções para configurar a cor e iluminação da tela do jogo
    glClearColor(1, 1, 1, 1.0);
    glEnable(GL_DEPTH_TEST); 
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

int main(int argc, char** argv) {//cria a janela do glut
    cam_y = -20;//inclinação do mapa

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(MAX_X, MAX_Y);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) / 2) - MAX_X / 2, (glutGet(GLUT_SCREEN_HEIGHT) / 2) - MAX_Y / 2);
    glutCreateWindow("Cobra na estrada");
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer_callback, 0);
    init();
    glutMainLoop();
    return 0;
}

void timer_callback(int) { //tempo para as coisas serem desenhadas
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer_callback, 0);
}

void display_callback() {//atualizando a tela do jogo

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); 

    reshape_callback(MAX_X, MAX_Y);

    desenharPista();
    moveCobra();
    desenharCobra();
    comida();

    glutSwapBuffers();
}

void reshape_callback(int w, int h) { //redimensionar a tela
    float ratio = w / h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100, ratio, 1, 50);
    gluLookAt(cam_x, cam_y, 20.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
}