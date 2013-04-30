#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>

#include "glut.h"
#include "mesh.h"

#define PI 3.14159265f

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
    float cs = cos(PI/180 * angle);
    float sn = sin(PI/180 * angle);
    Vector3 t(v); // remember old v
    v.set(cs*t.x - sn*n.x, cs*t.y - sn*n.y, cs*t.z - sn*n.z);
    n.set(sn*t.x + cs*n.x, sn*t.y + cs*n.y, sn*t.z + cs*n.z);
    setModelViewMatrix();
}

void Camera::yaw (float angle)
{
    // yaw the camera through angle degrees around V
    float cs = cos(PI/180 * angle);
    float sn = sin(PI/180 * angle);
    Vector3 t(n); // remember old v
    n.set(cs*t.x - sn*u.x, cs*t.y - sn*u.y, cs*t.z - sn*u.z);
    u.set(sn*t.x + cs*u.x, sn*t.y + cs*u.y, sn*t.z + cs*u.z);
    setModelViewMatrix();
}

#endif
