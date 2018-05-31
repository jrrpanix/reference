// https://www.tutorialspoint.com/nodejs/nodejs_callbacks_concept.htm

var fs = require("fs");

console.log("Blocking Read : fs.readFileSync(<file_name>)");
var data = fs.readFileSync('input.txt');
console.log(data.toString());

console.log("NonBlocking Read: fs.readFile('input.txt', <callback>)");
fs.readFile('input.txt', function (err, data) {
	if (err) return console.error(err);
	console.log(data.toString());
    });



console.log("Program Ended");

