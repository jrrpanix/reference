
#include <math.h>
#include <iostream>
#include <numeric>
#include <iomanip>
#include <sstream>

// permutation
//
//   P(n,r) =  n! / ( n - r)! 
//
// permutations are distinct orderings 
//
// for example , permutations of 2 Letters From A B C 
//
// P(3,2) = 3! / 1! =  3 * 2 = 6
// P(ABC,2) = { AB , AC , BC , BA , CA , CB };
// 
// Combinations do not take into account ordering 
//
// C(n,r) = n! / ( r! * ( n-r)! )
//
// for example , combinations of 2 Letters From A B C 
// C(3,2) = 3! / (2! * 1! ) = 3
// 
//
//


typedef unsigned long long card_t;


//////////////////////////////////////////////////////////////////////
// Permutation
// P(n,r) = n!/(n-r)!
//////////////////////////////////////////////////////////////////////
card_t perm( card_t n  , card_t r )
{
  if ( r > n ) return 1;
  card_t p = 1;
  card_t end = n - r + 1;
  while ( n >= end )
    p *= n--;
  return p;
}

//////////////////////////////////////////////////////////////////////
// Combination C(n,r) 
// C(n,r) = n!/*r!(n-r)!
//////////////////////////////////////////////////////////////////////
card_t combo( card_t n  , card_t r )
{
  card_t nMr = n - r ;
  card_t f1 = std::max( nMr , r );
  card_t f2 = std::min( nMr , r );

  card_t p1 = 1;
  while( n > f1 )
    p1 *= n--;
  card_t p2 = 1;
  while( f2 > 1 )
    p2 *= f2--;

  return p1 / p2;
}

//////////////////////////////////////////////////////////////////////
// calculate probablity
//////////////////////////////////////////////////////////////////////
double prob( card_t x , card_t n )
{
  return static_cast<double>(x) / static_cast<double>(n);
}

//////////////////////////////////////////////////////////////////////
// print utilities
//////////////////////////////////////////////////////////////////////
void print( const char *str , card_t n , double p )
{
  std::cout << "N(" << std::setw(20) << str << ")= " << std::setw(12) 
	    << n << ";p(" << std::setw(20) << str << ") = " 
	    << std::setw(10) << std::setprecision(7) << std::fixed << p << std::endl;
}

void pp( const char *str , card_t r , card_t hands )
{
  std::cout << std::setw(20) << str << " " << std::setw(14) << r << " " << std::setw(14) << hands 
	    << " " 
	    << std::setw(12) << std::setprecision(8) << std::fixed << (double)r/double(hands) 
	    << std::endl;
}

//////////////////////////////////////////////////////////////////////
// 5 card probablities
//////////////////////////////////////////////////////////////////////
void poker_5_card_prob()
{
  card_t hands = combo(52,5);
  card_t _rflush = combo(4,1);
  card_t _sflush = combo(4,1)*combo(10,1)-_rflush;
  card_t _pair = 13*combo(4,2)*combo(12,3)*4*4*4;
  card_t _2pair = combo(13,2)*combo(4,2)*combo(4,2)*11*4;
  card_t _3kind = (13*combo(4,3))*combo(12,2)*4*4;
  card_t _straight = combo(10,1)*4*4*4*4*4 - ( _sflush + _rflush );
  card_t _flush = combo(4,1)*combo(13,5) - _sflush - _rflush;
  card_t _4kind = (13)*12*4;
  card_t _fullh = (13*combo(4,2))*12*combo(4,3);
  card_t _nopair = hands - (_pair+_2pair+_3kind+_straight+_flush+_4kind+_fullh+_rflush+_sflush);

  std::cout << "Hand Probabilities 5 - card poker" << std::endl;
  pp( "high card" , _nopair , hands );
  pp( "pair" , _pair , hands );
  pp( "2-pair" , _2pair , hands );
  pp( "3 of a kind" , _3kind , hands );
  pp( "straight" , _straight , hands );
  pp( "flush" , _flush , hands );
  pp( "full house" , _fullh , hands );
  pp( "4 of a kind" , _4kind , hands );
  pp( "straight flush" , _sflush , hands );
  pp( "royal flush" , _rflush , hands );


}

