//*********************************************************************
//  Modelagem Geométrica 3D usando OpenGL
//  Codigo Exemplo
// Editado por João Victor de Araujo Silva
//*********************************************************************

/* Inclui os headers do OpenGL e Freeglut */

#include <GL/freeglut.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>



/**********************************************************************/
/*                       Declaração de constantes                     */
/**********************************************************************/
float angulo = 0.0;
int mseg = 10;

/* Coordenadas dos vértices do objeto: */
#define N_VERTICES 8
const GLfloat vertices[N_VERTICES][7] = {
    { -0.5,  0.5,  0.5 },
    { -0.5, -0.5,  0.5 },
    {  0.5, -0.5,  0.5 },
    { 0.5,   0.5,  0.5 },
    { 0.5,   0.5, -0.5 },
    {  0.5, -0.5, -0.5 },
    {-0.5,  -0.5, -0.5 },
    { -0.5,  0.5, -0.5 }

};

/* Quadrados do objeto (vértices que os constituem) */
#define N_FACES 12
const GLuint faces[N_FACES][3] = {
    {0,1,2},
    {0,2,3},
    {5,6,7},
    {4,5,7},
    {0,3,5},
    {3,4,5},
    {1,2,6},
    {2,6,7},
    {0,1,5},
    {1,5,6},
    {2,3,4},
    {2,4,7}

      /* ATENÇÃO:                                                           *
       * De acordo com a vista superior, os vértices do triângulo da base   *
       * da pirâmide estão definidos no sentido horário, ao contrário do    *
       * que temos feito até aqui. Isso é porque, segundo a vista superior, *
       * esse triângulo está na realidade sendo visto "de costas".          */





};



/* Vetores normais aos vértices do objeto: */
GLfloat vertex_normals[N_VERTICES][7];

/* Vetores normais aos triângulos do objeto: */
GLfloat face_normals[N_FACES][5];

//Estrutura criada para facilitar o entendimento
struct ponto {
    float x;
    float y;
    float z;
};

void normalizar(ponto* n) {
    GLfloat length;

    length = (GLfloat)sqrt((n->x * n->x) + (n->y * n->y) + (n->z * n->z));

    if (length == 0.0f)
        length = 1.0f;

    n->x = n->x / length;
    n->y = n->y / length;
    n->z = n->z / length;
}

void calcularNormaisFaces() {
    float x[3], y[3], z[3];
    ponto a, b, n;

    for (int i = 0; i < N_FACES; i++) {
        x[0] = vertices[faces[i][0]][0];
        y[0] = vertices[faces[i][0]][1];
        z[0] = vertices[faces[i][0]][2];

        x[1] = vertices[faces[i][1]][0];
        y[1] = vertices[faces[i][1]][1];
        z[1] = vertices[faces[i][1]][2];

        x[2] = vertices[faces[i][2]][0];
        y[2] = vertices[faces[i][2]][1];
        z[2] = vertices[faces[i][2]][2];

        a.x = x[2] - x[0];
        a.y = y[2] - y[0];
        a.z = z[2] - z[0];

        b.x = x[2] - x[1];
        b.y = y[2] - y[1];
        b.z = z[2] - z[1];

        n.x = a.y * b.z - a.z * b.y;
        n.y = a.z * b.x - a.x * b.z;
        n.z = a.x * b.y - a.y * b.x;

        normalizar(&n);

        face_normals[i][0] = n.x;
        face_normals[i][1] = n.y;
        face_normals[i][2] = n.z;
    }
}

void calcularNormaisVertices() {
    ponto n;
    for (unsigned int i = 0; i < N_VERTICES; i++) {
        n.x = 0.0;
        n.y = 0.0;
        n.z = 0.0;
        for (int j = 0; j < N_FACES; j++) {
            if (faces[j][0] == i || faces[j][1] == i || faces[j][2] == i) {
                n.x += face_normals[j][0];
                n.y += face_normals[j][1];
                n.z += face_normals[j][2];
            }
        }

        n.x /= 3.0;
        n.y /= 3.0;
        n.z /= 3.0;

        normalizar(&n);

        vertex_normals[i][0] = n.x;
        vertex_normals[i][1] = n.y;
        vertex_normals[i][2] = n.z;

    }

}


