/*
* Módulo 4 - Exercício 1
* Nomes:
*      Arthur Henrique Coutinho
*      Breno Naodi Kusunoki
*      Luiz Guilherme Castilho Martins
*/
#include "glut.h"
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <math.h>
#include <vector>
#include <string>

using namespace std;

//################## Point3 class ##################
class Point3 {
    public:
    Point3();
    Point3(float, float, float);
    virtual ~Point3();
    void setPoint(float, float, float);
    float getX();
    float getY();
    float getZ();
    void draw(void);
    
    private:
    float x;
    float y;
    float z;
};

Point3::Point3()
{
}

Point3::Point3(float xx, float yy, float zz)
{
    x = xx;
    y = yy;
    z = zz;
}

void Point3::setPoint(float xx, float yy, float zz)
{
    x = xx;
    y = yy;
    z = zz;
}

float Point3::getX()
{
    return x;
}

float Point3::getY()
{
    return y;
}

float Point3::getZ()
{
    return z;
}

void Point3::draw()
{
    glBegin(GL_POINTS);
    glVertex3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
    glEnd();
}

Point3::~Point3()
{
}

//################## Norm class ##################
class Norm : public Point3 {
    public:
    Norm();
    Norm(float, float, float);
    virtual ~Norm();
};

Norm::Norm()
{
    Point3::setPoint(0.0,0.0,0.0);
}

Norm::Norm(float xx, float yy, float zz)
{
    Point3::setPoint(xx, yy, zz);
}

Norm::~Norm()
{
}

//################## VertexID ##################
class VertexID {
    public:
    int vertIndex; //Index of this vertex in the vertex list
    int normIndex; //Index of this vertex´s normal
};

//################## Face ##################
class Face {
    public:
    Face();
    virtual ~Face();
    void setFace(int[]);
    void setFace(int[],int[]);
    VertexID mVert[3];
    
};

Face::Face()
{
}

Face::~Face()
{
}

void Face::setFace(int v[])
{
    for(int i=0 ; i<3 ; i++)
    mVert[i].vertIndex = v[i];
}

void Face::setFace(int v[],int vn[])
{
    for(int i=0; i < 3; i++)
    {
        mVert[i].vertIndex = v[i];
        mVert[i].normIndex = vn[i];
    }
}

//################## Mesh ##################
class Mesh {
    public:
    Mesh();
    ~Mesh();
    int readFile(string fileName) throw(string);
    void drawMesh();
    
    vector<Face> mFaces;
    vector<Point3> mVertices;
    vector<Norm> nNormas;
};

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

int Mesh::readFile(string fileName) throw(string)
{
    fstream arquivoDat;
    arquivoDat.open(fileName.data(), fstream::in);
    
    if(!arquivoDat.good())
    {
        arquivoDat.close();
        throw string("Impossivel abrir arquivo .obj");
    }
    
    float x,y,z;
    
    int v[3];
    int vn[3];
    
    Point3* p;
    Face* f;
    
    char tmp = ' ';
    
    string vORf;
    
    while(!arquivoDat.eof())
    {
        switch(arquivoDat.peek())
        {
            case 'v':
            arquivoDat >> vORf >> x >> y;
            if(vORf != "vt")
            {
                arquivoDat >> z;
                p = new Point3;
                p->setPoint(x,y,z);
                this->mVertices.push_back(*p);
                delete p;
            }
            break;
            
            case 'f':
            f = new Face;
            arquivoDat >> vORf;
            for(int i = 0; i < 3; i++)
            {
                arquivoDat >> v[i];
                v[i]--;
                if(arquivoDat.peek() == '/')
                {
                    arquivoDat >> tmp >> vn[i];
                }
            }
            
            if(tmp == '/')
            {
                f->setFace(v,vn);
            }
            else
            {
                f->setFace(v);
            }
            this->mFaces.push_back(*f);
            delete f;
            break;
            default:
            char com[256];
            arquivoDat.getline(com,256);
            break;
        }
    }
    
    arquivoDat.close();
    return 0;
}

void Mesh::drawMesh() {
    for(unsigned int f = 0; f < this->mFaces.size(); f++)
    {
        float normalX=0, normalY=0, normalZ=0;
        for(int v = 0; v < 3; v++)
        {
            int iv = this->mFaces[f].mVert[v].vertIndex;
            normalX += this->mVertices[iv].getX();
            normalY += this->mVertices[iv].getY();
            normalZ += this->mVertices[iv].getZ();
        }
        normalX /= 3;
        normalY /= 3;
        normalZ /= 3;
        glBegin(GL_LINE_STRIP);
        for(int v = 0; v < 3 ; v++)
        {
            int iv = this->mFaces[f].mVert[v].vertIndex;
            
            glNormal3f(normalX, normalY, normalZ);
            glVertex3f(this->mVertices[iv].getX(), this->mVertices[iv].getY(), this->mVertices[iv].getZ());
        }
        glEnd();
    }
    glFlush();
    glutSwapBuffers();
}


