/*
* Ex3 - Módulo 1 - Parte 2
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

using namespace std;

ImageClass *Image, *NovaImagem;


// **********************************************************************
// Funcionalidades dos exercícios
// **********************************************************************

#define LIMIAR 220 //100 //200
void ConvertBlackAndWhite()
{
    // Tarefa 1:
    //        Mude o valor do LIMIAR e veja os resultados
    
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

void GrayScale()
{
    // Tarefa 2:
    //        Gerar uma imagem em tons de cinza
    //        Use os métodos
    //            'ReadPixel' e 'DrawPixel' da classe 'ImageClass'
    
    //Image->ReadPixel(GLint x, GLint y, unsigned char &r, unsigned char &g, unsigned char &b)
    // Obtém os valores R,G,B de um ponto X,Y da image
    
    //Image->DrawPixel(GLint x, GLint y, unsigned char r, unsigned char g, unsigned char b)
    // Exibe na coordenada X,Y um ponto com a cor R,G,B na imagem
    int x,y;
    unsigned char r, g, b;
    for(x=0; x<Image->SizeX(); x++)
    {
        for(y=0; y<Image->SizeY(); y++)
        {
			Image->ReadPixel(x, y, r, g, b);
			unsigned char gs = (r+g+b)/3;
			NovaImagem->DrawPixel(x, y, gs, gs, gs);
        }
    }
}

void negativo()
{
	int x,y;
    unsigned char r, g, b;
    for(x=0; x<Image->SizeX(); x++)
    {
        for(y=0; y<Image->SizeY(); y++)
        {
			Image->ReadPixel(x, y, r, g, b);
			r = 255 - r;
			g = 255 - g;
			b = 255 - b;
			NovaImagem->DrawPixel(x, y, r, g, b);
        }
    }
}

void zoomout()
{
}
void zoominQ() //Quadriculado
{
}

void zoominL() //Linear
{
}

void EqualizaHistograma()
{
}

void media()
{
}
void lapaciano()
{
}
void sobel()
{
}
void mediana()
{
}

// **********************************************************************
// Funcionalidades adicionais
// **********************************************************************

enum ModoAtual
{
	BLACKANDWHITE,
	GRAYSCALE,
	NEGATIVO,
	ZOOMOUT,
	ZOOMINQ,
	ZOOMINL,
	EQUALIZAHISTOGRAMA,
	MEDIA,
	LAPACIANO,
	SOBEL,
	MEDIANA
};

ModoAtual modo_atual = BLACKANDWHITE;

void AplicarModo(ModoAtual modo)
{
	modo_atual = modo;

	switch(modo)
	{
		case BLACKANDWHITE:
		ConvertBlackAndWhite();
		break;

		case GRAYSCALE:
		GrayScale();
		break;

		case NEGATIVO:
		negativo();
		break;

		case ZOOMOUT:
		zoomout();
		break;

		case ZOOMINQ:
		zoominQ();
		break;

		case ZOOMINL:
		zoominL();
		break;

		case EQUALIZAHISTOGRAMA:
		EqualizaHistograma();
		break;

		case MEDIA:
		media();
		break;

		case LAPACIANO:
		lapaciano();
		break;

		case SOBEL:
		sobel();
		break;

		case MEDIANA:
		mediana();
		break;
	}

	glutPostRedisplay();
}

double **Y = NULL;
double **I = NULL;
double **Q = NULL;
void fillYIQ()
{
	int sizeX = Image->SizeX();
	int sizeY = Image->SizeY();

	Y = new double*[sizeX];
	I = new double*[sizeX];
	Q = new double*[sizeX];
	for(int i=0; i<sizeX; i++ )
	{
		Y[i] = new double[sizeY];
		I[i] = new double[sizeY];
		Q[i] = new double[sizeY];

		for( int j=0; j<sizeY; j++ )
		{
			unsigned char r, g, b;
			Image->ReadPixel( i, j, r, g, b );
			Y[i][j] = 0.299*r + 0.587*g + 0.114*b;
			I[i][j] = 0.71*(r-Y[i][j]) - 0.27*(b-Y[i][j]);
			Q[i][j] = 0.48*(r-Y[i][j]) + 0.41*(b-Y[i][j]);
		}
	}

}

double br_level = 0.0;
void AumentarBrilho()
{
	if( br_level == 120.0 ) return;
	br_level += 20.0;

	int sizeX = Image->SizeX();
	int sizeY = Image->SizeY();

	unsigned char r, g, b;
	double _y, _i, _q;
	for( int x=0; x<sizeX; x++ )
	{
		for( int y=0; y<sizeY; y++ )
		{
			_y = Y[x][y] + br_level;
			_i = I[x][y];
			_q = Q[x][y];

			double aux;
			aux = _y + 0.956*_i + 0.620*_q;
			r = ( aux > 255) ? 255 : ( ( aux < 0) ? 0 : (unsigned char)aux );
			aux = _y - 0.272*_i - 0.647*_q;
			g = ( aux > 255) ? 255 : ( ( aux < 0) ? 0 : (unsigned char)aux );
			aux = _y - 1.108*_i + 1.705*_q;
			b = ( aux > 255) ? 255 : ( ( aux < 0) ? 0 : (unsigned char)aux );
			Image->DrawPixel(x, y, r, g, b);
		}
	}

	AplicarModo(modo_atual);
}

void DiminuirBrilho()
{
	if( br_level == -120.0 ) return;
	br_level -= 20.0;

	int sizeX = Image->SizeX();
	int sizeY = Image->SizeY();

	unsigned char r, g, b;
	double _y, _i, _q;
	for( int x=0; x<sizeX; x++ )
	{
		for( int y=0; y<sizeY; y++ )
		{
			_y = Y[x][y] + br_level;
			_i = I[x][y];
			_q = Q[x][y];

			double aux;
			aux = _y + 0.956*_i + 0.620*_q;
			r = ( aux > 255) ? 255 : ( ( aux < 0) ? 0 : (unsigned char)aux );
			aux = _y - 0.272*_i - 0.647*_q;
			g = ( aux > 255) ? 255 : ( ( aux < 0) ? 0 : (unsigned char)aux );
			aux = _y - 1.108*_i + 1.705*_q;
			b = ( aux > 255) ? 255 : ( ( aux < 0) ? 0 : (unsigned char)aux );
			Image->DrawPixel(x, y, r, g, b);
		}
	}

	AplicarModo(modo_atual);
}


// **********************************************************************
//  void init(void)
//        Inicializa os parâmetros globais de OpenGL
//      Cria os objetos que representam as imagens
//
// **********************************************************************
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
    
    // Instacia o objeto que irá exibir a nova imagem
    // Caso precise alterar o tamanho da nova imagem, mude os parâmetros
    // da construtura, na chamada abaixo
    int sizeX = Image->SizeX();
    int sizeY = Image->SizeY();
    NovaImagem = new ImageClass(sizeX, sizeY);
    
    // Posiciona a imagem nova ao lado da antiga
    NovaImagem->SetPos(sizeX+10, 0);
    //    Image->CopyTo(NovaImagem);
    //    NovaImagem->DrawLineV(10,2, 100, 0,0,0);
    AplicarModo(NEGATIVO);

    // Matrizes auxiliares que permitem alterar brilho sem alterar a imagem.
    fillYIQ();
}

// **********************************************************************
//  void reshape( int w, int h )
//        trata o redimensionamento da janela OpenGL
//
// **********************************************************************
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
// **********************************************************************
//  void display( void )
//      Esta rotina é chamada toda vez que a tela precisa ser
//  redesenhada e toda vez qua a rotina 'glutPostRedisplay' é chamada
//
// **********************************************************************
void display( void )
{
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    Image->Display();
    NovaImagem->Display();
    
    glutSwapBuffers();
}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
    
    switch ( key )
    {
        case 27:        // Termina o programa qdo
        exit ( 0 );   // a tecla ESC for pressionada
        break;

        case '1':
        AplicarModo(BLACKANDWHITE);
        break;

        case '2':
        AplicarModo(GRAYSCALE);
        break;

        case '3':
        AplicarModo(NEGATIVO);
        break;

        case 'B':
        AumentarBrilho();
        break;

        case 'b':
        DiminuirBrilho();
        break;

        default:
        break;
    }
}

// **********************************************************************
//  void main ( int argc, char** argv )
// **********************************************************************
int main ( int argc, char** argv )
{
	cout << endl;
	cout << " ESC - termina programa" << endl;
	cout << " 1 - preto e branco" << endl;
	cout << " 2 - escala de cinza" << endl;
	cout << " 3 - negativo" << endl;

	cout << endl;
	cout << " Funcionalidades adicionais: " << endl;
	cout << " B - aumenta brilho" << endl;
	cout << " b - diminui brilho" << endl;
	cout << endl;


    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);
    
    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize  ( 650, 500);
    glutCreateWindow    ( "PT2 - Modulo 1 - ex2" );
    
    init ();
	//glutFullScreen();
    
    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    
    glutMainLoop ( );
    return 0;
}

