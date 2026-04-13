#include "Mundo.h"
#include "glut.h"
#include <math.h>
#include "Vector2D.h"

//Instancia global del mundo para que las funciones de glut puedan acceder a él

Mundo mundo;

//Funciones de glut

void glueDibuja() { mundo.dibuja(); }

//Función de temporizador para actualizar el mundo cada 50ms
void glueTimer(int v) {
	//Calcula el nuevo valor de la luz y el ángulo para el parpadeo
    mundo.mueve();
	//Solicita que se vuelva a dibujar la pantalla con el nuevo estado
    glutPostRedisplay();
	//Vuelve a llamar a esta función después de 50ms para seguir actualizando
    glutTimerFunc(50, glueTimer, 0); // Cada 50ms
}


//Funciones de la clase Mundo

void Mundo::inicializa() {
	//Inicializamos el valor de la luz y el ángulo para el parpadeo
    valorLuz = 0.5f;
    angulo = 0.0f;

    //---------------PRUEBA PIEZATEST-------------------
        // Creamos una pieza cian en la casilla (0,0)
    piezaLuz = new PiezaTest({ 0.0, 0.0 }, Bando::LUZ);

    // Creamos una pieza magenta en la casilla (8,8)
    piezaOscuridad = new PiezaTest({ 8.0, 8.0 }, Bando::OSCURIDAD);

}

//Movemos el estado del mundo para crear el efecto de parpadeo suave en las casillas variables. 
void Mundo::mueve() {
	//Incrementamos el ángulo para crear una oscilación suave
    angulo += 0.05f;
	//Calculamos el nuevo valor de la luz usando una función seno para que oscile entre 0 y 1. Usamos forma cíclica para que el parpadeo sea suave y continuo
    valorLuz = (sin(angulo) + 1.0f) / 2.0f; // Oscilación entre 0 y 1

   
}

void Mundo::dibuja() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluOrtho2D(-6, 6, -6, 6);

    // 1. Tablero de fondo
    tablero.dibuja(valorLuz);

    // 2. Ratón con cambio de color
    // Si hay algo seleccionado, pintamos el recuadro de AMARILLO, si no, de ROJO
    if (seleccionada != nullptr) {
        // Podemos añadir un método a Raton para cambiar color o hacerlo aquí
        glColor3f(1.0f, 1.0f, 0.0f); // Amarillo (Seleccionado)
    }
    else {
        glColor3f(1.0f, 0.0f, 0.0f); // Rojo (Normal)
    }
    raton.dibuja();

    // 3. Piezas
    if (piezaLuz) piezaLuz->dibuja();
    if (piezaOscuridad) piezaOscuridad->dibuja();

    glutSwapBuffers();
}

void Mundo::clickRaton(int button, int state, int x, int y) {
    // 1. Actualizamos la casilla donde se ha hecho click
    raton.actualizaPosicion(x, y, 800, 800);
    Vector2D c = raton.casilla;

    // Si el click es fuera del tablero, no hacemos nada
    if (c.x == -1) return;

    // ESTADO A: No hay nada seleccionado (Primer Click)
    if (seleccionada == nullptr) {
        // Miramos si en la casilla 'c' hay alguna de nuestras piezas de prueba
        if (piezaLuz && (int)piezaLuz->obtenerPosicion().x == (int)c.x && (int)piezaLuz->obtenerPosicion().y == (int)c.y) {
            seleccionada = piezaLuz;
        }
        else if (piezaOscuridad && (int)piezaOscuridad->obtenerPosicion().x == (int)c.x && (int)piezaOscuridad->obtenerPosicion().y == (int)c.y) {
            seleccionada = piezaOscuridad;
        }
    }
    // ESTADO B: Ya hay una pieza seleccionada (Segundo Click)
    else {
        // 1. Obtenemos los movimientos que dicta tu lógica de Movimiento.cpp
        std::vector<Vector2D> validos = seleccionada->obtenerMovimientosValidos(&tablero);

        // 2. Comprobamos si la casilla donde hemos hecho el segundo click está en la lista
        bool esDestinoValido = false;
        for (auto& v : validos) {
            if ((int)v.x == (int)c.x && (int)v.y == (int)c.y) {
                esDestinoValido = true;
                break;
            }
        }

        // 3. Si es válido, movemos la pieza
        if (esDestinoValido) {
            seleccionada->establecerPosicion(c);
        }

        // 4. Pase lo que pase (se mueva o no), liberamos la selección para el siguiente turno
        seleccionada = nullptr;
    }

    // Forzamos a redibujar para que el movimiento se vea al instante
    glutPostRedisplay();
}

void glueRaton(int button, int state, int x, int y) {
    // Solo actuamos cuando se PULSA (state == 0) el botón IZQUIERDO (button == 0)
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 1. Decirle al objeto raton que calcule la casilla
        // Pasamos 800, 800 que es el tamaño de tu ventana en el main
        mundo.clickRaton(button, state, x, y);
    }
}

void mousePasivo(int x, int y) {
    mundo.raton.actualizaPosicion(x, y, 800, 800);
}


int main(int argc, char* argv[]) {
	//Inicializamos GLUT y creamos la ventana
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Archon - ETSIDI");
    

	//Registramos las funciones de dibujo y temporizador
    glutDisplayFunc(glueDibuja);
    glutMouseFunc(glueRaton);
    glutPassiveMotionFunc(mousePasivo);
    glutTimerFunc(50, glueTimer, 0);
 
    

	//Inicializamos el mundo y comenzamos el bucle principal de GLUT
    mundo.inicializa();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glutMainLoop();
    return 0;
}