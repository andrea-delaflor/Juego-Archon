#include "batalla.h"
#include "glut.h" 
#include "ProyectilHijos.h"
#include "Interaccion.h"
#include <iostream>
#include <cstdlib> // Necesario para rand()
#include <cmath> // Necesario para sin() y cos() ciculo bonus visual


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
void Batalla::inicializa(Pieza* atacante, Pieza* defensor, int tipoArena, int ventajaRecibida) {
   
    l1 = atacante;
    l2 = defensor;
    ventaja = ventajaRecibida;

    terminado = false;
    empate = false;
    ganador = nullptr;
    perdedor = nullptr;

    std::cout << "Iniciando batalla. Ventaja tipo: " << ventaja << std::endl;

    //resetear los poderes cada vez que empieza una pelea
    velJ1 = l1->obtenerVelocidad() * 0.1f;
    velJ2 = l2->obtenerVelocidad() * 0.1f;
    invulnerableJ1 = false; invulnerableJ2 = false;
    multDanoJ1 = 1.0f; multDanoJ2 = 1.0f;
    temporizadorBonusJ1 = 0.0f; temporizadorBonusJ2 = 0.0f;

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
        //vamos a forzar la posicionVisual para que coincida sprite con hitbox:
        Vector2D backup1 = l1->obtenerPosicionVisual();
        l1->forzarPosicionVisual(Vector2D(0, 0));

        glPushMatrix();
        glTranslatef(pos1.x, pos1.y, 0);
        if (temporizadorBonusJ1 > 0) {
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // 1. Calcular animación basada en el tiempo real
            float tiempoObj = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
            float pulso = sin(tiempoObj * 8.0f) * 0.15f; // Efecto de latido
            float radio = 1.4f + pulso;

            // 2. Definir los colores base según el bonus
            float r = 1.0f, g = 1.0f, b = 1.0f;
            if (invulnerableJ1) { r = 0.0f; g = 1.0f; b = 1.0f; } // Hielo: Cian brillante
            else if (velJ1 > 0.5f) { r = 1.0f; g = 1.0f; b = 0.0f; } // Rayo: Amarillo eléctrico
            else if (multDanoJ1 > 1.0f) { r = 1.0f; g = 0.1f; b = 0.1f; } // Nube: Rojo sangre

            // 3. Dibujar el núcleo del aura (Degradado radial difuminado)
            glBegin(GL_TRIANGLE_FAN);
            glColor4f(r, g, b, 0.6f); // Centro muy brillante
            glVertex2f(0.0f, 0.0f);
            glColor4f(r, g, b, 0.0f); // Bordes invisibles para suavizar
            for (int i = 0; i <= 360; i += 10) {
                float theta = i * 3.14159f / 180.0f;
                glVertex2f(cos(theta) * radio, sin(theta) * radio);
            }
            glEnd();

            // 4. Dibujar runas/anillos mágicos giratorios alrededor
            glPushMatrix();
            glRotatef(tiempoObj * 120.0f, 0.0f, 0.0f, 1.0f); // Rota el primer anillo hacia la derecha

            glLineWidth(2.5f);
            glColor4f(r, g, b, 0.8f);
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 360; i += 60) { // Un hexágono mágico
                float theta = i * 3.14159f / 180.0f;
                glVertex2f(cos(theta) * (radio + 0.2f), sin(theta) * (radio + 0.2f));
            }
            glEnd();

            glRotatef(-tiempoObj * 200.0f, 0.0f, 0.0f, 1.0f); // Rota el segundo anillo muy rápido a la izquierda
            glBegin(GL_LINE_LOOP);
            for (int i = 30; i < 390; i += 60) { // Otro hexágono desfasado
                float theta = i * 3.14159f / 180.0f;
                glVertex2f(cos(theta) * (radio + 0.1f), sin(theta) * (radio + 0.1f));
            }
            glEnd();
            glPopMatrix();

            glLineWidth(1.0f); // Restaurar grosor de línea por defecto
            glEnable(GL_TEXTURE_2D);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Restaurar a blanco puro
        }
        glScalef(2.0f, 2.0f, 1.0f); // ajusta escalas de lo que dibujamos

        // dibuja la pieza + el arma
        l1->dibujaEnBatalla();
        glPopMatrix();

        l1->forzarPosicionVisual(backup1);

        // Dibujamos corazones vida
        l1->dibujaCorazones(-9.0f, 8.5f, 1.5f);
    }

    //JUGADOR 2
    if (l2 != nullptr) {
        Vector2D backup2 = l2->obtenerPosicionVisual();
        l2->forzarPosicionVisual(Vector2D(0, 0));

        glPushMatrix();
        glTranslatef(pos2.x, pos2.y, 0);
        if (temporizadorBonusJ2 > 0) {
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            float tiempoObj = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
            float pulso = sin(tiempoObj * 8.0f) * 0.15f;
            float radio = 1.4f + pulso;

            float r = 1.0f, g = 1.0f, b = 1.0f;
            if (invulnerableJ2) { r = 0.0f; g = 1.0f; b = 1.0f; }
            else if (velJ2 > 0.5f) { r = 1.0f; g = 1.0f; b = 0.0f; }
            else if (multDanoJ2 > 1.0f) { r = 1.0f; g = 0.1f; b = 0.1f; }

            glBegin(GL_TRIANGLE_FAN);
            glColor4f(r, g, b, 0.6f);
            glVertex2f(0.0f, 0.0f);
            glColor4f(r, g, b, 0.0f);
            for (int i = 0; i <= 360; i += 10) {
                float theta = i * 3.14159f / 180.0f;
                glVertex2f(cos(theta) * radio, sin(theta) * radio);
            }
            glEnd();

            glPushMatrix();
            glRotatef(tiempoObj * 120.0f, 0.0f, 0.0f, 1.0f);

            glLineWidth(2.5f);
            glColor4f(r, g, b, 0.8f);
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 360; i += 60) {
                float theta = i * 3.14159f / 180.0f;
                glVertex2f(cos(theta) * (radio + 0.2f), sin(theta) * (radio + 0.2f));
            }
            glEnd();

            glRotatef(-tiempoObj * 200.0f, 0.0f, 0.0f, 1.0f);
            glBegin(GL_LINE_LOOP);
            for (int i = 30; i < 390; i += 60) {
                float theta = i * 3.14159f / 180.0f;
                glVertex2f(cos(theta) * (radio + 0.1f), sin(theta) * (radio + 0.1f));
            }
            glEnd();
            glPopMatrix();

            glLineWidth(1.0f);
            glEnable(GL_TEXTURE_2D);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
        glScalef(2.0f, 2.0f, 1.0f);

        l2->dibujaEnBatalla();
        glPopMatrix();

        l2->forzarPosicionVisual(backup2);

        l2->dibujaCorazones(8.0f, 8.5f, 1.5f);
    }
    
    for (auto p : proyectiles) p->dibuja();
    for (auto o : obstaculos)  o->dibuja();

    
}

