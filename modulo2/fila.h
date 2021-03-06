/*
 * M�dulo 2
 * Nomes:
 *      Arthur Henrique Coutinho
 *      Breno Naodi Kusunoki
 *      Luiz Guilherme Castilho Martins
 */
#include <iostream>

using namespace std;

class AllPoints {
public:
        int x[100];
        int y[100];
        int qtd;
        int idPolyline[100];
};

class Node
{
public:
        Node *prox;
        int x, y;
        int idPolyL; //Armazena em qual conjunto de poligonos esse ponto pertence
};

class Fila
{
private:
        Node *inicio;
        Node *fim;
public:
        Fila() { inicio = NULL; }

        void inserir( int coordenadaX, int coordenadaY, int idPoligono);
        void remover();
        void remover(int coordX, int coordY);
        AllPoints* busca();
        void trocaValor(int oldCoordX, int oldCoordY, int newCoordX, int newCoordY);                
        ~Fila();
};

void Fila::inserir( int coordenadaX, int coordenadaY, int idPoligono)
{
        Node *novo;
        novo = new Node;
        novo->x = coordenadaX;
        novo->y = coordenadaY;
        novo->idPolyL = idPoligono;
        novo->prox = NULL;
        if( inicio == NULL ) {
                fim = novo;
                inicio = novo;
        } 
        else {
                fim->prox = novo;
                fim = fim->prox;
        }
}

void Fila::remover()
{
        if( inicio != NULL ) {
                Node *aux;
                aux = inicio;
                inicio = inicio->prox;
                delete aux;
        }
}

void Fila::remover(int coordX, int coordY) {
        if( inicio!=NULL ) {
                Node *aux, *aux2;
                aux = aux2 = inicio;

                if( aux->prox==NULL ) {
                        remover();
                }
                else {
                        bool achou;
                        achou = false;          

                        while( aux->prox!=NULL && achou==false) {
                                if( aux->x==coordX && aux->y==coordY ) {
                                        achou = true;
                                        if( aux==inicio ) {
                                                inicio = inicio->prox;
                                                aux = inicio;
                                                delete aux2;
                                        }
                                        else {
                                                aux2->prox = aux->prox;
                                                delete aux;
                                        }
                                }
                                else {
                                        aux2 = aux;
                                        aux = aux->prox;
                                } 
                        }
                        if( aux->prox==NULL && aux->x==coordX && aux->y==coordY) {
                                aux2->prox = NULL;
                                delete aux;
                        }
                }
        }
}

AllPoints* Fila::busca() {
        int count = 0;
        AllPoints* newCoord = new AllPoints();
        for(int i=0 ; i<100 ; i++) {
                newCoord->x[i] = 0;
                newCoord->y[i] = 0;
                newCoord->idPolyline[i] = 0;
        }
        newCoord->qtd = 0;

        for( Node *aux = inicio ; aux != NULL ; aux=aux->prox ) {
                newCoord->x[count] = aux->x;
                newCoord->y[count] = aux->y;
                newCoord->idPolyline[count] = aux->idPolyL;
                count++;
                newCoord->qtd = count;
        }
        return newCoord;
}

void Fila::trocaValor(int oldCoordX, int oldCoordY, int newCoordX, int newCoordY) {
        if( inicio!=NULL ) {
                Node *aux;
                aux = inicio;
                bool achou;
                achou = false;

                while( aux->prox!=NULL && achou==false) {
                        if( aux->x==oldCoordX && aux->y==oldCoordY )  {
                                aux->x = newCoordX;
                                aux->y = newCoordY;
                                achou = true;
                        }
                        else {
                                aux = aux->prox;
                        }
                }
                if( aux->prox==NULL && aux->x==oldCoordX && aux->y==oldCoordY ) {
                        aux->x = newCoordX;
                        aux->y = newCoordY;
                }
        }
}

Fila::~Fila()
{
        while( inicio != NULL ) {
                remover();
        }
}
