#include "Coordinador.h"

//MAQUINA DE ESTADOS: el coordinador se encarga de gestionar qué se muestra en cada momento
void Coordinador::dibuja() {
    if (estado == INICIO) {
        // Dibujar portada con ETSIDI::Sprite
        
    }
    else if (estado == MENU) {
        // Dibujar opciones: 1. Un jugador vs Profesores, 2. Dos jugadores
    }
    else if (estado == JUEGO) {
        mundo.dibuja(); // Aquí el coordinador le da el mando al mundo
    }
    else if (estado == PAUSA) {
        mundo.dibuja(); // Dibujamos el mundo de fondo
        // Dibujar encima el menú de pausa
    }
}

void Coordinador::tecla(unsigned char key) {
    if (estado == INICIO && key == 13) { // 13 es Enter
        estado = MENU;
    }
    else if (estado == MENU) {
        if (key == '1') { /* configurar mundo para 1 jugador */; estado = JUEGO; }
        if (key == '2') { /* configurar mundo para 2 jugadores */; estado = JUEGO; }
    }
    else if (estado == JUEGO) {
        if (key == 'p' || key == 'P') estado = PAUSA;
		// else mundo.tecla(key); //hay que añadir esta función en el mundo para que pueda gestionar teclas durante el juego, como por ejemplo para activar habilidades especiales
    }

void Coordinador::mueve() {
    switch (estado) {
    case JUEGO:
        mundo.mueve(); // Hace que la luz parpadee y las piezas caminen
        break;

    case BATALLA:
        // escenarioBatalla.mueve();
        break;

        // En INICIO, MENU y PAUSA el tiempo del juego se congela
        // porque no llamamos a mundo.mueve()
    }
}