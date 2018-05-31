
img_plus = new Image(9,9);
img_plus.src = "images/plus.gif";
img_minus = new Image(9,9);
img_minus.src = "images/minus.gif";


function filter( img , objsrc )
{
    if ( document.images )
	{
	    var cur_src = eval(objsrc+".src");
	    document.images[img].src = cur_src;
	}
}

function show_hide( id )
{
    
    if ( document.getElementById )
	{
	    if ( document.getElementById( id ).style.display == "none" )
		{
		    document.getElementById( id ).style.display = "block";
		    filter(("img"+id),'img_minus');
		}
	    else
		{
		    filter(("img"+id),'img_plus');
		    document.getElementById( id ).style.display = "none";
		}
        }
    else if ( document.layers )
	{
	    if ( document.id.display == "none" )
		{
		    document.id.display = "block";
		    filter(("img"+id),'img_minus');
		}
	    else
		{
		    filter(("img"+id),'img_plus');
		    document.id.display = "none";
		}
	}
    else if ( document.all.id.style.visibility == "none" )
	{
	    document.all.id.style.visibility = "block";
	    filter(("img"+id),'img_minus');
	}
    else
	{
	    filter(("img"+id),'img_plus');
	    document.all.id.style.visibility = "none";
	}
}


