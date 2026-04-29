#pragma once
#include "ETSIDI.h"
#include <vector>

class Historia {
private:
    std::vector<ETSIDI::Sprite*> diapositivas;
    int indiceActual;

public:
    Historia();
    ~Historia();
    void inicializa();
    void dibuja();
    void siguiente();
    bool esFin();
};
