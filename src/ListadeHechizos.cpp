#include "ListadeHechizos.h"
#include "Mundo.h"
#include "Tablero.h"
#include "Pieza.h"
#include <iostream>

// 1. TELEPORT
void HechizoTeleport::aplicar(Mundo* mundo, Vector2D destino) {
    Tablero& tablero = mundo->getTablero();
    Pieza* piezaEnClick = tablero.obtenerOcupante((int)destino.x, (int)destino.y);

    // PASO 1: Si la pieza seleccionada es el Mago, el jugador está eligiendo a quién teletransportar
    // (O si no hay nada seleccionado aún)
    if (mundo->seleccionada == nullptr || mundo->seleccionada->esLider()) {
        if (piezaEnClick != nullptr && piezaEnClick->obtenerBando() == mundo->bandoActual()) {
            mundo->seleccionada = piezaEnClick;
            std::cout << "Pieza elegida para Teleport: " << piezaEnClick->obtenerNombre() << std::endl;
            std::cout << "Ahora selecciona el destino vacio." << std::endl;
            // IMPORTANTE: No ponemos 'usado = true' porque solo hemos seleccionado
            return;
        }
    }

    // PASO 2: Si ya tenemos una pieza seleccionada (que no es el líder o ya es el objetivo)
    // y el destino está vacío, ejecutamos el salto.
    if (mundo->seleccionada != nullptr && piezaEnClick == nullptr) {
        Vector2D antigua = mundo->seleccionada->obtenerPosicion();
        tablero.colocarPieza((int)antigua.x, (int)antigua.y, nullptr);

        mundo->seleccionada->establecerPosicion(destino);
        tablero.colocarPieza((int)destino.x, (int)destino.y, mundo->seleccionada);

        usado = true; // AQUÍ es donde se gasta el hechizo
        std::cout << "Teletransporte completado." << std::endl;
    }
    else {
        std::cout << "Seleccion de destino invalida para Teleport." << std::endl;
    }
}

// 2. HEAL
void HechizoHeal::aplicar(Mundo* mundo, Vector2D destino) {
    Pieza* p = mundo->getTablero().obtenerOcupante((int)destino.x, (int)destino.y);
    if (p) {
        p->restaurarVidaCompleta();
        usado = true;
    }
}

// 3. SHIFT TIME
void HechizoShiftTime::aplicar(Mundo* mundo, Vector2D destino) {
    mundo->setValorLuz(1.0f - mundo->getValorLuz());
    usado = true;
}

// 4. EXCHANGE
void HechizoExchange::aplicar(Mundo* mundo, Vector2D destino) {
    Tablero& tablero = mundo->getTablero();
    Pieza* p1 = mundo->seleccionada;
    Pieza* p2 = tablero.obtenerOcupante((int)destino.x, (int)destino.y);

    if (p1 && p2 && p1 != p2) {
        Vector2D pos1 = p1->obtenerPosicion();
        Vector2D pos2 = p2->obtenerPosicion();
        p1->establecerPosicion(pos2);
        p2->establecerPosicion(pos1);
        tablero.colocarPieza((int)pos1.x, (int)pos1.y, p2);
        tablero.colocarPieza((int)pos2.x, (int)pos2.y, p1);
        usado = true;
    }
}

// 5. IMPRISON
void HechizoImprison::aplicar(Mundo* mundo, Vector2D destino) {
    Pieza* p = mundo->getTablero().obtenerOcupante((int)destino.x, (int)destino.y);
    if (p) {
        p->establecerEncarcelada(true);
        usado = true;
    }
}

// 6. REVIVE
void HechizoRevive::aplicar(Mundo* mundo, Vector2D destino) {
    Tablero& tablero = mundo->getTablero();
    bool esLuz = (mundo->faseActual == Mundo::TURNO_LUZ);
    std::vector<Pieza*>& cementerio = esLuz ? mundo->getCementerioLuz() : mundo->getCementerioOscuridad();
    int idx = mundo->getIndiceSeleccionado();

    if (idx >= 0 && idx < (int)cementerio.size()) {
        Pieza* p = cementerio[idx];
        cementerio.erase(cementerio.begin() + idx);
        p->restaurarVidaCompleta();
        p->establecerPosicion(destino);
        p->establecerViva(true);
        tablero.colocarPieza((int)destino.x, (int)destino.y, p);
        if (esLuz) mundo->getPiezasLuz().push_back(p);
        else mundo->getPiezasOscuridad().push_back(p);
        usado = true;
        mundo->setIndiceSeleccionado(-1);
    }
}

// 7. SUMMON
void HechizoSummon::aplicar(Mundo* mundo, Vector2D destino) {
    if (mundo->getTablero().obtenerOcupante((int)destino.x, (int)destino.y) == nullptr) {
        // Aquí puedes crear una pieza genérica o específica
        usado = true;
    }
}