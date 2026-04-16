#include "Coordinador.h"

//constructor--->recordamos que hemos quitado el inicializa del mundo aqui es donde lo vamos a poner
Coordinador::Coordinador() {
    estado = INICIO;//ponemos este como primer estado 
    mundo.inicializa();
}

//MAQUINA DE ESTADOS: el coordinador se encarga de gestionar qué se muestra en cada momento
void Coordinador::dibuja() {
    switch (estado) {
    case INICIO:
        //aqui dibujariamos la pantalla de inicio ---> la portada
        break;

    case MENU:
        //aqui vamos a dibujar las opciones del menu (jugar, instrucciones, etc..)
        break;

    case JUEGO:
        mundo.dibuja(); // El mundo se dibuja con normalidad
        break;

    case PAUSA:
        mundo.dibuja(); // Dibujamos el mundo de fondo

        //Dibujamos un recuadro que se dibuja por encima oscuro 
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f); // Negro al 70% de opacidad
        glBegin(GL_QUADS);
        glVertex2f(-10.0f, -10.0f);
        glVertex2f(10.0f, -10.0f);
        glVertex2f(10.0f, 10.0f);
        glVertex2f(-10.0f, 10.0f);
        glEnd();
        glDisable(GL_BLEND);

        //hay que imprimir y poner por pantalla como salir de la pausa
        break;

    case BATALLA:
        //Igual que en modo juego dibujamos mundo aqui llamaremos a dibujar mundo-batalla(o como lo llamemos)
        break;

    case VICTORIA_ALUMNOS:
    case VICTORIA_PROFESORES:
        // Hay que dibujar una pantalla de final partida
        break;
    }
}

void Coordinador::tecla(unsigned char key) {
    switch (estado) {
    case INICIO:
        if (key == 13) { // 13 es la tecla Enter
            estado = MENU;
        }
        break;

    case MENU:
        if (key == '1') {
            // hay que configurar el mundo para un jugador + IA
            estado = JUEGO;
        }
        else if (key == '2') {
            // configurar mundo para 2 jugadores
            estado = JUEGO;
        }
        break;

    case JUEGO:
        if (key == 'p' || key == 'P') {
            estado = PAUSA;
            //Si pulsamos la p nos vamos al estado pausa
        }
        //else pues seguimos en juego y las teclas que usemos en el juego (hay que añadirlas)
        break;

    case PAUSA:
        if (key == 'r' || key == 'R') {
            estado = JUEGO; 
            //es decir si pulsamos la R volvemos a la partida
        }
        break;

    case BATALLA:
        
        break;
    }
}

void Coordinador::gestionaRaton(int boton, int estadoR, int x, int y) {
    switch (estado) {
    case INICIO:
        //las opciones para llegar al menu
        if (boton == GLUT_LEFT_BUTTON && estadoR == GLUT_DOWN) {
            estado = MENU;
        }
        break;

    case MENU:
        //una vez en menu hay que coordinar los botones que nos llevan a los distintos estados
        break;

    case JUEGO:
        //Una vez en Juego el coordinador le delega la responsabilidad a mundo hasta que reciba una orden de cambiar de estado
        mundo.clickRaton(boton, estadoR, x, y);
        break;

    case PAUSA:
        // Gestion pausa---> si solo usamos la tecla R entonces obviamos el raton
        break;

    case BATALLA:
        //Lo mismo que en Juego pero en el mundo-batalla
        break;
    }
}

void Coordinador::mueve() {
    switch (estado) {
    case JUEGO:
        mundo.mueve(); //Basicamente usa a mundo para mover las piezas y tambien los cambios del tablero
        break;

    case BATALLA:
        //lo mismo para cuando creemos el mundo-batalla
        break;

        //aqui hay que ver lo que pasa cuando no estamos en juego ni en batalla
    }
}


void Coordinador::gestionaRatonPasivo(int x, int y) {
    switch (estado) {
    case JUEGO:
        // Si estamos jugando, dejamos que el Mundo actualice el cuadradito del ratón
        mundo.raton.actualizaPosicion(x, y, 800, 800);
        break;

    case PAUSA:
    case INICIO:
    case MENU:
        // En estos estados, el juego ignora por dónde mueves el ratón
        // (a menos que más adelante hagáis botones que se iluminen al pasar por encima)
        break;

    case BATALLA:
        // Si en la batalla necesitáis saber dónde está el ratón, iría aquí
        break;
    }
}
