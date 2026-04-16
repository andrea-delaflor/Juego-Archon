#pragma once
#include "Pieza.h"
#include "ETSIDI.h"
#include "Vector2D.h"

class Batalla {
public:
    Batalla();// Constructor de la clase (se llama cuando se crea la Batalla)

    void inicializa(Pieza* atacante, Pieza* defensor);// Función que prepara la arena ANTES de empezar a pelear
    void dibuja();// Función que dibuja el fondo y los personajes en pantalla
    void mueve();// Función para manejar el movimiento en tiempo real (disparos, caminar...)
    void tecla(unsigned char key);// Función para leer las teclas que pulsamos en la pelea
   
    // Funciones "para avisar" al Coordinador sepa qué ha pasado en la pelea
    bool combateTerminado() const { return terminado; }
    Pieza* obtenerGanador() const { return ganador; }
    Pieza* obtenerPerdedor() const { return perdedor; }

private:
    Pieza* l1; // Luchador 1 (Atacante)
    Pieza* l2; // Luchador 2 (Defensor)

    Vector2D pos1; // Posición temporal en la arena para L1
    Vector2D pos2; // Posición temporal en la arena para L2

    // Vidas de los luchadores (HP = Health Points)
    int hp1;
    int hp2;

    // Variables para controlar el final
    bool terminado; // Se pone a 'true' cuando alguien muere
    Pieza* ganador; // Guardará quién ha ganado
    Pieza* perdedor; // Guardará quién ha muerto

    ETSIDI::Sprite fondoArena;
};
