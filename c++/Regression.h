#ifndef _calc_Regression_h
#define _calc_Regression_h

#include <sstream>
#include <armadillo>
#include "CalcException.h"
#include "RegressionStats.h"

namespace calc
{
  template<typename T>
  class Regression
  {
  public:
 
    //---------------------------------------------------------------------
    // Normal Equation No Regularzation
    // Solution to J(theta) = min    1/2 * sum( X(i)(theta) - y(i))^2
    //                        theta
    //---------------------------------------------------------------------
    static void solve( const arma::Mat<T> & X ,  const arma::Mat<T> &y , arma::Mat<T> &theta )
    {
      if ( X.n_rows != y.n_rows )
	{
	  std::stringstream strm;
	  strm << "Regression::solve incompatible Matrix Dimensions X = " << X.n_rows
	       << "x" << X.n_cols << " y=" << y.n_rows << "x" << y.n_cols;
	  throw CalcException( strm.str() );
	} 
      arma::Mat<T> xT  = X.t();
      arma::Mat<T> xTx = xT * X;
      theta = xTx.i() * xT * y ;
    }

    //---------------------------------------------------------------------
    // Normal Equation W/ Regularzation
    // Solution to J(theta) = min  1/2 * sum( X(i)(theta) - y(i))^2 + lambda sum( theta^2)
    //                                   i=1:m                         j=1:n
    //
    // where m is the number of observations and n is the number of featrues
    //---------------------------------------------------------------------
    static void solve_reg( const arma::Mat<T> & X ,  const arma::Mat<T> &y , arma::Mat<T> &theta , T lambda )
    {
      if ( X.n_rows != y.n_rows )
	{
	  std::stringstream strm;
	  strm << "Regression::solve_reg incompatible Matrix Dimensions X = " << X.n_rows
	       << "x" << X.n_cols << " y=" << y.n_rows << "x" << y.n_cols;
	  throw CalcException( strm.str() );
	} 
      // (X'X + Lambda*Diag() w/ 0 in (0,0) )inv * (X'y)

      int nfeatures = X.n_cols;
      arma::Mat<T> xT = X.t();
      arma::Mat<T> xTx= xT * X;
      arma::Mat<T> lambdaM( nfeatures , nfeatures );
      lambdaM.zeros();

      // set diagonals equal to lamba except 0th
      for( int i = 1 ; i < nfeatures ; i++) 
	lambdaM(i,i)=lambda;
      xTx += lambdaM;
      theta = xTx.i() * xT * y ;
    }
   
    //---------------------------------------------------------------------
    // statistics given a feature set X and an estimate theta and observations y
    //
    // then
    // 
    // y_est = X*theta
    //
    // Total Sum of Squares       TSS = sum( y - mean(y) ) ^ 2
    // Regression Sum of Squares  RSS = sum( y_est - mean(y) ) ^ 2
    // Error Sum of Squares       ESS = sum( y - y_est ) ^2
    //
    // R-squared (coefficent of determination ) = 1 - ESS/TSS
    //
    // m = number of observations , n = number of features
    //
    // Adj R-squared = 1 - ( 1 - R-squared) * (m - 1)/(m-n-1)
    //
    //---------------------------------------------------------------------
    static RegressionStats<T> R_squared( const arma::Mat<T> &X , const arma::Mat<T> &y , const arma::Mat<T> &theta )
    {
      RegressionStats<T> stats;
      stats.m = X.n_rows;
      stats.n = X.n_cols-1;
      stats.df = X.n_rows - X.n_cols;
      stats.mean_y = arma::accu( y ) / stats.m;
      arma::Mat<T> y_est = X*theta;
      arma::Mat<T> y_err = y - X*theta;
      stats.ess = arma::accu( arma::square( y_err ));
      stats.tss = arma::accu( arma::square( y - stats.mean_y ) );
      stats.rss = arma::accu( arma::square( y_est - stats.mean_y ));
      stats.r_squared = 1 - stats.ess/stats.tss;
      stats.adj_r_squared = 1 - (( 1 - stats.r_squared )*(stats.m-1)/(stats.df));
      stats.F = (stats.rss / stats.n) / ( stats.ess/stats.df );
      stats.s_squared = stats.ess / stats.df ;
      
      arma::Mat<T> xx = (X.t() * X).i();
      stats.se.reshape( X.n_cols , 1 );
      for( int i = 0; i < X.n_cols ; i++ )
	stats.se(i,0) = sqrt( stats.s_squared *  xx(i,i) );
      

      return stats;
    }
  };

}

#endif 
