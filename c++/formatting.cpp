
#include <iostream>
#include <iomanip>
#include <string>


static void format_example()
{
  // double formatting
  std::cout << "double formatting exmaples" << std::endl;
  double x = 123.976537521;
  std::cout << x << std::endl;
  std::cout << std::setw(15) << std::setprecision( 10) << std::setiosflags( std::ios_base::fixed )<< x << std::endl;
  std::cout << std::setw(10) << std::setprecision(5) << std::setiosflags( std::ios_base::fixed )<< x << std::endl;

  // date formatting
  std::cout << std::endl;
  std::cout << "date formatting exmaples" << std::endl;
  int month = 2 , day = 2 , year = 2012 ;
  std::cout << std::setw(2) << std::setfill('0') << month << "/" 
	    << std::setw(2) << std::setfill('0') << day   << "/" 
	    << std::setw(4) << year << std::endl;

  // 
  std::string first_name = "John" , last_name = "Smith" ;
  int favorite_number = 77;

  struct name_rec
  {
    std::string first_name;
    std::string last_name;
    int         favorite_number;
    double      height;
  } NRec[] = { 
    "John" , "Smith", 77 ,60.5,
    "Joe" , "Able", 11 ,66.2,
    "Tim" , "Horseshoe", 14 ,72.3,
    "Jessica" , "Travler", 2, 60.5,
    "" , "" , -1,0.0
  };
  
  std::cout << std::endl;
  std::cout << "record formatting exmaples" << std::endl;
  for( int i = 0 ; NRec[i].favorite_number > 0 ; i++ )
    {
      std::cout << std::setw(12)
		<< std::setfill(' ') 
		<< std::setiosflags( std::ios_base::left )
		<< NRec[i].last_name 
		<< ","
		<< std::setw(8)
		<< std::setfill(' ') 
		<< std::setiosflags( std::ios_base::left )
		<< NRec[i].first_name 
		<< ","
		<< std::setw(5)
		<< std::setfill(' ') 
		<< std::setiosflags( std::ios_base::right )
		<< NRec[i].favorite_number 
		<< ","
		<< std::setw(6)
		<< std::setfill(' ') 
		<< std::setiosflags( std::ios_base::right )
		<< std::setiosflags( std::ios_base::fixed )
		<< std::setprecision(2)
		<< NRec[i].height
		<< std::resetiosflags( std::ios_base::right )
		<< std::resetiosflags( std::ios_base::fixed )
		<< std::endl;
    }

  std::cout << std::endl;
  std::cout << "base manipulation" << std::endl;
  int xb=16;
  std::cout << "base_10=" << xb << ", oct=" << std::oct << xb << ", hex=" << std::hex << xb << std::endl;
  std::cout << std::dec;
  xb=31;
  std::cout << "base_10=" << xb << ", oct=" << std::oct << xb << ", hex=" << std::hex << xb << std::endl;


  // scientific numbers
  double small = 0.000000013455;
  std::cout << std::endl;
  std::cout << "scientific numbers" << std::endl;
  std::cout << "scientific ="
	    << std::scientific << std::setprecision(3) << small
	    << ","
	    << " fixed="
	    << std::fixed << std::setprecision(14) << small
	    << std::endl;
  
  double large = 12570000;
  std::cout << "scientific ="
	    << std::scientific << std::setprecision(5) << large
	    << ","
	    << " fixed="
	    << std::fixed << std::setprecision(0) << large
	    << std::endl;
  


}
