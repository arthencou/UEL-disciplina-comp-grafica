/*
 * Módulo 3 - Exercício 2
 * Nomes:
 *      Arthur Henrique Coutinho
 *      Breno Naodi Kusunoki
 *      Luiz Guilherme Castilho Martins
 */
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include "glut.h"
#endif

#include <math.h>
#include <stdio.h>

using namespace std;

class GLintPoint
{
        public:
                GLint x,y;
};
GLintPoint peak;
const float DEG2RAD = 3.14159/180;


void drawORBody(float xc, float yc, float radius)
{
glBegin(GL_LINE_STRIP);

for(int i=270; i<=360; i++)
{
float degInRad = i* DEG2RAD;
float x=xc+1.4*radius*cos(degInRad);
float y=yc+0.5*radius*sin(degInRad);
glVertex2f(x,y);
}
glEnd();

glBegin(GL_LINE_STRIP);
        for(int i=0; i<=90; i++)
        {
                float degInRad = i* DEG2RAD;
                float x=xc+1.4*radius*cos(degInRad);
                float y=yc+0.5*radius*sin(degInRad);
                glVertex2f(x,y);
        }
        glEnd();


        glBegin(GL_LINE_STRIP);
        for(int i=0; i<=90; i++)
        {
                float degInRad = i* DEG2RAD;
                float x=xc+0.2*radius*cos(degInRad);
                float y=yc+0.5*radius*sin(degInRad);
                glVertex2f(x,y);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for(int i=270; i<=360; i++)
        {
                float degInRad = i* DEG2RAD;
                float x=xc+0.2*radius*cos(degInRad);
                float y=yc+0.5*radius*sin(degInRad);
                glVertex2f(x,y);
        }
        glEnd();

}

void DrawCircle(float cx, float cy, float r, int num_segments)
{
        float theta = (2 * 3.1415926) / float(num_segments);
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

void DrawHalfCircle(float cx, float cy, float r, int num_segments)
{
        float theta = ((2 * 3.1415926) / float(num_segments))/4;
        float c = cosf(theta);
        float s = sinf(theta);
        float t;

        float x = r;
        float y = 0;

        glBegin(GL_LINE_STRIP);
        for(int ii = 0; ii < num_segments; ii++) {
                /* Plotando o ponto */
                glVertex2f(x + cx, y + cy);

                /* Aplicando rotação de Matriz */
                t = x;
                x = c * x - s * y;
                y = s * t + c * y;
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        theta = -((2 * 3.1415926) / float(num_segments))/4;
        c = cosf(theta);
        s = sinf(theta);

        x = r;
        y = 0;

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

void AND()
{
        /* Desenhando o corpo da AND */
        glBegin(GL_LINE_STRIP);
                glVertex2i(323, 330);
                glVertex2i(353, 330);
        glEnd();

        glBegin(GL_LINE_STRIP);
                glVertex2i(323, 370);
                glVertex2i(353, 370);
        glEnd();

        glBegin(GL_LINE_STRIP);
                glVertex2i(323, 350);
                glVertex2i(323, 330);
        glEnd();

        glBegin(GL_LINE_STRIP);
                glVertex2i(323, 350);
                glVertex2i(323, 370);
        glEnd();

        /* Desenhando as entradas */
        glBegin(GL_LINE_STRIP);
                glVertex2i(323, 340);
                glVertex2i(300, 340);
        glEnd();

        glBegin(GL_LINE_STRIP);
                glVertex2i(323, 360);
                glVertex2i(300, 360);
        glEnd();

        /* Desenhando a saída */
        glBegin(GL_LINE_STRIP);
                glVertex2i(372, 350);
                glVertex2i(393, 350);
        glEnd();


        /* Desenhando o semi-círculo */
        DrawHalfCircle(352, 350, 20, 1000);
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
                glVertex2i(383, 300);
        glEnd();
}

void OR()
{
        /* Desenhando o corpo da OR */
        drawORBody(323, 250, 35);
        /* Desenhando as entradas */
        glBegin(GL_LINE_STRIP);
                glVertex2i(328, 240);
                glVertex2i(300, 240);
        glEnd();

        glBegin(GL_LINE_STRIP);
                glVertex2i(328, 260);
                glVertex2i(300, 260);
        glEnd();

        /* Desenhando a saída */
        glBegin(GL_LINE_STRIP);
                glVertex2i(372, 250);
                glVertex2i(393, 250);
        glEnd();
}

 void myInit(void)
 {
        glClearColor(1.0,1.0,1.0,0.0);
        glColor3f(0.0f, 0.0f, 0.0f);
        glPointSize(4.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void myDisplay(void)
{
        glClear(GL_COLOR_BUFFER_BIT);
        AND();
        NOT();
        OR();
        glFlush();
}

int main(int argc, char** argv)
{
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(640,480);
        glutInitWindowPosition(100, 150);
        glutCreateWindow("Primeiro programa");
        glutDisplayFunc(myDisplay);
        myInit();
        glutMainLoop();
}

