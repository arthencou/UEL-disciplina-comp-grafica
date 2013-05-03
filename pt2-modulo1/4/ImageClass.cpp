//    obs: para compilar inclua as seguintes bibliotecas
//            -lglaux  -lopengl32 -lglut32 -lglu32
// **********************************************************************
// PUCRS/FACIN - Falar sobre o Lançamento do Livro .
// COMPUTAÇÃO GRÁFICA
//obs: O programa original desenvolvido pelo autor foi alterado
// **********************************************************************
//
#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif
#include <GL\gl.h>
#include "glaux.h"
#include "glut.h"
#include "ImageClass.h"

#define TAM_PIXEL 3

// **********************************************************************
//
//    Constructor
// **********************************************************************
ImageClass::ImageClass()
{
    Image = NULL;
    PosX = PosY = 0;
    // NOVO
    // Isto resolve o problema de ter a imagem com
    // largura múltipla de 4
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    zoomH = zoomV = 1;
}


// **********************************************************************
//
//    Constructor
// **********************************************************************
ImageClass::ImageClass(int sizeX, int sizeY)
{
    unsigned int tam;
    
    // Aloca área para o apontador
    Image = new(AUX_RGBImageRec);
    Image->sizeX = sizeX;
    Image->sizeY = sizeY;
    
    tam = sizeof(unsigned char) * sizeX * sizeY * TAM_PIXEL;
    Image->data = (unsigned char *) malloc (tam);
    memset(Image->data,255,tam);
    
    PosX = PosY = 0;
    zoomH = zoomV = 1;
    
    // NOVO
    // Isto resolve o problema de ter a imagem com
    // largura múltipla de 4
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
}

// **********************************************************************
//
//
// **********************************************************************
void ImageClass::SetPos(int X, int Y)
{
    PosX = X;
    PosY = Y;
}
BYTE *JPEGLoad(char *filename, unsigned int *width, unsigned int *height);

// **********************************************************************
//
//
// **********************************************************************

int ImageClass::Load(char * nome)
{
    unsigned int x,y;
    if ( nome[strlen(nome)-3] == 'b' || nome[strlen(nome)-3] == 'B'){
        if (!Image){
            // Load the bitmap and assign our pointer to it
            Image = auxDIBImageLoad(nome);
            if (Image)
            return 1;
            else return 0;
        }
        else{
            free(Image->data); // é necessário desalocar a área da imagem tb
            free(Image);
            Image = auxDIBImageLoad(nome);
        }
    }
    printf ("ImageClass:Imagem inexistente !\n");
    return 0;
}


// **********************************************************************
//
//
// **********************************************************************
void ImageClass::Display()
{
    glPixelZoom(zoomH , zoomV);
    glRasterPos2f(PosX, PosY);
    glDrawPixels(Image->sizeX, Image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
    //    glDrawPixels(Image->sizeX, Image->sizeY, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image->data);
}


// **********************************************************************
//
//
// **********************************************************************
void ImageClass::Delete()
{
    // Cleanup
    if (Image)
    {
        if (Image->data)
        free(Image->data);
        
        free(Image);
    }
    
}
// **********************************************************************
//
//
// **********************************************************************
void ImageClass::DrawPixel(GLint x, GLint y, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned long addr;
    
    addr = (unsigned long)( y *(Image->sizeX)* TAM_PIXEL + x * TAM_PIXEL );
    Image->data[addr++] = r;
    Image->data[addr++] = g;
    Image->data[addr] = b;
}
// **********************************************************************
//
//
//
// **********************************************************************
void ImageClass::DrawLineH(int y, int x1, int x2,unsigned char r, unsigned char g, unsigned char b )
{
    int x;
    if (x1 <= x2)
    for (x = x1; x<=x2; x++)
    {
        DrawPixel(x,y,r,g,b);
    }
    else
    for (x = x2; x<=x1; x++)
    {
        DrawPixel(x,y,r,g,b);
    }
    
}
// **********************************************************************
//
//
//
// **********************************************************************
void ImageClass::DrawLineV(int x, int y1, int y2,unsigned char r, unsigned char g, unsigned char b )
{
    int y;
    for (y = y1; y<=y2; y++)
    {
        DrawPixel(x,y,r,g,b);
    }
}

// **********************************************************************
//
//
// **********************************************************************
void ImageClass::ReadPixel(GLint x, GLint y, unsigned char &r, unsigned char &g, unsigned char &b)
{
    unsigned long addr;
    
    addr = (unsigned long)( y *(Image->sizeX)* TAM_PIXEL + x * TAM_PIXEL );
    r = Image->data[addr++];
    g = Image->data[addr++];
    b = Image->data[addr];
}


// **********************************************************************
//
//
//
// **********************************************************************
double ImageClass::GetPointIntensity(int x, int y)
{
    unsigned char r,g,b;
    double i;
    
    ReadPixel(x,y,r,g,b);
    i = (0.3 * r + 0.59 * g + 0.11 * b);
    return i;
}


void ImageClass::CopyTo(ImageClass *i)
{
    
    unsigned int tam;
    
    tam = sizeof(unsigned char) * Image->sizeX * Image->sizeY * TAM_PIXEL;
    memcpy(i->Image->data, Image->data,tam);
    
}

void ImageClass::Clear()
{
    unsigned int tam;
    tam = sizeof(unsigned char) * SizeX() * SizeY() * TAM_PIXEL;
    memset(Image->data,255,tam);
    
}
