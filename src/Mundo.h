#pragma once
#include "Tablero.h"

class Mundo {
public:
	void inicializa();
	void dibuja();
	void mueve();
	void tecla(unsigned char t);

private:
	Tablero tablero;
};