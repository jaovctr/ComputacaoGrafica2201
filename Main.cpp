#include <GL/freeglut.h>
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include "MMSystem.h"
// -----------------------------------
int move = 0;

void display_callback();
void timer_callback(int);
void reshape_callback(int, int);


void keyboard(unsigned char key, int x, int y) { //teclas usadas para movimentacao da cobra e do mapa
    // dir.move  W-1 S-2 A-3 D-4
    if (!AlreadyUpdate) { return; }
    Vector2 dir;
    switch (key) {
    case 'w': // Mover para cima
        if (move == 2) {
            break;
        }
        dir.Set(0, 1, 1);
        move = 1;
        setCobraInput(dir);
        break;
    case 's': // Mover para baixo
        if (move == 1) {
            break;
        }
        dir.Set(0, -1, 2);
        move = 2;
        setCobraInput(dir);
        break;
    case 'a': // Mover para esquerda
        if (move == 4) {
            break;
        }
        dir.Set(-1, 0, 3);
        move = 3;
        setCobraInput(dir);
        break;
    case 'd': // Mover para direita
        if (move == 3) {
            break;
        }
        dir.Set(1, 0, 4);
        move = 4;
        setCobraInput(dir);
        break;
    case ESC:
        exit(0);
        break;
    case 'i':
        cam_z += 1;
        break;
    case 'k':
        cam_z -= 1;
        break;
    case 'l':
        cam_x += 1;
        break;
    case 'j':
        cam_x -= 1;
        break;

    default:
        break;
    }
    AlreadyUpdate = false;
}

void init()
{

    glClearColor(1, 1, 1, 1.0);
    glEnable(GL_DEPTH_TEST);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);


}

int main(int argc, char** argv)
{
    cam_z = -20;
   
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

void timer_callback(int) {
    glutPostRedisplay();
    AlreadyUpdate = true;
    glutTimerFunc(1000 / FPS, timer_callback, 0);
}


void display_callback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // carrega a matriz identidade

    reshape_callback(MAX_X, MAX_Y);

    desenharArena();
    moveCobra();
    desenharCobra();

    lidarComComida();

    glutSwapBuffers();
}



void reshape_callback(int w, int h)
{
    float ratio = w / h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100, ratio, 1, 50); //zoom
    gluLookAt(cam_x, cam_z, 20.0, 	// posicao da camera (olho) 
        0.0, 0.0, 0.0, 	// centro da cena
        0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
}

