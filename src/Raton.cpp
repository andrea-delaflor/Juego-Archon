#include "raton.h"
#include "glut.h"
#include <math.h>

void Raton::actualizaPosicion(int x, int y, int ancho, int alto) {
    float mx = (2.0f * x / (float)ancho) - 1.0f;
    float my = 1.0f - (2.0f * y / (float)alto);

    posicion.x = mx * 6.0f;
    posicion.y = my * 6.0f;

    casilla.x = (int)floor(posicion.x + 4.5f);
    casilla.y = (int)floor(4.5f - posicion.y);

    if (casilla.x < 0 || casilla.x > 8 || casilla.y < 0 || casilla.y > 8) {
        casilla.x = -1;
    }
}

void Raton::dibuja() {
    if (casilla.x == -1) return;

    // Usamos EXACTAMENTE la misma conversión que en tu PiezaTest
    float x_gl = (float)casilla.x - 4.0f;
    float y_gl = 4.0f - (float)casilla.y;

    // Establecemos el color rojo (como hacías con el cian/magenta)
    glColor3f(1.0f, 0.0f, 0.0f);

    // Dibujamos el marco. Usamos LINE_LOOP para que sea un recuadro hueco
    // y se vea el tablero (la imagen/colores) a través.
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x_gl - 0.45f, y_gl - 0.45f);
    glVertex2f(x_gl + 0.45f, y_gl - 0.45f);
    glVertex2f(x_gl + 0.45f, y_gl + 0.45f);
    glVertex2f(x_gl - 0.45f, y_gl + 0.45f);
    glEnd();

    // ESTA LÍNEA ES LA CLAVE: Devolvemos el color a blanco
    // Si no, el tablero en el siguiente frame se intentará dibujar en rojo
    glColor3f(1.0f, 1.0f, 1.0f);
}