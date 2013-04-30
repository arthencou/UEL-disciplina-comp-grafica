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

// Point3 *********************************************************************
class Point3 
{
    public:
         float x, y, z;  
         Point3();		
		 Point3(float, float, float);		
		 virtual ~Point3();		
		 void setPoint(float, float, float);		
		 float getX();		
		 float getY();		
		 float getZ();		
		 void draw(void);
		 
		 void set(float dx, float dy, float dz)
	     {
              x = dx; y = dy; z = dz;
         }
	
         void set(Point3& p)
	     {
              x = p.x; y = p.y; z = p.z;
         }
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

// Vector3 ********************************************************************
class Vector3 
{
    public:
         float x, y, z;  
         void set(float dx, float dy, float dz);
         void set(const Vector3& v);
         void flip(); // reverse this vector
         void setDiff(Point3& a, Point3& b); //set to difference a - b
         void normalize(); //adjust this vector to unit length
         Vector3() { x = y = z = 0; } //default constructor
         Vector3(float xx, float yy, float zz);
         Vector3(const Vector3& v);
         Vector3 cross(Vector3 b); //return this cross b
         float dot(Vector3 b); // return this dotted with b             
         
         Vector3 operator*(float scalar) // multiply vector by scalar
         {
              Vector3 c(scalar*x, scalar*y, scalar*z);
	          return c;   
         }
         
         Vector3 friend operator*(float scalar,Vector3 v) // multiply vector by scalar
         {
              Vector3 c(scalar*v.x, scalar*v.y, scalar*v.z);
	          return c;   
         }
         
         Vector3 operator+(const Vector3& v)
         {
              Vector3 c(x+v.x, y+v.y, z+v.z);
	          return c;   
         }
         
         Vector3 operator-(const Vector3& v)
         {
              Vector3 c(x-v.x, y-v.y, z-v.z);
	          return c;   
         }
};

void Vector3::set(float dx, float dy, float dz) 
{
    x = dx;  y = dy;  z = dz; 
}

void Vector3::set(const Vector3& v) 
{
    x = v.x;  y = v.y;  z = v.z; 
}

void Vector3::flip() 
{
    x = -x;  y = -y;  z = -z;  
}

void Vector3::setDiff(Point3& a, Point3& b) 
{
    x = a.x - b.x; 
    y = a.y - b.y; 
    z = a.z - b.z; 
}

void Vector3::normalize() 
{
    double sizeSq = x * x + y * y + z * z;
    if(sizeSq < 0.0000001)
		return; // does nothing to zero vectors;
    float scaleFactor = 1.0/(float)sqrt(sizeSq);
	
    x *= scaleFactor; 
    y *= scaleFactor; 
    z *= scaleFactor; 
}

Vector3::Vector3(float xx, float yy, float zz)
{
    x = xx;  y = yy;  z = zz;
}

Vector3::Vector3(const Vector3& v)
{
    x = v.x;  y = v.y;  z = v.z;
}

Vector3 Vector3::cross(Vector3 b)
{
    Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
    return c;    
}

float Vector3::dot(Vector3 b)
{
    return x * b.x + y * b.y + z * b.z;  
}


// Norm ***********************************************************************
class Norm : public Point3 
{
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

// VertexID *******************************************************************
class VertexID 
{
    public:
         int vertIndex; //Index of this vertex in the vertex list
         int normIndex; //Index of this vertex´s normal
};

// Face ***********************************************************************
class Face 
{
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

// Mesh ***********************************************************************
class Mesh 
{                  
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

void Mesh::drawMesh() 
{
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

// Camera *********************************************************************
class Camera 
{
    private:
         Point3 eye;
         Vector3 u, v, n;
         double viewAngle, aspect, nearDist, farDist; //view volume shape
         void setModelViewMatrix(); //tell OpenGl where the camera is   
    
    public:
         Camera(void) {}; //default constructor  
         void set(Point3 eye, Point3 look, Vector3 up); //like gluLookAt()
         void roll(float angle); //roll it
         void pitch(float angle); //increase pitch
         void yaw(float angle); //yaw it
         void slide(float delU, float delV, float delN); //slide it
         void rotate(Vector3 axis, float angle);
         void setShape(float vAng, float asp, float nearD, float farD);
};

void Camera::setShape(float vAngle, float asp, float nr, float fr)
{
    viewAngle = vAngle;
    aspect = asp;
    nearDist = nr;
    farDist = fr;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, aspect, nearDist, farDist);
    glMatrixMode(GL_MODELVIEW); 
}

void Camera::setModelViewMatrix(void)
{
    //load modelview matrix with existing camera values
    float m[16];
    Vector3 eVec(eye.x, eye.y, eye.z); //a vector version of eye
    
    m[0]=u.x; m[4]=u.y; m[8]=u.z;   m[12]=-eVec.dot(u);
	m[1]=v.x; m[5]=v.y; m[9]=v.z;   m[13]=-eVec.dot(v);
	m[2]=n.x; m[6]=n.y; m[10]=n.z;  m[14]=-eVec.dot(n);
	m[3]=0;   m[7]=0;   m[11]=0;    m[15]=1.0; 
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m); //load OpenGL's modelview matrix
}

void Camera::set(Point3 Eye, Point3 look, Vector3 up)
{
    //create a modelview matrix and send it to OpenGL
    eye.set(Eye);
    n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z); //make n 
    u.set(up.cross(n).x, up.cross(n).y, up.cross(n).z); //make u = up X n
    n.normalize();
    u.normalize(); //make them unit length
    v.set(n.cross(u).x, n.cross(u).y, n.cross(u).z); //make v = n X u
    setModelViewMatrix(); //tell OpenGL
}

void Camera::slide(float delU, float delV, float delN)
{
    eye.x += delU * u.x + delV * v.x + delN * n.x;
    eye.y += delU * u.y + delV * v.y + delN * n.y;
    eye.z += delU * u.z + delV * v.z + delN * n.z;
    setModelViewMatrix();
}

void Camera::roll(float angle)
{
    //u´=cos(\alpha)u+sin(\alpha)v, v´=-sin(\alpha)u+cos(\alpha)v
    float cs=cos(M_PI/180.0 * angle);
    float sn=sin(M_PI/180.0 * angle);
    Vector3 t = u;
    u.set(cs*t.x - sn*v.x, cs*t.y - sn*v.y, cs*t.z - sn*v.z);
    v.set(sn*t.x + cs*v.x, sn*t.y + cs*v.y, sn*t.z + cs*v.z);
    setModelViewMatrix();
}

void Camera::pitch (float angle)
{ 
    // pitch the camera through angle degrees around U
    float cs = cos(3.14159265/180 * angle);
    float sn = sin(3.14159265/180 * angle);
    Vector3 t(v); // remember old v
    v.set(cs*t.x - sn*n.x, cs*t.y - sn*n.y, cs*t.z - sn*n.z);
	n.set(sn*t.x + cs*n.x, sn*t.y + cs*n.y, sn*t.z + cs*n.z);
	setModelViewMatrix();
}

void Camera::yaw (float angle)
{ 
    // yaw the camera through angle degrees around V
    float cs = cos(3.14159265/180 * angle);
    float sn = sin(3.14159265/180 * angle);
    Vector3 t(n); // remember old v
    n.set(cs*t.x - sn*u.x, cs*t.y - sn*u.y, cs*t.z - sn*u.z);
	u.set(sn*t.x + cs*u.x, sn*t.y + cs*u.y, sn*t.z + cs*u.z);
	setModelViewMatrix();
}


Mesh mesh;
Camera cam;

double centroX, centroY, centroZ;
double x, y, z;

double _left = -100.0;
double _right = 100.0;
double _bottom = -100.0;
double _top = 100.0;


void myInit(void)
{
    glClearColor(0.0,0.0,0.0,0.0);        // set white background color
    glColor3f(1.0f, 1.0f, 1.0f);          // set the drawing color
    glPointSize(1.0);                 // a 'dot' is 4 by 4 pixels
    glLineWidth(0.5);      
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0, 100.0, -200, 200);
    gluLookAt(0,0,0,320,240,50,0,0,1);
    
