
#ifndef calc_KmeansCluster_cpp
#define calc_KmeansCluster_cpp


template<typename T>
inline void calc::KmeansCluster<T>::normalize( const arma::Mat<T> &X , arma::Mat<T> &X_norm )
{
  int cols = X.n_cols;
  X_norm  = X;
  for( int j = 0; j < cols ; j++ )
    {
      T s = arma::stddev(X_norm.col(j));
      T mu = arma::mean(X_norm.col(j));
      
      X_norm.col(j) -= mu;
      X_norm.col(j) /= s;
    }
}


template<typename T>
inline void calc::KmeansCluster<T>::init_centroids( arma::Mat<T> &centroids , int K , int cols )
{
  centroids = arma::randn( K , cols );
  for( int i = 0 ;i < K/2 ; i++ ) centroids(i) *= 2;
}

// for each observation find the closest centroid ...
template<typename T>
inline void calc::KmeansCluster<T>::find_closest_centroids( const arma::Mat<T> &X ,const arma::Mat<T> &centroids , 
							    std::vector<int> &index)
{
  int rows = X.n_rows , cols = X.n_cols;

  index.resize(rows);

  int K = centroids.n_rows;
  for( int i = 0; i < rows ; i++ )
    {
      T min_cx = 0;
      int min_idx = -1;

      for( int k = 0 ; k < K ; k++)
	{
	  T sum =0;
	  for( int ll = 0; ll < cols ; ll++ )
	    {
	      T s = X(i,ll) - centroids(k,ll);
	      sum += s*s;
	    }
	  if ( k == 0 || sum < min_cx )
	    {
	      min_cx = sum;
	      min_idx = k;
	    }
	}
      index[i] = min_idx;
    }
}


template<typename T>
void calc::KmeansCluster<T>::compute_centroids( const arma::Mat<T> &X , const std::vector<int> &indx , int K , arma::Mat<T> &centroids )
{
  std::vector<int> n_in_cluster( K , 0 );

  int rows = X.n_rows, cols = X.n_cols ;
  centroids.resize( K , cols );
  centroids.fill( 0 );

  for( int i = 0 ; i < rows ; i++ )
    {
      int centriod_row_i = indx[i];
      n_in_cluster[centriod_row_i] += 1;
      for( int j = 0; j < cols ; j++ )
	centroids(centriod_row_i , j ) += X(i,j);
    }

  for( int i = 0 ;i < K ; i++ )
    {
      T d = (T)n_in_cluster[i];
      if ( d != 0 )
	for( int j = 0 ; j < cols ; j++ )
	  centroids(i,j) /= d;
    }

}

template<typename T>
T calc::KmeansCluster<T>::cost_function( const arma::Mat<T> &X , const std::vector<int> &indx , arma::Mat<T> &centroids )
{
  T J = 0;
  int m = X.n_rows , n = X.n_cols;
  for( int i = 0; i < m ; i++ )
    {
      int c_i = indx[i];
      T d = 0;
      for( int j = 0; j < n ;j++)
	{
	  d = X(i,j) - centroids(c_i,j);
	  d *= d;
	}
      J += d;
    }
  return J;
}
template< typename T>
inline void calc::KmeansCluster<T>::run( const arma::Mat<T> &X , int K , int max_iter )
{
  K_=K;
  normalize( X , X_norm_ );
  init_centroids( centroids_ , K_ , X_norm_.n_cols);
  T J = 0;
  int itr = 0;
  while ( itr++ < max_iter )
    {
      find_closest_centroids( X_norm_ , centroids_ , index_ );
      T J_i = cost_function( X_norm_ , index_, centroids_ );
      compute_centroids( X_norm_ , index_, K_ , centroids_ );
    }

}
template<typename T>
T calc::KmeansCluster<T>::cost_function()
{
  return cost_function( X_norm_ , index_ , centroids_ );
}

#endif
