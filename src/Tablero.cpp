#include "Tablero.h"
#include "glut.h"
#include <math.h>


/*void Tablero::dibuja() {
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
}*/


bool Tablero::esPowerPoint(int i, int j) {
    // Puntos de poder: esquinas, centros de bordes y centro total
    if ((i == 0 && j == 0) || (i == 0 && j == 8) || (i == 8 && j == 0) || (i == 8 && j == 8) ||
        (i == 0 && j == 4) || (i == 4 && j == 0) || (i == 4 && j == 8) || (i == 8 && j == 4) ||
        (i == 4 && j == 4)) return true;
    return false;
}

bool Tablero::esCasillaVariable(int i, int j) {
    if (esPowerPoint(i, j)) return false;
    // En Archon, las variables son las que no son ni blancas ni negras puras
    // Forman un patrón donde i o j son impares pero no ambos
    return (i % 2 != j % 2);
}

void Tablero::dibuja(float luminosidad) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            if (esPowerPoint(i, j)) {
                glColor3f(1.0f, 0.9f, 0.0f); // Dorado
            }
            else if (esCasillaVariable(i, j)) {
                glColor3f(luminosidad, luminosidad, luminosidad); // Variable!
            }
            else {
                // Casillas fijas
                if ((i + j) % 2 == 0) glColor3f(0.0f, 0.0f, 0.0f); // Negro
                else glColor3f(1.0f, 1.0f, 1.0f); // Blanco
            }

            float x = i - 4.0f; // Centrado
            float y = j - 4.0f;
            glBegin(GL_QUADS);
            glVertex2f(x - 0.5f, y - 0.5f);
            glVertex2f(x + 0.5f, y - 0.5f);
            glVertex2f(x + 0.5f, y + 0.5f);
            glVertex2f(x - 0.5f, y + 0.5f);
            glEnd();
        }
    }
}