
#include "MLReader.h"
#include <fcntl.h>
#include <zlib.h>
#include <string.h>
#include <iterator>

using namespace calc;

namespace 
{
  const int _ML_BAD_VERSION = -1;
  const int _ML_ASCII = 1;
  const int _ML_5 = 2;  
  const int _ML_BINARY = 3;


  const int _ML_MATRIX = 14;
  const int _ML_COMPRESSED_MATRIX = 15;

  const int _ML_CHAR_MATRIX = 4;
  const int _ML_DOUBLE_MATRIX = 6;

  const char *name_delim = "# name: ";
  const char *type_delim = "# type: ";
  const char *row_delim  = "# rows: ";
  const char *col_delim  = "# columns: ";
  const char *matrix_str = "matrix";
  const char *scalar_str = "scalar";

  union i_convert
  { char b[4]; int i; };

  union s_convert
  { char  b[2]; short s; };

  union d_convert
  { char   b[8];double d; };

  int get_file_size( int fd_ )
  {
    int n = lseek( fd_ , 0 ,SEEK_END );
    lseek( fd_ , 0 , SEEK_SET );
    return n ;
  }


  struct _buffer_
  {
    _buffer_() : pos_(0),eol_(0),sz_(0),tr_(0) {}

    std::vector<char> p_; // buffer position
    int pos_ ;            // current position
    int eol_;             // next \n
    int sz_;              // buffer size 
    int tr_;              // total chars read/skipped

    //---------------------------------------
    // read from file into buffer
    //---------------------------------------
    void read_to( int fd , int sz )
    {
      pos_=0;
      p_.clear();
      p_.resize(sz);
      int nr = read( fd , (void *)&p_[0] , sz );
      sz_= nr;
      tr_ += nr;
    }

    //---------------------------------------
    // uncompress a source buffer onto this buffer
    //---------------------------------------
    int uncompress_to( const _buffer_ &src_buffer )
    {
      // make a conservative estimate of compression ratio
      // uncompress using zlib 
      const int compression_ratio = 10; // 
      uLongf uncompressed_size = src_buffer.sz_ * compression_ratio;
      p_.resize(uncompressed_size);
      int fv = uncompress( (Bytef*)&p_[0], &uncompressed_size , (const Bytef *)&src_buffer.p_[0],src_buffer.sz_ );
      pos_=0, sz_=uncompressed_size ;
      return fv;
    }

    //---------------------------------------
    // ascii buffer operations
    //---------------------------------------
    bool at_eob() 
    {
      return pos_ >= sz_;
    }

    void set_eol()
    {
      if ( pos_ >= sz_ ) eol_ = sz_;
      else{
	char *p = strchr( &p_[pos_] , '\n' );
	if ( !p) eol_ = sz_;
	else eol_ = p - &p_[0];
      }
    }

    bool matches_str( const char *str )
    {
      int len = strlen(str);
      int ss = eol_ - pos_;
      if ( len > ss ) return false;
      return strncmp( &p_[pos_] , str , len ) == 0 ;
    }
		      
    std::string get_value( const char *tag )
    {
      int sz = strlen(tag);
      int ss = eol_ - pos_;
      int n = ss -sz;
      std::string str;
      if ( n > 0  )
	str.append( &p_[pos_+sz] , n );
      return str;
    }

    std::string next_token( const char ch = ' ' )
    {
      std::string str;
      while( pos_ < eol_ && p_[pos_] == ch ) pos_++;
      if ( pos_ < eol_ )
	{
	  const char *p = strchr( &p_[pos_] , ch );
	  int n = p - &p_[pos_];
	  str.append( &p_[pos_] , n );
	  pos_ = p - &p_[0];
	}
      return str;
    }

    void advance()
    {
      pos_ = eol_ + 1;
      set_eol();
    }

    //---------------------------------------
    // binary file read operations
    //---------------------------------------
    int read_int( int fd )
    {
      const int n = 4;
      i_convert ic;
      int nr = read( fd , (void *)ic.b , n );
      tr_ += nr;
      return ic.i;
    }

    int get_int()
    {
      const int n = 4;
      i_convert ic;
      std::copy( &p_[pos_] , &p_[pos_] +n , ic.b );
      pos_ += n;
      return ic.i;
    }

    int get_short()
    {
      const int n = 2;
      s_convert ic;
      std::copy( &p_[pos_] , &p_[pos_] +n , ic.b );
      pos_ += n;
      return ic.s;
    }

    double get_double()
    {
      const int n = 8;
      d_convert dc;
      std::copy( &p_[pos_] , &p_[pos_] +n , dc.b );
      pos_ += n;
      return dc.d;
    }

