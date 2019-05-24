<pre>

    function make_diag( context , w , h )
    {
      context.beginPath();
      context.moveTo( 0, 0 );
      context.lineTo( w,h );

      context.moveTo( 0, h );
      context.lineTo( w,0 );
      context.stroke();
    }

    function make_rect( context , w , h )
    {
      var rw_i = 50;
      var w_mid = w/2 , h_mid = h/2;
      for( var rw = rw_i; rw < h - rw_i ; rw+= rw_i )
      {
        context.strokeRect( w_mid-rw/2 , h_mid-rw/2 , rw ,rw );
      }
    }

    function make_grid( context , w  , h )
    {
      var x = w;
      var y = h;
      var x_mid = x/2;
      var y_mid = y/2;

      context.beginPath();
      context.moveTo( 0 , y_mid );
      context.lineTo( x , y_mid );

      context.moveTo( x_mid , 0 );
      context.lineTo( x_mid , y );
      context.stroke();
    }

    function make_triang( context , w , h )
    {
      var x = w;
      var y = h;
      var dmin = x < y ? x : y;
      var x_mid = x/2;
      var y_mid = y/2;
      var l = 16;
      var slope = y/x;		     
     
      context.beginPath();

      for( var d = l ; dmin/2 - 2*d > 0 ; d += l )
      {
        context.beginPath();
        context.moveTo( x_mid - d , y_mid + slope*d );
        context.lineTo( x_mid + 2*d , y_mid + slope*d );
        context.moveTo( x_mid - d , y_mid + slope*d );
        context.lineTo( x_mid - d , y_mid - 2*slope*d );
        context.lineTo( x_mid + 2*d , y_mid + slope*d );
        context.stroke();
      }
    }

    function c_circles( context , w , h , r , incr )
    {
      var x = w;
      var y = h;
      var dmin = x < y ? x : y;
      var x_mid = x/2;
      var y_mid = y/2;

      for( var rc = r ; 2*rc <= dmin ; rc+= incr )
      {			    
      context.beginPath();
      context.arc( x_mid , y_mid , rc , 0 , 2*Math.PI , true );
      context.stroke();
      }

    }
			      
    function button_click()
    {
      var canvas = document.getElementById("grapha");
      var w = canvas.width , h = canvas.height;
      var context = canvas.getContext("2d");
      context.clearRect(0,0,w,h);
      var shapes = document.forms.shapesel.elements.shape;
      var circle = false, cross = false , diag = false , rect=false , triang = false;
      for( var i = 0 ; i < shapes.length ; i++)
      {
         if( shapes[i].checked == true )
         {
           if( shapes[i].value == "circle" ) circle = true;
           if( shapes[i].value == "cross" )  cross = true;
           if( shapes[i].value == "diag" )   diag = true;
           if( shapes[i].value == "rect" )   rect = true;
           if( shapes[i].value == "triang" )   triang = true;
           
         }  			 
       
      }
      if ( circle )
         c_circles( context, w , h  , 50 , 50 );
      if ( cross )
         make_grid( context, w , h );
      if ( diag )
         make_diag( context, w , h );
      if ( rect )
         make_rect( context, w , h );
      if ( triang )
         make_triang( context, w , h );
    } 
</pre>
