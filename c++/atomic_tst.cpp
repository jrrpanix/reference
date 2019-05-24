
#include <iostream>
#include <pthread.h>
#include <vector>
#include "time_wrap.h"

using namespace time_wrap;

//-----------------------------------------------------------------------------
// Data structure to be modified by nthreads
//-----------------------------------------------------------------------------
template< typename T>
struct Add_Data
{
  Add_Data() : loop_count_(0) 
  { pthread_mutex_init( &lock, (pthread_mutexattr_t *)NULL ); }

  void loop_count( unsigned int x ) { loop_count_=x;}
  unsigned int loop_count() const { return loop_count_; }

  void init()
  { begin_=0 , end_ = 0 ; }

  void incr() { end_++; }

  T begin() const { return begin_ ; }
  T end() const { return end_ ; }

public:
  pthread_mutex_t  lock;

public:
  T                begin_;
  T                end_;
  unsigned int     loop_count_;
};

//-----------------------------------------------------------------------------
// increment via method with locking
//-----------------------------------------------------------------------------
template< typename T> 
void *incr_with_mutex( void *p )
{
  Add_Data<T> *data = (Add_Data<T> *)(p);
  unsigned int l_count = 0;
  while( l_count++ < data->loop_count() )
    {
      pthread_mutex_lock(&data->lock);
      data->incr();
      pthread_mutex_unlock(&data->lock);
    }
  return p;
}

//-----------------------------------------------------------------------------
// increment via method no locking
//-----------------------------------------------------------------------------
template< typename T>
void *incr_no_lock( void *p )
{
  Add_Data<T> *data = (Add_Data<T> *)(p);
  unsigned int l_count = 0;
  while( l_count++ < data->loop_count() )
    {
      data->incr();
    }
  return p;
}

//-----------------------------------------------------------------------------
// increment direct no locking
//-----------------------------------------------------------------------------
template< typename T>
void *incr_direct_no_lock( void *p )
{
  Add_Data<T> *data = (Add_Data<T> *)(p);
  unsigned int l_count = 0;
  while( l_count++ < data->loop_count() )
    {
      data->end_++;
    }
  return p;
}

//-----------------------------------------------------------------------------
// check condition then increment via method
//-----------------------------------------------------------------------------
template< typename T>
void *if_x_incr( void *p )
{
  Add_Data<T> *data = (Add_Data<T> *)(p);
  unsigned int l_count = 0;
  while( l_count++ <data->loop_count() )
    {
      if ( data->loop_count() )
	data->incr();
    }
  return p;
}

//-----------------------------------------------------------------------------
// check condition then increment direct
//-----------------------------------------------------------------------------
template< typename T>
void *if_x_incr_direct( void *p )
{
  Add_Data<T> *data = (Add_Data<T> *)(p);
  unsigned int l_count = 0;
  while( l_count++ <data->loop_count() )
    {
      if ( data->loop_count() ) data->end_++;
    }
  return p;
}


//-----------------------------------------------------------------------------
// print results
//-----------------------------------------------------------------------------
void print_header( std::ostream &stream )
{
  stream << std::setw(15) << "type"
	 << std::setw(15) << "operation"
	    << " | "
	 << std::setw(15) << "check_sum"
	    << " | "
	 << std::setw(15) << "run_sum"
	    << " | "
	 << std::setw(15) << "is_atomic?"
	    << " | "
	 << std::setw(15) << "time(us)"
	 << std::endl;
}

void print_result( std::ostream &stream , 
		   const char * typ_str,
		   const char * op_str,
		   unsigned long check_sum,
		   unsigned long sum,
		   bool         is_atomic,
		   long         usec )
{
  std::cout << std::setw(15) << typ_str 
	    << std::setw(15) << op_str
	    << " | "
	    << std::setw(15) << check_sum
	    << " | "
	    << std::setw(15) << sum
	    << " | "
	    << std::setw(15) << ( is_atomic ? "atomic" : "not atomic"  )
	    << " | "
	    << std::setw(15) << usec
	    << std::endl;
}

//-----------------------------------------------------------------------------
// run experiment
//-----------------------------------------------------------------------------
template< typename T >
bool thread_run( void *(*thr_func)(void *) , Add_Data<T> *data  , int threads ,
		 unsigned int &check_sum , unsigned int &run_sum , long &usec )
{
  std::vector<pthread_t> tidV;
  data->init();
  Time tbegin = Time::now();
  for( int i = 0 ; i < threads ; i++ )
    {
      pthread_t tid;
      pthread_create( &tid , (pthread_attr_t *) NULL , thr_func , (void *)(data) );
      tidV.push_back(tid);
    }

  for( size_t i = 0 ; i < tidV.size() ; i++ )
    {
      pthread_join(tidV[i], (void **) NULL );
    }
  if ( threads == 0 )
    (*thr_func)( (void *) data);
  int niter = threads == 0 ? 1 : threads;
  Time tend = Time::now();
  check_sum = niter * data->loop_count();
  run_sum = data->end() - data->begin();
  usec = ( tend.ticks() - tbegin.ticks() ) / Time::ticks_per_usec();
  bool is_atomic = check_sum == run_sum;
  return is_atomic;
}

//-----------------------------------------------------------------------------
// test if atomic
//-----------------------------------------------------------------------------
template< typename T>
bool atomic_test( void *(*thr_func)(void *) , Add_Data<T> *data  , int threads , 
		  const char *typ_str , const char *op_str )
{
  bool is_atomic;
  int i = 0 , num_trials = 15;
  unsigned int check_sum = 0 , run_sum = 0;
  long usec_avg = 0;
  while( i < num_trials )
    {
      unsigned int csum = 0 , rsum = 0;
      long usec = 0;
      is_atomic = thread_run<T>( thr_func , data , threads , csum , rsum , usec );
      i++;
      check_sum+= csum;
      run_sum += rsum;
      usec_avg += usec;

      if ( !is_atomic )
	break;
      
    }
  
  check_sum /= i;
  run_sum /= i;
  usec_avg /= i;

  print_result( std::cout , typ_str , op_str , check_sum , run_sum , is_atomic, usec_avg  );

}


//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
typedef unsigned long run_type;
const char *typ_str = "unsigned long";

int main( int , char ** )
{
  Add_Data<run_type> data;
  data.loop_count(1000000);

  const int  nthreads = 2;

  print_header( std::cout );
  atomic_test<run_type>( if_x_incr<run_type>   , &data , nthreads ,  typ_str , "if(x) incr()" );
  atomic_test<run_type>( incr_no_lock<run_type>   , &data , nthreads , typ_str, "incr()" );
  atomic_test<run_type>( if_x_incr_direct<run_type> , &data , nthreads , typ_str , "if(x) ++ " );
  atomic_test<run_type>( incr_direct_no_lock<run_type> , &data , nthreads , typ_str , "++ " );
  atomic_test<run_type>( incr_with_mutex<run_type> , &data , nthreads , typ_str , "incr() w/mutex" );
  atomic_test<run_type>( incr_no_lock<run_type> , &data , 1 , typ_str ,"incr()new thr" );
  atomic_test<run_type>( incr_no_lock<run_type> , &data , 0 , typ_str ,"incr()main thr" );
}
