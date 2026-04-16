#include "batalla.h"
#include "glut.h"
#include <iostream>

// CONSTRUCTOR: Prepara el fondo y pone las variables a cero por seguridad
Batalla::Batalla() : fondoArena("imagenes/fondoinicio.png", 0, 0, 20, 20) {
    terminado = false;
    ganador = nullptr;
    perdedor = nullptr;
    l1 = nullptr;
    l2 = nullptr;
    hp1 = 100;
    hp2 = 100;
}

// INICIALIZA: El Coordinador llama a esta función justo cuando dos fichas chocan.
// Recibe como parámetros quién ataca y quién defiende.
void Batalla::inicializa(Pieza* atacante, Pieza* defensor) {
    // Guardamos quiénes se van a pelear
    l1 = atacante;
    l2 = defensor;

    // Reiniciamos variables cada vez que entramos
    terminado = false;
    ganador = nullptr;
    perdedor = nullptr;
    hp1 = 100;
    hp2 = 100;

    // Colocamos a los personajes a izquierda y derecha
    pos1 = Vector2D(-4, 0);
    pos2 = Vector2D(4, 0);
}

void Batalla::dibuja() {
    // 1. Configurar cámara de la arena
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // 2. Dibujar fondo de la arena
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    fondoArena.draw();
    glDisable(GL_TEXTURE_2D);

    // 3. Dibujar a los luchadores
    // Usamos glTranslatef para dibujarlos en las posiciones de la arena sin alterar su estado real
    if (l1 != nullptr) {
        glPushMatrix();
        glTranslatef(pos1.x - l1->obtenerPosicion().x, pos1.y - l1->obtenerPosicion().y, 0);
        l1->dibuja();
        glPopMatrix();
    }

    if (l2 != nullptr) {
        glPushMatrix();
        glTranslatef(pos2.x - l2->obtenerPosicion().x, pos2.y - l2->obtenerPosicion().y, 0);
        l2->dibuja();
        glPopMatrix();
    }

    // 4. Interfaz de controles (Temporal)
    glDisable(GL_TEXTURE_2D);
    ETSIDI::setTextColor(1, 0, 0);
    ETSIDI::setFont("fuentes/bitwise.ttf", 16);
    ETSIDI::printxy("ATACANTE: PULSA ESPACIO PARA GANAR\n", -9, 8);
    ETSIDI::printxy("DEFENSOR: PULSA ENTER PARA GANAR", -9, 6);
}

void Batalla::mueve() {
    // Aquí pondrás el movimiento de los luchadores en tiempo real más adelante
}

void Batalla::tecla(unsigned char key) {
    // Lógica temporal para decidir quién gana y probar el cambio de pantallas
    if (key == ' ') {
        hp2 = 0;
        ganador = l1;
        perdedor = l2;
        terminado = true;
    }
    else if (key == 13) { // 13 es la tecla Enter
        hp1 = 0;
        ganador = l2;
        perdedor = l1;
        terminado = true;
    }
}