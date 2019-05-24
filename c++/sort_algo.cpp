
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include "time_wrap.h"

static void init( unsigned int seed=500 )
{ srand(seed); }

static inline int rnum()
{ return rand() % 1000 ; }

template< typename T>
static void genvec( std::vector<T> &v , int n )
{
  v.resize(n);
  std::generate( v.begin() , v.end() , rnum);
}

template <typename T>
inline void dump( const T *A , const T *end , bool flag=false)
{
  if ( !flag ) return;
  std::copy( A , end , std::ostream_iterator<T>( std::cout , " " ) );
  std::cout << std::endl;
}

//-----------------------------------------------------------------------
// Bubble Sort
// O(N^2)
// Most inefficient sort Algo 
//-----------------------------------------------------------------------
template <typename T>
void bubble_sort( T *A , T *end )
{
  int n = end - A;
  dump( A , end );
  for( int i = 1 ; i < n ; i++ )
    {
      for( int j = n-1 ; j >= i ; j-- )
	{
	  if ( A[j] < A[j-1] )
	    {
	      std::swap( A[j],A[j-1]);
	    }
      }
      dump( A , end );
    }
}

//-----------------------------------------------------------------------
// Insertion Sort
//-----------------------------------------------------------------------
template <typename T>
void insertion_sort( T *A , T *end )
{
  int n = end - A;
  dump( A , end );
  for( int j = 1 ; j < n ; j++ )
    {
      T key = A[j];
      int i = j - 1;
      while( i >= 0 && A[i] > key )
	{
	  A[i+1]=A[i];
	  i--;
	}
      A[i+1]=key;
      dump( A , end );
    }
}

//-----------------------------------------------------------------------
// Merge Sort
// Take Array A
// Break into 2
// call 
//  mergesort( lower side )
//  mergesort( upper side )
// then merge
// 
//-----------------------------------------------------------------------
template <typename T>
void merge( T *A , T* A_end , T *B_end )
{
  T *a = A , *a1 = 0 , *a1_end = 0;
  T *b = A_end;
  
  int n = B_end - A;

  T *c = new T[ n ];
  int i = 0;
  while ( a != A_end  && b != B_end )
    {
      if ( *a < *b )
	c[i++] = *a++;
      else
	c[i++] = *b++;
    }
  if ( a != A_end )
    std::copy( a , A_end , &c[i] );
  else if ( b != B_end )
    std::copy( b , B_end , &c[i] );
  std::copy( &c[0] , &c[0] + n , A );
  delete [] c;
}

template <typename T>
void merge_sort( T *A , T *end )
{
  int n = (end - A);
  if ( n <= 1 ) return;
  int mid =  n / 2;
  merge_sort( A , A + mid );
  merge_sort( A + mid , end );
  merge( A , A + mid , end );
}


//-----------------------------------------------------------------------
// Heap Sort
//
// heap P-parent
// P[key] >= child key
// indexing parent / child by using array to hold heap
// parent = i/2
// left   = 2*i
// right  = 2*i + 1
//-----------------------------------------------------------------------
inline int div2( int n )
{ return n >> 1; }

inline int mul2( int n )
{ return n << 1; }

inline int mul2p1( int n )
{ return n << 1 | 1; }

inline int right( int i , int size )
{ 
  int j = mul2p1(i);
  return j < size ? j : size;
}

inline int left( int i , int size )
{ 
  int j = mul2(i);
  return j < size ? j : size;
}


template< typename T>
void heapify( T *H , T *end , int i )
{
  int size = end - H;
  if ( i >= size ) return;
  int l = left(i,size);
  int r = right(i,size);
  int largest;
  if ( l < size && H[l] > H[i])
    largest = l;
  else
    largest = i;
  if ( r < size && H[r] > H[largest] )
    largest = r;
  if ( largest != i )
    {
      std::swap( H[i] , H[largest] );
      heapify( H , end , largest );
    }
}

template< typename T>
void build_heap( T *H , T *end )
{
  int size = end - H;
  for( int i = size / 2; i >= 0 ; i-- )
    heapify( H , end , i );
}

template <typename T>
void heap_sort( T*H , T *end )

