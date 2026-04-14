#include "TrollO.h"

void TrollO::dibuja() {

    //Convertir de coordenadas tablero a las de Glut
    float x_gl = (float)posicion.x - 4.0f;
    float y_gl = 4.0f - (float)posicion.y;

    // Color del Troll —--> Rojo oscuro (por ahora)
    glColor3f(0.8f, 0.1f, 0.1f);

    // Lo hacemos en forma de triangulo por ahora
    glBegin(GL_TRIANGLES);
    glVertex2f(x_gl, y_gl + 0.38f);  
    glVertex2f(x_gl - 0.38f, y_gl - 0.38f);  
    glVertex2f(x_gl + 0.38f, y_gl - 0.38f); 
    glEnd();
}