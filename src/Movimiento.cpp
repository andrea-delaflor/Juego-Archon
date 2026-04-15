#include "Movimiento.h"
#include "Pieza.h"
#include "Tablero.h"

// que trayectoria se usa en funcion de que pieza es
TipoTrayectoria Movimiento::obtenerTipoTrayectoria(Pieza* pieza) {
    
    return (TipoTrayectoria)pieza->obtenerTipoMovimiento();
}

// ── calcularTerrestre ──
std::vector<Vector2D> Movimiento::calcularTerrestre(Pieza* pieza, Tablero* tablero) {
    std::vector<Vector2D> movimientos;

    // Solo 4 direcciones: arriba, abajo, izquierda, derecha
    int dirs[4][2] = { {0,-1}, {0,1}, {-1,0}, {1,0} };

    for (auto& dir : dirs)
        expandirDireccion(pieza, tablero, dir[0], dir[1], movimientos);

    return movimientos;
}

// Avanza una dirección hasta obstáculo o borde
void Movimiento::expandirDireccion(Pieza* pieza, Tablero* tablero,
    int di, int dj,
    std::vector<Vector2D>& movimientos) {

    // accede a atributos protegidos
    int i = (int)pieza->obtenerPosicion().x;
    int j = (int)pieza->obtenerPosicion().y;
    int rango = pieza->obtenerRangoMovimiento();
    Bando miBando = pieza->obtenerBando();

    for (int paso = 1; paso <= rango; paso++) {
        int ni = i + di * paso;
        int nj = j + dj * paso;

        // verificamos si la nueva pieza es válida en el tablero
        if (!tablero->posicionValida(ni, nj)) break;

        Pieza* ocupante = tablero->obtenerOcupante(ni, nj);

        if (ocupante == nullptr) {
            // si esta vacía es valida
            movimientos.push_back(Vector2D(ni, nj));
        }
        else if (ocupante->obtenerBando() != miBando) {
            // válida,enemiga bloquea camino,  se para despues de añadir
            movimientos.push_back(Vector2D(ni, nj));
            break;
        }
        else {
            //  valida, aliada bloquea camino, paramos sin añadir
            break;
        }
    }
}

// ── calcularVolador ───
std::vector<Vector2D> Movimiento::calcularVolador(Pieza* pieza, Tablero* tablero) {
    std::vector<Vector2D> movimientos;

    // accede a atributos protegidos 
    int i = (int)pieza->obtenerPosicion().x;
    int j = (int)pieza->obtenerPosicion().y;
    int rango = pieza->obtenerRangoMovimiento();
    Bando miBando = pieza->obtenerBando();

    // Cuadrado completo de rango en las 8 direcciones
    // Las voladoras ignoran obstáculos  solo miramos punto final
    for (int di = -rango; di <= rango; di++) {
        for (int dj = -rango; dj <= rango; dj++) {

            if (di == 0 && dj == 0) continue;  // Casilla propia

            int ni = i + di;
            int nj = j + dj;

            if (!tablero->posicionValida(ni, nj)) continue;

            Pieza* ocupante = tablero->obtenerOcupante(ni, nj);

            // Vacía o enemiga : válido
            if (ocupante == nullptr || ocupante->obtenerBando() != miBando)
                movimientos.push_back(Vector2D(ni, nj));
        }
    }

    return movimientos;
}

// ── calcularTeletransporte ────
std::vector<Vector2D> Movimiento::calcularTeletransporte(Pieza* pieza, Tablero* tablero) {
    std::vector<Vector2D> movimientos;

    
    int miX = (int)pieza->obtenerPosicion().x;
    int miY = (int)pieza->obtenerPosicion().y;
    Bando miBando = pieza->obtenerBando();

    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            // Saltamos la casilla donde ya estamos
            if (i == miX && j == miY) continue;

            Pieza* ocupante = tablero->obtenerOcupante(i, j);

            
            if (ocupante == nullptr || ocupante->obtenerBando() != miBando)
                movimientos.push_back(Vector2D(i, j));
        }
    }

    return movimientos;
}