#pragma once
#include "Vector2D.h"
#include "Pieza.h"

// Los 4 tipos de objetos que pueden caer
enum class TipoObstaculo { DANO, VELOCIDAD, CONGELACION, AUMENTO_DANO};

class Obstaculo {
public:
    Obstaculo(Vector2D pos, Vector2D vel, TipoObstaculo t);
    ~Obstaculo() = default;

    void dibuja();
    void mueve(float dt);

    Vector2D getPos() const { return posicion; }
    TipoObstaculo getTipo() const { return tipo; }

    // Función matemática para saber si choca con un jugador
    bool colisionaCon(Vector2D posJugador, float radioJugador = 2.5f) const;

private:
    Vector2D posicion;
    Vector2D velocidad;
    TipoObstaculo tipo;
    float radio; // Tamaño de la bola para calcular el choque

    ETSIDI::Sprite spriteObs;
};
