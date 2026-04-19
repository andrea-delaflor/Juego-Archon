#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

//Troll ----->Bando oscuridad
namespace ETSIDI { class Sprite; }

class TrollO : public Pieza {
private:
    ETSIDI::Sprite* sprite;
    ETSIDI::Sprite* spriteCodigo;
public:

    TrollO(Vector2D pos) :
        Pieza("Estadistica", Bando::OSCURIDAD,
            100,   // vida        
            1.5f,  // velocidad
            30,    // la duerza de Ataque
            0.8f,  // velocidad de Ataque
            40.0f, // alcance
            2,     // rango Movimiento
            TipoArma::CUERPO_A_CUERPO,
            pos)
    {
        // CARGAMOS EL SPRITE 
        sprite = new ETSIDI::Sprite("imagenes/profes/orgaestadistica.png", 0, 0, 0.9f, 0.9f);
        // CARGAMOS EL SPRITE DEL ARMA   
        spriteCodigo = new ETSIDI::Sprite("imagenes/armas/codigotecnico.png", 0, 0, 0.8f, 0.8f);

    }

    // El destructor para que no haya fugas de memoria
    virtual ~TrollO() {
        delete sprite;
        delete spriteCodigo;
    }

   // le indicamos a Pieza el Tipo de Movimiento que usa
    TipoMovimiento obtenerTipoMovimiento() override {
        return TipoMovimiento::TERRESTRE;
    }

    std::string obtenerNombreSprite() override { return "osc_troll"; }

    void dibuja() override;
    void dibujaEnBatalla() override;
};