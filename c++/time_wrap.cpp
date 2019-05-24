#include <time.h>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace mycode
{
  class Time
  {
  public:
    enum Zone { Zone_LOCAL , Zone_GMT };
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
    
    long sec() const
    { return t_.tv_sec ; }
    
    long nsec() const
    { return t_.tv_nsec; }

    // conversion to tm struct and put either in local time(default) or gmt
    void convert_to_tm( struct tm &t, Zone zone=Zone_LOCAL) const
    {
      if ( Zone_LOCAL == zone )
	localtime_r( &t_.tv_sec , &t);
      else
	gmtime_r(  &t_.tv_sec , &t);
    }

    // human reabable
    int day( Zone zone=Zone_LOCAL ) const
    { struct tm t; convert_to_tm( t , zone); return t.tm_mday; }

    int month( Zone zone=Zone_LOCAL ) const
    { struct tm t; convert_to_tm( t , zone); return t.tm_mon + 1; }

    int year( Zone zone=Zone_LOCAL ) const
    { struct tm t; convert_to_tm( t , zone); return 1900 + t.tm_year; }

    int hour( Zone zone=Zone_LOCAL ) const
    { struct tm t; convert_to_tm( t , zone); return t.tm_hour; }

    int minute( Zone zone=Zone_LOCAL ) const
    { struct tm t; convert_to_tm( t , zone); return t.tm_min; }

    int second( Zone zone=Zone_LOCAL ) const
    { struct tm t; convert_to_tm( t , zone); return t.tm_sec; }

    int micro_seconds( Zone zone=Zone_LOCAL ) const
    { return nsec() / ticks_per_us(); }

    std::string date_as_string( Zone zone=Zone_LOCAL ) const
    { 
      struct tm t; convert_to_tm( t , zone);
      std::stringstream s;
      s << std::setw(2) 
	<< std::setfill( '0')
	<< t.tm_mon + 1
	<< "/"
	<< std::setw(2) 
	<< std::setfill( '0')
	<< t.tm_mday
	<< "/"
	<< std::setw(4) 
	<< 1900 + t.tm_year;
      return s.str();
    }

    std::string time_as_string( Zone zone=Zone_LOCAL ) const
    { 
      struct tm t; convert_to_tm( t , zone);
      std::stringstream s;
      s << std::setw(2) 
	<< std::setfill( '0')
	<< t.tm_hour
	<< ":"
	<< std::setw(2) 
	<< std::setfill( '0')
	<< t.tm_min
	<< ":"
	<< std::setw(2) 
	<< std::setfill( '0')
	<< t.tm_sec
	<< std::setfill(' ' )
	<< "."
	<< std::setw(6)
	<< std::setfill('0')
	<< micro_seconds();
      return s.str();
    }

  private:
    struct timespec t_;
  };
}


using namespace mycode;

int main( int argc ,char **argv )
{
  Time time = Time::now();
  std::cout << "LOCAL TIME="<< time.date_as_string() << " " << time.time_as_string() << std::endl;
  std::cout << "GMT   TIME="<<time.date_as_string(Time::Zone_GMT) << " " << time.time_as_string(Time::Zone_GMT) << std::endl;
}
