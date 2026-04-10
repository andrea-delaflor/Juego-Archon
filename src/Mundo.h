#pragma once
#include "Tablero.h"
#include "Vector2D.h"
#include "ETSIDI.h"

class Mundo {
public:
    
    void inicializa();
    void mueve();
    void dibuja();
private:
    Tablero tablero;
    float valorLuz;
    float angulo;

    // --- AÑADIR PARA LA PRUEBA ---
    Vector2D posPrueba; // Tu struct Vector2D 
    // -----------------------------
<<<<<<< Updated upstream
    // otra prueba:
=======
>>>>>>> Stashed changes
    Vector2D posCirculo;
};