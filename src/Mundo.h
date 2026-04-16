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
#include <vector>

class Mundo {
public:
    enum FaseJuego {
        TURNO_LUZ,
        TURNO_OSCURIDAD,
        ANIMANDO_MOVIMIENTO,
        FIN_PARTIDA
    };

    FaseJuego faseActual = TURNO_LUZ;

    Mundo();
    ~Mundo();

    void inicializa(int estado);
    void mueve();
    void dibuja(int estado);
    void clickRaton(int button, int state, int x, int y);

    // Función nueva para resolver la victoria de la batalla
    void finalizaCombate(Pieza* ganador, Pieza* perdedor);

    Raton raton;
    Pieza* seleccionada = nullptr;
    bool hayCombate = false;
    Pieza* atacante = nullptr;
    Pieza* defensor = nullptr;
    Pieza* posibleDefensor = nullptr;

    void resetCombate() { hayCombate = false; atacante = defensor = nullptr; }

private:
    Tablero tablero;
    float valorLuz = 0.5f;
    float angulo = 0.0f;

    std::vector<Pieza*> piezasLuz;
    std::vector<Pieza*> piezasOscuridad;
};