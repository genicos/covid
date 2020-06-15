//modules are like libraries
//require('shit') is like #include shit

var cum = require('./test2');
var http = require('http');

http.createServer(              //meta function
  function (req, res) {
    res.writeHead(200, {'Content-Type': 'text/html'});
    res.write("What: " + cum.cumrag(2));
    res.end('Hello World!');
  }
).listen(8080);
