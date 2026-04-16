#pragma once
#include "Pieza.h"
#include "ETSIDI.h"
#include "Vector2D.h"
#include <vector>  // ¡NUEVO! Para poder hacer listas de elementos
#include <string>  // ¡NUEVO! Para guardar los textos de las rutas de las imágenes

class Batalla {
public:
    Batalla();

    void inicializa(Pieza* atacante, Pieza* defensor);
    void dibuja();
    void mueve();
    void tecla(unsigned char key);

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

    // ¡NUEVAS VARIABLES PARA ALTERNAR ARENAS!
    std::vector<std::string> rutasArenas; // Aquí guardaremos los nombres de los archivos de imagen
    int indiceArenaActual;                // Nos dice en qué arena de la lista estamos peleando ahora
};