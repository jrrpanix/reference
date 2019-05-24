<pre>
<!-- uncomment this line when inserting into html code <?php -->
//---------------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------------
$G_Clubs    = 1;
$G_Diamonds = 2;
$G_Hearts   = 3;
$G_Spades   = 4;

$G_Clubs_Str    = "Clubs";
$G_Diamonds_Str = "Diamonds";
$G_Hearts_Str   = "Hearts";
$G_Spades       = "Spades";


//---------------------------------------------------------------------------------
// function demonstrating switch statement, that uses Globally Defined Constants
//---------------------------------------------------------------------------------
function suit_as_string( $suit )
{
   //
   // Tell function which variables are local 
   global $G_Clubs;
   global $G_Diamonds ;
   global $G_Hearts;
   global $G_Spades   ;

   global $G_Clubs_Str   ;
   global $G_Diamonds_Str;
   global $G_Hearts_Str  ;
   global $G_Spades_Str  ;

   switch( $suit )
   {
   case $G_Clubs : return $G_Clubs_Str;
   case $G_Diamonds : return $G_Diamonds_Str;
   case $G_Hearts : return $G_Hearts_Sttr;
   case $G_Spades : return $G_Spades_Str;
   }
}

//---------------------------------------------------------------------------------
// iterate through an array an concatenate elements together in a string
//---------------------------------------------------------------------------------
function array_to_str( $V )
{
   $str;
   $cnt=0;
   foreach( $V as $i )
   {
     if ( $cnt++ != 0 )
       $str .= ",";
     $str .= $i;
   }
   return $str;
}

//---------------------------------------------------------------------------------
// factorial
//---------------------------------------------------------------------------------
function factorial( $x )
{
   $f =1 ;
   for( $i=2 ; $i <= $x ; $i++ )
      $f *= $i;		     
   print "$x! = $f<br/>\n";
}

//---------------------------------------------------------------------------------
// php calls
//---------------------------------------------------------------------------------
print "Factorial Call </br>\n";
factorial( 5 );
print "</br>\n";

print "Array to String Call </br>\n";
$seq = array(1,2,3,4,5,6,7,8,9,10);
$seq_str = array_to_str($seq);
print "seq_str = $seq_str </br>\n";
print "</br>\n";

print "Suit to Stirng Call </br>\n";
$suit_str = suit_as_string( $G_Clubs);
print "suit = $suit_str </br>\n";
print "</br>\n";

print "String Replace Call </br>\n";
$str ="XXXNew York CityXXX";
print "Before Replace XXX str = $str </br>\n";
$str = str_replace( "X" , "" , $str);
print "After  Replace XXX str = $str </br>\n";
print "</br>\n";


<!-- uncomment this line when inserting into html code ?> -->
</pre>

