
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <iostream>
#include <queue>
#include <iomanip>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#include <ext/hash_map>

//---------------------------------------------------------------------
// To Compile 
//
// g++ -O4 prod_cons.cpp -opc -Wno-deprecated -lrt -lpthread
//
// This example is a simple implementation of the producer - consumer
//
// The Producer Thread will generate random number in a range from
// lower_bound to upper_bound and place them on a Q
//
// The Consumer Thread will read the randomly generated number from
// the Q and assign the "consumer" that subscribes to that randomly 
// generated number.  In this example a consumer is generated for
// each random number in the range.
//
// Each instance will run for N seconds on the range and then
// stop the Q so that each thread exits.  It then prints out
// the nubmer of messages processed by the consumer Q and the 
// number left on the Q still unprocessed.
//
// In general as time N increses and the number of subscribers increses the 
// number of unprocessed messages will grow as the consumer will
// not be able to keep up with the producer.
// 
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// class to hold time
//---------------------------------------------------------------------
class Time
{
public:

  Time()
  { t_.tv_sec=0,t_.tv_nsec=0;}
  
  Time( const struct timespec &t ) : t_(t) 
  {}

  static Time now() 
  { Time tm; clock_gettime( CLOCK_REALTIME, &tm.t_ );  return tm; }

  static long sec_per_nsec()
  { return 1000000000; }

  static long ticks_per_us()
  { return 1000; }

  static long ticks_per_ms()
  { return 1000000; }

  static long ticks_per_sec()
  { return 1000000000; }
  
  long ticks() const 
  { return t_.tv_sec * sec_per_nsec() + t_.tv_nsec; }

  long seconds() const
  { return t_.tv_sec ; }

  long nsec() const
  { return t_.tv_nsec; }
    

private:
  struct timespec t_;
};

//---------------------------------------------------------------------
// Guard class for easier and safer lock/unlock
//---------------------------------------------------------------------
class Guard
{
public:

  Guard( pthread_mutex_t &m )
    :lock_(m)
  { pthread_mutex_lock(&lock_); }

  ~Guard() 
  { pthread_mutex_unlock(&lock_); }

private:
  pthread_mutex_t &lock_;

};

//---------------------------------------------------------------------
// Runnable - virtual object to pass to pthread_create
//---------------------------------------------------------------------
class Runnable
{
public:
  
  virtual void* run( void * ) =  0;
  virtual ~Runnable();
};

Runnable::~Runnable() 
{}

//---------------------------------------------------------------------
// synchronized Q
//---------------------------------------------------------------------
template< class T>
class LockedQ
{
public:
  LockedQ(): stop_(false) 
  { pthread_mutex_init( &lock_ , (pthread_mutexattr_t *)NULL );  }

  bool empty() const
  { Guard guard(lock_); return q_.empty(); }

  T next() const
  { Guard guard(lock_); return q_.front();  }
  
  bool stop() const
  { Guard guard(lock_); return stop_;  }
  
  size_t size() const
  { Guard guard(lock_);  return q_.size();  }
    
  void push( const T &x ) 
  { Guard guard(lock_); q_.push(x);  }

  // operations of empty() and pop have to be combined otherwise it will be a race condition
  bool pop( T &t)
  { Guard guard(lock_); if ( q_.empty() ) return false; t=q_.front(); q_.pop(); return true; }

  void stop( bool x ) 
  { Guard guard(lock_); stop_=x; }

private:
  std::queue<T>           q_;
  bool                    stop_;
  mutable pthread_mutex_t lock_;
};


//---------------------------------------------------------------------
// delcare a int LockedQ
//---------------------------------------------------------------------
typedef LockedQ<int> IntQ;


//---------------------------------------------------------------------
// Producer class
// will generate random numbers between lower bound and upper bound
//---------------------------------------------------------------------
class Producer : public Runnable
{
public:
  Producer( IntQ &q , int lower_bound , int upper_bound )
    :q_(q),lb_(lower_bound),ub_(upper_bound) {}

