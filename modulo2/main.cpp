#include <cstdlib>
#include <iostream>
#include "glut.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <fstream>
#include <time.h>
#include "Fila.h"

#define ARROW_LEFT 100
#define ARROW_UP 101
#define ARROW_RIGHT 102
#define ARROW_DOWN 103

#define PLUS_DELTA 4
#define MINUS_DELTA -4

#define SCREEN_HEIGHT 500

/**
typedef struct Ponto
{
  int x;
  int y;
  unsigned short int idForma;
  int tipo;
};

std::vector*<Ponto*> pontos;
**/
Fila Poly, Line;
Coordenadas C;
bool inserirPol, inserirLinConect, erasePoint;
int countTypePolyLine, nextLocation, localX, localY;


//########################################Desenha os poligonos registrados na fila
void desenhaNaTela() {
  /**
  unsigned short int lastId = 0xffff;
  std::vector<Pontos*>::iterator p = pontos->begin();
  lastId = p->
  glBegin(p->tipo);
  while( p != pontos->end() )
  {
    unsigned short int id = p->idForma;
    if ( (lastId && id) == id )
    {
      glVertex2i( p->x; p->y );
    }
    else
    {
      glEnd();
      glFlush();
      glBegin(p->tipo);
      glColor3f(0.0, 0.0, 0.0);
      glVertex2i( p->x, p->y );
    }
    lastId = id;
    ++p;
  }
  glEnd();
  glFlush();
  **/
  C = Poly.busca(); 
  int auxID;
  auxID = C.idPolyline[0];

  glBegin(GL_POLYGON);
  glColor3f(0.0, 0.0, 0.0);
  for(int i=0 ; i<C.qtd ; i++)
  {
    if(auxID == C.idPolyline[i]) {
      glVertex2i( C.x[i] , C.y[i]);
    }
    else {
      glEnd();  
      glFlush();
      auxID = C.idPolyline[i];

      glBegin(GL_POLYGON);
      glColor3f(0.0, 0.0, 0.0);
      glVertex2i( C.x[i] , C.y[i]);
    }
  }    
  glEnd();  
  glFlush(); 
      //--------------------------------------------------------------------------
  C = Line.busca();
  auxID = C.idPolyline[0];
  int firstPoint;
  firstPoint = 0;

  glBegin(GL_LINE_STRIP); 
  for(int i = 0; i <= C.qtd; i++) 
  {
    if(i != C.qtd) 
    {  
      if(auxID == C.idPolyline[i]) {
        glVertex2i(C.x[i], C.y[i]);
      }
      else {
        glVertex2i(C.x[firstPoint], C.y[firstPoint]);
        glEnd();  
        glFlush();
        firstPoint = i;
        auxID = C.idPolyline[i];

        glBegin(GL_LINE_STRIP); 
        glVertex2i( C.x[i] , C.y[i]);
      }
    }
    else {
      glVertex2i(C.x[firstPoint], C.y[firstPoint]);
    }
  }
  glEnd();  
  glFlush();          
}

//########################################Grava as coordenadas no arquivo
void saveFile(char *fileName) {
  int auxCountLines;
  auxCountLines = 0;
  C = Poly.busca();
  auxCountLines = C.qtd;
  C = Line.busca();
  auxCountLines = auxCountLines + C.qtd;

  ofstream outStream(fileName);
  if(outStream.is_open()) {
    outStream << auxCountLines; 
    outStream << "\n";
    cout << "SALVANDO NO ARQUIVO \n";
    cout << auxCountLines << "\n";
    for(int i=0 ; i<C.qtd ; i++) {
      outStream << C.x[i] << " " << C.y[i] << " " << C.idPolyline[i] << " " << 0 <<"\n";
      cout << C.x[i] << C.y[i] << C.idPolyline[i] << "\n";
    }
    C = Poly.busca();
    cout << "\n";
    for(int i=0 ; i<C.qtd ; i++) {
      outStream << C.x[i] << " " << C.y[i] << " " << C.idPolyline[i] << " " << 1 <<"\n";
      cout << C.x[i] << C.y[i] << C.idPolyline[i] << "\n";
    }         
    outStream.close();
  }
}

//######################################Lê um arquivo com as coordenadas do poligono
void readFile(char *fileName) {
  fstream inStream;
  inStream.open(fileName, ios ::in); //Open the file
  if(inStream.fail()){
    return;
  }

  Poly.~Fila();
  Line.~Fila();     
  GLint numLines, x, y, id, type;
  inStream >> numLines; //Read the number of lines
  for(int j=0 ; j<numLines ; j++)
  {
    inStream >> x >> y >> id >> type;
    if(type == 0){ //Se for LINHA
      Line.inserir(x, y, id);
    }
    else if(type == 1){ //Se for POLYLINE
      Poly.inserir(x, y, id);
    }
  }
  inStream.close();
  desenhaNaTela();
}

