#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

namespace ETSIDI { class Sprite; }

class DragonO : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    DragonO(Vector2D posInicial) :
        Pieza("Mecanica", Bando::OSCURIDAD,
            150,    // vida
            4.0f,   // velocidad
            30,     // poder de ataque
            0.8f,   // velocidad de ataque
            2.0f,   // alcance
            4,      // rango de movimiento
            TipoArma::BOLA_DE_FUEGO,
            posInicial)
    {
        // Ruta de imagen original del dragón
        sprite = new ETSIDI::Sprite("imagenes/profes/dragonsinfondo.png", 0, 0, 0.8f, 0.8f);
    }

    virtual ~DragonO() {
        delete sprite;
    }

    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::VOLADOR; }
    std::string obtenerNombreSprite() override { return "oscuridad_dragon"; }

    void dibuja() override;
};