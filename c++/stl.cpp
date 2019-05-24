

// STL Predicated function, returns true if x is odd, false oterwise
struct IsOdd : public std::unary_function<bool,int>
{ bool operator() ( int x ) { return x % 2 != 0 ; } };


int main( int argc , char **argv )
{

  std::vector<int> x;

  // initialize x with integers ranging from 1 to 10
  for( int i = 1 ; i <= 10 ; i++ ) x.push_back(i);

  // (1) using stl built-in binary predicates
  //
  // some built in binary predicates 
  // std::less, std::less_equal , std::greater , equal , std::not_equal_to , 
  // a binary predicate takes 2 arguments x1 and x2 , does an operation and returnes eitehr true or false
  // predicate funtions evaluate using the operator() method from a defined class which typicall has just
  // the operator() defined.
  // the structure for is bool opearator()( const arg1 , const arg2 );

  // for example the less<> binary predicate for an integer would be defined as 
  std::less<int> lt_predicate;
  std::cout << "Example 1" << std::endl;
  std::cout << "is " << x[1] << " <  " << x[2] << " ? " << ( lt_predicate(x[1],x[2]) ? "yes" : " no" ) << std::endl;
  std::cout << "is " << x[2] << " <  " << x[1] << " ? " << ( lt_predicate(x[2],x[1]) ? "yes" : " no" ) << std::endl;
  std::cout << std::endl;

  //
  // (2) using stl built-in binary predicates on the fly
  //
  std::cout << "Example 2" << std::endl;
  std::cout << "is "  << x[0] << " <  " << x[1] << " ? " << ( std::less<int>()(x[0],x[1]) ? "yes" : " no" ) << std::endl;
  std::cout << "is "  << x[1] << " <  " << x[0] << " ? " << ( std::less<int>()(x[1],x[0]) ? "yes" : " no" ) << std::endl;
  std::cout << "is "  << x[0] << " >  " << x[1] << " ? " << ( std::greater<int>()(x[0],x[1]) ? "yes" : " no" ) << std::endl;
  std::cout << "is "  << x[1] << " >  " << x[0] << " ? " << ( std::greater<int>()(x[1],x[0]) ? "yes" : " no" ) << std::endl;
  std::cout << "is "  << x[1] << " <= " << x[1] << " ? " << ( std::less_equal<int>()(x[1],x[1]) ? "yes" : " no" ) << std::endl;
  std::cout << "is "  << x[1] << " == " << x[1] << " ? " << ( std::equal_to<int>()(x[1],x[1]) ? "yes" : " no" ) << std::endl;
  std::cout << "is "  << x[2] << " == " << x[3] << " ? " << ( std::equal_to<int>()(x[2],x[3]) ? "yes" : " no" ) << std::endl;
  std::cout << "is "  << x[1] << " <> " << x[1] << " ? " << ( std::not_equal_to<int>()(x[1],x[1]) ? "yes" : " no" ) << std::endl;
  std::cout << "is "  << x[2] << " <> " << x[3] << " ? " << ( std::not_equal_to<int>()(x[2],x[3]) ? "yes" : " no" ) << std::endl;
  std::cout << std::endl;

  //
  // (3) Converting binary predicates to unary predicates using binder2nd
  // 
  // suppose you want to use less<> binary predicate to iterate through an array
  // and see if each element is less than the constant "4", the binary predicate
  // can be turned into a unary predicate by using the bind2nd
  //
  const int four = 4;
  std::cout << "Example 3" << std::endl;
  std::remove_copy_if( x.begin() , x.end() , std::ostream_iterator<int>( std::cout , " " ) , std::bind2nd( std::less<int>() , four ));
  std::cout << std::endl;
  
  // an example of a declaration 
  std::binder2nd< std::less<int> > lt_4_predicate( std::less<int>() , four );
  for( size_t i = 0 ; i < x.size() ; i++ )
    std::cout << x[i] << " < " << four << " ? " << ( lt_4_predicate( x[i] ) ? "yes" : " no" ) << std::endl;
  std::cout << std::endl;

  //
  // (4) remove_copy_if
  // 
  std::cout << "Example 4" << std::endl;
  std::cout << "even "; 
  std::vector<int> xeven;
  std::remove_copy_if( x.begin() , x.end() , std::back_inserter( xeven ) , IsOdd() );
  std::copy(xeven.begin(),xeven.end(),std::ostream_iterator<int>(std::cout, " " ) );
  std::cout << std::endl;
  std::cout << std::endl;

  //
  // (6) 
  //
  int n = std::count_if( x.begin() , x.end() , std::bind2nd( std::less<int>() , 4 ) );
  std::cout << "elements lt 4 = " << n << std::endl;


  return 0;
}


