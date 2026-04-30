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
    // Solo ejecutar si la pieza 'ia' es realmente del bando OSCURIDAD
    if (ia->obtenerBando() == Bando::LUZ) return;
    
    if (!ia || !humano) return;

    Vector2D pIA = ia->obtenerPosicion();
    Vector2D pHumano = humano->obtenerPosicion();
    float distanciaX = (float)(pHumano.x - pIA.x);
    float absDist = std::abs(distanciaX);

    // 1. LÓGICA DE MOVIMIENTO Y ATAQUE
    float velIA = ia->obtenerVelocidad(); 

        if (ia->obtenerArma() == TipoArma::CUERPO_A_CUERPO) {
            if (absDist > 1.8f) {
                // Movimiento: actualizamos la posición lógica
                float nuevaX = pIA.x + (distanciaX > 0 ? velIA : -velIA) * dt;
                ia->establecerPosicion(Vector2D(nuevaX, pIA.y));
            }
            else {
                ia->iniciarAnimacion();
                    // Uso correcto de la clase estática Interaccion[cite: 21]
                    if (Interaccion::colisionCuerpoACuerpo(pIA, pHumano, ia->obtenerAlcance())) {
                        humano->getVida().damage((int)(ia->obtenerPoderAtaque() * dt)); 
                    }
            }
        }
        else {
            // Lógica para arqueros o magos
            if (absDist < 5.0f) { // Demasiado cerca, retroceder
                float nuevaX = pIA.x + (distanciaX > 0 ? -velIA : velIA) * dt;
                ia->establecerPosicion(Vector2D(nuevaX, pIA.y)); 
            }
            else if (absDist <= ia->obtenerAlcance()) {
                ia->iniciarAnimacion(); // O disparar proyectil si tienes la función
            }
        }

    // 2. GESTIÓN DE ESCUDO (FenixL o similares)
    ia->actualizarEscudo(dt);
        if (ia->obtenerArma() == TipoArma::ESCUDO && ia->obtenerVida() < 30) {
            ia->activarEscudo(); 
        }

    // 3. ACTUALIZACIÓN DE ANIMACIÓN VISUAL
    ia->actualizar(0.005f); 
}