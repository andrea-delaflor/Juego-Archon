#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

namespace ETSIDI { class Sprite; }

class BasiliscoO : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    BasiliscoO(Vector2D pos) :
        Pieza("Matematicas", Bando::OSCURIDAD,
            90,          // Vida
            1.2f,        // Velocidad combate
            35,          // Poder de ataque
            0.8f,        // Velocidad de ataque
            40.0f,       // Alcance
            3,           // Rango de movimiento tablero
            TipoArma::RAYO_NUMERICO,
            pos)
    {
        salud = new Vida(90, 20.0f);
        sprite = new ETSIDI::Sprite("imagenes/profes/mate.png", 0, 0, 0.9f, 0.9f);
    }

    virtual ~BasiliscoO() {
        delete sprite;
    }

    float getRadio() override { return 2.5f; }

    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TERRESTRE; }
    std::string obtenerNombreSprite() override { return "oscuridad_basilisco"; }

    void dibuja() override;
    void dibujaEnBatalla() override;
};
