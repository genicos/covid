var http = require('http');
var url = require('url');

function onstart(req, res){
  res.writeHead(200, {'Contest-Type': 'text/html'});
  res.write('aw shit, here we go again\n');
  res.write('What appears after localhost:8080 is.... ');
  res.write(req.url + "\n");
  
  var q = url.parse(req.url, true).query;
  var txt = q.year + "  " + q.month;
  res.write(txt);
  
  res.end();
}

http.createServer(onstart).listen(8080);

//req is the requests from the client as an http.IncomingMessage object

//If you open from localhost:8080/?year=2020&month=july then txt will have those values
