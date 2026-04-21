#include "GolemL.h"

void GolemL::dibuja() {
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

void GolemL::dibujaEnBatalla() {
    // se dibuja el Golem 
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    sprite->setPos(0, 0);
    sprite->draw();
    glPopMatrix();

    
    actualizaAnimacionAtaque(0.02f);

    // se dibuja la Llave Inglesa
    glPushMatrix();
    // se coloca el arma al lado del gholem
    glTranslatef(0.6f, 0.3f, 0.0f);
    // se rota la imagen
    glRotatef(-anguloAtaque, 0, 0, 1);

    if (spriteLlave) {
        spriteLlave->setPos(0, 0);
        spriteLlave->draw();
    }
    glPopMatrix();
}