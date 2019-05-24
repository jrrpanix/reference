<pre>
$price=100.0;
$mean = 100.0;
$tick_size = 0.01;
for( $x=1 ; $x <= 30 ; $x++ ) 
{
	$ticks = ($price-$mean)/$tick_size;
	$rv = rand(-100,100);
	$d = $rv > $ticks ? 1.0 : -1.0; 
	$price = $price + $tick_size * $d;
	print "$price<br/>";
}

</pre>