    x = y = z = 5;

    Point3 ey(x,y,z);
    Point3 lo(0,0,0);
    Vector3 up(0,1,0);
    cam.set(ey,lo,up);
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
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

void GerenciaMouse(int button, int state, int x, int y)
{	
	if (button == GLUT_LEFT_BUTTON)
		 if (state == GLUT_DOWN) {  // Zoom-in
              // 1 quadrante			
			  if (x <= 640/2 && y <= 480/2)
                   glTranslatef (-10, 0, 0);
                   
              // 2 quadrante             
              else if (x > 640/2 && y <= 480/2)             
                   glTranslatef (10, 0, 0);
                 
              // 3 quadrante
              else if (x <= 640/2 && y > 480/2)
                   glTranslatef (-10, 0, 0);
                 
              // 4 quadrante
              else
                   glTranslatef (10, 0, 0);
             
             myDisplay();			
		 }

	if (button == GLUT_RIGHT_BUTTON)
		 if (state == GLUT_DOWN) {  // Zoom-out
              // 1 quadrante			
			  if (x <= 640/2 && y <= 480/2)
                   glTranslatef (0, 10, 0);
                   
              // 2 quadrante
              else if (x > 640/2 && y <= 480/2)
                   glTranslatef (0, 10, 0);
                   
              // 3 quadrante
              else if (x <= 640/2 && y > 480/2) 
                   glTranslatef (0, -10, 0);
                   
              // 4 quadrante
              else             
                   glTranslatef (0, -10, 0);
                    
              myDisplay();			
		 }
}

void GerenciaTeclado(unsigned char key, int mouseX, int mouseY)
{
    switch(key)
    {
         case 'f':
              cam.yaw(-10);
              myDisplay();
              break;
                 
         case 'F':
              cam.yaw(10);
              myDisplay();
              break;

         case 's':
              cam.pitch(10);
              myDisplay();
              break;

         case 'S':
              cam.pitch(-10);
              myDisplay();
              break;

         case 'D':
              cam.roll(10);
              myDisplay();
              break;

         case 'd':
              cam.roll(-10);
              myDisplay();
              break;

         case 'a':
              glMatrixMode(GL_PROJECTION);
              glLoadIdentity();
              glOrtho(_left -= 5, _right += 5, _bottom -= 5, _top += 5, -200, 200);
              myDisplay();
              break;

         case 'A':
              glMatrixMode(GL_PROJECTION);
              glLoadIdentity();
              glOrtho(_left += 5, _right -= 5, _bottom += 5, _top -= 5, -200, 200);
              myDisplay();
              break;

         case 'q':
              exit(0);
              break;
         case 'Q':
              exit(0);
              break;         
    }     
}

int main(int argc, char** argv)
{     
    string objFile;
	
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
		mesh.readFile(objFiles[--arquivoDes]);
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
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Visualizacao 3D");
	glutDisplayFunc(myDisplay);
	
	glutMouseFunc(GerenciaMouse);
	glutKeyboardFunc(GerenciaTeclado);
	
	myInit();
	glutMainLoop();
	
	return EXIT_SUCCESS;
}
