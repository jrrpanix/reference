#ifndef _calc_MLReader_h
#define _calc_MLReader_h

#include "CalcException.h"
#include "MLVar.h"

namespace calc
{
  class MLReader
  {
  public:

    MLVarMapPtr load( const char *file ) throw(CalcException);

  public:
    bool debug_;

  private:
    int fd_;
    int sz_;
  private:
    std::string get_header();
    int get_version();
    void unsupported_version();

    void read_ascii();
    MLVarPtr asc_next_ele( void *b );

    void read_5();
    MLVarPtr process_matrix( void *p );;
    MLVarPtr process_compressed_matrix( void *p );
    MLVarPtr create_matrix( void *p );
    void dump_matrix( void *p , int spos , std::ostream &strm );
  private:
    MLVarMapPtr res_;
  };
}



#endif
