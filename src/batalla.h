#pragma once
#include "Pieza.h"
#include "ETSIDI.h"
#include "Vector2D.h"
#include "Proyectil.h"
#include <vector>  // Para poder hacer listas de elementos
#include <string>  //Para guardar los textos de las rutas de las imágenes
#include <list>

class Batalla {
public:
    Batalla();

    void inicializa(Pieza* atacante, Pieza* defensor, int tipoArena);
    void dibuja();
    void mueve();
    void tecla(unsigned char key);
    void teclaEspecial(int key);

    bool combateTerminado() const { return terminado; }
    Pieza* obtenerGanador() const { return ganador; }
    Pieza* obtenerPerdedor() const { return perdedor; }
    bool esEmpate() const { return empate; }

private:
    Pieza* l1;
    Pieza* l2;

    Vector2D pos1;
    Vector2D pos2;

    int hp1;
    int hp2;

    bool terminado;
    bool empate;
    Pieza* ganador;
    Pieza* perdedor;

    ETSIDI::Sprite fondoArena;

    // ALTERNAR ARENAS
    std::vector<std::string> rutasArenas; 
    int indiceArenaActual;                // Nos dice en qué arena de la lista estamos peleando ahora

    std::list<Proyectil*> proyectiles;
    void generarDisparo(bool esJugador1);
};