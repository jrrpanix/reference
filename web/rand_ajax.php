<?php
print "<pre>";
$vxc=1000.0;
$mean = 100.0;
$d_size = 10.0;
for( $x=1 ; $x <= 30 ; $x++ ) 
{
	$ticks = ($vcx-$mean)/$d_size;
	$rv = rand(-100,100);
	$d = $rv > $ticks ? 1.0 : -1.0; 
	$vxc = $vxc + $d_size * $d;
	print "$vxc<br/>";
}
print "</pre>";


?>
