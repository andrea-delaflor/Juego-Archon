#include "BrujaO.h"

void BrujaO::dibuja() {
    float x_gl = (float)posicion.x - 4.0f;
    float y_gl = 4.0f - (float)posicion.y;

    // COLOR GRIS MUY OSCURO (para representar la Oscuridad)
    glColor3f(0.2f, 0.2f, 0.2f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x_gl, y_gl - 0.4f);           // Punta abajo
    glVertex2f(x_gl - 0.35f, y_gl + 0.35f);  // Esquina superior izquierda
    glVertex2f(x_gl + 0.35f, y_gl + 0.35f);  // Esquina superior derecha
    glEnd();

    // Opcional: Un pequeño borde blanco para que no se pierda del todo en el negro
    glLineWidth(1.0f);
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x_gl, y_gl - 0.4f);
    glVertex2f(x_gl - 0.35f, y_gl + 0.35f);
    glVertex2f(x_gl + 0.35f, y_gl + 0.35f);
    glEnd();
}