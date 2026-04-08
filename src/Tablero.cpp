#include "Tablero.h"
#include "glut.h"

bool Tablero::esPowerPoint(int i, int j) {
    // Los 5 puntos de poder (E5, A5, I5, E1, E9)
    if ((i == 4 && j == 4) || // Centro
        (i == 0 && j == 4) || (i == 8 && j == 4) || // Centros laterales
        (i == 4 && j == 0) || (i == 4 && j == 8))   // Centros superior/inferior
        return true;
    return false;
}

bool Tablero::esVariable(int i, int j) {
    if (esPowerPoint(i, j)) return false;

    // 1. La Cruz Central (toda la fila 4 y columna 4)
    if (i == 4 || j == 4) return true;

    // 2. DIAGONALES
    // Diagonal A4 a D1: (0,3), (1,2), (2,1), (3,0) -> suma i+j = 3
    if (i + j == 3) return true;

    // Diagonal F1 a I4: (5,0), (6,1), (7,2), (8,3) -> resta i-j = 5
    if (i - j == 5) return true;

    // Diagonal I6 a F9: (8,5), (7,6), (6,7), (5,8) -> suma i+j = 13
    if (i + j == 13) return true;

    // Diagonal D9 a A6: (3,8), (2,7), (1,6), (0,5) -> resta j-i = 5
    if (j - i == 5) return true;

    return false;
}




void Tablero::dibuja(float luminosidad) {
	// Recorremos cada casilla del tablero
	// COLUMNAS: i=0 (A) a i=8 (I)
    // FILAS: j=0  a j=8 
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            if (esPowerPoint(i, j)) {
                glColor3f(1.0f, 0.85f, 0.0f); // Dorado Archon
            }
            else if (esVariable(i, j)) {
                // Casillas que parpadean
                glColor3f(luminosidad, luminosidad, luminosidad);
            }
            else {
                // LÓGICA DE ESQUINAS:
                // Para que i=0 (A) sea negra y i=8 (I) sea blanca en j=0
                // Usamos la paridad de (i+j) pero invertida según la columna
                if (i < 4) { // Parte izquierda (A, B, C, D)
                    if ((i + j) % 2 != 0) glColor3f(1.0f, 1.0f, 1.0f); // Blanca
                    else glColor3f(0.0f, 0.05f, 0.15f);               // Negra
                }
                else { // Parte derecha (F, G, H, I)
                    if ((i + j) % 2 == 0) glColor3f(1.0f, 1.0f, 1.0f); // Blanca
                    else glColor3f(0.0f, 0.05f, 0.15f);               // Negra
                }
            }

            // Dibujo de la casilla centrado
            float x = i - 4.0f;
            float y = 4.0f - j; // Fila 0 arriba
            glBegin(GL_QUADS);
            glVertex2f(x - 0.48f, y - 0.48f);
            glVertex2f(x + 0.48f, y - 0.48f);
            glVertex2f(x + 0.48f, y + 0.48f);
            glVertex2f(x - 0.48f, y + 0.48f);
            glEnd();
        }
    }
}