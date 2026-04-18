#pragma once
#include "ETSIDI.h"

// Promesa al compilador para no tener dependencias circulares
class Pieza; 

class Tablero {
public:
    // Constructor para inicializar el tablero vacío
    Tablero();

    // Dibuja el tablero pasando la luminosidad actual (0.0 a 1.0)
    void dibuja(float luminosidad);
    void vaciar();
    //Función para saber de qué color / tipo es una casilla específica
        // Devuelve: 
        //   0 si la casilla es Blanca (Arena de luz)
        //   1 si la casilla es Negra (Arena de oscuridad)
        //   2 si la casilla es Variable (las que cambian de color)
        //   3 si la casilla es un Punto de Poder
        // =====================================================================
        int obtenerTipoArena(int i, int j, float luminosidad);
    // --- FUNCIONES NECESARIAS PARA EL MOVIMIENTO ---
    bool posicionValida(int i, int j);
    Pieza* obtenerOcupante(int i, int j);
    void colocarPieza(int i, int j, Pieza* p); // Para poder meter las fichas

private:
    Pieza* casillas[9][9]; //Matriz de punteros --> la usamos para determinar entre casilla ocupada y libre

    // Punto de poder : E5, A5, I5, E1, E9
    bool esPowerPoint(int i, int j);

    // Casillas variables: Cruz central + diagonales
    bool esVariable(int i, int j);
};