#pragma once
#include "Proyectil.h"

class PelotaFutbol : public Proyectil {
    ETSIDI::Sprite sprite;
public:
    PelotaFutbol(Vector2D pos, Vector2D vel, int d, bool j1)
        : Proyectil(pos, vel, d, j1), 
        sprite("imagenes/armas/pelotafutbol.png", 0, 0, 1.0f, 1.0f) {
        sprite.setCenter(0.5f, 0.5f);
        sprite.setSize(1.8f, 1.8f);
    } 

    void dibuja() override {
        sprite.setPos(posicion.x, posicion.y);
        sprite.draw();
    }
};

class BolaFuego : public Proyectil {
    ETSIDI::Sprite sprite;
public:
    BolaFuego(Vector2D pos, Vector2D vel, int d, bool j1)
        : Proyectil(pos, vel, d, j1), 
        sprite("imagenes/armas/fuego.png", 0, 0, 1.5f, 1.5f) {
        sprite.setCenter(0.75f, 0.75f);
        sprite.setSize(1.8f, 1.8f);
    } 

    void dibuja() override {
        sprite.setPos(posicion.x, posicion.y);
        sprite.draw();
    }
};

class RayoLaser : public Proyectil {
    ETSIDI::Sprite sprite;
    Vector2D origen;  //para el punto de disparo, como hicimos en el pang
public:
    RayoLaser(Vector2D pos, Vector2D vel, int d, bool j1)
        : Proyectil(pos, vel, d, j1), sprite("imagenes/armas/rayolaser.png", 0, 0, 1.5f, 1.5f) {
        origen = pos; // guardamos el sitio desde donde se disparó
        sprite.setCenter(0.75f, 0.75f);
        sprite.setSize(1.8f, 1.8f);
    }

    void dibuja() override {
        // Se dibuja la estela 
        glDisable(GL_TEXTURE_2D);     //desactiva texturas
        glDisable(GL_LIGHTING);      //desactiva luces y sombras
        glLineWidth(8.0f);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 1.0f);    
        glVertex3f(origen.x, origen.y, 0);      // Punto fijo inicial
        glVertex3f(posicion.x, posicion.y, 0);  // punto actual de la imagen la imagen ahora
        glEnd();
        glEnable(GL_TEXTURE_2D); // Se habilitan texturas para el sprite
        glEnable(GL_LIGHTING);
        sprite.setPos(posicion.x, posicion.y);
        sprite.draw();
    }
};

class RayoNumerico : public Proyectil {
    ETSIDI::Sprite sprite;
    Vector2D origen;
public:
    RayoNumerico(Vector2D pos, Vector2D vel, int d, bool j1)
        : Proyectil(pos, vel, d, j1), 
        sprite("imagenes/armas/rayonumerico.png", 0, 0, 1.5f, 1.5f) {
        sprite.setCenter(0.75f, 0.75f);
        sprite.setSize(1.8f, 1.8f);
        origen = pos;
    }

    void dibuja() override {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glLineWidth(8.0f);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f); // Color Verde para el Basilisco
        glVertex3f(origen.x, origen.y, 0);
        glVertex3f(posicion.x, posicion.y, 0);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        
        sprite.setPos(posicion.x, posicion.y);
        sprite.draw();
    }
};

class Acta : public Proyectil {
    ETSIDI::Sprite sprite;
public:
    Acta(Vector2D pos, Vector2D vel, int d, bool j1)
        : Proyectil(pos, vel, d, j1), 
        sprite("imagenes/armas/guiasaprendizaje.png", 0, 0, 1.5f, 1.5f) {
        sprite.setCenter(0.75f, 0.75f);
        sprite.setSize(1.8f, 1.8f);
    }

    void dibuja() override {
        sprite.setPos(posicion.x, posicion.y);
        sprite.draw();
    }
};



