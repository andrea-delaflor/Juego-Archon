#include "Mundo.h"
#include "glut.h"
#include <math.h>
#include "Vector2D.h"
#include <iostream>

Mundo::Mundo() {
    seleccionada = nullptr;
}

Mundo::~Mundo() {
    for (auto p : piezasLuz) delete p;
    for (auto p : piezasOscuridad) delete p;
}

void Mundo::inicializa(int estado) {
    for (auto p : piezasLuz) delete p;
    for (auto p : piezasOscuridad) delete p;
    piezasLuz.clear();
    piezasOscuridad.clear();
    tablero.vaciar();

    valorLuz = 0.5f;
    angulo = 0.0f;
    seleccionada = nullptr;
    resetCombate();

    switch (estado) {
    case 0:
        break;
    case 1:
        break;
    case 2: // JUEGO
        faseActual = TURNO_LUZ;
        seleccionada = nullptr;

        piezasLuz.push_back(new GolemL(Vector2D(6, 4)));
        piezasLuz.push_back(new MagoL(Vector2D(0, 4)));
        piezasLuz.push_back(new DjiniL(Vector2D(2, 4)));
        piezasLuz.push_back(new ArqueraL(Vector2D(1, 2)));
        piezasLuz.push_back(new FenixL(Vector2D(1, 3)));

        piezasOscuridad.push_back(new BrujaO(Vector2D(8, 4)));
        piezasOscuridad.push_back(new DragonO(Vector2D(6, 6)));
        piezasOscuridad.push_back(new BasiliscoO(Vector2D(7, 6)));
        piezasOscuridad.push_back(new TrollO(Vector2D(3, 2)));
        piezasOscuridad.push_back(new BansheeO(Vector2D(5, 2)));

        for (auto p : piezasLuz) {
            tablero.colocarPieza((int)p->obtenerPosicion().x, (int)p->obtenerPosicion().y, p);
        }
        for (auto p : piezasOscuridad) {
            tablero.colocarPieza((int)p->obtenerPosicion().x, (int)p->obtenerPosicion().y, p);
        }
        break;
    }
}

void Mundo::mueve() {
    angulo += 0.05f;
    valorLuz = (sin(angulo) + 1.0f) / 2.0f;

    switch (faseActual) {
    case ANIMANDO_MOVIMIENTO:
        if (seleccionada != nullptr && !seleccionada->estaAnimando()) {

            if (hayCombate) {
                // El Coordinador detectará el combate
            }
            else {
                if (seleccionada->obtenerBando() == Bando::LUZ) {
                    faseActual = TURNO_OSCURIDAD;
                    std::cout << "--> TURNO DE LA OSCURIDAD" << std::endl;
                }
                else {
                    faseActual = TURNO_LUZ;
                    std::cout << "--> TURNO DE LA LUZ" << std::endl;
                }
                seleccionada = nullptr;
            }
        }
        break;

    case TURNO_LUZ:
    case TURNO_OSCURIDAD:
    case FIN_PARTIDA:
        break;
    }
}

