#include "Mundo.h"
#include "glut.h"
#include <math.h>
#include "Vector2D.h"
#include <iostream>



Mundo::Mundo() {
    // Valores por defecto
    seleccionada = nullptr;
}

Mundo::~Mundo() {
    // Limpiamos los vectores para no dejar fugas de memoria
    for (auto p : piezasLuz) delete p;
    for (auto p : piezasOscuridad) delete p;
}

void Mundo::inicializa(int estado) {

    // 1. LIMPIEZA   
    for (auto p : piezasLuz) delete p;
    for (auto p : piezasOscuridad) delete p;
    piezasLuz.clear();
    piezasOscuridad.clear();
    tablero.vaciar(); // Asume que tienes un método para poner todas las casillas a nullptr

    valorLuz = 0.5f;
    angulo = 0.0f;
    seleccionada = nullptr;

    switch (estado) {
    case 0: // INICIO: No creamos nada, el tablero se queda vacío y limpio
        break;
    case 2: // MENU: 
        // ...
        break;
        //Inicializamos el valor de la luz y el ángulo para el parpadeo
        valorLuz = 0.5f;
        angulo = 0.0f;
    case 3:
        //Reseteamos maquina de estados al empezar la partida cada vez
        faseActual = TURNO_LUZ;
        seleccionada = nullptr;

        // --- BANDO DE LA LUZ ---
        piezasLuz.push_back(new GolemL(Vector2D(6, 4)));
        piezasLuz.push_back(new MagoL(Vector2D(0, 4)));
        piezasLuz.push_back(new DjiniL(Vector2D(2, 4)));
        piezasLuz.push_back(new ArqueraL(Vector2D(1, 2)));
        piezasLuz.push_back(new FenixL(Vector2D(1, 3)));


        // --- BANDO DE LA OSCURIDAD ---

        piezasOscuridad.push_back(new BrujaO(Vector2D(8, 4)));
        piezasOscuridad.push_back(new DragonO(Vector2D(6, 6)));
        piezasOscuridad.push_back(new BasiliscoO(Vector2D(7, 6)));
        piezasOscuridad.push_back(new TrollO(Vector2D(3, 2)));
        piezasOscuridad.push_back(new BansheeO(Vector2D(5, 2)));

        // guardamos la pos de la pieza
        for (auto p : piezasLuz) {
            tablero.colocarPieza((int)p->obtenerPosicion().x, (int)p->obtenerPosicion().y, p);
        }

        for (auto p : piezasOscuridad) {
            tablero.colocarPieza((int)p->obtenerPosicion().x, (int)p->obtenerPosicion().y, p);
        }
    }

}

//Movemos el estado del mundo para crear el efecto de parpadeo suave en las casillas variables. 
void Mundo::mueve() {
	//Incrementamos el ángulo para crear una oscilación suave
    angulo += 0.05f;
	//Calculamos el nuevo valor de la luz usando una función seno para que oscile entre 0 y 1. Usamos forma cíclica para que el parpadeo sea suave y continuo
    valorLuz = (sin(angulo) + 1.0f) / 2.0f; // Oscilación entre 0 y 1

    //Aqui vamos a controlar los turnos de cada bando y su animación en pantalla
    switch (faseActual) {
    case ANIMANDO_MOVIMIENTO:
        //Se cumple si tenemos una pieza seleccionada y ha terminado de moverse por el tablero
        if (seleccionada != nullptr && !seleccionada->estaAnimando()) {

            //cambiamos de truno! le toca al otro jugador
            if (seleccionada->obtenerBando() == Bando::LUZ) {
                faseActual = TURNO_OSCURIDAD;
                std::cout << "--> TURNO DE LA OSCURIDAD" << std::endl;
            }
            else {
                faseActual = TURNO_LUZ;
                std::cout << "--> TURNO DE LA LUZ" << std::endl;
            }

            //dejamos de seleccionar pieza para que se pueda seleccionar otra (soltamos la pieza para que pueda jugar el siguoente jugador)
            seleccionada = nullptr;
        }
        break;

    case TURNO_LUZ:
    case TURNO_OSCURIDAD:
    case FIN_PARTIDA:
        
        break;
    }
   
}

