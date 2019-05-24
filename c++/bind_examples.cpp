#include <functional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <boost/bind.hpp>

int sub( int a , int b ) { return a-b ; }
int sub2( int a , int b ) { int t = a-b ; std::cout << t << std::endl ; return t ;}
int add3( int a , int b , int c ) { return a + b + c; }

void stl_bind_overview()
{
  // 
  // the funciton std::less<int>( int x1 , int x2 ) takes 2 arguments and returns a bool which si the result of X1 < X2; 
  //
  /*
    Example 0 demonstration of stl 2 argument functtion
  */
  std::cout << "STL bind1st and bind2nd overview " << std::endl;
  std::cout << "Is 3 < 5 ? " << (  std::less<int>()(3,5) ? "true" : "false" ) << std::endl;; // is three less than 5
  std::cout << std::endl;
  //
  // we can use the bind function in conjuction with a predicate like std::less to "attach" a varaible to either the 
  // first or second argment of bind;
  //
  //
  // int X ;
  // so the expression std::bind1st( std::less<int>() , 3 )(X);
  //
  // makes 3 the first argument of std::less<int>(int,int) and X the second;
  //
  // while the expression std::bind2nd( std::less<int>() , 3 )(X);
  //
  // makes 3 the second argument and X the first

  /*
    Example 1 Demonstration of using std::bind1st and std::bind2nd
  */
  int X = 5;
  
  std::cout << "using bind1st , bind 3 as the first  arg to std::less<>, so is 3 < 5 ? " << ( std::bind1st( std::less<int>() , 3 )(X) ? "true" : "false" ) << std::endl;

  std::cout << "using bind2nd , bind 3 as the second arg to std::less<>, so is 5 < 3 ? " << ( std::bind2nd( std::less<int>() , 3 )(X) ? "true" : "false" ) << std::endl;

  std::cout << std::endl;


  /*
    Example 2 Demonstration of using std::bind2nd with stl::algorithms
  */

  // using bind in conjution with other stl functions to manipualte a vectorwith a vector
  // lets create a vector with 10 elements going in sequence from 1 to 10
  
  std::vector<int> V(10); // note space is pre-allocated ! 

  std::cout << "create a vector with elements               : V = " ;
  std::iota( V.begin() , V.end() , 1 ) ; // creates a sequence from 1 to 10
  std::copy( V.begin() , V.end() , std::ostream_iterator<int>(std::cout," " ) ) ; std::cout << std::endl;

  
  std::vector<int> V_lower, V_upper;

  // dont copy any element from V into V_lower if V[i] >= 5 
  // bind2nd is used to bind 5 to the second argument of std::greater_equal<>
  std::cout << "remove elements greatner than or equal to 5 : V_lower = ";
  std::remove_copy_if( V.begin() , V.end() , std::back_inserter( V_lower ) , 
		       std::bind2nd( std::greater_equal<int>() , 5 ) );
  std::copy( V_lower.begin() , V_lower.end() , std::ostream_iterator<int>(std::cout," " ) ) ; std::cout << std::endl;

  // dont copy any element from V into V_upper if V[i] < 5 
  // bind2nd is used to bind 5 to the second argument of std::less<>
  std::cout << "remove elements less than 5                 : V_upper = ";
  std::remove_copy_if( V.begin() , V.end() , std::back_inserter( V_upper ) , 
		       std::bind2nd( std::less<int>() , 5 ) );
  std::copy( V_upper.begin() , V_upper.end() , std::ostream_iterator<int>(std::cout," " ) ) ; std::cout << std::endl;
  std::cout << std::endl;


  /*
    Example 3 - using stl::ptr_fun , stl::bind1st and stl::bind2nd with custom functions
  */

  int Z = 3;
  std::cout << "bind 5 to the first  arg of sub , 5 - Z = 5 - 3 = " << (std::bind1st( std::ptr_fun(sub) , 5 )(Z)) << std::endl;
  std::cout << "bind 5 to the second arg of sub , Z - 5 = 3 - 5 = " << (std::bind2nd( std::ptr_fun(sub) , 5 )(Z)) << std::endl;

  std::cout << std::endl;
}

