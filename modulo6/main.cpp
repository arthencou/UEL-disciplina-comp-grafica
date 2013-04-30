/*
* Módulo 6
* Nomes:
*      Arthur Henrique Coutinho
*      Breno Naodi Kusunoki
*      Luiz Guilherme Castilho Martins
*/
#include <iostream>
#include <string>
#include <vector>

#include "mesh.h"
#include "camera.h"

#define PI 3.14159265f

using namespace std;

Mesh mesh;
Camera camera;

double centroX, centroY, centroZ;
double x, y, z;

double _left = -100.0;
double _right = 100.0;
double _bottom = -100.0;
double _top = 100.0;


void myInit(void)
{
    // set the material light properties
    GLfloat mat_ambient[] = {0.19225, 0.19225, 0.19225, 1.0};
    GLfloat mat_diffuse[] = {0.50754, 0.50754, 0.50754, 1.0};
    GLfloat mat_specular[] = {0.508273, 0.508273, 0.508273}; 
    GLfloat mat_shininess[] = {0.4};

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);

    // set the light source properties  
    GLfloat lightIntensity[] = {0.7f, 0.7f, 0.7f, 0.7f};
    GLfloat light_position[] = {2.0f,6.0f,3.0f,1.0f};
    GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
     
    glClearColor(1.0f,1.0f,1.0f,0.1f);        // set white background color
    glColor3f(0.7f, 0.7f, 0.7f);          // set the drawing color
    glPointSize(1.0);                 // a 'dot' is 4 by 4 pixels
    glLineWidth(0.5);      
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double winHt = 50.0;
    glOrtho(-winHt*64/48.0, winHt*64/48.0, -winHt, winHt, 0.1, 100.0);
    float eyeX=2.3, eyeY=1.3, eyeZ=2, centerX=0, centerY=0.1, centerZ=0.1;
    gluLookAt( eyeX, eyeY, eyeZ, 
        centerX, centerY, centerZ, 
        0.0, 1.0, 0.0 );
    
    x = y = z = 5;

    Point3 ey(x,y,z);
    Point3 lo(0,0,0);
    Vector3 up(0,1,0);
    camera.set(ey,lo,up);
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.5f, 0.5f, 0.5f);
    mesh.drawMesh();
}
     
void Translacao(float x, float y, float z)
{
    for(unsigned int f = 0; f < mesh.mVertices.size(); f++)
    {
         float XVert = mesh.mVertices[f].getX();
         float YVert = mesh.mVertices[f].getY();
         float ZVert = mesh.mVertices[f].getZ();
         mesh.mVertices[f].setPoint(XVert+x, YVert+y, ZVert+z);               
    }
}

void CentroDeMassa()
{      
       
    for(unsigned int f = 0; f < mesh.mVertices.size(); f++)
    {
        centroX += mesh.mVertices[f].getX();
        centroY += mesh.mVertices[f].getY();
        centroZ += mesh.mVertices[f].getZ();               
    }
   
    centroX /= mesh.mVertices.size();
    centroY /= mesh.mVertices.size();
    centroZ /= mesh.mVertices.size();
}

void Escala(double scx, double scy, double scz)
{
    CentroDeMassa();
    Translacao(-centroX, -centroY, -centroZ);
    for(unsigned int f = 0; f < mesh.mVertices.size(); f++)
    {
        float XVert = mesh.mVertices[f].getX();
        float YVert = mesh.mVertices[f].getY();
        float ZVert = mesh.mVertices[f].getZ();
        mesh.mVertices[f].setPoint(XVert*scx, YVert*scy, ZVert*scz);               
    }
    Translacao(centroX, centroY, centroZ);     
} 

void RotacaoEixoZ(double angle)
{
    CentroDeMassa();     
    Translacao(-centroX, -centroY, -centroZ);
    for(unsigned int f = 0; f < mesh.mVertices.size(); f++)
    {
        float XVert = mesh.mVertices[f].getX();
        float YVert = mesh.mVertices[f].getY();
        float ZVert = mesh.mVertices[f].getZ();
        double XLinha = (cos(angle) * XVert) - (sin(angle) * YVert);
        double YLinha = (sin(angle) * XVert) + (cos(angle) * YVert);
        mesh.mVertices[f].setPoint(XLinha, YLinha, ZVert);               
    }       
    Translacao(centroX, centroY, centroZ);   
}

