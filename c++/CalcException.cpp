
#include "CalcException.h"

using namespace calc;

CalcException::~CalcException() throw() 
{
}

const char* CalcException::what() const throw() 
{ 
  return what_.c_str(); 
}
