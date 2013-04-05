#ifdef __APPLE__
#include <GLUT/glut.h> /* Open GL Util APPLE */
#else
#include <GL/glut.h> /* Open GL Util OpenGL*/
#endif

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>

using namespace std;

/* Função que desenha o círculo */
void DrawCircle(float cx, float cy, float r, int num_segments)
{
	float theta = 2 * 3.1415926 / float(num_segments);

    /* Cálculo do seno e cosseno */
    float c = cosf(theta);
    float s = sinf(theta);
	float t;

	float x = r;
    float y = 0;

	glBegin(GL_LINE_LOOP);
	for(int ii = 0; ii < num_segments; ii++) {
            /* Plotando o ponto */
            glVertex2f(x + cx, y + cy);

            /* Aplicando rotação de Matriz */
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
	}
	glEnd();
}

void NOT()
{
        /* Desenhando a entrada da Porta NOT */
        glBegin(GL_LINE_STRIP);
                glVertex2i(320, 300);
                glVertex2i(300, 300);
        glEnd();

        /* Desenhando o Triangulo */
        glBegin(GL_TRIANGLES);
                glVertex2i(320, 320);
                glVertex2i(320, 280);
                glVertex2i(350, 300);
        glEnd();

        /* Definindo a cor como BRANCA para futura Gambiarra */
        glColor3f( 1.0f, 1.0f, 1.0f );

        /*
         * GAMBIARRA: Desenhando o triangulo interno em BRANCO, para
         * parecer que não existe preenchimento.
         */
        glBegin(GL_TRIANGLES);
                glVertex2f(320.6, 319);
                glVertex2f(320.6, 281);
                glVertex2f(349, 300);
        glEnd();

        /* Definindo a cor como PRETA */
        glColor3f( 0.0f, 0.0f, 0.0f );

        /* Desenhando o círculo */
        DrawCircle(353, 300, 4, 1000);

        /* Desenhando a saída da porta NOT */
        glBegin(GL_LINE_STRIP);
                glVertex2i(357, 300);
                glVertex2i(363, 300);
        glEnd();
}

 void myInit(void)
 {
    glClearColor(1.0,1.0,1.0,0.0);        // set white background color
    glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color
    glPointSize(4.0);                 // a 'dot' is 4 by 4 pixels
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}
//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
     glClear(GL_COLOR_BUFFER_BIT);     // clear the screen
     NOT();
}
//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
        glutInit(&argc, argv);            // initialize the toolkit
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
        glutInitWindowSize(640,480);          // set window size
        glutInitWindowPosition(100, 150);     // set window position on screen
        glutCreateWindow("Primeiro programa"); // open the screen window
        glutDisplayFunc(myDisplay);           // register redraw function
        myInit();
        glutMainLoop();                       // go into a perpetual loop
}
