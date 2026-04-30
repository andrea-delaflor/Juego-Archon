#pragma once
#include <iostream>
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
#include "IA.h"




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

	void actualizarVidaPiezas(); // Nueva función para gestionar la curación al final de cada turno

    //Comprobar victoria
    void comprobarVictoria();
    int obtenerGanador() const { return ganadorPartida; }

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
    void setModoMagia(bool v) {
        modoMagiaActivo = v;

        if (v == true) {
            // Al entrar en modo magia, quitamos al Mago de la selección
            // Esto permite que el primer clic del Teleport sea para elegir al aliado
            seleccionada = nullptr;
        }
        else {
            // Al salir (v == false), limpiamos el índice del menú
            setIndiceSeleccionado(-1);
        }
    }

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

    void invertirCicloTiempo() {
        // Para invertir el flujo en una onda senoidal, sumamos PI (180 grados)
        // Esto hace que si la luz estaba subiendo, ahora esté exactamente en el punto opuesto bajando.
            angulo += 3.14159f;

        // Actualizamos el valor inmediatamente para que se vea el salto
        valorLuz = (sin(angulo) + 1.0f) / 2.0f;

        std::cout << "Shift Time: Angulo desplazado. Nueva luz: " << valorLuz << std::endl;
    }
    void clickIA(int casillaX, int casillaY);

  
    
private:
    Tablero tablero;
    float valorLuz = 0.5f;
    float angulo = 0.0f;
	

    bool modoMagiaActivo = false; // Indica si estamos en proceso de lanzar un hechizo

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

    //Ganra juego ---> 1=luz, 2=oscuridad, 0= empate
    int ganadorPartida = 0;
    // Función interna segura para borrar una pieza y que desaparezca gráficamente
    void eliminarPieza(Pieza* p);



    //organización de codigo en funcion dibuja();
    void dibujarCajasMovimiento();
    void dibujarCarceles();
    void dibujarInterfazSuperior();
    void dibujarGrimorio();
    
};