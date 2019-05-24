
#include <unistd.h>
#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//---------------------------------------------------------------------
//
// pthread compoments covered in this example
//
// functions
// void pthread_create( pthread_t *, pthread_attr_t * , void * (*runnable)( void * ) , void *data )
// void pthread_join( pthread_t * , void ** )
// void pthread_exit( void * )
// pthread_t pthread_self()
// void pthread_once( pthread_once_t * , void (*init_function)())
// int pthread_attr_init( pthread_attr_t * )
// int pthread_attr_getstacksize( pthread_attr_t * , size_t * );
// int pthread_attr_getdetachstate( pthread_attr_t * , int * );
// int pthread_attr_setdetachstate( pthread_attr_t * , int  );
// int pthread_attr_destroy( pthread_attr_t * )
// int sysconf(int) - from <unistd.h>
//
// types
// pthread_t
// pthread_attr_t
// pthread_once_t 
//
// MACROS
// PTHREAD_ONCE_INIT
// PTHREAD_CREATE_JOINABLE
// PTHREAD_CREATE_DETACHED
// PTHREAD_STACK_MIN - from <limits.h>
// _SC_PAGESIZE - from <unistd.h> 
//
// Memory 
// Page - fixed-length contiguous block of memory
// Page table - mapping between virtual and physical memory
// Page size - size of a page
//
// TLB - Translation Lookaside Buffer (CPU cache to improve virtual address translation speed)
// Hash table where key is virtual address, looks in TLB if it finds it its a hit , if not
// its a page miss and must go to the Page Table by doing a "Page Walk"
// 
// Most CPU Archictures are created with multiple TLB Layers
// L1 - Fastest , L2 - Larger and Slower

// Clock Cycles for looking up memory
// TLB hit - 0.5 to 1 clock cycle
// Miss - 10 to 100 clock cycles
//
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// error handeling
//---------------------------------------------------------------------
void thread_error_exit( std::ostream &stream , const char *err , int rval )
{
  stream << err << " error return code" << std::endl;
  exit(-1);
}


//---------------------------------------------------------------------
// thread runnable functions
//---------------------------------------------------------------------
void *thread1_hello( void *info )
{
  pthread_t tid = info ? *((pthread_t *)(info)) : 0 ;
  std::cout << "thread-1=" << tid << ", sleeping for 2 seconds" << std::endl;
  sleep(2) ;
  pthread_exit( (void *) NULL );
  return 0;
}

void *multi_thr_run( void *info )
{
  int *data = (int *)info;
  std::cout << "thread id = " << pthread_self() << ": multi_thr_run data = " << *data << std::endl;
  pthread_exit( (void *) NULL );
  return 0;
}

void init_function()
{
  std::cout << "init_function - called by " << pthread_self() << std::endl;
}

void *init_example( void * )
{
  static pthread_once_t is_init = PTHREAD_ONCE_INIT;
  pthread_once( &is_init , init_function );
  std::cout << "init_example " << pthread_self() << std::endl;
}

typedef void (*Init_Function)() ;

//---------------------------------------------------------------------
// various thread flow of control functions
//---------------------------------------------------------------------
void single_thread_join()
{
  // create a thread wait for it to finish
  pthread_t tid;
  int rval = pthread_create( &tid , (pthread_attr_t *) NULL , thread1_hello , (void *) (&tid) );
  if ( rval != 0 ) thread_error_exit( std::cerr , "pthread create error" , rval );
  std::cout << "thread-main: waiting for " << tid << std::endl;
  rval = pthread_join( tid , (void **) NULL );
  if ( rval != 0 ) thread_error_exit( std::cerr , "pthread join error" , rval );
  std::cout << "thread-main: done! " << std::endl;

}

void single_thread_nojoin()
{
  // create a thread do not wait for it to finish
  // this will lead to the condition where the main thread will exit before
  // the calling thread funciton is complete which is undesired behavior
  pthread_t tid;
  int rval = pthread_create( &tid , (pthread_attr_t *) NULL , thread1_hello , (void *) (&tid) );
  if ( rval != 0 ) thread_error_exit( std::cerr , "pthread create error" , rval );
  std::cout << "Not waiting for " << tid << " to finish <not advisable> :  thread-main: done! " << std::endl;
}

void multi_thread( int nthr )
{
  std::vector<pthread_t> thr_id(nthr,static_cast<pthread_t>(0));
  std::vector<int>       thr_data(nthr,static_cast<int>(0));
  
  for( int i = 0 ; i < nthr ; i++ )
    {
      int *tdata = &(thr_data[i]=i);
      std::cout <<"thread-main: data= " << *tdata << std::endl;
      int rval = pthread_create( &thr_id[i] , (pthread_attr_t *) NULL , multi_thr_run , (void *) (tdata) );
      if ( rval != 0 ) thread_error_exit( std::cerr , "pthread create error" , rval );
      std::cout << "thread-main: created " << thr_id[i] << " thread_no=" << i+1 << std::endl;
    }

  for( size_t i = 0 ; i < thr_id.size() ; i++ )
    {
      int rval = pthread_join( thr_id[i] , (void **) NULL );
      if ( rval != 0 ) thread_error_exit( std::cerr , "pthread join error" , rval );
    }
}

