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
        if (p != seleccionada) p->dibuja();
    }
    for (auto p : piezasOscuridad) {
        if (p != seleccionada) p->dibuja();
    }

    // Dibujamos la pieza seleccionada la ÚLTIMA
    if (seleccionada != nullptr && seleccionada->estaViva()) {
        // Dibujamos
        seleccionada->dibuja();
    }

    glDisable(GL_TEXTURE_2D);

    // --- BLOQUE DE PRUEBA: FLAG DE COMBATE ---
    if (hayCombate && atacante != nullptr && defensor != nullptr) {
        glDisable(GL_LIGHTING);

        // Caja de texto superior
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-4.5f, 4.3f);
            glVertex2f(4.5f, 4.3f);
            glVertex2f(4.5f, 3.5f);
            glVertex2f(-4.5f, 3.5f);
        glEnd();

        // Texto informativo
        glColor3f(1.0f, 0.8f, 0.0f); // Dorado

        // Creamos los strings de bando
        std::string bAtacante = (atacante->obtenerBando() == Bando::LUZ) ? "Alumnos" : "Profesores";
        std::string bDefensor = (defensor->obtenerBando() == Bando::LUZ) ? "Alumnos" : "Profesores";

        // Formato: NOMBRE (BANDO) vs NOMBRE (BANDO)
        // Ejemplo: Crea (Alumnos) VS Dirección (Profesores)
        std::string mensaje = "COMBATE: " + atacante->obtenerNombre() + " (" + bAtacante + ") VS " +
            defensor->obtenerNombre() + " (" + bDefensor + ")";

        // Calcular el centrado del texto
        float anchoEstimado = mensaje.length() * 0.12f;
        float xInicio = -(anchoEstimado / 2.0f);

        // Dibujar el texto centrado
        glColor3f(1.0f, 0.8f, 0.0f); // Dorado
        glRasterPos2f(xInicio, 3.8f);

        for (char c : mensaje) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

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
            // --- LÓGICA DE INTERACCIÓN, CHOQUE ---
            Pieza* ocupanteDestino = tablero.obtenerOcupante((int)c.x, (int)c.y);

            // 1. Quitamos la pieza de su posición actual en el tablero
            Vector2D posAntigua = seleccionada->obtenerPosicion();
            tablero.colocarPieza((int)posAntigua.x, (int)posAntigua.y, nullptr);

            // 2. Si hay un enemigo (CHOQUE), lo guardamos como defensor antes de "pisarlo"
            if (ocupanteDestino != nullptr && ocupanteDestino->obtenerBando() != seleccionada->obtenerBando()) {
                this->hayCombate = true;
                this->atacante = seleccionada;
                this->defensor = ocupanteDestino;
                std::cout << "Iniciando combate..." << std::endl;
            }
            else {
                std::cout << "Movimiento libre realizado." << std::endl;
            }

            // 3. Movemos la pieza a la nueva casilla (lógica y visualmente)
            seleccionada->establecerPosicion(c);
            tablero.colocarPieza((int)c.x, (int)c.y, seleccionada);
        }
        else {
            std::cout << "Movimiento no permitido." << std::endl;
        }

        seleccionada = nullptr;
    }

    // Forzamos a redibujar
    glutPostRedisplay();
}

void glueRaton(int button, int state, int x, int y) {
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