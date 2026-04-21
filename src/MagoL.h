#pragma once
#include "Pieza.h"
#include "glut.h"
#include "ETSIDI.h"
#include "Hechizo.h"
#include "ListadeHechizos.h"
#include <vector>


namespace ETSIDI { class Sprite; }

class MagoL : public Pieza {
private:
    std::vector<Hechizo*> libroHechizos;
    ETSIDI::Sprite* sprite;

public:
    MagoL(Vector2D pos) :
        Pieza("Delegacion", Bando::LUZ,
            60,     // vida 
            2.5f,   // velocidad
            40,     // poder de ataque (mucha magia)
            1.2f,   // velocidad de ataque
            100.0f, // alcance (ataque a distancia)
            9,      // rango de movimiento 
            TipoArma::ACTAS,
            pos)
    
        {
        salud = new Vida(60, 20.0f);
            // CARGAR EL SPRITE 
            sprite = new ETSIDI::Sprite("imagenes/alumnos/magosinfondo1.png", 0, 0, 0.9f, 0.9f);
        }

    bool esLider() override { return true; }

    float getRadio() override { return 6.0f; }

    //El destructor    
    virtual ~MagoL() {
        if (sprite) delete sprite;
        for (auto h : libroHechizos) {
            delete h;
        }
        libroHechizos.clear();
    }

    
    TipoMovimiento obtenerTipoMovimiento() override { return TipoMovimiento::TELETRANSPORTE; }

    std::string obtenerNombreSprite() override { return "luz_mago"; }

    void dibuja() override;

    void inicializarHechizos() {
        // Evitar duplicados si se llama dos veces
        if (!libroHechizos.empty()) return;

        libroHechizos.push_back(new HechizoTeleport());
        libroHechizos.push_back(new HechizoHeal());
        libroHechizos.push_back(new HechizoShiftTime());
        libroHechizos.push_back(new HechizoExchange());
        libroHechizos.push_back(new HechizoImprison());
        // HECHIZOS PENDIENTES: Añadir aquí el de Summon y Revive 
    }
    
    std::vector<Hechizo*>& getHechizos() { return libroHechizos; }

    void dibujaEnBatalla() override;
    
};