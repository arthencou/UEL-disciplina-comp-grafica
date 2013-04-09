#include <iostream>
#include <cstdlib>
#include <fstream>
#include <math.h>
#include <time.h>

#include "glut.h"
#include "Fila.h"

#define ARROW_LEFT 100
#define ARROW_UP 101
#define ARROW_RIGHT 102
#define ARROW_DOWN 103

#define PLUS_DELTA 4
#define MINUS_DELTA -4

#define SCREEN_H 500

Fila Poly, Line;
bool inserirPol, inserirLinConect, erasePoint;
int countTypePolyLine, nextLocation, localX, localY;


//########################################Desenha os poligonos registrados na fila
void desenhaNaTela()
{
        AllPoints* All = Poly.busca(); 

        int auxID;
        auxID = All->idPolyline[0];

        glBegin(GL_POLYGON);
        glColor3f(0.0, 0.0, 0.0);
        for(int i=0 ; i<All->qtd ; i++)
        {
                if(auxID == All->idPolyline[i]) {
                        glVertex2i( All->x[i] , All->y[i]);
                }
                else {
                        glEnd();  
                        glFlush();
                        auxID = All->idPolyline[i];

                        glBegin(GL_POLYGON);
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i( All->x[i] , All->y[i]);
                }
        }
        glEnd();  
        glFlush();

        delete All;

        //--------------------------------------------------------------------------

        All = Line.busca();

        auxID = All->idPolyline[0];
        int firstPoint;
        firstPoint = 0;

        glBegin(GL_LINE_STRIP); 
        for(int i = 0; i <= All->qtd; i++) 
        {
                if(i != All->qtd) 
                { 
                        if(auxID == All->idPolyline[i]) {
                                glVertex2i(All->x[i], All->y[i]);
                        }
                        else {
                                glVertex2i(All->x[firstPoint], All->y[firstPoint]);
                                glEnd();  
                                glFlush();
                                firstPoint = i;
                                auxID = All->idPolyline[i];

                                glBegin(GL_LINE_STRIP); 
                                glVertex2i( All->x[i] , All->y[i]);
                        }
                }
                else {
                        glVertex2i(All->x[firstPoint], All->y[firstPoint]);
                }
        }
        glEnd();  
        glFlush();

        delete All;        
}

//########################################Grava as AllPoints no arquivo
void saveFile(char *fileName)
{
        int auxCountLines;
        auxCountLines = 0;

        AllPoints* All = Poly.busca();
        auxCountLines = All->qtd;
        delete All;

        All = Line.busca();
        auxCountLines = auxCountLines + All->qtd;

        ofstream outStream(fileName);
        if(outStream.is_open())
        {
                outStream << auxCountLines; 
                outStream << "\n";
                cout << "SALVANDO NO ARQUIVO \n";
                cout << auxCountLines << "\n";

                for(int i=0 ; i<All->qtd ; i++) {
                        outStream << All->x[i] << " " << All->y[i] << " " << All->idPolyline[i] << " " << 0 <<"\n";
                        cout << All->x[i] << All->y[i] << All->idPolyline[i] << "\n";
                }
                delete All;

                All = Poly.busca();
                cout << "\n";
                for(int i=0 ; i<All->qtd ; i++) {
                        outStream << All->x[i] << " " << All->y[i] << " " << All->idPolyline[i] << " " << 1 <<"\n";
                        cout << All->x[i] << All->y[i] << All->idPolyline[i] << "\n";
                }  
                delete All;

                outStream.close();
        }
}