/* Determina se vamos usar Gouraud ou Flat shading */
unsigned char use_gouraud = 1;

/* Determina se vamos usar Z-buffering */
unsigned char use_depth_test = 1;

/* Determina se Liga a Luz */
unsigned char use_light = 1;

/* Determina se vamos habilitar as características do material */
unsigned char use_material = 1;



/**********************************************************************/
/*                  Declaração de funções forward                     */
/**********************************************************************/
void init_glut(const char* window_name, int argc, char** argv);
void draw_object_smooth(void);
void draw_object_flat(void);
void display_callback(void);
void reshape_callback(int w, int h);
void animate_callback(void);
void timer_callback(int value);
void keyboard_callback(unsigned char key, int x, int y);
void keyboard_callback_special(int key, int x, int y);
void info_modotexto();

/**********************************************************************/
/*                                                                    */
/*                       Função principal (main)                      */
/*                                                                    */
/**********************************************************************/

int main(int argc, char** argv) {
    calcularNormaisFaces();
    calcularNormaisVertices();

    info_modotexto();
    /* inicia o GLUT e alguns parâmetros do OpenGL */

    init_glut("Desenhando Objetos em 3D", argc, argv);

    /* função de controlo do GLUT */
    glutMainLoop();

    return 0;
}

/**********************************************************************/
/*                                                                    */
/*                         Definição de funções                       */
/*                                                                    */
/**********************************************************************/


/*
 * inicia o GLUT e alguns parâmetros do OpenGL
 */
void init_glut(const char* nome_janela, int argc, char** argv) {
    /* inicia o GLUT */
    glutInit(&argc, argv);

    /* inicia o display usando RGB e double-buffering */
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(nome_janela);

    /* define as funcões de callback */
    glutKeyboardFunc(keyboard_callback);
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutSpecialFunc(keyboard_callback_special);

    //    glutIdleFunc(animate_callback);
    glutTimerFunc(mseg, timer_callback, mseg);

    /* Inicia a iluminação */
    GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 };
    GLfloat light_color[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    /* Inicia as características gerais dos materiais */
    GLfloat mat_ambient_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient_diffuse);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    /* Ativa o modelo de sombreagem de "Gouraud". */
    glShadeModel(GL_SMOOTH);

    /* Ativa o z-buffering, de modo a remover as superfícies escondidas */
    glEnable(GL_DEPTH_TEST);

    /* define a cor com a qual o ecrã será apagado */
    glClearColor(1.0, 1.0, 1.0, 1.0);

    /* define a cor de desenho inicial (azul) */
    glColor3f(0.0, 0.0, 1.0);
}

/*
 * Processa o reshape da janela
 */
void reshape_callback(int w, int h) {

    /* muda para o modo GL_PROJECTION e reinicia a projecção */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* define a zona da janela onde se vai desenhar */
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    /* Define a forma do "viewing volume" para termos *
     * uma projecção de perspectiva (3D).             */
    gluPerspective(30, (float)w / (float)h, 1.0, 100.0);

    /* Transformação de visão */
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    /* muda para o modo GL_MODELVIEW (não pretendemos alterar a projecção
     * quando estivermos a desenhar no display) */
    glMatrixMode(GL_MODELVIEW);

}

void timer_callback(int value) {
    glutTimerFunc(value, timer_callback, value);
    glutPostRedisplay(); // Manda redesenhar o display em cada frame
}

/*
 * Desenha o objeto, triângulo por triângulo, definindo os vetores
 * normais de cada vértice (ideal para usar com Gouraud shading).
 */
void draw_object_smooth(void) {
    GLuint i;

    /* Desenha todos os triângulos do objeto */
    glBegin(GL_TRIANGLES);
    for (i = 0; i < N_FACES; i++) {
        glNormal3fv(vertex_normals[faces[i][0]]);
        glVertex3fv(vertices[faces[i][0]]);

        glNormal3fv(vertex_normals[faces[i][1]]);
        glVertex3fv(vertices[faces[i][1]]);

        glNormal3fv(vertex_normals[faces[i][2]]);
        glVertex3fv(vertices[faces[i][2]]);
    }
    glEnd();
}

