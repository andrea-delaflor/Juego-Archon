#include "ArqueraL.h"

void ArqueraL::dibuja() {
    actualizar(0.0005f); // Para la suavidad del movimiento visual

    if (sprite != nullptr) {
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
        
        sprite->draw();

        glPopAttrib();
        glPopMatrix();
    }
}

void ArqueraL::dibujaEnBatalla(){
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3ub(255, 255, 255);

    sprite->setPos(0.0f, 0.0f);  // ← siempre en el origen
    sprite->setSize(0.9f, 0.9f);
    sprite->draw();
}
