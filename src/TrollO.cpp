#include "TrollO.h"

void TrollO::dibuja() {

    actualizar(0.0005f);

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

        sprite->setSize(0.9f, 0.9f);

        sprite->draw();

        glPopAttrib();
        glPopMatrix();
    }
}

void TrollO::dibujaEnBatalla() {
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    sprite->setPos(0, 0); // En batalla, el origen es la posición de la pieza
    sprite->draw();
    glPopMatrix();

   
    actualizaAnimacionAtaque(0.02f);

    
    glPushMatrix();
    // mover ka imagen al lado de la anterior
    glTranslatef(-0.6f, 0.3f, 0.0f);
    // Rotamos según el ángulo de ataque
    glRotatef(anguloAtaque, 0, 0, 1);

    if (spriteCodigo) {
        spriteCodigo->setPos(0, 0);
        spriteCodigo->draw();
    }
    glPopMatrix();
}