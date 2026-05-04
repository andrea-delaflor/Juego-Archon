#include "Coordinador.h"
#include <iostream>


Coordinador::Coordinador() :
    fondo("imagenes/fondoinicio.png", 0, 0, 20, 20),
    pantallahistoria("imagenes/historiainicio.png", 0, 0, 20, 20)
{
    estado = INICIO;
    mundo.inicializa(estado);
    ETSIDI::playMusica("sonidos/cantocelestial.wav", true);  //el true es para que la musica suene todo el rato sin parar, el false es para que solo suena una vez
}

void Coordinador::dibuja()
{

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Esto hace que las imágenes no se pixelen al hacerse grandes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // RESET DE ESTADO: Obligamos a OpenGL a volver al color blanco puro
    // y a reactivar la iluminación antes de dibujar nada más.
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);

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
        glColor3f(1.0f, 1.0f, 1.0f); // Reset de color para que las imágenes se vean bien

        if (historiaActiva) {
            // 2. Si la historia está activa, dibujamos el Sprite de la historia[cite: 5, 6]
            pantallahistoria.draw();
        }
        else {
            // 3. Si no, dibujamos el fondo del menú y las opciones de texto
            fondo.draw();

            
            

           
        }

        glDisable(GL_TEXTURE_2D);
        break;

    case JUEGO:
        mundo.dibuja(estado);
        break;

    case INSTRUCCIONES:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Dibujamos la página actual
        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        fondo.draw();
        glDisable(GL_TEXTURE_2D);

        //Imprimimos este texto para indicar al usuario como moverse por el menu
        ETSIDI::setTextColor(1, 1, 1);
        ETSIDI::setFont("fuentes/bitwise.ttf", 20);

        // Lo ponemos un poco más a la izquierda (-8.0) 
        ETSIDI::printxy("FLECHAS IZQ/DER PASAR PAGINA  |  ESC PARA SALIR", -8.0f, -8.0f);

        break;
    case PAUSA:
        // Guardamos TODO el estado actual de OpenGL
        glPushAttrib(GL_ALL_ATTRIB_BITS);


        // Dibujamos el fondo
        if (estadoAnterior == JUEGO) {
            mundo.dibuja(estado);
        }
        else if (estadoAnterior == BATALLA) { // Aquí tenemos que mirarlo porque no se ven los botones
            batalla.dibuja();
        }

        // Limpiamos el estado
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Aquí lo que vamos a hacer es ajustar la camara porque sino se mueve lo que dibujemos cada vez que la ventana cambie de tamaño
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(-10.0, 10.0, -10.0, 10.0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        // Dibujar el rectángulo traslúcido de menú pausa
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
        if (hoverReanudar == true) {
            glColor3f(0.2f, 0.9f, 0.3f);//Esto es lo nuevo de detectar si el raton esta por encima (verde mas chillon)
        }
        else {
            glColor3f(0.1f, 0.6f, 0.2f);//Color normal si el raton no esta por encima
        }
        glBegin(GL_QUADS);
        glVertex2f(-3.0f, 1.0f);
        glVertex2f(3.0f, 1.0f);
        glVertex2f(3.0f, 3.0f);
        glVertex2f(-3.0f, 3.0f);
        glEnd();

        //dibujamos un rectangulo rojo que actua como boton abandonar partida
        if (hoverAbandonar == true) {
            glColor3f(1.0f, 0.3f, 0.3f);//Esto es lo nuevo de detectar si el raton esta por encima (rojo mas chillon)
        }
        else {
            glColor3f(0.8f, 0.2f, 0.2f);//Color normal si el raton no esta por encima
        }
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

        // Restauración de la cámara
        glMatrixMode(GL_PROJECTION);
        glPopMatrix(); // Recuperamos la cámara original del juego

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix(); // Recuperamos la vista original
        glPopAttrib(); // Recuperamos luces y colores

        break;

    case BATALLA:
        batalla.dibuja();
        break;

    case VICTORIA_ALUMNOS:
    case VICTORIA_PROFESORES:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // 1. Dibujamos la imagen gigante
        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        fondo.draw();
        glDisable(GL_TEXTURE_2D);

        // 2. Un texto parpadeante o normal para volver al menú
        ETSIDI::setTextColor(1, 1, 1);
        ETSIDI::setFont("fuentes/bitwise.ttf", 20);
        ETSIDI::printxy("PULSA ENTER PARA VOLVER AL MENU", -6.0f, -8.0f);

        break;
    }
}

