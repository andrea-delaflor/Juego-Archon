#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

namespace ETSIDI { class Sprite; }

class MagoL : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    MagoL(Vector2D pos) :
        Pieza("Delegacion", Bando::LUZ,
            60,     // vida 
            2.5f,   // velocidad
            40,     // poder de ataque (mucha magia)
            1.2f,   // velocidad de ataque
            100.0f, // alcance (ataque a distancia)
            9,      // rango de movimiento 
            TipoArma::MAGIA,
            pos)
    
        {
            // CARGAMOS EL SPRITE 
            sprite = new ETSIDI::Sprite("imagenes/alumnos/magosinfondo1.png", 0, 0, 0.9f, 0.9f);
        }

        // El destructor para que no haya fugas de memoria
        virtual ~MagoL() {
            delete sprite;
        }

    
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TELETRANSPORTE; }

    std::string obtenerNombreSprite() override { return "luz_mago"; }

    void dibuja() override;
};