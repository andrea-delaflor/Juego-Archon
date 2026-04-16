#include "batalla.h"
#include "glut.h" 
#include <iostream>

// CONSTRUCTOR: Prepara las variables iniciales
Batalla::Batalla() : fondoArena("imagenes/fondoinicio.png", 0, 0, 20, 20) {
    terminado = false;
    empate = false;
    ganador = nullptr;
    perdedor = nullptr;
    l1 = nullptr;
    l2 = nullptr;
    hp1 = 100;
    hp2 = 100;

    // ¡NUEVO! Rellenamos nuestra lista de arenas con imágenes de prueba
    // Tienen que estar en la carpeta bin/imagenes
    rutasArenas.push_back("imagenes/fondoinicio.png");   // Arena 0
    rutasArenas.push_back("imagenes/menuprincipal.png"); // Arena 1
    rutasArenas.push_back("imagenes/menupausa.png");     // Arena 2

    // Empezamos apuntando a la última para que al sumarle 1 en el primer combate, empiece por la Arena 0.
    indiceArenaActual = -1;
}

// INICIALIZA: Se ejecuta CADA VEZ que hay un combate nuevo
void Batalla::inicializa(Pieza* atacante, Pieza* defensor) {
    l1 = atacante;
    l2 = defensor;

    terminado = false;
    empate = false;
    ganador = nullptr;
    perdedor = nullptr;
    hp1 = 100;
    hp2 = 100;

    pos1 = Vector2D(-4, 0);
    pos2 = Vector2D(4, 0);

    // ¡NUEVO! ALTERNAR ARENAS
    // Sumamos 1 al índice. El símbolo % (módulo) hace que si llegamos al final de la lista,
    // vuelva automáticamente a 0 en lugar de dar error. ¡Así es un bucle infinito!
    indiceArenaActual = (indiceArenaActual + 1) % rutasArenas.size();

    // Cambiamos el dibujo de fondoArena usando la ruta que toca de la lista
    fondoArena = ETSIDI::Sprite(rutasArenas[indiceArenaActual].c_str(), 0, 0, 20, 20);

    std::cout << "Cargando Arena número: " << indiceArenaActual << std::endl;
}

// DIBUJA: Pinta todo en la pantalla
void Batalla::dibuja() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Dibuja el fondo de la arena que haya tocado esta vez
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    fondoArena.draw();
    glDisable(GL_TEXTURE_2D);

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

    // Textos informativos temporales
    glDisable(GL_TEXTURE_2D);
    ETSIDI::setTextColor(1, 0, 0);
    ETSIDI::setFont("fuentes/bitwise.ttf", 16);
    ETSIDI::printxy("ATACANTE: PULSA ESPACIO PARA GANAR", -9, 8);
    ETSIDI::printxy("DEFENSOR: PULSA ENTER PARA GANAR", 1, 8);
    ETSIDI::printxy("PULSA 'E' PARA EMPATE (MUEREN LOS DOS)", -5, 7);
}

// MUEVE: Aquí irá la física de la pelea en el futuro
void Batalla::mueve() {
    // Vacío
}

// TECLA: Lee lo que pulsas
void Batalla::tecla(unsigned char key) {
    if (key == ' ') {
        hp2 = 0;
        ganador = l1;
        perdedor = l2;
        empate = false;
        terminado = true;
    }
    else if (key == 13) {
        hp1 = 0;
        ganador = l2;
        perdedor = l1;
        empate = false;
        terminado = true;
    }
    else if (key == 'e' || key == 'E') {
        hp1 = 0;
        hp2 = 0;
        ganador = nullptr;
        perdedor = nullptr;
        empate = true;
        terminado = true;
    }
}