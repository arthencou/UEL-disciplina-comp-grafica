
//foi utilizado o exemplo do livro texto da disciplina:
//         Computer Graphics using Open GL, 2nd edition, by Francis S. Hill
//inserir no linker da opção de projetos: -lopengl32 -lglut32 -lglu32

#include "glut.h"

#include <math.h> 

// coordenadas mundo
void setWindow(float left,float right,float bottom,float top)
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(left,right,bottom,top);
}
// subjanela de visualização
// considera a janela definida no comando glutInitWindowSize(640,480); 
//                                        *** tamanho da Janela ***
void setViewport(int left,int right,int bottom,int top)
{
glViewport(left,bottom,right-left,top-bottom);
}

 void myInit(void)
 {
    glClearColor(1.0,1.0,1.0,0.0);        // set white background color
    glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color
    glPointSize(4.0);                 // a 'dot' is 4 by 4 pixels
    glLineWidth(2.0);       
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
    } glEnd();
    
    glFlush();   
}
void myDisplay(void)
{ 
     setWindow(-5.0,5.0,-0.3,1.0); // coordenada mundo obs:pode-se inverter os eixos para que a figura fique de cabeça para baixo
     setViewport(0,640,0,480);     // Sub Janela de visualização  
     glClear(GL_COLOR_BUFFER_BIT);
           
     setViewport(0,640,0,480);
     plotsin();
      
     setViewport(340,640,260,480);
     plotsin();
}
//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
        glutInit(&argc, argv);            // initialize the toolkit
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
        glutInitWindowSize(640,480);          // *** tamanho da Janela ***
        glutInitWindowPosition(100, 150);     // set window position on screen
        glutCreateWindow("Primeiro programa"); // open the screen window
        glutDisplayFunc(myDisplay);           // register redraw function
        myInit();
        glutMainLoop();                       // go into a perpetual loop
}

