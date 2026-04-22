#pragma once
#include "mundo.h"
#include "ETSIDI.h"
#include "batalla.h"

class Coordinador {
public:
    enum Estado { INICIO, MENU, JUEGO, PAUSA, BATALLA, VICTORIA_ALUMNOS, VICTORIA_PROFESORES, INSTRUCCIONES };

private:
    Estado estado;
    Estado estadoAnterior; //necesitamos esto para despues de la pausa volver al estado en el que estabamos
    int paginaInstrucciones = 1; //esto lo añado poruq en Intrucciones quiero dos hojas
    Mundo mundo;
    Batalla batalla;
    ETSIDI::Sprite fondo;

    //estos dos bool actuan como un sensor de detectar el raton
    bool hoverReanudar = false;
    bool hoverAbandonar = false;
    bool modoUnJugador = false; //esto lo vamos a utilizar para saber si estamos en modo 1 o 2 jugadores
    
public:
    Coordinador();

    void tecla(unsigned char key);
    void teclaEspecial(int key);
    void dibuja();
    void mueve();
    void gestionaRaton(int boton, int estadoR, int x, int y);
    void gestionaRatonPasivo(int x, int y);
};