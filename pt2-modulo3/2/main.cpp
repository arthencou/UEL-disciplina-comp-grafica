
/*
* Ex2 - Módulo 3 - Parte 2
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

#include "Fila.h"
#include "ImageClass.h"
#define LIMIAR 220


ImageClass *Image, *NovaImagem;
Fila objeto;


/** ***************************************************************************
* 1) Implemente o controle de qualidade na fabricação de arruelas. Descreva a
* sua solução.
**/
void ControleQualidade()
{
}

/** ***************************************************************************
* 2) Considere uma imagem segmentada em branco e preto. Considere que o fundo é
* representado pela cor branca e os objetos pela cor preta. Os objetos não estão
* encostados um no outro. Faça um programa que conte quantos objetos estão con-
* tidos na imagem e o total de pixels de cada objeto. Por fim pinte de verde o 
* menor objeto contido na imagem e de azul o maior objeto contido na imagem.
**/

int static countID = 0;
int countQtddPixel = 0;

void marcaObjeto(int **inMatriz, int x, int y) {
    inMatriz[x][y] = (0-countID);
    countQtddPixel++;
    
    for(int i=(x-1) ; i<=(x+1) ; i++) {
        for(int j=(y-1) ; j<=(y+1) ; j++) {
            if(inMatriz[i][j] == 0)
            marcaObjeto(inMatriz, i, j);
        }
    }
}

void buscaObjeto(int **inMatriz, int sizeX, int sizeY)
{
    int x, y;
    
    for(x=0 ; x<sizeX ; x++){
        for(y=0 ; y<sizeY ; y++){
            if(inMatriz[x][y]==0){
                countID++;
                countQtddPixel = 0;
                marcaObjeto(inMatriz, x, y);
                
                objeto.inserir(countID, countQtddPixel);
            }
        }
    }
}

ImageClass ContaObjeto(ImageClass *inImage)
{
    ImageClass *outImage  = new ImageClass((inImage->SizeX()), (inImage->SizeY()));
    
    int x, y;
    
    int **matrizCor;
    matrizCor = (int **)malloc(inImage->SizeX() * sizeof(int *));
    for(x=0 ; x<inImage->SizeX() ; x++)
    matrizCor[x] = (int*)malloc(inImage->SizeY() * sizeof(int));
    
    byte r,g,b;
    float media;
    
    for(x=0 ; x<inImage->SizeX() ; x++) {
        for(y=0 ; y<inImage->SizeY() ; y++) {
            inImage->ReadPixel(x,y,r,g,b);
            media = ((float)r+(float)g+(float)b )/3;
            matrizCor[x][y] = (int)media;
        }
    }
    
    buscaObjeto(matrizCor, inImage->SizeX(), inImage->SizeY());
    
    printf("\n\n-----------------------------\n\n");
    printf("maior: %d \n\n", objeto.buscaMaiorQttdPixel());
    printf("\n\nQuantidade de objetos: %d \n\n", countID);
    
    int maiorObjeto = objeto.buscaMaiorQttdPixel();
    int menorObjeto = objeto.buscaMenorQttdPixel();
    for(x=0 ; x<inImage->SizeX() ; x++) {
        for(y=0 ; y<inImage->SizeY() ; y++) {
            if(matrizCor[x][y]==(-menorObjeto))
                outImage->DrawPixel(x, y,0,255,0);
            else if(matrizCor[x][y]==(-maiorObjeto))
                outImage->DrawPixel(x, y,0,0,255);
            else if(matrizCor[x][y] < 0)
                outImage->DrawPixel(x, y,0,0,0);
            else
                outImage->DrawPixel(x, y,255,255,255);
        }
    }
    
    for(x=0 ; x<inImage->SizeX() ; x++)
    free(matrizCor[x]);
    free(matrizCor);
    
    return *outImage;
}

/** ***************************************************************************
* 3) Uma máquina produz peças com 1, com 2 e sem buracos. Utilizando processa-
* mento de imagem como podemos classificar estas peças?
**/
void ClassificadorPecas()
{
}

/** ***************************************************************************
* 4) Implemente os filtros no domínio do espaço: Média, Sobel, Mediana.
**/
void Media()
{
}

void Sobel()
{
}

void Mediana()
{
}

/** ***************************************************************************
* 5) Implemente a translação e rotação em uma image.
**/
void Translacao()
{
}

void Rotacao()
{
}

/** ***************************************************************************
* 6) Implemente a Deformação da imagem de acordo com a teoria apresentada.
**/
void Deformacao()
{
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
    r = Image->Load("objetos.bmp");  // Leitura da Imagem
    if (!r)
    {
        printf("Imagem não encontrada. Verifique o nome do Arquivo.\n");
        printf("Pressione ENTRE para encerrar.");
        getchar();
        exit(1);
    }
    else printf ("Imagem carregada!\n");
    
    NovaImagem = new ImageClass(Image->SizeX(), Image->SizeY());
    
    *NovaImagem = ContaObjeto(Image);
    NovaImagem->SetPos(Image->SizeX()+10, 0);
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
    
    glutInitWindowSize  ( 1280, 500);
    glutCreateWindow    ( "Leitura de Imagens" );
    
    init ();
    
    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    
    glutMainLoop ( );
    return 0;
}
// ****************************************************************************
 
