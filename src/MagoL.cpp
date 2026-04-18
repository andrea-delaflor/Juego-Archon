#include "MagoL.h"


void MagoL::dibuja() {
    if (sprite != nullptr) {
         float x_gl = (float)posicion.x - 4.0f;
         float y_gl = 4.0f - (float)posicion.y;

         // 1. GUARDAR EL ESTADO ACTUAL
         glPushMatrix();          // Guarda la posición de la cámara
         glPushAttrib(GL_ALL_ATTRIB_BITS); // Guarda todos los colores, texturas y luces

            // 2. CONFIGURAR PARA EL SPRITE
         glEnable(GL_TEXTURE_2D);
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glColor3ub(255, 255, 255); // Reset de color a blanco

         sprite->setPos(x_gl, y_gl);
         sprite->draw();

         // 3. RECUPERAR EL ESTADO ANTERIOR
         glPopAttrib();  // Restaura los colores y texturas anteriores
         glPopMatrix();  // Restaura la cámara a como estaba antes del mago
        }
    }