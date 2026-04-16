#include "Coordinador.h"

//constructor--->recordamos que hemos quitado el inicializa del mundo aqui es donde lo vamos a poner
Coordinador::Coordinador(): 
    fondo("imagenes/fondoinicio.png", 0, 0, 20, 20) 
    
{
   
    estado = INICIO;//ponemos este como primer estado 
    mundo.inicializa(estado);
}

//MAQUINA DE ESTADOS: el coordinador se encarga de gestionar qué se muestra en cada momento
void Coordinador::dibuja() 
{  
    switch (estado) {
    case INICIO:

        // 1. Cámara (para que todo esté en su sitio)
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // 2. Fondo
        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f); 
        fondo.draw();
        glDisable(GL_TEXTURE_2D);

		// 3. Título y texto 
        glDisable(GL_LIGHTING);     // 1. Apagamos luces (para que no den sombra a las letras)
        glDisable(GL_DEPTH_TEST);   // 2. Quitamos profundidad (para que nada las tape)
        glDisable(GL_TEXTURE_2D);
        
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3f(1.0f, 1.0f, 1.0f);

        // Título ARCHON
        ETSIDI::setTextColor(1.0, 0.9, 0.7); // Amarillo puro
        ETSIDI::setFont("fuentes/games.ttf", 50); // Un poco más grande
        ETSIDI::printxy("ARCHON:", -3, 2); // Lo subimos un poco más para que no roce el tablero

        glColor3f(1.0f, 1.0f, 1.0f);

        // Subtítulo
        ETSIDI::setFont("fuentes/games.ttf", 20);
        ETSIDI::printxy("Alumnos VS Profesores", -4.5, 0);

        glColor3f(1.0f, 1.0f, 1.0f);

        // Instrucción de abajo
        ETSIDI::setTextColor(1, 1, 1); // Blanco puro
        ETSIDI::setFont("fuentes/bitwise.ttf", 22);
        ETSIDI::printxy("PULSA ENTER PARA EMPEZAR", -6, -4);

        glEnable(GL_DEPTH_TEST); // Lo reactivamos para el resto del juego
        break;

    case MENU:
	
        // 1. Cámara (igual que en inicio)
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-10, 10, -10, 10);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // 2. Dibujar fondo de MENU
        glEnable(GL_TEXTURE_2D);
        fondo.draw(); // 
        glDisable(GL_TEXTURE_2D);

        break;
      

    case JUEGO:
        mundo.dibuja(estado); // El mundo se dibuja con normalidad
        break;

    case PAUSA:
        mundo.dibuja(estado); // Dibujamos el mundo de fondo
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
            fondo = ETSIDI::Sprite("imagenes/menuprincipal.png", 0, 0, 20, 20);
        }
        break;

    case MENU:
        if (key == '1') {
            // hay que configurar el mundo para un jugador + IA
            estado = JUEGO;
            mundo.inicializa(estado);
        }
        else if (key == '2') {
            // configurar mundo para 2 jugadores
            estado = JUEGO;
            mundo.inicializa(estado);
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
