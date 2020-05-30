const http = require('http');
const fs = require('fs'); /* File System module */

const hostname = '127.0.0.1';
const port = 3000;

// once server is created, listen is called as a callback
const server = http.createServer((req, res) => {
  // statusCode of 200 means server creation was successful
  res.statusCode = 200;
  //res.end('Hello Mama');

  fs.readFile('B:/COVID Visualization Website/covid/website/d3test/us.csv', 'utf-8', (err,data) => {
    if (err) {
        console.error(err);
        return;
    }
    console.log(data);
    res.setHeader('Content-Type', 'text/plain');
    res.write(`Data: ${data}`);
  });
});

// res = response, req = request

// listen is a callback function
server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});

/* readFile reads entire file before processing callback, which could be slow - the alternative
* is to set up a data stream, but I don't think we can make graphs without loading the whole file :/
*/


process.on('SIGTERM', () => {
    server.close(() => {
        console.log('Process terminated');
    })
});
