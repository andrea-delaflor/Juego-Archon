#include "Historia.h"

Historia::Historia() {
    indiceActual = 0;
}

Historia::~Historia() {
    for (auto d : diapositivas) delete d;
    diapositivas.clear();
}

void Historia::inicializa() {
    // Limpiamos por si acaso se llama varias veces
    for (auto d : diapositivas) delete d;
    diapositivas.clear();
    indiceActual = 0;

    // CARGA DE LA IMAGEN: Asegúrate de que el archivo existe en esa ruta
    // Usamos 20x20 para que coincida con un gluOrtho2D de -10 a 10
    diapositivas.push_back(new ETSIDI::Sprite("imagenes/pantallahistoria.png", -1, 0, 26, 20));
}

void Historia::dibuja() {
    if (indiceActual < diapositivas.size()) {
        glDisable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_TEXTURE_2D);
        diapositivas[indiceActual]->draw();
        glDisable(GL_TEXTURE_2D);
    }
}

void Historia::siguiente() {
    indiceActual++;
}

bool Historia::esFin() {
    return indiceActual >= diapositivas.size();
}