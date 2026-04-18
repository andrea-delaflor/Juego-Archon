#pragma once
#include "Proyectil.h"

class PelotaFutbol : public Proyectil {
    ETSIDI::Sprite sprite;
public:
    PelotaFutbol(Vector2D pos, Vector2D vel, int d, bool j1)
        : Proyectil(pos, vel, d, j1), sprite("imagenes/armas/pelotafutbol.png", 0, 0, 1, 1) {
    } 

    void dibuja() override {
        sprite.setPos(posicion.x, posicion.y);
        sprite.draw();
    }
};

class BolaFuego : public Proyectil {
    ETSIDI::Sprite sprite;
public:
    BolaFuego(Vector2D pos, Vector2D vel, int d, bool j1)
        : Proyectil(pos, vel, d, j1), sprite("imagenes/armas/fuego.png", 0, 0, 1.5, 1.5) {
    } 

    void dibuja() override {
        sprite.setPos(posicion.x, posicion.y);
        sprite.draw();
    }
};
