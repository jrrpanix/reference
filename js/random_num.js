<pre>


    //
    // function "start_click()" is referenced in the htlm for a buttons "onClick" attribute
    // so on a click of a mousel this routine will get invoked 
    //	
    function start_click()
    {
      // this funciton is a demonstration of  AJAX call that is made synchronoously
      // in this case when the start_click button is called 
      // we will invoke the php source "rand_ajax.php"
      // can get back the data in the response
      var ro = new XMLHttpRequest();

      //  the 3rd argument to "open" is "false" this means make the call "synchronously"
      //  if set to "true" then the call would be "asynchronous" and you would have to
      //  supply a callback function for entry upon return of the server call you are making
      ro.open( "GET" , "rand_ajax.php" , false );



      // wait for a return because we are calling synchronously
      // if the call were asynchronous we would exit from this routine 
      // and the code would be free to do something else until the
      // server returned
      ro.send();

      // get the element on the html document we want to
      // write the result to
      var num = document.getElementById( "data_dump" );

      // take the response text from the server and assign it to the html element
      num.innerHTML=ro.responseText;
    }

</pre>