  virtual ~Producer() ;
  virtual void * run( void * ) ;

private:
  IntQ &q_;
  int   lb_;
  int   ub_;
private:
  static void init();
  int next() const;
};

Producer::~Producer()
{}

void Producer::init()
{
  srand( time(NULL) );
}

int Producer::next() const
{
  int rng = ub_ - lb_ + 1;
  return (rand() % rng) + lb_;
}

void *Producer::run( void *ptr )
{
  static pthread_once_t init_flag = PTHREAD_ONCE_INIT;
  pthread_once( &init_flag , Producer::init );
  int x = 0;
  while( !q_.stop() )
    {
      q_.push( next() );
    }
  return ptr;
}

//---------------------------------------------------------------------
// Consumer
// Each consumer is assigned a number, when that number comes up
// their count goes up
//---------------------------------------------------------------------
class Consumer
{
public:
  Consumer( int s )
    :sub_(s),cnt_(0) {}
  
  int      incr() { return ++cnt_; }

  int      sub() const { return sub_; }
  int      cnt() const { return cnt_; }

private:
  int      sub_;
  int      cnt_;
};

//---------------------------------------------------------------------
// Distributor
// The Distribution take items off the Q and looks to give it to a
// consumer that has that number
//---------------------------------------------------------------------
class Distributor : public Runnable
{
public:
  enum Dist_Algo { Dist_Hash, Dist_Linear };
public:
  Distributor( IntQ &q , std::vector<Consumer *> consumers , Dist_Algo algo )
    :q_(q),consumers_(consumers),algo_(algo)
  {}

  virtual ~Distributor();
  virtual void *run( void * );
private:
  IntQ                    &q_;
  std::vector<Consumer *> consumers_;
  Dist_Algo               algo_; 
private:
  typedef __gnu_cxx::hash_map<int,Consumer *> ConsumerM;
  ConsumerM    cm_;
private:
  void distribute_linear( int x );
  void distribute_hash( int x );
  void init_hash();
};

Distributor::~Distributor()
{}

void *Distributor::run( void *p )
{
  int cnt = 0;
  init_hash();
  while( !q_.stop() )
    {
      int x;
      if ( q_.pop(x) )
	{
	  switch( algo_ )
	    {
	    case Dist_Hash: 
	      distribute_hash(x);
	      break;
	    case Dist_Linear: 
	      distribute_linear(x);
	      break;
	    }
	}
    }
  return p;
}

void Distributor::init_hash()
{
  cm_.clear();
  for( size_t i = 0; i < consumers_.size() ; i++ )
    {
      cm_[ consumers_[i]->sub() ] = consumers_[i];
    }
}

void Distributor::distribute_linear( int x )
{
  for( size_t i = 0 ; i < consumers_.size() ; i++ )
    if ( consumers_[i]->sub() == x )
      {
	consumers_[i]->incr();
	break;
      }
}

void Distributor::distribute_hash( int x )
{
  ConsumerM::iterator i = cm_.find(x);
  if ( i != cm_.end()) (*i).second->incr();
}

//---------------------------------------------------------------------
// Q Performance Statics
//---------------------------------------------------------------------
class QStats
{
public:
  QStats() : seconds(0),range(0),backup(0) {}
  QStats( int s , int r , double b) : seconds(s),range(r),backup(b) {}
public:
  int    seconds;
  int    range;
  double backup;
};

struct qstats_range_ne : public std::binary_function<QStats,int,bool>
{ bool operator()( const QStats &s , int x ) const { return s.range != x ; } };


//---------------------------------------------------------------------
// run function for pthread_create
//---------------------------------------------------------------------

void *run_function( void *p)
{
  if (!p) return p;
  Runnable *runnable = (Runnable *)p;
  return runnable->run(p);
}

//---------------------------------------------------------------------
// producer-consumer
//---------------------------------------------------------------------