    std::string get_string( int n )
    {
      std::string str;
      int k = 0;
      for( ; k < n ; k++ )
	if ( (int)p_[pos_+k]==0 )
	  break;
      str.append( &p_[pos_] , k );
      pos_ += n;
      return str;
    }

    char get_char()
    {
      return  p_[pos_++];
    }


  };
}


MLVarMapPtr MLReader::load( const char *file ) throw(CalcException)
{
  debug_=false;
  res_=MLVarMapPtr( new MLVarMap() );
  fd_ = open( file , O_RDONLY );
  if ( fd_ < 0 )
    {
      std::stringstream strm;
      strm << "MLReader::read - unable to open file " << file ;
      throw CalcException( strm.str());
    }
  sz_ = get_file_size(fd_);
  int ver = get_version();
  switch( ver )
    {
    case _ML_ASCII: 
      read_ascii();
      break;
    case _ML_5:
      read_5();
      break;
    default:
      unsupported_version();
    }
  return res_;
}
std::string MLReader::get_header()
{
  char buffer[116];
  int nr = read( fd_, (void *)buffer , 116 );
  lseek( fd_ , 0 , SEEK_SET );
  std::string str;
  str.append( buffer , nr );
  return str;
}

int MLReader::get_version()
{
  static const char *ml_5 = "MATLAB 5.0";
  static const char *ml_ascii = "#";

  std::string header = get_header();

  if ( header.length() >= strlen(ml_5) && !strncmp(ml_5,header.c_str(),strlen(ml_5) ) )
    return _ML_5;
  if ( header.length() >= strlen(ml_ascii) && !strncmp(ml_ascii,header.c_str(),strlen(ml_ascii) ) )
    return _ML_ASCII;
  return _ML_BAD_VERSION;
}

void MLReader::unsupported_version()
{
  std::string header = get_header();
  std::stringstream strm;
  strm << "MLReader unsuported MATLab file header = " << header;
  throw CalcException(strm.str());
}

//--------------------------------------------------------
// Read ASCII
//--------------------------------------------------------
void MLReader::read_ascii()
{
  std::string line;
  lseek( fd_ , 0 , SEEK_SET );
  _buffer_ buff;
  buff.read_to( fd_ , sz_ );
  buff.set_eol(); // skip header line
  while( !buff.at_eob() )
    {
      buff.advance();
      MLVarPtr mvar = asc_next_ele( (void *)&buff );
      if ( mvar )
	(*res_)[mvar->name]=mvar;
    }
}

MLVarPtr MLReader::asc_next_ele( void *ptr )
{
  MLVarPtr varP;
  _buffer_ *b = (_buffer_ *)ptr;
  if ( !b->matches_str( name_delim ) ) return varP;
  std::string name = b->get_value( name_delim );
  b->advance();
  std::string type = b->matches_str( type_delim ) ? b->get_value(type_delim) : "" ;
  if ( type == matrix_str )
    {
      varP = MLVarPtr( new MLVar() );
      varP->name = name;
      b->advance();
      std::string rstr = b->matches_str( row_delim ) ? b->get_value(row_delim) : "";
      b->advance();
      std::string cstr = b->matches_str( col_delim ) ? b->get_value(col_delim) : "";
      int rows = atoi( rstr.c_str() ) , cols = atoi( cstr.c_str() );
      varP->value.resize(rows,cols);
      for( int i = 0 ; i < rows ; i++ )
	{
	  b->advance();
	  for( int j = 0 ; j < cols ; j++ )
	    {
	      std::string tok = b->next_token();
	      double d = atof( tok.c_str() );
	      varP->value(i,j) = d;
	    }
	}
    }
  else if ( type == scalar_str )
    {
      varP = MLVarPtr( new MLVar() );
      varP->name = name;
      b->advance();
      std::string sstr = b->get_value( "" );
      int s = atoi( sstr.c_str() );
      varP->value.resize(1,1);
      varP->value(0,0)=s;
    }
  return varP;
}

//--------------------------------------------------------
// Read MATLAB 5.0 
//--------------------------------------------------------
void MLReader::read_5()
{
  if ( debug_)
    std::cout << "loading 5" << std::endl;
  int header_skip = 116 + 4 + 4 + 2  + 2 ;
  _buffer_ buff;
  buff.tr_ = header_skip;
  lseek( fd_ , header_skip , SEEK_SET );
  while( buff.tr_ < sz_ )
    {
      if (debug_)
	std::cout << buff.tr_ << " " << sz_ << std::endl;
      MLVarPtr varP;
      int vtype = buff.read_int( fd_ );
      switch( vtype )
	{
	case _ML_MATRIX:
	  varP=process_matrix( (void *) &buff );
	  break;
	case _ML_COMPRESSED_MATRIX:
	  varP=process_compressed_matrix( (void *) &buff );
	  break;
	default:
	  std::stringstream strm;
	  strm << "MLReader::read_5 currently does not support Matlab type " << vtype ;
	  throw CalcException( strm.str());
	  break;
	}
      if ( varP )
	(*res_)[varP->name]=varP;
    }
}

