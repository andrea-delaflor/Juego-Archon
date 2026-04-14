#include "GolemL.h"
#include "glut.h" // Añadimos esto para poder usar los comandos de OpenGL

void GolemL::dibuja() {
    // 1. APAGAMOS LAS TEXTURAS TEMPORALMENTE PARA DIBUJAR FORMAS PURAS
    glDisable(GL_TEXTURE_2D);

    //Convertir de coordenadas tablero a las de Glut
    float x_gl = (float)posicion.x - 4.0f;
    float y_gl = 4.0f - (float)posicion.y;

    //color pieza (Cian)
    glColor3f(0.0f, 1.0f, 1.0f);

    //Forma Golem ---->Por ahora cuadrado
    glBegin(GL_QUADS);
    glVertex2f(x_gl - 0.35f, y_gl - 0.35f);
    glVertex2f(x_gl + 0.35f, y_gl - 0.35f);
    glVertex2f(x_gl + 0.35f, y_gl + 0.35f);
    glVertex2f(x_gl - 0.35f, y_gl + 0.35f);
    glEnd();

    // 2. VOLVEMOS A ENCENDER LAS TEXTURAS PARA NO ROMPER LAS DEMÁS FICHAS
    glEnable(GL_TEXTURE_2D);
}