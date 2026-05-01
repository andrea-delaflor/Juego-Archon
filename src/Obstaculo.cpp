#include "Obstaculo.h"
#include "glut.h"

Obstaculo::Obstaculo(Vector2D pos, Vector2D vel, TipoObstaculo t) : spriteObs("imagenes/vela.png", 0, 0, 1.2f, 1.8f) {
    posicion = pos;
    velocidad = vel;
    tipo = t;
    radio = 0.6f; // Tamaño genérico del obstáculo

    if (tipo == TipoObstaculo::DANO_VELA) {
        spriteObs = ETSIDI::Sprite("imagenes/vela.png", 0, 0, 0.8f, 1.8f);
        spriteObs.setCenter(0.4f, 0.9f); // El centro es siempre la MITAD del ancho y la MITAD del alto
    }
    else if (tipo == TipoObstaculo::DANO_CALAVERA) {
        spriteObs = ETSIDI::Sprite("imagenes/calavera.png", 0, 0, 1.2f, 1.2f);
        spriteObs.setCenter(0.6f, 0.6f);
    }
    else if (tipo == TipoObstaculo::VELOCIDAD) {
        spriteObs = ETSIDI::Sprite("imagenes/rayo.png", 0, 0, 1.5f, 1.5f);
       spriteObs.setCenter(0.75f, 0.75f);
    }
    else if (tipo == TipoObstaculo::CONGELACION) {
        spriteObs = ETSIDI::Sprite("imagenes/CubosHielo.png", 0, 0, 1.5f, 1.5f);
        spriteObs.setCenter(0.75f, 0.75f);
    }
    else if (tipo == TipoObstaculo::AUMENTO_DANO) {
        spriteObs = ETSIDI::Sprite("imagenes/granada.png", 0, 0, 1.5f, 1.5f);
        spriteObs.setCenter(0.75f, 0.75f);
    }

    
    
}

void Obstaculo::dibuja() {
    spriteObs.setPos(posicion.x, posicion.y);
    spriteObs.draw();
    
}

void Obstaculo::mueve(float dt) {
    // Físicas básicas: nueva posición = posición antigua + (velocidad * tiempo)
    posicion = posicion + velocidad * dt;
}


bool Obstaculo::colisionaCon(Vector2D posJugador, float radioJugador) const {
    Vector2D dist = posicion - posJugador;
    // Ahora usamos el radio que le pasemos específicamente
    return dist.modulo() < (radio + radioJugador);
}