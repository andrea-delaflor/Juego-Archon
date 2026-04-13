#include "MagoL.h"

void MagoL::dibuja() {
    // Convertir de coordenadas tablero a las de Glut
    float x_gl = (float)posicion.x - 4.0f;
    float y_gl = 4.0f - (float)posicion.y;

    // Color azul claro/blanco para el mago
    glColor3f(0.8f, 0.8f, 1.0f);

    // Dibujamos un triángulo para diferenciarlo del cuadrado del Golem
    glBegin(GL_TRIANGLES);
    glVertex2f(x_gl, y_gl + 0.35f);          // Punta arriba
    glVertex2f(x_gl - 0.35f, y_gl - 0.35f);  // Esquina abajo izq
    glVertex2f(x_gl + 0.35f, y_gl - 0.35f);  // Esquina abajo der
    glEnd();
}