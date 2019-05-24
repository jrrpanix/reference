

#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/lognormal_distribution.hpp>
#include <boost/random/binomial_distribution.hpp>
#include <boost/shared_ptr.hpp>

#include "Random.h"

namespace calc
{

  class UniformGen::Impl
  {
  public:
    Impl( unsigned seed , int lower , int upper ) 
      :rgen_(seed),
       udist_(lower,upper),
       gen_(rgen_,udist_)
    {}

    int next() { return gen_(); }

  public:
    boost::mt19937       rgen_;
    boost::uniform_int<> udist_;
    boost::variate_generator< boost::mt19937 , boost::uniform_int<> > gen_;
  };


  class NormalGen::Impl
  {
  public:
    Impl( unsigned seed , double mu , double sigma )
      :rgen_(seed),
       ndist_(mu,sigma),
       gen_(rgen_,ndist_)
    {}

    double next() { return gen_(); }

  public:
    boost::mt19937       rgen_;
    boost::normal_distribution<double>  ndist_;
    boost::variate_generator< boost::mt19937 , boost::normal_distribution<double>  > gen_;
  };

  class LogNormalGen::Impl
  {
  public:
    Impl( unsigned seed , double mu , double sigma )
      :rgen_(seed),
       ndist_(mu,sigma),
       gen_(rgen_,ndist_)
    {}

    double next() { return gen_(); }

  public:
    boost::mt19937       rgen_;
    boost::lognormal_distribution<double>  ndist_;
    boost::variate_generator< boost::mt19937 , boost::lognormal_distribution<double>  > gen_;
  };


}

calc::UniformGen::UniformGen( unsigned seed , int lower , int upper )
  :seed_(seed),lower_(lower),upper_(upper),
   impl_( new Impl( seed , lower, upper ) )
{}

int calc::UniformGen::operator()() { return impl_->next(); }


calc::NormalGen::NormalGen( unsigned seed , double mu , double sigma )
  :seed_(seed),mu_(mu),sigma_(sigma),
   impl_( new Impl( seed , mu , sigma ) )
{}

double calc::NormalGen::operator()() { return impl_->next(); }

calc::LogNormalGen::LogNormalGen( unsigned seed , double mu , double sigma )
  :seed_(seed),mu_(mu),sigma_(sigma),
   impl_( new Impl( seed , mu , sigma ) )
{}

double calc::LogNormalGen::operator()() { return impl_->next(); }
