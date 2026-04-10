#include "Mundo.h"
#include "glut.h"
#include <math.h>

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

    // --- PRUEBA: Coordenada concreta (Fila 2, Columna 6) ---
    // Esto corresponde a la casilla G7 en el tablero original
    posPrueba.x = 6.0;
    posPrueba.y = 2.0;
    // ------------------------------------------------------
    // NUEVA: Posición del círculo amarillo (Casilla D6)
    posCirculo.x = 3.0;
    posCirculo.y = 5.0;
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
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

    // ============================================================
    // BLOQUE DE PRUEBA: DIBUJO DEL PUNTO ROJO (Basado en Vector2D)
    // ============================================================

    // Convertimos la coordenada del Vector2D (0 a 8) al mundo OpenGL (-4 a 4)
    // Usamos los miembros .x y .y de tu struct Vector2D
    float x_opengl = (float)posPrueba.x - 4.0f;
    float y_opengl = 4.0f - (float)posPrueba.y; // Invertimos Y para que (0,0) sea arriba

    // Definimos un tamaño de punto grande para que sea visible
    glPointSize(20.0f);

    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo puro
    glVertex2f(x_opengl, y_opengl);
    glEnd();
    // ============================================================
<<<<<<< Updated upstream
    // Convertimos la posición de Vector2D al mundo OpenGL
    float x_yellow = (float)posCirculo.x - 4.0f;
    float y_yellow = 4.0f - (float)posCirculo.y;

=======
    // NUEVO: DIBUJO DEL CÍRCULO AMARILLO
    // ============================================================

    // Convertimos la posición de Vector2D al mundo OpenGL
    float x_yellow = (float)posCirculo.x - 4.0f;
    float y_yellow = 4.0f - (float)posCirculo.y;

>>>>>>> Stashed changes
    // Dibujamos el círculo (usando la misma técnica que los PowerPoints)
    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
    glBegin(GL_POLYGON);
    for (int i = 0; i < 30; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / 30.0f;
        // Radio de 0.3f para que quede centrado en la casilla
        glVertex2f(x_yellow + 0.3f * cosf(theta), y_yellow + 0.3f * sinf(theta));
    }
    glEnd();
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
    glutMainLoop();
    return 0;
}