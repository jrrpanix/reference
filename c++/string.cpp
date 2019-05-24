

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>


static std::string example_line()
{
  std::stringstream strm;
  strm << "ABCDE" << "," << 100.0 << "," << 75 << "," << false ;
  return strm.str();
}

struct Trim { void operator()( std::string &s) { boost::trim(s); } };

static void parse_example()
{
  std::string line = example_line();
  std::vector< std::string> sv;
  boost::split( sv , line , boost::is_any_of( "," ) ); 
  for_each( sv.begin() , sv.end() , Trim() );
  std::string sval ; double dval ; int ival ; bool bval;
  size_t i = 0;
  if ( i < sv.size() ) sval = sv[i++];
  if ( i < sv.size() ) dval = boost::lexical_cast<double>(sv[i++].c_str());
  if ( i < sv.size() ) ival = boost::lexical_cast<int>(sv[i++].c_str());
  if ( i < sv.size() ) bval = boost::lexical_cast<bool>(sv[i++].c_str());

  std::cout << "raw line   : " << line << std::endl;
  std::cout << "parsed line: s[0]=" << sval << ", s[1]=" << dval << ", s[2]=" << ival << ",s[3]=" << bval << std::endl;

}

static void case_example()
{
  std::string case_str = "XyzzZZ";
  std::cout << "mixed case = " << case_str << std::endl;

  boost::to_lower(case_str);
  std::cout << "lower case = " << case_str << std::endl;

  boost::to_upper(case_str);
  std::cout << "upper case = " << case_str << std::endl;
}


static void trim_example()
{
  std::string str  = "   abcde  "; 

  std::cout << "untrimmed   :" << str << std::endl;
  std::cout << "trim left   :" << boost::trim_left_copy(str) << std::endl;
  std::cout << "trim right  :" << boost::trim_right_copy(str) << std::endl;

  std::string line = "0123456789";
  std::cout << "number line :" << line << std::endl;
  boost::trim_left_if( line , boost::is_any_of( "0" ));
  std::cout << "trim 0      :" << line << std::endl;
  boost::trim_right_if( line , boost::is_any_of( "9" ));
  std::cout << "trim 9      :" << line << std::endl;
  
}


static void replace_example()
{

  std::string str = "XXX4567This is aXTestXXX123456";

  std::cout << "before replace : " << str << std::endl;
  boost::replace_all( str , "X" , " " );
  std::cout << "after  replace : " << str << std::endl;
  boost::erase_all( str , "Test" );
  std::cout << "after  erase Test : " << str << std::endl;
  boost::erase_head( str , 6 );
  std::cout << "after erase head 6 : "  << str << std::endl;
  boost::erase_tail( str , 5 );
  std::cout << "after erase tail 5 : "  << str << std::endl;
  

}

static void compare_example()
{
  std::string x1 = "abcde";
  std::string x2 = "Abcde.exe";
  std::string x3 = "AbcDe";

  bool result ;

  result = boost::lexicographical_compare( x1 , x2 );
  std::cout << "comparing " << x1 << " to " << x2 << " result = " << ( result ? "true" : "false" ) << std::endl;

  result = boost::lexicographical_compare( x2 , x1 );
  std::cout << "comparing " << x2 << " to " << x1 << " result = " << ( result ? "true" : "false" ) << std::endl;

  result = boost::equals( x1 , x3 );
  std::cout << "equals " << x1 << " to " << x3 << " result = " << ( result ? "true" : "false" ) << std::endl;

  result = boost::iequals( x1 , x3 );
  std::cout << "iequals " << x1 << " to " << x3 << " result = " << ( result ? "true" : "false" ) << std::endl;

  result = boost::starts_with( x1 , "abc" );
  std::cout << "starts_with " << x1 << " " << "abc"  << " result = " << ( result ? "true" : "false" ) << std::endl;

  result = boost::ends_with( x2 , ".exe" );
  std::cout << "ends_with " << x2 << " " << ".exe"  << " result = " << ( result ? "true" : "false" ) << std::endl;

  result = boost::iends_with( x2 , ".ExE" );
  std::cout << "iends_with " << x2 << " " << ".ExE"  << " result = " << ( result ? "true" : "false" ) << std::endl;

  result = boost::contains( x2 , "cde" );
  std::cout << "contains " << x2 << " " << "cde"  << " result = " << ( result ? "true" : "false" ) << std::endl;


  
}

