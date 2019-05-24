#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <map>

int main( int , char ** )
{
  int lb=10 , ub=20;
  int r = ub-lb+1;

  std::map<int,int> m;
  for( int i = lb ; i <= ub ; i++ ) m[i]=0;
  srand(123456);
  for( size_t i = 0; i < 100000 ; i++ )
    {
      int n = lb + ( rand() % r );
      std::map<int,int>::iterator itr = m.find(n);
      (*itr).second++;
    }

  for( std::map<int,int>::iterator itr = m.begin() ; itr != m.end() ; ++itr )
    std::cout << (*itr).first << " " << (*itr).second << std::endl;
}
