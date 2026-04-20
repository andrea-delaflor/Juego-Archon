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

void Mundo::actualizarVidaPiezas() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Pieza* p = tablero.obtenerOcupante(i, j);
            if (p != nullptr) {
                // Usamos tu clase Vida.h
                if (tablero.esPowerPoint(i, j)) {
                    // Curación rápida en puntos de poder (ej. medio corazón)
                    p->getVida().heal(10);
                    std::cout << p->obtenerNombre() << " se cura rapido en punto de poder." << std::endl;
                }
                else {
                    // Curación lenta en cualquier casilla (ej. 2 puntos)
                    p->getVida().heal(2);
                }
            }
        }
    }
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
        libroLuz.push_back(new HechizoShiftTime());
		libroLuz.push_back(new HechizoExchange());
        libroLuz.push_back(new HechizoImprison());
        libroLuz.push_back(new HechizoRevive());
		libroLuz.push_back(new HechizoSummon());
        
        // Oscuridad
        libroOscuridad.push_back(new HechizoTeleport());
        libroOscuridad.push_back(new HechizoHeal());
        libroOscuridad.push_back(new HechizoShiftTime());
		libroOscuridad.push_back(new HechizoExchange());
        libroOscuridad.push_back(new HechizoImprison());
        libroOscuridad.push_back(new HechizoRevive());
		libroOscuridad.push_back(new HechizoSummon());
       

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
    // 1. ACTUALIZAR EL RELOJ DEL MUNDO
    angulo += 0.01f;
    valorLuz = (sin(angulo) + 1.0f) / 2.0f;

    // 2. LÓGICA DE LIBERACIÓN (IMPRISON)
    // Recorremos el tablero buscando piezas encarceladas
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Pieza* p = tablero.obtenerOcupante(i, j);
            if (p && p->estaEncarcelada()) {
                // Comparamos la luz actual con la luz que había cuando fue encerrada
                // Usamos fabsf (valor absoluto) para ver cuánto ha cambiado el tablero
                float diferencia = fabsf(valorLuz - p->getLuzDeCaptura());

                // Si la diferencia es grande (ej: de 0.1 a 0.9), significa que el ciclo
                // ha pasado de oscuro a claro (o viceversa) y el hechizo se rompe.
                if (diferencia > 0.8f) {
                    p->establecerEncarcelada(false,0.0f);
                    std::cout << "EL SELLO SE ROMPE: " << p->obtenerNombre() << " es libre." << std::endl;
                }
            }
        }
    }

    // 3. GESTIÓN DE TURNOS Y ANIMACIONES

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
                comprobarVictoria(); //para saber ganador
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
         
        // LÓGICA PARA INFO VIDA PIEZAS CON CURSOR RATÓN
        {
            Vector2D c = raton.casilla; // Obtenemos la casilla del ratón
            if (c.x != -1) { // Si el ratón está sobre el tablero
                Pieza* pBajoRaton = tablero.obtenerOcupante((int)c.x, (int)c.y);
                if (pBajoRaton != nullptr) {

                    // Coordenadas visuales (ajustadas al sistema -6 a 6)
                    float x_gl = (float)c.x - 4.0f;
                    float y_gl = 4.0f - (float)c.y;

                    glPushMatrix();
                    // Movemos el origen de dibujo un poco arriba y a la derecha de la pieza
                    glTranslatef(x_gl - 1.6f, y_gl + 1.2f, 0.0f);

                    // DIBUJAR FONDO DE ALTO CONTRASTE (Cian oscuro o similar)
                    glDisable(GL_LIGHTING);
                    glDisable(GL_DEPTH_TEST);
                    glDisable(GL_TEXTURE_2D);

                    // COLOR CONSTANTE (Gris oscuro)
                    glColor4f(0.1f, 0.1f, 0.1f, 0.9f);

                    // 2. DIMENSIONES (Ancho grande para nombres largos)
                    float x_min = -0.4f;
                    float x_max = 4.1f;
                    float y_min = -0.2f;
                    float y_max = 0.4f;

                     // Caja
                    glBegin(GL_QUADS);
                    glVertex2f(x_min, y_min);
                    glVertex2f(x_max, y_min);
                    glVertex2f(x_max, y_max);
                    glVertex2f(x_min, y_max);
                    glEnd();

                    // BORDE BLANCO
                    glColor3f(1.0f, 1.0f, 1.0f);
                    glLineWidth(2.0f);
                    glBegin(GL_LINE_LOOP);
                    glVertex2f(x_min, y_min);
                    glVertex2f(x_max, y_min);
                    glVertex2f(x_max, y_max);
                    glVertex2f(x_min, y_max);
                    glEnd();

                    // DIBUJAR TEXTO (En el mismo origen exacto)
                    glEnable(GL_TEXTURE_2D);
                    // Limpiamos profundidad para que el texto siempre gane
                    glClear(GL_DEPTH_BUFFER_BIT);

                    std::string info = pBajoRaton->obtenerNombre() + ": " +
                        std::to_string(pBajoRaton->obtenerVida()) + " de vida";

                    ETSIDI::setTextColor(1, 1, 1); // Blanco
                    ETSIDI::setFont("fuentes/bitwise.ttf", 14);

                    // Dibujamos en 0,0 porque ya hicimos el glTranslatef
                    ETSIDI::printxy(info.c_str(), 0.0f, 0.0f);

                    glPopMatrix(); // Restauramos la matriz original
                    glColor3f(1.0f, 1.0f, 1.0f);
                    glEnable(GL_DEPTH_TEST);
                    
                }
            }
        }

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

    // Si no hemos hecho click en el Mago, las teclas no hacen nada
    if (!modoMagiaActivo || hechizoSeleccionado != nullptr) return;
    std::cout << "Tecla pulsada: " << key << " | Modo Magia: " << modoMagiaActivo << std::endl;


    // Rango de teclas de hechizos
    if (key >= '1' && key <= '7') {
          int idx = key - '1';
          std::vector<Hechizo*>& libro = (faseActual == TURNO_LUZ) ? libroLuz : libroOscuridad;

          // Validamos que el índice exista en nuestro libro
          if (idx < (int)libro.size()) {
              if (libro[idx]->estaUsado()) {
                  std::cout << "Hechizo ya agotado." << std::endl;
                  return;
              }
              
              // Simplemente lo marcamos como el hechizo que el ratón va a usar
              hechizoSeleccionado = libro[idx];
              std::cout << "Seleccionado: " << hechizoSeleccionado->getNombre() << std::endl;
              std::cout << "Haz click en el tablero para actuar." << std::endl;
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
        // 1. Llamamos a la lógica interna del hechizo
        hechizoSeleccionado->aplicar(this, raton.casilla);

        // 2. ¿El hechizo considera que ya ha terminado su trabajo?
        if (hechizoSeleccionado->estaUsado()) {
            actualizarVidaPiezas();
            modoMagiaActivo = false;
            hechizoSeleccionado = nullptr;
            seleccionada = nullptr;

            // Cambio de turno
            faseActual = (faseActual == TURNO_LUZ) ? TURNO_OSCURIDAD : TURNO_LUZ;
            std::cout << (faseActual == TURNO_LUZ ? "--> TURNO LUZ" : "--> TURNO OSCURIDAD") << std::endl;
        }

        glutPostRedisplay();
        return;       
    }

    switch (faseActual) {
    case TURNO_LUZ:
    case TURNO_OSCURIDAD:

        if (seleccionada == nullptr) {
            Pieza* piezaEnCasilla = tablero.obtenerOcupante((int)c.x, (int)c.y);

            if (piezaEnCasilla != nullptr) {
                if ((faseActual == TURNO_LUZ && piezaEnCasilla->obtenerBando() == Bando::LUZ) ||
                    (faseActual == TURNO_OSCURIDAD && piezaEnCasilla->obtenerBando() == Bando::OSCURIDAD)) {

                    if (piezaEnCasilla->estaEncarcelada()) {
                        std::cout << "¡ESTA PIEZA ESTA ENCARCELADA! No puede moverse hasta que cambie el ciclo." << std::endl;
                        return; // Importante: salimos para que 'seleccionada' siga siendo nullptr
                    }

                    seleccionada = piezaEnCasilla;

                    if (seleccionada->esLider()) {
                        std::cout << "Lider seleccionado. Pulsa 1-7 para magia o mueve." << std::endl;
                        std::cout << "1.Teleport: mueve una pieza aliada a otra casilla válida." << std::endl;
                        std::cout << "2.Heal: cura completamente a una pieza aliada." << std::endl;
                        std::cout << "3.Shift Time: cambia el valor de luz del mundo." << std::endl;
                        std::cout << "4.Exchange: intercambia la posición de dos piezas." << std::endl;
                        std::cout << "5.Imprison: encarcela a una pieza enemiga." << std::endl;
                        std::cout << "6.Revive: revive a una pieza del cementerio." << std::endl;
                        std::cout << "7.Summon: invoca una nueva pieza temporalmente en el tablero." << std::endl;
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

                actualizarVidaPiezas();

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
    comprobarVictoria();
}

void Mundo::comprobarVictoria() {
    bool pierdeLuz = false;
    bool pierdeOscuridad = false;

    //Forma de ganar --> eliminar todas las piezas o dejar solo una encarcelada
    if (piezasLuz.empty() || (piezasLuz.size() == 1 && piezasLuz[0]->estaEncarcelada())) { 
        pierdeLuz = true;
    }

    if (piezasOscuridad.empty() || (piezasOscuridad.size() == 1 && piezasOscuridad[0]->estaEncarcelada())) {
        pierdeOscuridad = true;
    }

    // Comprobamos si alguien ha perdido por estas reglas
    if (pierdeLuz && pierdeOscuridad) { //El empate es muy raro pero puede ocurrir (batalla final tablero contrarreloj)
        ganadorPartida = 3;
        faseActual = FIN_PARTIDA;
        return;
    }
    else if (pierdeLuz) {
        ganadorPartida = 2; // Gana Oscuridad
        faseActual = FIN_PARTIDA;
        return;
    }
    else if (pierdeOscuridad) {
        ganadorPartida = 1; // Gana Luz
        faseActual = FIN_PARTIDA;
        return;
    }

    //Forma de ganar consigiendo estar en los puento de poder ----> hay q comprobar las coordenadas
    Vector2D puntosPoder[5] = {
        Vector2D(4, 4), // Centro exacto
        Vector2D(4, 0), // Abajo
        Vector2D(4, 8), // Arriba
        Vector2D(0, 4), // Izquierda
        Vector2D(8, 4)  // Derecha
    };

    int contadorLuz = 0;
    int contadorOscuridad = 0;

    for (int i = 0; i < 5; i++) {
        Pieza* ocupante = tablero.obtenerOcupante((int)puntosPoder[i].x, (int)puntosPoder[i].y);
        if (ocupante != nullptr) {
            if (ocupante->obtenerBando() == Bando::LUZ) contadorLuz++;
            else contadorOscuridad++;
        }
    }

    if (contadorLuz == 5) {
        ganadorPartida = 1; // Gana Luz
        faseActual = FIN_PARTIDA;
    }
    else if (contadorOscuridad == 5) {
        ganadorPartida = 2; // Gana Oscuridad
        faseActual = FIN_PARTIDA;
    }
}