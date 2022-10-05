//*********************************************************************
//  Codigo exemplo para aula pratica de Computacao Grafica 
//  Assunto: Rasterizacao e Transformacoes Geometricas
//
//  Autor: Prof. Laurindo de Sousa Britto Neto
//  Editado por: João Victor de Araujo Silva
//*********************************************************************

// Bibliotecas gerais
#include "Input.h"

// Bibliotecas utilizadas pelo OpenGL
#include <GL/freeglut.h>

// Biblioteca com funcoes matematicas
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <math.h>

// Variaveis Globais
bool click1 = false, click2 = false;

double x_1, y_1, x_2, y_2, x_3, y_3, x_4, y_4;

int width = 512, height = 512; //Largura e altura da janela

State state_struct;
// Estrututa de dados para o armazenamento dinamico dos pontos
// selecionados pelos algoritmos de rasterizacao
struct ponto {
    int x;
    int y;
    ponto* prox;
};

// Lista encadeada de pontos
// indica o primeiro elemento da lista
ponto* pontos = NULL;

// Funcao para armazenar um ponto na lista
// Armazena como uma Pilha (empilha)
ponto* pushPonto(int x, int y) {
    ponto* pnt;
    pnt = new ponto;
    pnt->x = x;
    pnt->y = y;
    pnt->prox = pontos;
    pontos = pnt;
    return pnt;
}

// Funcao para desarmazenar um ponto na lista
// Desarmazena como uma Pilha (desempilha)
ponto* popPonto() {
    ponto* pnt;
    pnt = NULL;
    if (pontos != NULL) {
        pnt = pontos->prox;
        delete pontos;
        pontos = pnt;
    }
    return pnt;
}

// Declaracoes forward das funcoes utilizadas
void init(void);
void reshape(int w, int h);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

// Funcao que implementa o Algoritmo Imediato para rasterizacao de retas
void retaImediata(double x1, double y1, double x2, double y2);
// Funcao que percorre a lista de pontos desenhando-os na tela
void drawPontos();

void bresenham(double x1, double x2, double y1, double y2);
void draw_polygon(void);
void flood_fill(double x, double y, Color new_color, Color old_color);
void set_pixel_color(GLint x, GLint y, Color color);
void f_fill(float x, float y);

void retaImediata(double x1, double y1, double x2, double y2);
void draw_circle();
void translate(Direction dir, int pixels);

void breseham_circle(int x_center, int y_center, int radius);

// Funcao Principal do C
int main(int argc, char** argv) {
    printf("esc: sair; l: linha; p: lapis; c:cirulo; q: retangulo; t: triangulo; o: poligono\n");
    printf("f: desenhar poligono; u: translacao cima; h: translacao esq; j: translacao baixo; k: translacao dir;\n");
    printf("m: circunferencia de brenseham.");

    glutInit(&argc, argv); // Passagens de parametro C para o glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Selecao do Modo do Display e do Sistema de cor utilizado
    glutInitWindowSize(width, height);  // Tamanho da janela do OpenGL
    glutInitWindowPosition(100, 100); //Posicao inicial da janela do OpenGL
    glutCreateWindow("Rasterizacao"); // Da nome para uma janela OpenGL
    init(); // Chama funcao init();
    glutReshapeFunc(reshape); //funcao callback para redesenhar a tela
    glutKeyboardFunc(keyboard); //funcao callback do teclado
    glutMouseFunc(mouse); //funcao callback do mouse
    glutDisplayFunc(display); //funcao callback de desenho
    glutMainLoop(); // executa o loop do OpenGL
    return 0; // retorna 0 para o tipo inteiro da funcao main();
}

// Funcao com alguns comandos para a inicializacao do OpenGL;
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
}

void reshape(int w, int h)
{
    // Reinicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Definindo o Viewport para o tamanho da janela
    glViewport(0, 0, w, h);

    width = w;
    height = h;
    glOrtho(0, w, 0, h, -1, 1);

    // muda para o modo GL_MODELVIEW (n�o pretendemos alterar a projec��o
    // quando estivermos a desenhar na tela)
    glMatrixMode(GL_MODELVIEW);
    click1 = true; //Para redesenhar os pixels selecionados
    click2 = true;
}

