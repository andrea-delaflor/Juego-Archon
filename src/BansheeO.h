#pragma once

#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

namespace ETSIDI { class Sprite; }

class BansheeO : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    BansheeO(Vector2D pos) :
        Pieza("Fisica", Bando::OSCURIDAD,
            50,     // vida 
            4.0f,   // velocidad
            35,     // poder de ataque (mucha magia)
            2.0f,   // velocidad de ataque
            45.0f, // alcance (ataque a distancia)
            9,      // rango de movimiento 
            TipoArma::MAGIA,
            pos)

    {
        // PARA PONER LA IMAGEN
        sprite = new ETSIDI::Sprite("imagenes/profes/bansheesinfondo.png", 0, 0, 0.9f, 0.9f);
    }

    // El destructor para que no haya fugas de memoria
    virtual ~BansheeO() {
        delete sprite;
    }

    //le indicamos que tipo de moviemiento tiene la pieza
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::VOLADOR; }

    std::string obtenerNombreSprite() override { return "oscuridad_banshee"; }

    void dibuja() override;
};