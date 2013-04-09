#include <math.h>
// Support Classes for Canvas
class Point2   //single point w/ floating point coordinates
{
public:
  Point2();     //contructor 1
  Point2(float xx, float yy);   //constructor 2
  void set(float xx, float yy);
  float getX();
  float getY();
  void draw(void);
private:
  float x, y;
};

//constructor 1
Point2::Point2()
{
  x = y = 0.0f;
}

//constructor 2
Point2::Point2(float xx, float yy)
{
  x=xx; y=yy;
}

void Point2::set(float xx, float yy)
{
  x=xx; y=yy;
}

float Point2::getX()
{
  return x;
}

float Point2::getY()
{
  return y;
}

void Point2::draw(void)
{
  glBegin(GL_POINTS); //draw this point
    glVertex2f((GLfloat)x, (GLfloat)y);
  glEnd();
}



//para descrever uma viewport utilizaremos esta classe
class IntRect
{
   public:
      IntRect(); //constructors
      IntRect(int left, int right, int bottom, int top);
      void set(int left, int right, int bottom, int top);
      float getL(void);     //left boundary
      float getR(void);     //right
      float getT(void);
      float getB(void);
      void draw(void); //desenha um retângulo usando OpenGL
  private:
      float l, r, b, t;
};

IntRect::IntRect()
{
  l = 0; r=100; b=0; t=100;
}

IntRect::IntRect(int left, int right, int bottom, int top)
{
  l = left; r=right; b=bottom; t=top;
}

void IntRect::set(int left, int right, int bottom, int top)
{
  l=left; r=right; b=bottom; t=top;
}

float IntRect::getL(void)      //left boundary
{
  return l;
}

float IntRect::getR(void)      //right
{
  return r;
}

float IntRect::getT(void)
{
  return t;
}

float IntRect::getB(void)
{
  return b;
}

void IntRect::draw(void)
{
  glRectf( l, b, r, t);
}










//world window rectangle class
class RealRect
{
public:
  RealRect(); //constructors
  RealRect(float left, float right, float bottom, float top);
  void set(float left, float right, float bottom, float top);
  float getL(void);     //left boundary
  float getR(void);     //right
  float getT(void);
  float getB(void);
  void draw(void); 	//draw this rectangle using OpenGL
private:
  float l, r, b, t;
};

//constructors
RealRect::RealRect()
{
  l = 0; r=100; b=0; t=100;
}

RealRect::RealRect(float left, float right, float bottom, float top)
{
  l = left; r=right; b=bottom; t=top;
}

void RealRect::set(float left, float right, float bottom, float top)
{
  l=left; r=right; b=bottom; t=top;
}

float RealRect::getL(void)      //left boundary
{
  return l;
}

float RealRect::getR(void)      //right
{
  return r;
}

float RealRect::getT(void)
{
  return t;
}

float RealRect::getB(void)
{
  return b;
}

void RealRect::draw(void)
{
  glRectf( l, b, r, t);
}







class Canvas
{
public:
  Canvas(int width, int height, char* windowTitle); //constructor
  void setWindow(float l, float r, float b, float t);
  void setViewport(int l, int r, int b, int t);
  IntRect getViewport(void); //divulge the viewport data
  RealRect getWindow(void); // divulge the window data
  float getWindowAspectRatio(void);
  void clearScreen();
  void setBackgroundColor(float r, float g, float b);
  void setColor(float r, float g, float b);
  void lineTo(float x, float y);
  void lineTo(Point2 p);
  void moveTo(float x, float y);
  void moveTo(Point2 p);
  void moveRel(float dx, float dy);


private:
  Point2 CP;         //current position in the world
  IntRect viewport;  //the current window
  RealRect window;   //the current viewport
  float CD;
} ;

//constructor
Canvas:: Canvas(int width, int height, char* windowTitle)
{
  char* argv[1];  //dummy argument list for glutinit()
  char dummyString[8];
  argv[0] = dummyString;  //hook up the pointer
  int argc = 1;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutInitWindowPosition (20,20);
  glutCreateWindow (windowTitle);
  setWindow(0, (float)width, 0, (float)height); // default world window
  setViewport (0, width, 0, height); //default viewport
  CP.set(0.0f, 0.0f); //initialize the cp to (0,0)
}

void Canvas::moveTo(float x, float y)
{
   CP.set(x, y);
}


void Canvas:: moveTo(Point2 p)  //moves current point CP to point p object
{
  float x1, y1;
  x1 = p.getX();
  y1 = p.getY();
  CP.set(x1, y1);
}

void Canvas::lineTo(float x, float y)
{
  glBegin (GL_LINES);
    glVertex2f((GLfloat) CP.getX(), (GLfloat) CP.getY());
    glVertex2f((GLfloat) x, (GLfloat) y);
  glEnd();
  CP.set(x, y);
  glFlush();
}

void Canvas:: lineTo(Point2 p)
{
  glBegin (GL_LINES);
    glVertex2f((GLfloat) CP.getX(), (GLfloat) CP.getY());
    glVertex2f((GLfloat) p.getX(), (GLfloat) p.getY());
  glEnd();
  CP.set(p.getX(), p.getY());
  glFlush();
}
void Canvas::setWindow(float l,float r,float b,float t)
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(l,r,b,t);
}
// subjanela de visualização
// considera a janela definida no comando glutInitWindowSize(640,480);
//                                    *** tamanho da Janela ***
void Canvas::setViewport(int l,int r,int b,int t)
{
glViewport(l,b,r-l,t-b);
}
void Canvas::setBackgroundColor(float r, float g, float b)
{
  glClearColor(r,g,b,0.0);
}

void Canvas::setColor(float r, float g, float b)
{
  glColor3f(r, g, b);
}
