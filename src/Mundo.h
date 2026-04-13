#pragma once
#include "Tablero.h"
#include "Vector2D.h"
#include "ETSIDI.h"
#include "PiezaTest.h"

class Mundo {
public:
    
    void inicializa();
    void mueve();
    void dibuja();
private:
    Tablero tablero;
    float valorLuz;
    float angulo;

<<<<<<< Updated upstream

    // --- Declaramos punteros a Pieza para hacer la prueba de PiezaTest ---
    // Usamos la clase base Pieza* para aprovechar el polimorfismo
    Pieza* piezaLuz = nullptr;
    Pieza* piezaOscuridad = nullptr;
    //--------------------------------------------------------
=======
    // --- AÑADIR PARA LA PRUEBA ---
    Vector2D posPrueba; // Tu struct Vector2D 
    // -----------------------------

    // otra prueba:
    Vector2D posCirculo;
>>>>>>> Stashed changes
};