//#############################################################################
//*****************************************************************************

Mesh teste;
GLfloat angle, fAspect;
double centroX = 0.0, centroY = 0.0, centroZ = 0.0;
double x = 0.0, y = 0.0, z = 0.0;

float TranslacaoBool, RotacaoBool, EscalaBool;

//################## myInit ##################
void myInit(void)
{
    glPointSize(3.0);
    glOrtho(-800.0, 800.0, -600.0, 600.0,-10,10);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    angle = 45;
}

//################## myDisplay ##################
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glViewport(0,0,800,600);
    teste.drawMesh();
    
}

//################## Especifica Parametros Visualizacao ##################
void EspecificaParametrosVisualizacao(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(angle,fAspect,0.5,700);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0,80,200, 0,0,0, 0,1,0);
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    if ( h == 0 ) h = 1;
    glViewport(0, 0, w, h);
    
    fAspect = (GLfloat)w/(GLfloat)h;
    EspecificaParametrosVisualizacao();
}

//################## Translacao ##################
void Translacao(float x, float y, float z)
{
    for(unsigned int f = 0; f < teste.mVertices.size(); f++)
    {
        float XVert = teste.mVertices[f].getX();
        float YVert = teste.mVertices[f].getY();
        float ZVert = teste.mVertices[f].getZ();
        teste.mVertices[f].setPoint(XVert+x, YVert+y, ZVert+z);
    }
}

//################## Centro de Massa ##################
void CentroDeMassa()
{
    
    for(unsigned int f = 0; f < teste.mVertices.size(); f++)
    {
        centroX += teste.mVertices[f].getX();
        centroY += teste.mVertices[f].getY();
        centroZ += teste.mVertices[f].getZ();
    }
    
    centroX /= teste.mVertices.size();
    centroY /= teste.mVertices.size();
    centroZ /= teste.mVertices.size();
}

//################## Escala ##################
void Escala(double scx, double scy, double scz)
{
    CentroDeMassa();
    Translacao(-centroX, -centroY, -centroZ);
    for(unsigned int f = 0; f < teste.mVertices.size(); f++)
    {
        float XVert = teste.mVertices[f].getX();
        float YVert = teste.mVertices[f].getY();
        float ZVert = teste.mVertices[f].getZ();
        teste.mVertices[f].setPoint(XVert*scx, YVert*scy, ZVert*scz);
    }
    Translacao(centroX, centroY, centroZ);
}

//################## Rotacao Eixo Z ##################
void RotacaoEixoZ(double angle)
{
    CentroDeMassa();
    Translacao(-centroX, -centroY, -centroZ);
    for(unsigned int f = 0; f < teste.mVertices.size(); f++)
    {
        float XVert = teste.mVertices[f].getX();
        float YVert = teste.mVertices[f].getY();
        float ZVert = teste.mVertices[f].getZ();
        double XLinha = (cos(angle) * XVert) - (sin(angle) * YVert);
        double YLinha = (sin(angle) * XVert) + (cos(angle) * YVert);
        teste.mVertices[f].setPoint(XLinha, YLinha, ZVert);
    }
    Translacao(centroX, centroY, centroZ);
}

//################## Rotacao Eixo X ##################
void RotacaoEixoX(double angle)
{
    CentroDeMassa();
    Translacao(-centroX, -centroY, -centroZ);
    for(unsigned int f = 0; f < teste.mVertices.size(); f++)
    {
        float XVert = teste.mVertices[f].getX();
        float YVert = teste.mVertices[f].getY();
        float ZVert = teste.mVertices[f].getZ();
        double YLinha = (cos(angle) * YVert) - (sin(angle) * ZVert);
        double ZLinha = (sin(angle) * YVert) + (cos(angle) * ZVert);
        teste.mVertices[f].setPoint(XVert, YLinha, ZLinha);
    }
    Translacao(centroX, centroY, centroZ);
}

//################## Rotacao Eixo Y ##################
void RotacaoEixoY(double angle)
{
    CentroDeMassa();
    Translacao(-centroX, -centroY, -centroZ);
    for(unsigned int f = 0; f < teste.mVertices.size(); f++)
    {
        float XVert = teste.mVertices[f].getX();
        float YVert = teste.mVertices[f].getY();
        float ZVert = teste.mVertices[f].getZ();
        double XLinha = (sin(angle) * ZVert) + (cos(angle) * XVert);
        double ZLinha = (cos(angle) * ZVert) - (sin(angle) * XVert);
        teste.mVertices[f].setPoint(XLinha, YVert, ZLinha);
    }
    Translacao(centroX, centroY, centroZ);
}

