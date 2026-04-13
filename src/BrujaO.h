#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

namespace ETSIDI { class Sprite; }

class BrujaO : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    BrujaO(Vector2D pos) :
        Pieza("Bruja Oscuridad", Bando::OSCURIDAD,
            55,      // Vida (un poco menos que el mago, pero quizás más rápida)
            3.0f,    // Velocidad
            42,      // Poder de ataque
            1.3f,    // Velocidad de ataque
            110.0f,  // Alcance
            9,       // Rango de movimiento (3 casillas)
            TipoArma::VENENO,
            pos)
    {
        // CARGAMOS EL SPRITE 
        sprite = new ETSIDI::Sprite("imagenes/profes/direccion.png", 0, 0, 0.9f, 0.9f);
    }

    // El destructor para que no haya fugas de memoria
    virtual ~BrujaO() {
        delete sprite;
    }


    // Al ser TELETRANSPORTE, usará la lógica de Movimiento.cpp automáticamente
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TELETRANSPORTE; }

    std::string obtenerNombreSprite() override { return "oscuridad_bruja"; }
    void dibuja() override;
};