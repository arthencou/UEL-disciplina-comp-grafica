
/*
* Ex5 - Módulo 3 - Parte 2
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

#include "ImageClass.h"
#define LIMIAR 220

#define PI 3.14159265
#define HEIGHT 750
#define WIDTH 650

ImageClass *Image, *CopiaImage;
int angleDelta = 0;

/** ***************************************************************************
* 5) Implemente a translação e rotação em uma image.
**/
ImageClass Translacao(ImageClass *inImage,int deltaX, int deltaY)
{
    ImageClass *outImage = new ImageClass(inImage->SizeX(), inImage->SizeY());
    
    int x,y;
    byte r,g,b;
    for(x=0 ; x<inImage->SizeX() ; x++) {
        for(y=0 ; y<inImage->SizeY() ; y++) {
            inImage->ReadPixel(x,y,r,g,b);
            outImage->DrawPixel(x,y,r,g,b);
        }
    }
    
    outImage->SetPos(inImage->GetPosX()+deltaX, inImage->GetPosY()+deltaY);
    return *outImage;
}
// --------------
ImageClass Rotacao(ImageClass *inImage, ImageClass *positionImage, int angle)
{
    int size;
    size = (int)(sqrt((inImage->SizeX())*(inImage->SizeX()) + (inImage->SizeY())*(inImage->SizeY())));
    
    ImageClass *outImage = new ImageClass(size*2, size*2);
    
    
    int x,y;
    byte r,g,b;
    for(x=0 ; x<inImage->SizeX() ; x++) {
        for(y=0 ; y<inImage->SizeY() ; y++) {
            inImage->ReadPixel(x,y,r,g,b);
            
            int newX, newY;
            newX = (int)((x*cos((angle*PI)/180) - y*sin((angle*PI)/180)));
            newY = (int)((x*sin((angle*PI)/180) + y*cos((angle*PI)/180)));
            
            //newX = (int)((x*cos(angle) - y*sin(angle)));
            //newY = (int)((x*sin(angle) - y*cos(angle)));
            newX += size;
            newY += size;
            outImage->DrawPixel(newX,newY,r,g,b);
        }
    }
    
    //outImage->SetPos(positionImage->GetPosX(), positionImage->GetPosY());
    outImage->SetPos(inImage->GetPosX()+size/2, inImage->GetPosY()+size/2);
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
ImageClass copyImage(ImageClass *inImage)
{
    ImageClass *outImage = new ImageClass(inImage->SizeX(), inImage->SizeY());
    
    int x,y;
    byte r,g,b;
    for(x=0 ; x<inImage->SizeX() ; x++) {
        for(y=0 ; y<inImage->SizeY() ; y++) {
            inImage->ReadPixel(x,y,r,g,b);
            outImage->DrawPixel(x,y,r,g,b);
        }
    }
    return *outImage;
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
    
    Image->SetPos((HEIGHT/2)-(Image->SizeX()/2), (WIDTH/2)-(Image->SizeY()/2));
    
    CopiaImage = new ImageClass(Image->SizeX(), Image->SizeY());
    *CopiaImage = copyImage(Image);
    
    //NovaImagem = new ImageClass(Image->SizeX(), Image->SizeY());
    //NovaImagem->SetPos((HEIGHT/2)-(Image->SizeX()/2), (WIDTH/2)-(Image->SizeY()/2));
    
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
    //NovaImagem->Display();
    
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

void GerenciaTeclasEspeciais(int key, int mouseX, int mouseY)
{
    switch(key)
    {
        case GLUT_KEY_UP:
        *Image = Translacao(Image, 0,5);
        display();
        break;
        case GLUT_KEY_DOWN:
        *Image = Translacao(Image, 0,-5);
        display();
        break;
        case GLUT_KEY_RIGHT:
        *Image = Translacao(Image, 5,0);
        display();
        break;
        case GLUT_KEY_LEFT:
        *Image = Translacao(Image, -5,0);
        display();
        break;
        case GLUT_KEY_PAGE_UP:
        angleDelta += 30;
        *Image = Rotacao(CopiaImage, Image, angleDelta);
        display();
        break;
        case GLUT_KEY_PAGE_DOWN:
        angleDelta -= 30;
        *Image = Rotacao(CopiaImage, Image, angleDelta);
        display();
        break;
    }
}

// ****************************************************************************
int main ( int argc, char** argv )
{
    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);
    
    glutInitWindowSize  ( HEIGHT, WIDTH);
    glutCreateWindow    ( "Leitura de Imagens" );
    
    init ();
    
    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc(GerenciaTeclasEspeciais);
    
    glutMainLoop ( );
    return 0;
}
// ****************************************************************************
