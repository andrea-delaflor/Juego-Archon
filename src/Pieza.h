#pragma once
#include "Vector2D.h"
#include <vector>
#include <string>

enum class Bando { LUZ, OSCURIDAD };
enum class TipoMovimiento { TERRESTRE, VOLADOR, TELETRANSPORTE };
enum class TipoArma {
    CUERPO_A_CUERPO, PROYECTIL, BOLA_DE_FUEGO,
    RAYO, FLECHA, VENENO, MAGIA
};

class Tablero;

class Pieza {
public:
    // Constructor LIMPIO (sin Movimiento* al final)
    Pieza(const std::string& _nombre,
        Bando _bando,
        int _vidaMaxima,
        float _velocidad,
        int _poderAtaque,
        float _velocidadAtaque,
        float _alcance,
        int _rangoMovimiento,
        TipoArma _arma,
        Vector2D _posInicial);

    virtual ~Pieza() = default; 

    Pieza(const Pieza&) = delete;
    Pieza& operator=(const Pieza&) = delete;

    // ── Métodos virtuales puros ──────────────────────────────
    virtual std::string obtenerNombreSprite() = 0;
    virtual void dibuja() = 0;
    virtual TipoMovimiento obtenerTipoMovimiento() = 0; // Obliga a los hijos a decir qué son
    virtual void usarHabilidadEspecial(Tablero* tablero) {}

   
    std::vector<Vector2D> obtenerMovimientosValidos(Tablero* tablero);

    // ── Getters ──────────────────────────────────────────────
    std::string obtenerNombre() { return nombre; }
    Bando       obtenerBando() { return bando; }
    Vector2D    obtenerPosicion() { return posicion; }
    int         obtenerVida() { return vida; }
    bool        estaViva() { return viva; }
    int         obtenerRangoMovimiento() { return rangoMovimiento; }
    TipoArma    obtenerArma() { return arma; }
    float       obtenerAlcance() { return alcance; }
    int         obtenerPoderAtaque() { return poderAtaque; }
    bool        estaEncarcelada() { return encarcelada; }

    // ── Setters ──────────────────────────────────────────────
    void establecerPosicion(Vector2D pos) { posicion = pos; }
    void establecerEncarcelada(bool v) { encarcelada = v; }
    void establecerViva(bool _viva);

    // ── Gestión de vida ──────────────────────────────────────
    void  recibirDanio(int cantidad);
    void  curar(int cantidad);
    void  restaurarVidaCompleta();
    float obtenerBonusCombate(Bando bandoCasilla);

    virtual void imprimir();

protected:
    std::string nombre;
    Bando       bando;
    Vector2D    posicion;
    int         vida;
    int         vidaMaxima;
    bool        viva;
    bool        encarcelada;
    float       velocidad;
    int         poderAtaque;
    float       velocidadAtaque;
    float       alcance;
    int         rangoMovimiento;
    TipoArma    arma;

   
};