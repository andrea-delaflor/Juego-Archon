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
        mundo.dibuja(estado); //recordamos que esto sirve para dibujar el tablero debajo--->hay q ver si queremos esto en batalla tambien
        
        //aqui lo que vamos a hacer es ajustar la camara porque sino se mueve lo que dibujemos cada vez q la ventana cambie de tamaño
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        //dibujar el rectangulo traslucido de menu pausa
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

        // Dibujamos un rectangulo verde que actua como boton reanudar partida
        glColor3f(0.1f, 0.6f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(-3.0f, 1.0f);
        glVertex2f(3.0f, 1.0f);
        glVertex2f(3.0f, 3.0f);
        glVertex2f(-3.0f, 3.0f);
        glEnd();

        //dibujamos un rectangulo rojo que actua como boton abandonar partida
        glColor3f(0.8f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(-3.0f, -3.0f);
        glVertex2f(3.0f, -3.0f);
        glVertex2f(3.0f, -1.0f);
        glVertex2f(-3.0f, -1.0f);
        glEnd();

        //escribimos nuestro texto en los "botones que hemos creado"
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        ETSIDI::setTextColor(1, 1, 1);

        ETSIDI::setFont("fuentes/bitwise.ttf", 24);
        ETSIDI::printxy("REANUDAR", -2.8f, 2.0f);
        ETSIDI::printxy("EXIT", -1.8f, -2.8f);

        //por ultimo agregamos el titulo del menu de pausa
        ETSIDI::setFont("fuentes/games.ttf", 50);
        ETSIDI::printxy("PAUSA", -2.5f, 6.0f);


        break;

    case BATALLA:
        batalla.dibuja();
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
        
        batalla.tecla(key);
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
        //porque queremos que el menu pausa se controle con el raton
        if (boton == GLUT_LEFT_BUTTON && estadoR == GLUT_DOWN) {
             //esto como siempre lo de traducir coordenadas a las de OpenGL
            float mouseX = ((float)x / 800.0f) * 20.0f - 10.0f;
            float mouseY = ((1.0f - (float)y / 800.0f)) * 20.0f - 10.0f;

            //Con esta función comprobamos que el raton ha hecho click en las coordenadas donde se encuentra nuestro boton verde 
            if (mouseX >= -3.0f && mouseX <= 3.0f && mouseY >= 1.0f && mouseY <= 3.0f) {
                estado = JUEGO;
            }

            //lo mismo de arriba pero comprobando si son las coordenadas del botn rojo
            else if (mouseX >= -3.0f && mouseX <= 3.0f && mouseY >= -3.0f && mouseY <= -1.0f) {
                estado = MENU;
                fondo = ETSIDI::Sprite("imagenes/menuprincipal.png", 0, 0, 20, 20);
                mundo.inicializa(0); // Reseteamos el mundo por si quieren echar otra partida
            }
        }
        break;
    case BATALLA:
        break;
    }
}

void Coordinador::mueve() {
    switch (estado) {
    case JUEGO:
        mundo.mueve();

        if (mundo.hayCombate && mundo.seleccionada != nullptr && !mundo.seleccionada->estaAnimando()) {
            estado = BATALLA;
            batalla.inicializa(mundo.atacante, mundo.defensor);
        }
        break;

    case BATALLA:
        batalla.mueve();

        if (batalla.combateTerminado()) {
            estado = JUEGO;

            // ¡AQUÍ ESTÁ EL ARREGLO! Ahora le pasamos 3 argumentos (ganador, perdedor, y si es empate)
            mundo.finalizaCombate(batalla.obtenerGanador(), batalla.obtenerPerdedor(), batalla.esEmpate());
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