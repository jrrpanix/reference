
//------------------------------------------------------------------------------------------
// to compile
// 
// g++ -O4 mean_reversion.cpp -omean_rev 
//
// this uses boost_1_47_0
// if boost is installed in /usr/include/boost_1_47_0
// with boost -> boost_1_47_0/boost the boost includes will be automatically picked up by g++
//------------------------------------------------------------------------------------------

#ifdef _USING_BOOST_
#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#endif

#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <math.h>

//------------------------------------------------------------------------------------------
// calcualte the mean, stdev, min and max
//------------------------------------------------------------------------------------------
void calc_stats( const std::vector<double> &v , double &mean, double &min_v , double &max_v , double &stdev)
{
  if( v.empty() )
    {
      mean=min_v=max_v=stdev=0;
      return;
    }
  mean = std::accumulate( v.begin() , v.end() , 0.0 ) / static_cast<double>(v.size());
  std::vector<double>::const_iterator itr;
  itr = std::min_element( v.begin() , v.end() );
  min_v = (*itr);
  itr = std::max_element( v.begin() , v.end() );
  max_v = (*itr);

  double sum = 0.0;
  for( size_t i = 0; i < v.size() ; i++ )
    {
      double d = v[i] - mean;
      d *= d;
      sum += d;
    }
  stdev = v.size() == 1 ? 0.0 : sqrt( sum / static_cast<double>(v.size()-1) );
}

//------------------------------------------------------------------------------------------
// print mean , min , max, range and stdev of randomly generated prices
//------------------------------------------------------------------------------------------
void print_stats( std::ostream &stream , const std::vector<double> &v , int draws , int tick_upper , int drift )
{
  double avg_mr , min_mr , max_mr , stdev_mr;
  calc_stats( v , avg_mr , min_mr , max_mr , stdev_mr );

  stream << std::setw(15)<< "mean reverting"
	 << " "
	 << std::setw(6) << draws
	 << " "
	 << std::setw(6) << tick_upper
	 << " "
	 << std::setw(6) << drift
	 << " "
	 << std::setw(10) << std::setprecision(2) << std::fixed << avg_mr
	 << " "
	 << std::setw(10) << std::setprecision(2) << std::fixed << min_mr
	 << " "
	 << std::setw(10) << std::setprecision(2) << std::fixed << max_mr
	 << " " 
	 << std::setw(10) << std::setprecision(2) << std::fixed << (max_mr-min_mr)
	 << " " 
	 << std::setw(10) << std::setprecision(2) << std::fixed << stdev_mr
	 << std::endl;
}

//------------------------------------------------------------------------------------------
// generate a uniform distribution of number in the range from lb to ub
//------------------------------------------------------------------------------------------
void gen_uniform( int lb , int ub , unsigned seed , int trials, std::vector<int> &rv )
{
  #ifdef _USING_BOOST
  boost::mt19937 random_num_gen( seed );
  boost::uniform_int<> urange(lb,ub);
  boost::variate_generator< boost::mt19937 , boost::uniform_int<> > uv( random_num_gen , urange );
  for( int i = 0 ; i < trials ; i++ ) rv.push_back( uv() );
  #else
  srand( seed );
  int range = ub - lb + 1;
  for( int i = 0 ; i < trials ; i++ ) rv.push_back( lb + ( rand() % range) );
  #endif
}
		  
//------------------------------------------------------------------------------------------
// generate prices according to a random walk
// this is highly unstable and for illistrative pruposes only, can't be used 
// as releastic source of mid price changes
//------------------------------------------------------------------------------------------
void random_walk( double start ,
		  double tick_size,
		  int draws , 
		  std::vector<double> &px )
{
  const unsigned  seed = 54897u;
  std::vector<int> rv;
  gen_uniform( -1, 1, seed , draws , rv );
  px.clear();
  px.push_back(start);
  px.reserve(draws);
  for( int i = 0; i < draws ; i++ )
    {
      double d = rv[i] == 0 ? 0 : ( rv[i] < 0 ? -1.0 : 1.0 ) ;
      px.push_back( px.back() + d * tick_size );
    }
}

//------------------------------------------------------------------------------------------
// the function generates random prices that have the following contraints:
// the sample of tick movements is constrained to -tick_upper to tick_upper+1
// the price chainge is biased towards the mean but allowed to drift away by drift ticks
// for drift
//------------------------------------------------------------------------------------------
void mean_reversion( double start ,
		     double tick_size ,
		     double mean ,
		     int    tick_upper ,
		     int    drift,
		     int    draws , 
		     std::vector<double> &px )
{
  const unsigned  seed = 54897u;
  std::vector<int> rv;
  gen_uniform( -tick_upper, tick_upper+1, seed , draws , rv );
  px.clear();
  px.push_back(start);
  px.reserve(draws);
  for( int i = 0; i < draws ; i++ )
    {
      int ticks = static_cast<int>((px.back() - mean) / tick_size);
      double delta = 0.0;
      if ( ( ticks < 0 && ticks + drift >= 0 ) ||
	   ( ticks > 0 && ticks - drift <= 0 ) )
	delta = rv[i] > 0 ? 1.0 : -1.0;
      else
	delta = rv[i] > ticks ? 1.0 : -1.0;
      px.push_back( px.back() + tick_size * delta );
    }

}

//---------------------------------------------------------------
// generate mean reverting prices for various tick ranges
// allowing for slight drift. the higher the drift the more
// variation there will be in prices, the higher the ticks the more
// variation there will be in prices
//---------------------------------------------------------------
int main( int , char ** )
{
  const double price      = 100.0;
  const double mean_price = 100.0;
  const double tick_size  = 0.01;
  const int    draws      = 100;
  const int    drift_div  = 2;

  for( int ticks =1000 ; ticks <= 1000 ; ticks += 100 )
    {
      std::vector<double> px;
      int drift = ticks / drift_div;
      mean_reversion( price , tick_size , mean_price , ticks , drift , draws , px );
      //print_stats(std::cout,px,draws,ticks,drift);
      for( size_t i = 0 ; i < px.size() ; i++ )
	std::cout << std::setw(10) << std::setprecision(3) << std::fixed << px[i] << std::endl;
    }
}

