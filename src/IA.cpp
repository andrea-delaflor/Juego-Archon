#include "IA.h"
#include "Mundo.h"
#include "Batalla.h"
#include "Pieza.h"
#include "Movimiento.h"
#include <cstdlib>

float IA::congelarAtaque = 0.0f;

std::vector<Vector2D> IA::decidirMovimientoTablero(Mundo* mundo) {
    std::vector<Vector2D> decision;
    auto& piezas = mundo->getPiezasOscuridad();

    if (piezas.empty()) return decision;

    for (int i = 0; i < 15; i++) {
        int idx = rand() % piezas.size();
        Pieza* p = piezas[idx];

        
        if (!p->estaViva() || p->estaEncarcelada()) continue;

        Movimiento motor;
        std::vector<Vector2D> posibles;

        auto tipo = p->obtenerTipoMovimiento();
        if (tipo == TipoMovimiento::TERRESTRE)
            posibles = motor.calcularTerrestre(p, &mundo->getTablero());
        else if (tipo == TipoMovimiento::VOLADOR)
            posibles = motor.calcularVolador(p, &mundo->getTablero());
        else
            posibles = motor.calcularTeletransporte(p, &mundo->getTablero());

        if (!posibles.empty()) {
            int destIdx = rand() % posibles.size();
            decision.push_back(p->obtenerPosicion());    // Origen
            decision.push_back(posibles[destIdx]);       // Destino
            return decision;
        }
    }
    return decision;
}

void IA::ejecutarAccionBatalla(Batalla* batalla, Pieza* iaPieza, Pieza* rivalPieza, float dt) {
    if (!iaPieza || !rivalPieza) return;

    
    Vector2D posIA = iaPieza->obtenerPosicion();
    Vector2D posRival = rivalPieza->obtenerPosicion();
    Vector2D velFinal(0, 0);
    float vMax = iaPieza->obtenerVelocidad();

    // movimiento en eje y
    float margenY = 0.8f;
    if (posIA.y < posRival.y - margenY) velFinal.y = vMax;
    else if (posIA.y > posRival.y + margenY) velFinal.y = -vMax;

    // movimiento en eje x
    float distH = abs(posIA.x - posRival.x);

    
    if (iaPieza->obtenerArma() == TipoArma::CUERPO_A_CUERPO) {
        if (posIA.x < posRival.x - 1.5f) velFinal.x = vMax;
        else if (posIA.x > posRival.x + 1.5f) velFinal.x = -vMax;
    }
    else {
        if (distH > 14.0f) velFinal.x = (posIA.x < posRival.x) ? vMax : -vMax;
        else if (distH < 7.0f) velFinal.x = (posIA.x < posRival.x) ? -vMax : vMax;
    }

    
    Vector2D nuevaPos = posIA + (velFinal * dt);
    iaPieza->establecerPosicion(nuevaPos);

    // para que el ataque vaya con margen de timepo
    if (congelarAtaque > 0) {
        congelarAtaque -= dt;
    }
    else {
        float distTotal = (posIA - posRival).modulo();
        // ataca si esta en rango
        if (distTotal <= iaPieza->obtenerAlcance() + 1.0f) {
            if (iaPieza->obtenerArma() == TipoArma::ESCUDO) {
                iaPieza->activarEscudo();
            }
            else {
                batalla->generarDisparo(false); // false = IA
            }
            congelarAtaque = 1.0f;
        }
    }
}