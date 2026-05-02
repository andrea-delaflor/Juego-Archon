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
	if (!ia || !humano || !batalla) return; // Seguridad: si alguno es nullptr, no hacemos nada

    // Obtener referencias y datos básicos
    Vector2D& pIA = batalla->obtenerPos2();
    Vector2D& pH = batalla->obtenerPos1();
    Vector2D dir = pH - pIA;
    double dist = dir.modulo();
    Vector2D dN = dir.unitario();

	// LÓGICA DE TIEMPO (Ciclo de 4 segundos) para crear patron de movimiento y pausa
    float tiempoTotal = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; 
    // El ciclo dura 4 segundos:
    float ciclo = fmod(tiempoTotal, 4.0f);
    // 2.5 segundos se mueve, 1.5 segundos se para
    bool debeMoverse = (ciclo < 2.5f);

    // VELOCIDAD Y TIPO DE PIEZA
    double velIA = ia->obtenerVelocidad();
    if (velIA <= 0) velIA = 25.0;
    bool esCerca = (ia->obtenerArma() == TipoArma::ESCUDO || ia->obtenerArma() == TipoArma::CUERPO_A_CUERPO);

    // EJECUCIÓN DEL MOVIMIENTO
    if (debeMoverse) {
        if (esCerca) {
            // Persecución si es de cuerpo a cuerpo
            if (dist > 0.5) pIA = pIA + dN * (velIA * dt);

        }
        else {
            // Movimiento táctico si es de proyectil
            double alcance = ia->obtenerAlcance();
            if (dist < (alcance * 0.4)) pIA = pIA - dN * (velIA * dt); // Retrocede
            else if (dist > (alcance * 0.8)) pIA = pIA + dN * (velIA * dt); // Avanza
        }
    }
    

    //  LÍMITES DE PANTALLA 
    if (pIA.x > 8.0f)  pIA.x = 8.0f;
    if (pIA.x < -8.0f) pIA.x = -8.0f;
    if (pIA.y > 8.0f)  pIA.y = 8.0f;
    if (pIA.y < -8.0f) pIA.y = -8.0f;

    // ATAQUE (La IA puede seguir disparando aunque esté parada)
    if (dist <= ia->obtenerAlcance()) {
        if (esCerca) {
            if (ia->obtenerArma() == TipoArma::ESCUDO) {
                ia->activarEscudo();
                // El daño por escudo lo hace automáticamente Batalla::mueve
            }
            else {
                // LÓGICA DE DAÑO PARA GOLPE FÍSICO 
                static float cdGolpe = 0; // Para que no quite vida en cada frame
                if (cdGolpe <= 0) {
                    ia->iniciarAnimacion(); 
                        // Verificamos si el golpe alcanza al humano
                        if (Interaccion::colisionCuerpoACuerpo(pIA, pH, ia->obtenerAlcance())) {
                           // Restamos vida al objeto Vida del humano
                            humano->getVida().damage(ia->obtenerPoderAtaque()); 
                            // Sincronizamos la vida de la batalla
                            batalla->actualizarVidaVisual();
                        }
                    cdGolpe = 1.0f; // Tiempo entre golpes
                }
                cdGolpe -= dt;
            }
        }
        else {
			static float cd = 0; // Cooldown para evitar disparos continuos
            if (cd <= 0) {
                batalla->generarDisparo(false); 
                    cd = 1.5f;
            }
            cd -= dt;
        }
    }
}