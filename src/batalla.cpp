#include "batalla.h"
#include "glut.h" 
#include "ProyectilHijos.h"
#include <iostream>
#include <cstdlib> // Necesario para rand()


Batalla::Batalla() : fondoArena("imagenes/batallacancha.png", 0, 0, 20, 20) {
    terminado = false;
    empate = false;
    ganador = nullptr;
    perdedor = nullptr;
    l1 = nullptr;
    l2 = nullptr;
    hp1 = 100;
    hp2 = 100;

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
    hp1 = 100;
    hp2 = 100;

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

    if (l1 != nullptr) {
        glPushMatrix();
        //Calculamos el offset exacto que hace la pieza al dibujarse
        float offsetX_l1 = l1->obtenerPosicion().x - 4.0f;
        float offsetY_l1 = 4.0f - l1->obtenerPosicion().y;
        // Le restamos ese offset a nuestra posición de batalla para que cuadre perfecto
        glTranslatef(pos1.x - offsetX_l1, pos1.y - offsetY_l1, 0);
        //hacemos la pieza un poco mas grande
        glScalef(2.0f, 2.0f, 1.5f);
        l1->dibuja();
        glPopMatrix();
    }

    if (l2 != nullptr) {
        glPushMatrix();
        // Hacemos exactamente lo mismo para el defensor
        float offsetX_l2 = l2->obtenerPosicion().x - 4.0f;
        float offsetY_l2 = 4.0f - l2->obtenerPosicion().y;
        glTranslatef(pos2.x - offsetX_l2, pos2.y - offsetY_l2, 0);
        //hacemos la pieza un poco mas grande
        glScalef(2.0f, 2.0f, 1.5f);
        l2->dibuja();
        glPopMatrix();
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

    for (auto it = proyectiles.begin(); it != proyectiles.end(); ) {
        (*it)->mueve(dt);

        bool impactado = false;
        // Colisión simple por distancia
        if ((*it)->esDeJugador1()) {
            if (((*it)->getPos() - pos2).modulo() < 1.2f) { // Choca con rival
                hp2 -= (*it)->getDanio();
                impactado = true;
            }
        }
        else {
            if (((*it)->getPos() - pos1).modulo() < 1.2f) { // Choca con rival
                hp1 -= (*it)->getDanio();
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

    // 2. Mover obstáculos y comprobar colisiones
    for (auto it = obstaculos.begin(); it != obstaculos.end(); ) {
        (*it)->mueve(dt);

        bool borrar = false;

        // Comprobar colisión con el atacante (L1)
        if ((*it)->colisionaCon(pos1)) {
            std::cout << "¡El Jugador L1 ha tocado un objeto tipo " << (int)(*it)->getTipo() << "!" << std::endl;
            // Aquí en el futuro le sumaremos/restaremos vida
            borrar = true;
        }
        // Comprobar colisión con el defensor (L2)
        else if ((*it)->colisionaCon(pos2)) {
            std::cout << "¡El Jugador L2 ha tocado un objeto tipo " << (int)(*it)->getTipo() << "!" << std::endl;
            // Aquí en el futuro le sumaremos/restaremos vida
            borrar = true;
        }
        // Comprobar si la bola se ha caído fuera de la pantalla por abajo
        else if ((*it)->getPos().y < -12.0f) {
            borrar = true;
        }

        // Si ha chocado o se ha salido, lo borramos de la memoria
        if (borrar) {
            delete* it;
            it = obstaculos.erase(it);
        }
        else {
            ++it; // Si no, pasamos a comprobar la siguiente bola
        }
    }

    // Comprobar fin de batalla
    if (hp1 <= 0) { terminado = true; ganador = l2; perdedor = l1; }
    else if (hp2 <= 0) { terminado = true; ganador = l1; perdedor = l2; }
}




void Batalla::tecla(unsigned char key) {
    float vel = 0.5f; //Esto es lo q se traslada la pieza por pulsación tecla

    // JUGADOR 2
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


}

void Batalla::teclaEspecial(int key) {
    float vel = 0.5f;

    // JUGADOR 1
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
    Vector2D posDisparo = esJugador1 ? pos1 : pos2;
    // El J1 dispara a la derecha (positivo), el J2 a la izquierda (negativo)
    Vector2D vel = esJugador1 ? Vector2D(8, 0) : Vector2D(-8, 0);

    if (p->obtenerArma() == TipoArma::FLECHA) {
        proyectiles.push_back(new PelotaFutbol(posDisparo, vel, p->obtenerPoderAtaque(), esJugador1));
    }
    else if (p->obtenerArma() == TipoArma::BOLA_DE_FUEGO) {
        proyectiles.push_back(new BolaFuego(posDisparo, vel, p->obtenerPoderAtaque(), esJugador1));
    }
}