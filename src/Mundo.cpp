#include "Mundo.h"
#include "glut.h"
#include <math.h>
#include "Vector2D.h"
#include <iostream>
#include "Movimiento.h"

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
                // Usamos clase Vida.h
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

		// PIEZAS DEL BANDO DE LA LUZ!!

        // EMPEZAMOS CON LA FILA 0 (aqui iran el mago y las piezas no peon == golem)
        piezasLuz.push_back(new ArqueraL(Vector2D(0, 0)));
        piezasLuz.push_back(new ArqueraL(Vector2D(0, 1)));
        piezasLuz.push_back(new DjiniL(Vector2D(0, 2)));
        piezasLuz.push_back(new FenixL(Vector2D(0, 3)));
        piezasLuz.push_back(new MagoL(Vector2D(0, 4))); // El es el principal lo ponemos en el medio en el punto de poder!!
        piezasLuz.push_back(new FenixL(Vector2D(0, 5)));
        piezasLuz.push_back(new DjiniL(Vector2D(0, 6)));
        piezasLuz.push_back(new ArqueraL(Vector2D(0, 7)));
        piezasLuz.push_back(new ArqueraL(Vector2D(0, 8)));

        //FILA 1 ---> peones == golem
        for (int i = 0; i < 9; i++) {
            piezasLuz.push_back(new GolemL(Vector2D(1, i)));
        }
		
        //PIEZAS DEL BANDO DE LA OSCURIDAD

        // EMPEZAMOS CON LA FILA 8 (aqui iran el mago y las piezas no peon == troll)
        piezasOscuridad.push_back(new BasiliscoO(Vector2D(8, 0)));
        piezasOscuridad.push_back(new BasiliscoO(Vector2D(8, 1)));
        piezasOscuridad.push_back(new BansheeO(Vector2D(8, 2)));
        piezasOscuridad.push_back(new DragonO(Vector2D(8, 3)));
        piezasOscuridad.push_back(new BrujaO(Vector2D(8, 4))); //  El es el principal lo ponemos en el medio en el punto de poder!!
        piezasOscuridad.push_back(new DragonO(Vector2D(8, 5)));
        piezasOscuridad.push_back(new BansheeO(Vector2D(8, 6)));
        piezasOscuridad.push_back(new BasiliscoO(Vector2D(8, 7)));
        piezasOscuridad.push_back(new BasiliscoO(Vector2D(8, 8)));

        //FILA 9 ---> peones == trol
        for (int i = 0; i < 9; i++) {
            piezasOscuridad.push_back(new TrollO(Vector2D(7, i)));
        }

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

        // 1. Al seleccionar una pieza nos de opciones de sus movimientos posibles
        if (faseActual != ANIMANDO_MOVIMIENTO) {
            dibujarCajasMovimiento();
        }


        raton.dibuja(); 

        // 2. DIBUJO DE PIEZAS 
        
         glEnable(GL_TEXTURE_2D);
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glColor3ub(255, 255, 255);
         for (auto p : piezasLuz) p->dibuja();
         for (auto p : piezasOscuridad) p->dibuja();


        // 3. CÁRCEL MÁGICA PARA PIEZAS ENCARCELADAS 
         dibujarCarceles();
         
        
        // 4. INTERFAZ DE TURNOS Y HECHIZOS ---> GRIMORIO
         dibujarInterfazSuperior(); // Encima de todo (Arriba)
         dibujarGrimorio();         // Encima de todo (Lateral)

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

    // Si pulsamos '0' y NO estamos en medio de un proceso de Revive, cancelamos
    if (key == '0' && (hechizoSeleccionado == nullptr)) {
        modoMagiaActivo = false;      
        hechizoSeleccionado = nullptr;
        std::cout << ">> Menú de hechizos cerrado." << std::endl;
        return;
    }


    //En caso de elegir REVIVE las teclas 0-9 && a-f eligen a los muertos
    if (modoMagiaActivo && hechizoSeleccionado != nullptr) {
        HechizoRevive* revive = dynamic_cast<HechizoRevive*>(hechizoSeleccionado);
        if (revive != nullptr) {
            bool esLuz = (faseActual == TURNO_LUZ);
            auto& cementerio = esLuz ? cementerioLuz : cementerioOscuridad;

            if (!cementerio.empty()) {
                int nuevoIdx = -1;

                // Teclas 0-9
                if (key >= '0' && key <= '9') {
                    nuevoIdx = key - '0';
                }
                // Teclas a-f para índices 10-15
                else if (key >= 'a' && key <= 'f') {
                    nuevoIdx = 10 + (key - 'a');
                }
                else if (key >= 'A' && key <= 'F') {
                    nuevoIdx = 10 + (key - 'A');
                }

                if (nuevoIdx >= 0 && nuevoIdx < (int)cementerio.size()) {
                    indiceSeleccionado = nuevoIdx;
                    std::cout << ">> Pieza para revivir: ["
                        << nuevoIdx << "] "
                        << cementerio[nuevoIdx]->obtenerNombre()
                        << " — Ahora haz click en casilla vacia." << std::endl;
                }
                else if (nuevoIdx >= 0) {
                    std::cout << "No hay pieza en el indice " << nuevoIdx << std::endl;
                }
            }
        }
        return; // Bloqueamos el resto de teclas mientras hay hechizo activo ----> SUPER IMPORTANTE PORQUE SINO NO DEJA SELECC MUERTOS
    }

    //solo TRUE si lider selecc ---> mago o bruja
    if (seleccionada == nullptr || !seleccionada->esLider()) return;

    auto& libro = (faseActual == TURNO_LUZ) ? libroLuz : libroOscuridad;

    int indice = -1;
    if (key >= '1' && key <= '7') indice = key - '1';
    if (indice < 0 || indice >= (int)libro.size()) return;

    Hechizo* h = libro[indice];
    if (h->estaUsado()) {
        std::cout << "El hechizo '" << h->getNombre() << "' ya fue usado." << std::endl;
        return;
    }

    // ShiftTime: Este hechizo es instantaneo no necesita mas acciones por parte del usuario
    if (key == '3') {
        h->aplicar(this, Vector2D(0, 0));

         // al no hacer click en el tablero no es automatico el cambio de turno qu ehay en la funcion click raton
        if (h->estaUsado()) {
            setModoMagia(false);
            hechizoSeleccionado = nullptr;
            seleccionada = nullptr;
            faseActual = (faseActual == TURNO_LUZ) ? TURNO_OSCURIDAD : TURNO_LUZ;
        }
        return;
    }

    // Para hechizo REVIVE ---> te muestra piezas muertas y te da a elegir
    if (key == '6') {
        bool esLuz = (faseActual == TURNO_LUZ);
        auto& cementerio = esLuz ? cementerioLuz : cementerioOscuridad;

        if (cementerio.empty()) {
            std::cout << "Cementerio vacio. No hay piezas que revivir." << std::endl;
            return;
        }

        hechizoSeleccionado = h;
        modoMagiaActivo = true;
        indiceSeleccionado = 0;  // Por defecto la primera

        std::cout << "=== REVIVE ===" << std::endl;
        for (int i = 0; i < (int)cementerio.size(); i++) {
            std::string tecla = (i < 10) ? std::to_string(i)
                : std::string(1, (char)('a' + i - 10));
            std::cout << "[" << tecla << "] " << cementerio[i]->obtenerNombre() << std::endl;
        }
        std::cout << "Pulsa la tecla de la pieza que quieres revivir." << std::endl;
        std::cout << "Luego haz click en una casilla vacia del tablero." << std::endl;
        return;
    }

    // Resto de hechizos: activar y esperar click
    hechizoSeleccionado = h;
    modoMagiaActivo = true;
    
    if (key == '1') {
        hechizoSeleccionado = h; // h es el hechizo Teleport que has buscado
        modoMagiaActivo = true;

        this->seleccionada = nullptr;

        if (faseActual == TURNO_LUZ)
            std::cout << "Mago (LUZ) deseleccionado. El Teleport Caótico está listo." << std::endl;
        else
            std::cout << "Bruja (OSCURIDAD) deseleccionada. El Teleport Caótico está listo." << std::endl;

        std::cout << "Haz click en CUALQUIER ficha (aliada o enemiga, líder o no) para teletransportarla." << std::endl;
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
            setModoMagia(false);           // Esto quita la leyenda
            hechizoSeleccionado = nullptr; // Limpiamos la selección
            seleccionada = nullptr;        // Quitamos la selección del Mago

            comprobarVictoria();  

            // Solo cambiamos turno si no ha terminado la partida
            if (faseActual != FIN_PARTIDA) {
                faseActual = (faseActual == TURNO_LUZ) ? TURNO_OSCURIDAD : TURNO_LUZ;
            }

            /*
            // Cambio de turno automático tras la magia
            faseActual = (faseActual == TURNO_LUZ) ? TURNO_OSCURIDAD : TURNO_LUZ;
            */
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

                    // Si clicamos una pieza aliada, SIEMPRE reseteamos magia y cambiamos selección
                    setModoMagia(false);
                    seleccionada = piezaEnCasilla;
                    std::cout << "Nueva pieza seleccionada: " << seleccionada->obtenerNombre() << std::endl;

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
                    return;
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
    // Reset total de interfaz POST-BATALLA
    this->hechizoSeleccionado = nullptr;
    this->setModoMagia(false);
    this->seleccionada = nullptr;

    // 4. Bajamos los interruptores rojos del combate
    resetCombate();
    //seleccionada = nullptr;
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








//********************************************************
///encapsular funciones para mundo::dibuja mas legible:
//*********************************************************

void Mundo::dibujarCajasMovimiento() {
    if (seleccionada == nullptr) return;

    //Aqui calculamos los posibles movimientos de la pieza seleccionada
    std::vector<Vector2D> movimientosValidos = seleccionada->obtenerMovimientosValidos(&tablero);

    //Configuramos OpenGL para debujar lineas
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(3.0f); // este es el grosor de la linea por si queremos mas o menos
    glColor3f(0.2f, 1.0f, 0.2f); //este color es verde fosforito pero lo podemos cambiar

    //Aqui es donde dibujamos el marco del movimiento posible
    for (Vector2D mov : movimientosValidos) {
        // Traducimos la coordenada de la matriz a las de OpenGL
        float x_gl = (float)mov.x - 4.0f;
        float y_gl = 4.0f - (float)mov.y;

        glBegin(GL_LINE_LOOP);
        //Usaos el mismo valor que habiamos usado en Tablero.cpp
        glVertex2f(x_gl - 0.48f, y_gl - 0.48f);
        glVertex2f(x_gl + 0.48f, y_gl - 0.48f);
        glVertex2f(x_gl + 0.48f, y_gl + 0.48f);
        glVertex2f(x_gl - 0.48f, y_gl + 0.48f);
        glEnd();
    }

    // Restauramos las configuraciones para no estropear el resto del dibujo
    glLineWidth(1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void Mundo::dibujarCarceles() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3ub(255, 255, 255);
    for (auto p : piezasLuz) p->dibuja();
    for (auto p : piezasOscuridad) p->dibuja();

    //  CÁRCEL MÁGICA PARA PIEZAS ENCARCELADAS 
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Hacemos que la cárcel parpadee un poco
    float tiempoCarcel = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float parpadeo = (sin(tiempoCarcel * 5.0f) + 1.0f) / 2.0f; // Va de 0 a 1

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Pieza* p = tablero.obtenerOcupante(i, j);
            // Si hay una pieza y está encarcelada, le dibujamos la jaula
            if (p != nullptr && p->estaEncarcelada()) {

                // Calculamos dónde está la pieza en coordenadas de pantalla
                float x_gl = (float)i - 4.0f;
                float y_gl = 4.0f - (float)j;

                glPushMatrix();
                glTranslatef(x_gl, y_gl, 0.0f);

                // 1. DIBUJAR LOS BARROTES MÁGICOS (Color Morado Neón)
                glLineWidth(3.0f);
                glColor4f(0.8f, 0.1f, 1.0f, 0.5f + parpadeo * 0.5f); // Morado que palpita

                glBegin(GL_LINES);
                // Barrotes verticales
                for (float bx = -0.4f; bx <= 0.4f; bx += 0.2f) {
                    glVertex2f(bx, -0.45f);
                    glVertex2f(bx, 0.45f);
                }
                // Dos barrotes horizontales para asegurar la jaula
                glVertex2f(-0.45f, 0.3f);
                glVertex2f(0.45f, 0.3f);
                glVertex2f(-0.45f, -0.3f);
                glVertex2f(0.45f, -0.3f);
                glEnd();

                // 2. DIBUJAR MARCO DE LA JAULA
                glLineWidth(5.0f);
                glBegin(GL_LINE_LOOP);
                glVertex2f(-0.45f, -0.45f);
                glVertex2f(0.45f, -0.45f);
                glVertex2f(0.45f, 0.45f);
                glVertex2f(-0.45f, 0.45f);
                glEnd();

                // 3. DIBUJAR UNA GRAN 'X' ROJA EN EL CENTRO (Para que se entienda que está bloqueada)
                glColor4f(1.0f, 0.0f, 0.0f, 0.8f); // Rojo intenso
                glBegin(GL_LINES);
                glVertex2f(-0.3f, -0.3f);
                glVertex2f(0.3f, 0.3f);
                glVertex2f(-0.3f, 0.3f);
                glVertex2f(0.3f, -0.3f);
                glEnd();

                glPopMatrix();
            }
        }
    }

    // Restaurar opciones de dibujado
    glLineWidth(1.0f);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Mundo::dibujarInterfazSuperior() { //INTERFAZ DE TURNOS
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 1. Fondo de la barra superior (de y=5.2 a y=6.0)
    glColor4f(0.0f, 0.0f, 0.0f, 0.75f);
    glBegin(GL_QUADS);
    glVertex2f(-6.0f, 4.8f);
    glVertex2f(6.0f, 4.8f);
    glVertex2f(6.0f, 6.0f);
    glVertex2f(-6.0f, 6.0f);
    glEnd();

    // 2. Texto del turno
    if (faseActual == TURNO_LUZ) {
        ETSIDI::setTextColor(1, 1, 1); // Blanco
        ETSIDI::setFont("fuentes/Bitwise.ttf", 20);
        ETSIDI::printxy("TURNO: FUERZAS DE LA LUZ", -3.2f, 5.9f);
    }
    else {
        ETSIDI::setTextColor(0.6f, 0.2f, 1.0f); // Morado
        ETSIDI::setFont("fuentes/Bitwise.ttf", 20);
        ETSIDI::printxy("TURNO: LEGION DE LA OSCURIDAD", -3.2f, 5.9f);
    }

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}

void Mundo::dibujarGrimorio() {
    if (!modoMagiaActivo) return;

    // 1. Limpieza de estado
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // --- LÓGICA DE VISIBILIDAD ---
    // Si hay un hechizo seleccionado y NO es Revive, NO dibujamos el panel lateral.
    bool esRevive = (hechizoSeleccionado != nullptr && dynamic_cast<HechizoRevive*>(hechizoSeleccionado) != nullptr);
    bool mostrarPanel = (hechizoSeleccionado == nullptr || esRevive);

    if (mostrarPanel) {
        // 2. Fondo del Grimorio (Solo si no hay hechizo o es Revive)
        glColor4f(0.0f, 0.0f, 0.0f, 0.85f);
        glBegin(GL_QUADS);
        glVertex2f(1.8f, -6.0f);
        glVertex2f(6.0f, -6.0f);
        glVertex2f(6.0f, 5.0f);
        glVertex2f(1.8f, 5.0f);
        glEnd();

        // 3. Menú de selección (Solo si no hay hechizo activo)
        if (hechizoSeleccionado == nullptr) {
            ETSIDI::setTextColor(1, 1, 0);
            ETSIDI::setFont("fuentes/Bitwise.ttf", 20);
            ETSIDI::printxy("LIBRO DE HECHIZOS", 2.2f, 4.2f);
            ETSIDI::setFont("fuentes/Bitwise.ttf", 12);
            ETSIDI::printxy("o pulse 0 para cerrar el libro", 2.2f, 3.5f);

            std::vector<Hechizo*>& libro = (faseActual == TURNO_LUZ) ? libroLuz : libroOscuridad;
            ETSIDI::setFont("fuentes/Bitwise.ttf", 13);

            for (int i = 0; i < (int)libro.size(); i++) {
                float yPos = 2.2f - (i * 1.1f);
                if (libro[i]->estaUsado()) ETSIDI::setTextColor(0.5f, 0.5f, 0.5f);
                else ETSIDI::setTextColor(1, 1, 1);

                std::string texto = std::to_string(i + 1) + ". " + libro[i]->getNombre();
                ETSIDI::printxy(texto.c_str(), 2.2f, yPos);
            }
            ETSIDI::setTextColor(1, 1, 1);
            ETSIDI::printxy("PULSA 1 a 7", 2.2f, -5.5f);
        }
        // 4. Lista del Revive (Solo si es Revive)
        else if (esRevive) {
            ETSIDI::setTextColor(0, 1, 1);
            ETSIDI::setFont("fuentes/Bitwise.ttf", 18);
            ETSIDI::printxy("CEMENTERIO", 2.1f, 3.2f);

            auto& cementerio = (faseActual == TURNO_LUZ) ? cementerioLuz : cementerioOscuridad;
            for (int i = 0; i < (int)cementerio.size(); i++) {
                if (i == indiceSeleccionado) ETSIDI::setTextColor(0, 1, 1);
                else ETSIDI::setTextColor(1, 0.5f, 0);

                std::string tecla = (i < 10) ? std::to_string(i) : std::string(1, (char)('a' + i - 10));
                std::string linea = "[" + tecla + "] " + cementerio[i]->obtenerNombre();
                ETSIDI::printxy(linea.c_str(), 2.2f, 2.2f - (i * 0.8f));
            }
        }
    }

    // --- INDICADOR MINIMALISTA (Solo cuando el panel se quita) ---
    if (hechizoSeleccionado != nullptr && !esRevive) {
        // Un cartelito pequeño arriba para saber qué estás haciendo sin tapar el tablero
        glColor4f(0.0f, 0.3f, 0.5f, 0.7f);
        glBegin(GL_QUADS);
        glVertex2f(-5.8f, 3.4f);
        glVertex2f(-1.0f, 3.4f);
        glVertex2f(-1.0f, 4.9f);
        glVertex2f(-5.8f, 4.9f);
        glEnd();
        ETSIDI::setTextColor(1, 1, 1);
        ETSIDI::setFont("fuentes/Bitwise.ttf", 14);
        std::string info = "LANZANDO: " + hechizoSeleccionado->getNombre();
        ETSIDI::printxy(info.c_str(), -5.5f, 4.9f);
    }

    // 5. El indicador que sigue al ratón 
    if (hechizoSeleccionado != nullptr) {
        ETSIDI::setTextColor(1.0f, 0.0f, 0.0f);
        ETSIDI::setFont("fuentes/games.ttf", 12);
        std::string msg = "CLICK EN CASILLA";
        ETSIDI::printxy(msg.c_str(), raton.posicion.x + 0.4f, raton.posicion.y + 0.4f);
    }

    // 6. Lógica info de vidas de piezas al cursor del ratón

    Vector2D c = raton.casilla;
    if (c.x != -1) {
        Pieza* pBajoRaton = tablero.obtenerOcupante((int)c.x, (int)c.y);
        if (pBajoRaton != nullptr) {

            std::string info = pBajoRaton->obtenerNombre() + ": " + std::to_string(pBajoRaton->obtenerVida()) + " hp";
            float anchoDinamico = (float)info.length() * 0.4f;

            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            gluOrtho2D(-10.0, 10.0, -10.0, 10.0);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            float x_gl = (float)c.x - 4.0f;
            float y_gl = 4.0f - (float)c.y;

            // --- BLOQUE UNIFICADO ---
            // Aplicamos un desplazamiento base para que el conjunto flote sobre la pieza
            glTranslatef(x_gl - 1.8f, y_gl + 1.2f, 0.0f);

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // A. DIBUJO DE LA CAJA (Coordenadas relativas al nuevo origen 0,0)
            glDisable(GL_TEXTURE_2D);
            glColor4f(1.0f, 1.0f, 1.0f, 0.95f);
            glBegin(GL_QUADS);
            glVertex2f(-0.2f, -0.2f);
            glVertex2f(anchoDinamico - 0.2f, -0.2f);
            glVertex2f(anchoDinamico - 0.2f, 0.5f);
            glVertex2f(-0.2f, 0.5f);
            glEnd();

            // Borde negro
            glColor3f(0.0f, 0.0f, 0.0f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(-0.2f, -0.2f);
            glVertex2f(anchoDinamico - 0.2f, -0.2f);
            glVertex2f(anchoDinamico - 0.2f, 0.5f);
            glVertex2f(-0.2f, 0.5f);
            glEnd();

            // B. DIBUJO DEL TEXTO (Coordenadas relativas al mismo origen 0,0)
            glEnable(GL_TEXTURE_2D);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

            glColor3f(0.0f, 0.0f, 0.0f);
            ETSIDI::setTextColor(0, 0, 0);
            ETSIDI::setFont("fuentes/bitwise.ttf", 16);

            // Al estar bajo el mismo glTranslatef, 0,0 siempre es el interior de la caja
            ETSIDI::printxy(info.c_str(), 0.0f, 0.0f);

            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
            glPopAttrib();
        }
    }
    glEnable(GL_TEXTURE_2D);
}