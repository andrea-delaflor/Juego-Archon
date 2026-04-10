#pragma once
#include "Vector2D.h"
#include <vector>
#include <iostream>
#include <string>

// Bando al que pertenece la pieza
enum class Bando
{
    LUZ,
    OSCURIDAD
};

// Tipo de movimiento
enum class TipoMovimiento
{
    TERRESTRE,       // Sin diagonal, no atraviesa piezas
    VOLADOR,         // Libre, sin obstáculos
    TELETRANSPORTE   // Instantáneo a cualquier casilla válida
};

// Tipo de arma (para CombatArena y el renderizador)
enum class TipoArma
{
    CUERPO_A_CUERPO,  // Melee
    PROYECTIL,        // Proyectil recto
    BOLA_DE_FUEGO,    // Área pequeña
    RAYO,             // Atraviesa al enemigo
    FLECHA,           // Largo alcance
    VENENO,           // Daño continuo
    MAGIA             // Proyectil mágico (hechiceros)
};

// Avisamos al compilador de que existe la clase Tablero
class Tablero;


class Pieza
{
public:
    Pieza(const std::string& nombre,
        Bando _bando,
        int _vidaMaxima,
        float _velocidad,
        int _poderAtaque,
        float _velocidadAtaque,
        float _alcance,
        int _rangoMovimiento,
        TipoArma _arma,
        Vector2D _posInicial);

    // --- Destructor y Seguridad ---
    virtual ~Pieza() = default;

    // Bloqueamos la copia para usar siempre punteros (como en tu código original)
    Pieza(const Pieza&) = delete;
    Pieza& operator=(const Pieza&) = delete;

 
    // --- Métodos Virtuales Puros ---
    // Usamos std::vector con tu Vector2D
    virtual std::vector<Vector2D> obtenerMovimientosValidos(Tablero* tablero) = 0;
    virtual TipoMovimiento obtenerTipoMovimiento() = 0;
    virtual std::string obtenerNombreSprite() = 0;
    virtual void dibuja() = 0; // Para el renderizado

    // Habilidad especial (con cuerpo vacío para que sea opcional)
    virtual void usarHabilidadEspecial(Tablero* tablero) {}

    // --- Getters y Setters (Identidad y Posición) ---
    std::string obtenerNombre() { return nombre; }
    Bando obtenerBando() { return bando; }

    Vector2D obtenerPosicion() { return posicion; }
    void establecerPosicion(Vector2D pos) { posicion = pos; }

    // --- Gestión de Vida ---
    int obtenerVida() { return vida; }
    bool estaViva() { return viva; }
    void establecerViva(bool _viva);
    void recibirDanio(int cantidad);
    void curar(int cantidad);
    void restaurarVidaCompleta();

    // --- Atributos de Combate ---
    float obtenerVelocidad() { return velocidad; }
    int obtenerPoderAtaque() { return poderAtaque; }
    float obtenerAlcance() { return alcance; }
    int obtenerRangoMovimiento() { return rangoMovimiento; }
    TipoArma obtenerArma() { return arma; }

    // --- Estado y Bonus ---
    bool estaEncarcelada() { return encarcelada; }
    void establecerEncarcelada(bool v) { encarcelada = v; }
    float obtenerBonusCombate(Bando bandoCasilla);

    // --- Debug ---
    virtual void imprimir();

protected:
    // Atributos protegidos — accesibles para subclases
    // (Cambiaformas los necesita para copiar stats del rival)
    std::string nombre;
    Bando bando;
    Vector2D posicion;

    int vida;
    int vidaMaxima;
    bool viva;


    // Estado especial
    bool encarcelada;

    // Estadísticas de combate y tablero
    float velocidad;        // Para el orden de turno o movimiento
    int poderAtaque;        // Daño que hace
    float velocidadAtaque;  // Rapidez en la arena de combate
    float alcance;          // Distancia de ataque
    int rangoMovimiento;    // Cuántas casillas mueve en el tablero
    TipoArma arma;          // Tipo de ataque (MELEE, MAGIA, etc.)
};