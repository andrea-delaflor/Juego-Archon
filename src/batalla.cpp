#include "batalla.h"
#include "glut.h" 
#include "ProyectilHijos.h"
#include "Interaccion.h"
#include <iostream>
#include <cstdlib> // Necesario para rand()


Batalla::Batalla() : fondoArena("imagenes/batallacancha.png", 0, 0, 20, 20) {
    terminado = false;
    empate = false;
    ganador = nullptr;
    perdedor = nullptr;
    l1 = nullptr;
    l2 = nullptr;
    hp1 = 0;
    hp2 = 0;


    temporizadorObstaculos = 0.0f;
    arenaConObstaculos = false;

    //  Rellenamos nuestra lista de arenas con imágenes de prueba
    // Tienen que estar en la carpeta bin/imagenes
    rutasArenas.push_back("imagenes/batallacancha.png");   // Índice 0: Casilla Blanca
    rutasArenas.push_back("imagenes/batalladespacho.png"); // Índice 1: Casilla Negra
    rutasArenas.push_back("imagenes/batallaaula.png");     // Índice 2: Casilla Variable
    rutasArenas.push_back("imagenes/batallacafeteria.png");    // Índice 3: Punto de Poder

    // Empezamos apuntando a la última para que al sumarle 1 en el primer combate, empiece por la Arena 0.
    indiceArenaActual = -1;
}

// INICIALIZA: Se ejecuta CADA VEZ que hay un combate nuevo
void Batalla::inicializa(Pieza* atacante, Pieza* defensor, int tipoArena) {
    l1 = atacante;
    l2 = defensor;

    terminado = false;
    empate = false;
    ganador = nullptr;
    perdedor = nullptr;

    // Sincronizamos los HP con la vida real de la pieza (clase Vida)
    hp1 = l1->obtenerVida();
    hp2 = l2->obtenerVida();

    pos1 = Vector2D(-4, 0);
    pos2 = Vector2D(4, 0);

    indiceArenaActual = tipoArena;
    // ALTERNAR ARENAS
    // Sumamos 1 al índice. El símbolo % (módulo) hace que si llegamos al final de la lista,
    // vuelva automáticamente a 0 en lugar de dar error. ¡Así es un bucle infinito!
   // indiceArenaActual = (indiceArenaActual + 1) % rutasArenas.size();

    // Cambiamos el dibujo de fondoArena usando la ruta que toca de la lista
    fondoArena = ETSIDI::Sprite(rutasArenas[indiceArenaActual].c_str(), 0, 0, 20, 20);

    std::cout << "Cargando Arena número: " << indiceArenaActual << std::endl;

    //para no acumular disparos anteiores
    for (auto p : proyectiles) delete p;
    proyectiles.clear();
    // Limpiamos la lista de obstáculos de combates anteriores
    for (auto o : obstaculos) delete o;
    obstaculos.clear();
    // Lanzamos un "dado" PROBABILIDAD DEL 80%
    // rand() % 100 genera un número del 0 al 99. 
    // Si es menor que 80, hay obstáculos (80% de las veces).
    int tiradaDado = rand() % 100;
    arenaConObstaculos = (tiradaDado < 80);

    temporizadorObstaculos = 0.0f;

    if (arenaConObstaculos) {
        std::cout << "¡CUIDADO! Esta arena tiene obstaculos cayendo del cielo." << std::endl;
    }
    else {
        std::cout << "Arena normal, sin obstaculos." << std::endl;
    }
}

// DIBUJA: Pinta todo en la pantalla
void Batalla::dibuja() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Dibuja el fondo de la arena que haya tocado esta vez
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    fondoArena.draw();
    glDisable(GL_TEXTURE_2D);

  
    //  JUGADOR 1
    if (l1 != nullptr) {
        glPushMatrix();
        glTranslatef(pos1.x, pos1.y, 0);
        glScalef(2.0f, 2.0f, 1.0f); // ajusta escalas de lo que dibujamos

        // dibuja la pieza + el arma
        l1->dibujaEnBatalla();
        glPopMatrix();
        // Dibujamos corazones vida
        l1->dibujaCorazones(-9.0f, 8.5f, 1.5f);
    }

    //JUGADOR 2
    if (l2 != nullptr) {
        glPushMatrix();
        glTranslatef(pos2.x, pos2.y, 0);
        glScalef(2.0f, 2.0f, 1.0f);

        l2->dibujaEnBatalla();
        glPopMatrix();

        l2->dibujaCorazones(8.0f, 8.5f, 1.5f);
    }
    
    for (auto p : proyectiles) {
        p->dibuja();
    }

    //  Dibujar los obstáculos cayendo
    for (auto o : obstaculos) {
        o->dibuja();
    }
}

