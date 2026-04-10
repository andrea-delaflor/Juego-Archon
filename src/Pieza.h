#pragma once
#include <string>
#include <vector>

// i (columna), j (fila)
using Posicion = std::pair<int, int>;   

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
        Bando              bando,
        int                vidaMaxima,
        float              velocidad,
        int                poderAtaque,
        float              velocidadAtaque,
        float              alcance,
        int                rangoMovimiento,
        TipoArma           arma);

    virtual ~Pieza() = default;

    // Prohibimos clonar una pieza ---> Pieza p2 = p1 (no se puede, usamos punteros)
    Pieza(const Pieza&) = delete;
    Pieza& operator=(const Pieza&) = delete;

    // ── Métodos virtuales puros (cada subclase los implementa) ──

    // Devuelve las posiciones válidas a las que puede moverse
    virtual std::vector<Posicion> obtenerMovimientosValidos(const Tablero& tablero) const = 0;

    // Tipo de movimiento de esta pieza
    virtual TipoMovimiento obtenerTipoMovimiento() const = 0;

    // Nombre del sprite para el renderizador (ej: "luz_caballero")
    virtual std::string obtenerNombreSprite() const = 0;

    // Habilidad especial — por defecto no hace nada
    // Las subclases con habilidad (Fenix, Cambiaformas) la sobreescriben
    virtual void usarHabilidadEspecial(Tablero& tablero) {}

    // ── Identidad ────────────────────────────────────────────
    const std::string& obtenerNombre() const;
    Bando              obtenerBando()  const;

    // ── Posición en el tablero ───────────────────────────────
    Posicion obtenerPosicion()                 const;
    void     establecerPosicion(const Posicion& pos);

    // ── Vida ─────────────────────────────────────────────────
    int     obtenerVida()       const;
    int     obtenerVidaMaxima() const;
    bool    estaViva()          const;
    void    establecerViva(bool viva);

    // Recibe daño — si la vida llega a 0 marca la pieza como muerta
    void    recibirDanio(int cantidad);

    // Cura una cantidad concreta (sin superar el máximo)
    void    curar(int cantidad);

    // Restaura la vida al máximo (HealSpell / inicio de arena)
    void    restaurarVidaCompleta();

    // ── Atributos de combate ─────────────────────────────────
    float    obtenerVelocidad()        const;
    int      obtenerPoderAtaque()      const;
    float    obtenerVelocidadAtaque()  const;   // Ataques por segundo en arena
    float    obtenerAlcance()          const;   // Alcance del arma en la arena
    int      obtenerRangoMovimiento()  const;   // Casillas por turno en tablero
    TipoArma obtenerArma()             const;

    // ── Estado especial ──────────────────────────────────────
    bool estaEncarcelada()               const;
    void establecerEncarcelada(bool v);

    // ── Bonus de combate por color de casilla ────────────────
    // Devuelve multiplicador: 1.25 en casilla propia, 0.85 en contraria
    float obtenerBonusCombate(Bando bandoCasilla) const;

    // ── Debug ────────────────────────────────────────────────
    virtual std::string aTexto() const;

protected:
    // Atributos protegidos — accesibles para subclases
    // (Cambiaformas los necesita para copiar stats del rival)
    std::string m_nombre;
    Bando       m_bando;
    Posicion    m_posicion;

    int         m_vida;
    int         m_vidaMaxima;
    bool        m_viva;
    bool        m_encarcelada;

    float       m_velocidad;
    int         m_poderAtaque;
    float       m_velocidadAtaque;
    float       m_alcance;
    int         m_rangoMovimiento;
    TipoArma    m_arma;
};