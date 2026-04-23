#pragma once
#include "ETSIDI.h"
#include <cmath>

class Vida {
private:
    int actual;
    int maximo;
    float puntosPorCorazon;
    ETSIDI::Sprite spriteCorazon;
   

public:
    // puntosC: cuánta vida representa cada corazón (ej. 20)
    Vida(int max, float puntosC = 20.0f) :
        spriteCorazon("imagenes/corazonvida.png", 0, 0, 0.5f, 0.5f)
    {
        maximo = max;
        actual = max;
        puntosPorCorazon = puntosC;
    }

    void damage(int d) { actual -= d; if (actual < 0) actual = 0; }
    void heal(int h) { actual += h; if (actual > maximo) actual = maximo; }
    int getActual() { return actual; }
    bool muerto() { return actual <= 0; }

    void dibuja(float x, float y, float escala = 1.0f) {
        if (actual <= 0) return;

        // Calculamos cuántos corazones mostrar
        int numCorazones = (int)std::ceil(actual / puntosPorCorazon);
        spriteCorazon.setSize(0.6f * escala, 0.6f * escala);

        for (int i = 0; i < numCorazones; i++) {
            // Centramos la fila de corazones sobre la pieza
            float separacion = 0.5f * escala;
            float xPos = x + (i - (numCorazones - 1) / 2.0f) * separacion;

            spriteCorazon.setPos(xPos, y + (0.4f * escala));
            spriteCorazon.draw();
        }
    }

    void restaurarAlMaximo() {
        actual = maximo;
    }
};