// MUEVE: Aquí irá la física de la pelea en el futuro
void Batalla::mueve() {
    float dt = 0.05f; // Ajusta según la velocidad de tu timer

    //LÓGICA DE PROYECTILES
    //GESTIÓN DE PROYECTILES
    for (auto it = proyectiles.begin(); it != proyectiles.end(); ) {
        (*it)->mueve(dt);
        bool impactado = false;

        // Colisión simple por distancia
        if ((*it)->esDeJugador1()) {
            if (Interaccion::colision(**it, pos2)) { // Usamos la clase Interaccion
                l2->getVida().damage((*it)->getDanio());
                hp2 = l2->getVida().getActual(); // Actualizamos HP local para el chequeo de fin
                impactado = true;
            }
        }
        else {
            if (Interaccion::colision(**it, pos1)) {
                l1->getVida().damage((*it)->getDanio());
                hp1 = l1->getVida().getActual();
                impactado = true;
            }
        }

        // Borrar proyectil si impacta o sale de rango
        if (impactado || (*it)->getPos().x > 12 || (*it)->getPos().x < -12) {
            delete* it;
            it = proyectiles.erase(it);
        }
        else {
            ++it;
        }
    }
    // LÓGICA DE LOS OBSTÁCULOS
    // GENERACIÓN DE OBSTÁCULOS
    // 1. Si la arena es de las "peligrosas", generamos nuevos objetos cada cierto tiempo
    if (arenaConObstaculos) {
        temporizadorObstaculos += dt;

        // Cada 2 segundos (aprox) cae un nuevo objeto
        if (temporizadorObstaculos > 2.0f) {
            temporizadorObstaculos = 0.0f;

            // Posición aleatoria arriba de la pantalla (x entre -8 y 8, y = 10)
            float xAleatorio = -8.0f + (rand() % 1600) / 100.0f;
            Vector2D posObj(xAleatorio, 10.0f); 

            // Caen hacia abajo a velocidad 3
            Vector2D velObj(0.0f, -3.0f);// velocidad de caida 

            // Elegimos un tipo al azar (0, 1 o 2)
            TipoObstaculo tipoAleatorio = static_cast<TipoObstaculo>(rand() % 3);

            obstaculos.push_back(new Obstaculo(posObj, velObj, tipoAleatorio));
        }
    }

    // 2. COLISIÓN DE OBSTÁCULOS
    for (auto it = obstaculos.begin(); it != obstaculos.end(); ) {
        (*it)->mueve(dt);
        bool borrar = false;

        // Comprobar Jugador 1 (Oscuridad / Atacante)
        if (Interaccion::colision(**it, pos1, l1->getRadio())) {
            if ((*it)->getTipo() == TipoObstaculo::DANO) {
                l1->getVida().damage(10);
                hp1 = l1->getVida().getActual();
            }
            borrar = true;
        }

        // Comprobar Jugador 2 (Luz / Defensor)     
        if (!borrar && Interaccion::colision(**it, pos2, l2->getRadio())) {
            if ((*it)->getTipo() == TipoObstaculo::DANO) {
                l2->getVida().damage(10);
                hp2 = l2->getVida().getActual();
            }
            borrar = true;
        }

        // Si se sale por debajo de la pantalla
        if (!borrar && (*it)->getPos().y < -12.0f) {
            borrar = true;
        }

        if (borrar) {
            delete* it;
            it = obstaculos.erase(it);
        }
        else {
            ++it;
        }
    }
    //CONDICIÓN DE VICTORIA
    // Comprobar fin de batalla
    if (l1->getVida().muerto()) { terminado = true; ganador = l2; perdedor = l1; } 
    else if (l2->getVida().muerto()) { terminado = true; ganador = l1; perdedor = l2; }
}