{
  if ( H == end ) return;
  build_heap( H , end );
  int size = end - H;
  for( int i = size - 1 ; i >= 1 ; i-- )
    {
      std::swap( H[0], H[i] );
      end--;
      heapify( H , end , 0 );
    }
}


//-----------------------------------------------------------------------
// Quick Sort
//
// Take array A that goes from index p to r
// pick a piviot element (can be random)
// piviot Element = A[i]
// partition 
//   move all elements <= A[i] to left
//   move all elements >= A[i] to rigth
//   return piviot index q
// 
// now sort the subpartitions 
//   quicksort( A , p , q ) 
//   quicksort( A , q+1 , r ) 
// stop when p >= r
//
//-----------------------------------------------------------------------

// A = Array , p = starting index , q = ending index (not length)
// return index of where partiion is
template <typename T>
int partition( T *A , int p , int q )
{
  T x = A[p];
  int i = p-1;
  int j = q+1;
  
  while( true )
    {
      while( A[++i] < x );
      while( A[--j] > x );
      if ( i >= j ) break;
      std::swap( A[i] , A[j] );
    }
  return j;
}

template <typename T>
void quick_sort( T *A , int p , int r )
{
  if ( p >= r ) return;
  int q = partition( A , p , r );
  quick_sort( A , p , q );
  quick_sort( A , q + 1 , r );
}

template <typename T>
void quick_sort( T*A , T *end )
{
  if ( A == end ) return;
  quick_sort<T>( A , 0 , end - A - 1);
}



//-----------------------------------------------------------------------
// Run Trial
//-----------------------------------------------------------------------
template <typename T>
void trial( void (*SortF)(T * , T * ) ,
	    std::vector<int>    &nsim , 
	    std::vector<long>   &usec , 
	    const char          *name, 
	    bool order_before=false )
{
  nsim.clear();
  usec.clear();
  int last = 102400;
  for( int i = 100 ; i <= last ; i *= 2 )
    {
      std::vector<T> v;
      genvec(v,i);
      if ( order_before ) SortF( &v[0] , &v[0] + v.size() );
      time_wrap::Time begin = time_wrap::Time::now();
      SortF( &v[0] , &v[0] + v.size() );
      time_wrap::Time end = time_wrap::Time::now();
      nsim.push_back(i);
      usec.push_back( (end.ticks_usec()-begin.ticks_usec());
    }
}

template <typename T>
void run_algo( void (*SortF)(T * , T * ) ,
	       std::vector<int>         &nsim , 
	       std::vector<long>        &usecT , 
	       std::vector<std::string> &nameV , 
	       const char               *name, 
	       bool order_before=false )
{
  std::vector<long> usec;
  trial<int>(SortF,nsim,usec,name,order_before);
  usecT.insert( usecT.end() , usec.begin() , usec.end());
  nameV.push_back(name);
}


void sort_test( bool order_before )
{
  std::vector<int> nsim;
  std::vector<long> usecT;
  std::vector<std::string> nameV;
  run_algo<int>(quick_sort,nsim,usecT,nameV,"quick",order_before);
  run_algo<int>(merge_sort,nsim,usecT,nameV,"merge",order_before);
  run_algo<int>(heap_sort,nsim,usecT,nameV,"heap",order_before);
  run_algo<int>(insertion_sort,nsim,usecT,nameV,"insertion",order_before);


  int n = nsim.size();
  for( size_t i = 0 ; i < nameV.size() ; i++ )
    {
      if ( i == 0 ) std::cout << std::setw(15) << "n" << ",";
      if ( i != 0 ) std::cout << ",";
      std::cout << std::setw(15) << nameV[i];
    }
  std::cout << std::endl;
  for( int i = 0 ; i < nsim.size() ; i++ )
    {
      std::cout << std::setw(15) << nsim[i] << ",";
      for( int j = 0 ; j < nameV.size() ; j++ )
	{
	  if ( j != 0 ) std::cout << ",";
	  std::cout << std::setw(15) << usecT[j*n+i];
	}
      std::cout << std::endl;
    }
  
}

int main( int , char ** )
{
  init();
  std::cout << "Running sort timing (us) with random data" << std::endl;
  sort_test(false);
  std::cout << std::endl;
  std::cout << "Running sort timing (us) with sorted data" << std::endl;
  sort_test(true);
}
