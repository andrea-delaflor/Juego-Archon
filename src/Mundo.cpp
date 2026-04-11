#include "Mundo.h"
#include "glut.h"
#include <math.h>
#include "Vector2D.h"

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
	//Limpiamos la pantalla y el buffer de profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Resetea el punto de vista
    glLoadIdentity();
	//Configuramos una vista ortográfica (2D) centrada en el tablero
    gluOrtho2D(-6, 6, -6, 6); // Ajuste de cámara

	//Dibujamos el tablero pasando el valor de luz actual
    tablero.dibuja(valorLuz);

    
    // ================PRUEBA PIEZATEST ====================

    if (piezaLuz != nullptr) {
        piezaLuz->dibuja(); // Esto ya hace la conversión interna a OpenGL
    }

    if (piezaOscuridad != nullptr) {
        piezaOscuridad->dibuja();
    }
    //-------------------------------------------------------------
   
    //Intercambia el lienzo oculto con el visible para mostrar el dibujo de golpe y sin parpadeos.
    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	//Inicializamos GLUT y creamos la ventana
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Archon - ETSIDI");

	//Registramos las funciones de dibujo y temporizador
    glutDisplayFunc(glueDibuja);
    glutTimerFunc(50, glueTimer, 0);

	//Inicializamos el mundo y comenzamos el bucle principal de GLUT
    mundo.inicializa();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glutMainLoop();
    return 0;
}