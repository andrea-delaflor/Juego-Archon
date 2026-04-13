#pragma once
#include "Vector2D.h"
#include <vector>

class Tablero;
class Pieza;


class Movimiento {
public:

    // Movimiento en cruz, no atraviesa piezas 
    std::vector<Vector2D> calcularTerrestre(Pieza* pieza, Tablero* tablero);

    // Movimiento en 8 direcciones, ignora obstáculos 
    std::vector<Vector2D> calcularVolador(Pieza* pieza, Tablero* tablero);

    // Cualquier casilla del tablero, sin límite de distancia
    std::vector<Vector2D> calcularTeletransporte(Pieza* pieza, Tablero* tablero);

private:

    // CalcularTerrestre: verifica trayectorias en cruz (arriba, abajo, izquierda, derecha)
    // y se detiene al encontrar una pieza o el borde del tablero
    // Avanza casilla a casilla en una dirección hasta encontrar obstáculo
    // di = delta columna, dj = delta fila
    void expandirDireccion(Pieza* pieza, Tablero* tablero,
        int di, int dj,
        std::vector<Vector2D>& movimientos);
};