void poker_7_card_prob()
{
  // Number of Hands
  card_t hands = combo(52,7);

  // Royal Flush
  card_t _rflush = combo(4,1)*combo(47,2);

  // Straight Flush
  card_t _sflush = (combo(4,1)*combo(9,1)*combo(46,2));

  // 4 of a Kind
  card_t _4kind = (13)*combo(48,3);

  // Full House
  // 3 one , 3 of another , 1 extra card 
  // 3 one , 2 of another , 2 of another
  // 3 one , 2 of another , 2 extra
  card_t f1 = combo(13,2)*combo(4,3)*combo(4,3)*(52-8);
  card_t f2 = (13*combo(4,3))*combo(12,2)*combo(4,2)*combo(4,2);
  card_t f3 = (13*combo(4,2))*(12*combo(4,3))*(combo(11,2)*4*4);
  card_t _fullh = f1+f2+f3;

  // Flush
  //  7 cards same suit
  //  6 cards same 1 different
  //  5 cards same 2 different
  // total flush = 7 card flush + 6 card flush + 5 card flush - straight flush
  //
  card_t num_7_flush = combo(4,1)*combo(13,7);
  card_t num_6_flush = combo(4,1)*combo(13,6)*combo(39,1);
  card_t num_5_flush = combo(4,1)*combo(13,5)*combo(39,2);
  card_t _flush = num_7_flush + num_6_flush + num_5_flush - _rflush - _sflush ;

  // Straight
  // lots of enumerations with 7 cards
  // 7 distinct card values
  //
  card_t str_7_ace_high    = combo(8,2); // number of ways to from a ACE high straight with 7 cards
  card_t str_7_no_ace_high = combo(7,2)*combo(9,1); // number of ways to form remianing 9 straights with 7 cards
  card_t str7_ranks = str_7_ace_high + str_7_no_ace_high;
  card_t str_7_suit_combos = pow(4,7);
  card_t str_7_flush = combo(4,1) + combo(4,1)*combo(7,6)*combo(3,1) + combo(4,1)*combo(7,5)*combo(3,1)*combo(3,1);
  str_7_suit_combos -= str_7_flush;
  card_t str_7 = str7_ranks * str_7_suit_combos;

  // 6 distinct cards 1 extra
  card_t str_6_ranks = combo(8,1) + combo(7,1)*combo(9,1);
  card_t str_6_pair = combo(6,1)*combo(4,2);
  card_t str_6_suit = pow(4,5);
  card_t str_6_flush = combo(4,1) + combo(5,4)*combo(2,1)*combo(3,1);
  str_6_suit -= str_6_flush;
  card_t str_6 = str_6_ranks*str_6_pair*str_6_suit;


  //5 cards 2 extra
  card_t str_5_ranks = combo(5,1)*combo(4,3);
  card_t str_5_suits = pow(4,4)-combo(3,1);
  card_t str_5_triple = 10 *  str_5_ranks * str_5_suits;
  card_t str_5 = (6*(64-combo(2,1))+24*(64-1) + 6*64)*10*10 + str_5_triple;

  card_t _straight = str_7 + str_6 + str_5;

  // 3 of a kind
  // will contain 5 different value cards example 3C 3D 3H 2H AD 10C 7S
  // it can't contain a straight or flush
  card_t _3kind = (combo(13,5)-10)*combo(4,3)*combo(5,1)*(combo(4,1)*combo(4,1)*combo(4,1)*combo(4,1)-combo(3,1));

  // 2Pairs
  // 3-Pairs 1 Kicker
  // 2-Pairs 3 Kickers
  card_t thr_pr_1 = combo(13,4)*combo(4,3)*combo(4,2)*combo(4,2)*combo(4,2)*4;
  card_t two_pr_3 = (combo(13,5)-10)*combo(5,2)*2268;
  card_t _2pair = thr_pr_1 +  two_pr_3;

  // 1 Pair
  card_t _pair = (combo(13,6)-9-(2*combo(7,1)+8*combo(6,1)))*(pow(4,5)-34)*(6*combo(4,2));

  // High Card
  card_t _nopair = hands - (_pair+_2pair+_3kind+_straight+_flush+_4kind+_fullh+_rflush+_sflush);

  std::cout << "Hand Probabilities 7 - card poker" << std::endl;
  pp( "high card" , _nopair , hands );
  pp( "pair" , _pair , hands );
  pp( "2-pair" , _2pair , hands );
  pp( "3 of a kind" , _3kind , hands );
  pp( "straight" , _straight , hands );
  pp( "flush" , _flush , hands );
  pp( "full house" , _fullh , hands );
  pp( "4 of a kind" , _4kind , hands );
  pp( "straight flush" , _sflush , hands );
  pp( "royal flush" , _rflush , hands );


}


int main( int argc , char **argv )
{
  poker_5_card_prob();
  poker_7_card_prob();
}
