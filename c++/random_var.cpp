#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>

static void boost_uniform( const int trials , unsigned seed )
{
  // Generate numbers from a uniforma distribution using boost
  // Roll dice and keep track of the some of the dice
  // Print the resulting distribuiton for the trial

  boost::mt19937 random_num_gen( seed );
  boost::uniform_int<> urange(1,6);
  boost::variate_generator< boost::mt19937 , boost::uniform_int<> > dice( random_num_gen , urange );

  // create a map to store the count of the number of occurrences of the sum on the dist
  // first is the dice sum the second is the count;
  std::map<int,int> throw_cnt;
  for( int i = 2 ; i <=12 ; i++ ) throw_cnt[i]=0;

  for( int i = 0; i < trials ; i++ )
    {
      int d1 = dice() , d2 = dice() , s = d1 + d2;
      std::map<int,int>::iterator itr = throw_cnt.find(s);
      (*itr).second++;
    }

  double n = static_cast<double>(trials);
  std::cout << "Dice Throw : Trials = " << std::setprecision(1) << std::scientific << n << std::endl;
  for( std::map<int,int>::iterator itr = throw_cnt.begin() ; itr != throw_cnt.end() ; ++itr )
    {
      double p = static_cast<double>((*itr).second) / n;
      std::cout << std::setw(3) << (*itr).first << "," << std::setw(10) << std::setprecision(8) << std::fixed << p << std::endl;
    }
  std::cout << std::endl;
}

static void boost_normal( const int trials , unsigned seed , double mean , double variance )
{
  // Generate Normal Random Variables using boost libraties
  // Then assess the density at +/- 2 sigma and +/- 1 sigma 

  boost::mt19937 random_num_gen( seed );
  boost::normal_distribution<double> n_dist(mean,variance);
  boost::variate_generator< boost::mt19937 , boost::normal_distribution<double> > normal_rv( random_num_gen , n_dist );

  std::vector<double> dist;
  dist.reserve(trials);
  for( int i = 0 ; i < trials ; i++ )
    {
      dist.push_back( normal_rv() );
    }

  std::sort( dist.begin(), dist.end() );

  double s = sqrt(variance);

  if( dist.empty() ) return;
  std::vector<double>::iterator itr_2slb = std::lower_bound( dist.begin() , dist.end() , -2.0*s );
  std::vector<double>::iterator itr_2sub = std::lower_bound( dist.begin() , dist.end() , 2.0*s );

  std::vector<double>::iterator itr_1slb = std::lower_bound( dist.begin() , dist.end() , -1.0*s );
  std::vector<double>::iterator itr_1sub = std::lower_bound( dist.begin() , dist.end() , 1.0*s );

  double nobs_2sd = itr_2sub - itr_2slb;
  double nobs_1sd = itr_1sub - itr_1slb;
  double n = static_cast<double>( trials );
  std::cout << "Normal RV : ";
  std::cout << std::setw(8) << std::setprecision(1) << std::scientific << static_cast<double>(trials);
  std::cout << " : mean +/- 2sigma % = " << std::setw(10) << std::setprecision(6) << std::fixed << nobs_2sd/n;
  std::cout << " : mean +/- 1sigma % = " << std::setw(10) << std::setprecision(6) << std::fixed << nobs_1sd/n;
  std::cout << std::endl;
}

static void boost_number_gen()
{
  const unsigned  seed = 5489u;;
  boost_uniform(1e3,seed);
  boost_uniform(1e6,seed);
  boost_uniform(1e7,seed);

  const double mean= 0.0 , variance = 1.0 ;
  boost_normal( 1e2 , seed , mean , variance );
  boost_normal( 1e3 , seed , mean , variance );
  boost_normal( 1e6 , seed , mean , variance );
  boost_normal( 1e7 , seed , mean , variance );
}
