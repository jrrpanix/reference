#ifndef calc_KmeansCluster_h
#define calc_KmeansCluster_h

#include <armadillo>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <vector>

namespace calc
{
  // implements the KmeansCluster clustering Algorithm ...
  template<typename T>
  class KmeansCluster
  {

  public:

    void run( const arma::Mat<T> &X , int K , int max_iter );

    T cost_function();

    // KMenas clustring algorithm can be called using separate functions or 
    // as an object
  public:

    //  normalize matrix via column to (x - mean)/s
    static void normalize( const arma::Mat<T> &X , arma::Mat<T> &NormM );

    // X is an mxn matrix , will return a K x n matrix
    static void init_centroids( arma::Mat<T> &centroids , int K_rows , int cols  );

    // for each observation find the closest centroid ...
    static void find_closest_centroids( const arma::Mat<T> &X ,const arma::Mat<T> &centroids ,
					std::vector<int> &index );

    // calculate the new value of the centriods after finding the initial values
    static void compute_centroids( const arma::Mat<T> &X , const std::vector<int> &indx , int K , arma::Mat<T> &centroids );

    static T cost_function( const arma::Mat<T> &X , const std::vector<int> &indx , arma::Mat<T> &centroids );


  public:
    int              K_;
    std::vector<int> index_;
    arma::Mat<T>     X_norm_;
    arma::Mat<T>     centroids_;

  };

  #include "KmeansCluster.tcc"
}


#endif