//######################################
void erasePointPolyline(int x, int y) {
  double distPoly, distLine, auxCalc; 
  int coordX_Poly, coordY_Poly, coordX_Line, coordY_Line;                          
  distPoly = distLine = 100000;
  coordX_Poly = coordY_Poly = coordX_Line = coordY_Line = 0;
  C = Poly.busca();
  for(int i=0 ; i<C.qtd ; i++) 
  {
    auxCalc = sqrt( ((x-C.x[i])*(x-C.x[i])) + ((y-C.y[i])*(y-C.y[i])) );    
    if(auxCalc < distPoly) {
      distPoly = auxCalc;
      coordX_Poly = C.x[i];
      coordY_Poly = C.y[i];
    }
  }
  C = Line.busca();
  for(int i=0 ; i<C.qtd ; i++) 
  {
    auxCalc = sqrt( ((x-C.x[i])*(x-C.x[i])) + ((y-C.y[i])*(y-C.y[i])) );
    if(auxCalc < distLine) {
      distLine = auxCalc;
      coordX_Line = C.x[i];
      coordY_Line = C.y[i];
    }
  }

  if( distPoly < distLine ) 
  {
    if( coordX_Poly!=0 && coordY_Poly!=0) {              
      Poly.remover(coordX_Poly, coordY_Poly);              
      glClear(GL_COLOR_BUFFER_BIT);
      glFlush(); 
      desenhaNaTela();
    }
  }
  else if( distPoly > distLine) 
  {
    if( coordX_Line!=0 && coordY_Line!=0) {
      Line.remover(coordX_Line, coordY_Line);
      glClear(GL_COLOR_BUFFER_BIT);
      glFlush(); 
      desenhaNaTela();
    }
  } 
}

//######################################

int* getNearPointAllInfo(int x, int y)
{
  double distPoly, distLine, auxCalc; 
  int coordX_Poly, coordY_Poly, coordX_Line, coordY_Line;                          
  distPoly = distLine = 100000;
  coordX_Poly = coordY_Poly = coordX_Line = coordY_Line = 0;
  C = Poly.busca();
  int id_Poly;
  for(int i=0 ; i<C.qtd ; i++) 
  {
    auxCalc = sqrt( ((x-C.x[i])*(x-C.x[i])) + ((y-C.y[i])*(y-C.y[i])) );    
    if(auxCalc < distPoly) {
      distPoly = auxCalc;
      coordX_Poly = C.x[i];
      coordY_Poly = C.y[i];
      id_Poly = C.idPolyline[i];
    }
  }
  C = Line.busca();
  int id_Line;
  for(int i=0 ; i<C.qtd ; i++) 
  {
    auxCalc = sqrt( ((x-C.x[i])*(x-C.x[i])) + ((y-C.y[i])*(y-C.y[i])) );    
    if(auxCalc < distLine) {
      distLine = auxCalc;
      coordX_Line = C.x[i];
      coordY_Line = C.y[i];
      id_Line = C.idPolyline[i];
    }
  }

  int* ret = new int[3];
  if( distPoly < distLine ) {
    if( coordX_Poly!=0 && coordY_Poly!=0) {
      ret[0] = coordX_Poly;
      ret[1] = coordY_Poly;
      ret[2] = id_Poly;
    }
  }
  else if( distPoly > distLine) {
    if( coordX_Line!=0 && coordY_Line!=0) {
      ret[0] = coordX_Line;
      ret[1] = coordY_Line;
      ret[2] = id_Poly;
    }
  }

  return ret;
}

int* getNearPointXY(int x, int y)
{
  int* arg;
  arg = getNearPointAllInfo(x, y);
  int* ret = new int[3];
  ret[0] = arg[0];
  ret[1] = arg[1];

  delete arg;

  return ret;
}

void getNearPoint( int x, int y )
{
  int* arg = getNearPointXY(x, y);
  localX = arg[0];
  localY = arg[1];

  delete arg;
}

//######################################
void wait(int seconds)
{
  clock_t endwait;
  endwait = clock() + seconds * CLOCKS_PER_SEC;
  while(clock() < endwait) {}
}

//##############################################################################
class GLintPoint
{
public:
  GLint x,y;
};

