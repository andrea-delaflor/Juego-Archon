#include "Mundo.h"
#include "glut.h"
#include <math.h>

// Objeto global y variable de luz
Mundo mundo;
float valorLuz = 0.5f;
float angulo = 0.0f;

// Funciones de enlace para GLUT
void glueDibuja() { mundo.dibuja(); }
void glueTimer(int v) {
	mundo.mueve();
	glutPostRedisplay();
	glutTimerFunc(50, glueTimer, 0); // Se llama cada 50ms
}

void Mundo::mueve() {
	angulo += 0.05f; // Velocidad del ciclo de luz
	// El seno oscila entre -1 y 1, lo pasamos a 0 y 1
	valorLuz = (sin(angulo) + 1.0f) / 2.0f;
}

void Mundo::dibuja() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-6, 6, -6, 6); // Cámara ajustada al tablero de 9x9
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	tablero.dibuja(valorLuz);

	glutSwapBuffers();
}

void Mundo::inicializa() {}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Archon ETSIDI - Ciclo de Luz");

	glutDisplayFunc(glueDibuja);
	glutTimerFunc(50, glueTimer, 0);

	mundo.inicializa();
	glutMainLoop();
	return 0;
}