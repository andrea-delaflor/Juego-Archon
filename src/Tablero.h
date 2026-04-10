#pragma once
#include "ETSIDI.h"

class Tablero {
public:
    // Dibuja el tablero pasando la luminosidad actual (0.0 a 1.0)
    void dibuja(float luminosidad);

private:
	//Punto de poder : E5, A5, I5, E1, E9
    bool esPowerPoint(int i, int j);

	// Casillas variables: Cruz central + diagonales
    bool esVariable(int i, int j);
};