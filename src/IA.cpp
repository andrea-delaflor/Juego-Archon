#include "IA.h"
#include "Mundo.h"
#include "Batalla.h"
#include "Pieza.h"
#include "Movimiento.h"
#include "Interaccion.h"
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

void IA::ejecutarAccionBatalla(Batalla* batalla, Pieza* ia, Pieza* humano, float dt) {
    if (!ia || !humano || !batalla) return;

    // 1. Obtener las referencias a las posiciones reales de la batalla
    Vector2D& pIA = batalla->obtenerPos2(); // Referencia a pos2
    Vector2D& pH = batalla->obtenerPos1();  // Referencia a pos1

    Vector2D dir = pH - pIA;
    double dist = dir.modulo();

    // 2. Lógica de acercamiento según el arma
    bool necesitaAcercarse = (ia->obtenerArma() == TipoArma::ESCUDO ||
        ia->obtenerArma() == TipoArma::CUERPO_A_CUERPO);

    // Si tiene escudo, busca distancia casi 0 para el daño por contacto
    double distObjetivo = necesitaAcercarse ? 0.6 : (ia->obtenerAlcance() * 0.7);

    if (dist > distObjetivo) {
        Vector2D dN = dir.unitario();
        double vel = ia->obtenerVelocidad();
        if (vel <= 0) vel = 20.0; // Seguro por si la pieza tiene velocidad 0

        // 3. ACTUALIZACIÓN: Esto moverá la pieza en la pantalla
        pIA = pIA + dN * (vel * dt);
    }

    // 4. Lógica de ataque
    if (dist <= ia->obtenerAlcance()) {
        if (necesitaAcercarse) {
            if (ia->obtenerArma() == TipoArma::ESCUDO) {
                ia->activarEscudo(); // El daño por contacto lo hará Batalla::mueve()[cite: 1]
            }
            ia->iniciarAnimacion();
        }
        else {
            // Lógica de disparo para arqueros/magos
            static float cd = 0;
            if (cd <= 0) {
                batalla->generarDisparo(false);
                cd = 1.5f;
            }
            cd -= dt;
        }
    }
}