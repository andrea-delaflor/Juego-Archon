#pragma once
#include "Pieza.h"
#include "ETSIDI.h"

class DragonO : public Pieza {
public:
    DragonO(Vector2D posInicial);
    ~DragonO() override;

    std::string obtenerNombreSprite() override;
    void dibuja() override;
    TipoMovimiento obtenerTipoMovimiento() override;

private:
    ETSIDI::Sprite* sprite;
};