double producer_consumer( Distributor::Dist_Algo algo , int run_seconds , int lower_bound , int upper_bound ,bool verbose )
{
  // run the producer consumer model, return the % the Q is backed up
  // create the Q
  IntQ q;
  // the producer
  Producer *producer = new Producer( q , lower_bound , upper_bound );

  // the consumers
  std::vector<Consumer *> consumers;
  for( int i = lower_bound ; i <= upper_bound ; i++ )
    consumers.push_back( new Consumer(i) );
  
  // the distributor
  Distributor *distributor = new Distributor( q , consumers , algo );

  // create the consumer thread
  pthread_t consumer_thread ;
  pthread_create( &consumer_thread , (pthread_attr_t *)NULL , run_function , (void *)distributor );

  // create the producer thread
  pthread_t producer_thread ;
  pthread_create( &producer_thread , (pthread_attr_t *)NULL , run_function , (void *)producer );

  // let it run for n seconds
  sleep(run_seconds);
  q.stop(true);


  pthread_join( producer_thread , (void **)NULL );
  pthread_join( consumer_thread , (void **)NULL );

  int sum = 0;
  for( size_t i = 0; i < consumers.size() ; i++ )
    sum += consumers[i]->cnt();
  
  double unprocessed = static_cast<double>(q.size());
  double processed = static_cast<double>(sum);

  double pct_unproc = unprocessed/(processed+unprocessed);

  if( verbose )
    {
      std::cerr << "run time = " << std::setw(3) << run_seconds
		<< " lb = " << std::setw(3) << lower_bound
		<< " ub = " << std::setw(3) << upper_bound
		<< " processed = " << std::setw(10) << sum
		<< " unprocessed = " << std::setw(10) << q.size()
		<< " %not processed = " << std::setw(6) << std::setprecision(3) << std::fixed << pct_unproc
		<< std::endl;
    }

  // clean up
  for( size_t i = 0 ; i < consumers.size() ; i++ ) 
    delete consumers[i];
  delete producer;
  delete distributor;

  return pct_unproc;

}


//---------------------------------------------------------------------
// Run Benchmark
//
//  run the producer consumer for a given distribution algorithm
//  (linear or hash) and print out the results of the 
//  range of number getting larger and time getting larger
//  and record the number of messages still left in the Q
//---------------------------------------------------------------------
void run_benchmark( std::ostream &stream , Distributor::Dist_Algo algo )
{
  std::vector<QStats> stats;
  const int seconds_lower = 1;
  const int seconds_upper = 21;
  const int seconds_step = 5;

  const int lower_bound = 1;
  const int lower_range = 1 ;
  const int upper_range = 101;
  const int range_step = 20;
  
  // run the producer consumer for various times and rangnes of numbers
  for( int seconds=seconds_lower; seconds <= seconds_upper ; seconds += seconds_step )
    for( int range=lower_range; range<=upper_range; range += range_step )
      {
	double b = producer_consumer(algo,seconds,lower_bound,lower_bound+range,false);
	stats.push_back( QStats( seconds, range , b ) );
      }

  // print header
  stream << "Q Backup for Distributor Algo " << ( algo == Distributor::Dist_Linear ? "Linear" : "Hash" ) << std::endl;
  stream << std::setw(8) << "   Range" ;
  for( int seconds=seconds_lower; seconds <= seconds_upper ; seconds += seconds_step )
    stream << " " << std::setw(7) << seconds;
  stream << std::endl;
  for( int range = lower_range ; range <= upper_range  ; range += range_step )
    {
      std::vector<QStats> tmpV;
      std::remove_copy_if( stats.begin() , stats.end() , std::back_inserter( tmpV ) , 
			   std::bind2nd( qstats_range_ne() , range ));
      if ( tmpV.empty() ) continue;
      stream << std::setw(8) << range;
      for( int i = 0 ; i < tmpV.size() ; i++ ) 
	stream << " " << std::setw(7) << std::setprecision(4) << std::fixed << tmpV[i].backup;
      stream << std::endl;
    }
}


//---------------------------------------------------------------------
// main
//---------------------------------------------------------------------
int main( int argc , char **argv )
{
  run_benchmark( std::cout , Distributor::Dist_Linear );
  run_benchmark( std::cout , Distributor::Dist_Hash );
  return (0);
}
