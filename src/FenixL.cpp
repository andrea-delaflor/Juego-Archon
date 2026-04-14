#include"FenixL.h"
void FenixL::dibuja() {
    actualizar(0.016f);
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

        // Efecto visual extra: Si está animando, que sea un poco más grande (vuelo)      
        if (animando) {
            sprite->setSize(0.9f, 0.9f);
        }
        else {
            sprite->setSize(0.9f, 0.9f);
        }

        sprite->draw();

        glPopAttrib();
        glPopMatrix();
    }
    glutPostRedisplay();
}