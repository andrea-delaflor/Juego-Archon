#include <iostream>
#include "glut.h"
#include "Coordinador.h"

//Instancia global del coordinador para que las funciones de glut puedan acceder a él
Coordinador coordinador;

//Funciones de glut
void glueDibuja() { 
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Un gris azulado
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. Llamar al coordinador
    coordinador.dibuja();

    // 3. Mostrar en pantalla
    glutSwapBuffers();
}

//Función de temporizador para actualizar el mundo cada 50ms
void glueTimer(int v) {
    //Calcula el nuevo valor de la luz y el ángulo para el parpadeo
    coordinador.mueve();
    //Solicita que se vuelva a dibujar la pantalla con el nuevo estado
    glutPostRedisplay();
    //Vuelve a llamar a esta función después de 50ms para seguir actualizando
    glutTimerFunc(50, glueTimer, 0); // Cada 50ms
}

//añadimos esta funcion glue para que funcionen las teclas
void glueTeclado(unsigned char key, int x, int y) {
    coordinador.tecla(key);
}

//creamos teclado especial para leer flechas del teclado
void glueTecladoEspecial(int key, int x, int y) {
    coordinador.teclaEspecial(key);
}


void glueRaton(int button, int state, int x, int y) {
    coordinador.gestionaRaton(button, state, x, y);
}

void glueRatonPasivo(int x, int y) {
    coordinador.gestionaRatonPasivo(x, y);
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
    glutPassiveMotionFunc(glueRatonPasivo);
    glutTimerFunc(50, glueTimer, 0);
    glutKeyboardFunc(glueTeclado);
    glutSpecialFunc(glueTecladoEspecial);

    //observamos que hemos quitado el mundo.inicializa() porqu ahora se inicializa en coordinador con su constructor 

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glutMainLoop();
    return 0;
}