void Mundo::dibuja(int estado) {
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -1, 1); // Versión más robusta de Ortho

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


   switch (estado) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // Probamos con un rango más amplio (de -5 a 15) para asegurar que vemos el tablero
        gluOrtho2D(-5.0, 15.0, -5.0, 15.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

      

    case 0: // ESTADO INICIO (Coordinador::INICIO)
        // Aquí dibujamos algo artístico para la portada
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f); // Fuerza color blanco
        tablero.dibuja(1.0f);

        // Si tienes una imagen de portada:
        // ETSIDI::drawSprite("imagenes/portada.png", -5, -5, 10, 10);
        break;

    case 1: // ESTADO MENU
        // Dibujamos el tablero normal pero quizás sin el ratón rojo
        tablero.dibuja(0.5f);
        break;

    case 2: // ESTADO JUEGO (El que ya tenías hecho)
        tablero.dibuja(valorLuz);

        // Dibujamos las piezas
        glEnable(GL_TEXTURE_2D);
        for (auto p : piezasLuz) if (p->estaViva()) p->dibuja();
        for (auto p : piezasOscuridad) if (p->estaViva()) p->dibuja();
        glDisable(GL_TEXTURE_2D);

        raton.dibuja(); // Aquí sí queremos ver el ratón
        break;

    case 3: // ESTADO PAUSA
        tablero.dibuja(0.3f); // Tablero sombreado

        if (seleccionada != nullptr) {
            // Podemos añadir un método a Raton para cambiar color o hacerlo aquí
            glColor3f(1.0f, 1.0f, 0.0f); // Amarillo (Seleccionado)
        }
        else {
            glColor3f(1.0f, 0.0f, 0.0f); // Rojo (Normal)
        }
        raton.dibuja();

        // 3. Piezas
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor3ub(255, 255, 255);
        for (auto p : piezasLuz) p->dibuja();
        for (auto p : piezasOscuridad) p->dibuja();
        break;
    }glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void Mundo::clickRaton(int button, int state, int x, int y) {
    // Solo actuamos si se presiona el botón izquierdo
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    // 1. Actualizamos la casilla donde se ha hecho click
    raton.actualizaPosicion(x, y, 800, 800);
    Vector2D c = raton.casilla;

    // Si el click es fuera del tablero, no hacemos nada
    if (c.x == -1) return;

    switch (faseActual) {

    case TURNO_LUZ:
    case TURNO_OSCURIDAD:

        // ESTADO A: No hay nada seleccionado (Primer Click)
        if (seleccionada == nullptr) {
            Pieza* piezaEnCasilla = tablero.obtenerOcupante((int)c.x, (int)c.y);

            if (piezaEnCasilla != nullptr) {
                // Esto garantiza que solo puedas seleccionar fichas de tu bando
                if ((faseActual == TURNO_LUZ && piezaEnCasilla->obtenerBando() == Bando::LUZ) ||
                    (faseActual == TURNO_OSCURIDAD && piezaEnCasilla->obtenerBando() == Bando::OSCURIDAD)) {

                    seleccionada = piezaEnCasilla;
                    std::cout << "LOG: Seleccionada pieza de tu bando." << std::endl;
                }
                else {
                    std::cout << "ERROR: ¡No es tu turno o no es tu ficha!" << std::endl;
                }
            }
        }
        // ESTADO B: Ya hay una pieza seleccionada (Segundo Click: Intentar mover)
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
                // AActualizamos el tablero---->estamos tocando la parte logica del tablero
                Vector2D posAntigua = seleccionada->obtenerPosicion();
                tablero.colocarPieza((int)posAntigua.x, (int)posAntigua.y, nullptr);

                seleccionada->establecerPosicion(c); // Esto activa animando=true
                tablero.colocarPieza((int)c.x, (int)c.y, seleccionada);

                std::cout << "LOG: Movimiento valido. Empezando animacion..." << std::endl;

                // ¡CAMBIO DE ESTADO! Bloqueamos el juego mientras se mueve
                faseActual = ANIMANDO_MOVIMIENTO;
            }
            else {
                std::cout << "LOG: Movimiento no permitido. Deseleccionando." << std::endl;
                seleccionada = nullptr; // dejamos de seleccionar pieza queda libre para que el otro jugador pueda seleccionar
            }
        }
        break;

    case ANIMANDO_MOVIMIENTO:
        // Mientras la ficha en movimiento obviamos clicks
        std::cout << "LOG: Espera a que termine la animacion..." << std::endl;
        break;

    case FIN_PARTIDA:
        // Si el juego termina no hacemos nada 
        break;
    }

    // Forzamos a redibujar
    glutPostRedisplay();
}


