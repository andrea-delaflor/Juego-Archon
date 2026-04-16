#pragma once
#include "Vector2D.h"
#include "ETSIDI.h"

class Proyectil {
protected:
    Vector2D posicion;
    Vector2D velocidad;
    float radio;
    int danio;
    bool deJugador1;

public:
    Proyectil(Vector2D pos, Vector2D vel, int d, bool j1)
        : posicion(pos), velocidad(vel), danio(d), deJugador1(j1), radio(0.4f) {
    }

    virtual ~Proyectil() {}

    void mueve(float dt) {
        posicion = posicion + velocidad * dt;
    }

    virtual void dibuja() = 0; 

    Vector2D getPos() { return posicion; }
    int getDanio() { return danio; }
    bool esDeJugador1() { return deJugador1; }
};