void Coordinador::tecla(unsigned char key) {
    switch (estado) {
    case INICIO:
        if (key == 13) {
            estado = MENU;
            fondo = ETSIDI::Sprite("imagenes/menuprincipal.png", 0, 0, 20, 20);
            ETSIDI::stopMusica();
            ETSIDI::playMusica("sonidos/menuppal2.wav", true);
        }
        break;

    case MENU:
        // 1. Si la historia está en pantalla, el usuario solo puede pulsar ESPACIO para jugar
        if (historiaActiva) {
            if (key == ' ') {
                historiaActiva = false; // Quitamos la imagen[cite: 6]
                estado = JUEGO;         // Cambiamos al tablero
                mundo.inicializa(estado);

                ETSIDI::stopMusica();
                ETSIDI::playMusica("sonidos/pantallatablero.wav", true);
            }
            // Este 'return' impide que se procesen otras teclas mientras se ve la historia
            return;
        }

       
        break;

        // 2. Si NO hay historia (menú principal), elegimos el modo
        if (key == '1' || key == '2') {
            // Guardamos la elección (siempre se guarda antes de la historia)
            if (key == '1') modoUnJugador = true;
            else modoUnJugador = false;

            // ACTIVAMOS LA HISTORIA
           
            historiaActiva = true;

            // ¡CUIDADO! Aquí NO debe haber "estado = JUEGO"
            // Si pones estado = JUEGO aquí, ignorarás el orden que acabamos de crear.
        }
        break;

    case INSTRUCCIONES:
        // El código ASCII de la tecla ESC es 27
        if (key == 27) {
            estado = MENU;
            fondo = ETSIDI::Sprite("imagenes/menuprincipal.png", 0, 0, 20, 20);
        }
        break;
    case JUEGO:
        mundo.teclahechizos(key);
       
        if (key == 'p' || key == 'P') {
            estadoAnterior = estado;
            estado = PAUSA;
        }
        
            break;

    case PAUSA:
        if (key == 'r' || key == 'R') {
            estado = JUEGO;
        }
        break;

    case BATALLA:
        if (key == 'p' || key == 'P') {
            estadoAnterior = estado; // Guardamos que venimos de la BATALLA
            estado = PAUSA;
        }
        else {
            batalla.tecla(key); // Si no es la P, pasamos las teclas a la pelea
        }
        break;
    case VICTORIA_ALUMNOS:
    case VICTORIA_PROFESORES:
        if (key == 13) { // Tecla ENTER
            estado = MENU;
            fondo = ETSIDI::Sprite("imagenes/menuprincipal.png", 0, 0, 20, 20);
            mundo.inicializa(0); // Reseteamos el mundo
        }
        break;
    }
}


