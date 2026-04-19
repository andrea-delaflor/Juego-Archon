#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

namespace ETSIDI { class Sprite; }

class DjiniL : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    DjiniL(Vector2D posInicial) :
        Pieza("CREA", Bando::LUZ,
            100,    // vida
            5.0f,   // velocidad
            20,     // poder de ataque
            1.0f,   // velocidad de ataque
            3.0f,   // alcance
            4,      // rango de movimiento
            TipoArma::RAYO_LASER,
            posInicial)
    {
        salud = new Vida(100, 20.0f);
        // Ruta de imagen original del Djinn
        sprite = new ETSIDI::Sprite("imagenes/alumnos/djinisinfondo.png", 0, 0, 0.8f, 0.8f);
    }

    virtual ~DjiniL() {
        delete sprite;
    }

    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::VOLADOR; }
    std::string obtenerNombreSprite() override { return "luz_djinn"; }

    void dibuja() override;
};

