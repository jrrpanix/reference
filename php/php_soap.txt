<pre>
<!-- uncomment when inserting into html <?php -->

$client = new SoapClient( "http://footballpool.dataaccess.eu/data/info.wso?wsdl" );
$result_1 = $client->TopGoalScorers(array('iTopN'=>5));
$array_1 = $result_1->TopGoalScorersResult->tTopGoalScorer;

foreach( $array_1 as $k=>$v)
{
	print "$v->sName <br/>";
}

<!-- uncomment when inserting into html ?> -->
</pre>
