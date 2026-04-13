#pragma once
#include "Tablero.h"
#include "Vector2D.h"
#include "ETSIDI.h"
#include "PiezaTest.h"
#include "Raton.h"

class Mundo {
public:
    
    void inicializa();
    void mueve();
    void dibuja();
    void clickRaton(int button, int state, int x, int y);
    Raton raton;               // Objeto que calcula la casilla
    Pieza* seleccionada = nullptr; // Puntero a la pieza que queremos mover
private:
    Tablero tablero;
    float valorLuz = 0.5f; // Añade el = 0.5f
    float angulo = 0.0f;   // Añade el = 0.0f

    
    Pieza* piezaLuz = nullptr;
    Pieza* piezaOscuridad = nullptr;
    
};