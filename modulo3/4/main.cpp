/*
 * Módulo 3 - Exercício 4
 * Nomes:
 *      Arthur Henrique Coutinho
 *      Breno Naodi Kusunoki
 *      Luiz Guilherme Castilho Martins
 */
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include "glut.h"
#endif

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>


using namespace std;

class Ponto
{
public:
	float x;
	float y;

	Ponto()
	{
		x=0;
		y=0;
	}

	Ponto( float _x, float _y )
	{
        Ponto();
		fill( _x, _y );
	}

	void fill( float _x, float _y)
	{
		x = _x;
		y = _y;
	}
};

float distanciaDoisPontos(float x1, float y1, float x2, float y2)
{
	float distancia = 0.0;
	distancia = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );

	return distancia;
}

float anguloSegmentoReta(float x1, float y1, float x2, float y2)
{
	float ang = atan2( y2-y1, x2-x1 );

	return ang;
}


Ponto* interRetas(Ponto k, Ponto l, Ponto m, Ponto n)
{
    double det;
	det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

	Ponto* inter = new Ponto();
	if (det != 0.0) //há intersecção
	{
		double s;
		s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;

		inter->x = k.x + (l.x-k.x)*s;
		inter->y = k.y + (l.y-k.y)*s;
	}
	
	return inter;
}

const float PI = 3.1416f;
Ponto *p1;
Ponto *p2;
int noImpressoes;

void desenhaPentagrama()
{
	//obtendo o tamanho da reta inicial a partir dos dois pontos
	float tamRetaPivo = distanciaDoisPontos( p1->x, p1->y, p2->x, p2->y );

	//obtendo a inclinação da reta inicial
	float anguloAtual = anguloSegmentoReta( p1->x, p1->y, p2->x, p2->y );

	/**Desenhando os pentagramas do mais externo ao mais interno a partir da reta inicial**/
	//desenhando o primeiro ponto dado
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	float x = p1->x;
	float y = p1->y;
	glVertex2f(x, y);
	float ptsExt[5][2];
	ptsExt[0][0] = x;
	ptsExt[0][1] = y;
	//laço desenhando as arestas do pentagrama mais externo
	for( int i=1; i<5; i++ )
	{
		x += tamRetaPivo*cos(anguloAtual);
		y += tamRetaPivo*sin(anguloAtual);

		ptsExt[i][0] = x;
		ptsExt[i][1] = y;

		glVertex2f( x, y );

		//proxima aresta com angulo de +144 graus da atual
		anguloAtual -= ( 4.0 * PI ) / 5.0;
	}
	glEnd();

	//conectando os pontos externos da figura
	glBegin(GL_LINE_LOOP);
	glVertex2f( ptsExt[0][0], ptsExt[0][1] );
	glVertex2f( ptsExt[3][0], ptsExt[3][1] );
	glVertex2f( ptsExt[1][0], ptsExt[1][1] );
	glVertex2f( ptsExt[4][0], ptsExt[4][1] );
	glVertex2f( ptsExt[2][0], ptsExt[2][1] );
	glEnd();

	//laço desenhando os pentagramas internos;
	for( int j=1; j<noImpressoes; j++)
	{
		//obtendo nova reta pivo
		Ponto k( ptsExt[3][0], ptsExt[3][1] );
		Ponto l( ptsExt[4][0], ptsExt[4][1] );
		Ponto m( ptsExt[1][0], ptsExt[1][1] );
		Ponto n( ptsExt[2][0], ptsExt[2][1] );
		Ponto *pt1Pivo;
		pt1Pivo = interRetas( k,l,m,n );
		k.fill( ptsExt[0][0], ptsExt[0][1] );
		l.fill( ptsExt[4][0], ptsExt[4][1] );
		m.fill( ptsExt[2][0], ptsExt[2][1] );
		n.fill( ptsExt[3][0], ptsExt[3][1] );
		Ponto *pt2Pivo;
		pt2Pivo = interRetas( k,l,m,n );

		//obtendo tamanho da nova reta pivo
		tamRetaPivo = distanciaDoisPontos( pt1Pivo->x, pt1Pivo->y, pt2Pivo->x, pt2Pivo->y );

		//obtendo inclinação da nova reta pivo
		anguloAtual = anguloSegmentoReta( pt1Pivo->x, pt1Pivo->y, pt2Pivo->x, pt2Pivo->y );

		//desenhando o primeiro ponto da reta pivo
		glBegin(GL_LINE_LOOP);
		float x = pt1Pivo->x;
		float y = pt1Pivo->y;
		delete pt1Pivo;
		glVertex2f( x, y );
		ptsExt[0][0] = x;
		ptsExt[0][1] = y;
		//laço desenhando as arestas do pentagrama interno atual
		for( int i=1; i<5; i++ )
		{
			x += tamRetaPivo*cos(anguloAtual);
			y += tamRetaPivo*sin(anguloAtual);

			ptsExt[i][0] = x;
			ptsExt[i][1] = y;

			glVertex2f( x, y );
			
			//proxima aresta com angulo de +144 graus da atual
			anguloAtual -= ( 4.0 * PI ) / 5.0;
		}
		glEnd();
	}

	delete p1;
	delete p2;
}

void init( void )
{
    glClearColor( 1.0, 1.0, 1.0, 0.0 ); //get white background color
    glColor3f( 0.0f, 0.0f, 0.0f );  //set drawing color
    glPointSize( 4.0 );     //a dot is 4x4
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 500.0, 0.0, 500.0 );
}

void display()
{
    glClear( GL_COLOR_BUFFER_BIT );
    desenhaPentagrama();
    glFlush();
}

int main(int argc, char** argv)
{
	//armazenando os parâmetros como pontos de uma reta do pentagrama
	if( argc != 6 )
	{
        printf("\n\n ** Sintaxe errada! ** \n");
		printf("\n Uso correto: \n");
		printf(" %s <No de estrelas> <X1> <Y1> <X2> <Y2> \n\n\n", argv[0]);

		float x_inicial = 100.00;
		float y_inicial = 50.00;
		float delta = 145.00;
		float ang_inicial = 72.00; //angulo da reta pivo é 72 graus em relação ao eixo X
		float ang_inicial_rad = ang_inicial*PI/180.00;
		p1 = new Ponto( x_inicial, y_inicial );
		p2 = new Ponto( x_inicial+delta, y_inicial+(delta*tan(ang_inicial_rad)) );
		noImpressoes = 3;
    }
	else
	{
		noImpressoes = atoi(argv[1]);
		p1->x = atoi(argv[2]);
		p1->y = atoi(argv[3]);
		p2->x = atoi(argv[4]);
		p2->y = atoi(argv[5]);
	}

	if( noImpressoes <= 0 ) return -1;

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize(500, 500);
	glutInitWindowPosition( 100, 150 );
	glutCreateWindow("Modulo 3 - Ex4");
	init();
	glutDisplayFunc( display );
	glutMainLoop();

    return 0;
}