void Coordinador::gestionaRaton(int boton, int estadoR, int x, int y) {
    //Aqui vamos a calcular las coordenadas de OpenGL y asi como usamos switch no hay que añadir{}
    int anchoVentana = glutGet(GLUT_WINDOW_WIDTH);
    int altoVentana = glutGet(GLUT_WINDOW_HEIGHT);
    float mouseX = ((float)x / (float)anchoVentana) * 20.0f - 10.0f;
    float mouseY = ((1.0f - (float)y / (float)altoVentana)) * 20.0f - 10.0f;


    switch (estado) {
    case INICIO:
        if (boton == GLUT_LEFT_BUTTON && estadoR == GLUT_DOWN) {
            estado = MENU;
        }
        break;
    case MENU:
        if (historiaActiva) break;

        if (boton == GLUT_LEFT_BUTTON && estadoR == GLUT_DOWN) {
            //esto es el rango para el "boton" de 1 jugador
            if (mouseX >= -5.5f && mouseX <= -0.6f && mouseY >= -4.45f && mouseY <= 0.125f) {
                modoUnJugador = true;
                mundo.modoIA = true;
                std::cout << "se ha seleccionado modo 1 jugador" << std::endl;
                mundo.inicializa(estado);
                ETSIDI::stopMusica();
                ETSIDI::playMusica("sonidos/pantallatablero.wav", true);

                // ACTIVACIÓN DE HISTORIA (En lugar de estado = JUEGO)
                
                historiaActiva = true;

            }
            //esto es el rango para el "boton" de 2 jugadores
            else if (mouseX >= 0.65f && mouseX <= 5.275f && mouseY >= -4.425f && mouseY <= 0.15f) {
                modoUnJugador = false;
                mundo.modoIA = false;
                std::cout << "se ha seleccionado modo 2 jugador" << std::endl;
                mundo.inicializa(estado);
                ETSIDI::stopMusica();
                ETSIDI::playMusica("sonidos/pantallatablero.wav", true);

                // ACTIVACIÓN DE HISTORIA
                
                historiaActiva = true;

            }
            // --- BOTÓN INSTRUCCIONES ---
            else if (mouseX >= -2.175f && mouseX <= 2.125f && mouseY >= -7.2f && mouseY <= -4.825f) {
                std::cout << "Entrando al Grimorio........ " << std::endl;
                estado = INSTRUCCIONES;
                paginaInstrucciones = 1; //lo ponemos a 1 para no empezar en la hoja 2
                fondo = ETSIDI::Sprite("imagenes/comojugar.png", -0.5, 0, 26, 20);
            }
        }
        break;
    case JUEGO:
        mundo.clickRaton(boton, estadoR, x, y);
        break;
    case PAUSA:
        //porque queremos que el menu pausa se controle con el raton
        if (boton == GLUT_LEFT_BUTTON && estadoR == GLUT_DOWN) {
            //estaba dando errores lo del raton asi que:
            int anchoVentana = glutGet(GLUT_WINDOW_WIDTH);
            int altoVentana = glutGet(GLUT_WINDOW_HEIGHT);
            
            //esto como siempre lo de traducir coordenadas a las de OpenGL
            float mouseX = ((float)x / (float)anchoVentana) * 20.0f - 10.0f;
            float mouseY = ((1.0f - (float)y / (float)altoVentana)) * 20.0f - 10.0f;

            //Con esta función comprobamos que el raton ha hecho click en las coordenadas donde se encuentra nuestro boton verde 
            if (mouseX >= -3.0f && mouseX <= 3.0f && mouseY >= 1.0f && mouseY <= 3.0f) {
                estado = estadoAnterior; //ahora vuelve al estado en el que estaba
            }

            //lo mismo de arriba pero comprobando si son las coordenadas del botn rojo
            else if (mouseX >= -3.0f && mouseX <= 3.0f && mouseY >= -3.0f && mouseY <= -1.0f) {
                estado = MENU;
                fondo = ETSIDI::Sprite("imagenes/menuprincipal.png", 0, 0, 20, 20);
                mundo.inicializa(0); // Reseteamos el mundo por si quieren echar otra partida
                ETSIDI::stopMusica();
                ETSIDI::playMusica("sonidos/menuppal2.wav", true);
            }
        }
        break;
    case BATALLA:
        break;
    }
}

