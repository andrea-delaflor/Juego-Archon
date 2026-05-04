#include "ListadeHechizos.h"
#include "Mundo.h"
#include "Tablero.h"
#include "Pieza.h"
#include <iostream>

// 1. TELEPORT
void HechizoTeleport::aplicar(Mundo* mundo, Vector2D destino) {
    Tablero& tablero = mundo->getTablero();

    // IMPORTANTE: Buscamos qué hay en las coordenadas del clic AQUÍ dentro
    Pieza* piezaEnClick = tablero.obtenerOcupante((int)destino.x, (int)destino.y);

    // PASO 1: Seleccionar la pieza (seleccionada es nullptr porque lo pusimos así en la tecla)
    if (mundo->seleccionada == nullptr) {

        if (piezaEnClick != nullptr) { // ¿Hay una pieza?
            if (piezaEnClick->obtenerBando() == mundo->bandoActual()) { // ¿Es nuestra?
                if (!piezaEnClick->esLider()) { // ¿No es el mago?

                    // ASIGNACIÓN CRUCIAL
                    mundo->seleccionada = piezaEnClick;

                    std::cout << "PIEZA SELECCIONADA: " << piezaEnClick->obtenerNombre() << std::endl;
                    std::cout << "Ahora haz click en una casilla VACIA para moverla." << std::endl;
                }
                else {
                    std::cout << "No puedes teletransportar al lider." << std::endl;
                }
            }
            else {
                std::cout << "Esa pieza es enemiga." << std::endl;
            }
        }
        else {
            std::cout << "Has clicado en suelo vacio. Selecciona una pieza primero." << std::endl;
        }

        // Salimos para que el siguiente clic sea el destino
        return;
    }

    // PASO 2: Mover la pieza (Solo entra aquí si mundo->seleccionada ya NO es nullptr)
    else {
        if (piezaEnClick == nullptr) { // Destino vacío
            Vector2D antigua = mundo->seleccionada->obtenerPosicion();

            // Intercambio en el tablero
            tablero.colocarPieza((int)antigua.x, (int)antigua.y, nullptr);
            mundo->seleccionada->establecerPosicion(destino);
            tablero.colocarPieza((int)destino.x, (int)destino.y, mundo->seleccionada);

            // Marcar como finalizado
            this->usado = true;
            // IMPORTANTE: No limpies mundo->seleccionada aquí si quieres 
            // que Mundo::clickRaton lo haga al detectar que estaUsado() es true
            std::cout << "Hechizo ejecutado con exito." << std::endl;
        }
        else {
            std::cout << "Casilla ocupada. Elige otra." << std::endl;
        }
    }
}

// 2. HEAL
//Cura a las piezas del MISMO bando a su vida max
void HechizoHeal::aplicar(Mundo* mundo, Vector2D destino) {
    Pieza* p = mundo->getTablero().obtenerOcupante((int)destino.x, (int)destino.y);
    if (p == nullptr) {
        //si la casilla no tiene pieza---> no es valido hay que esperar que haga click en una casilla con pieza
        std::cout << "Casilla vacia. Selecciona una pieza ALIADA para curarla." << std::endl;
        mundo->setModoMagia(true);   // seguimos esperando
        return;
    }

    if (p->obtenerBando() != mundo->bandoActual()) {
        //si la pieza NO es de NUESTRO bando no se puede avisamos
        std::cout << "No puedes curar a los enemigos! Selecciona una pieza ALIADA." << std::endl;
        mundo->setModoMagia(true);   // seguimos esperando
        return;
    }

    //CURAR
    p->getVida().restaurarAlMaximo();
    std::cout << "HEAL: " << p->obtenerNombre() << " ha recuperado toda su vida." << std::endl;
    usado = true;
    mundo->setModoMagia(false);
}

// 3. SHIFT TIME
void HechizoShiftTime::aplicar(Mundo* mundo, Vector2D destino) {
    // Usamos el método público para que Mundo gestione sus variables privadas
    mundo->invertirCicloTiempo();

    std::cout << "Hechizo Shift Time aplicado correctamente." << std::endl;

    usado = true;
    mundo->setModoMagia(false);
    mundo->seleccionada = nullptr; //para bloquear el mago y no se mueva tras conjurar

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
    
    bool esLuz = (mundo->faseActual == Mundo::TURNO_LUZ);
    auto& cementerio = esLuz ? mundo->getCementerioLuz() : mundo->getCementerioOscuridad();


    if (cementerio.empty()) {
        std::cout << "Cementerio vacio. Cancelando hechizo..." << std::endl;
        mundo->setModoMagia(false); // <--- IMPORTANTE: Desbloquea el mundo
        return;
    }

    // --- MODO EJECUCIÓN (Clic en suelo vacío) ---
    int idx = mundo->getIndiceSeleccionado();
    if (idx < 0 || idx >= (int)cementerio.size()) idx = 0;

    Tablero& tablero = mundo->getTablero();


    // Si la casilla esta ocupada esperamos a que den otro click en otro lado
    if (tablero.obtenerOcupante((int)destino.x, (int)destino.y) != nullptr) {
        std::cout << "Casilla ocupada. Elige una casilla VACIA para revivir a: "
            << cementerio[idx]->obtenerNombre() << std::endl;
        mundo->setModoMagia(true);  // seguimos esperando
        return;
    }

    // Acción de revivir
    Pieza* p = cementerio[idx];
    cementerio.erase(cementerio.begin() + idx);

    p->restaurarVidaCompleta();
    p->establecerViva(true);

    //Colocamos pieza revivida en el tablero
    p->forzarPosicionVisual(destino);
    tablero.colocarPieza((int)destino.x, (int)destino.y, p);

    if (esLuz) mundo->getPiezasLuz().push_back(p);
    else mundo->getPiezasOscuridad().push_back(p);

    std::cout << "REVIVE: " << p->obtenerNombre() << " resucitado." << std::endl;
    std::cout << "Ha vuelto en (" << (int)destino.x << "," << (int)destino.y << ")." << std::endl;
    
    // --- DESBLOQUEO DEL JUEGO ---
    usado = true; 
    mundo->setModoMagia(false);       // Cerramos el modo magia
    mundo->setIndiceSeleccionado(-1); // Reseteamos el índice
}


// 7. SUMMON
/*
Consiste en hacer aparecer a un golem/troll en una casilla vacia
*/
/*
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
*/