void init( void )
{
  glClearColor( 1.0, 1.0, 1.0, 0.0 );	//get white background color
  glColor3f( 0.0f, 0.0f, 0.0f );	//set drawing color
  glPointSize( 4.0 );			//a dot is 4x4
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( 0.0, 500.0, 0.0, 500.0 );
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glFlush();
}

void drawDot( int x, int y )
{
  glBegin( GL_POINTS );			
  glVertex2i( x, y );		//draw a points
  glEnd();
}

void myMouse(int button, int state, int x, int y)
{
  #define NUM 20
  //static GLintPoint List[NUM];
  static int last = -1;            	// last index used so far 

  // test for mouse button as well as for a full array
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && last < NUM -1)
  {
    if(inserirLinConect == true) { //Quando ativa a tecla 'r'
      GLint mouseX = x;
      GLint mouseY = SCREEN_HEIGHT - y;

      Line.inserir(mouseX, mouseY, countTypePolyLine);

      int auxID;
      auxID = C.idPolyline[0];
      int firstPoint;
      firstPoint = 0;

      C = Line.busca();
      glBegin(GL_LINE_STRIP);
      for(int i=0 ; i<C.qtd ; i++)
      {
        if(auxID == C.idPolyline[i]) {
          glVertex2i(C.x[i], C.y[i]);
        }
        else {
          glVertex2i(C.x[firstPoint], C.y[firstPoint]);
          glEnd();  
          glFlush();
          firstPoint = i;
          auxID = C.idPolyline[i];

          glBegin(GL_LINE_STRIP); 
          glVertex2i( C.x[i] , C.y[i]);
        }
      }
      glEnd();
      glFlush();	          
    }
    else if (inserirPol == true) { //Quando ativa a tecla 'p'
      GLint mouseX = x;
      GLint mouseY = SCREEN_HEIGHT - y;

      Poly.inserir(mouseX, mouseY, countTypePolyLine); 

      drawDot ( mouseX, mouseY );
      glFlush();
    }
    else if(erasePoint==true) { //Quando ativa a tecla 'a'
      GLint mouseX = x;
      GLint mouseY = SCREEN_HEIGHT - y;
      erasePointPolyline(mouseX,mouseY);
    }
    else if(nextLocation != 0) { //Quando ativa a tecla 'm'
      GLint mouseX = x;
      GLint mouseY = SCREEN_HEIGHT - y;
      if(nextLocation == 1) { //Primeiro clique
        nextLocation = 2;
        getNearPoint(mouseX, mouseY);
      }
      else if(nextLocation == 2) { //Segundo clique
        nextLocation = 0;
        Poly.trocaValor(localX, localY, mouseX, mouseY);
        Line.trocaValor(localX, localY, mouseX, mouseY);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        desenhaNaTela();
      }
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
      last = -1;	         // reset the list to empty
    }
  }
}

void myMovedMouse(  int mouseX, int mouseY)
{
}

void myAsciiKeyboard ( unsigned char key, int mouseX, int mouseY )
{
  GLint x = mouseX;
  GLint y = SCREEN_HEIGHT - mouseY;
  int auxType;
  
  switch( key )
  {
    case 'p': //Cria um novo polígono 	          
    inserirPol = true; 	      
    erasePoint = false;    
    nextLocation = 0;
    if(inserirLinConect==true) {
      inserirLinConect = false;
      countTypePolyLine++;
    } 	          
    break;

    case 'o': //Finaliza o polígono
    inserirPol = false;
    inserirLinConect = false;  
    erasePoint = false;  
    nextLocation = 0;          
    countTypePolyLine++;
    desenhaNaTela();	          
    break;

    case 'r': //Cria um conjunto de linhas conectadas
    inserirLinConect = true;  
    erasePoint = false;     
    nextLocation = 0;       
    if(inserirPol == true) {
      inserirPol = false;
      countTypePolyLine++;
    }
    break;

    case 'h': //Finaliza o conjunto de linhas conectadas
    inserirLinConect = false;
    inserirPol = false;    
    erasePoint = false;   
    nextLocation = 0;       
    countTypePolyLine++;
    desenhaNaTela();
    break;

    case 'a': //Apaga o ponto mais próximo do mouse
    inserirLinConect = false;
    inserirPol = false;
    erasePoint = true;
    nextLocation = 0;
    break;

    case 'm': //Move o ponto para a próxima localização (2 cliques, um marca e o outro clique move o ponto)
    inserirLinConect = false;
    inserirPol = false;    
    erasePoint = false;
    nextLocation = 1;
    break;

    case 'd': //Limpa a tela e redesenha todos os polígonos
    inserirLinConect = false;
    inserirPol = false;    
    erasePoint = false;
    nextLocation = 0;

    glClear(GL_COLOR_BUFFER_BIT);
    glFlush(); 
    wait(2);
    desenhaNaTela();
    break;

    case 't': //Termina o programa
    Poly.~Fila();
    Line.~Fila();
    exit ( -1 );
    break;

    case 's': //Salva os polígonos em um arquivo texto, utilizando o formato de arquivo usado no programa 4
    inserirLinConect = false;
    inserirPol = false;    
    erasePoint = false;
    nextLocation = 0;

    saveFile("coordenadas.dat");
    break;

    case 'l': //Leitura e visualização do arquivo de polígonos
    inserirLinConect = false;
    inserirPol = false;    
    erasePoint = false;
    nextLocation = 0;

    glClear(GL_COLOR_BUFFER_BIT);              
    glFlush();              
    readFile("coordenadas.dat"); 
    break;

    default :
    break;
  }
}



