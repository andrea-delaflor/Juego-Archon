#include"DragonO.h"

DragonO::DragonO(Vector2D posInicial)
    : Pieza("Dragon", Bando::OSCURIDAD, 150, 4.0f, 30, 0.8f, 2.0f, 4, TipoArma::BOLA_DE_FUEGO, posInicial)
{
    sprite = new ETSIDI::Sprite(obtenerNombreSprite().c_str(), 0, 0, 0.8f, 0.8f);
}

DragonO::~DragonO() {
    delete sprite;
}

std::string DragonO::obtenerNombreSprite() {
    return "imagenes/profes/mecanica.png";
}

void DragonO::dibuja() {
    float x_pantalla = posicion.x - 4.0f;
    float y_pantalla = 4.0f - posicion.y;

    sprite->setPos(x_pantalla, y_pantalla);
    sprite->draw();
}

TipoMovimiento DragonO::obtenerTipoMovimiento() {
    return TipoMovimiento::VOLADOR;
}