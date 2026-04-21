#pragma once
#include "Vector2D.h"
#include "Vida.h"
#include <vector>
#include <string>

enum class Bando { LUZ, OSCURIDAD };
enum class TipoMovimiento { TERRESTRE, VOLADOR, TELETRANSPORTE };
enum class TipoArma {
    CUERPO_A_CUERPO,
    PELOTAFUTBOL,
    BOLA_DE_FUEGO,
    RAYO_LASER,
    RAYO_NUMERICO,
    ACTAS,
    ESCUDO
};

class Tablero;

class Pieza {
public:
    // Constructor
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

    virtual ~Pieza() {
        if (salud) delete salud;
    }

    // Deshabilitar copia
    Pieza(const Pieza&) = delete;
    Pieza& operator=(const Pieza&) = delete;

    // Métodos virtuales y lógicos
    virtual bool esLider() { return false; }
    void actualizar(float dt);
    virtual std::string obtenerNombreSprite() = 0;
    virtual void dibuja() = 0;
    virtual TipoMovimiento obtenerTipoMovimiento() = 0;
    virtual void usarHabilidadEspecial(Tablero* tablero) {}
    virtual void imprimir();
    std::vector<Vector2D> obtenerMovimientosValidos(Tablero* tablero);

    // GETTERS
    std::string obtenerNombre() const { return nombre; }
    Bando obtenerBando() const { return bando; }
    Vector2D obtenerPosicion() const { return posicion; }
    bool estaViva() const { return viva; }
    int obtenerRangoMovimiento() const { return rangoMovimiento; }
    TipoArma obtenerArma() const { return arma; }
    float obtenerAlcance() const { return alcance; }
    int obtenerPoderAtaque() const { return poderAtaque; }
    bool estaEncarcelada() const { return encarcelada; }
    float getLuzDeCaptura() const { return luzDeCaptura; }
    Vector2D obtenerPosicionVisual() const { return posicionVisual; }
    bool estaAnimando() const { return animando; }

    // OBTENER VIDA ÚNICO 
	//SPARA IMPRIMIR VIDA EN LA PANTALLA, NO PARA GESTIONAR VIDA
	int obtenerVida() const { // Devuelve la vida actual o 0 si no hay salud
        return (salud != nullptr) ? salud->getActual() : 0;
    }
	//PARA GESTIONAR VIDA DESDE EL MUNDO, NO PARA IMPRIMIR
	Vida& getVida() { // Devuelve una referencia a la vida para saber cuanta vida tiene la pieza
        return *salud;
    }

	// OBTENER RADIO DE ATAQUE ÚNICO (para la lógica de batalla)
    virtual float getRadio() { return 3.5f; } // Valor por defecto

    // SETTERS
    void establecerPosicion(Vector2D pos);
    void establecerViva(bool _viva);
    void establecerEncarcelada(bool estado, float luz = 0.0f) {
        encarcelada = estado;
        luzDeCaptura = luz;
    }

    // GESTIÓN DE VIDA ÚNICA
    //funcion damage desde vida se repite
    /*
    void recibirDanio(int cantidad) {
        if (salud != nullptr) {
            salud->damage(cantidad);
            if (salud->muerto()) viva = false;
        }
    }*/

    void restaurarVidaCompleta() {
        if (salud != nullptr) {
            salud->heal(1000); // Cura al máximo
        }
        viva = true;
    }

    void dibujaCorazones(float x, float y, float e) {
        if (salud != nullptr) salud->dibuja(x, y, e);
    }

    // BATALLA Y ANIMACIÓN
    virtual void dibujaEnBatalla() { dibuja(); }
    void iniciarAnimacion() { atacando = true; }
    void actualizaAnimacionAtaque(float dt);

    void actualizarEscudo(float dt) {
        if (escudoActivado) {
            tiempoEscudoActivo -= dt;
            if (tiempoEscudoActivo <= 0) escudoActivado = false;
        }
        if (bloqueoEscudo > 0) bloqueoEscudo -= dt;
    }

    void activarEscudo() {
        if (bloqueoEscudo <= 0) {
            escudoActivado = true;
            tiempoEscudoActivo = 4.0f;  // Dura 4 segundos
            bloqueoEscudo = 10.0f;     // Recarga en 10 segundos
        }
    }

    bool tieneEscudoActivo() { return escudoActivado; }

    void forzarPosicionVisual(Vector2D pos) { posicionVisual = pos; }

protected:
    std::string nombre;
    Bando bando;
    Vector2D posicion;
    bool viva;
    bool encarcelada = false;
    float velocidad;
    int poderAtaque;
    float velocidadAtaque;
    float alcance;
    int rangoMovimiento;
    TipoArma arma;

    Vector2D posicionVisual;
    bool animando;

    float anguloAtaque = 0.0f;
    bool atacando = false;
    float luzDeCaptura = 0.0f;

    float bloqueoEscudo = 0.0f;
    float tiempoEscudoActivo = 0.0f;
    bool escudoActivado = false;

    // PUNTERO ÚNICO A SALUD
    Vida* salud = nullptr;
};