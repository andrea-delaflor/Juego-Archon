#include "Obstaculo.h"
#include "glut.h"

Obstaculo::Obstaculo(Vector2D pos, Vector2D vel, TipoObstaculo t) {
    posicion = pos;
    velocidad = vel;
    tipo = t;
    radio = 0.6f; // Tamaño genérico del obstáculo
}

void Obstaculo::dibuja() {
    glPushMatrix();
    glTranslatef(posicion.x, posicion.y, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    // Cambiamos el color dependiendo de lo que hace el objeto
    switch (tipo) {
    case TipoObstaculo::DANO:
        glColor3f(1.0f, 0.0f, 0.0f); // Rojo (Malo, quita vida)
        break;
    case TipoObstaculo::VELOCIDAD:
        glColor3f(0.0f, 1.0f, 0.0f); // Verde (Bueno, bonus de velocidad)
        break;
    case TipoObstaculo::CONGELACION:
        glColor3f(0.0f, 0.5f, 1.0f); // Azul claro (Te congela)
        break;
    }

    // Dibujamos una esfera 3D sencilla para representar el objeto
    glutSolidSphere(radio, 15, 15);

    glPopMatrix();
    glColor3f(1.0f, 1.0f, 1.0f); // Restaurar el color a blanco para no pintar otras cosas de rojo
}

void Obstaculo::mueve(float dt) {
    // Físicas básicas: nueva posición = posición antigua + (velocidad * tiempo)
    posicion = posicion + velocidad * dt;
}


bool Obstaculo::colisionaCon(Vector2D posJugador, float radioJugador) const {
    Vector2D dist = posicion - posJugador;
    // Ahora usamos el radio que le pasemos específicamente
    return dist.modulo() < (radio + radioJugador);
}