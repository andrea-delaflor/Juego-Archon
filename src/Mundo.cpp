#include "Mundo.h"
#include "glut.h"
#include <math.h>
#include "Vector2D.h"
#include <iostream>

Mundo::Mundo() {
    modoMagiaActivo = false;
    hechizoSeleccionado = nullptr;
    seleccionada = nullptr;
}

Mundo::~Mundo() {
    // Borrar piezas activas
    for (auto p : piezasLuz) delete p;
    for (auto p : piezasOscuridad) delete p;
    // Borrar piezas muertas (cementerio)
    for (auto p : cementerioLuz) delete p;
    for (auto p : cementerioOscuridad) delete p;
    // Borrar hechizos
    for (auto h : libroLuz) delete h;
    for (auto h : libroOscuridad) delete h;
}
void Mundo::inicializa(int estado) {
    // Limpieza previa
    for (auto p : piezasLuz) delete p;
    for (auto p : piezasOscuridad) delete p;
    for (auto h : libroLuz) delete h;
    for (auto h : libroOscuridad) delete h;

    piezasLuz.clear();
    piezasOscuridad.clear();
    cementerioLuz.clear();
    cementerioOscuridad.clear();
    libroLuz.clear();
    libroOscuridad.clear();
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

		// Inicializar piezas en el tablero
        // Luz
        piezasLuz.push_back(new GolemL(Vector2D(6, 4)));
        piezasLuz.push_back(new MagoL(Vector2D(0, 4)));
        piezasLuz.push_back(new DjiniL(Vector2D(2, 4)));
        piezasLuz.push_back(new ArqueraL(Vector2D(1, 2)));
        piezasLuz.push_back(new FenixL(Vector2D(1, 3)));
		// Oscuridad
        piezasOscuridad.push_back(new BrujaO(Vector2D(8, 4)));
        piezasOscuridad.push_back(new DragonO(Vector2D(6, 6)));
        piezasOscuridad.push_back(new BasiliscoO(Vector2D(7, 6)));
        piezasOscuridad.push_back(new TrollO(Vector2D(3, 2)));
        piezasOscuridad.push_back(new BansheeO(Vector2D(5, 2)));

		// Inicializar libros de hechizos 
        // Luz
        libroLuz.push_back(new HechizoTeleport());
        libroLuz.push_back(new HechizoHeal());
        libroLuz.push_back(new HechizoRevive());
        libroLuz.push_back(new HechizoShiftTime());
        // Oscuridad
        libroOscuridad.push_back(new HechizoTeleport());
        libroOscuridad.push_back(new HechizoHeal());
        libroOscuridad.push_back(new HechizoRevive());
        libroOscuridad.push_back(new HechizoShiftTime());

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


void Mundo::teclahechizos(unsigned char key) {


    std::cout << "Tecla pulsada: " << key << " | Modo Magia: " << modoMagiaActivo << std::endl;


    // 1. Elegimos el cementerio según el turno
    std::vector<Pieza*>& cementerio = (faseActual == TURNO_LUZ) ? cementerioLuz : cementerioOscuridad;

    // 2. Si ya elegimos el hechizo "Revive" y estamos esperando la pieza muerta
    if (esperandoEleccionRevive) {
        if (key >= '1' && key <= '9') {
            int num = key - '1';
            if (num < (int)cementerio.size()) {
                indiceSeleccionado = num;
                esperandoEleccionRevive = false;
                // Pieza suele usar obtenerNombre()
                std::cout << "Has elegido resucitar a: " << cementerio[num]->obtenerNombre() << std::endl;
                std::cout << "Ahora haz click en una casilla vacia del tablero." << std::endl;
            }
        }
        return;
    }

    // 3. Si NO hemos elegido hechizo todavía, los números eligen el hechizo del libro
    if (modoMagiaActivo && hechizoSeleccionado == nullptr) {
        if (key >= '1' && key <= '7') {
            int idx = key - '1';

            // CORRECCIÓN: Usamos los nombres de los vectores de Mundo.h
            std::vector<Hechizo*>& libro = (faseActual == TURNO_LUZ) ? libroLuz : libroOscuridad;

            if (idx < (int)libro.size()) {
                // Comprobamos si ya está usado antes de seleccionarlo
                if (libro[idx]->estaUsado()) {
                    std::cout << "Este hechizo ya ha sido agotado." << std::endl;
                    return;
                }

                hechizoSeleccionado = libro[idx];
                // Hechizo usa getNombre() según tu Hechizo.h
                std::cout << "Seleccionado: " << hechizoSeleccionado->getNombre() << std::endl;

                // Caso especial: Hechizo Revive
                if (hechizoSeleccionado->getNombre() == "Revive") {
                    if (cementerio.empty()) {
                        std::cout << "No hay nadie para resucitar. Elige otro hechizo." << std::endl;
                        hechizoSeleccionado = nullptr;
                    }
                    else {
                        esperandoEleccionRevive = true;
                        std::cout << "--- CEMENTERIO ---" << std::endl;
                        for (int i = 0; i < (int)cementerio.size(); i++) {
                            std::cout << i + 1 << ": " << cementerio[i]->obtenerNombre() << std::endl;
                        }
                    }
                }
                else {
                    std::cout << "Haz click en el tablero para aplicar el hechizo." << std::endl;
                }
            }
        }
    }
}

void Mundo::clickRaton(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    //Preguntamos tamaño real pnatalla
    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);

    raton.actualizaPosicion(x, y, ancho, alto);
    Vector2D c = raton.casilla;

    if (c.x == -1) return;

    // --- LÓGICA DE APLICAR HECHIZO ---
    if (modoMagiaActivo && hechizoSeleccionado != nullptr) {
        std::cout << "Aplicando magia..." << std::endl;
        hechizoSeleccionado->aplicar(this, c);

        // 2. RESETEO OBLIGATORIO DE ESTADOS
         modoMagiaActivo = false;
        hechizoSeleccionado = nullptr;
        seleccionada = nullptr; // Deseleccionamos al Mago tras la magia
        
        /*
        // Si el hechizo se marcó como usado, pasamos turno
        if (hechizoSeleccionado->estaUsado()) {
            faseActual = (faseActual == TURNO_LUZ) ? TURNO_OSCURIDAD : TURNO_LUZ;
            std::cout << "Hechizo lanzado con exito. Cambio de turno." << std::endl;
        }
        */
        // 3. CAMBIO DE TURNO FORZADO
        if (faseActual == TURNO_LUZ) {
            faseActual = TURNO_OSCURIDAD;
            std::cout << "--> TURNO DE LA OSCURIDAD" << std::endl;
        }
        else {
            faseActual = TURNO_LUZ;
            std::cout << "--> TURNO DE LA LUZ" << std::endl;
        }

        // 4. ACTUALIZAMOS PANTALLA Y SALIMOS DE LA FUNCIÓN
        glutPostRedisplay();
        return; //
        
    }

    switch (faseActual) {
    case TURNO_LUZ:
    case TURNO_OSCURIDAD:

        if (seleccionada == nullptr) {
            Pieza* piezaEnCasilla = tablero.obtenerOcupante((int)c.x, (int)c.y);

            if (piezaEnCasilla != nullptr) {
                if ((faseActual == TURNO_LUZ && piezaEnCasilla->obtenerBando() == Bando::LUZ) ||
                    (faseActual == TURNO_OSCURIDAD && piezaEnCasilla->obtenerBando() == Bando::OSCURIDAD)) {

                    seleccionada = piezaEnCasilla;

                    if (seleccionada->esLider()) {
                        std::cout << "Lider seleccionado. Pulsa 1-7 para magia o mueve." << std::endl;
                        modoMagiaActivo = true;
                    }
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
                // Si movemos físicamente al mago, cancelamos el modo magia por si lo había activado
                modoMagiaActivo = false;
                Pieza* piezaDestino = tablero.obtenerOcupante((int)c.x, (int)c.y);

                if (piezaDestino != nullptr && piezaDestino->obtenerBando() != seleccionada->obtenerBando()) {
                    hayCombate = true;
                    atacante = seleccionada;
                    defensor = piezaDestino;
                    tipoArenaCombate = tablero.obtenerTipoArena((int)c.x, (int)c.y, valorLuz);
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


// Función que saca una pieza de las listas activas y la manda al cementerio
void Mundo::eliminarPieza(Pieza* p) {
    if (p == nullptr) return;

    // 1. Antes de sacarla, la marcamos como no viva (por si acaso)
    p->establecerViva(false);

    // 2. Buscamos en qué bando está para meterla en su cementerio correspondiente
    if (p->obtenerBando() == Bando::LUZ) {
        // La metemos en la lista de muertas de luz
        cementerioLuz.push_back(p);

        // La borramos de la lista de piezas activas (visuales)
        for (auto it = piezasLuz.begin(); it != piezasLuz.end(); ++it) {
            if (*it == p) {
                piezasLuz.erase(it);
                break; // Salimos del bucle una vez encontrada
            }
        }
    }
    else {
        // La metemos en la lista de muertas de oscuridad
        cementerioOscuridad.push_back(p);

        // La borramos de la lista de piezas activas (visuales)
        for (auto it = piezasOscuridad.begin(); it != piezasOscuridad.end(); ++it) {
            if (*it == p) {
                piezasOscuridad.erase(it);
                break;
            }
        }
    }

    
}
void Mundo::finalizaCombate(Pieza* ganador, Pieza* perdedor, bool empate) {

    // Guardamos la casilla y de qué bando era el atacante ANTES de borrar nada
    Vector2D casillaCombate = atacante->obtenerPosicion();
    Bando turnoAtacante = atacante->obtenerBando();

    // COMPORTAMIENTO SEGURO!
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