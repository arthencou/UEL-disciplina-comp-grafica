#include<iostream>
using namespace std;

struct NoFila
{
    NoFila *prox;
    int id;
    int numberOfPixel;
};

class Fila
{
    private:
         NoFila *inicio;
         NoFila *fim;
    public:
         Fila() { inicio = NULL; }                
                
         void inserir(int identificador, int qttdPixel);
         void remover();
         int buscaMenorQttdPixel();
         int buscaMaiorQttdPixel();
         
         ~Fila();
};

void Fila::inserir(int identificador, int qttdPixel)
{
    NoFila *novo;
    novo = new NoFila;
    novo->id = identificador;
    novo->numberOfPixel = qttdPixel;
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
    if( inicio == NULL )
         cout << "# UNDERFLOW #" << endl;
    else
    {
         NoFila *aux;
         aux = inicio;
         inicio = inicio->prox;
         delete aux;
    }
}

int Fila::buscaMenorQttdPixel()
{
    int menorQttdPixel;
    int identif;
    menorQttdPixel = inicio->numberOfPixel;
    identif = inicio->id;
    
    if(inicio == NULL)
         return 0;
    else {
         NoFila *aux;
         aux = inicio->prox;
         
         if(aux!=NULL) {       
              while(aux->prox!=NULL){
                   if(menorQttdPixel > aux->numberOfPixel) {
                        menorQttdPixel = aux->numberOfPixel;
                        identif = aux->id;
                   }
                   aux = aux->prox;
              }
         }
    }
    printf("\n\nID do Objeto: %d \nQuantidade de Pixel: %d \n\n", identif, menorQttdPixel);
    return identif;
}

int Fila::buscaMaiorQttdPixel()
{
    int maiorQttdPixel;
    int identif;
    maiorQttdPixel = inicio->numberOfPixel;
    identif = inicio->id;
    
    if(inicio == NULL)
         return 0;
    else {
         NoFila *aux;
         aux = inicio->prox;
         
         if(aux!=NULL) {       
              while(aux->prox!=NULL){
                   if(maiorQttdPixel < aux->numberOfPixel) {
                        maiorQttdPixel = aux->numberOfPixel;
                        identif = aux->id;
                   }
                   aux = aux->prox;
              }
         }
    }
    printf("\n\nID do Objeto: %d \nQuantidade de Pixel: %d \n\n", identif, maiorQttdPixel);
    return identif;
}


Fila::~Fila()
{
    while( inicio != NULL )
         remover();
}
