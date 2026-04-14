#pragma once
#include "Pieza.h"
#include "ETSIDI.h"

class BasiliscoO : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    BasiliscoO(Vector2D pos) :
        Pieza("Basilisco", Bando::OSCURIDAD,
            90,          // Vida (más resistente)
            1.2f,        // Velocidad
            35,          // Poder de ataque
            0.8f,        // Velocidad de ataque
            40.0f,       // Alcance corto
            3,           // Rango de movimiento (máximo 3)
            TipoArma::VENENO,
            pos)
    {
        sprite = new ETSIDI::Sprite("imagenes/profes/mate.png", 0, 0, 0.9f, 0.9f);
    }

    virtual ~BasiliscoO() { delete sprite; }

    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TERRESTRE; }
    std::string obtenerNombreSprite() override { return "oscuridad_basilisco"; }

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