//######################################Lê um arquivo com as AllPoints do poligono
void readFile(char *fileName)
{
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
void erasePointPolyline(int x, int y)
{
        /**/
        double distPoly, distLine, auxCalc; 
        int coordX_Poly, coordY_Poly, coordX_Line, coordY_Line;                          
        distPoly = distLine = 100000;
        coordX_Poly = coordY_Poly = coordX_Line = coordY_Line = 0;

        AllPoints* All = Poly.busca();
        for(int i=0 ; i<All->qtd ; i++) 
        {
                auxCalc = sqrt( ((x-All->x[i])*(x-All->x[i])) + ((y-All->y[i])*(y-All->y[i])) );    
                if(auxCalc < distPoly) {
                        distPoly = auxCalc;
                        coordX_Poly = All->x[i];
                        coordY_Poly = All->y[i];
                }
        }
        delete All;

        All = Line.busca();
        for(int i=0 ; i<All->qtd ; i++) 
        {
                auxCalc = sqrt( ((x-All->x[i])*(x-All->x[i])) + ((y-All->y[i])*(y-All->y[i])) );
                if(auxCalc < distLine) {
                        distLine = auxCalc;
                        coordX_Line = All->x[i];
                        coordY_Line = All->y[i];
                }
        }
        delete All;

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
        /**/

}

//######################################

int* getNearPointAllInfo(int x, int y)
{
        double distPoly;
        double distLine;
        double auxCalc;
        int coordX_Poly;
        int coordY_Poly;
        int coordX_Line;
        int coordY_Line;

        distPoly = 100000;
        distLine = 100000;
        auxCalc = 0;
        coordX_Poly = 0;
        coordY_Poly = 0;
        coordX_Line = 0;
        coordY_Line = 0;

        AllPoints* All = Poly.busca();
        int id_Poly;
        for(int i=0 ; i<All->qtd ; i++) 
        {
                auxCalc = sqrt( ((x-All->x[i])*(x-All->x[i])) + ((y-All->y[i])*(y-All->y[i])) );    
                if(auxCalc < distPoly) {
                        distPoly = auxCalc;
                        coordX_Poly = All->x[i];
                        coordY_Poly = All->y[i];
                        id_Poly = All->idPolyline[i];
                }
        }
        delete All;

        All = Line.busca();
        int id_Line;
        for(int i=0 ; i<All->qtd ; i++) 
        {
                auxCalc = sqrt( ((x-All->x[i])*(x-All->x[i])) + ((y-All->y[i])*(y-All->y[i])) );    
                if(auxCalc < distLine) {
                        distLine = auxCalc;
                        coordX_Line = All->x[i];
                        coordY_Line = All->y[i];
                        id_Line = All->idPolyline[i];
                }
        }
        delete All;

        int* ret = new int[3];
        if( distPoly < distLine ) {
                if( coordX_Poly!=0 & coordY_Poly!=0) {
                        ret[0] = coordX_Poly;
                        ret[1] = coordY_Poly;
                        ret[2] = id_Poly;
                }
        }
        else /**if( distPoly > distLine)/**/ {
                if( coordX_Line!=0 & coordY_Line!=0) {
                        ret[0] = coordX_Line;
                        ret[1] = coordY_Line;
                        ret[2] = id_Line;
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
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glFlush();
}

void drawDot( int x, int y )
{
        glBegin( GL_POINTS );     
        glVertex2i( x, y );   //draw a points
        glEnd();
}

void myMouse(int button, int state, int x, int y)
{
        #define NUM 20

        static int last = -1;             // last index used so far 

        // test for mouse button as well as for a full array
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && last < NUM -1)
        {
                if(inserirLinConect == true)  //Quando ativa a tecla 'r'
                {
                        GLint mouseX = x;
                        GLint mouseY = SCREEN_H - y;

                        Line.inserir(mouseX, mouseY, countTypePolyLine);

                        AllPoints* All = Poly.busca();
                        int auxID = All->idPolyline[0];
                        delete All;

                        All = Line.busca();
                        int firstPoint;
                        firstPoint = 0;
                        glBegin(GL_LINE_STRIP);
                        for(int i=0 ; i<All->qtd ; i++)
                        {
                                if(auxID == All->idPolyline[i]) {
                                        glVertex2i(All->x[i], All->y[i]);
                                }
                                else {
                                        glVertex2i(All->x[firstPoint], All->y[firstPoint]);
                                        glEnd();  
                                        glFlush();
                                        firstPoint = i;
                                        auxID = All->idPolyline[i];

                                        glBegin(GL_LINE_STRIP); 
                                        glVertex2i( All->x[i] , All->y[i]);
                                }
                        }
                        delete All;

                        glEnd();
                        glFlush();            
                }
                else if (inserirPol == true) { //Quando ativa a tecla 'p'
                        GLint mouseX = x;
                        GLint mouseY = SCREEN_H - y;

                        Poly.inserir(mouseX, mouseY, countTypePolyLine); 

                        drawDot ( mouseX, mouseY );
                        glFlush();
                }
                else if(erasePoint==true) { //Quando ativa a tecla 'a'
                        GLint mouseX = x;
                        GLint mouseY = SCREEN_H - y;
                        erasePointPolyline(mouseX,mouseY);
                }
                else if(nextLocation != 0) { //Quando ativa a tecla 'm'
                        GLint mouseX = x;
                        GLint mouseY = SCREEN_H - y;
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
                        last = -1;           // reset the list to empty
                }
        }
}

void myMovedMouse(  int mouseX, int mouseY)
{
}

void myAsciiKeyboard ( unsigned char key, int mouseX, int mouseY )
{
        GLint x = mouseX;
        GLint y = SCREEN_H - mouseY;
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

                saveFile("AllPoints.dat");
                break;

                case 'l': //Leitura e visualização do arquivo de polígonos
                inserirLinConect = false;
                inserirPol = false;    
                erasePoint = false;
                nextLocation = 0;

                glClear(GL_COLOR_BUFFER_BIT);              
                glFlush();              
                readFile("AllPoints.dat"); 
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
                GLint mX = mouseX;
                GLint mY = SCREEN_H - mouseY;

                //encontrar a forma mais próxima do mouse
                int* ptProx = getNearPointAllInfo(mX, mY);
                int idFormaProxima = ptProx[2];

                delete ptProx;

                printf("\n\n\n\n------ID da forma mais proxima eh %d------", idFormaProxima);

                //somar todos os seus pontos com o vetor de deslocamento
                AllPoints* _Poly = Poly.busca();
                AllPoints* _Line = Line.busca();
                int i = 0;
                int polyQtd = _Poly->qtd;
                int lineQtd = _Line->qtd;
                bool buscaPoly = TRUE;
                bool buscaLine = TRUE;
                //while( (buscaPoly = (i < polyQtd)) || (buscaLine = (i < lineQtd)) )
                do
                {
                        /**/
                        if( buscaPoly )
                        {
                                printf("\n\n\tAnalisando ponto %d %d em Poly", _Poly->x[i], _Poly->y[i]);
                                if ( _Poly->idPolyline[i] == idFormaProxima ){
                                        int x = _Poly->x[i];
                                        int y = _Poly->y[i];
                                        printf("\n\n\t\tMovendo ponto %d %d", x, y);
                                        Poly.trocaValor( x, y, x+desloque[0], y+desloque[1] );
                                }
                        }
                        /**/

                        printf("\n\ni=%d, lineQtd=%d, buscaLine = %s",i, lineQtd, ((buscaLine)?"TRUE":"FALSE") );
                        if( buscaLine )
                        {
                                printf("\n\n\tAnalisando ponto %d %d em Line", _Line->x[i], _Line->y[i]);
                                if ( _Line->idPolyline[i] == idFormaProxima ){
                                        int x = _Line->x[i];
                                        int y = _Line->y[i];
                                        printf("\n\n\t\tMovendo ponto %d %d", x, y);
                                        Line.trocaValor( x, y, x+desloque[0], y+desloque[1] );
                                }
                        }

                        ++i;
                        if ( i < polyQtd ) {
                                buscaPoly = TRUE;
                        }
                        else {
                                buscaPoly = FALSE;
                        }
                        if ( i< lineQtd ) {
                                buscaLine = TRUE;
                        }
                        else {
                                buscaLine = FALSE;
                        }
                        /**
                        buscaPoly = (i < polyQtd);
                        buscaLine = (i < lineQtd);
                        /**/
                } while( buscaPoly | buscaLine );

                glClear(GL_COLOR_BUFFER_BIT);
                glFlush();
                desenhaNaTela();

                delete _Poly;
                delete _Line;
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

        glutInit(&argc, argv);  //initialize toolkit
        glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );  //set display mode
        glutInitWindowSize(500, 500);   //set window size on screen
        glutInitWindowPosition( 100, 150 );   //set window position on screen
        glutCreateWindow(argv[0]);    //open screen widow
        init();
        glutMouseFunc( myMouse );
        glutMotionFunc( myMovedMouse );
        glutKeyboardFunc( myAsciiKeyboard );
        glutSpecialFunc( mySpecialKeyboard );
        glutDisplayFunc( display );
        glutMainLoop();     //go into perpetual loop

        return 0;
}
