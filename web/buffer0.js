// https://www.tutorialspoint.com/nodejs/nodejs_buffers.htm

var b0 = new Buffer([10, 20, 30, 40, 50]);

// "ascii", "utf8", "utf16le", "ucs2", "base64" ,  "hex"
var b1 = new Buffer("Simply Easy Learning", "utf-8");


var b2 = new Buffer(256);
var len = b2.write("Simply Easy Learning");

console.log("Octets written : "+  len);
var json = b0.toJSON(b0);
console.log(json);