/*
 * Desenha o objeto, triângulo por triângulo, utilizando um vetor
 * normal para cada triângulo (ideal para usar com flat shading).
 */
void draw_object_flat(void) {
    GLuint i;

    /* Desenha todos os triângulos do objeto */

    glBegin(GL_TRIANGLES);
    for (i = 0; i < N_FACES; i++) {
        glNormal3fv(face_normals[i]);
        glVertex3fv(vertices[faces[i][0]]);
        glVertex3fv(vertices[faces[i][1]]);
        glVertex3fv(vertices[faces[i][2]]);
    }
    glEnd();
}

/*
 * função para controlar o display
 */
void display_callback(void) {

    /* Apaga o video e o depth buffer, e reinicia a matriz */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /* faz variar o ângulo de rotação entre 0 e 360 graus, e roda o *
     * referencial segundo o eixo (1, 1, 0).                        */
    angulo++;
    if (angulo > 360.0) angulo -= 360.0;
    glRotatef(angulo, 1.0, 1.0, 0.0);

    /* Desenha o objeto */
    if (use_gouraud) draw_object_smooth();
    else draw_object_flat();

    /* Troca os buffers, mostrando o que acabou de ser desenhado */
    glutSwapBuffers();
}

/*
 * Função necessária para a animação
 */
void animate_callback(void) {
    glutPostRedisplay(); /* Manda redesenhar o display em cada frame */
}


void info_modotexto() {
    puts("MENU DE AJUDA\n");
    puts("F1: Muda a cor para Azul");
    puts("F2: Muda a cor para Verde");
    puts("F3: Muda a cor para Vermelho");
    puts("F4: liga/desliga a luz");
    puts("F5: liga/desliga o gouraud shading");
    puts("F6: liga/desliga o Z-buffering");
    puts("ESC: sai do programa");
    puts("");
    puts("NORMAIS DA FACE\n");
    for (int i = 0; i < N_FACES; i++) {
        printf("Normal Face %d: (%f, %f, %f)\n", i, face_normals[i][0], face_normals[i][1], face_normals[i][2]);
    }
    puts("");
    puts("NORMAIS DO VERTICE\n");
    for (int i = 0; i < N_VERTICES; i++) {
        printf("Normal Vertice %d: (%f, %f, %f)\n", i, vertex_normals[i][0], vertex_normals[i][1], vertex_normals[i][2]);
    }
}

/*
 * Controle das teclas normais
 */
void keyboard_callback(unsigned char key, int x, int y) {
    if (key == 27) exit(0); /* Esc: sai do programa */
}


/*
 * Controle das teclas especiais (Cursores, F1 a F12, etc...)
 */
void keyboard_callback_special(int key, int x, int y) {
    switch (key) {
    case 1:
        glColor3f(0.0, 0.0, 1.0);      /* F1: muda a cor para azul */
        break;

    case 2:
        glColor3f(0.0, 1.0, 0.0);      /* F2: muda a cor para verde */
        break;

    case 3:
        glColor3f(1.0, 0.0, 0.0);      /* F3: muda a cor para vermelho */
        break;

    case 4:                      /* F4: liga/desliga a luz */
        if (use_light == 1) {
            use_light = 0;
            glDisable(GL_LIGHTING);
        }
        else {
            use_light = 1;
            glEnable(GL_LIGHTING);
        }
        break;

    case 5:                      /* F5: liga/desliga o gouraud shading */
        if (use_gouraud == 1) {
            use_gouraud = 0;
            glShadeModel(GL_FLAT);

        }
        else {
            use_gouraud = 1;
            glShadeModel(GL_SMOOTH);
        }
        break;

    case 6:                          /* F6: liga/desliga o Z-buffering */
        if (use_depth_test == 1) {
            use_depth_test = 0;
            glDisable(GL_DEPTH_TEST);
        }
        else {
            use_depth_test = 1;
            glEnable(GL_DEPTH_TEST);
        }
        break;

    }
    glutPostRedisplay();
}


