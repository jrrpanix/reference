
//--------------------------------------------------------------------------------
//
// this program demonstrates one of the uses of the keyword "volatile" in c++.
// 
// one of the purposes of volatile is a compilier directive when the compilier optimized code.
// compiliers can optimize code by caching values, in single threded code this is not a 
// a problem, becuase there is only one control sequence to modify variables.  In multi-thredded
// code, thread one maybe waiting on the variable change in a loop and thread two may be modifiying
// the variable.  If the variable is not marked as volatile, when code is optimized, it will cache
// the value at teh beginning of the loop and never relize its value is changing in another thread..
//
// If a variable is marked with the keyword "volatile" the comipier will not cache the varaible
// value in a register and will instead access an actual memory location. this code demonstrates
// that behavior. the code is going to be compiled 2 ways , first with out optimization, then
// with optimization
// 
//
//---------------------------------------------------------------------------------
// first comiplie without optimization 
//
//  g++ -g vol_test.cpp -ovol_test -lpthread
//
//---------------------------------------------------------------------------------
// output - With NO Compilier Optimizations the code works

// calling volatile variable thread first 
// start : volatile bool
// vstop Done
// exit_thread flag set, we can join 
// stop  : volatile bool

// calling non-volatile variable thread next
// start : regular bool
// stop Done
// exit_thread flag set, we can join 
// stop  : regular bool

// program exit

//---------------------------------------------------------------------------------
// next comiplie with optimization 
//
// g++ -O4 vol_test.cpp -ovol_test -lpthread
//
//---------------------------------------------------------------------------------

// output - With Compilier Optimizations the regular bool check stays in an infinite loop
// in the case the thread that checks varible that is volatile works, while the thread
// that checks the variable that is not-volatile does not work correctly!
//
// calling volatile variable thread first 
// start : volatile bool
// vstop Done
// exit_thread flag set, we can join 
// stop  : volatile bool
//
// calling non-volatile variable thread next
// start : regular bool
// exit_thread flag never set!!!!!!!!
// 
// program exit



//---------------------------------------------------------------------------------
// begin actul code
//---------------------------------------------------------------------------------
#include <pthread.h>
#include <errno.h>
#include <iostream>



struct StopCond
{
  StopCond() : stop(false),vstop(false),exit_thread(false)
  {}

  bool          stop;
  volatile bool vstop;

  bool          exit_thread;

};

void *loop_till_stop( void *p )
{
  StopCond *scond = (StopCond *)p;
  while( scond->stop == false ) {}
  scond->exit_thread = true;
  std::cout << "stop Done" << std::endl;
}

void *loop_till_vstop( void *p )
{
  StopCond *scond = (StopCond *)p;
  while( scond->vstop == false ) {}
  scond->exit_thread = true;
  std::cout << "vstop Done" << std::endl;
}

void start_expr( void *(*run_func)(void *) , const char *str )
{
  
  std::cout << "start : " << str << std::endl;
  StopCond cond;
  pthread_t tid;
  pthread_create( &tid , (pthread_attr_t *)NULL , run_func , (void *) &cond );
  sleep(1);
  cond.stop = true;
  cond.vstop =true;
  sleep(1);
  if ( cond.exit_thread )
    {
      std::cout << "exit_thread flag set, we can join " << std::endl;
      pthread_join( tid , (void **) NULL );
      std::cout << "stop  : " << str << std::endl;
    }
  else
    {
      std::cout << "exit_thread flag never set!!!!!!!!" << std::endl;
    }
}


int main( int , char ** )
{
  std::cout << "calling volatile variable thread first " << std::endl;
  start_expr( loop_till_vstop , "volatile bool" );
  std::cout << std::endl;

  std::cout << "calling non-volatile variable thread next" << std::endl;
  start_expr( loop_till_stop , "regular bool" );
  std::cout << std::endl;

  std::cout << "program exit" << std::endl;
  return 0;
}


