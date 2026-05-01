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
    // PUNTO DE CONTROL 1: ¿Llegan los punteros?
    if (!ia || !humano || !batalla) return;

    // PUNTO DE CONTROL 2: ¿El bando es correcto?
    // Si tus piezas de IA son del bando LUZ por error, aquí se detendría.
    if (ia->obtenerBando() == Bando::LUZ) return;

    // 1. Obtener datos básicos
    Vector2D posIA = ia->obtenerPosicion();
    Vector2D posH = humano->obtenerPosicion();
    Vector2D dir = posH - posIA;
    double dist = dir.modulo();

    // 2. FORZAR MOVIMIENTO (Prueba diagnóstica)
    // Vamos a ignorar el "alcance" por un momento para ver si se mueven.
    if (dist > 0.1) {
        Vector2D dN = dir.unitario();

        // REVISIÓN CRÍTICA: La velocidad.
        // Si ia->obtenerVelocidad() devuelve 0, 'paso' será (0,0).
        double vel = ia->obtenerVelocidad();
        if (vel <= 0) vel = 20.0; // Forzamos una velocidad si la pieza tiene 0

        Vector2D paso = dN * (vel * dt);

        // 3. ACTUALIZACIÓN DE POSICIÓN
        Vector2D nuevaPos = posIA + paso;

        // ¿Cómo se llama tu función exactamente? 
        // Asegúrate de que sea la que realmente cambia las variables x e y del personaje.
        ia->establecerPosicion(nuevaPos);
    }

    // 4. DISPARO
    if (dist <= ia->obtenerAlcance()) {
        static float cooldown = 0;
        if (cooldown <= 0) {
            batalla->generarDisparo(false); // false para J2 (IA)
            cooldown = 1.5f;
        }
        cooldown -= dt;
    }
}