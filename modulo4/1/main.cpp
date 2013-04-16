//#############################################################################
//***************             Ernesto Yuiti Saito               ***************
//*************** 5COP005 - Introdução à Computação Gráfica     ***************
//#############################################################################

#include <cstdlib>
#include <iostream>
#include "glut.h"
#include <stdlib.h>
#define   screenHeight  500
#include <math.h>
#include <stdio.h>

#include <fstream>
#include <time.h>
#include "Fila.h"

Fila Poly, Line;
Coordenadas C;
bool inserirPol, inserirLinConect, erasePoint;
int countTypePolyLine, nextLocation;
float localX, localY;
int modoTranslacao, modoRotacao, modoEscala; 
float centroX = 0.0, centroY = 0.0;


//########################################Desenha os poligonos registrados na fila
void desenhaNaTela() {
    C = Poly.busca(); 
    int auxID;
    auxID = C.idPolyline[0];
	          
	glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0);
    for(int i=0 ; i<C.qtd ; i++) {
         if(auxID == C.idPolyline[i]) {
              glVertex2f( C.x[i] , C.y[i]);
         }
         else {
              glEnd();  
              glFlush();
              auxID = C.idPolyline[i];
                      
              glBegin(GL_POLYGON);
              glColor3f(0.0, 0.0, 0.0);
              glVertex2f( C.x[i] , C.y[i]);
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
    for(int i = 0; i <= C.qtd; i++) {
         if(i != C.qtd) {  
		      if(auxID == C.idPolyline[i]) {
                   glVertex2f(C.x[i], C.y[i]);
              }
              else {
                   glVertex2f(C.x[firstPoint], C.y[firstPoint]);
                   glEnd();  
                   glFlush();
                   firstPoint = i;
                   auxID = C.idPolyline[i];
                        	
                   glBegin(GL_LINE_STRIP); 
                   glVertex2f( C.x[i] , C.y[i]);
              }
         }
         else {
              glVertex2f(C.x[firstPoint], C.y[firstPoint]);
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
    if(inStream.fail())
         return;
    
    Poly.~Fila();
    Line.~Fila();     
    GLint numLines, x, y, id, type;
    inStream >> numLines; //Read the number of lines
    for(int j=0 ; j<numLines ; j++) {
         inStream >> x >> y >> id >> type;
         if(type == 0) //Se for LINHA
              Line.inserir(x, y, id);
         else if(type == 1) //Se for POLYLINE
              Poly.inserir(x, y, id);      
    }
    inStream.close();
    desenhaNaTela();
}

//######################################
void erasePointPolyline(int x, int y) {
    double distPoly, distLine, auxCalc; 
    float coordX_Poly, coordY_Poly, coordX_Line, coordY_Line;                          
    distPoly = distLine = 100000;
    coordX_Poly = coordY_Poly = coordX_Line = coordY_Line = 0;
    C = Poly.busca();
    for(int i=0 ; i<C.qtd ; i++) {
         auxCalc = sqrt( ((x-C.x[i])*(x-C.x[i])) + ((y-C.y[i])*(y-C.y[i])) );    
         if(auxCalc < distPoly) {
              distPoly = auxCalc;
              coordX_Poly = C.x[i];
              coordY_Poly = C.y[i];
         }
    }
    C = Line.busca();
    for(int i=0 ; i<C.qtd ; i++) {
         auxCalc = sqrt( ((x-C.x[i])*(x-C.x[i])) + ((y-C.y[i])*(y-C.y[i])) );    
         if(auxCalc < distLine) {
              distLine = auxCalc;
              coordX_Line = C.x[i];
              coordY_Line = C.y[i];
         }
    }
    
    if( distPoly < distLine ) {
         if( coordX_Poly!=0 && coordY_Poly!=0) {              
              Poly.remover(coordX_Poly, coordY_Poly);              
              glClear(GL_COLOR_BUFFER_BIT);
              glFlush(); 
              desenhaNaTela();
         }
    }
    else if( distPoly > distLine) {
         if( coordX_Line!=0 && coordY_Line!=0) {
              Line.remover(coordX_Line, coordY_Line);
              glClear(GL_COLOR_BUFFER_BIT);
              glFlush(); 
              desenhaNaTela();
         }
    } 
}

//######################################
void getNearPoint(int x, int y) {
    double distPoly, distLine, auxCalc; 
    float coordX_Poly, coordY_Poly, coordX_Line, coordY_Line;                          
    distPoly = distLine = 100000;
    coordX_Poly = coordY_Poly = coordX_Line = coordY_Line = 0;
    C = Poly.busca();
    for(int i=0 ; i<C.qtd ; i++) {
         auxCalc = sqrt( ((x-C.x[i])*(x-C.x[i])) + ((y-C.y[i])*(y-C.y[i])) );    
         if(auxCalc < distPoly) {
              distPoly = auxCalc;
              coordX_Poly = C.x[i];
              coordY_Poly = C.y[i];
         }
    }
    C = Line.busca();
    for(int i=0 ; i<C.qtd ; i++) {
         auxCalc = sqrt( ((x-C.x[i])*(x-C.x[i])) + ((y-C.y[i])*(y-C.y[i])) );    
         if(auxCalc < distLine) {
              distLine = auxCalc;
              coordX_Line = C.x[i];
              coordY_Line = C.y[i];
         }
    }
    
    if( distPoly < distLine ) {
         if( coordX_Poly!=0 && coordY_Poly!=0) {
              localX = coordX_Poly;
              localY = coordY_Poly;
         }
    }
    else if( distPoly > distLine) {
         if( coordX_Line!=0 && coordY_Line!=0) {
              localX = coordX_Line;
              localY = coordY_Line;
         }
    }  
}

//*****************************************************************************
//*****************************************************************************
void translacao(float deltaX, float deltaY) {
    C = Poly.busca();
    for(int i=0 ; i<C.qtd ; i++) {
         Poly.trocaValor(C.x[i], C.y[i], C.x[i]+deltaX, C.y[i]+deltaY);
    }
    C = Line.busca();
    for(int i=0 ; i<C.qtd ; i++) {
         Line.trocaValor(C.x[i], C.y[i], C.x[i]+deltaX, C.y[i]+deltaY);
    }      
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    //desenhaNaTela();           
}

void CentroDeMassa()
{      
    C = Line.busca();       
	for(int f = 0; f < C.qtd; f++)
	{
		centroX += C.x[f];
		centroY += C.y[f];             
	}
   
	centroX /= C.qtd + 1;
	centroY /= C.qtd + 1;
}

void rotacao(int angle) {
    CentroDeMassa();     
	translacao(-centroX, -centroY); 
     
    double angleX, angleY;
    
    C = Poly.busca();    
    for(int i=0 ; i<C.qtd ; i++) {
         angleX = C.x[i]*cos(angle)-C.y[i]*sin(angle);
         angleY = C.x[i]*sin(angle)+C.y[i]*cos(angle);
         Poly.trocaValor(C.x[i], C.y[i], angleX, angleY);
    }
    
    C = Line.busca();    
    for(int i=0 ; i<C.qtd ; i++) {
         angleX = C.x[i]*cos(angle)-C.y[i]*sin(angle);
         angleY = C.x[i]*sin(angle)+C.y[i]*cos(angle);   
         Line.trocaValor(C.x[i], C.y[i], angleX, angleY);
    }      
    translacao(centroX, centroY);
    
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    desenhaNaTela();
}

void escala(int zoom) {
    C = Poly.busca();
    for(int i=0 ; i<C.qtd ; i++) {
         if(zoom > 0)   
              Poly.trocaValor(C.x[i], C.y[i], C.x[i]*1.2, C.y[i]*1.2);
         else if(zoom < 0)
              Poly.trocaValor(C.x[i], C.y[i], (int)C.x[i]*0.8, (int)C.y[i]*0.8);
    }
    C = Line.busca();
    for(int i=0 ; i<C.qtd ; i++) {
         if(zoom > 0)   
              Line.trocaValor(C.x[i], C.y[i], C.x[i]*1.2, C.y[i]*1.2);
         else if(zoom < 0)
              Line.trocaValor(C.x[i], C.y[i], (int)C.x[i]*0.8, (int)C.y[i]*0.8);
    }      
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    desenhaNaTela();
}
//*****************************************************************************
//*****************************************************************************


//######################################
void wait(int seconds) {
    clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    while(clock() < endwait) {}
}

//##############################################################################
class GLintPoint{
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
              GLint mouseY = screenHeight - y;
              
	          Line.inserir(mouseX, mouseY, countTypePolyLine);
	          
	          int auxID;
	          auxID = C.idPolyline[0];
	          int firstPoint;
              firstPoint = 0;
	          
	          C = Line.busca();
	          glBegin(GL_LINE_STRIP);
	          for(int i=0 ; i<C.qtd ; i++) {
                   if(auxID == C.idPolyline[i]) {
                        glVertex2f(C.x[i], C.y[i]);
                   }
                   else {
                        glVertex2f(C.x[firstPoint], C.y[firstPoint]);
                        glEnd();  
                        glFlush();
                        firstPoint = i;
                        auxID = C.idPolyline[i];
                        
                        glBegin(GL_LINE_STRIP); 
                        glVertex2f( C.x[i] , C.y[i]);
                   }  
              }
              glEnd();
	          glFlush();	          
         }
         else if (inserirPol == true) { //Quando ativa a tecla 'p'
	          GLint mouseX = x;
              GLint mouseY = screenHeight - y;
              
	          Poly.inserir(mouseX, mouseY, countTypePolyLine); 
	          
              drawDot ( mouseX, mouseY );
              glFlush();
         }
         else if(erasePoint==true) { //Quando ativa a tecla 'a'
              GLint mouseX = x;
              GLint mouseY = screenHeight - y;
              erasePointPolyline(mouseX,mouseY);
         }
         else if(nextLocation != 0) { //Quando ativa a tecla 'm'
              GLint mouseX = x;
              GLint mouseY = screenHeight - y;
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
	     
	}
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	     last = -1;	         // reset the list to empty
}

void myMovedMouse(  int mouseX, int mouseY)
{
} 

void myKeyboard ( unsigned char key, int mouseX, int mouseY )
{
    GLint x = mouseX;
    GLint y = screenHeight - mouseY;
    int auxType;
    
    switch( key )
    {
         case 'p': //Cria um novo polígono 	          
              inserirPol = true; 	      
              erasePoint = false;    
              nextLocation = 0;
              modoTranslacao = modoRotacao = modoEscala = 0;
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
              modoTranslacao = modoRotacao = modoEscala = 0;      
              countTypePolyLine++;
              desenhaNaTela();	          
              break;
              
         case 'r': //Cria um conjunto de linhas conectadas
              inserirLinConect = true;  
              erasePoint = false;     
              nextLocation = 0;     
              modoTranslacao = modoRotacao = modoEscala = 0;  
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
              modoTranslacao = modoRotacao = modoEscala = 0;   
              countTypePolyLine++;
              desenhaNaTela();
              break;
              
         case 'a': //Apaga o ponto mais próximo do mouse
              inserirLinConect = false;
              inserirPol = false;
              erasePoint = true;
              nextLocation = 0;
              modoTranslacao = modoRotacao = modoEscala = 0;
              break;
              
         case 'm': //Move o ponto para a próxima localização (2 cliques, um marca e o outro clique move o ponto)
              inserirLinConect = false;
              inserirPol = false;    
              erasePoint = false;
              nextLocation = 1;
              modoTranslacao = modoRotacao = modoEscala = 0;
              break;
              
         case 'd': //Limpa a tela e redesenha todos os polígonos
              inserirLinConect = false;
              inserirPol = false;    
              erasePoint = false;
              nextLocation = 0;
              modoTranslacao = modoRotacao = modoEscala = 0;
              
              glClear(GL_COLOR_BUFFER_BIT);
              glFlush(); 
              wait(2);
              desenhaNaTela();
              break;
              
         case 'q': //Termina o programa
              Poly.~Fila();
              Line.~Fila();
              exit ( -1 );
              break;
              
         case 's': //Salva os polígonos em um arquivo texto, utilizando o formato de arquivo usado no programa 4
              inserirLinConect = false;
              inserirPol = false;    
              erasePoint = false;
              nextLocation = 0;
              modoTranslacao = modoRotacao = modoEscala = 0;
              
              saveFile("coordenadas.dat");
              break;
              
         case 'l': //Leitura e visualização do arquivo de polígonos
              inserirLinConect = false;
              inserirPol = false;    
              erasePoint = false;
              nextLocation = 0;
              modoTranslacao = modoRotacao = modoEscala = 0;
              
              glClear(GL_COLOR_BUFFER_BIT);              
              glFlush();              
              readFile("coordenadas.dat"); 
              break;
              
         case 'T': //Ativa a translação
              inserirLinConect = false;
              inserirPol = false;    
              erasePoint = false;
              nextLocation = 0;
              modoRotacao = modoEscala = 0;
              modoTranslacao = 1;
              break;
         case 'E': //Ativa a Escala
              inserirLinConect = false;
              inserirPol = false;    
              erasePoint = false;
              nextLocation = 0;
              modoRotacao = modoTranslacao = 0;
              modoEscala = 1;
              break;
         case 'R': //Ativa a Rotação
              inserirLinConect = false;
              inserirPol = false;    
              erasePoint = false;
              nextLocation = 0;
              modoEscala = modoTranslacao = 0;
              modoRotacao = 1;
              break;
                  
         default :
              inserirLinConect = false;
              inserirPol = false;    
              erasePoint = false;
              nextLocation = 0;
              modoRotacao = modoEscala = modoTranslacao = 0;   
	          break;
    }  
}

void GerenciaTeclasEspeciais(int key, int mouseX, int mouseY)
{
     switch(key)
     {
         case GLUT_KEY_UP:
              if(modoTranslacao!=0) {
                   translacao(0,10); 
                   desenhaNaTela();
              }      
              else if(modoRotacao!=0)
                   rotacao(45);
              else if(modoEscala!=0)
                   escala(5);
              break;
              
         case GLUT_KEY_DOWN:
              if(modoTranslacao!=0) {
                   translacao(0,-10);  
                   desenhaNaTela();
              }
              else if(modoEscala!=0)
                   escala(-5);
              break;
              
         case GLUT_KEY_RIGHT:
              if(modoTranslacao!=0) {
                   translacao(10,0);  
                   desenhaNaTela();
              }
              break;                  
         case GLUT_KEY_LEFT:
              if(modoTranslacao!=0) {
                   translacao(-10,0); 
                   desenhaNaTela();
              }
              break;   
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
    modoTranslacao = modoRotacao = modoEscala = 0;
    
    cout << "[p] - Cria um novo polígono " << endl;
    cout << "[o] - Finaliza o polígono" << endl;
    cout << "[r] - Cria um conjunto de linhas conectadas" << endl;
    cout << "[h] - Finaliza o conjunto de linhas conectadas" << endl;
    cout << "[a] - Apaga o ponto mais próximo do mouse" << endl;
    cout << "[m] - Move o ponto para a próxima localização (2 cliques, um marca e o outro clique move o ponto)" << endl;
    cout << "[d] - Limpa a tela e redesenha todos os polígonos" << endl;
    cout << "[q] - Termina o programa" << endl;
    cout << "[s] - Salva os polígonos em um arquivo texto" << endl;
    cout << "[l] - Leitura e visualização do arquivo de polígonos" << endl << endl;
    cout << "[T] - Ativa a translação" << endl;
    cout << "[E] - Ativa a Escala" << endl;
    cout << "[R] - Ativa a Rotação" << endl;
    cout << "[Key_UP] - Controla a Translação ou Escala ou Rotação" << endl;
    cout << "[Key_DOWN] - Controla a Translação ou Escala" << endl;
    cout << "[Key_LEFT] - Controla a Translação" << endl;
    cout << "[Key_RIGHT] - Controla a Translação" << endl;
    
    glutInit(&argc, argv);	//initialize toolkit
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );	//set display mode
    glutInitWindowSize(500, 500);		//set window size on screen
    glutInitWindowPosition( 100, 150 ); 	//set window position on screen
    glutCreateWindow(argv[0]);		//open screen widow
    init();
    glutMouseFunc( myMouse );
    glutMotionFunc( myMovedMouse );
    glutKeyboardFunc( myKeyboard );
    glutSpecialFunc(GerenciaTeclasEspeciais);
    glutDisplayFunc( display );
    glutMainLoop();			//go into perpetual loop
    return 0;
}
