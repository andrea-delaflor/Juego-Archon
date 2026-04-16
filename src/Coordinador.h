#pragma once
#include "mundo.h"
#include "ETSIDI.h"

class Coordinador {
public:
    enum Estado { INICIO, MENU, JUEGO, PAUSA, BATALLA, VICTORIA_ALUMNOS, VICTORIA_PROFESORES };

private:
    Estado estado;
    Mundo mundo;
    ETSIDI::Sprite fondo;
    
public:
    Coordinador();

    void tecla(unsigned char key);
    void dibuja();
    void mueve();
    void gestionaRaton(int boton, int estadoR, int x, int y);
    void gestionaRatonPasivo(int x, int y);
};