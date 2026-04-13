#pragma once
#include "Pieza.h"
#include "glut.h"


// MAGO: Bando de la LUZ ---- DELEGACIÓN
class MagoL : public Pieza {
public:
    MagoL(Vector2D pos) :
        Pieza("Mago Luz", Bando::LUZ,
            60,     // vida 
            2.5f,   // velocidad
            40,     // poder de ataque (mucha magia)
            1.2f,   // velocidad de ataque
            100.0f, // alcance (ataque a distancia)
            3,      // rango de movimiento 
            TipoArma::MAGIA,
            pos)
    {
    }

    
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TELETRANSPORTE; }


    // Imagen o identificador
    std::string obtenerNombreSprite() override { return "luz_mago"; }

    void dibuja() override;
};