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

    void inicializa();
    void mueve();
    void dibuja();
    void clickRaton(int button, int state, int x, int y);
    Raton raton;               // Objeto que calcula la casilla
    Pieza* seleccionada = nullptr; // Puntero a la pieza que queremos mover
    bool hayCombate = false; // El flag para salir a la pantalla de lucha
    Pieza* atacante = nullptr;
    Pieza* defensor = nullptr;
    Pieza* posibleDefensor = nullptr; // Posible defensor para que el flag salte despues de que acabe la animacion de la pieza
    void resetCombate() { hayCombate = false; atacante = defensor = nullptr; }  // Método para limpiar el flag tras leerlo
private:
    Tablero tablero;
    float valorLuz = 0.5f;
    float angulo = 0.0f;

    std::vector<Pieza*> piezasLuz;
    std::vector<Pieza*> piezasOscuridad;
};