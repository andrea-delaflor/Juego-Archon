#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"

namespace ETSIDI { class Sprite; }
// MAGO: Bando de la LUZ ---- DELEGACIÓN
class MagoL : public Pieza {
private:
    ETSIDI::Sprite* sprite;

public:
    MagoL(Vector2D pos) :
        Pieza("Mago Luz", Bando::LUZ,
            60,     // vida 
            2.5f,   // velocidad
            40,     // poder de ataque (mucha magia)
            1.2f,   // velocidad de ataque
            100.0f, // alcance (ataque a distancia)
            9,      // rango de movimiento 
            TipoArma::MAGIA,
            pos)
    
        {
            // CARGAMOS EL SPRITE AQUÍ DENTRO
            // He usado la ruta que pusiste antes
            sprite = new ETSIDI::Sprite("imagenes/alumnos/delegacion2.png", 0, 0, 0.9f, 0.9f);
        }

        // El destructor para que no haya fugas de memoria
        virtual ~MagoL() {
            delete sprite;
        }

    
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TELETRANSPORTE; }


    // Imagen o identificador
    std::string obtenerNombreSprite() override { return "luz_mago"; }

    void dibuja() override;
};