void Batalla::tecla(unsigned char key) {
    float vel = 0.5f; //Esto es lo q se traslada la pieza por pulsación tecla

    // JUGADOR 1
    if (key == 'w' || key == 'W') pos1.y += vel;
    if (key == 's' || key == 'S') pos1.y -= vel;
    if (key == 'a' || key == 'A') pos1.x -= vel;
    if (key == 'd' || key == 'D') pos1.x += vel;

    // Disparos y Controles
    if (key == ' ') generarDisparo(true);  //pulsando espacio jugador 1 usa su poder
    if (key == 13)  generarDisparo(false); //pulsando enter jugador 2 usa su poder
    if (key == 'e' || key == 'E') { terminado = true; empate = true; }

    // Límites de la pantalla 
    if (pos1.x > 9.5f) pos1.x = 9.5f;
    if (pos1.x < -9.5f) pos1.x = -9.5f;
    if (pos1.y > 9.5f) pos1.y = 9.5f;
    if (pos1.y < -9.5f) pos1.y = -9.5f;

    // Para el Jugador 1
    if ((key == 't' || key == 'T') && l1) {                       //vamos a utilizar las mismas teclas para activar los poderes, asi es mas facil de jugar
        if (l1->obtenerArma() == TipoArma::ESCUDO) {
            l1->activarEscudo();
        }
        else {
            l1->iniciarAnimacion(); // Esto es para el Golem y su llave
        }
    }

    // Para el Jugador 2
    if ((key == 'y' || key == 'Y') && l2) {
        if (l2->obtenerArma() == TipoArma::ESCUDO) {
            l2->activarEscudo();
        }
        else {
            l2->iniciarAnimacion(); // Esto es para el Troll y su código
        }
    }

}

void Batalla::teclaEspecial(int key) {
    float vel = 0.5f;

    // JUGADOR 2
    if (key == GLUT_KEY_UP)    pos2.y += vel;
    if (key == GLUT_KEY_DOWN)  pos2.y -= vel;
    if (key == GLUT_KEY_LEFT)  pos2.x -= vel;
    if (key == GLUT_KEY_RIGHT) pos2.x += vel;

    // Límites de la pantalla 
    if (pos2.x > 9.5f) pos2.x = 9.5f;
    if (pos2.x < -9.5f) pos2.x = -9.5f;
    if (pos2.y > 9.5f) pos2.y = 9.5f;
    if (pos2.y < -9.5f) pos2.y = -9.5f;
}

void Batalla::generarDisparo(bool esJugador1) {
    Pieza* p = esJugador1 ? l1 : l2;
    //Vector2D posDisparo = esJugador1 ? pos1 : pos2;

    Vector2D posPieza = esJugador1 ? pos1 : pos2;
    float offsetX = -2.0f;
    float offsetY = 3.75f;
    Vector2D posDisparo = posPieza + Vector2D(offsetX, offsetY);

    // El J1 dispara a la derecha (positivo), el J2 a la izquierda (negativo)
    Vector2D vel = esJugador1 ? Vector2D(10, 0) : Vector2D(-10, 0);

    switch (p->obtenerArma()) {
    case TipoArma::PELOTAFUTBOL: // Arquera
        proyectiles.push_back(new PelotaFutbol(posDisparo, vel, p->obtenerPoderAtaque(), esJugador1));
        break;

    case TipoArma::BOLA_DE_FUEGO: // Dragón
        proyectiles.push_back(new BolaFuego(posDisparo, vel, p->obtenerPoderAtaque(), esJugador1));
        break;

    case TipoArma::RAYO_LASER: // Djinni (Láser)
        proyectiles.push_back(new RayoLaser(posDisparo, vel, p->obtenerPoderAtaque(), esJugador1));
        break;

    case TipoArma::RAYO_NUMERICO: // Basilisco (Cálculo)
        proyectiles.push_back(new RayoNumerico(posDisparo, vel, p->obtenerPoderAtaque(), esJugador1));
        break;

    case TipoArma::ACTAS: // Mago y Bruja (Actas)
        proyectiles.push_back(new Acta(posDisparo, vel, p->obtenerPoderAtaque(), esJugador1));
        break;

    default:
        break;
    }
}