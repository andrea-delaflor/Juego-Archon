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

    void finalizaCombate(Pieza* ganador, Pieza* perdedor, bool empate);

    Raton raton;
    Pieza* seleccionada = nullptr;

    bool hayCombate = false;
    Pieza* atacante = nullptr;
    Pieza* defensor = nullptr;
    Pieza* posibleDefensor = nullptr;

    //Aquí guardaremos el número(0, 1 o 2) que nos diga el tablero para pasárselo a la arena
        int tipoArenaCombate = 0;
    void resetCombate() { hayCombate = false; atacante = defensor = nullptr; }

private:
    Tablero tablero;
    float valorLuz = 0.5f;
    float angulo = 0.0f;

    std::vector<Pieza*> piezasLuz;
    std::vector<Pieza*> piezasOscuridad;

    // Función interna segura para borrar una pieza y que desaparezca gráficamente
    void eliminarPieza(Pieza* p);
};