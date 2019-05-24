#ifndef _calc_RegressionStats_h
#define _calc_RegressionStats_h

#include <armadillo>

namespace calc
{
  template<typename T>
  class RegressionStats
  {
  public:
    RegressionStats()
      :m(0),n(0),df(0),ess(0),tss(0),rss(0),mean_y(0),r_squared(0),adj_r_squared(0),F(0),s_squared(0) {}

  public:
 
    T m   ; // observations
    T n   ; // features
    T df  ; // degress of freedom
    T ess ; // Error Sum of Squares       sum( y - y_est ) ^2
    T tss ; // Total Sum of Squares       sum( y - mean(y) ) ^2
    T rss ; // Regression Sum of Squares  sum( y_est - mean(y) ) ^2
    T mean_y; // Average of dependent var
    T r_squared ; // 1 - ess/tss;
    T adj_r_squared ;// 1 - ( 1- r_squared) * (m-1)/(m-n-1);
    T F; // rss/(n-1)/ess/(m-n)
    T s_squared; // (m-n-1)/(m)*sum(y-mean()^2 
    arma::Mat<T> se; //standard errors of the estimates
    
  };
}

#endif
