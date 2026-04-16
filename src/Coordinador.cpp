#include "Coordinador.h"

Coordinador::Coordinador() :
    fondo("imagenes/fondoinicio.png", 0, 0, 20, 20)
{
    estado = INICIO;
    mundo.inicializa(estado);
}

void Coordinador::dibuja()
{
    switch (estado) {
    case INICIO:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        fondo.draw();
        glDisable(GL_TEXTURE_2D);

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3f(1.0f, 1.0f, 1.0f);

        ETSIDI::setTextColor(1.0, 0.9, 0.7);
        ETSIDI::setFont("fuentes/games.ttf", 50);
        ETSIDI::printxy("ARCHON:", -3, 2);

        glColor3f(1.0f, 1.0f, 1.0f);
        ETSIDI::setFont("fuentes/games.ttf", 20);
        ETSIDI::printxy("Alumnos VS Profesores", -4.5, 0);

        glColor3f(1.0f, 1.0f, 1.0f);
        ETSIDI::setTextColor(1, 1, 1);
        ETSIDI::setFont("fuentes/bitwise.ttf", 22);
        ETSIDI::printxy("PULSA ENTER PARA EMPEZAR", -6, -4);

        glEnable(GL_DEPTH_TEST);
        break;

    case MENU:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-10, 10, -10, 10);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_TEXTURE_2D);
        fondo.draw();
        glDisable(GL_TEXTURE_2D);
        break;

    case JUEGO:
        mundo.dibuja(estado);
        break;

    case PAUSA:
        mundo.dibuja(estado);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
        glBegin(GL_QUADS);
        glVertex2f(-10.0f, -10.0f);
        glVertex2f(10.0f, -10.0f);
        glVertex2f(10.0f, 10.0f);
        glVertex2f(-10.0f, 10.0f);
        glEnd();
        glDisable(GL_BLEND);
        break;

    case BATALLA:
        batalla.dibuja(); // Dibuja la nueva arena de batalla
        break;

    case VICTORIA_ALUMNOS:
    case VICTORIA_PROFESORES:
        break;
    }
}

void Coordinador::tecla(unsigned char key) {
    switch (estado) {
    case INICIO:
        if (key == 13) {
            estado = MENU;
            fondo = ETSIDI::Sprite("imagenes/menuprincipal.png", 0, 0, 20, 20);
        }
        break;

    case MENU:
        if (key == '1') {
            estado = JUEGO;
            mundo.inicializa(estado);
        }
        else if (key == '2') {
            estado = JUEGO;
            mundo.inicializa(estado);
        }
        break;

    case JUEGO:
        if (key == 'p' || key == 'P') {
            estado = PAUSA;
        }
        break;

    case PAUSA:
        if (key == 'r' || key == 'R') {
            estado = JUEGO;
        }
        break;

    case BATALLA:
        batalla.tecla(key); // Envía los controles de teclado a la batalla
        break;
    }
}

void Coordinador::gestionaRaton(int boton, int estadoR, int x, int y) {
    switch (estado) {
    case INICIO:
        if (boton == GLUT_LEFT_BUTTON && estadoR == GLUT_DOWN) {
            estado = MENU;
        }
        break;
    case MENU:
        break;
    case JUEGO:
        mundo.clickRaton(boton, estadoR, x, y);
        break;
    case PAUSA:
        break;
    case BATALLA:
        break;
    }
}

void Coordinador::mueve() {
    switch (estado) {
    case JUEGO:
        mundo.mueve();

        // ¡NUEVO! Detectamos si hay un combate y la ficha que atacó ya ha terminado de moverse al sitio
        if (mundo.hayCombate && mundo.seleccionada != nullptr && !mundo.seleccionada->estaAnimando()) {
            estado = BATALLA;
            batalla.inicializa(mundo.atacante, mundo.defensor); // Preparamos la arena
        }
        break;

    case BATALLA:
        batalla.mueve();
        // Si alguien ganó en la batalla...
        if (batalla.combateTerminado()) {
            estado = JUEGO; // Volvemos al juego principal
            mundo.finalizaCombate(batalla.obtenerGanador(), batalla.obtenerPerdedor()); // El Mundo limpia a los muertos
        }
        break;
    }
}

void Coordinador::gestionaRatonPasivo(int x, int y) {
    switch (estado) {
    case JUEGO:
        mundo.raton.actualizaPosicion(x, y, 800, 800);
        break;
    case PAUSA:
    case INICIO:
    case MENU:
    case BATALLA:
        break;
    }
}