// Funcao usada na funcao callback para utilizacao das teclas normais do teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) { // key - variavel que possui valor ASCII da tecla precionada
    case 27: // codigo ASCII da tecla ESC
        exit(0); // comando pra finalizacao do programa
        break;
    case 'p': // p
        state_struct.click_state.first = PENCIL;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 0;
        break;
    case 'c': // c
        state_struct.click_state.first = CIRCLE;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 0;
        break;
    case 'q': // q
        state_struct.click_state.first = RECTANGLE;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 1;
        break;
    case 'l': // l
        state_struct.click_state.first = LINE;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 1;
        break;
    case 't': // t
        state_struct.click_state.first = TRIANGLE;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 2;
        break;
    case 'o': // iniciar desenho do poligono
        state_struct.click_state.first = POLYGON;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 0;
        break;
    case 'f': // terminar poligono
        if (state_struct.click_state.first == POLYGON)
            glutPostRedisplay();
        break;
    case 'u': // transladar cima
        state_struct.click_state.first = TRANSLATE;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 0;
        global_dir = UP;
        glutPostRedisplay();
        break;
    case 'h': // transladar esquerda
        state_struct.click_state.first = TRANSLATE;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 0;
        global_dir = LEFT;
        glutPostRedisplay();
        break;
    case 'j': // transladar baixo
        state_struct.click_state.first = TRANSLATE;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 0;
        global_dir = DOWN;
        glutPostRedisplay();
        break;
    case 'k': // transladar direita
        state_struct.click_state.first = TRANSLATE;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 0;
        global_dir = RIGHT;
        glutPostRedisplay();
        break;
    case 'm': // bresenham circle
        state_struct.click_state.first = B_CIRCLE;
        state_struct.click_state.second = 0;
        state_struct.num_of_clicks = 0;
        break;
    }
}

//Funcao usada na funcao callback para a utilizacao do mouse
void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            if (click1) {
                click2 = true;
                x_2 = x;
                y_2 = height - y;
                printf("x2y2(%.0f,%.0f)\n", x_2, y_2);
                glutPostRedisplay();
            }
            else {
                click1 = true;
                x_1 = x;
                y_1 = height - y;
                printf("x1y1(%.0f,%.0f)\n", x_1, y_1);
            }
            if(state_struct.click_state.first == LINE) {
                if (state_struct.click_state.second == 1) {
                    x_2 = x;
                    y_2 = height - y;
                    glutPostRedisplay();
                }
                else {
                    x_1 = x;
                    y_1 = height - y;
                    state_struct.click_state.second++;
                }
            }
            if (state_struct.click_state.first == RECTANGLE) {
                if (state_struct.click_state.second == 1) {
                    x_2 = x;
                    y_2 = height - y;
                    glutPostRedisplay();
                }
                else {
                    x_1 = x;
                    y_1 = height - y;
                    state_struct.click_state.second++;
                }
            }
            if (state_struct.click_state.first == TRIANGLE) {
                if (state_struct.click_state.second == 1) {
                    x_2 = x;
                    y_2 = height - y;
                    state_struct.click_state.second++;
                }
                else if (state_struct.click_state.second == 2) {
                    x_3 = x;
                    y_3 = height - y;
                    glutPostRedisplay();
                }
                else {
                    x_1 = x;
                    y_1 = height - y;
                    state_struct.click_state.second++;
                }
            }
            if (state_struct.click_state.first == PENCIL) {
                x_1 = x;
                y_1 = height - y;
                glutPostRedisplay();
            }

            if (state_struct.click_state.first == CIRCLE) {
                x_1 = x;
                y_1 = height - y;
                glutPostRedisplay();
            }

            if (state_struct.click_state.first == POLYGON) {
                x_1 = x;
                y_1 = height - y;
                Point point;

                point.x = x_1;
                point.y = y_1;

                point_vector.push_back(point);
            }

            if (state_struct.click_state.first == B_CIRCLE) {
                x_1 = x;
                y_1 = height - y;

                glutPostRedisplay();
            }
        }
        break;

        /**/
    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN) {
             glutPostRedisplay();
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN) {
            glutPostRedisplay();
        }
        break;
        /**/
    default:
        break;
    }
}

// Funcao usada na funcao callback para desenhar na tela
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT); //Limpa o buffer de cores e reinicia a matriz
    glLoadIdentity();

    glColor3f(0.0, 0.0, 0.0); // Seleciona a cor default como preto


    if (click1 && click2) {
        retaImediata(x_1, y_1, x_2, y_2);
        drawPontos();
        click1 = false;
        click2 = false;
    }

    glutSwapBuffers(); // manda o OpenGl renderizar as primitivas

}

//Funcao que desenha os pontos contidos em uma lista de pontos
void drawPontos() {
    ponto* pnt;
    pnt = pontos;
    glBegin(GL_POINTS); // Seleciona a primitiva GL_POINTS para desenhar
    while (pnt != NULL) {
        glVertex2i(pnt->x, pnt->y);
        pnt = pnt->prox;
    }
    glEnd();  // indica o fim do desenho
}

