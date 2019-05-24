#ifndef _calc_Random_h
#define _calc_Random_h

#include <boost/shared_ptr.hpp>

namespace calc
{
  // Uniform
  // Normal
  // LogNormal
  // Binomial

  // Generate numbers from a uniform distribution
  class UniformGen
  {
  public:
    UniformGen( unsigned seed , int lower , int upper ) ;

  public:
    unsigned get_seed()  const { return seed_ ; }
    unsigned get_lower() const { return lower_ ; }
    unsigned get_upper() const { return upper_ ; }

  public:

    int operator()() ;
    

  private:
    unsigned seed_;
    int      lower_;
    int      upper_;

  private:
    class Impl;
    boost::shared_ptr<Impl> impl_;
  };



  // Generate numbers from a normal distribution with mean - mu and std deviation sigma
  class NormalGen
  {
  public:
    // seed , mean and std deviatgion
    NormalGen( unsigned seed , double mu , double sigma );

  public:
    unsigned get_seed() const { return seed_ ; }
    double   get_mu() const { return mu_ ; }
    double   get_sigma() const { return sigma_ ; }

  public:

    double operator()() ;
    
  private:
    unsigned seed_;
    double   mu_;
    double   sigma_;

  private:
    class Impl;
    boost::shared_ptr<Impl> impl_;
  };


  // Generate numbers from a log normal distribution with mean - mu and std deviation sigma
  class LogNormalGen
  {
  public:
    // seed , mean and std deviatgion
    LogNormalGen( unsigned seed , double mu , double sigma );

  public:
    unsigned get_seed() const { return seed_ ; }
    double   get_mu() const { return mu_ ; }
    double   get_sigma() const { return sigma_ ; }

  public:

    double operator()() ;
    
  private:
    unsigned seed_;
    double   mu_;
    double   sigma_;

  private:
    class Impl;
    boost::shared_ptr<Impl> impl_;
  };

}


#endif