void Coordinador::mueve() {
    float dt = 0.05f;
    switch (estado) {
    case JUEGO:
        mundo.mueve();

        //logica ia para el tablero
        if (modoUnJugador && mundo.faseActual == Mundo::TURNO_OSCURIDAD) {

            
            // Si hay una pieza moviéndose o un hechizo activo, esperamos.
            if (mundo.seleccionada != nullptr && mundo.seleccionada->estaAnimando()) {
                return;
            }

            timerIA += dt;

            //la ia ejecuta esto
            if (timerIA > 1.2f) { 
                std::vector<Vector2D> mov = IA::decidirMovimientoTablero(&mundo);

                if (mov.size() == 2) {
                    // La IA "clica" el origen
                    mundo.clickIA((int)mov[0].x, (int)mov[0].y);

                    // La IA seleccina el destino 
                    mundo.clickIA((int)mov[1].x, (int)mov[1].y);

                    std::cout << "IA ejecutando movimiento automático..." << std::endl;
                }
                timerIA = 0; // Resetear para el próximo movimiento
            }
        }

        if (mundo.hayCombate && mundo.seleccionada != nullptr && !mundo.seleccionada->estaAnimando()) {
            float luzActual = mundo.getValorLuz();
            int ventaja = 0; // Por defecto Gris/Neutral

            if (luzActual > 0.7f) ventaja = 1; // Blanco (Ventaja Alumnos)
            else if (luzActual < 0.3f) ventaja = 2; // Negro (Ventaja Profesores)
            else ventaja = 0; // Gris (Neutral)

            // Cambiamos el estado a BATALLA
            estado = BATALLA;

            ETSIDI::stopMusica();
            ETSIDI::playMusica("sonidos/pantallabatalla.wav", true);

            batalla.inicializa(mundo.atacante, mundo.defensor, mundo.tipoArenaCombate, ventaja);
            batalla.setIA(modoUnJugador);
        }

        // Comprobamos si hay ganador y quien 
        if (mundo.faseActual == Mundo::FIN_PARTIDA) {
            ETSIDI::stopMusica();
            if (mundo.obtenerGanador() == 1) {
                estado = VICTORIA_ALUMNOS;
                ETSIDI::playMusica("sonidos/cantocelestial.wav", false);  //el false es para que suena solo una vez
                if (modoUnJugador) {
                    //fondo = ETSIDI::Sprite("imagenes/victoriacontraIA.png", 0, 0, 20, 20);
                    fondo = ETSIDI::Sprite("imagenes/VictoriadeIA.png", 0, 0, 20, 20);
                }
                else {
                    fondo = ETSIDI::Sprite("imagenes/victoriaalumnos.png", 0, 0, 20, 20);
                }
                
            }
            else if (mundo.obtenerGanador() == 2) {
                estado = VICTORIA_PROFESORES;
                ETSIDI::playMusica("sonidos/cantocelestial.wav", false);
                if (modoUnJugador) {
                    //fondo = ETSIDI::Sprite("imagenes/VictoriadeIA.png", 0, 0, 20, 20);
                    fondo = ETSIDI::Sprite("imagenes/victoriacontraIA.png", 0, 0, 20, 20);
                } else {
                    fondo = ETSIDI::Sprite("imagenes/victoriaprofes.png", 0, 0, 20, 20);
                }
                
            }
            else {
                //esto seria para lo del empate.....
                estado = MENU;
                ETSIDI::playMusica("sonidos/menuppal2.wav", true);
            }
        }


        break;

    case BATALLA:
        batalla.mueve();

      

        //modo ia para la batalla
        if (modoUnJugador) {
            // Suponiendo que tienes métodos getPiezaLuz y getPiezaOscuridad en Batalla
            // o que puedes acceder a l1 (humano) y l2 (IA)
            IA::ejecutarAccionBatalla(&batalla, batalla.getPiezaJ2(), batalla.getPiezaJ1(), dt);
        }

        if (batalla.combateTerminado()) {
            estado = JUEGO;
            ETSIDI::stopMusica();
            ETSIDI::playMusica("sonidos/pantallatablero.wav", true);

            // Ahora le pasamos 3 argumentos (ganador, perdedor, y si es empate)
            mundo.finalizaCombate(batalla.obtenerGanador(), batalla.obtenerPerdedor(), batalla.esEmpate());
        }
        break;
    }
}

