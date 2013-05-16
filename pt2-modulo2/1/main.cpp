/*
* Ex1 - Módulo 2 - Parte 2
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

ImageClass *Imagem1 = NULL;
ImageClass *Imagem2 = NULL;

// **********************************************************************
// Funcionalidades dos exercícios
// **********************************************************************

#define LIMIAR 220 //100 //200
void ConvertBlackAndWhite()
{
	if ( Imagem2 != NULL ) delete Imagem2;
    Imagem2 = new ImageClass ( Imagem1 -> SizeX(), Imagem1 -> SizeY() );
    Imagem2 -> SetPos ( Imagem1 -> SizeX()+10, 0 );
    // Tarefa 1:
    //        Mude o valor do LIMIAR e veja os resultados
    
    int x,y;
    
    for(x=0;x<Imagem1->SizeX();x++)
    {
        for(y=0;y<Imagem1->SizeY();y++)
        {
            if (Imagem1->GetPointIntensity(x,y) < LIMIAR)
            {
                Imagem2->DrawPixel(x, y,0,0,0);
            }
            else Imagem2->DrawPixel(x, y,255,255,255);
        }
    }
};

void GrayScale()
{
	if ( Imagem2 != NULL ) delete Imagem2;
    Imagem2 = new ImageClass ( Imagem1 -> SizeX(), Imagem1 -> SizeY() );
    Imagem2 -> SetPos ( Imagem1 -> SizeX()+10, 0 );
    // Tarefa 2:
    //        Gerar uma imagem em tons de cinza
    //        Use os métodos
    //            'ReadPixel' e 'DrawPixel' da classe 'ImageClass'
    
    //Imagem1->ReadPixel(GLint x, GLint y, unsigned char &r, unsigned char &g, unsigned char &b)
    // Obtém os valores R,G,B de um ponto X,Y da image
    
    //Imagem1->DrawPixel(GLint x, GLint y, unsigned char r, unsigned char g, unsigned char b)
    // Exibe na coordenada X,Y um ponto com a cor R,G,B na imagem
    int x,y;
    unsigned char r, g, b;
    for(x=0; x<Imagem1->SizeX(); x++)
    {
        for(y=0; y<Imagem1->SizeY(); y++)
        {
			Imagem1->ReadPixel(x, y, r, g, b);
			unsigned char gs = (r+g+b)/3;
			Imagem2->DrawPixel(x, y, gs, gs, gs);
        }
    }
}

void negativo()
{
	if ( Imagem2 != NULL ) delete Imagem2;
    Imagem2 = new ImageClass ( Imagem1 -> SizeX(), Imagem1 -> SizeY() );
    Imagem2 -> SetPos ( Imagem1 -> SizeX()+10, 0 );

	int x,y;
    unsigned char r, g, b;
    for(x=0; x<Imagem1->SizeX(); x++)
    {
        for(y=0; y<Imagem1->SizeY(); y++)
        {
			Imagem1->ReadPixel(x, y, r, g, b);
			r = 255 - r;
			g = 255 - g;
			b = 255 - b;
			Imagem2->DrawPixel(x, y, r, g, b);
        }
    }
}

//Pega a imagem armazenada no ponteiro Imagem1 e desenha ao lado dele
//uma nova imagem em escala menor.
void zoomout()
{
	if ( Imagem1 == NULL ) return;

	int posX = Imagem1 -> getPosX();
	posX += Imagem1->SizeX() + 10;

	int sX = Imagem1 -> SizeX();
	int sY = Imagem1 -> SizeY();
	int sizeX = (int) ceil( sX/2 );
	int sizeY = (int) ceil( sY/2 );

	if ( Imagem2 != NULL ) delete Imagem2;
	Imagem2 = new ImageClass ( sizeX, sizeY );
	Imagem2 -> SetPos ( posX, 0 );

	unsigned char r, g, b;
	for ( int i=0; i<sizeX; i++ )
	{
		for ( int j=0; j<sizeY; j++ )
		{
			unsigned int _r, _g, _b;
			int cont = 0;

			Imagem1 -> ReadPixel( i*2, j*2, r, g, b );
			_r = r; _g = g; _b = b;
			cont++;

			if ( j*2+1 < sY ) {
				Imagem1 -> ReadPixel( i*2, j*2+1, r, g, b );
				_r += r; _g += g; _b += b;
				cont++;
				if ( i*2+1 < sX ) {
					Imagem1 -> ReadPixel( i*2+1, j*2, r, g, b );
					_r += r; _g += g; _b += b;
					cont++;
					Imagem1 -> ReadPixel( i*2+1, j*2+1, r, g, b );
					_r += r; _g += g; _b += b;
					cont++;
				}
			}
			else if( i*2+1 < sX ) {
				Imagem1 -> ReadPixel( i*2+1, j*2, r, g, b );
				_r += r; _g += g; _b += b;
				cont++;
			}

			if ( cont != 0 ) {
				r = (unsigned char) ( (int) _r/cont);
				g = (unsigned char) ( (int) _g/cont);
				b = (unsigned char) ( (int) _b/cont);

				Imagem2 -> DrawPixel( i, j, r, g, b );
			}

		}
	}

}

//Pega a imagem armazenada no ponteiro Imagem1 e desenha ao lado dele
//uma nova imagem em escala maior sem interpolação.
void zoominQ() //Quadriculado
{
	if ( Imagem1 == NULL ) return;

	int posX = Imagem1 -> getPosX();
	posX += Imagem1 -> SizeX() + 10;

	int sX = Imagem1 -> SizeX();
	int sY = Imagem1 -> SizeY();
	int sizeX = (int) sX*2;
	int sizeY = (int) sY*2;

	if ( Imagem2 != NULL ) delete Imagem2;

	Imagem2 = new ImageClass ( sizeX, sizeY );
	Imagem2 -> SetPos ( posX, 0 );

	unsigned char r, g, b;
	for ( int i=0; i<sX; i++ )
	{
		for ( int j=0; j<sY; j++ )
		{
			Imagem1 -> ReadPixel( i, j, r, g, b );
			Imagem2 -> DrawPixel( 2*i, 2*j, r, g, b );
			Imagem2 -> DrawPixel( 2*i, 2*j+1, r, g, b );
			Imagem2 -> DrawPixel( 2*i+1, 2*j, r, g, b );
			Imagem2 -> DrawPixel( 2*i+1, 2*j+1, r, g, b );
		}
	}

}

//Pega a imagem armazenada no ponteiro Imagem1 e desenha ao lado dele
//uma nova imagem em escala maior com interpolação.
void zoominL() //Linear
{
	if ( Imagem1 == NULL ) return;

	int posX = Imagem1 -> getPosX();
	posX += Imagem1 -> SizeX() + 10;

	int sX = Imagem1 -> SizeX();
	int sY = Imagem1 -> SizeY();
	int sizeX = (int) sX*2;
	int sizeY = (int) sY*2;

	if ( Imagem2 != NULL ) delete Imagem2;

	Imagem2 = new ImageClass ( sizeX, sizeY );
	Imagem2 -> SetPos ( posX, 0 );

	unsigned char ra, ga, ba;
	unsigned char rb, gb, bb;
	for ( int i=0; i<sizeX; i+=2 )
	{
		for ( int j=0; j<sizeY; j+=2 )
		{
			unsigned char r, g, b;
			Imagem1 -> ReadPixel ( i/2, j/2, r, g, b );
			Imagem2 -> DrawPixel ( i, j, r, g, b );
		}
		for ( int j=1; j<sizeY; j+=2 )
		{
			Imagem2 -> ReadPixel ( i, j-1, ra, ga, ba );
			Imagem2 -> ReadPixel ( i, j+1, rb, gb, bb );
			Imagem2 -> DrawPixel ( i, j, 
				(ra+rb)/2, (ga+gb)/2, (ba+bb)/2 );
		}
	}
	for ( int i=1; i<sizeX; i+=2 )
	{
		for ( int j=0; j<sizeY; j++ )
		{
			Imagem2 -> ReadPixel ( i-1, j, ra, ga, ba );
			Imagem2 -> ReadPixel ( i+1, j, rb, gb, bb );
			Imagem2 -> DrawPixel ( i, j, 
				(ra+rb)/2, (ga+gb)/2, (ba+bb)/2 );
		}
	}

}

//Pega a imagem armazenada no ponteiro Imagem1 e desenha ao lado dele
//uma nova imagem em escala maior com interpolação por Splines Quadraticos.
void zoomInSplineQuadratico()
{
	if ( Imagem1 == NULL ) return;

	int posX = Imagem1 -> getPosX();
	posX += Imagem1 -> SizeX() + 10;

	int sX = Imagem1 -> SizeX();
	int sY = Imagem1 -> SizeY();
	int sizeX = (int) sX*2;
	int sizeY = (int) sY*2;

	if ( Imagem2 != NULL ) delete Imagem2;

	Imagem2 = new ImageClass ( sizeX, sizeY );
	Imagem2 -> SetPos ( posX, 0 );

    ImageClass *auxImage = new ImageClass ( sizeX, sizeY );
    
    int x,y;
	byte r0,g0,b0;
	byte r1,g1,b1;
	byte r2,g2,b2;
	double nc;

	for(y=0;y<sY;y++)
	{
 		float cR[sX], cG[sX], cB[sX];
 		cR[0] = cG[0] = cB[0] = 0;
 		float bR[sX], bG[sX], bB[sX];
 		float SR, SG, SB;
 		
		for(x=0;x<(sX);x++)
        {
              
              Imagem1->ReadPixel(x,y,r1,g1,b1);
              Imagem1->ReadPixel(x+1,y,r2,g2,b2);
              
              if(x!=0) {
                   Imagem1->ReadPixel(x-1,y,r0,g0,b0);
                   cR[x] = ((float)r2-(float)r1)-((float)r1-(float)r0)-(cR[x-1]);
                   cG[x] = ((float)g2-(float)g1)-((float)g1-(float)g0)-(cG[x-1]);
                   cB[x] = ((float)b2-(float)b1)-((float)b1-(float)b0)-(cB[x-1]);
              }
              
              bR[x] = ((float)r2-(float)r1)-cR[x];
              bG[x] = ((float)g2-(float)g1)-cG[x];
              bB[x] = ((float)b2-(float)b1)-cB[x];
              
              SR = (float)r1 + bR[x]*(0.5) + cR[x]*(0.25);
              SG = (float)g1 + bG[x]*(0.5) + cG[x]*(0.25);
              SB = (float)b1 + bB[x]*(0.5) + cB[x]*(0.25);
              
              auxImage->DrawPixel(2*x,y,r1,g1,b1);
              
              if( (SR>=0.0f && SR <256.0f) && (SG>=0.0f && SG<256.0f) && (SB>=0.0f && SB <256.0f))
                   auxImage->DrawPixel(2*x+1,y,(byte)(((float)r1+SR)/2),(byte)(((float)g1+SG)/2),(byte)(((float)b1+SB)/2));
              else
                   auxImage->DrawPixel(2*x+1,y,r2,g2,b2);
		}
	} 
	
	for(y=0;y<sY;y++)
	{
 		float cR[auxImage->SizeX()], cG[auxImage->SizeX()], cB[auxImage->SizeX()];
 		cR[0] = cG[0] = cB[0] = 0;
 		float bR[auxImage->SizeX()], bG[auxImage->SizeX()], bB[auxImage->SizeX()];
 		float SR, SG, SB;
 		
 		for(x=0;x<(auxImage->SizeX());x++)			
        {              
              auxImage->ReadPixel(x,y,r1,g1,b1);
              auxImage->ReadPixel(x,y+1,r2,g2,b2);
              
              if(x!=0) {
                   auxImage->ReadPixel(x,y-1,r0,g0,b0);
                   cR[x] = ((float)r2-(float)r1)-((float)r1-(float)r0)-(cR[x-1]);
                   cG[x] = ((float)g2-(float)g1)-((float)g1-(float)g0)-(cG[x-1]);
                   cB[x] = ((float)b2-(float)b1)-((float)b1-(float)b0)-(cB[x-1]);
              }
              
              bR[x] = ((float)r2-(float)r1)-cR[x];
              bG[x] = ((float)g2-(float)g1)-cG[x];
              bB[x] = ((float)b2-(float)b1)-cB[x];
              
              SR = (float)r1 + bR[x]*(0.5) + cR[x]*(0.25);
              SG = (float)g1 + bG[x]*(0.5) + cG[x]*(0.25);
              SB = (float)b1 + bB[x]*(0.5) + cB[x]*(0.25);
              
              Imagem2->DrawPixel(x,y*2,r1,g1,b1);
              
              if( (SR>=0.0f && SR <256.0f) && (SG>=0.0f && SG<256.0f) && (SB>=0.0f && SB <256.0f))
                   Imagem2->DrawPixel(x,2*y+1,(byte)(((float)r1+SR)/2),(byte)(((float)g1+SG)/2),(byte)(((float)b1+SB)/2));
              else
                   Imagem2->DrawPixel(x,2*y+1,r2,g2,b2);
		}
	}

}

double **Y = NULL;
double **I = NULL;
double **Q = NULL;
double br_level = 0.0;
double L1, T1, L2, T2;
void EqualizaHistograma()
{
	if ( Imagem2 != NULL ) delete Imagem2;
    Imagem2 = new ImageClass ( Imagem1 -> SizeX(), Imagem1 -> SizeY() );
    Imagem2 -> SetPos ( Imagem1 -> SizeX()+10, 0 );

	int sizeX = Imagem1->SizeX();
	int sizeY = Imagem1->SizeY();

	double m1, m2, m3;

	m1 = T1/L1;
	m2 = (T2-T1)/(L2-L1);
	m3 = (255.0-T2)/(255.0-L2);

	double _y, _i, _q;
	unsigned char r, g, b;
	for(int w=0; w<sizeX; w++)
	{
		for(int z=0; z<sizeY; z++)
		{
			// obtém YIQ da imagem original e aplica a mudança de brilho
			_y = Y[w][z] + br_level;
			_i = I[w][z];
			_q = Q[w][z];

			// este pixel está em qual intervalo de luminância?
			double m;
			if(_y <= L1)
				m = m1;
			else if(_y <= L2)
				m = m2;
			else
				m = m3;

			// luminância transformada
			_y *= m;

			// YIQ para RGB
			double aux;
			aux = _y + 0.956*_i + 0.620*_q;
			r = ( aux > 255) ? 255 : ( ( aux < 0) ? 0 : (unsigned char)aux );
			aux = _y - 0.272*_i - 0.647*_q;
			g = ( aux > 255) ? 255 : ( ( aux < 0) ? 0 : (unsigned char)aux );
			aux = _y - 1.108*_i + 1.705*_q;
			b = ( aux > 255) ? 255 : ( ( aux < 0) ? 0 : (unsigned char)aux );

			// Desenha outra imagem com a transformação
			Imagem2->DrawPixel(w, z, r, g, b);
		}
	}
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
	ZOOMINSPLINEQUADRATICO,
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

		case ZOOMINSPLINEQUADRATICO:
		zoomInSplineQuadratico();
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

void fillYIQ()
{
	int sizeX = Imagem1->SizeX();
	int sizeY = Imagem1->SizeY();

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
			Imagem1->ReadPixel( i, j, r, g, b );
			Y[i][j] = 0.299*r + 0.587*g + 0.114*b;
			I[i][j] = 0.71*(r-Y[i][j]) - 0.27*(b-Y[i][j]);
			Q[i][j] = 0.48*(r-Y[i][j]) + 0.41*(b-Y[i][j]);
		}
	}

}

void AumentarBrilho()
{
	if( br_level == 120.0 ) return;
	br_level += 20.0;

	int sizeX = Imagem1->SizeX();
	int sizeY = Imagem1->SizeY();

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
			Imagem1->DrawPixel(x, y, r, g, b);
		}
	}

	AplicarModo(modo_atual);
}

void DiminuirBrilho()
{
	if( br_level == -120.0 ) return;
	br_level -= 20.0;

	int sizeX = Imagem1->SizeX();
	int sizeY = Imagem1->SizeY();

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
			Imagem1->DrawPixel(x, y, r, g, b);
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
    Imagem1 = new ImageClass();
    
    r = Imagem1->Load("f5.bmp");  // Leitura da Imagem
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
    int sizeX = Imagem1->SizeX();
    int sizeY = Imagem1->SizeY();
    Imagem2 = new ImageClass(sizeX, sizeY);
    
    // Posiciona a imagem nova ao lado da antiga
    Imagem2->SetPos(sizeX+10, 0);
    //    Imagem1->CopyTo(Imagem2);
    //    Imagem2->DrawLineV(10,2, 100, 0,0,0);

    // Matrizes auxiliares que permitem alterar brilho sem alterar a imagem.
    fillYIQ();

    AplicarModo(ZOOMOUT);
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
    Imagem1->Display();
    Imagem2->Display();
    
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

        case '4':
        AplicarModo(EQUALIZAHISTOGRAMA);
        break;

        case '5':
        AplicarModo (ZOOMINQ);
        break;

        case '6':
        AplicarModo(ZOOMINL);
        break;

        case '7':
        AplicarModo(ZOOMOUT);
        break;

        case '8':
        AplicarModo(ZOOMINSPLINEQUADRATICO);
        break;

        case 'B':
        AumentarBrilho();
        glutPostRedisplay();
        break;

        case 'b':
        DiminuirBrilho();
        glutPostRedisplay();
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
	cout << endl << " Insira os valores L1, T1, L2, T2. Obs.: L1<L2 e T1<T2 obrigatoriamente" << endl;

	cout << endl << " L1: ";
	cout << "40"; L1 = 40.0;
	//cin >> L1;
	cout << endl << " T1: ";
	cout << "70"; T1 = 70.0;
	//cin >> T1;
	if( L1 > 255.0 | T1 > 255.0 | L1 < 0.0 | T1 < 0.0) return 1;

	cout << endl << " L2: ";
	cout << "90"; L2 = 90.0;
	//cin >> L2;
	cout << endl << " T2: ";
	cout << "150" << endl; T2 = 150.0;
	//cin >> T2;
	if( L2 > 255.0 | T2 > 255.0 | L2 < 0.0 | T2 < 0.0 ) return 1;

	if( L1 >= L2 | T1 >= T2 ) return 1;

	cout << endl;
	cout << " ESC - termina programa" << endl;
	cout << " 1 - preto e branco" << endl;
	cout << " 2 - escala de cinza" << endl;
	cout << " 3 - negativo" << endl;
	cout << " 4 - equalizar histograma" << endl;
	cout << " 5 - zoom in quadriculado" << endl;
	cout << " 6 - zoom in linear" << endl;
	cout << " 7 - zoom out" << endl;
	cout << " 8 - zoom in com interpolação por splines quadraticos" << endl;

	cout << endl;
	cout << " Funcionalidades adicionais: " << endl;
	cout << " B - aumenta brilho" << endl;
	cout << " b - diminui brilho" << endl;
	cout << endl;


    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);
    
    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize  ( 900, 650);
    glutCreateWindow    ( "PT2 - Modulo 1 - ex2" );
    
    init ();
	//glutFullScreen();
    
    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    
    glutMainLoop ( );
    return 0;
}

