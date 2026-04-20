#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

namespace ETSIDI { class Sprite; }

class FenixL : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    FenixL(Vector2D pos) :
        Pieza("Zinico", Bando::LUZ,
            60,     // vida 
            2.5f,   // velocidad
            40,     // poder de ataque (mucha magia)
            1.2f,   // velocidad de ataque
            100.0f, // alcance (ataque a distancia)
            9,      // rango de movimiento 
            TipoArma::ESCUDO,
            pos)

    {
        salud = new Vida(60, 20.0f);
        // PARA PONER LA IMAGEN
        sprite = new ETSIDI::Sprite("imagenes/alumnos/teatrosinfondo.png", 0, 0, 0.9f, 0.9f);
    }

    // El destructor para que no haya fugas de memoria
    virtual ~FenixL() {
        delete sprite;
    }
    float getRadio() override { return 6.0f; }

    //le indicamos que tipo de moviemiento tiene la pieza
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::VOLADOR; }

    std::string obtenerNombreSprite() override { return "luz_fenix"; }

    void dibuja() override;
};