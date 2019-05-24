
#include <iostream>
#include <typeinfo>
#include <new>

//
// this example covers constructor basiscs
// implicit construction
// explicit construction
// operator++
// operator +
// operator *
// overloading operator new
// overloading operator delete
// overloading operator<<

class A
{
public:

  // constructors
  A() : key_(0) { std::cout << "A::A()" << std::endl; }

  A ( int x ) : key_(x) { std::cout << "A::A(int)" << std::endl; }

  explicit A( const char *str ) 
  { 
    key_ = str && *str ? (int)str[0] : 0;
    std::cout << "A::A(const char *)" << std::endl; 
  }

  A( const A &x ) : key_(x.key_) { std::cout << "A::A(const A&)" << std::endl; }

  // assignment
  A& operator=( const A &x ) 
  { 
    std::cout << "A::operator==(const A&)" << std::endl; 
    if ( &x == this )
      return *this;
    key_=x.key_;
    return *this;
  }
  
  // destructor
  ~A() { std::cout << "A::~A() : key=" << key_ << std::endl; }

  // post increment
  const A operator++(int)
  {
    // fetch the value , store old, increment , return old
    std::cout << "A A::operator++(int)" << std::endl;
    const A tmp = (*this);
    key_++;
    return tmp;
  }

  // pre increment
  A & operator++()
  {
    // increment then return
    ++key_;
    return *this;
  }
  
  // operator new
  static void * operator new (size_t size )
  {
    std::cout << " static void * operator new (size_t size )" << std::endl;
    if ( size != sizeof(A))
      return ::operator new(size);
    return malloc( size );
  }

  // operator delete
  static void operator delete( void *obj , size_t size )
  {
    std::cout << "  static void operator delete( void *obj , size_t size )" << std::endl;
    if ( size != sizeof(A))
      {
	::operator delete(obj);
	return;
      }
    free(obj);
  }


  int key() const { return key_ ; }

private:
  int key_;
};


// note the return value is const!!!!
const A operator+( const A &x1 , const A &x2 )
{
  // Return on Value Optimization
  std::cout << "const A operator+( const A &x1 , const A &x2 )" << std::endl;
  return A( x1.key() + x2.key() );
}

// note the return value is const !!!!
// if it were not we could to this A a4 = ( 3 + 4 )++;
const A operator*( const A &x1 , const A &x2 )
{
  // No Return on Value Optimization
  std::cout << "const A operator+( const A &x1 , const A &x2 )" << std::endl;
  A a = x1.key() * x2.key();
  return a;
}

std::ostream &operator<<( std::ostream &stream , const A &x )
{ 
  stream << x.key();
  return stream;
}

//------------------------------------------------------------------
// On Stack 
//------------------------------------------------------------------
void simple()
{
  std::cout << "defaut constructor " ;
  A a0;           // calls default constructor

  std::cout << "implicit constructor " ;
  A a1 = 5;       // calls A(int) implicitly

  std::cout << "copy constructor " ;
  A a2 = a1;      // const copy constructor

  std::cout << "assignmnet operator " ;
  a2 = a1;        // calls the assignment operator

  std::cout << "assignmnet operator " ;
  A a3( "abcd" ); // explicit construction calls A( const char * )

  std::cout << "operator +" ;
  A a4 = A(5) + A(11);

  std::cout << "operator *" ;
  A a5 = 5 * 11;

  std::cout << "post increment ";
  a4++;

  std::cout << "pre increment ";
  ++a5;
  
  std::cout << std::endl << "a5=" << a5 << std::endl;

  std::cout << std::endl << "begin destruction" << std::endl;
}

//------------------------------------------------------------------
// void on head
//------------------------------------------------------------------
void heap()
{
  std::cout << "new A()" ;
  A *a0 = new A;
  std::cout << "new A(int)" ;
  A *a1 = new A(5);
  std::cout << "new A[2]" << std::endl;;
  A *a2 = new A[2];

  std::cout << std::endl;
  std::cout << "delete" ;
  delete a0;
  std::cout << "delete" ;
  delete a1;
  std::cout << "delete []" << std::endl;;
  delete [] a2;
}

int main( int , char ** )
{
  simple();
  heap();
}
