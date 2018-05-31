
google.load("visualization", "1", {packages:["corechart"]});

function Point( date , inv )
{
    this.date = date;
    this.inv = inv;
}

function add_data( data_table , data )
{
    data_table.addColumn('date','Date');
    data_table.addColumn('number','Inventory');
    for( var i = 0; i < data.length ; i++ )
	{
	    data_table.addRows( [[data[i].date , data[i].inv]] );
	}
}

function create_data_set()
{
    var A = new Array(25);
    var m = 1 , d = 1 , y = 2012;
    var inv = 100;
    for( var i = 0; i < 25 ; i++ )
	{
	    A[i] = new Point( new Date( y , m , d ) , inv );
	    inv -=2;
	    d++;
	}
    return A;
}

function draw_graph()
{
    var data_table = new google.visualization.DataTable();
    var title_str = "Widget Inventory" ;
    var data = create_data_set();
    add_data( data_table , data );

    var options = {
	width: 500, height: 500,
	title: title_str
    };

    var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
    chart.draw(data_table, options);
}