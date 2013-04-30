#ifndef MESH_H
#define MESH_H

#include <dirent.h>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

#include "glut.h"

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
        glBegin(GL_POLYGON);
        for(int v = 0; v < 3 ; v++)
        {
            int iv = this->mFaces[f].mVert[v].vertIndex;
            
            glVertex3f(this->mVertices[iv].getX(), this->mVertices[iv].getY(), this->mVertices[iv].getZ());
        }
        glEnd();
    }
    glFlush();
    glutSwapBuffers();
}

#endif
