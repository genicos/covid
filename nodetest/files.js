var fs = require('fs');
var http = require('http');

function onstart(req,res){
  res.writeHead(200, {'Content-Type': 'text/html'});
  
  fs.readFile('readthis.html', function(err, data) {
    res.write(data);
    return res.end();
  });
  
}

http.createServer(onstart).listen(8080);