void mySpecialKeyboard ( int key, int mouseX, int mouseY )
{
  inserirLinConect = false;
  inserirPol = false;    
  erasePoint = false;
  nextLocation = 0;

  bool tecla_valida = true;

  int desloque[2] = {0, 0};

  switch( key )
  {
    case ARROW_LEFT:
    //salvar -deltaX no vetor "desloque"
    desloque[0] = MINUS_DELTA;
    desloque[1] = 0;
    break;

    case ARROW_UP:
    //salvar +deltaY no vetor "desloque"
    desloque[0] = 0;
    desloque[1] = PLUS_DELTA;
    break;

    case ARROW_RIGHT:
    //salvar +deltaX no vetor "desloque"
    desloque[0] = PLUS_DELTA;
    desloque[1] = 0;
    break;

    case ARROW_DOWN:
    //salvar -deltaY no vetor "desloque"
    desloque[0] = 0;
    desloque[1] = MINUS_DELTA;
    break;

    default:
    tecla_valida = false;
    break;
  }

  if (tecla_valida)
  {

    printf("\n\nPassou por aqui.\n\n");

    GLint mX = mouseX;
    GLint mY = SCREEN_HEIGHT - mouseY;

    //encontrar a forma mais próxima do mouse
    int* ptProx = getNearPointAllInfo(mX, mY);
    int idFormaProxima = ptProx[2];

    delete ptProx;

    printf("\n\n ID da forma mais proxima eh %d\n\n", idFormaProxima);
    
    //somar todos os seus pontos com o vetor de deslocamento
    Coordenadas _Poly = Poly.busca();
    Coordenadas _Line = Line.busca();
    int i = 0;
    int polyQtd = _Poly.qtd;
    int lineQtd = _Line.qtd;
    bool buscaPoly, buscaLine;
    while( (buscaPoly = (i < polyQtd)) || (buscaLine = (i < lineQtd)) )
    {
      if( buscaPoly )
      {
        printf("\n\n Analisando ponto %d %d em Poly", _Poly.x[i], _Poly.y[i]);
        if ( _Poly.idPolyline[i] == idFormaProxima ){
          int x = _Poly.x[i];
          int y = _Poly.y[i];
          printf("\n\n Movendo ponto %d %d\n\n", x, y);
          Poly.trocaValor( x, y, x+desloque[0], y+desloque[1] );
        }
      }

      if( buscaLine  )
      {
        printf("\n\n Analisando ponto %d %d em Line", _Line.x[i], _Line.y[i]);
        if ( _Line.idPolyline[i] == idFormaProxima ){
          int x = _Line.x[i];
          int y = _Line.y[i];
          printf("\n\n Movendo ponto %d %d", x, y);
          Line.trocaValor( x, y, x+desloque[0], y+desloque[1] );
        }
      }

      ++i;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    desenhaNaTela();
  }
}

int main(int argc, char** argv)
{
  inserirPol = false;
  inserirLinConect = false;
  erasePoint = false;
  countTypePolyLine = 0;
  nextLocation = 0;
  localX = localY = 0;

  glutInit(&argc, argv);	//initialize toolkit
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );	//set display mode
  glutInitWindowSize(500, 500);		//set window size on screen
  glutInitWindowPosition( 100, 150 ); 	//set window position on screen
  glutCreateWindow(argv[0]);		//open screen widow
  init();
  glutMouseFunc( myMouse );
  glutMotionFunc( myMovedMouse );
  glutKeyboardFunc( myAsciiKeyboard );
  glutSpecialFunc( mySpecialKeyboard );
  glutDisplayFunc( display );
  glutMainLoop();			//go into perpetual loop
  
  return 0;
}
