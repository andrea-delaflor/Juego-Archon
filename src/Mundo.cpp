#include "Mundo.h"
#include "glut.h"
#include <math.h>
#include "Vector2D.h"
#include <iostream>
#include "Coordinador.h"

//Instancia global del mundo para que las funciones de glut puedan acceder a él

//Mundo mundo;
Coordinador coordinador; //cambiamos el que manda 

//Funciones de glut

void glueDibuja() { coordinador.dibuja(); }

//Función de temporizador para actualizar el mundo cada 50ms
void glueTimer(int v) {
	//Calcula el nuevo valor de la luz y el ángulo para el parpadeo
    coordinador.mueve();
	//Solicita que se vuelva a dibujar la pantalla con el nuevo estado
    glutPostRedisplay();
	//Vuelve a llamar a esta función después de 50ms para seguir actualizando
    glutTimerFunc(50, glueTimer, 0); // Cada 50ms
}

//añadimos esta funcion glue para que funcionen las teclas
void glueTeclado(unsigned char key, int x, int y) {
    coordinador.tecla(key);
}

//Funciones de la clase Mundo

void Mundo::inicializa() {
	//Inicializamos el valor de la luz y el ángulo para el parpadeo
    valorLuz = 0.5f;
    angulo = 0.0f;

    //Reseteamos maquina de estados al empezar la partida cada vez
    faseActual = TURNO_LUZ;
    seleccionada = nullptr;

    piezasLuz.clear();
    piezasOscuridad.clear();

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

void Mundo::dibuja() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- RESETEO PROFUNDO DE OPENGL ---
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-6, 6, -6, 6);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);

    // 1. Tablero de fondo
    
    tablero.dibuja(valorLuz);

    // 2. Ratón con cambio de color
    // Si hay algo seleccionado, pintamos el recuadro de AMARILLO, si no, de ROJO
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

    for (auto p : piezasLuz) {
        if (p->estaViva()) p->dibuja();
    }

    for (auto p : piezasOscuridad) {
        if (p->estaViva()) p->dibuja();
    }


    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
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

void glueRaton(int button, int state, int x, int y) {
    coordinador.gestionaRaton(button, state, x, y);
    
}

void mousePasivo(int x, int y) {
    coordinador.gestionaRatonPasivo(x, y);
}


//Aunque es el coordinador el que organiza todo el main permanece en mundo. Ahora el main llama a coordinador

int main(int argc, char* argv[]) {
	//Inicializamos GLUT y creamos la ventana
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Archon - ETSIDI");
    

	//Registramos las funciones de dibujo y temporizador
    glutDisplayFunc(glueDibuja);
    glutMouseFunc(glueRaton);
    glutPassiveMotionFunc(mousePasivo);
    glutTimerFunc(50, glueTimer, 0);
    glutKeyboardFunc(glueTeclado);
 
    //observamos que hemos quitado el mundo.inicializa() porqu ahora se inicializa en coordinador con su constructor 

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glutMainLoop();
    return 0;
}