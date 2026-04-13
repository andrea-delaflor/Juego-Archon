#pragma once
#include "Pieza.h"
#include "glut.h"

class PiezaTest : public Pieza {
public:
    
    PiezaTest(Vector2D pos, Bando b) :
        Pieza("Test", b, 100, 1.0f, 10, 1.0f, 1.0f, 3, TipoArma::MAGIA, pos) {
    }

    // Especificamos el tipo de pieza
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TERRESTRE; }

    std::string obtenerNombreSprite() override { return ""; }

    void dibuja() override {
        float x_gl = (float)posicion.x - 4.0f;
        float y_gl = 4.0f - (float)posicion.y;
        if (bando == Bando::LUZ) glColor3f(0.0f, 1.0f, 1.0f);
        else glColor3f(1.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(x_gl - 0.3f, y_gl - 0.3f);
        glVertex2f(x_gl + 0.3f, y_gl - 0.3f);
        glVertex2f(x_gl + 0.3f, y_gl + 0.3f);
        glVertex2f(x_gl - 0.3f, y_gl + 0.3f);
        glEnd();
    }
};