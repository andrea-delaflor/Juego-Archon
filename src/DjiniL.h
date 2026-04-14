#pragma once
#include "Pieza.h"
#include "ETSIDI.h" 

class DjiniL : public Pieza {
public:
    DjiniL(Vector2D posInicial);
    ~DjiniL() override;

    std::string obtenerNombreSprite() override;
    void dibuja() override;
    TipoMovimiento obtenerTipoMovimiento() override;

private:
    ETSIDI::Sprite* sprite;
};

