#include "DragonO.h"

void DragonO::dibuja() {
    // Actualizamos la posición en cada frame de dibujo (aprox 60fps)
    actualizar(0.005f);

    if (sprite != nullptr) {
        // Usamos la posición visual para el desplazamiento suave
        Vector2D vPos = obtenerPosicionVisual();

        float x_gl = (float)vPos.x - 4.0f;
        float y_gl = 4.0f - (float)vPos.y;

        glPushMatrix();
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor3ub(255, 255, 255);

        sprite->setPos(x_gl, y_gl);
        sprite->setSize(0.8f, 0.8f); // Tamaño original
        sprite->draw();

        glPopAttrib();
        glPopMatrix();
    }
    // Forzamos a que el sistema vuelva a dibujar para ver el movimiento
    glutPostRedisplay();
}