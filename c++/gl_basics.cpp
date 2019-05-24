
#include <GL/glut.h>    /* OpenGL Utility Toolkit header */
#include <iostream>
#include <math.h>
#include <iomanip>

//---------------------------------------------------------
//
// This is a sample program to draw basic shapes in 2D
// using open gl
// to compile 
// 
//  g++ gl_basics.cpp -ogl_basics -lglut -lGL -lGLU
//
//---------------------------------------------------------


//---------------------------------------------------------
// color functions methods
//---------------------------------------------------------
inline void c_red() { glColor3f(1.0,0.0,0.0); }
inline void c_green() { glColor3f(0.0,1.0,0.0); }
inline void c_blue() { glColor3f(0.0,0.0,1.0); }
inline void c_yellow() { glColor3f(1.0,1.0,0.0); }
inline void c_grey() { glColor3f(0.5,0.5,0.5); }
inline void c_dark_blue() { glColor3f(0.0,0.0,.3); }
inline void c_white() { glColor3f(1.0,1.0,1.0); }
inline void c_black() { glColor3f(0.0,0.0,0.0); }

//---------------------------------------------------------
// point drawing method
//---------------------------------------------------------
void draw_point_circle( double r , double cx , double cy , void (*color_func)() )
{
  static const double pi = M_PI;
  static const double one_deg = pi / 180.0;
  static const double _2pi = 2.0*pi;
  glPointSize(1);
  glBegin(GL_POINTS);
  double stp = one_deg * 3;
  bool debug = false;
  for( double rad = 0 ; rad < _2pi ; rad += stp )
    {
      double x = r*sin(rad) + cx , y = r*cos(rad) + cy;
      color_func();
      glVertex2f(x,y);
      if ( debug )
	std::cout << std::setprecision(6) << std::fixed << x << " " 
		  << std::setprecision(6) << std::fixed << y << " " 
		  << std::endl;
    }
  glEnd();
}

void draw_point_center( void (*color_f) () )
{
  glPointSize(20);
  glBegin(GL_POINTS);
  color_f();
  glVertex2f(0.0,0.0);
  glEnd();
}

void draw_point_square( void (*color_f) () , double scale)
{
  glPointSize(5);
  glBegin(GL_POINTS);
  for( int i = -1 ; i <= 1 ; i += 2)
    for( int j = -1 ; j <= 1 ; j += 2 )
      {
	color_f();
	glVertex2f((GLfloat)i*scale,(GLfloat)j*scale);
      }
  glEnd();

}

void point_draw_demo()
{
  draw_point_square(c_black,1.0);
  draw_point_square(c_green,0.75);
  draw_point_square(c_green,0.5);
  draw_point_square(c_green,0.25);

  draw_point_center(c_grey);

  draw_point_circle(.25,.5,.5,c_red);
  draw_point_circle(.25,-.5,-.5,c_green);
  draw_point_circle(.25,.5,-.5,c_blue);
  draw_point_circle(.25,-.5,.5,c_black);


}

//----------------------------------------------------------
// Trialgle Draw
//----------------------------------------------------------

void draw_triangle()
{
  /*
  glBegin(GL_TRIANGLE_FAN);
  c_red();
  glVertex3f(0, 0, 0);
  glVertex3f(2, 1, 1);
  glVertex3f(2, -1, 1);
  glVertex3f(2, -1, -1);
  glVertex3f(2, 1, -1);
  glVertex3f(2, 1, 1);
  glEnd();
  */

  glBegin(GL_TRIANGLES);
  c_red();
  glVertex2f(0.0,0.0);
  glVertex2f(.5,0.0);
  glVertex2f(.5,.5);
  glEnd();


  glBegin(GL_TRIANGLES);
  c_blue();
  glVertex2f(0.0,0.0);
  glVertex2f(0.,.5);
  glVertex2f(.5,.5);
  glEnd();

}

void draw_lines()
{
  glBegin(GL_LINES);
  c_black();
  glVertex2f(-1.0,-1.0);
  glVertex2f(1.0,1.0);
  glEnd();

  glBegin(GL_LINES);
  c_black();
  glVertex2f(-1.0,1.0);
  glVertex2f(1.0,-1.0);
  glEnd();

  glBegin(GL_LINES);
  c_black();
  glVertex2f(0.0,-1.0);
  glVertex2f(0.0,1.0);
  glEnd();

  glBegin(GL_LINES);
  c_black();
  glVertex2f(-1.0,0.0);
  glVertex2f(1.0,0.0);
  glEnd();


}


void draw_line_strips()
{
  glBegin(GL_LINE_STRIP);
  c_black();
  glVertex2f(-.5,0.0);
  glVertex2f(-.5,.5);
  glVertex2f(.5,.5);
  glEnd();

}

void draw_line_loops()
{
  glBegin(GL_LINE_LOOP);
  c_black();
  glVertex2f(-.5,0.0);
  glVertex2f(-.5,.5);
  glVertex2f(.5,.5);
  glEnd();

}

void draw_quads()
{
  glBegin(GL_QUADS);
  c_black();
  glVertex2f(-.5,-.5);
  glVertex2f(-.5,.5);
  glVertex2f(.5,.5);
  glVertex2f(.5,-.5);
  glEnd();

}


//---------------------------------------------------------
// 
//---------------------------------------------------------


void my_display_function()
{
  std::cout << "callback - my_display_function" << std::endl;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  point_draw_demo();
  draw_triangle();
  draw_lines();
  draw_line_strips();
  draw_line_loops();
  //draw_quads();
  glutSwapBuffers();
}


void my_reshape_function( int width , int height )
{
  std::cout << "callback - my_reshape_function " << width << " " << height << std::endl;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//---------------------------------------------------------
// GL initializaiton
//---------------------------------------------------------
void init( int argc , char **argv)
{
  // (1) Initialize GLUT Library!
  glutInit(&argc,argv);

  // (2) Set the Display Mode
  //
  // Display Mode RGBA or INDEX
  // GLUT_RGBA color buffer reg,green,blue,alpah - alpha a measure of opaqueness 1.0 opaque , 0.0 transparent
  // 4th color used for shading
  // GLUT_INDEX - 
  //
  // DISPLAY Mode - Double Buffering or Single Buffering.
  // GLUT_SINGLE - for single buffer
  // GLUT_DOUBLE - double buffer
  //
  // GLUT_DEPTH  - stores a depth pixel, will display objects of a smaller depth value in front of objects with a larger
  // GLUT_STENCIL - restrict drawing to a certian portion of the screen - just like a stencil
  // GLUT_ACCUM - accumulate a series of images into a final composed image
  // 
  // GLUT_MULTISMAPLE

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  //glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH );

  // initialize the window size
  glutInitWindowSize( 400, 400 );
  
  // create window
  glutCreateWindow( "window title" );

  // clear color
  glClearColor(1.0,1.0,1.0,1.0);

  // specify a callback function in response to evenst
  glutDisplayFunc(my_display_function);

  // reshpe callback funciton
  //glutReshapeFunc(my_reshape_function);
}


int main( int argc , char **argv)
{
  init( argc, argv );
  glutMainLoop();
  return 0;
}
