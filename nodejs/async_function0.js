
function elapsed(start, stop) {
    return (stop.valueOf() - start.valueOf())/1000.0;
}

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

async function demo(intervals) {
    var start = new Date();
    console.log(start.toLocaleTimeString() + "." + start.getMilliseconds());
    while(intervals > 0) {
	await sleep(100);
	var stop = new Date();
	console.log(stop.toLocaleTimeString() + "." + stop.getMilliseconds());
	intervals--;
    }
}

demo(10);



