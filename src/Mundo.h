#pragma once
#include "Tablero.h"
#include "Vector2D.h"
#include "ETSIDI.h"
#include "PiezaTest.h"
#include "Raton.h"
#include "GolemL.h"
#include "MagoL.h"
#include "BrujaO.h"
#include "DragonO.h"
#include"DjiniL.h"
#include"FenixL.h"
#include"BansheeO.h"
#include "ArqueraL.h"
#include "BasiliscoO.h"
#include "TrollO.h"

class Mundo {
public:
    //creamos los estados del juego:

    enum FaseJuego {
        TURNO_LUZ,
        TURNO_OSCURIDAD,
        ANIMANDO_MOVIMIENTO,
        FIN_PARTIDA
    };

    FaseJuego faseActual = TURNO_LUZ;

    Mundo();          // Constructor
    ~Mundo();

    void inicializa(int estado);
    void mueve();
    void dibuja(int estado);
    void clickRaton(int button, int state, int x, int y);
    Raton raton;               // Objeto que calcula la casilla
    Pieza* seleccionada = nullptr; // Puntero a la pieza que queremos mover
private:
    Tablero tablero;
    float valorLuz = 0.5f;
    float angulo = 0.0f;


    std::vector<Pieza*> piezasLuz;
    std::vector<Pieza*> piezasOscuridad;
};