//################## Gerencia Mouse ##################
void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    if (state == GLUT_DOWN) {  // Zoom-in
        Escala(1.1, 1.1, 1.1);
        myDisplay();
    }
    if (button == GLUT_RIGHT_BUTTON)
    if (state == GLUT_DOWN) {  // Zoom-out
        Escala(0.9,0.9,0.9);
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

//################## Gerencia Teclado ##################
void GerenciaTeclado(unsigned char key, int mouseX, int mouseY)
{
    switch(key)
    {
        case 'x':
        if(EscalaBool!=0.0)
        Escala(EscalaBool, 1, 1);
        else if(TranslacaoBool!=0.0)
        Translacao(TranslacaoBool,0,0);
        else if(RotacaoBool!=0.0)
        RotacaoEixoX(M_PI/6);
        
        myDisplay();
        break;
        
        case 'y':
        if(EscalaBool!=0.0)
        Escala(1, EscalaBool, 1);
        else if(TranslacaoBool!=0.0)
        Translacao(0,TranslacaoBool,0);
        else if(RotacaoBool!=0.0)
        RotacaoEixoY(M_PI/6);
        myDisplay();
        break;
        
        case 'z':
        if(EscalaBool!=0.0)
        Escala(1, 1,EscalaBool);
        else if(TranslacaoBool!=0.0)
        Translacao(0,0,TranslacaoBool);
        else if(RotacaoBool!=0.0)
        RotacaoEixoZ(M_PI/6);
        
        myDisplay();
        break;
        case 'E':
        TranslacaoBool = RotacaoBool = 0.0;
        EscalaBool = 1.1;
        break;
        case 'e':
        TranslacaoBool = RotacaoBool = 0.0;
        EscalaBool = 0.9;
        break;
        case 'T':
        RotacaoBool = EscalaBool = 0.0;
        TranslacaoBool = 1;
        break;
        case 't':
        RotacaoBool = EscalaBool = 0.0;
        TranslacaoBool = -1;
        break;
        case 'R':
        EscalaBool = TranslacaoBool = 0;
        RotacaoBool = 1;
        break;
        case 'r':
        EscalaBool = TranslacaoBool = 0;
        RotacaoBool = -1;
        break;
    }
}

//################## Gerencia Teclas Especiais ##################
void GerenciaTeclasEspeciais(int key, int mouseX, int mouseY)
{
    switch(key)
    {
        case GLUT_KEY_UP:
        Translacao(0,1,0);
        myDisplay();
        break;
        case GLUT_KEY_DOWN:
        Translacao(0,-1,0);
        myDisplay();
        break;
        case GLUT_KEY_RIGHT:
        Translacao(1,0,0);
        myDisplay();
        break;
        case GLUT_KEY_LEFT:
        Translacao(-1,0,0);
        myDisplay();
        break;
        case GLUT_KEY_PAGE_UP:
        Translacao(0,0,1);
        myDisplay();
        break;
        case GLUT_KEY_PAGE_DOWN:
        Translacao(0,0,-1);
        myDisplay();
        break;
    }
}

//##################  main  ####################################
int main(int argc, char** argv)
{
    string objFile;
    TranslacaoBool = RotacaoBool = EscalaBool = 0.0;
    
    DIR *pdir;
    struct dirent *pent;
    pdir = opendir(".");
    
    if (!pdir)
    {
        cout << "Impossivel Abrir Diretorio" << endl;
        return 1;
    }
    
    string arq;
    size_t found;
    vector<string> objFiles;
    
    while ((pent=readdir(pdir)))
    {
        arq = pent->d_name;
        found = arq.rfind(".obj");
        
        if(found!=string::npos)
        objFiles.push_back(arq);
    }
    
    for(unsigned int i = 0; i < objFiles.size(); i++)
    cout << "[" << i+1 << "] - " << objFiles[i] << endl;
    
    int arquivoDes = 0;
    while(arquivoDes <= 0 || arquivoDes > objFiles.size())
    {
        cout << "Selecione o arquivo desejado: ";
        cin >> arquivoDes;
    }
    closedir(pdir);

    try
    {
        teste.readFile(objFiles[--arquivoDes]);
    }
    catch (string excessao)
    {
        cout << excessao;
        return 1;
    }
    
    cout << endl << endl;
    cout << "[x] - Controla a Translacao, Rotacao ou Escala no eixo X" << endl;
    cout << "[y] - Controla a Translacao, Rotacao ou Escala no eixo Y" << endl;
    cout << "[z] - Controla a Translacao, Rotacao ou Escala no eixo Z" << endl;
    cout << "[E] - Ativa Escala de 1.1x" << endl;
    cout << "[e] - Ativa Escala de 0.9x" << endl;
    cout << "[T] - Ativa Translacao positiva" << endl;
    cout << "[t] - Ativa Translacao negativa" << endl;
    cout << "[R] - Ativa a Rotacao" << endl;
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,600);
    glutCreateWindow("Visualizacao 3D");
    glutDisplayFunc(myDisplay);
    
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(GerenciaTeclado);
    glutSpecialFunc(GerenciaTeclasEspeciais);
    
    myInit();
    glutMainLoop();
    
    return EXIT_SUCCESS;
}
