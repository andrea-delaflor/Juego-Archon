#pragma once
#include "Pieza.h"
#include "ETSIDI.h"

class ArqueraL : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    ArqueraL(Vector2D pos) :
        Pieza("Arquera", Bando::LUZ,
            50,          // Vida (más frágil)
            2.0f,        // Velocidad
            25,          // Poder de ataque
            1.5f,        // Velocidad de ataque
            80.0f,       // Alcance (Rango de flecha)
            3,           // Rango de movimiento (máximo 3)
            TipoArma::FLECHA,
            pos)
    {
        sprite = new ETSIDI::Sprite("imagenes/alumnos/cestidi2.png", 0, 0, 0.9f, 0.9f);
    }

    virtual ~ArqueraL() { delete sprite; }

    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TERRESTRE; }
    std::string obtenerNombreSprite() override { return "luz_arquera"; }

    void dibuja() override {
        if (sprite != nullptr) {
            float x_gl = (float)posicion.x - 4.0f;
            float y_gl = 4.0f - (float)posicion.y;
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
};
