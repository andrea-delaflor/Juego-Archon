#pragma once
#include "Pieza.h"
#include "glut.h"

//GOLEM ----> Bando de la LUZ

class GolemL : public Pieza {
public:

    GolemL(Vector2D pos) :
        Pieza("Golem Luz", Bando::LUZ,
            100,    // esta es la vida
            1.5f,   // esta es la vel
            30,     // la fuerza de golpeo
            0.8f,   // la vel de ataque
            40.0f,  // radio de ataque
            2,      // rango de movimiento
            TipoArma::CUERPO_A_CUERPO,
            pos)
    {
    }

    // Le comunicamos a nuestra Pieza.cpp que se trata de una Pieza tipo TERRESTRE-----> clase Moviemiento
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TERRESTRE; }

    // Para tener la imagen
    std::string obtenerNombreSprite() override { return "luz_golem"; }

    void dibuja() override;
};