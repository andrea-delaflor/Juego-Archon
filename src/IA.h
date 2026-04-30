#pragma once
#pragma once
#include "Vector2D.h"
#include <vector>

// Forward declarations
class Mundo;
class Batalla;
class Pieza;

class IA {
public:
    // Lógica para el tablero: elige pieza y destino aleatorio
    static std::vector<Vector2D> decidirMovimientoTablero(Mundo* mundo);

    // Lógica para la batalla: mueve la pieza y decide acciones
    static void ejecutarAccionBatalla(Batalla* batalla, Pieza* iaPieza, Pieza* rivalPieza, float dt);

private:
    static float congelarAtaque; //esto es para que no dispare continuamente
};