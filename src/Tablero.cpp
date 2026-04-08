#include "Tablero.h"
#include "glut.h"

Tablero::Tablero() {
    ladoCasilla = 2.0f; // Tamaño de cada cuadro
}

void Tablero::dibuja() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // Lógica para alternar colores
            if ((i + j) % 2 == 0)
                glColor3f(0.2f, 0.2f, 0.2f); // Gris oscuro
            else
                glColor3f(0.8f, 0.8f, 0.8f); // Gris claro

            // Dibujar el cuadrado de la casilla
            float x = (i - 4.5f) * ladoCasilla;
            float y = (j - 4.5f) * ladoCasilla;

            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + ladoCasilla, y);
            glVertex2f(x + ladoCasilla, y + ladoCasilla);
            glVertex2f(x, y + ladoCasilla);
            glEnd();
        }
    }
}