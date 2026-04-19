#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

//GOLEM ----> Bando de la LUZ

namespace ETSIDI { class Sprite; }

class GolemL : public Pieza {
private:
    ETSIDI::Sprite* sprite;
    ETSIDI::Sprite* spriteLlave;
public:

    GolemL(Vector2D pos) :
        Pieza("MotoStudent", Bando::LUZ,
            100,    // esta es la vida
            1.5f,   // esta es la vel
            30,     // la fuerza de golpeo
            0.8f,   // la vel de ataque
            40.0f,  // radio de ataque
            2,      // rango de movimiento
            TipoArma::CUERPO_A_CUERPO,
            pos)
    {
        salud = new Vida(100, 20.0f);
        // CARGAMOS EL SPRITE 
        sprite = new ETSIDI::Sprite("imagenes/alumnos/motostudent2.png", 0, 0, 0.9f, 0.9f);
        //el sprite del arma
        spriteLlave = new ETSIDI::Sprite("imagenes/armas/llaveinglesa.png", 0, 0, 0.8f, 0.8f);
    }

    // El destructor para que no haya fugas de memoria
    virtual ~GolemL() {
        delete sprite;
        delete spriteLlave;
    }

    // Le comunicamos a nuestra Pieza.cpp que se trata de una Pieza tipo TERRESTRE-----> clase Moviemiento
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TERRESTRE; }

    // Para tener la imagen
    std::string obtenerNombreSprite() override { return "luz_golem"; }

    void dibuja() override;
    void dibujaEnBatalla() override;
};