void RotacaoEixoX(double angle)
{
    CentroDeMassa();     
    Translacao(-centroX, -centroY, -centroZ);
    for(unsigned int f = 0; f < mesh.mVertices.size(); f++)
    {
        float XVert = mesh.mVertices[f].getX();
        float YVert = mesh.mVertices[f].getY();
        float ZVert = mesh.mVertices[f].getZ();
        double YLinha = (cos(angle) * YVert) - (sin(angle) * ZVert);
        double ZLinha = (sin(angle) * YVert) + (cos(angle) * ZVert);
        mesh.mVertices[f].setPoint(XVert, YLinha, ZLinha);    
    }
    Translacao(centroX, centroY, centroZ);   
}

void RotacaoEixoY(double angle)
{
    CentroDeMassa();     
    Translacao(-centroX, -centroY, -centroZ);
    for(unsigned int f = 0; f < mesh.mVertices.size(); f++)
    {
        float XVert = mesh.mVertices[f].getX();
        float YVert = mesh.mVertices[f].getY();
        float ZVert = mesh.mVertices[f].getZ();
        double XLinha = (sin(angle) * ZVert) + (cos(angle) * XVert);
        double ZLinha = (cos(angle) * ZVert) - (sin(angle) * XVert);
        mesh.mVertices[f].setPoint(XLinha, YVert, ZLinha);      
    }
    Translacao(centroX, centroY, centroZ);
}

float TranslacaoBool, RotacaoBool, EscalaBool; 

void GerenciaTeclado(unsigned char key, int mouseX, int mouseY)
{
    switch(key)
    {
        //Modulo 4
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

        case 'E':
        TranslacaoBool = RotacaoBool = 0.0;
        EscalaBool = 1.1;
        break;

        case 'e':
        TranslacaoBool = RotacaoBool = 0.0;
        EscalaBool = 0.9;
        break;

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

        
        //Modulo 5
        case 'A':
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(_left += 5, _right -= 5, _bottom += 5, _top -= 5, -200, 200);
        myDisplay();
        break;
        
        case 'a':
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(_left -= 5, _right += 5, _bottom -= 5, _top += 5, -200, 200);
        //myDisplay();
        glutPostRedisplay();
        break;
        
        case 'S':
        camera.pitch(-10);
        myDisplay();
        break;
        
        case 's':
        camera.pitch(10);
        myDisplay();
        break;
        
        case 'D':
        camera.roll(10);
        myDisplay();
        break;
        
        case 'd':
        camera.roll(-10);
        myDisplay();
        break;

        case 'F':
        camera.yaw(10);
        myDisplay();
        break;

        case 'f':
        camera.yaw(-10);
        myDisplay();
        break;
        

        case 'Q':
        exit(0);
        break;
        
        case 'q':
        exit(0);
        break;


        //Modulo 6
        case 'L':
        glShadeModel(GL_FLAT);
        glutPostRedisplay();
        break;

        case 'l':
        glShadeModel(GL_SMOOTH);
        glutPostRedisplay();
        break;
    }
}

int main(int argc, char** argv)
{    
    DIR *pdir;
    struct dirent *pent;
    pdir = opendir(".");
    
    if (!pdir)
    {
        cout << "Erro ao procurar arquivos OBJ" << endl;
        return 1;
    }
    
    string arq;
    size_t found;
    vector<string> arquivos_obj;
    
    while ( (pent = readdir(pdir)) )
    {
        arq = pent->d_name;
        found = arq.rfind(".obj");
        
        if( found != string::npos )
        arquivos_obj.push_back(arq);
    }
    
    for(unsigned int i = 0; i < arquivos_obj.size(); i++)
    cout << "[" << i+1 << "] - " << arquivos_obj[i] << endl;
    
    int opcao_sel = 0;
    while(opcao_sel <= 0 || opcao_sel > arquivos_obj.size())
    {
        cout << "Selecione o arquivo desejado: ";
        cin >> opcao_sel;
    }
    closedir(pdir);
    
    try
    {
        mesh.readFile(arquivos_obj[--opcao_sel]);
    }
    catch (string excessao)
    {
        cout << excessao;
        return 1;
    }
    
    cout << endl << endl;
    cout << "[A] - aproxima camera" << endl;
    cout << "[a] - distancia camera" << endl;
    cout << "[S] - rotacao sentido anti-horario - eixo x" << endl;
    cout << "[s] - rotacao sentido horario - eixo x" << endl;
    cout << "[D] - rotacao sentido anti-horario - eixo z" << endl;
    cout << "[d] - rotacao sentido horario - eixo z" << endl;
    cout << "[F] - rotacao sentido anti-horario - eixo y" << endl;
    cout << "[f] - rotacao sentido horario - eixo y" << endl;
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Modulo 6");
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(GerenciaTeclado);
    
    myInit();
    glutMainLoop();
    
    return 0;
}
