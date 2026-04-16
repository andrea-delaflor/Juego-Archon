#pragma once

#include "Proyectil.h"
#include "ETSIDI.h"

class PelotaFutbol : public Proyectil {
    ETSIDI::Sprite sprite;
public:
    PelotaFutbol(Vector2D pos, Vector2D vel, int d, bool j1)
        : Proyectil(pos, vel, d, j1), sprite("imagenes/pelotafutbol.png", 0, 0, 1, 1) {
    } // Pon el nombre de tu archivo

    void dibuja() override {
        sprite.setPos(posicion.x, posicion.y);
        sprite.draw();
    }
};

class BolaFuego : public Proyectil {
    ETSIDI::Sprite sprite;
public:
    BolaFuego(Vector2D pos, Vector2D vel, int d, bool j1)
        : Proyectil(pos, vel, d, j1), sprite("imagenes/fuego.png", 0, 0, 1.5, 1.5) {
    } // Pon el nombre de tu archivo

    void dibuja() override {
        sprite.setPos(posicion.x, posicion.y);
        sprite.draw();
    }
};