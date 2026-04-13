#pragma once
#include "Pieza.h"
#include "glut.h"

class BrujaO : public Pieza {
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
    }

    // Al ser TELETRANSPORTE, usará la lógica de Movimiento.cpp automáticamente
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TELETRANSPORTE; }

    std::string obtenerNombreSprite() override { return "oscuridad_bruja"; }
    void dibuja() override;
};