void boost_bind_overview()
{
  // boost::bind uses places holders _1, _2 , _3 for args 1 , 2 , 3 etc
  // #include <boost/bind.hpp>
  //
  // 
  // so while stl requires use of std::ptr_fun and either std::bind1st or std::bind2nd boost
  // boost uses place holers , to from our previous example :

  std::cout << "boost::bind overview " << std::endl;

  int Z = 3;

  // this statment binds "Z" to the first argument of sub, note the "_1" placeholder in position to
  // so the effect is to call ls sub( Z , 5 ) which is sub( 3 , 5 ) = -2
  int R1 = boost::bind( sub , _1, 5 )(Z);
  std::cout << "the result of boost::bind( sub , _1 , 5 )(Z), where Z = 3 , is : " << R1 << std::endl;

  // this statment binds "Z" to the second argument of sub, note the "_1" placeholder in position to
  // so the effect is to call ls sub( 5 , Z ) which is sub( 5, 3 ) = 2
  int R2 = boost::bind( sub , 5 , _1 )(Z);
  std::cout << "the result of boost::bind( sub , 5 , _1 )(Z), where Z = 3 , is : " << R2 << std::endl;

  int X1=11,X2=18;
  int R3 = boost::bind( sub , _1 , _2 )(X1,X2);
  std::cout << "the result of boost::bind( sub , _1 , _2 )(X1,X2), where X1=11,X2=18 is : " << R3 << std::endl;

  // note its possible to call boost bind with no place holders
  int R4 = boost::bind( sub , 11, 15 )();
  std::cout << "calling boost::bind( sub, 11, 15)() , note no place holders is equivalent to calling sub(11,15) : " << R4 << std::endl;

  // example binding 3 arguments
  int A=1,B=2,C=3;
  int R5 = boost::bind( add3, _1 , _2 , _3 )(A,B,C);
  std::cout << "calling boost::bind( add3, _1,_2,_3 ),where A=1,B=2,C=3 : " << R5 << std::endl;

  // this will bind all three argument to place holder 1, which is "A"
  int R6 = boost::bind( add3, _1 , _1 , _1 )(A,B,C);
  std::cout << "calling boost::bind( add3, _1,_1,_1 ),where A=1,B=2,C=3 : " << R6 << std::endl;

  // boost bind can be used to make a new function pointer
  // In ths example we are going to bind the first arg of sub to the variable i and get the resultant function pointer 
  // note the return type is really ugly , 
  // 
  // boost::_bi::bind_t<int, int (*)(int, int), boost::_bi::list2<boost::_bi::value<int>, boost::arg<1> > >
  //
  // int j = 3
  // boost::_bi::bind_t<int, int (*)(int, int), boost::_bi::list2<boost::_bi::value<int>, boost::arg<1> > > q = boost::bind( sub, j , _1);  
  //
  // so the better way is to use the key word "auto"

  int j = 3;
  // this creaes a function pointer where the first arg of sub is bindecd to variable j =3 in this example
  auto q = boost::bind( sub, j , _1); 
  std::cout << "result of auto q = boost::bind( sub, j , _1), where j = 3 and calling , q(4) = 3 - 4 = " << q(4) << std::endl;


  

  std::vector<int> W(10);
  std::iota( W.begin(), W.end() , 1 );
  auto q1 = boost::bind( sub2, 2 , _1 );
  for_each( W.begin() , W.end() , q1);

  for_each( W.begin() , W.end() , boost::bind( sub2, 1000 , _1 ));

  std::cout << "done" << std::endl;
}


int main( int argc , char ** argv )
{
  //stl_bind_overview();

  boost_bind_overview();
}

