
/*
* Ex1 - Módulo 3 - Parte 2
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

ImageClass *Image, *NovaImagem;

/** ***************************************************************************
* 1) Implemente o controle de qualidade na fabricação de arruelas. Descreva a
* sua solução.
**/
ImageClass ControleQualidade(ImageClass *inImage)
{
    ImageClass *outImage;
    
    int x, y, altura1, altura2, largura1, largura2, height, width;
    byte r, g, b;
    
    for(x=0 ; x<inImage->SizeX() ; x++){
        for(y=0 ; y<inImage->SizeY() ; y++) {
            inImage->ReadPixel(x,y,r,g,b);
            if( (((float)r+(float)g+(float)b)/3)<255 ){
                largura1 = x;
                x = inImage->SizeX();
                y = inImage->SizeY();
            }
        }
    }
    for(x=(inImage->SizeX()-1) ; x>=0 ; x--){
        for(y=0 ; y<inImage->SizeY() ; y++) {
            inImage->ReadPixel(x,y,r,g,b);
            if( (((float)r+(float)g+(float)b)/3)<255 ){
                largura2 = x;
                x = -1;
                y = inImage->SizeY();
            }
        }
    }
    width = largura2 - largura1;
    printf("\nLargura: %d", width);
    
    for(y=0 ; y<inImage->SizeY() ; y++) {
        for(x=0 ; x<inImage->SizeX() ; x++){
            inImage->ReadPixel(x,y,r,g,b);
            if( (((float)r+(float)g+(float)b)/3)<255 ){
                altura1 = y;
                x = inImage->SizeX();
                y = inImage->SizeY();
            }
        }
    }
    for(y=(inImage->SizeY()-1); y>=0 ; y--) {
        for(x=0 ; x<inImage->SizeX() ; x++) {
            inImage->ReadPixel(x,y,r,g,b);
            if( (((float)r+(float)g+(float)b)/3)<255 ){
                altura2 = y;
                y = -1;
                x = inImage->SizeX();
            }
        }
    }
    height = altura2 - altura1;
    printf("\t\tAltura: %d", height);
    
    int raio;
    if(height > width) {
        if(height%2!=0) {
            height++;
        }
        raio = height / 2;
        outImage = new ImageClass(height, height);
        for(x=largura1 ; x<=(height+largura1) ; x++) {
            for(y=altura1 ; y<=(height+altura1) ; y++) {
                inImage->ReadPixel(x,y,r,g,b);
                outImage->DrawPixel(x-largura1, y-altura1,r,g,b);
            }
        }
        } else {
        if(width%2!=0) {
            width++;
        }
        raio = width / 2;
        outImage = new ImageClass(width, width);
        for(x=largura1 ; x<=(width+largura1) ; x++) {
            for(y=altura1 ; y<=(width+altura1) ; y++) {
                inImage->ReadPixel(x,y,r,g,b);
                outImage->DrawPixel(x-largura1, y-altura1,r,g,b);
            }
        }
    }
    printf("\n\nRaio: %d", raio);
    printf("\nx: %d ; y: %d", (largura1+raio),(altura1+raio));
    
    
    double precisaoY;
    int countPixelRed = 0;
    
    byte r1, g1, b1, r2, g2, b2;
    int meiaCircunferencia1[raio*2+1][raio], meiaCircunferencia2[raio*2+1][raio];
    for(x=0 ; x<raio*2+1 ; x++) {
        for(y=0 ; y<raio ; y++) {
            meiaCircunferencia1[x][y] = 0;
            meiaCircunferencia2[x][y] = 0;
        }
    }
    for(precisaoY=0.0f ; precisaoY<raio ; precisaoY = precisaoY + 0.05f)
    {
        for(x=0 ; x<(raio*2) ; x++)
        {
            // Parte positiva da circunferencia
            double circYSup = sqrt( (precisaoY)*(precisaoY) - (x - raio)*(x - raio) ) + raio;
            outImage->ReadPixel(x, abs((int)circYSup),r1,g1,b1);
            if((int)circYSup >= 0 && (((float)r1 + (float)g1 + (float)b1)/3)<=200.0f) {
                outImage->DrawPixel(x, abs((int)circYSup),255,0,0);
                countPixelRed ++;
            }
            
            // Parte negativa da circunferencia
            double circYInf = -(sqrt( (precisaoY)*(precisaoY) - (x - raio)*(x - raio) )) + raio;
            outImage->ReadPixel(x, abs((int)circYInf),r2,g2,b2);
            
            if((int)circYInf >= 0 && (((float)r2 + (float)g2 + (float)b2)/3)<=200.0f) {
                outImage->DrawPixel(x, abs((int)circYInf),255,0,0);
                countPixelRed ++;
            }
            
            outImage->ReadPixel(x, abs((int)circYSup),r1,g1,b1);
            outImage->ReadPixel(x, abs((int)circYInf),r2,g2,b2);
            if(x<=raio) {
                if(r1!=r2 || g1!=g2 || b1!=b2) {
                    meiaCircunferencia1[x][(int)circYSup] += 1;
                    meiaCircunferencia1[x][(int)circYInf] += 1;
                }
                } else{
                if(r1!=r2 || g1!=g2 || b1!=b2) {
                    meiaCircunferencia2[x][(int)circYSup] += 1;
                    meiaCircunferencia2[x][(int)circYInf] += 1;
                }
            }
        }
    }
    
    int countDiferente = 0;
    for(x=0 ; x<raio*2+1 ; x++) {
        for(y=0 ; y<raio ; y++) {
            if(meiaCircunferencia1[x][y]!=meiaCircunferencia2[x][raio-y]) {
                countDiferente += 1;
            }
        }
    }
    float porcentagem = ((countDiferente*100)/((raio*2)*(raio*2)));
    printf("\n\n%d (%.5f porcento) pixels estao diferentes", countDiferente, porcentagem);
    
    int raioInterno;
    x = 1;
    outImage->ReadPixel(raio+1, raio,r,g,b);
    while((((float)r + (float)g + (float)b)/3)>=128.0f) {
        x++;
        outImage->ReadPixel(raio+x, raio,r,g,b);
    }
    raioInterno = x;
    
    printf("\n\nRaio Externo: %d \nRaio Interno: %d", raio, raioInterno);
    
    outImage->DrawPixel(raio, raio,255,0,0);
    return *outImage;
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
    
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    Image = new ImageClass();
    r = Image->Load("arruela.bmp");  // Leitura da Imagem
    if (!r)
    {
        printf("Imagem não encontrada. Verifique o nome do Arquivo.\n");
        printf("Pressione ENTRE para encerrar.");
        getchar();
        exit(1);
    }
    else printf ("Imagem carregada!\n");
    
    NovaImagem = new ImageClass(Image->SizeX(), Image->SizeY());
    *NovaImagem = ControleQualidade(Image);
    NovaImagem->SetPos(Image->SizeX()+10, 10);
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
    
    glutInitWindowSize  ( 500, 500);
    glutCreateWindow    ( "Leitura de Imagens" );
    
    init ();
    
    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    
    glutMainLoop ( );
    return 0;
}
// ****************************************************************************
