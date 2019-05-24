#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <iterator>

//
// to compile g++ -I/usr/include -I/usr/local/include prog_options.cpp -o prog_options -L/usr/local/lib -lboost_program_options
//

namespace po = boost::program_options;

po::variables_map init( int argc , char **argv )
{
  int begin,end;
  std::vector<std::string> sv;
  po::options_description desc;

  /*
    allows command line options like 
    --begin or -b
    --end or -e
    --symbols -s

    example

    ./program_options -b 20120101 -s A -s B -s C

   */
    
  desc.add_options()
    ("help,h", "help")
    ("begin,b", po::value<int>()->default_value(20000101) , "begin date" )
    ("end,e", po::value<int>()->default_value(29990101) , "end_date" )
    ("symbols,s", po::value<std::vector< std::string > >(), "symbols" )
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc) , vm );

  if ( vm.count("help") )
    std::cout << desc << std::endl;
  if ( vm.count("begin") )
    std::cout << "begin = " << vm["begin"].as<int>() << std::endl;
  if ( vm.count("end") )
    std::cout << "end = " << vm["end"].as<int>() << std::endl;
  if ( vm.count("symbols") )
    {
      std::vector< std::string > xv = vm["symbols"].as< std::vector< std::string> >();
      std::copy( xv.begin() , xv.end() , std::ostream_iterator<std::string>(std::cout,"\n"));
    }
  return vm;
}

int main( int argc , char **argv )
{
  po::variables_map vm =  init( argc , argv );
}

