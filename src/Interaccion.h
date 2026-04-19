#pragma once
#include "Pieza.h"
#include "Proyectil.h"
#include "Obstaculo.h"

class Interaccion {
public:
    // Verifica si un proyectil toca a una pieza en batalla
    static bool colision(Proyectil& p, Vector2D posPiezaEnBatalla) {
        Vector2D dist = p.getPos() - posPiezaEnBatalla;
        return dist.modulo() < 1.5f; // Radio de colisión estándar
    }

    // Verifica si un obstáculo (bola que cae) toca a una pieza
    static bool colision(Obstaculo& o, Vector2D posPiezaEnBatalla) {
        Vector2D dist = o.getPos() - posPiezaEnBatalla;
        // Compensamos el glScalef(2,2,1) de la batalla con un radio mayor
        return dist.modulo() < 2.5f;
    }
};