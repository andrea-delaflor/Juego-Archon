#pragma once
#include "Tablero.h"
#include "Vector2D.h"
#include "ETSIDI.h"
#include "Pieza.h"
#include "Raton.h"
#include "GolemL.h"
#include "MagoL.h"
#include "BrujaO.h"
#include "DragonO.h"
#include"DjiniL.h"
#include"FenixL.h"
#include"BansheeO.h"
#include "ArqueraL.h"
#include "BasiliscoO.h"
#include "TrollO.h"
#include "ListadeHechizos.h"
#include <vector>




class Mundo {
public:
    enum FaseJuego {
        TURNO_LUZ,
        TURNO_OSCURIDAD,
        ANIMANDO_MOVIMIENTO,
        FIN_PARTIDA
    };

    FaseJuego faseActual = TURNO_LUZ;

    Mundo();
    ~Mundo();

    void inicializa(int estado);
    void mueve();
    void dibuja(int estado);
    void clickRaton(int button, int state, int x, int y);
    void teclahechizos(unsigned char key);

    std::vector<Pieza*>& getCementerioActual() {
        if (faseActual == TURNO_LUZ) return cementerioLuz;
        else return cementerioOscuridad;
    }

    void finalizaCombate(Pieza* ganador, Pieza* perdedor, bool empate);

    // Variables de estado de selección
    Raton raton;
    Pieza* seleccionada = nullptr;

    // Estado de combate
    bool hayCombate = false;
    Pieza* atacante = nullptr;
    Pieza* defensor = nullptr;
    Pieza* posibleDefensor = nullptr;
    //Aquí guardaremos el número(0, 1 o 2) que nos diga el tablero para pasárselo a la arena
    int tipoArenaCombate = 0;

    void resetCombate() { hayCombate = false; atacante = defensor = nullptr; }
	

    //SISTEMA DE MAGIA
    bool modoMagiaActivo = false; // Indica si estamos en proceso de lanzar un hechizo
    Hechizo* hechizoSeleccionado = nullptr; // Guarda qué hechizo se va a aplicar

    // Getters y Setters para Hechizos y Revive
    bool getEsperandoEleccionRevive() { return esperandoEleccionRevive; }
    void setEsperandoEleccionRevive(bool valor) { esperandoEleccionRevive = valor; }
    void setIndiceSeleccionado(int i) { indiceSeleccionado = i; }
    int getIndiceSeleccionado() { return indiceSeleccionado; }

    // Getters de Tablero y Valor de Luz
    Tablero& getTablero() { return tablero; }
    float getValorLuz() { return valorLuz; }
    void setValorLuz(float nuevoValor) { valorLuz = nuevoValor; }
    // Getters de Listas de Piezas y Cementerios
    std::vector<Pieza*>& getPiezasLuz() { return piezasLuz; }
    std::vector<Pieza*>& getPiezasOscuridad() { return piezasOscuridad; }
    std::vector<Pieza*>& getCementerioLuz() { return cementerioLuz; }
    std::vector<Pieza*>& getCementerioOscuridad() { return cementerioOscuridad; }

    // Función auxiliar para el bando actual
    Bando bandoActual() { return (faseActual == TURNO_LUZ) ? Bando::LUZ : Bando::OSCURIDAD; }

	    
private:
    Tablero tablero;
    float valorLuz = 0.5f;
    float angulo = 0.0f;

    // Listas de piezas vivas
    std::vector<Pieza*> piezasLuz;
    std::vector<Pieza*> piezasOscuridad;

    // Libros de hechizos (Se llenan en inicializa)
    std::vector<Hechizo*> libroLuz;
    std::vector<Hechizo*> libroOscuridad;

    // Gestión de bajas y resurrección
    std::vector<Pieza*> cementerioLuz;
    std::vector<Pieza*> cementerioOscuridad;
    bool esperandoEleccionRevive = false;
    int indiceSeleccionado = -1;

    // Función interna segura para borrar una pieza y que desaparezca gráficamente
    void eliminarPieza(Pieza* p);
};