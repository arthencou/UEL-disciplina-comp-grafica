
/*
* Ex6 - Módulo 3 - Parte 2
* Nomes:
*      Arthur Henrique Coutinho
*      Breno Naodi Kusunoki
*      Luiz Guilherme Castilho Martins
*/
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <GL\gl.h>
#include "glaux.h"
#include "glut.h"
#include <time.h>

#include "ImageClass.h"
#define LIMIAR 220

ImageClass *Image, *NovaImagem, *ImageDeformada;
/** ***************************************************************************
* 6) Implemente a Deformação da imagem de acordo com a teoria apresentada.
**/
ImageClass Deformacao(ImageClass *inImage)
{
    ImageClass *outImage = new ImageClass(inImage->SizeX(), inImage->SizeY());
    
    int x, y;
    byte r, g, b;
    
    int VX1, VY1, VX2, VY2, VX3, VY3;
    
    VX1 = 1;
    VY1 = 1;
    
    VX2 = (int)((inImage->SizeX()-1)/2);
    VY2 = (int)((inImage->SizeY()-10));
    
    VX3 = inImage->SizeX()-10;
    VY3 = 10;
    
    float C1, C2, C3;
    float Vx, Vy;
    for(C1=0.0f ; C1<1.0f ; C1+=0.005f) {
        for(C2=0.0f ; C2<1.0f ; C2+=0.005f) {
            C3 = 1 - C1 - C2;
            if(C3>=0.0f) {
                Vx = (C1*VX1) + (C2*VX2) + (C3*VX3);
                Vy = (C1*VY1) + (C2*VY2) + (C3*VY3);
                inImage->ReadPixel((int)Vx,(int)Vy,r,g,b);
                outImage->DrawPixel((int)Vx,(int)Vy,r,g,b);
            }
        }
    }
    
    // Deformação em W
    int WX1, WY1, WX2, WY2, WX3, WY3;
    
    WX1 =  (int)((inImage->SizeX()-2)/2);//50;
    WY1 = (int)((inImage->SizeY()-20));//10;
    
    WX2 = inImage->SizeX()-30;//(int)((inImage->SizeX()/2)-25);
    WY2 = 50;//(int)(inImage->SizeY()-30);
    
    WX3 = 15;//inImage->SizeX()-5;
    WY3 = 25;//75;
    
    float Wx, Wy;
    for(C1=0.0f ; C1<1.0f ; C1+=0.005f) {
        for(C2=0.0f ; C2<1.0f ; C2+=0.005f) {
            C3 = 1 - C1 - C2;
            if(C3>=0.0f) {
                Vx = (C1*VX1) + (C2*VX2) + (C3*VX3);
                Vy = (C1*VY1) + (C2*VY2) + (C3*VY3);
                outImage->ReadPixel((int)Vx,(int)Vy,r,g,b);
                
                Wx = (C1*WX1) + (C2*WX2) + (C3*WX3);
                Wy = (C1*WY1) + (C2*WY2) + (C3*WY3);
                ImageDeformada->DrawPixel((int)Wx,(int)Wy,r,g,b);
            }
        }
    }
    
    return *outImage;
}


// ****************************************************************************
void ConvertBlackAndWhite()
{
}

// ****************************************************************************
void GrayScale()
{
}

// ****************************************************************************
void negativo()
{
}

// ****************************************************************************
void zoomout()
{
}

// ****************************************************************************
void zoominQ() //Quadriculado
{
}

// ****************************************************************************
void zoominL() //Linear
{
}

// ****************************************************************************
void equalizahistograma()
{
}

// ****************************************************************************
void lapaciano()
{
}

// ****************************************************************************
void init(void)
{
    int r;
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    Image = new ImageClass();
    r = Image->Load("f5.bmp");  // Leitura da Imagem
    if (!r)
    {
        printf("Imagem não encontrada. Verifique o nome do Arquivo.\n");
        printf("Pressione ENTRE para encerrar.");
        getchar();
        exit(1);
    }
    else printf ("Imagem carregada!\n");
    
    NovaImagem = new ImageClass(Image->SizeX(), Image->SizeY());
    ImageDeformada = new ImageClass(Image->SizeX(), Image->SizeY());
    
    *NovaImagem = Deformacao(Image);
    
    NovaImagem->SetPos(Image->SizeX()+20, 0);
    ImageDeformada->SetPos(Image->SizeX()+240, 0);
    //ConvertBlackAndWhite();
}

// ****************************************************************************
void reshape( int w, int h )
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluOrtho2D(0,w,0,h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ****************************************************************************
void display( void )
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    Image->Display();
    NovaImagem->Display();
    ImageDeformada->Display();
    
    glutSwapBuffers();
}

// ****************************************************************************
void keyboard ( unsigned char key, int x, int y )
{
    
    switch ( key )
    {
        case 27:        // Termina o programa qdo
        exit ( 0 );   // a tecla ESC for pressionada
        break;
        case '2':
        ConvertBlackAndWhite();
        glutPostRedisplay(); // obrigatório para redesenhar a tela
        break;
        default:
        break;
    }
}

// ****************************************************************************
int main ( int argc, char** argv )
{
    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);
    
    glutInitWindowSize  ( 600, 350);
    glutCreateWindow    ( "Leitura de Imagens" );
    
    init ();
    
    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    
    glutMainLoop ( );
    return 0;
}
// ****************************************************************************
