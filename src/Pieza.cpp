#include "Pieza.h"
#include "Tablero.h"
#include "Movimiento.h"
#include <iostream>
#include<cmath>

Pieza::Pieza(const std::string& _nombre,
    Bando _bando,
    int _vidaMaxima,
    float _velocidad,
    int _poderAtaque,
    float _velocidadAtaque,
    float _alcance,
    int _rangoMovimiento,
    TipoArma _arma,
    Vector2D _posInicial)
{
    nombre = _nombre;
    bando = _bando;
    viva = true;
    velocidad = _velocidad;
    poderAtaque = _poderAtaque;
    velocidadAtaque = _velocidadAtaque;
    alcance = _alcance;
    rangoMovimiento = _rangoMovimiento;
    arma = _arma;

    posicion = _posInicial;
    posicionVisual = _posInicial;
    animando = false;
    encarcelada = false;
    salud = nullptr; 
}

//Para ir actualizando la taryectoria
void Pieza::actualizar(float velocidadtrayectoria) {
    // Si la visual es distinta a la lógica, es que debe moverse
    Vector2D dir = posicion - posicionVisual;

    // Calculamos la distancia
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist > 0.01f) {
        // En lugar de dt, usamos un valor fijo pequeño para que se mueva
        // cada vez que se dibuja. 
        //float velocidadAnimacion = 0.0005f;
        
        posicionVisual.x += (float)(dir.x * velocidadtrayectoria);
        posicionVisual.y += (float)(dir.y * velocidadtrayectoria);
    }
    else {
        posicionVisual = posicion;
        animando = false; //Con esto mandamos una señal a mundo para que pueda salir del estado ANIMANDO_MOVIMIENTO
    }
}

void Pieza::establecerPosicion(Vector2D pos) {
    posicion = pos;

    // Si la pieza se teletransporta, la visual salta de inmediato
    if (obtenerTipoMovimiento() == TipoMovimiento::TELETRANSPORTE) {
        posicionVisual = pos;
        animando = false;
    }
    else {
        // Si es terrestre o volador, activamos la transición suave
        animando = true;
    }
}

// logica del movimiento
std::vector<Vector2D> Pieza::obtenerMovimientosValidos(Tablero* tablero) {
    Movimiento motor;
    TipoMovimiento miTipo = this->obtenerTipoMovimiento();

    if (miTipo == TipoMovimiento::TERRESTRE) {
        return motor.calcularTerrestre(this, tablero);
    }
    else if (miTipo == TipoMovimiento::VOLADOR) {
        return motor.calcularVolador(this, tablero);
    }
    else if (miTipo == TipoMovimiento::TELETRANSPORTE) {
        return motor.calcularTeletransporte(this, tablero);
    }

    return std::vector<Vector2D>();
}

void Pieza::actualizaAnimacionAtaque(float dt) {
    if (atacando) {
        anguloAtaque += 500.0f * dt; // La velocidad a la que sube el arma
        if (anguloAtaque > 60.0f) {
            atacando = false;
        }
    }
    else if (anguloAtaque > 0.0f) {
        anguloAtaque -= 150.0f * dt; // La velocidad a la que baja
        if (anguloAtaque < 0.0f) anguloAtaque = 0.0f;
    }
}

// vida y el daño

/*void Pieza::curar(int cantidad) {
    if (!viva || cantidad <= 0) return;

    vida += cantidad;
    if (vida > vidaMaxima) {
        vida = vidaMaxima;
    }
}*/


void Pieza::establecerViva(bool _viva) {
    viva = _viva;
}

/*float Pieza::obtenerBonusCombate(Bando bandoCasilla) {
    if (bandoCasilla == bando) return 1.25f;
    return 0.85f;
}*/

// imprimir
void Pieza::imprimir() {
    std::cout << "Pieza: " << nombre
        << " | Bando: " << (bando == Bando::LUZ ? "LUZ" : "OSCURIDAD")
        << " | Vida: " << obtenerVida()
        << " | Pos: (" << posicion.x << "," << posicion.y << ")";

    if (!viva) std::cout << " [ELIMINADA]";
    if (encarcelada) std::cout << " [EN PRISION]";
    std::cout << std::endl;
}