void Coordinador::gestionaRatonPasivo(int x, int y) {

    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);

    switch (estado) {
    case JUEGO:
        mundo.raton.actualizaPosicion(x, y, ancho, alto);
        break;
    case PAUSA:
    {//como hemos creado variables en un switch hay que poner {}
        //estaba dando errores lo del raton asi que:
        int anchoVentana = glutGet(GLUT_WINDOW_WIDTH);
        int altoVentana = glutGet(GLUT_WINDOW_HEIGHT);
        //estos es lo de cambio de coordenadas
        float mouseX = ((float)x / (float)anchoVentana) * 20.0f - 10.0f;
        float mouseY = ((1.0f - (float)y / (float)altoVentana)) * 20.0f - 10.0f;
        
        
        //comprobamos si toca boton verde
        if (mouseX >= -3.0f && mouseX <= 3.0f && mouseY >= 1.0f && mouseY <= 3.0f) {
            hoverReanudar = true;
        }
        else {
            hoverReanudar = false;
        }

        //comprobamos si toca boton rojo
        if (mouseX >= -3.0f && mouseX <= 3.0f && mouseY >= -3.0f && mouseY <= -1.0f) {
            hoverAbandonar = true;
        }
        else {
            hoverAbandonar = false;
        }
        break;
    }
    case INICIO:
    case MENU:
    case BATALLA:
        break;
    }
    //Forzamos que la caja de texto siga al ratón
    glutPostRedisplay();
}

void Coordinador::teclaEspecial(int key) {
    // Las flechas solo hacen algo si estamos en la pantalla de batalla
    if (estado == BATALLA) {
        batalla.teclaEspecial(key);
    }
    //añadimos esto para movernos entre las hojas de instrucciones
    else if (estado == INSTRUCCIONES) {

        // Si pulsamos DERECHA y estamos en la hoja 1, pasamos a la hoja 2
        if (key == GLUT_KEY_RIGHT) {
            if (paginaInstrucciones == 1) {
                paginaInstrucciones = 2;
                fondo = ETSIDI::Sprite("imagenes/personajes.png", -0.5, 0, 26, 20);
            }
			else if (paginaInstrucciones == 2) { // Pasamos de la hoja 2 a la hoja 3
                paginaInstrucciones = 3;
                fondo = ETSIDI::Sprite("imagenes/instruccioneshechizo.png", -0.5, 0, 26, 20);
            }
			else if (paginaInstrucciones == 3) { // Pasamos de la hoja 3 a la hoja 4
                paginaInstrucciones = 4;
                fondo = ETSIDI::Sprite("imagenes/instruccionesataques.png", -0.5, 0, 26, 20);
                
            }
        }

        // --- IR HACIA ATRÁS (Izquierda) ---
        else if (key == GLUT_KEY_LEFT) {
			if (paginaInstrucciones == 4) { // Para volver de la hoja 4 a la hoja 3
                paginaInstrucciones = 3;
                fondo = ETSIDI::Sprite("imagenes/instruccioneshechizo.png", -0.5, 0, 26, 20);
            }
			else if (paginaInstrucciones == 3) { // Para volver de la hoja 3 a la hoja 2
                paginaInstrucciones = 2;
                fondo = ETSIDI::Sprite("imagenes/personajes.png", -0.5, 0, 26, 20);
            }
			else if (paginaInstrucciones == 2) { // Para volver de la hoja 2 a la hoja 1
                paginaInstrucciones = 1;
                fondo = ETSIDI::Sprite("imagenes/comojugar.png", -0.5, 0, 26, 20);
            }
        }
    }
    
}
