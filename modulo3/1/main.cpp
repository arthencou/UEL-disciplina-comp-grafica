/*
 * Módulo 3 - Exercício 1
 * Nomes:
 *      Arthur Henrique Coutinho
 *      Breno Naodi Kusunoki
 *      Luiz Guilherme Castilho Martins
 */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>

using namespace std;

void myInit(void)
{
        glClearColor(1.0,1.0,1.0,0.0);
        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(4.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, 640.0, 0.0, 480.0);
        glLineWidth(3.4f);
        glClear(GL_COLOR_BUFFER_BIT);
        glFrontFace(GL_CW);
        glColor3f(0.0f, 0.0f, 0.0f);

}

void padrao(int x, int y)
{
              glBegin(GL_LINE_STRIP);
                glVertex2i( x, y);
                glVertex2i( x, y + 40);
                glVertex2i( x +  40, y + 60);
                glVertex2i( x +  40, y + 100);
                glVertex2i( x +  80, y + 80);
                glVertex2i( x +  80, y + 40);
                glVertex2i( x + 120, y + 20);
                glVertex2i( x +  80, y);
                glVertex2i( x +  40, y + 20);
                glVertex2i(x, y);
        glEnd();
        glFlush();
}

void myDisplay(void)
{
        padrao(100,100);
        padrao(220, 120);
        padrao(340, 140);
        padrao(140, 200);
        padrao(260, 220);
        padrao(180, 20);
        padrao(300, 40);
        padrao(340, 340);
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


