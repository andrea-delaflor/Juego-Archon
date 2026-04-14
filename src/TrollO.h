#pragma once
#include "Pieza.h"
#include "glut.h"

//Troll ----->Bando oscuridad

class TrollO : public Pieza {
public:

    TrollO(Vector2D pos) :
        Pieza("Troll Oscuridad", Bando::OSCURIDAD,
            100,   // vida        
            1.5f,  // velocidad
            30,    // la duerza de Ataque
            0.8f,  // velocidad de Ataque
            40.0f, // alcance
            2,     // rango Movimiento
            TipoArma::CUERPO_A_CUERPO,
            pos)
    {
    }

   // le indicamos a Pieza el Tipo de Movimiento que usa
    TipoMovimiento obtenerTipoMovimiento() override {
        return TipoMovimiento::TERRESTRE;
    }

    std::string obtenerNombreSprite() override { return "osc_troll"; }

    void dibuja() override;
};