#pragma once
#include "Pieza.h"
#include "glut.h"

class PiezaTest : public Pieza {
public:
    PiezaTest(Vector2D pos, Bando b) :
        Pieza("Test", b, 100, 1.0f, 10, 1.0f, 1.0f, 3, TipoArma::MAGIA, pos) {
    }

    std::vector<Vector2D> obtenerMovimientosValidos(Tablero* t) override { return {}; }
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TERRESTRE; }
    std::string obtenerNombreSprite() override { return ""; }

    void dibuja() override {
        // Esta es la lógica que usabas para el punto rojo
        float x_gl = (float)posicion.x - 4.0f;
        float y_gl = 4.0f - (float)posicion.y;

        // Dibujamos con el color del bando
        if (bando == Bando::LUZ) glColor3f(0.0f, 1.0f, 1.0f); // Cyan
        else glColor3f(1.0f, 0.0f, 1.0f);                    // Magenta

        glBegin(GL_QUADS);
        glVertex2f(x_gl - 0.3f, y_gl - 0.3f);
        glVertex2f(x_gl + 0.3f, y_gl - 0.3f);
        glVertex2f(x_gl + 0.3f, y_gl + 0.3f);
        glVertex2f(x_gl - 0.3f, y_gl + 0.3f);
        glEnd();
    }
};
