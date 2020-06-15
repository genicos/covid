var http = require('http');

http.createServer(function (req,res) {
  res.write('Hello Mother Fucker!'); //res means response
  res.end(); //end the response
}).listen(8080);//Server object listens on port 8080

//running just above will display 'Hello Mother Fucker' to page

//the function passed to createServer is what is called whan a user accessess port 8080


