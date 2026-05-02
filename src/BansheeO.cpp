#include"BansheeO.h"

#include"BansheeO.h"
void BansheeO::dibuja() {
    actualizar(0.015f);
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
    glutPostRedisplay();
}

void BansheeO::dibujaEnBatalla() {
    // 1. Dibujamos el sprite de la Banshee centrado
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    sprite->setPos(0, 0);
    sprite->draw();
    glPopMatrix();

    // 2. Dibujamos el escudo 
    if (tieneEscudoActivo()) {
        glPushMatrix();
        
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        
        glColor4ub(150, 0, 255, 255);
        glLineWidth(3.5f); // Grosor del anillo

        
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i += 10) {
            float rad = i * 3.14159f / 180.0f;
            
            glVertex2f(cos(rad) * 1.2f, sin(rad) * 1.2f);
        }
        glEnd();

        glEnable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}