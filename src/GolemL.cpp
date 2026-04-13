#include"GolemL.h"

void GolemL::dibuja() {
    //Convertir de coordenadas tablero a las de Glut
    float x_gl = (float)posicion.x - 4.0f;
    float y_gl = 4.0f - (float)posicion.y;

    //color pieza
    glColor3f(0.0f, 1.0f, 1.0f);

    //Forma Golem ---->Por ahora cuadrado
    glBegin(GL_QUADS);
    glVertex2f(x_gl - 0.35f, y_gl - 0.35f);
    glVertex2f(x_gl + 0.35f, y_gl - 0.35f);
    glVertex2f(x_gl + 0.35f, y_gl + 0.35f);
    glVertex2f(x_gl - 0.35f, y_gl + 0.35f);
    glEnd();
}