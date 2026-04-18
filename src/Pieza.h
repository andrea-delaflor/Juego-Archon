#pragma once
#include "Vector2D.h"
#include <vector>
#include <string>

enum class Bando { LUZ, OSCURIDAD };
enum class TipoMovimiento { TERRESTRE, VOLADOR, TELETRANSPORTE };
enum class TipoArma {
    CUERPO_A_CUERPO, // Para Golem, Troll, etc.
    PELOTAFUTBOL,     // Arquera 
    BOLA_DE_FUEGO,   // Dragón
    RAYO_LASER,       // Djinni (Láser)
    RAYO_NUMERICO,   // Basilisco (Cálculo)
    ACTAS           // Mago y Bruja 
};

class Tablero;

class Pieza {
public:

    virtual bool esLider() { return false; }

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
    //actualiza la trayectoria
    void actualizar(float dt);

    // ── Métodos virtuales puros ─
    virtual std::string obtenerNombreSprite() = 0;
    virtual void dibuja() = 0;
    virtual TipoMovimiento obtenerTipoMovimiento() = 0; // Obliga a los hijos a decir qué son
    virtual void usarHabilidadEspecial(Tablero* tablero) {}


    std::vector<Vector2D> obtenerMovimientosValidos(Tablero* tablero);

    // ── Getters ─
    std::string obtenerNombre() const  { return nombre; }
    Bando       obtenerBando() const { return bando; }
    Vector2D    obtenerPosicion() const { return posicion; }
    int         obtenerVida() const { return vida; }
    bool        estaViva() const { return viva; }
    int         obtenerRangoMovimiento() const { return rangoMovimiento; }
    TipoArma    obtenerArma() const { return arma; }
    float       obtenerAlcance() const { return alcance; }
    int         obtenerPoderAtaque() const { return poderAtaque; }
	bool        estaEncarcelada() const { return encarcelada; } // Para el hechizo Imprison
	float       getLuzDeCaptura() const { return luzDeCaptura; } // Para el hechizo Imprison, nos dice la luz del mundo cuando fue encarcelada, para saber cuándo liberarla
	void establecerEncarcelada(bool estado, float luz = 0.0f) { // Para el hechizo Imprison, al establecer encarcelada a true, guardamos la luz actual del mundo para luego compararla y saber cuándo liberar a la pieza
        encarcelada = estado;
        luzDeCaptura = luz;
    }

    //para las armas correctas
    TipoArma obtenerArma() const { return arma; }

    Vector2D    obtenerPosicionVisual() const { return posicionVisual; } // Posición suave
    bool        estaAnimando() const { return animando; }

    // ── Setters ──
    void establecerPosicion(Vector2D pos);
    void establecerViva(bool _viva);

    // ── Gestión de vida ──
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
    bool        encarcelada=false;
    float       velocidad;
    int         poderAtaque;
    float       velocidadAtaque;
    float       alcance;
    int         rangoMovimiento;
    TipoArma    arma;

    Vector2D    posicionVisual; // Coordenadas para el dibujo 
    bool        animando;       // Indica si la pieza se está desplazando

  
    float luzDeCaptura = 0.0f;
};