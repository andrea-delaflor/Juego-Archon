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
            mundo->setModoMagia(false);
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
        mundo->setModoMagia(false);
        std::cout << "Teletransporte completado." << std::endl;
    }
    else {
        std::cout << "Seleccion de destino invalida para Teleport." << std::endl;
    }
}

//este hechizo no se puede hacer hasta que este la clase vida, pero lo dejo aquí para que se vea la estructura de los hechizos
// 2. HEAL
void HechizoHeal::aplicar(Mundo* mundo, Vector2D destino) {
    Pieza* p = mundo->getTablero().obtenerOcupante((int)destino.x, (int)destino.y);
    if (p) {
        p->restaurarVidaCompleta();
        usado = true;
        mundo->setModoMagia(false);
    }
}

// 3. SHIFT TIME
void HechizoShiftTime::aplicar(Mundo* mundo, Vector2D destino) {
    // Usamos el método público para que Mundo gestione sus variables privadas
    mundo->invertirCicloTiempo();

    std::cout << "Hechizo Shift Time aplicado correctamente." << std::endl;

    usado = true;
    mundo->setModoMagia(false);
}

// 4. EXCHANGE
void HechizoExchange::aplicar(Mundo* mundo, Vector2D destino) {
    Tablero& tablero = mundo->getTablero();
    Pieza* piezaActual = tablero.obtenerOcupante((int)destino.x, (int)destino.y);

    // PASO 1: Elegir la primera pieza
    if (primeraPieza == nullptr) {
        if (piezaActual != nullptr) {
            primeraPieza = piezaActual;
            std::cout << "Primera pieza seleccionada: " << primeraPieza->obtenerNombre() << std::endl;
            std::cout << "Ahora haz click en la segunda pieza para intercambiarlas." << std::endl;
        }
        else {
            std::cout << "Haz click sobre una pieza real." << std::endl;
            mundo->setModoMagia(false);
        }
        return; // Salimos sin marcar 'usado = true' porque falta el segundo click
    }

    // PASO 2: Elegir la segunda pieza y ejecutar
    if (piezaActual != nullptr && piezaActual != primeraPieza) {
        Vector2D pos1 = primeraPieza->obtenerPosicion();
        Vector2D pos2 = piezaActual->obtenerPosicion();

        // Intercambio en el tablero
        tablero.colocarPieza((int)pos2.x, (int)pos2.y, primeraPieza);
        tablero.colocarPieza((int)pos1.x, (int)pos1.y, piezaActual);

        std::cout << "¡INTERCAMBIO COMPLETADO!" << std::endl;

        // Limpiamos para el próximo uso y finalizamos
        primeraPieza = nullptr;
        usado = true;
        mundo->setModoMagia(false);
    }
    else {
        std::cout << "Selecciona una pieza diferente a la primera." << std::endl;
    }
}

// 5. IMPRISON
void HechizoImprison::aplicar(Mundo* mundo, Vector2D destino) {
    Pieza* p = mundo->getTablero().obtenerOcupante((int)destino.x, (int)destino.y);

    // 1. Comprobamos que haya una pieza
    // 2. Comprobamos que el bando de esa pieza sea distinto al bandoActual del mundo
    if (p != nullptr && p->obtenerBando() != mundo->bandoActual()) {

        p->establecerEncarcelada(true, mundo->getValorLuz());

        std::cout << "IMPRISON: " << p->obtenerNombre() << " ha sido neutralizada." << std::endl;
        usado = true;
        mundo->setModoMagia(false);
    }
    else if (p != nullptr) {
        std::cout << "No puedes encarcelar a tus propios aliados." << std::endl;
    }
    else {
        std::cout << "No hay ningun objetivo en esa casilla." << std::endl;
    }
}

// 6. REVIVE
void HechizoRevive::aplicar(Mundo* mundo, Vector2D destino) {
    Tablero& tablero = mundo->getTablero();
    bool esLuz = (mundo->faseActual == Mundo::TURNO_LUZ);
    auto& cementerio = esLuz ? mundo->getCementerioLuz() : mundo->getCementerioOscuridad();

    if (cementerio.empty()) {
        std::cout << "Cementerio vacio. Cancelando hechizo..." << std::endl;
        mundo->setModoMagia(false); // <--- IMPORTANTE: Desbloquea el mundo
        return;
    }

    Pieza* ocupante = tablero.obtenerOcupante((int)destino.x, (int)destino.y);

    // --- MODO SELECCIÓN (Clic en cualquier pieza viva) ---
    if (ocupante != nullptr) {
        std::cout << "\n--- CEMENTERIO ---" << std::endl;
        for (int i = 0; i < (int)cementerio.size(); i++) {
            std::cout << "[" << i << "] " << cementerio[i]->obtenerNombre() << std::endl;
        }

        int actual = mundo->getIndiceSeleccionado();
        actual = (actual + 1) % cementerio.size(); // Ciclo automático
        if (actual < 0) actual = 0;
        
        mundo->setIndiceSeleccionado(actual);
        std::cout << ">> ELEGIDA: [" << actual << "] " << cementerio[actual]->obtenerNombre() << std::endl;
        return; // Seguimos en modo magia esperando el clic en el suelo
    }

    // --- MODO EJECUCIÓN (Clic en suelo vacío) ---
    int idx = mundo->getIndiceSeleccionado();
    if (idx < 0 || idx >= (int)cementerio.size()) idx = 0;

    Pieza* p = cementerio[idx];

    // Acción de revivir
    cementerio.erase(cementerio.begin() + idx);
    p->restaurarVidaCompleta();
    p->establecerPosicion(destino);
    p->forzarPosicionVisual(destino);
    p->establecerViva(true);

    tablero.colocarPieza((int)destino.x, (int)destino.y, p);
    if (esLuz) mundo->getPiezasLuz().push_back(p);
    else mundo->getPiezasOscuridad().push_back(p);

    std::cout << "REVIVE: " << p->obtenerNombre() << " resucitado." << std::endl;
    
    // --- DESBLOQUEO DEL JUEGO ---
    usado = true; 
    mundo->setModoMagia(false);       // Cerramos el modo magia
    mundo->setIndiceSeleccionado(-1); // Reseteamos el índice
}

// 7. SUMMON
/*
Consiste en hacer aparecer a un golem/troll en una casilla vacia
*/
void HechizoSummon::aplicar(Mundo* mundo, Vector2D destino) {
    Tablero& tablero = mundo->getTablero();
    Pieza* ocupante = tablero.obtenerOcupante((int)destino.x, (int)destino.y);

    // Casilla ocupada → avisar y esperar
    if (ocupante != nullptr) {
        std::cout << "La casilla esta ocupada. Elige una casilla VACIA para invocar." << std::endl;
        mundo->setModoMagia(true);
        return;
    }

    // Casilla vacía → invocar pieza según bando
    Pieza* nuevaPieza = nullptr;

    if (mundo->bandoActual() == Bando::LUZ) {
        nuevaPieza = new GolemL(destino);
        mundo->getPiezasLuz().push_back(nuevaPieza);
        std::cout << "SUMMON: Golem invocado en ("
            << (int)destino.x << "," << (int)destino.y << ")." << std::endl;
    }
    else {
        nuevaPieza = new TrollO(destino);
        mundo->getPiezasOscuridad().push_back(nuevaPieza);
        std::cout << "SUMMON: Troll invocado en ("
            << (int)destino.x << "," << (int)destino.y << ")." << std::endl;
    }

    tablero.colocarPieza((int)destino.x, (int)destino.y, nuevaPieza);
    usado = true;
    mundo->setModoMagia(false);

}