// MUEVE: Aquí irá la física de la pelea en el futuro
void Batalla::mueve() {
    float dt = 0.05f; // Ajusta según la velocidad de tu timer


    if (congelarDisparoJ1 > 0) congelarDisparoJ1 -= dt;
    if (congelarDisparoJ2 > 0) congelarDisparoJ2 -= dt;

    if (l1) l1->actualizarEscudo(dt);      //para gestionar los tiempos de los escudos
    if (l2) l2->actualizarEscudo(dt);

    //LÓGICA DE PROYECTILES
    //GESTIÓN DE PROYECTILES
    for (auto it = proyectiles.begin(); it != proyectiles.end(); ) {
        (*it)->mueve(dt);
        bool impactado = false;

        if ((*it)->esDeJugador1()) {
            Vector2D dist = (*it)->getPos() - pos2;
            // comprobar si hay escudo en J2
            if (l2 && l2->tieneEscudoActivo() && dist.modulo() < 1.5f) {
                impactado = true;
            }
            // si no hay escudo entra aqui
            else if (dist.modulo() < 1.5f) {
                float factor = 1.0f;
                
                if ((ventaja == 1 && l2->obtenerBando() == Bando::LUZ) ||
                    (ventaja == 2 && l2->obtenerBando() == Bando::OSCURIDAD)) {
                    factor = 0.5f;
                }
                
                if (!invulnerableJ2) {
                    l2->getVida().damage((*it)->getDanio() * factor * multDanoJ1);
                    hp2 = l2->getVida().getActual();
                }
                impactado = true;
            }
        }
        else {
            Vector2D dist = (*it)->getPos() - pos1;
            // comprobar si J1 tiene escudo
            if (l1 && l1->tieneEscudoActivo() && dist.modulo() < 1.5f) {
                impactado = true;
            }
            else if (dist.modulo() < 1.5f) {
                float factor = 1.0f;
                // Si la arena es LUZ (1) y el defensor es LUZ, recibe menos daño
                if ((ventaja == 1 && l1->obtenerBando() == Bando::LUZ) ||
                    (ventaja == 2 && l1->obtenerBando() == Bando::OSCURIDAD)) {
                    factor = 0.5f;
                }

                if (!invulnerableJ1) {
                    l1->getVida().damage((*it)->getDanio() * factor * multDanoJ2);
                    hp1 = l1->getVida().getActual();
                }
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
    if (arenaConObstaculos) {
        temporizadorObstaculos += dt;
        if (temporizadorObstaculos > 2.0f) {
            temporizadorObstaculos = 0.0f;
            float xAleatorio = -8.0f + (rand() % 1600) / 100.0f;
            Vector2D posObj(xAleatorio, 10.0f);
            Vector2D velObj(0.0f, -3.0f);

            TipoObstaculo tipoObj;
            if (indiceArenaActual == 0) tipoObj = TipoObstaculo::DANO; // Casilla Blanca -> Velas
            else if (indiceArenaActual == 1) tipoObj = TipoObstaculo::VELOCIDAD; // Casilla Negra -> Rayo
            else if (indiceArenaActual == 2) tipoObj = TipoObstaculo::CONGELACION; // Variable -> Hielo
            else tipoObj = TipoObstaculo::AUMENTO_DANO; // Punto Poder -> Nube
            //TipoObstaculo tipoAleatorio = static_cast<TipoObstaculo>(rand() % 3);
            //obstaculos.push_back(new Obstaculo(posObj, velObj, tipoAleatorio));

            obstaculos.push_back(new Obstaculo(posObj, velObj, tipoObj));
           
        }
    }
    // GESTIÓN DE TIEMPOS DE LOS BONUS (Duran 5 segundos)
    if (temporizadorBonusJ1 > 0) {
        temporizadorBonusJ1 -= dt;
        if (temporizadorBonusJ1 <= 0) { velJ1 = 0.5f; invulnerableJ1 = false; multDanoJ1 = 1.0f; }
    }
    if (temporizadorBonusJ2 > 0) {
        temporizadorBonusJ2 -= dt;
        if (temporizadorBonusJ2 <= 0) { velJ2 = 0.5f; invulnerableJ2 = false; multDanoJ2 = 1.0f; }
    }

    // 2. COLISIÓN DE OBSTÁCULOS
    for (auto it = obstaculos.begin(); it != obstaculos.end(); ) {
        (*it)->mueve(dt);
        bool borrar = false;

        Vector2D distL1 = (*it)->getPos() - pos1;
        Vector2D distL2 = (*it)->getPos() - pos2;

        // Comprobar Jugador 1
        if (distL1.modulo() < 1.5f) {
            if ((*it)->getTipo() == TipoObstaculo::DANO && !l1->tieneEscudoActivo() && !invulnerableJ1) {
                float factorObs = 1.0f;
                if ((ventaja == 1 && l1->obtenerBando() == Bando::LUZ) || (ventaja == 2 && l1->obtenerBando() == Bando::OSCURIDAD)) factorObs = 0.5f;
                l1->getVida().damage(10 * factorObs);
                hp1 = l1->getVida().getActual();
            }
            else if ((*it)->getTipo() == TipoObstaculo::VELOCIDAD) { velJ1 = 1.0f; temporizadorBonusJ1 = 5.0f; }
            else if ((*it)->getTipo() == TipoObstaculo::CONGELACION) { invulnerableJ1 = true; temporizadorBonusJ1 = 5.0f; }
            else if ((*it)->getTipo() == TipoObstaculo::AUMENTO_DANO) { multDanoJ1 = 2.0f; temporizadorBonusJ1 = 5.0f; }
            borrar = true;
        }
        // Comprobar Jugador 2
        else if (distL2.modulo() < 1.5f) {
            if ((*it)->getTipo() == TipoObstaculo::DANO && !l2->tieneEscudoActivo() && !invulnerableJ2) {
                float factorObs = 1.0f;
                if ((ventaja == 1 && l2->obtenerBando() == Bando::LUZ) || (ventaja == 2 && l2->obtenerBando() == Bando::OSCURIDAD)) factorObs = 0.5f;
                l2->getVida().damage(10 * factorObs);
                hp2 = l2->getVida().getActual();
            }
            else if ((*it)->getTipo() == TipoObstaculo::VELOCIDAD) { velJ2 = 1.0f; temporizadorBonusJ2 = 5.0f; }
            else if ((*it)->getTipo() == TipoObstaculo::CONGELACION) { invulnerableJ2 = true; temporizadorBonusJ2 = 5.0f; }
            else if ((*it)->getTipo() == TipoObstaculo::AUMENTO_DANO) { multDanoJ2 = 2.0f; temporizadorBonusJ2 = 5.0f; }
            borrar = true;
        }
        else if ((*it)->getPos().y < -12.0f) borrar = true;

        if (borrar) { delete* it; it = obstaculos.erase(it); }
        else ++it;
    }

    //  DAÑO POR CONTACTO DEL ESCUDO 
    
    static int contadorDano = 0;
    contadorDano++;

    if (contadorDano >= 2) { // se hace daño cada 2 tiempos, que sino es muy fuerte

        
        if (l1 && l2 && l1->tieneEscudoActivo()) {
            if (Interaccion::colisionConEscudo(pos2, pos1)) {
                l2->getVida().damage(1); 
                hp2 = l2->getVida().getActual(); 
            }
        }

        
        if (l2 && l1 && l2->tieneEscudoActivo()) {
            if (Interaccion::colisionConEscudo(pos1, pos2)) {
                l1->getVida().damage(1); 
                hp1 = l1->getVida().getActual(); 
            }
        }

        contadorDano = 0; // se reincia contador
    }

    //CONDICIÓN DE VICTORIA
    if (l1->getVida().muerto()) { terminado = true; ganador = l2; perdedor = l1; }
    else if (l2->getVida().muerto()) { terminado = true; ganador = l1; perdedor = l2; }
}




void Batalla::tecla(unsigned char key) {
   // float vel = 0.5f; //Esto es lo q se traslada la pieza por pulsación tecla
    

    // JUGADOR 1
    if (key == 'w' || key == 'W') pos1.y += velJ1;
    if (key == 's' || key == 'S') pos1.y -= velJ1;
    if (key == 'a' || key == 'A') pos1.x -= velJ1;
    if (key == 'd' || key == 'D') pos1.x += velJ1;

    // Disparos y Controles
    if (key == ' ') {
        if (congelarDisparoJ1 <= 0.0f) {
            generarDisparo(true); //pulsando espacio jugador 1 usa su poder
            congelarDisparoJ1 = 0.5f; // Medio segundo de recarga
        }
    }
        
       
    if (key == 13) {
        if (congelarDisparoJ2 <= 0.0f) {
			generarDisparo(false); //pulsando enter jugador 2 usa su poder
            congelarDisparoJ2 = 0.5f; // Medio segundo de recarga
        }
    }
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
            if (Interaccion::colisionCuerpoACuerpo(pos1, pos2, l1->obtenerAlcance())) {
                l2->getVida().damage(l1->obtenerPoderAtaque());
                hp2 = l2->getVida().getActual();
            }
        }
    }

    // Para el Jugador 2
    if ((key == 'y' || key == 'Y') && l2) {
        if (l2->obtenerArma() == TipoArma::ESCUDO) {
            l2->activarEscudo();
        }
        else {
            l2->iniciarAnimacion(); // Esto es para el Troll y su código
            if (Interaccion::colisionCuerpoACuerpo(pos2, pos1, l2->obtenerAlcance())) {
                l1->getVida().damage(l2->obtenerPoderAtaque());
                hp1 = l1->getVida().getActual();
            }
        }
    }

}

void Batalla::teclaEspecial(int key) {
    //float vel = 0.5f;

   // JUGADOR 2
    if (key == GLUT_KEY_UP)    pos2.y += velJ2;
    if (key == GLUT_KEY_DOWN)  pos2.y -= velJ2;
    if (key == GLUT_KEY_LEFT)  pos2.x -= velJ2;
    if (key == GLUT_KEY_RIGHT) pos2.x += velJ2;

    // Límites de la pantalla 
    if (pos2.x > 9.5f) pos2.x = 9.5f;
    if (pos2.x < -9.5f) pos2.x = -9.5f;
    if (pos2.y > 9.5f) pos2.y = 9.5f;
    if (pos2.y < -9.5f) pos2.y = -9.5f;
}

void Batalla::generarDisparo(bool esJugador1) {
    Pieza* p = esJugador1 ? l1 : l2;
    Vector2D posDisparo = esJugador1 ? pos1 : pos2;
    /*
    Vector2D posPieza = esJugador1 ? pos1 : pos2;
    float offsetX = -2.0f;
    float offsetY = 3.75f;
    Vector2D posDisparo = posPieza + Vector2D(offsetX, offsetY);
    */

    int ataque = p->obtenerPoderAtaque(); //obtenemos el poder de ataque de cada pieza

    // El J1 dispara a la derecha (positivo), el J2 a la izquierda (negativo)
    Vector2D vel = esJugador1 ? Vector2D(10, 0) : Vector2D(-10, 0);

    switch (p->obtenerArma()) {
    case TipoArma::PELOTAFUTBOL: // Arquera
        proyectiles.push_back(new PelotaFutbol(posDisparo, vel, ataque,  esJugador1));
        break;

    case TipoArma::BOLA_DE_FUEGO: // Dragón
        proyectiles.push_back(new BolaFuego(posDisparo, vel * 0.7f, ataque, esJugador1));
        break;

    case TipoArma::RAYO_LASER: // Djinni (Láser)
        proyectiles.push_back(new RayoLaser(posDisparo, vel* 1.3f, ataque, esJugador1));
        break;

    case TipoArma::RAYO_NUMERICO: // Basilisco (Cálculo)
        proyectiles.push_back(new RayoNumerico(posDisparo, vel*1.15f, ataque, esJugador1));
        break;

    case TipoArma::ACTAS: // Mago y Bruja (Actas)
        proyectiles.push_back(new Acta(posDisparo, vel * 0.5f, ataque, esJugador1));
        break;

    default:
        break;
    }
}

