

//-----------------------------------------------------------------------
//
//  Date  : 11/26/2011
// 
//  This program shows the basics of reading a .png file using the png
//  library, extracting the image and setting into a 2D Texture 
//  using openGL.
//
//  to get libpng you will need to do the following on ubunutu linux
// 
//  sudo apt-get install zlib1g-dev
//  wget http://sourceforge.net/projects/libpng/files/libpng15/1.5.6/libpng-1.5.6.tar.gz/download
//  tar -xvzf libpng-1.5.6.tar.gz 
//  ./configure 
//  make 
//  sudo make install
//
// copies to /usr/local/lib
//           /usr/local/install
//
//
//-----------------------------------------------------------------------


#include <GL/glut.h>    /* OpenGL Utility Toolkit header */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>
#include <png.h>
#include <pngconf.h>
#include <pnglibconf.h>
#include <string>
#include <sstream>


//-----------------------------------------------------------------------
// Encapsulate the reading of the image into a class for easeier
// resource cleanup
//-----------------------------------------------------------------------
class PNGLoader
{
public:

  PNGLoader() { init(); }
  ~PNGLoader() { clean_up();}
  

  bool set( const char *filename , bool debug=false );

  int get_bit_depth()  const { return bit_depth; }
  int get_color_type() const { return color_type; }
  int get_width()      const { return width; }
  int get_height()     const { return height; }

  
  std::string get_error() const { return errormsg; }

private:
  FILE        *fp;
  png_byte    header[8];
  png_structp png_ptr;
  png_infop   info_ptr;
  png_infop   end_info;
  png_byte    *image_data;
  png_bytep   *row_pointers;
  
  int         bit_depth;
  int         color_type;
  png_uint_32 twidth; 
  png_uint_32 theight;
  int         width;
  int         height;
  int         rowbytes;

  std::string errormsg;

private:
  void init()
  {
    init_pointers();
    init_parameters();
  }

  void init_parameters()
  {
    bit_depth=0;
    color_type=0;
    twidth=0;
    theight=0;
    width=0;
    height=0;
  }

  void init_pointers()
  {
    fp=0;
    png_ptr=NULL;
    info_ptr=NULL;
    end_info=NULL;
    image_data=0;
    row_pointers=0;
  }

  void clean_up()
  {
    if ( fp) fclose(fp);
    if ( png_ptr && info_ptr && end_info )
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    else if ( png_ptr && info_ptr )
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
    else if ( png_ptr )
      png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
    if ( image_data ) delete [] image_data;
    if ( row_pointers ) delete [] row_pointers;
    init_pointers();
  }

private:

  bool read_image( const char * , bool );
  void set_image();
};

bool PNGLoader::set( const char *filename , bool debug )
{
  if ( !read_image( filename , debug )) return false;
  set_image();
  return true;
}

bool PNGLoader::read_image( const char *filename , bool debug )
{
  clean_up();
  fp = fopen(filename, "rb");
  if (!fp) 
    {
      std::stringstream strm;
      strm << "PNGLoader::set : error opening file " << filename;
      errormsg=strm.str();
      return false;
    }

   //read the header & test check if its a png file
  fread(header, 1, 8, fp);
  int is_png = !png_sig_cmp(header, 0, 8);
  if (!is_png) 
    {
      std::stringstream strm;
      strm << "PNGLoader::set : is not in png format " << filename;
      errormsg=strm.str();
      return false;
    }
  //create png struct
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
				   NULL,
				   NULL, 
				   NULL);
  if (!png_ptr) 
    {
      std::stringstream strm;
      strm << "PNGLoader::set : unable to allocate png_ptr ";
      errormsg=strm.str();
      return false;
    }

  //create png info struct

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) 
    {
      std::stringstream strm;
      strm << "PNGLoader::set : unable to allocate png_info_ptr ";
      errormsg=strm.str();
      return false;
    }

  //create png info struct
  end_info = png_create_info_struct(png_ptr);
  if (!end_info) 
    {
      std::stringstream strm;
      strm << "PNGLoader::set : unable to allocate png_info_ptr end ";
      errormsg=strm.str();
      return false;
    }

  //init png reading
  png_init_io(png_ptr, fp);
 
  //let libpng know you already read the first 8 bytes
  png_set_sig_bytes(png_ptr, 8);
 
  // read all the info up to the image data
  png_read_info(png_ptr, info_ptr);


  // get info about png
  png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
	       NULL, NULL, NULL);

  width = twidth;
  height = theight;
 
  if ( debug )
    std::cout << "png_header info: width= " << twidth 
	      << " height = " << theight
	      << " bit_depth = " << bit_depth 
	      << " color_type = " << color_type
	      << std::endl;


  // Update the png info struct.
  png_read_update_info(png_ptr, info_ptr);
 
  // Row size in bytes.
  rowbytes = png_get_rowbytes(png_ptr, info_ptr);
 
  // Allocate the image_data as a big block, to be given to opengl
  image_data = new png_byte[rowbytes * height];
  //GLubyte *image_data;
  //image_data = (unsigned char *)malloc(rowbytes * height );

  if (!image_data) 
    {
      std::stringstream strm;
      strm << "PNGLoader::set : error loading texture image";
      errormsg=strm.str();
      return false;
    }
 
  //row_pointers is for pointing to image_data for reading the png with libpng
  row_pointers = new png_bytep[height];
  if (!row_pointers) 
    {
      //clean up memory and close stuff
      std::stringstream strm;
      strm << "PNGLoader::set : error allocating row_pointers";
      errormsg=strm.str();
      return false;
    }

  // set the individual row_pointers to point at the correct offsets of image_data
  for (int i = 0; i < height; ++i)
    row_pointers[height - 1 - i] = image_data + i * rowbytes;
 
  //read the png into image_data through row_pointers
  png_read_image(png_ptr, row_pointers);
  return true;
}


void PNGLoader::set_image()
{
  GLuint texture;
  glEnable( GL_TEXTURE_2D );
  glEnable( GL_NORMALIZE );
  glGenTextures( 1 , &texture );
  glBindTexture( GL_TEXTURE_2D, texture );

  // select modulate to mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  // when texture area is small, bilinear filter the closest mipmap
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		   
  // when texture area is large, bilinear filter the original
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // the texture wraps over at the edges (repeat)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  bool hasAlpha = color_type == PNG_COLOR_TYPE_RGBA;
  gluBuild2DMipmaps( GL_TEXTURE_2D, hasAlpha ? 4 : 3 , width , height , 
		     hasAlpha ? GL_RGBA : GL_RGB , GL_UNSIGNED_BYTE , image_data );
}

//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBegin( GL_QUADS );
  glTexCoord2d(0.0,0.0); glVertex3f(0.0,0.0,0);
  glTexCoord2d(1.0,0.0); glVertex3f(1.0,0.0,0);
  glTexCoord2d(1.0,1.0); glVertex3f(1.0,1.0,0);
  glTexCoord2d(0.0,1.0); glVertex3f(0.0,1.0,0);
  glEnd();
  glutSwapBuffers();
}

void load_image( const char *filename )
{
  PNGLoader png_loader;
  if ( !png_loader.set( filename ) )
    std::cerr << png_loader.get_error() << std::endl;
}

int main( int argc , char **argv )
{
  if ( argc < 2 ) 
    {
      std::cerr << "usage " << argv[0] << "<image_file>" << std::endl;
      return -1;
    }
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( 400, 400 );
  glutCreateWindow( "window title" );
  glClearColor(1.0,1.0,1.0,1.0);
  glutDisplayFunc(display);
  load_image( argv[1] );
  glutMainLoop();
  return 0;
}
