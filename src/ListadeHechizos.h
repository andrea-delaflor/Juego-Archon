#pragma once
#include "Hechizo.h"

/*
 IMPLEMENTACIONES CONCRETAS DE HECHIZOS
 Aquí definimos el comportamiento específico de cada magia mediante HERENCIA.
 Cada clase hija (ej: HechizoHeal) hereda de la clase base 'Hechizo' y
 SOBREESCRIBE (override) el método 'aplicar'.
 Cada hechizo es una clase distinta.
 */

class Mundo;

// 1. TELEPORT: Mueve una pieza aliada a cualquier casilla vacía
class HechizoTeleport : public Hechizo {
public:
    HechizoTeleport() : Hechizo("Teleport") {}
    void aplicar(Mundo* mundo, Vector2D destino) override;
};

// 2. HEAL: Restaura la vida de una pieza al máximo
class HechizoHeal : public Hechizo {
public:
    HechizoHeal() : Hechizo("Heal") {}
    void aplicar(Mundo* mundo, Vector2D destino) override;
};

// 3. SHIFT TIME: Invierte el ciclo de luz del tablero
class HechizoShiftTime : public Hechizo {
public:
    HechizoShiftTime() : Hechizo("Shift Time") {}
    void aplicar(Mundo* mundo, Vector2D destino) override;
};

// 4. EXCHANGE: Intercambia las posiciones de dos piezas
class HechizoExchange : public Hechizo {
public:
    HechizoExchange() : Hechizo("Exchange") {}
    void aplicar(Mundo* mundo, Vector2D destino) override;
};

// 5. IMPRISON: Inmoviliza una pieza enemiga
class HechizoImprison : public Hechizo {
public:
    HechizoImprison() : Hechizo("Imprison") {}
    void aplicar(Mundo* mundo, Vector2D destino) override;
};

// 6. REVIVE: Resucita la última pieza muerta del bando que lanza el hechizo
class HechizoRevive : public Hechizo {
public:
    HechizoRevive() : Hechizo("Revive") {}
    void aplicar(Mundo* mundo, Vector2D destino) override;
};

// 7. SUMMON: Invoca un elemental (Lógica simplificada)
class HechizoSummon : public Hechizo {
public:
    HechizoSummon() : Hechizo("Summon") {}
    void aplicar(Mundo* mundo, Vector2D destino) override;
};
