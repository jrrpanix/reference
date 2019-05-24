#ifndef _calc_CalcException_h_
#define _calc_CalcException_h_

#include <exception>
#include <string>

namespace calc
{
  class CalcException : public std::exception
    {
    public:
      CalcException() throw() {}
      CalcException( const std::string &w ) throw() : what_(w) {}
      
      virtual ~CalcException() throw();
      virtual const char* what() const throw();
      
    private:
      std::string what_;
      
    };
}


#endif
