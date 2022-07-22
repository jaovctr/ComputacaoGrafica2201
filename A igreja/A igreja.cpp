// João Victor de Araujo Silva

#include<GL/freeglut.h>
#include<math.h>

void torreCentral() {
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 1.0f);
	glVertex2f(-0.4f, -0.8f);
	glVertex2f(0.4f, -0.8f);
	glVertex2f(0.4f, 0.4f);
	glVertex2f(-0.4f, 0.4f);
	glEnd();
}

void torreEsquerda() {
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.5f, 0.5f);
	glVertex2f(-0.4f, -0.8f);
	glVertex2f(-1.0f, -0.8f);
	glVertex2f(-1.0f, 0.2f);
	glVertex2f(-0.4f, 0.2f);
	glEnd();
}

void torreDireita() {
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.5f, 0.5f);
	glVertex2f(0.4f, -0.8f);
	glVertex2f(1.0f, -0.8f);
	glVertex2f(1.0f, 0.2f);
	glVertex2f(0.4f, 0.2f);
	glEnd();
}

void porta() {
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.2f, -0.8f);
	glVertex2f(0.2f, -0.8f);
	glVertex2f(0.2f, -0.3f);
	glVertex2f(-0.2f, -0.3f);
	glEnd();
}

void telhadoEsquerdo() {
	glBegin(GL_TRIANGLES);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex2f(-1.0f, 0.2f);
	glVertex2f(-0.4f, 0.2f);
	glVertex2f(-0.7f, 0.7f);
	
	glEnd();

}

void telhadoCentral() {
	glBegin(GL_TRIANGLES);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex2f(0.4f, 0.4f);
	glVertex2f(-0.4f, 0.4f);
	glVertex2f(0.0f, 0.6f);

	glEnd();

}

void telhadoDireito() {
	glBegin(GL_TRIANGLES);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex2f(1.0f, 0.2f);
	glVertex2f(0.4f, 0.2f);
	glVertex2f(0.7f, 0.7f);

	glEnd();

}

void cruzEsquerda() {
	glBegin(GL_LINES);
	glLineWidth(5.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(-0.7f, 0.7f);
	glVertex2f(-0.7f, 1.0f);
	glVertex2f(-0.2f, 0.9f);
	glVertex2f(0.2f, 0.9f);
	glEnd();
}

void cruzCentral() {
	glBegin(GL_LINES);
	glLineWidth(5.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0.0f,1.0f);
	glVertex2f(0.0f,0.6f);
	glVertex2f(-0.6f, 0.9f);
	glVertex2f(-0.8f, 0.9f);

	glEnd();
}

void cruzDireita() {
	glBegin(GL_LINES);
	glLineWidth(5.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0.7f, 0.7f);
	glVertex2f(0.7f, 1.0f);
	glVertex2f(0.6f, 0.9f);
	glVertex2f(0.8f, 0.9f);
	glEnd();
}

void janelaEsquerda() {
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 1.0f);
	
	for (int i = 0; i <= 20; i++){
		glVertex2f(
			-0.7f + (0.2f * cos(i * (2.0f * 3.14f) / 20)),
			-0.1f + (0.2f *sin(i * (2.0f * 3.14f) / 20))
			);
	}

	glEnd();
}

void janelaDireita() {
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 1.0f);

	for (int i = 0; i <= 20; i++) {
		glVertex2f(
			0.7f + (0.2f * cos(i * (2.0f * 3.14f) / 20)),
			-0.1f + (0.2f * sin(i * (2.0f * 3.14f) / 20))
		);
	}

	glEnd();
}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	torreCentral();
	torreEsquerda();
	torreDireita();
	telhadoEsquerdo();
	telhadoCentral();
	telhadoDireito();
	porta();
	cruzEsquerda();
	cruzCentral();
	cruzDireita();
	janelaEsquerda();
	janelaDireita();

	glFlush();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutCreateWindow("Trabalho 1: Igreja");
	glutInitWindowSize(1366, 768);
	glutInitWindowPosition(50, 50);
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}