void Mundo::dibuja(int estado) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-6.0, 6.0, -6.0, 6.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch (estado) {
    case 0:
        break;

    case 1:
        tablero.dibuja(1.0f);
        break;

    case 2:
        tablero.dibuja(valorLuz);
        raton.dibuja();

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor3ub(255, 255, 255);
        for (auto p : piezasLuz) p->dibuja();
        for (auto p : piezasOscuridad) p->dibuja();
        break;

    case 3:
        tablero.dibuja(0.3f);
        if (seleccionada != nullptr) {
            glColor3f(1.0f, 1.0f, 0.0f);
        }
        else {
            glColor3f(1.0f, 0.0f, 0.0f);
        }
        raton.dibuja();
        break;
    }

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void Mundo::clickRaton(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    raton.actualizaPosicion(x, y, 800, 800);
    Vector2D c = raton.casilla;

    if (c.x == -1) return;

    switch (faseActual) {
    case TURNO_LUZ:
    case TURNO_OSCURIDAD:

        if (seleccionada == nullptr) {
            Pieza* piezaEnCasilla = tablero.obtenerOcupante((int)c.x, (int)c.y);

            if (piezaEnCasilla != nullptr) {
                if ((faseActual == TURNO_LUZ && piezaEnCasilla->obtenerBando() == Bando::LUZ) ||
                    (faseActual == TURNO_OSCURIDAD && piezaEnCasilla->obtenerBando() == Bando::OSCURIDAD)) {

                    seleccionada = piezaEnCasilla;
                }
            }
        }
        else {
            std::vector<Vector2D> validos = seleccionada->obtenerMovimientosValidos(&tablero);
            bool esDestinoValido = false;

            for (auto& v : validos) {
                if ((int)v.x == (int)c.x && (int)v.y == (int)c.y) {
                    esDestinoValido = true;
                    break;
                }
            }

            if (esDestinoValido) {
                Pieza* piezaDestino = tablero.obtenerOcupante((int)c.x, (int)c.y);

                if (piezaDestino != nullptr && piezaDestino->obtenerBando() != seleccionada->obtenerBando()) {
                    hayCombate = true;
                    atacante = seleccionada;
                    defensor = piezaDestino;
                }
                else {
                    hayCombate = false;
                }

                Vector2D posAntigua = seleccionada->obtenerPosicion();
                tablero.colocarPieza((int)posAntigua.x, (int)posAntigua.y, nullptr);

                seleccionada->establecerPosicion(c);
                tablero.colocarPieza((int)c.x, (int)c.y, seleccionada);

                faseActual = ANIMANDO_MOVIMIENTO;
            }
            else {
                seleccionada = nullptr;
            }
        }
        break;

    case ANIMANDO_MOVIMIENTO:
    case FIN_PARTIDA:
        break;
    }
    glutPostRedisplay();
}

// ¡NUEVO! Función que arranca una pieza de cuajo del juego para que desaparezca
void Mundo::eliminarPieza(Pieza* p) {
    if (p == nullptr) return;

    // Buscamos si la pieza está en el bando de la luz y la borramos
    for (auto it = piezasLuz.begin(); it != piezasLuz.end(); ++it) {
        if (*it == p) {
            piezasLuz.erase(it);
            return;
        }
    }
    // Buscamos si la pieza está en el bando de la oscuridad y la borramos
    for (auto it = piezasOscuridad.begin(); it != piezasOscuridad.end(); ++it) {
        if (*it == p) {
            piezasOscuridad.erase(it);
            return;
        }
    }
}

void Mundo::finalizaCombate(Pieza* ganador, Pieza* perdedor, bool empate) {

    // Guardamos la casilla y de qué bando era el atacante ANTES de borrar nada
    Vector2D casillaCombate = atacante->obtenerPosicion();
    Bando turnoAtacante = atacante->obtenerBando();

    // ¡NUEVO COMPORTAMIENTO SEGURO!
    if (empate) {
        // 1. Vaciamos la casilla del tablero (la dejamos vacía)
        tablero.colocarPieza((int)casillaCombate.x, (int)casillaCombate.y, nullptr);

        // 2. Usamos nuestra nueva función para borrarlos de las listas visuales
        eliminarPieza(atacante);
        eliminarPieza(defensor);

        std::cout << "¡EMPATE! Ambos luchadores han sido eliminados del juego." << std::endl;
    }
    else {
        // 1. Colocamos al ganador en el tablero
        tablero.colocarPieza((int)casillaCombate.x, (int)casillaCombate.y, ganador);
        ganador->establecerPosicion(casillaCombate);

        // 2. Eliminamos al perdedor de las listas visuales
        eliminarPieza(perdedor);

        std::cout << "Gana el combate: " << ganador->obtenerNombre() << std::endl;
    }

    // 3. Cambiamos de turno con seguridad
    if (turnoAtacante == Bando::LUZ) {
        faseActual = TURNO_OSCURIDAD;
        std::cout << "--> TURNO DE LA OSCURIDAD" << std::endl;
    }
    else {
        faseActual = TURNO_LUZ;
        std::cout << "--> TURNO DE LA LUZ" << std::endl;
    }

    // 4. Bajamos los interruptores rojos del combate
    resetCombate();
    seleccionada = nullptr;
}