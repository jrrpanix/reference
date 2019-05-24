
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>

// 
// This example shows how to write and read data in binary format to files using 
// C++ streams
//
// It have the following examples :

// (1-A) write an int a float and a dobule in binary format
// (1-B) read an int a float and a dobule in binary format
// (1-C) get the size of a file in bytes using stream operations 

// (2-A)  create a struct and write its contents separately
// (2-A1) create a struct and write its contents in one operaation -
// (3-A2) create the same struct with better word alignment 

//-----------------------------------------------------------------------
// (1-A) write an int a float and a double to a file in binary form
//-----------------------------------------------------------------------
void simple_write( const std::string &file , int x , float f , double d )
{
  
  std::ofstream out;

  // open a file for writing in binary mode, do not append
  // if we wanted to append we would add std::ios::app 
  out.open( file.c_str() ,  std::ios::out | std::ios::binary );

  if ( !out ) 
    {
      std::cerr << "unable to create file " << file << std::endl;
      return ;
    }
  out.write( reinterpret_cast<const char*>(&x) , sizeof(int)) ;
  out.write( reinterpret_cast<const char*>(&f) , sizeof(float)) ;
  out.write( reinterpret_cast<const char*>(&d) , sizeof(double)) ;
  out.close();
}

//-----------------------------------------------------------------------
// (1-B) read an int a float and a double to a file in binary form
//-----------------------------------------------------------------------
void simple_read(  const std::string &file , int &x , float &f , double &d )
{
  std::ifstream in;

  // open a file for writing in binary mode, do not append
  // if we wanted to append we would add std::ios::app 
  in.open( file.c_str() ,  std::ios::in | std::ios::binary );
  
  if ( !in )
    {
      std::cout << "unable to open file " << file << std::endl;
    }

  in.read( (char *)&x, sizeof(int));
  in.read( (char *)&f, sizeof(float));
  in.read( (char *)&d, sizeof(double));
  in.close();
}

//-----------------------------------------------------------------------
// (1-C) get the size of a file using stream operations
//-----------------------------------------------------------------------
long get_file_size( const std::string &file )
{
  std::ifstream s( file.c_str() );
  if ( !s ) return 0;
  long begin = s.tellg();
  s.seekg(0, std::ios::end );
  long end = s.tellg();
  return end - begin;
}

//-----------------------------------------------------------------------
// A Structure having various data types, with word alignment that will cuase
// its size to be 8 bytes larger on platformst with 8 byte word sizes
//
// Note sizes are for X_64 Linux
//
// Mixed_Unoptimal - size is 32 bytes because and extra 4 bytes was used for padding
// Mixed_Align     - size is 24 bytes because the float and int are next to each other
//                   so no padding is needed  
//-----------------------------------------------------------------------

struct Mixed_Unaligned
{
  // the member ordering has a 4 byte element, int i1 followecd by
  // an 8 byte element long l1 so to align everything 4 bytes of
  // padding (wasted sapce) are appended to the i1. 


  int    i1; // 4 bytes , 4 bytes padding    , total size = 8
  long   l1; // 8 bytes , 0 bytes padding    , total size = 8
  float  f1; // 4 bytes , 4 bytes of padding , total size = 8
  double d1; // 8 bytes , 0 bytes of padding , total size = 8
};

struct Mixed_Aligned
{
  Mixed_Aligned( int i , float f , long l , double d ) 
    : i1(i),f1(f),l1(l),d1(d) {}

  Mixed_Aligned()
    : i1(0),f1(0),l1(0),d1(0) {}

  int    i1; // 4 bytes , 0 bytes padding    , total size = 4
  float  f1; // 4 bytes , 0 bytes of padding , total size = 4
  long   l1; // 8 bytes , 0 bytes padding    , total size = 8
  double d1; // 8 bytes , 0 bytes of padding , total size = 8
};

void write_mixed_aligned( const std::string &file , const Mixed_Aligned &m )
{
  std::ofstream out;

  // open a file for writing in binary mode, do not append
  // if we wanted to append we would add std::ios::app 
  out.open( file.c_str() ,  std::ios::out | std::ios::binary );

  if ( !out ) 
    {
      std::cerr << "unable to create file " << file << std::endl;
      return ;
    }
  out.write( reinterpret_cast<const char*>(&m) , sizeof(Mixed_Aligned)) ;
  out.close();
}

void read_mixed_aligned( const std::string &file , Mixed_Aligned &m )
{
  std::ifstream in;

  // open a file for writing in binary mode, do not append
  // if we wanted to append we would add std::ios::app 
  in.open( file.c_str() ,  std::ios::in | std::ios::binary );
  
  if ( !in )
    {
      std::cout << "unable to open file " << file << std::endl;
    }

  in.read( (char *)&m, sizeof(Mixed_Aligned));
  in.close();
}

//-----------------------------------------------------------------------
// simple test
//-----------------------------------------------------------------------
void simple_test()
{
  const char *file = "/tmp/simple";
  int    iw=10;
  float  fw=3.14;
  double dw=2.71828;

  simple_write( file , iw, fw , dw );

  std::cout << "writing " << iw << " " << fw << " " << dw << " to file in binary format " << std::endl;
  std::cout << "file " << file << " written to disk, size = " << get_file_size( file ) << std::endl;

  int    ir;
  float  fr;
  double dr;

  simple_read( file , ir, fr , dr );
  std::cout << "read " << ir << " " << fr << " " << dr << " from file in binary format " << std::endl;
}

//-----------------------------------------------------------------------
// struct test
//-----------------------------------------------------------------------
void struct_test()
{
  const char *file="/tmp/struct_test";
  
  std::cout << "size of Mixed_Unaligned = " << sizeof( Mixed_Unaligned) << std::endl;
  std::cout << "size of Mixed_Aligned   = " << sizeof( Mixed_Aligned) << std::endl;

  Mixed_Aligned mw( 33 , 1.11 , 25000000 , 79.545454 );
  std::cout << "writing Mixed_Aligned " << mw.i1 << " " << mw.f1 << " " << mw.l1 << " " << mw.d1 << std::endl;
  write_mixed_aligned( file , mw );
  std::cout << "file " << file << " written to disk, size = " << get_file_size( file ) << std::endl;
  

  Mixed_Aligned mr;
  read_mixed_aligned( file , mr );
  std::cout << "reading Mixed_Aligned from " << file 
	    << " results = " << mr.i1 << " " << mr.f1 << " " << mr.l1 << " " << mr.d1 << std::endl;
}

//-----------------------------------------------------------------------
// main 
//-----------------------------------------------------------------------
main( int , char ** )
{
  std::cout << "Simple Binary I/0 Test " << std::endl;
  simple_test();
  std::cout << std::endl;
  std::cout << "Word Aligned Struct Binary I/0 Test " << std::endl;
  struct_test();
}

