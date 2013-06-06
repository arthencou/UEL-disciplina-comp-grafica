
/*
* Ex3 - Módulo 3 - Parte 2
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

ImageClass *Image;//, *NovaImagem;

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
* tidos na imagem e o total de pixels de cada objeto. Por fim pinte de vermelho
* o maior objeto contido na imagem.
**/
void ContaObjeto()
{
}

/** ***************************************************************************
* 3) Uma máquina produz peças com 1, com 2 e sem buracos. Utilizando processa-
* mento de imagem como podemos classificar estas peças?
**/
int static countObj = 0;

void contaBuraco(int **inMatriz, int x, int y)
{
    inMatriz[x][y] = (-countObj);
    int i, j;
    for(i=(x-1) ; i<=(x+1) ; i++) {
        for(j=(y-1) ; j<=(y+1) ; j++) {
            //printf("\n -- %d = %d --", i, j);
            if(inMatriz[i][j]==255) {
                
                contaBuraco(inMatriz, i, j);
            }
        }
    }
}

void ClassificadorPecas(ImageClass *inImage)
{
    //ImageClass *outImage  = new ImageClass((inImage->SizeX())+2, (inImage->SizeY())+2);
    
    int **matrizObj;
    int x, y;
    matrizObj = (int **)malloc((inImage->SizeX()+2) * sizeof(int *));
    for(x=0 ; x<=inImage->SizeX()+1 ; x++)
    matrizObj[x] = (int*)malloc((inImage->SizeY()+2) * sizeof(int));
    
    //printf("\n- %d ; %d -",inImage->SizeX()+2,inImage->SizeY()+2);
    byte r,g,b;
    float media;
    for(x=0 ; x<=inImage->SizeX()+1 ; x++) {
        for(y=0 ; y<=inImage->SizeY()+1 ; y++) {
            matrizObj[x][y] = 0;
        }
        
    }
    
    for(x=1 ; x<=inImage->SizeX() ; x++) {
        for(y=1 ; y<=inImage->SizeY() ; y++) {
            inImage->ReadPixel(x-1,y-1,r,g,b);
            media = ((float)r+(float)g+(float)b )/3;
            matrizObj[x][y] = (int)media;
        }
        
    }
    for(x=1 ; x<=inImage->SizeX() ; x++) {
        for(y=1 ; y<=inImage->SizeY() ; y++) {
            if(matrizObj[x][y]==255) {
                countObj++;
                contaBuraco(matrizObj, x, y);
            }
        }
    }
    
    printf("\n\n ---------------------------------------");
    printf("\n\n Quantidade de Buracos: %d",countObj-1);
    
    for(x=0 ; x<inImage->SizeX() ; x++)
    free(matrizObj[x]);
    free(matrizObj);
    
    //return *outImage;
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
    
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    Image = new ImageClass();
    
//r = Image->Load("objeto1.bmp");
//r = Image->Load("objeto2.mp");
//r = Image->Load("objeto3.bmp");
r = Image->Load("objeto4.bmp");
    
    if (!r)
    {
        printf("Imagem não encontrada. Verifique o nome do Arquivo.\n");
        printf("Pressione ENTRE para encerrar.");
        getchar();
        exit(1);
    }
    else printf ("Imagem carregada!\n");
    
    //NovaImagem = new ImageClass(Image->SizeX(), Image->SizeY());
    //*NovaImagem = ClassificadorPecas(Image);
    //NovaImagem->SetPos(Image->SizeX()+10, 0);
    ClassificadorPecas(Image);
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

// ****************************************************************************
int main ( int argc, char** argv )
{
    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);
    
    glutInitWindowSize  ( 350, 300);
    glutCreateWindow    ( "Leitura de Imagens" );
    
    init ();
    
    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    
    glutMainLoop ( );
    return 0;
}
// ****************************************************************************
 
