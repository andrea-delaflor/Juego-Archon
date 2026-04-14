#include "Mundo.h"
#include "glut.h"
#include <math.h>
#include "Vector2D.h"
#include <iostream>

//Instancia global del mundo para que las funciones de glut puedan acceder a él

Mundo mundo;

//Funciones de glut

void glueDibuja() { mundo.dibuja(); }

//Función de temporizador para actualizar el mundo cada 50ms
void glueTimer(int v) {
	//Calcula el nuevo valor de la luz y el ángulo para el parpadeo
    mundo.mueve();
	//Solicita que se vuelva a dibujar la pantalla con el nuevo estado
    glutPostRedisplay();
	//Vuelve a llamar a esta función después de 50ms para seguir actualizando
    glutTimerFunc(50, glueTimer, 0); // Cada 50ms
}


//Funciones de la clase Mundo

void Mundo::inicializa() {
	//Inicializamos el valor de la luz y el ángulo para el parpadeo
    valorLuz = 0.5f;
    angulo = 0.0f;

    piezasLuz.clear();
    piezasOscuridad.clear();

    // --- BANDO DE LA LUZ ---
    piezasLuz.push_back(new GolemL(Vector2D(6, 4)));
    piezasLuz.push_back(new MagoL(Vector2D(0, 4)));
	piezasLuz.push_back(new DjiniL(Vector2D(2, 4)));
    piezasLuz.push_back(new ArqueraL(Vector2D(1, 2)));
    

    // --- BANDO DE LA OSCURIDAD ---
 
    piezasOscuridad.push_back(new BrujaO(Vector2D(8, 4)));
	piezasOscuridad.push_back(new DragonO(Vector2D(6, 6)));
    piezasOscuridad.push_back(new BasiliscoO(Vector2D(7, 6)));
    piezasOscuridad.push_back(new TrollO(Vector2D(3, 2)));

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

    // ESTADO A: No hay nada seleccionado (Primer Click)
    if (seleccionada == nullptr) {
        // En lugar de piezaLuz o piezaOscuridad, preguntamos al tablero quién está ahí
        Pieza* piezaEnCasilla = tablero.obtenerOcupante((int)c.x, (int)c.y);

        if (piezaEnCasilla != nullptr) {
            seleccionada = piezaEnCasilla;

            // --- INFO EN CONSOLA ---
            std::vector<Vector2D> movimientos = seleccionada->obtenerMovimientosValidos(&tablero);

            std::cout << "\n===============================" << std::endl;
            std::cout << "SELECCIONADO: " << seleccionada->obtenerNombre() << std::endl;
            std::cout << "Bando: " << (seleccionada->obtenerBando() == Bando::LUZ ? "LUZ" : "OSCURIDAD") << std::endl;
            std::cout << "Rango: " << seleccionada->obtenerRangoMovimiento() << std::endl;
            std::cout << "Casillas validas: " << movimientos.size() << std::endl;

            for (auto& pos : movimientos) {
                std::cout << " -> Puede ir a: (" << (int)pos.x << ", " << (int)pos.y << ")" << std::endl;
            }
            std::cout << "===============================" << std::endl;
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
            // 1. IMPORTANTE: Quitar la pieza de la posición antigua en la matriz del tablero
            Vector2D posAntigua = seleccionada->obtenerPosicion();
            tablero.colocarPieza((int)posAntigua.x, (int)posAntigua.y, nullptr);

            // 2. Actualizar la posición de la pieza
            seleccionada->establecerPosicion(c);

            // 3. Colocar la pieza en la nueva posición del tablero
            tablero.colocarPieza((int)c.x, (int)c.y, seleccionada);

            std::cout << "LOG: Movimiento realizado con exito." << std::endl;
        }
        else {
            std::cout << "LOG: Movimiento no permitido." << std::endl;
        }

        // Liberamos la selección
        seleccionada = nullptr;
    }

    // Forzamos a redibujar
    glutPostRedisplay();
}void glueRaton(int button, int state, int x, int y) {
    // Solo actuamos cuando se PULSA (state == 0) el botón IZQUIERDO (button == 0)
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 1. Decirle al objeto raton que calcule la casilla
        // Pasamos 800, 800 que es el tamaño de tu ventana en el main
        mundo.clickRaton(button, state, x, y);
    }
}

void mousePasivo(int x, int y) {
    mundo.raton.actualizaPosicion(x, y, 800, 800);
}


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
 
    

	//Inicializamos el mundo y comenzamos el bucle principal de GLUT
    mundo.inicializa();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glutMainLoop();
    return 0;
}