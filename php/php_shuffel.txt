<pre>

<--! <?php ->
//------------------------------------------------
// Suit Values
//------------------------------------------------
$Clubs = 1;
$Diamonds = 2;
$Hearts = 3;
$Spades = 4;


//------------------------------------------------
// Card Class
//------------------------------------------------
class Card
{
    function Card( $f , $s=0 )
    {
       // if the suit is 0 then construct from Deck index
       if ( $s == 0 )
       {
            $s = floor($f/13);
            $f = $f - $s * 13;
            $this->face = $f + 2;
            $this->suit = $s + 1;
       }
       else
       {
	   $this->face = $f;
	   $this->suit = $s;
       }
    }


    function get_index()
    {
       $i = ($this->suit - 1) * 13 + $this->face - 2;
       return $i;	
    }

    function get_png_image_file()
    {
       global $Clubs;
       global $Diamonds;
       global $Hearts;
       global $Spades;

       $i=0;
       switch( $this->suit )
       {
         case $Clubs: $i = 1;
	 break;
         case $Diamonds: $i = 4;
	 break;
         case $Hearts: $i = 3;
	 break;
         case $Spades: $i = 2;
	 break;
       }
       $j = (14 - $this->face)*4;
       $n = $i + $j;
       $file = "../images/cards/" . $n . ".png";
       return $file;
    }

    var $face;
    var $suit;
}

//--------------------------------------------------------------------------------------
// class Deck
//--------------------------------------------------------------------------------------
class Deck
{
    var $deck_ ;
    var $index_;
    private function _init_deck()
    {
        $this->deck_ = array();
        $this->deck_ = array_fill(0,51,0);
	$this->index_=0;
	for( $i = 0 ; $i < 52 ; $i++)
	{
	   $this->deck_[$i]=$i;
        }
    }
   
    function Deck()
    {
	$this->_init_deck();
    }

    function next()
    {
       $j = $this->deck_[$this->index_];	
       $this->index_++;
       return $j;
    }

    function is_empty()
    {
      if ( $this->index_ >= 52 ) return 1;
      return 0;
    }

    function shuffel()
    {
       $i = 0;
       $l = count( $this->deck_) - 1;
       foreach( $this->deck_ as $ele )
       {
	  $ni = rand(0,$l);
	  $tmp = $this->deck_[$i];
          $this->deck_[$i]= $this->deck_[$ni];
	  $this->deck_[$ni]=$tmp;
	  $i++;
       }
    }
};


//--------------------------------------------------------------------------------------
// Test stuff
//--------------------------------------------------------------------------------------
function shuffel_test()
{
    $deck = new Deck();
    $deck->shuffel();
    $k = 0;
    print "<h2 align=\"center\"> Shuffel Deck</h2>\n";
    while( $deck->is_empty() == 0 )
    {
      $n = $deck->next();
      $card = new Card($n);
      if ( $k == 0 )
       {
	print "<p style=\"text-align:center\" >\n";
       }
      $file =$card->get_png_image_file();
      print "<img width=\"50\" height=\"75\" src=\"$file\">\n";
      $k++;
      if( $k == 13 )
      {
       print "</p><br/>";
       $k = 0;
      }
     if( $k > 70 )
       break;
    }
}


shuffel_test();

<--! ?> ->

</pre>
