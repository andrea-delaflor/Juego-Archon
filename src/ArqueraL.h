#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

namespace ETSIDI { class Sprite; }

class ArqueraL : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    ArqueraL(Vector2D pos) :
        Pieza("Cetsidi", Bando::LUZ,
            50,          // Vida
            2.0f,        // Velocidad combate
            25,          // Poder de ataque
            1.5f,        // Velocidad de ataque
            80.0f,       // Alcance
            3,           // Rango de movimiento tablero
            TipoArma::PELOTAFUTBOL,
            pos)
    {
        salud = new Vida(50, 20.0f);
        sprite = new ETSIDI::Sprite("imagenes/alumnos/cestidi2.png", 0, 0, 0.9f, 0.9f);
    }

    virtual ~ArqueraL() {
        delete sprite;
    }

    float getRadio() override { return 6.0f; }

    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TERRESTRE; }
    std::string obtenerNombreSprite() override { return "luz_arquera"; }

    void dibuja() override;
};
