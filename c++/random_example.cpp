
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <map>
#include <iostream>
#include <iomanip>
#include <numeric>

static void sample_mean_stdev( const std::vector<double> &v , double &mean , double &stdev )
{
  mean=stdev=0.0;
  if ( v.empty() ) return;
  mean = std::accumulate( v.begin() , v.end() , 0.0 ) / static_cast<double>(v.size());
  for( size_t i = 0 ; i < v.size() ; i++ )
    {
      double d = v[i] - mean;
      stdev += d*d;
    }
  stdev = v.size() == 1 ? 0 : sqrt(stdev) / ( static_cast<double>(v.size()) - 1.0 );
}

static inline int roll_dice()
{
  return (rand() % 6) + 1;
}

static double dice_trial( int sample_size )
{
  if ( sample_size == 0) return 0.0;
  double mean_roll = 0.0;
  for (size_t i = 0 ; i < sample_size ; i++ )
    {
      int d1 = roll_dice() , d2 = roll_dice() ;
      mean_roll += d1 + d2;
    }
  return mean_roll / static_cast<double>(sample_size);
}

static void monte_carlo_dice_roll( int samples , int trials , std::vector<double> &sample_means )
{
  sample_means.clear();
  std::map<int,int> sum_cnt;
  for( int i = 0; i < samples ; i++ )
    {
      sample_means.push_back( dice_trial( trials ));
    }
}

void init_random()
{
  srand( time(NULL) );
}

void random_example()
{
  //
  //  sample 100 times the experiment of rolling a dice n times and calculating the mean 
  //  roll. calcualte the mean of the samples and the std deviation of the mean as n gets larger
  //  as the number of trials(rolls) gets larget the std dev of the means will get smaller
  init_random();
  const int samples = 100;
  const int max_trials = 1e7;
  for( int trials = 1; trials <= max_trials ; trials *= 10 )
    {
      std::vector<double> sample_means;
      monte_carlo_dice_roll( samples , trials , sample_means );
      double sample_mean = 0 , sample_dev = 0.0 ;
      sample_mean_stdev( sample_means , sample_mean , sample_dev ); 
      
      std::cout << "sample mean = " << std::setprecision(7) << std::fixed << sample_mean << ":"
		<< " stdev of means = " << std::setprecision(7) << std::fixed << sample_dev << ":"
		<< " dice rolls = " << std::setprecision(1) << std::scientific << (double)trials << ":"
		<< " samples = " << samples 
		<< std::endl;
    }
}
