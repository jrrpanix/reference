#ifndef _calc_MLVar_h
#define _calc_MLVar_h

#include <string>
#include <armadillo>
#include <boost/shared_ptr.hpp>
#include <unordered_map>

namespace calc
{
  class MLVar
  {
  public:
    std::string        name;
    arma::Mat<double>  value;
  };

  


  typedef boost::shared_ptr<MLVar> MLVarPtr;
  typedef std::unordered_map<std::string,MLVarPtr> MLVarMap;
  typedef boost::shared_ptr<MLVarMap> MLVarMapPtr;

};



#endif
