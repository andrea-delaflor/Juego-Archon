#pragma once
#include "Vector2D.h"

class Raton
{
public:
    // Estas líneas son las que eliminan el error en el .cpp
    Vector2D casilla{ -1.0f, -1.0f };
    Vector2D posicion{ 0.0f, 0.0f };

    // Declaración de los métodos (deben coincidir con el .cpp)
    void actualizaPosicion(int x, int y, int ancho, int alto);
    void dibuja();
};