void pthread_once_example()
{
  pthread_t tid;
  // call init_example from new thread
  int rval = pthread_create( &tid , (pthread_attr_t *) NULL , init_example , (void *) (&tid) );
  if ( rval != 0 ) thread_error_exit( std::cerr , "pthread create error" , rval );

  // call init_example from main thread
  init_example((void *) NULL );

  // wait for everything to finish
  std::cout << "thread-main: waiting for " << tid << std::endl;
  rval = pthread_join( tid , (void **) NULL );
  if ( rval != 0 ) thread_error_exit( std::cerr , "pthread join error" , rval );
  std::cout << "thread-main: done! " << std::endl;

}

void thread_attribute()
{
  pthread_attr_t thr1_attr;

  // initialize thread attribute structure
  int rv = pthread_attr_init( &thr1_attr );


  int detach_state = 0;
  rv = pthread_attr_getdetachstate( &thr1_attr , &detach_state );
  std::cout << "JOIN STATE = " << detach_state << ":" 
	    << "PTHREAD_CREATE_JOINABLE = " << PTHREAD_CREATE_JOINABLE << " : " 
	    << "PTHREAD_CREATE_DETACHED = " << PTHREAD_CREATE_DETACHED << std::endl;

  rv = pthread_attr_setdetachstate( &thr1_attr, PTHREAD_CREATE_DETACHED );
  rv = pthread_attr_getdetachstate( &thr1_attr , &detach_state );

  std::cout << "NEW JOIN STATE = " << detach_state << ":" 
	    << "PTHREAD_CREATE_JOINABLE = " << PTHREAD_CREATE_JOINABLE << " : " 
	    << "PTHREAD_CREATE_DETACHED = " << PTHREAD_CREATE_DETACHED << std::endl;

  // check stack size
  size_t stack_size;
  rv = pthread_attr_getstacksize( &thr1_attr , &stack_size );
  std::cout << "STACK SIZE = " << stack_size 
	    << " MIN STACK SIZE = " << PTHREAD_STACK_MIN 
	    << " multiple = " << stack_size / PTHREAD_STACK_MIN << std::endl;

  // check system page size
  int page_size;
  page_size = sysconf(_SC_PAGESIZE);
  std::cout << "PAGE SIZE = " << page_size << std::endl;


  // Free attribue memory
  rv = pthread_attr_destroy( &thr1_attr );
  std::cout << "done with attr destroy " << rv << std::endl;
}


//---------------------------------------------------------------------
// Run Options
//---------------------------------------------------------------------
enum RunOption
  {
    RunOpt_Join=1,
    RunOpt_Detach,
    RunOpt_NoJoin,
    RunOpt_Multi,
    RunOpt_Init,
    RunOpt_Attr
  };

RunOption get_RunOption( const char *str )
{
  if ( !strcmp( str, "--j") || !strcmp( str, "j" )) return RunOpt_Join;
  if ( !strcmp( str, "--d") || !strcmp( str, "d" )) return RunOpt_Detach;
  if ( !strcmp( str, "--nj") || !strcmp( str, "nj" )) return RunOpt_NoJoin;
  if ( !strcmp( str, "--mt") || !strcmp( str, "mt" )) return RunOpt_Multi;
  if ( !strcmp( str, "--i") || !strcmp( str, "i" )) return RunOpt_Init;
  if ( !strcmp( str, "--a") || !strcmp( str, "a" )) return RunOpt_Attr;
  return RunOpt_Join;
}

std::ostream &operator<<(std::ostream &stream , RunOption ro )
{
  switch( ro)
    {
    case RunOpt_Join: stream << "join"; return stream ;
    case RunOpt_Detach: stream << "detach"; return stream ;
    case RunOpt_NoJoin: stream << "no_join"; return stream ;
    case RunOpt_Multi: stream << "multi"; return stream ;
    case RunOpt_Init: stream << "init"; return stream ;
    case RunOpt_Attr: stream << "attr"; return stream ;
    }
}

//---------------------------------------------------------------------
// main
//---------------------------------------------------------------------
int main ( int argc , char** argv )
{

  RunOption run_opt = argc > 1 ? get_RunOption( argv[1] ) :  RunOpt_Join;

  std::cout << "Running with run option = " << run_opt << std::endl;
  
  if ( run_opt == RunOpt_Join )
    {
      single_thread_join();
    }
  else if ( run_opt == RunOpt_NoJoin )
    {
      single_thread_nojoin();
    }
  else if ( run_opt == RunOpt_Multi )
    {
      multi_thread(3);
    }
  else if ( run_opt == RunOpt_Init )
    {
      pthread_once_example();
    }
  else if ( run_opt == RunOpt_Attr )
    {
      thread_attribute();
    }

  exit(0);
}