MLVarPtr MLReader::process_matrix( void *p )
{
  if ( debug_)
    std::cout << "process matrix" << std::endl;
  _buffer_ *b = (_buffer_ *)p;
  int bytes = b->read_int(fd_);
  b->read_to( fd_ , bytes );
  return create_matrix( (void *)b );
}

MLVarPtr MLReader::process_compressed_matrix( void *p )
{
  if ( debug_)
    std::cout << "process compressed" << std::endl;
  _buffer_ *b = (_buffer_ *)p;
  int bytes = b->read_int( fd_ );
  b->read_to( fd_ , bytes );
  _buffer_ uncomp_buffer ; // buffer to hold uncompressed data
  uncomp_buffer.uncompress_to( *b );
  return create_matrix( (void *)&uncomp_buffer );
}

void MLReader::dump_matrix( void *p , int spos , std::ostream &strm )
{
  _buffer_ *b = (_buffer_ *)p;
  b->pos_ = spos;
  for( int i = 0 ; i < 11 ; i++ )
    strm << b->get_int() << std::endl;
  strm << b->get_string(8) << std::endl;
  int ent = b->get_int();
  strm << ent << std::endl;
  for( int i = 0; i < ent; i++ )
    strm << i << " " << (int)b->get_char() << std::endl;
}

MLVarPtr  MLReader::create_matrix( void *p )
{
  _buffer_ *b = (_buffer_ *)p;
  if ( debug_ )
    std::cout << "size = " << b->sz_ << std::endl;
  int spos = b->pos_;
  int f1  = b->get_int();
  int bytes = b->get_int();
  int f2 = b->get_int();
  int f3 = b->get_int();
  int mt = b->get_int(); // matrix type 6 = double , 4 = char

  int f4 = b->get_int();
  int f5 = b->get_int();

  if ( debug_)
    {
      std::cout << f1 << std::endl;
      std::cout << bytes << std::endl;
      std::cout << f2 << std::endl;
      std::cout << f3 << std::endl;
      std::cout << mt << std::endl;
      std::cout << f4 << std::endl;
      std::cout << f5 << std::endl;
    }
  // get bytes in matrix dimension
  int mdim_bytes = b->get_int();
  if ( debug_)
    std::cout << mdim_bytes << std::endl;
  int ndim = mdim_bytes / 4;
  std::vector<int> dim(ndim,0);
  for(int k = 0 ; k < ndim; k++ )
    {
      int d = b->get_int();
      dim[k]= d ;
    }

  if ( ndim > 2 )
    {
      std::stringstream strm;
      strm << "MLReader::create_matrix only supports 2D Matricies "
	   << " ndim=" << ndim;
      throw CalcException( strm.str());
    }

  int f6 = b->get_int();
  if ( debug_)
    std::cout << f6 << std::endl;
  MLVarPtr varP( new MLVar() );

  varP->name = b->get_string(8);
  if ( debug_ )
    std::cout << varP->name << std::endl;
  if ( mt == _ML_DOUBLE_MATRIX )
    {
      int entries = b->get_int() / 8 ;
      int rows = dim[0] , cols = dim[1];
      if ( debug_ )
	{
	  std::cout << rows << std::endl;
	  std::cout << cols << std::endl;
	}
      if ( rows * cols != entries )
	{
	  std::stringstream strm;
	  strm << "MLReader::create_matrix unexpected format "
	       << " var = " << varP->name
	       << " rows = " << rows
	       << " cols = " << cols
	       << " but entries = " << entries << std::endl;
	  dump_matrix( p , spos , strm );
	  throw CalcException( strm.str() );
	}
      varP->value.resize(rows,cols);
      for( int j = 0 ; j < cols ; j++ )
	for( int i = 0 ; i < rows ; i++ )
	  varP->value(i,j) = b->get_double();
    }
  else if ( mt == _ML_CHAR_MATRIX )
    {
      // Have to figure out how ot represent a char variable ...
      int entries = b->get_int();
      std::string str;
      //std::cout << "entries = " << entries << std::endl;
      for( int k = 0 ; k < entries ; k++ )
	{
	  int ch = b->get_char();
	  if ( (int)ch == 0 ) continue;
	  str += ch;
	}
      //std::cout << str << std::endl;
      //std::cout << b->get_char() << std::endl;
    }
  return varP;
}




