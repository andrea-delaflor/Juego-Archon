#include"DjiniL.h"

DjiniL::DjiniL(Vector2D posInicial)
    : Pieza("Djinni", Bando::LUZ, 100, 5.0f, 20, 1.0f, 3.0f, 4, TipoArma::MAGIA, posInicial)
{
    sprite = new ETSIDI::Sprite(obtenerNombreSprite().c_str(), 0, 0, 0.8f, 0.8f);
}

DjiniL::~DjiniL() {
    delete sprite;
}

std::string DjiniL::obtenerNombreSprite() {
    return "imagenes/alumnos/crea2.png";
}

void DjiniL::dibuja() {
    float x_pantalla = posicion.x - 4.0f;
    float y_pantalla = 4.0f - posicion.y;

    sprite->setPos(x_pantalla, y_pantalla);
    sprite->draw();
}

TipoMovimiento DjiniL::obtenerTipoMovimiento() {
    return TipoMovimiento::VOLADOR;
}