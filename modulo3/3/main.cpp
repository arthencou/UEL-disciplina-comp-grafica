/*
 * Módulo 3 - Exercício 3
 * Nomes:
 *      Arthur Henrique Coutinho
 *      Breno Naodi Kusunoki
 *      Luiz Guilherme Castilho Martins
 */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "glut.h"
#endif

#include "canvas.h"
#include <math.h> 
#include <stdio.h>

#define LADOS 8 //DEFINA QUANTOS ARESTAS TEM O POLIGONO

Canvas cvs(640, 480, "try out Canvas"); //global canvas object
double vetorAngle[50];

void ngon(int n, float cx, float cy, float radius, float rotAngle)
{
    if(n<3)
         return;
    double angle = (rotAngle*3.14159265)/180; //Transforma em radianos
    double angleInc = (2*3.14159265)/n;
    
    vetorAngle[0] = angle;
    cvs.moveTo(cos(angle)+cx, radius*sin(angle)+cy);
    
    for(int k=0 ; k<n ; k++) 
    {
         angle += angleInc; 
         vetorAngle[k+1] = angle;
         cvs.lineTo(cos(angle)/(radius+cx-0.1), (radius)*sin(angle)+cy);
    }
}

void diagonal(float cx, float cy, float radius)
{
    for(int i=0 ; i<(LADOS+1) ; i++) 
    {  
         if(vetorAngle[i]!=-1)
         {   
              for(int j=0 ; j<(LADOS+1) ; j++)
              {
                   cvs.moveTo(cos(vetorAngle[i])/(radius+cx-0.1), (radius)*sin(vetorAngle[i])+cy);
                   cvs.lineTo(cos(vetorAngle[j])/(radius+cx-0.1), (radius)*sin(vetorAngle[j])+cy);
              }
         }
    }
}

void myInit(void)
{
    cvs.setBackgroundColor(1.0, 1.0, 1.0);
    cvs.setColor(0.0, 0.0, 0.0);
    glPointSize(4.0);                 // a 'dot' is 4 by 4 pixels
    glLineWidth(2.0);       
    
    for(int i=0 ; i<50 ; i++)
         vetorAngle[i] = -1;
}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void plotsin()
{
    glBegin(GL_LINE_STRIP); {
	     for(GLfloat x = -4.0; x < 4.0; x += 0.1)
	     {
		      GLfloat y = sin(3.14159 * x) / (3.14159 * x);
		      glVertex2f(x, y);
         }		
    } 
    glEnd();    
    glFlush();   
}
void myDisplay(void)
{ 
     cvs.setWindow(-5.0,5.0,-0.3,1.0); // coordenada mundo obs:pode-se inverter os eixos para que a figura fique de cabeça para baixo
     cvs.setViewport(0,640,0,480);     // Sub Janela de visualização  
     glClear(GL_COLOR_BUFFER_BIT);
          
     int diagonais;     
     diagonais = (LADOS*(LADOS-3))/2;
     printf("\n\n Lados: %d \n Diagonais: %d \n\n",LADOS,diagonais);
     
     ngon(LADOS, 0.0, 0.3, 0.5, 90.0);
     diagonal(0.0, 0.3, 0.5);
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
    myInit();
    glutDisplayFunc(myDisplay);           // register redraw function     
    glutMainLoop();                       // go into a perpetual loop
}
