#include "Mundo.h"
#include "glut.h"


Mundo mundo;


void glueDibuja() { 
	mundo.dibuja(); 
}

void glueMueve(int v) {
	mundo.mueve();
	glutPostRedisplay();
	glutTimerFunc(20, glueMueve, 0);
}
void glueTeclado(unsigned char t, int x, int y) { mundo.tecla(t); }

void Mundo::inicializa() {
	//CARGAR PIEZAS E IMAGENES
}

void Mundo::mueve() {
	// LOGICA MOVIMIENTOS
}

void Mundo::tecla(unsigned char t) {
	//CONTROLES
}

void Mundo::dibuja() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-12, 12, -12, 12);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	tablero.dibuja();

	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Archon - ETSIDI");

	
	glutDisplayFunc(glueDibuja);
	glutTimerFunc(20, glueMueve, 0);
	glutKeyboardFunc(glueTeclado);

	mundo.inicializa();

	glutMainLoop();
	return 0;
}