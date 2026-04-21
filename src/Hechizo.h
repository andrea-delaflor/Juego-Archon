#pragma once
#include <string>
#include "Vector2D.h"


/* CLASE BASE ABSTRACTA: Hechizo
 Define la interfaz común para todos los hechizos.
 - Es ABSTRACTA: No puedes crear un objeto 'Hechizo' a secas (por el '= 0').
 - POLIMORFISMO: El método 'aplicar' es virtual puro. Esto obliga a las clases hijas
 a implementar su propia lógica (curar, mover, etc.), permitiendo que el Mundo
 use cualquier hechizo sin saber exactamente qué hace por dentro.
 */
class Mundo; 

class Hechizo {
protected:
    std::string nombre;
    bool usado;

public:
    Hechizo(std::string n) : nombre(n), usado(false) {}
    virtual ~Hechizo() = default;

    // Métodos de consulta
    std::string getNombre() const { return nombre; }
    bool estaUsado() const { return usado; }

    // Método principal que cada hechizo implementará
    virtual void aplicar(Mundo* mundo, Vector2D destino) = 0;
};