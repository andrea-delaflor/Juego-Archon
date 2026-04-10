#pragma once
#include "Vector2D.h"
#include "Pieza.h"
#include "Tablero.h"
#include <algorithm>   // std::max, std::min
#include <iostream>

// Constructor
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
    // Identidad
    nombre = _nombre;
    bando = _bando;

    // Vida
    vidaMaxima = _vidaMaxima;
    vida = _vidaMaxima;
    viva = true;

    // Estadísticas
    velocidad = _velocidad;
    poderAtaque = _poderAtaque;
    velocidadAtaque = _velocidadAtaque;
    alcance = _alcance;
    rangoMovimiento = _rangoMovimiento;
    arma = _arma;

    // Posición y estado
    posicion = _posInicial;
    encarcelada = false;
}

void Pieza::recibirDanio(int cantidad) {
    if (!viva) return;

    vida -= cantidad;
    if (vida <= 0) {
        vida = 0;
        viva = false;
    }
}

void Pieza::curar(int cantidad) {
    if (!viva || cantidad <= 0) return;

    vida += cantidad;
    if (vida > vidaMaxima) {
        vida = vidaMaxima;
    }
}

void Pieza::restaurarVidaCompleta() {
    vida = vidaMaxima;
    viva = true;
}

void Pieza::establecerViva(bool _viva) {
    viva = _viva;
    // Si la revivimos y no tiene vida, le damos el mínimo
    if (viva && vida <= 0) vida = 1;
}


float Pieza::obtenerBonusCombate(Bando bandoCasilla) {
    // Si la casilla es de mi mismo bando, soy más fuerte (125%)
    if (bandoCasilla == bando) {
        return 1.25f;
    }
    // Si es del bando contrario, me debilito (85%)
    return 0.85f;
}

//Mostrar Informacion

void Pieza::imprimir() {
    std::cout << "Pieza: " << nombre
		<< " | Bando: " << (bando == Bando::LUZ ? "LUZ" : "OSCURIDAD") //si es true imprime LUZ, si es false imprime OSCURIDAD
        << " | Vida: " << vida << "/" << vidaMaxima
        << " | Pos: (" << posicion.x << "," << posicion.y << ")";

    if (!viva) std::cout << " [ELIMINADA]";
    if (encarcelada) std::cout << " [EN PRISIÓN]";

    std::cout << std::endl;
}