
#include <GL/glut.h>    /* OpenGL Utility Toolkit header */
#include <iostream>
#include <math.h>
#include <iomanip>
#include <vector>
#include <algorithm>
//---------------------------------------------------------
//
// This is a sample program to plot to functions using
// open gl
// to compile 
// 
//  g++ gl_plot.cpp -ogl_plot -lglut -lGL -lGLU
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
double xsq( double x )
{ return x*x; }

double mxsq( double x )
{ return -x*x; }

double xsq_10( double x )
{ return ( 10 - x ) * ( 10 - x ); }

double x_10( double x )
{ return x*10; }

double sxq( double x )
{
  return x < 0 ? -x*x : x*x ;
}

double xcube( double x )
{ return x*x*x; }

double x4( double x )
{ return x*x*x*x; }


void fgen( double lb , double ub , double stp ,
	   double (*F)(double) , 
	   std::vector<double> &x , std::vector<double> &y )
{
  x.clear();
  y.clear();

  for( ; lb <= ub ; lb += stp )
    {
      x.push_back(lb);
      y.push_back(F(lb));
    }

}

void scale_eq( double min_e , double max_e , double &m , double &b )
{
  double x1 = min_e , y1 = -1;
  double x2 = max_e , y2 = 1;

  m = ( y2-y1) / ( x2 - x1 );
  b = y1 - m*x1;

  std::cout << m << " " << b << " " << x1  << " " << x2 << std::endl;
}


void plot( std::vector<double> &x , std::vector<double> &y , 
	   double min_x, double max_x , double min_y , double max_y ,
	   void (*color_func)()
	   )
{


  double mx,bx,my,by;
  scale_eq(min_x,max_x,mx,bx);
  scale_eq(min_y,max_y,my,by);

  std::cout << "plot = " << mx << " " << bx << " " << min_x << " " << max_x << std::endl;
  std::cout << "plot = " << my << " " << by << " " << min_y << " " << max_y << std::endl;

  //glPointSize(3);
  //glBegin(GL_POINTS);
  glBegin(GL_LINE_STRIP);
  color_func();
  for( size_t i = 0 ; i < x.size() ; i++ )
    {
      double xc = mx*x[i]+bx;
      double yc = my*y[i]+by;
      std::cout << xc << " " << yc << std::endl;
      glVertex2f(xc,yc);
    }
  glEnd();

}

double get_min( std::vector<double> &x )
{
  return (*std::min_element( x.begin() , x.end() ));
}

double get_max( std::vector<double> &x )
{
  return (*std::max_element( x.begin() , x.end() ));
}

void get_scale( std::vector<double> &x1 , std::vector<double> &y1 ,
		std::vector<double> &x2 , std::vector<double> &y2 ,
		double &min_x, double &max_x , double &min_y , double &max_y )
{
  min_x = std::min( get_min(x1) , get_min(x2));
  max_x = std::max( get_max(x1) , get_max(x2));

  min_y = std::min( get_min(y1) , get_min(y2));
  max_y = std::max( get_max(y1) , get_max(y2));
}

void draw_axis( double x1 , double y1 ,double x2, double y2 , bool xaxis )
{
  double mx,bx,my,by;
  scale_eq(x1,x2,mx,bx);
  scale_eq(y1,y2,my,by);

  double px1,px2,py1,py2;
  if( xaxis )
    {
      px1 = mx * x1 + bx;
      px2 = mx * x2 + bx;
      py1 = my * 0 + by;
      py2 = my * 0 + by;
    }
  else
    {
      px1 = mx * 0 + bx;
      px2 = mx * 0 + bx;
      py1 = my * y1 + by;
      py2 = my * y2 + by;

    }

  glBegin(GL_LINES);
  c_black();
  std::cout << px1 << " " << py1 << std::endl;
  glVertex2f(px1,py1);
  std::cout << px2 << " " << py2 << std::endl;
  glVertex2f(px2,py2);
  glEnd();
}


void plot( std::vector<double> &x1 , std::vector<double> &y1 ,
	   std::vector<double> &x2 , std::vector<double> &y2 )
{
  double min_x, max_x , min_y , max_y ;
  get_scale( x1 , y1 , x2 , y2 , min_x , max_x , min_y , max_y );
  plot(x1,y1,min_x,max_x,min_y,max_y,c_red);
  plot(x2,y2,min_x,max_x,min_y,max_y,c_blue);
  draw_axis(min_x,min_y,max_x,max_y,true);
  draw_axis(min_x,min_y,max_x,max_y,false);
  //draw_axis(0,0,min_y,max_y);
}

//---------------------------------------------------------
// 
//---------------------------------------------------------
void init_vec( std::vector<double> & x1 , std::vector<double> & y1 ,
	       std::vector<double> & x2 , std::vector<double> & y2 
	       )
{
  static const double pi = M_PI;
  if ( !x1.empty()) return;
  //fgen( -5, 2.25 , 0.25 , exp , x , y );
  //fgen( -4*pi, 4*pi , pi/12 , sin , x1 , y1 );
  fgen( -4*pi, 4*pi , pi/12 , cos , x1 , y1 );
  fgen( -pi/2 + pi/24 , pi/2 - pi/24 , pi/24 , tan , x2 , y2 );
  //fgen( -5,5, 0.5 , xsq, x2, y2 );
  //fgen( -5,5, 0.5 , xcube, x1, y1 );
  //fgen( -5,5, 0.5 , x4, x1, y1 );
  //fgen( .1,10, 0.5 , log, x1, y1 );
  //fgen( .1,10, 0.5 , xsq, x2, y2 );
  //fgen( -20, 20, 0.5 , sxq, x2, y2 );
  //fgen( -20, 20, 0.5 , x_10, x1, y1 );
}


void my_display_function()
{
  static std::vector<double> x1 , y1 , x2 , y2 ;
  init_vec(x1,y1,x2,y2);
  std::cout << "callback - my_display_function" << std::endl;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //draw_axis();
  plot(x1,y1,x2,y2);
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
