#pragma once
#include "mundo.h"
#include "ETSIDI.h"
#include "batalla.h"

class Coordinador {
public:
    enum Estado { INICIO, MENU, JUEGO, PAUSA, BATALLA, VICTORIA_ALUMNOS, VICTORIA_PROFESORES };

private:
    Estado estado;
    Estado estadoAnterior; //necesitamos esto para despues de la pausa volver al estado en el que estabamos
    Mundo mundo;
    Batalla batalla;
    ETSIDI::Sprite fondo;
    
public:
    Coordinador();

    void tecla(unsigned char key);
    void dibuja();
    void mueve();
    void gestionaRaton(int boton, int estadoR, int x, int y);
    void gestionaRatonPasivo(int x, int y);
};