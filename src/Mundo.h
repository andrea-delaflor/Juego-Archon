#pragma once
#include "Tablero.h"

class Mundo {
public:
    
    void inicializa();
    void mueve();
    void dibuja();
private:
    Tablero tablero;
    float valorLuz;
    float angulo;
};