void retaImediata(double x1, double y1, double x2, double y2) {
    double m, b, yd, xd;
    double xmin, xmax, ymin, ymax;
    //Armazenando os extremos para desenho
    pontos = pushPonto((int)x1, (int)y1);
    pontos = pushPonto((int)x2, (int)y2);

    if (x2 - x1 != 0) { //Evita a divis�o por zero
        m = (y2 - y1) / (x2 - x1);
        b = y1 - (m * x1);

        if (m >= -1 && m <= 1) { // Verifica se o declive da reta tem tg de -1 a 1, se verdadeira calcula incrementando x
            xmin = (x1 < x2) ? x1 : x2;
            xmax = (x1 > x2) ? x1 : x2;

            for (int x = (int)xmin + 1; x < xmax; x++) {
                yd = (m * x) + b;
                yd = floor(0.5 + yd);
                pontos = pushPonto(x, (int)yd);
            }
        }
        else { // Se menor que -1 ou maior que 1, calcula incrementado os valores de y
            ymin = (y1 < y2) ? y1 : y2;
            ymax = (y1 > y2) ? y1 : y2;

            for (int y = (int)ymin + 1; y < ymax; y++) {
                xd = (y - b) / m;
                xd = floor(0.5 + xd);
                pontos = pushPonto((int)xd, y);
            }
        }

    }
    else { // se x2-x1 == 0, reta perpendicular ao eixo x
        ymin = (y1 < y2) ? y1 : y2;
        ymax = (y1 > y2) ? y1 : y2;
        for (int y = (int)ymin + 1; y < ymax; y++) {
            pontos = pushPonto((int)x1, y);
        }
    }
}

//Bresenham
void bresenham(double x1, double x2, double y1, double y2) {
    pontos = pushPonto((int)x1, (int) y1);
    pontos = pushPonto((int)x2, (int)y2);
    bool dec = 0, sim = 0;
    double dx = x2 - x1;
    double dy = y2 - y1;

    if (dx * dy < 0) {
        y1 = y1 * -1;
        y2 = y2 * -1;
        dy = dy * -1;
        sim = 1;
    }

    if (abs(dx) < abs(dy)) {
        std::swap(x1, y1);
        std::swap(x2, y2);
        std::swap(dx, dy);
        dec = 1;
    }

    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
        dx = dx * -1;
        dy = dy * -1;
    }

    double yi = y1, aux_y, aux_x;
    double d = 2 * dy - dx;
    double inc_e = 2 * dy;
    double inc_ne = 2 * (dy - dx);

    for (int x = x1 + 1; x < x2; x++) {
        if (d <= 0) {
            d = d + inc_e;
        }
        else {
            d = d + inc_ne;
            yi++;
        }
        aux_x = x;
        aux_y = yi;

        if (dec) {
            std::swap(aux_x, aux_y);
        }
        if (sim) {
            aux_y = aux_y - 1;
        }
        pontos = pushPonto((int)aux_x, (int)aux_y);
    }
}

//Bresenham circle
void bresenham_circle(int x_center, int y_center, int radius) {
    int p = 1 - radius;
    int y = radius;

    for (int x = 0; x <= y; x++) {

        pushPonto(x_center + x, y_center + y);
        pushPonto(x_center - x, y_center + y);
        pushPonto(x_center + x, y_center - y);
        pushPonto(x_center - x, y_center - y);

        pushPonto(x_center + y, y_center + x);
        pushPonto(x_center - y, y_center + x);
        pushPonto(x_center + y, y_center - x);
        pushPonto(x_center - y, y_center - x);


        if (p < 0) {
            p = p + 2 * x + 1;
        }
        else {
            y--;
            p = p + 2 * (x - y) + 1;
        }
    }
}

//Circulo
void draw_circle() {
    GLfloat radius = 35;
    GLfloat step = 1 / radius;
    GLfloat x, y;
    for (GLfloat theta = 0; theta <= 360; theta += step) {
        x = x_1 + (radius * cos(theta));
        y = y_1 + (radius * sin(theta));
        pushPonto((int)x, (int)y);
        glVertex2i(x, y);
    }
    drawPontos();
}

//transladar
void translate(Direction dir, int pixels) {
    ponto* pnt;
    pnt = pontos;
    int dx = 0, dy = 0, x, y;
    switch (dir) {
    case UP:
        dx = 0;
        dy = 10;
        break;

    case DOWN:
        dx = 0;
        dy = -10;
        break;

    case LEFT:
        dx = -10;
        dy = 0;
        break;

    case RIGHT:
        dx = 10;
        dy = 0;
        break;
    }
    while (pnt != NULL) {
        x = pnt->x + dx;
        y = pnt->y + dy;
        pushPonto(x, y);
        pnt->x = 0;
        pnt->y = 0;
        pnt = pnt->prox;
        free(pnt);
    }
}

//Polígono
void draw_polygon() {
    for (auto it = point_vector.begin(); it != point_vector.end(); ++it) {
        if (std::next(it) == point_vector.end()) {
            Point next_point = point_vector.front();
            Point cur_point = * it;

            bresenham(cur_point.x, next_point.x, cur_point.y, next_point.y);
        }else {
            Point cur_point = * it;
            Point next_point = * std::next(it);
            bresenham(cur_point.x, next_point.x, cur_point.y, next_point.y);
        }
    }
    point_vector.clear();
    drawPontos();
}

