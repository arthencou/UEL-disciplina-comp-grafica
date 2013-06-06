
/*
* Ex4 - Módulo 3 - Parte 2
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

#define LIMIAR 128

ImageClass *Image, *NovaImagem, *NovaImagem2, *NovaImagem3;
ImageClass *inImageSpray, *outImageSpray;

float intensidadeYIQ = 0.0f;
int vetorHist[256];

// [ORIGINAL] -> [NC (HISTOGRAMA)] -> [EQUALIZADA Y] -> [COLORIDO EQUALIZADO]

// ****************************************************************************
void ConvertBlackAndWhite()
{
    int x,y;
    
    for(x=0;x<Image->SizeX();x++)
    {
        for(y=0;y<Image->SizeY();y++)
        {
            if (Image->GetPointIntensity(x,y) < LIMIAR)
            {
                NovaImagem->DrawPixel(x, y,0,0,0);
            }
            else NovaImagem->DrawPixel(x, y,255,255,255);
        }
    }
    
};

// ****************************************************************************
ImageClass GrayScale(ImageClass *inImage)
{
    ImageClass *outImage = new ImageClass(inImage->SizeX(), inImage->SizeY());
    
    int x,y;
    byte r,g,b;
    double nc;
    
    for(x=0;x<inImage->SizeX();x++)
    {
        for(y=0;y<inImage->SizeY();y++)
        {
            inImage->ReadPixel(x,y,r,g,b);
            nc = (r+g+b) / 3;
            //if( (nc+intensidadeGrayScale)>= 0 && (nc+intensidadeGrayScale)< 256)
            //{
                //nc = nc+intensidadeGrayScale;
                outImage->DrawPixel(x,y,(byte)nc,(byte)nc,(byte)nc);
            //}
        }
    }
    
    return *outImage;
}

// ****************************************************************************
ImageClass SistemaYIQ(ImageClass *inImage)
{
    ImageClass *outImage = new ImageClass(inImage->SizeX(), inImage->SizeY());
    
    int x, y;
    byte r, g, b;
    float Yy, Ii, Qq;
    float R, G, B;
    
    for(x=0;x<inImage->SizeX();x++)
    {
        for(y=0;y<inImage->SizeY();y++)
        {
            inImage->ReadPixel(x,y,r,g,b);
            
            R = ((float)r)/255;
            G = ((float)g)/255;
            B = ((float)b)/255;
            
            Yy = 0.299*R + 0.587*G + 0.114*B;
            Ii = 0.596*R - 0.275*G - 0.321*B;
            Qq = 0.212*R - 0.523*G + 0.311*B;
            
            Yy += intensidadeYIQ;
            
            R = Yy + 0.956*Ii + 0.620*Qq;
            G = Yy - 0.272*Ii - 0.647*Qq;
            B = Yy - 1.108*Ii + 1.705*Qq;
            
            R = (R<0)?(0.0):((R>1.00)?(1.0):(R));
            G = (G<0)?(0.0):((G>1.00)?(1.0):(G));
            B = (B<0)?(0.0):((B>1.00)?(1.0):(B));
            
            r = (byte)(R*255);
            g = (byte)(G*255);
            b = (byte)(B*255);
            
            outImage->DrawPixel(x,y,r,g,b);
        }
    }
    
    return *outImage;
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
// * HISTOGRAMA ***************************************************************
// ****************************************************************************
ImageClass histogram(ImageClass *imageIn)
{
    ImageClass *imageOut = new ImageClass(imageIn->SizeX(), imageIn->SizeY());
    
    int x, y;
    byte r,g,b;
    float soma;
    int N=0;
    int newVetorHist[256];
    
    for(x=0 ; x<256 ; x++) {
        vetorHist[x] = 0;
        newVetorHist[x] = 0;
    }
    
    for(x=0;x<imageIn->SizeX();x++)
    {
        for(y=0;y<imageIn->SizeY();y++)
        {
            imageIn->ReadPixel(x,y,r,g,b);
            
            soma = ( (float)r+(float)g+(float)b ) / 3;
            vetorHist[(int)soma] += 1;
            N += 1;
        }
    }
    
    int k;
    for(k=0 ; k<256 ; k++) {
        soma = 0;
        for(x=0 ; x<k ; x++){
            soma += vetorHist[x];
        }
        
        newVetorHist[k] = (int)(soma*(255.0f / N));
    }
    
    for(x=0;x<imageIn->SizeX();x++)
    {
        for(y=0;y<imageIn->SizeY();y++)
        {
            imageIn->ReadPixel(x,y,r,g,b);
            soma = ( (float)r+(float)g+(float)b ) / 3;
            imageOut->DrawPixel(x,y,(byte)newVetorHist[(int)soma],(byte)newVetorHist[(int)soma],(byte)newVetorHist[(int)soma]);
        }
    }
    
    return *imageOut;
}
// ****************************************************************************

// ****************************************************************************
void equalizahistograma()
{
}

// ****************************************************************************
void media()
{
}

// ****************************************************************************
void lapaciano()
{
}

// ****************************************************************************
ImageClass sobel(ImageClass *inImage)
{
    ImageClass *outImage = new ImageClass(inImage->SizeX(), inImage->SizeY());
    
    int x, y;
    byte r,g,b;
    float media, somaR1, somaR2, result;
    
    for(x=1 ; x<(inImage->SizeX()-2) ; ++x) {
        for(y=1 ; y<(inImage->SizeY()-2) ; ++y) {
            
            inImage->ReadPixel(x-1,y-1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            somaR1 = media*(-1);
            somaR2 = media*(-1);
            
            inImage->ReadPixel(x,y-1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            somaR1 += media*(-2);
            somaR2 += media*(0);
            
            inImage->ReadPixel(x+1,y-1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            somaR1 += media*(-1);
            somaR2 += media*(1);
            
            inImage->ReadPixel(x-1,y,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            somaR1 += media*(0);
            somaR2 += media*(-2);
            
            inImage->ReadPixel(x,y,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            somaR1 += media*(0);
            somaR2 += media*(0);
            
            inImage->ReadPixel(x+1,y,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            somaR1 += media*(0);
            somaR2 += media*(2);
            
            inImage->ReadPixel(x-1,y+1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            somaR1 += media*(1);
            somaR2 += media*(-1);
            
            inImage->ReadPixel(x,y+1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            somaR1 += media*(2);
            somaR2 += media*(0);
            
            inImage->ReadPixel(x+1,y+1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            somaR1 += media*(1);
            somaR2 += media*(1);
            
            result = sqrt((somaR1*somaR1) + (somaR2*somaR2));
            
            if(result >= LIMIAR)
            outImage->DrawPixel(x,y,(byte)0.0,(byte)0.0,(byte)0.0);
            else
            outImage->DrawPixel(x,y,(byte)255.0,(byte)255.0,(byte)255.0);
            //outImage->DrawPixel(x,y,(byte)result,(byte)result,(byte)result);
        }
    }
    
    return *outImage;
}

// ****************************************************************************
ImageClass mediana(ImageClass *inImage)
{
    ImageClass *outImage = new ImageClass(inImage->SizeX(), inImage->SizeY());
    
    int x, y;
    byte r,g,b;
    float ordena[9];
    
    for(x=1 ; x<(inImage->SizeX()-2) ; ++x) {
        for(y=1 ; y<(inImage->SizeY()-2) ; ++y) {
            inImage->ReadPixel(x-1,y-1,r,g,b);
            ordena[0] = ((float)r + (float)g + (float)b) / 3;
            
            inImage->ReadPixel(x,y-1,r,g,b);
            ordena[1] = ((float)r + (float)g + (float)b) / 3;
            
            inImage->ReadPixel(x+1,y-1,r,g,b);
            ordena[2] = ((float)r + (float)g + (float)b) / 3;
            
            inImage->ReadPixel(x-1,y,r,g,b);
            ordena[3] = ((float)r + (float)g + (float)b) / 3;
            
            inImage->ReadPixel(x,y,r,g,b);
            ordena[4] = ((float)r + (float)g + (float)b) / 3;
            
            inImage->ReadPixel(x+1,y,r,g,b);
            ordena[5] = ((float)r + (float)g + (float)b) / 3;
            
            inImage->ReadPixel(x-1,y+1,r,g,b);
            ordena[6] = ((float)r + (float)g + (float)b) / 3;
            
            inImage->ReadPixel(x,y+1,r,g,b);
            ordena[7] = ((float)r + (float)g + (float)b) / 3;
            
            inImage->ReadPixel(x+1,y+1,r,g,b);
            ordena[8] = ((float)r + (float)g + (float)b) / 3;
            
            int w, z;
            float aux;
            for(w=0 ; w<9 ; w++) {
                for(z=0 ; z<8 ; z++) {
                    if(ordena[z] > ordena[z+1]) {
                        aux = ordena[z];
                        ordena[z] = ordena[z+1];
                        ordena[z+1] = aux;
                    }
                }
            }
            
            outImage->DrawPixel(x,y,(byte)ordena[4],(byte)ordena[4],(byte)ordena[4]);
        }
    }
    
    return *outImage;
}

// ****************************************************************************
ImageClass filtragemEspacial(ImageClass *inImage)
{
    ImageClass *outImage = new ImageClass(inImage->SizeX(), inImage->SizeY());
    
    int x, y;
    byte r,g,b;
    float media, soma;
    
    for(x=1 ; x<(inImage->SizeX()-2) ; ++x) {
        for(y=1 ; y<(inImage->SizeY()-2) ; ++y) {
            
            inImage->ReadPixel(x-1,y-1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            soma = media;
            
            inImage->ReadPixel(x,y-1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            soma += media;
            
            inImage->ReadPixel(x+1,y-1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            soma += media;
            
            inImage->ReadPixel(x-1,y,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            soma += media;
            
            inImage->ReadPixel(x,y,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            soma += media;
            
            inImage->ReadPixel(x+1,y,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            soma += media;
            
            inImage->ReadPixel(x-1,y+1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            soma += media;
            
            inImage->ReadPixel(x,y+1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            soma += media;
            
            inImage->ReadPixel(x+1,y+1,r,g,b);
            media = ((float)r + (float)g + (float)b) / 3;
            soma += media;
            
            soma = soma/9;
            outImage->DrawPixel(x,y,(byte)soma,(byte)soma,(byte)soma);
        }
    }
    
    return *outImage;
}

// ****************************************************************************
// INIT ***********************************************************************
// ****************************************************************************
void init(void)
{
    int r;
    
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto
    
    // Cria um objeto imagem
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
    
    // Cria um objeto imagem
    inImageSpray = new ImageClass();
    
    r = inImageSpray->Load("spray.bmp");  // Leitura da Imagem
    if (!r)
    {
        printf("Imagem não encontrada. Verifique o nome do Arquivo.\n");
        printf("Pressione ENTRE para encerrar.");
        getchar();
        exit(1);
    }
    else printf ("Imagem 2 carregada!\n");
    
    //*inImageSpray, outImageSpray;
    
    // Instacia o objeto que irá exibir a nova imagem
    // Caso precise alterar o tamanho da nova imagem, mude os parâmetros
    // da construtura, na chamada abaixo
    NovaImagem = new ImageClass(Image->SizeX(), Image->SizeY());
    NovaImagem2 = new ImageClass(Image->SizeX(), Image->SizeY());
    NovaImagem3 = new ImageClass(Image->SizeX(), Image->SizeY());
    
    outImageSpray = new ImageClass(inImageSpray->SizeX(), inImageSpray->SizeY());
    
    *NovaImagem = GrayScale(Image);//histogram(Image);
    *NovaImagem2 = filtragemEspacial(NovaImagem);//SistemaYIQ(Image);
    *NovaImagem3 = sobel(NovaImagem);
    
    *outImageSpray = mediana(inImageSpray);
    
    // Posiciona a imagem nova ao lado da antiga
    NovaImagem->SetPos(Image->SizeX()+5, 0);
    NovaImagem2->SetPos(Image->SizeX()+170, 0);
    NovaImagem3->SetPos(Image->SizeX()+340, 0);
    
    inImageSpray->SetPos(0, Image->SizeY()+5);
    //outImageSpray->SetPos(Image->SizeX()+285, Image->SizeY()+5);
    outImageSpray->SetPos(inImageSpray->SizeX()+285, Image->SizeY()+5);
}

// ****************************************************************************
void reshape( int w, int h )
{
    
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    gluOrtho2D(0,w,0,h);
    
    // Set the clipping volume
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
    NovaImagem2->Display();
    NovaImagem3->Display();
    
    inImageSpray->Display();
    outImageSpray->Display();
    
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

void specialKeyboard(int key, int mouseX, int mouseY)
{
    switch(key)
    {
        /*case GLUT_KEY_UP:
        intensidadeGrayScale += 5.0f;
        //GrayScale();
        *NovaImagem = GrayScale(Image);
        NovaImagem->SetPos(0, +245);
        display();
        break;
        case GLUT_KEY_DOWN:
        intensidadeGrayScale -= 5.0f;
        //GrayScale();
        *NovaImagem = GrayScale(Image);
        NovaImagem->SetPos(0, +245);
        display();
        break;*/
        case GLUT_KEY_LEFT:
        /*intensidadeYIQ += 0.01;
        //SistemaYIQ();
        *NovaImagem2 = SistemaYIQ(Image);
        *NovaImagem2 = histogram(NovaImagem2);
        NovaImagem2->SetPos(Image->SizeX()+10, 5);
        display();*/
        break;
        case GLUT_KEY_RIGHT:
        /*intensidadeYIQ -= 0.01;
        *NovaImagem2 = SistemaYIQ(Image);
        *NovaImagem2 = histogram(NovaImagem2);
        NovaImagem2->SetPos(Image->SizeX()+10, 5);
        display();*/
        break;
    }
}

// ****************************************************************************
//  void main ( int argc, char** argv ) ***************************************
// ****************************************************************************
int main ( int argc, char** argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);
    
    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize( 1280, 500);
    glutCreateWindow( "Leitura de Imagens" );
    
    init ();
    
    printf("\n\n\n[Figura 5 e 6 (Spray) - Mediana]\n\n\n[Figura 1 - Original] [Figura 2 - Cinza] [Figura 3 - Filtro da Media] [Figura 4 - Sobel]");
    //printf("\n\n[KEY_LEFT] - Aumenta a Intensidade YIQ");
    //    printf("\n[KEY_RIGHT] - Diminui a Intensidade YIQ");
    
    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc( specialKeyboard );
    
    glutMainLoop ( );
    return 0;
}
// ****************************************************************************
