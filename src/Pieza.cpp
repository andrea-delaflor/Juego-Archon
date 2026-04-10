// ============================================================
//  Pieza.cpp  —  Implementación de Pieza (base)
// ============================================================

#include "Pieza.h"
#include "Tablero.h"
#include <algorithm>   // std::max, std::min
#include <sstream>
#include <stdexcept>

// Constructor
Pieza::Pieza(const std::string& nombre,
    Bando              bando,
    int                vidaMaxima,
    float              velocidad,
    int                poderAtaque,
    float              velocidadAtaque,
    float              alcance,
    int                rangoMovimiento,
    TipoArma           arma)
    : m_nombre(nombre)
    , m_bando(bando)
    , m_posicion({ 0, 0 })        // El Tablero lo fija al colocarla
    , m_vida(vidaMaxima)
    , m_vidaMaxima(vidaMaxima)
    , m_viva(true)
    , m_encarcelada(false)
    , m_velocidad(velocidad)
    , m_poderAtaque(poderAtaque)
    , m_velocidadAtaque(velocidadAtaque)
    , m_alcance(alcance)
    , m_rangoMovimiento(rangoMovimiento)
    , m_arma(arma)
{
    if (vidaMaxima <= 0)
        throw std::invalid_argument("Pieza: vidaMaxima debe ser mayor que 0");
}

//  Identidad 
const std::string& Pieza::obtenerNombre() const { return m_nombre; }
Bando              Pieza::obtenerBando()  const { return m_bando; }

//  Posición 
Posicion Pieza::obtenerPosicion()                    const { return m_posicion; }
void     Pieza::establecerPosicion(const Posicion& pos) { m_posicion = pos; }

//  Vida 
int  Pieza::obtenerVida()       const { return m_vida; }
int  Pieza::obtenerVidaMaxima() const { return m_vidaMaxima; }
bool Pieza::estaViva()          const { return m_viva; }

void Pieza::establecerViva(bool viva)
{
    m_viva = viva;
    // Si la revivimos y no tiene vida, le damos 1 mínimo
    if (viva && m_vida <= 0)
        m_vida = 1;
}

void Pieza::recibirDanio(int cantidad)
{
    if (!m_viva)    return;
    if (cantidad < 0) cantidad = 0;

    m_vida -= cantidad;

    if (m_vida <= 0)
    {
        m_vida = 0;
        m_viva = false;
    }
}

void Pieza::curar(int cantidad)
{
    if (!m_viva || cantidad <= 0) return;
    m_vida = (std::min)(m_vida + cantidad, m_vidaMaxima);
}

void Pieza::restaurarVidaCompleta()
{
    m_vida = m_vidaMaxima;
    m_viva = true;
}

//  Atributos de combate 
float    Pieza::obtenerVelocidad()       const { return m_velocidad; }
int      Pieza::obtenerPoderAtaque()     const { return m_poderAtaque; }
float    Pieza::obtenerVelocidadAtaque() const { return m_velocidadAtaque; }
float    Pieza::obtenerAlcance()         const { return m_alcance; }
int      Pieza::obtenerRangoMovimiento() const { return m_rangoMovimiento; }
TipoArma Pieza::obtenerArma()            const { return m_arma; }

//  Estado especial 
bool Pieza::estaEncarcelada()              const { return m_encarcelada; }
void Pieza::establecerEncarcelada(bool v) { m_encarcelada = v; }

//  Bonus de combate 
float Pieza::obtenerBonusCombate(Bando bandoCasilla) const
{
    // Casilla del propio color → ventaja del 25%
    if (bandoCasilla == m_bando)
        return 1.25f;

    // Casilla de color contrario → ligera desventaja
    return 0.85f;
}

//  Debug 
std::string Pieza::aTexto() const
{
    std::ostringstream oss;
    oss << m_nombre
        << " [" << (m_bando == Bando::LUZ ? "LUZ" : "OSCURIDAD") << "]"
        << " Vida:" << m_vida << "/" << m_vidaMaxima
        << " pos:(" << m_posicion.first << "," << m_posicion.second << ")"
        << (m_viva ? "" : " [MUERTA]")
        << (m_encarcelada ? " [ENCARCELADA]" : "");
    return oss.str();
}