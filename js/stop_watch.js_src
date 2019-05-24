<pre>
    // Global Variables 
    var Start_Date  ; // Keeps Track of the Start Time
    var End_Date    ; // Keeps Track of the End Time
    var CT_Interval ; // Keeps Track of the ID for setInterval

    function update_current_time()
    {
    // update current time is called from setInterval

    var date = new Date();
    var curr = document.getElementById( "current" );
    curr.innerHTML =  date.toLocaleTimeString();
    elapsed_time( Start_Date , date );
    }

    function elapsed_time( s , e )
    {
    // this calculates the elapsed time between s - start and e - end
    // time difference is in milliseconds
    var d = e.valueOf() - s.valueOf();
    var seconds = d / 1000;
    var elap = document.getElementById( "elapsed" );
    elap.innerHTML = seconds + " sec" ;
    }

    function start_click()
    {
    // this function is called when the start button is clicked
    // set new start value for the time
    var date = new Date();
    var start = document.getElementById( "start" );
    var current = document.getElementById( "current" );
    var stop = document.getElementById( "stop" );
    var elap = document.getElementById( "elapsed" );
    start.innerHTML = date.toLocaleTimeString();
    current.innerHTML = date.toLocaleTimeString();
    stop.innerHTML = " ";
    elap.innerHTML = " ";
    Start_Date = date;

    // set a timer to go off every 100 milliseconds (1/10th of a second)
    // this will cause update_current_time to be called
    // keep track of the return value from setInterval using the variable CT_Interval
    // this value is needed otherwise we will not be able to stop the watch
    CT_Interval = setInterval( update_current_time , 100 );
    }

    function stop_click()
    {
    // this function is called when the stop button is clicked
    var date = new Date();
    var stop = document.getElementById( "stop" );
    var curr = document.getElementById( "current" );
    stop.innerHTML = date.toLocaleTimeString();
    curr.innerHTML = date.toLocaleTimeString();
    End_Date = date;
    elapsed_time(Start_Date,End_Date);

    // stop the timer
    clearInterval( CT_Interval );
    }

</pre>
