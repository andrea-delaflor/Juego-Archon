#pragma once
#include "ETSIDI.h"

class Tablero {
public:
    // Dibujamos pasando el valor de luz (0.0 a 1.0)
    void dibuja(float luminosidad);

private:
    bool esPowerPoint(int i, int j);
    bool esCasillaVariable(int i, int j);
};