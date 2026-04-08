#include "Mundo.h"
#include "glut.h"
#include <math.h>

Mundo mundo;

// Funciones de enlace para GLUT
void glueDibuja() { mundo.dibuja(); }

// Función de temporizador para actualizar el mundo cada 50ms
void glueTimer(int v) {
    mundo.mueve();
    glutPostRedisplay();
    glutTimerFunc(50, glueTimer, 0); // Cada 50ms
}

void Mundo::inicializa() {
    valorLuz = 0.5f;
    angulo = 0.0f;
}

void Mundo::mueve() {
    angulo += 0.05f;
    valorLuz = (sin(angulo) + 1.0f) / 2.0f; // Oscilación entre 0 y 1
}

void Mundo::dibuja() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluOrtho2D(-6, 6, -6, 6); // Ajuste de cámara

    tablero.dibuja(valorLuz);

    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Archon - ETSIDI");

    glutDisplayFunc(glueDibuja);
    glutTimerFunc(50, glueTimer, 0);

    mundo.inicializa();
    glutMainLoop();
    return 0;
}