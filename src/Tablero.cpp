#include "Tablero.h"
#include "Pieza.h"
#include "glut.h"


//Parte logica del tablero

// Inicializamos todas las "casillas" como vacias
Tablero::Tablero() {
    // Inicializamos todas las casillas a vacío (nullptr)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            casillas[i][j] = nullptr;
        }
    }
}

void Tablero::vaciar() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // Ponemos a null la casilla para que no apunte a piezas viejas
            casillas[i][j] = nullptr;
            // Si tu matriz es de punteros directos: casillas[i][j] = nullptr;
        }
    }
}
// con esto evitamos salirnos fuera del tablero
bool Tablero::posicionValida(int i, int j) {
    if (i >= 0 && i < 9 && j >= 0 && j < 9) {
        return true;
    }
    return false;
}

// Con esto lo que hacemos es que cuando elijamos una posicion == casilla primero
// determinamos si esta dentro del tablero y luego si esta libre
Pieza* Tablero::obtenerOcupante(int i, int j) {
    // Si nos preguntan por un sitio fuera del mapa, devolvemos nulo por seguridad
    if (!posicionValida(i, j)) {
        return nullptr;
    }
    return casillas[i][j];
}

// Aqui es como hacemos que una casilla se ocupe----> p es nuestra pieza luego le pedimos que 
// la coloque en una posicion y le mandamos a la propia pieza el donde esta para 
// que ella misma se acuerde de donde esta 
void Tablero::colocarPieza(int i, int j, Pieza* p) {
    if (posicionValida(i, j)) {
        casillas[i][j] = p;
        if (p != nullptr) {
            p->establecerPosicion(Vector2D(i, j)); // Sincronizamos la pieza
        }
    }
}

//Parte visual del tablero

//COLUMNAS: i=0 (A) a i=8 (I)
//FILAS: j=0  a j=8 

bool Tablero::esPowerPoint(int i, int j) {
    //Los 5 puntos de poder (E5, A5, I5, E1, E9)
    if ((i == 4 && j == 4) || //Centro
        (i == 0 && j == 4) || (i == 8 && j == 4) || //Centros laterales
        (i == 4 && j == 0) || (i == 4 && j == 8))   //Centros superior/inferior
        return true;
    return false;
}

bool Tablero::esVariable(int i, int j) {
	//Columna E (i=4) completa, excepto los puntos de poder E1 y E9 que ya son puntos de poder fijos
    if (i == 4) return true;

    if (j == 4 && i > 0 && i < 8) return true;

    //DIAGONALES
    //Diagonal A3 a D0: (0,3), (1,2), (2,1), (3,0) -> suma i+j = 3
    if (i + j == 3) return true;

    //Diagonal F0 a I3: (5,0), (6,1), (7,2), (8,3) -> resta i-j = 5
    if (i - j == 5) return true;

    //Diagonal I5 a F8: (8,5), (7,6), (6,7), (5,8) -> suma i+j = 13
    if (i + j == 13) return true;

    //Diagonal D8 a A5: (3,8), (2,7), (1,6), (0,5) -> resta j-i = 5
    if (j - i == 5) return true;

    return false;
}


void Tablero::dibuja(float luminosidad) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            //Prioridad absoluta: Si es variable, parpadea (esto incluye E1 y E9)
            if (esVariable(i, j)) {
                glColor3f(luminosidad, luminosidad, luminosidad);
            }
            else {
                //LÓGICA DE ESQUINAS:
                //Para que i=0 (A) sea negra y i=8 (I) sea blanca en j=0
				//A partir de ahí crea un patrón de tablero usando la paridad de (i+j) según la columna
                          
                
                if (i < 4) { //LADO IZQUIERDO: (A, B, C, D)
                    //Forzamos A4 a ser BLANCA, y el resto damero normal para que A0 sea NEGRA
                    if (i == 0 && j == 4) {
                        glColor3f(1.0f, 1.0f, 1.0f); //Blanco forzado en A4
                    }
                    else {
                        //Damero normal donde (0,0) es negro
                        if ((i + j) % 2 != 0) glColor3f(1.0f, 1.0f, 1.0f);
                        else glColor3f(0.0f, 0.0f, 0.0f);
					}
				}
				else if (i > 4) { //LADO DERECHO: (F, G, H, I)
					//Forzamos I4 a ser NEGRA, y el resto damero normal
                    if (i == 8 && j == 4) {
                        glColor3f(0.0f, 0.05f, 0.15f); //Negro forzado en I4 
                    }
                    else if ((i + j) % 2 == 0) glColor3f(1.0f, 1.0f, 1.0f); //Blanca
                    else glColor3f(0.0f, 0.05f, 0.15f);                     //Negra
                }
                
            }

            //DIBUJAR EL CUADRADO DE LA CASILLA
            float x = i - 4.0f;
            float y = 4.0f - j;

            glBegin(GL_QUADS);
            glVertex2f(x - 0.48f, y - 0.48f);
            glVertex2f(x + 0.48f, y - 0.48f);
            glVertex2f(x + 0.48f, y + 0.48f);
            glVertex2f(x - 0.48f, y + 0.48f);
            glEnd();

            //DIBUJAR EL CÍRCULO SI ES POWERPOINT (Independientemente de si es variable o no)
            if (esPowerPoint(i, j)) {
                glColor3f(1.0f, 0.85f, 0.0f); //Dorado
                glBegin(GL_POLYGON);
                for (int a = 0; a < 30; a++) {
                    float theta = 2.0f * 3.1415926f * float(a) / 30.0f;
                    glVertex2f(x + 0.3f * cosf(theta), y + 0.3f * sinf(theta));
                }
                glEnd();
            }
        }
    }
    glColor3f(1.0f, 1.0f, 1.0f);
}