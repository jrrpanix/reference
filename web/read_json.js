var fs = require("fs");

function AsJSON(fname) {
    return JSON.parse(fs.readFileSync(fname));
}

var obj = AsJSON("polo.json");
var c = obj["BTC"];
console.log(c["last"]);


