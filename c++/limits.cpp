#include <iostream>
#include <limits>

template<typename T>
static void print_limits( std::ostream &stream , const char *t)
{
 std::cout << t << " sizeof=" << sizeof(T) << std::endl
	   << t << " min="<< std::numeric_limits<T>::min() << std::endl
	   << t << " max="<< std::numeric_limits<T>::max() << std::endl
	   << t << " epsilon="<< std::numeric_limits<T>::epsilon() << std::endl
	   << t << " round_error="<< std::numeric_limits<T>::round_error() << std::endl
	   << t << " infinity="<< std::numeric_limits<T>::infinity() << std::endl
	   << t << " denorm_min="<< std::numeric_limits<T>::denorm_min() << std::endl
	   << std::endl;

}

int main( int argc , char ** argv )
{
  print_limits<long double>(std::cout,"long double");
  print_limits<double>(std::cout,"double");
  print_limits<float>(std::cout,"float");

  print_limits<long>(std::cout,"long");
  print_limits<int>(std::cout,"int");
  print_limits<short>(std::cout,"short");

  print_limits<unsigned>(std::cout,"unsigned");
  print_limits<unsigned long>(std::cout,"unsigned long");
  print_limits<unsigned int>(std::cout,"unsigned int");
  print_limits<unsigned short>(std::cout,"unsigned short");

  print_limits<wchar_t>(std::cout,"wchar_t");
  print_limits<char>(std::cout,"char");

  print_limits<bool>(std::cout,"bool");

}
