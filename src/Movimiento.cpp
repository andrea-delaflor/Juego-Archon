#include "Movimiento.h"
#include "Pieza.h"
#include "Tablero.h"
//cambio1


std::vector<Vector2D> Movimiento::calcularTerrestre(Pieza* pieza, Tablero* tablero) {
    std::vector<Vector2D> movimientos;

    // Solo 4 direcciones: arriba, abajo, izquierda, derecha
    int dirs[4][2] = { {0,-1}, {0,1}, {-1,0}, {1,0} };

    for (auto& dir : dirs)
        expandirDireccion(pieza, tablero, dir[0], dir[1], movimientos);

    return movimientos;
}

// ?? expandirDireccion ???????????????????????????????????????
void Movimiento::expandirDireccion(Pieza* pieza, Tablero* tablero,
    int di, int dj,
    std::vector<Vector2D>& movimientos) {
    // Acceso directo a atributos protegidos de Pieza gracias a friend
    int i = (int)pieza->posicion.x;
    int j = (int)pieza->posicion.y;

    for (int paso = 1; paso <= pieza->rangoMovimiento; paso++) {
        int ni = i + di * paso;
        int nj = j + dj * paso;

        // Verificamos si la nueva posición es válida en el tablero
        if (!tablero->posicionValida(ni, nj)) break;

        Pieza* ocupante = tablero->obtenerOcupante(ni, nj);

        if (ocupante == nullptr) {
            // Vacía: válida
            movimientos.push_back(Vector2D(ni, nj));
        }
        else if (ocupante->bando != pieza->bando) {
            // Enemiga: válida, pero bloquea el camino, paramos después de añadirla
            movimientos.push_back(Vector2D(ni, nj));
            break;
        }
        else {
            // Aliada: bloquea el camino, no es válida, paramos sin añadir
            break;
        }
    }
}

// ?? calcularVolador ??????????????????????????????????????????
std::vector<Vector2D> Movimiento::calcularVolador(Pieza* pieza, Tablero* tablero) {
    std::vector<Vector2D> movimientos;

    // Acceso directo a posicion, bando y rangoMovimiento
    int i = (int)pieza->posicion.x;
    int j = (int)pieza->posicion.y;

    // Cuadrado completo de rango en las 8 direcciones
    // Las voladoras ignoran obstáculos: solo miramos el destino
    for (int di = -pieza->rangoMovimiento; di <= pieza->rangoMovimiento; di++) {
        for (int dj = -pieza->rangoMovimiento; dj <= pieza->rangoMovimiento; dj++) {

            if (di == 0 && dj == 0) continue;  // Casilla propia

            int ni = i + di;
            int nj = j + dj;

            if (!tablero->posicionValida(ni, nj)) continue;

            Pieza* ocupante = tablero->obtenerOcupante(ni, nj);

            // Vacía o enemiga : válida
            if (ocupante == nullptr || ocupante->bando != pieza->bando)
                movimientos.push_back(Vector2D(ni, nj));
        }
    }

    return movimientos;
}

// ?? calcularTeletransporte ???????????????????????????????????
std::vector<Vector2D> Movimiento::calcularTeletransporte(Pieza* pieza, Tablero* tablero) {
    std::vector<Vector2D> movimientos;

    // Recorremos las 81 casillas del tablero entero
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            // Saltamos la casilla donde ya estamos
            if (i == (int)pieza->posicion.x &&
                j == (int)pieza->posicion.y) continue;

            Pieza* ocupante = tablero->obtenerOcupante(i, j);

            // Vacía o enemiga ? válida
            if (ocupante == nullptr || ocupante->bando != pieza->bando)
                movimientos.push_back(Vector2D(i, j));
        }
    }

    return movimientos;
}