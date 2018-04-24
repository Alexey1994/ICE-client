var express = require('express');
var bodyParser = require('body-parser');
var server = express();
var jsonParser = bodyParser.json();


var addresses = {};


server.post('/connect', jsonParser, function (request, response) {
    var body = request.body;

    if(addresses[body.source] && addresses[body.source].listenerId){
        clearInterval(addresses[body.source].listenerId);
        addresses[body.source].listenerId = undefined;
    }

    addresses[body.source] = {
        address: body.address
    };

    console.log('connect ' + body.source + ' candidate ' + body.address);
    response.send();
});

server.post('/get_candidate', jsonParser, function (request, response) {
    var body = request.body;
    var source = addresses[body.source];

    if(!source){
        console.log('client with id ' + body.source + ' not connected');
        response.send('error: client with id ' + body.source + ' not connected');
        return;
    }

    if(source.listenerId)
        clearInterval(source.listenerId);
    
    console.log('get_candidate source ' + body.source + ' destination ' + body.destination);
    
    source.listenerId = setInterval(function(){
        //console.log('listen notifications for ' + body.source);

        if(addresses[body.destination]){
            response.send(addresses[body.destination].address);
            clearInterval(source.listenerId);
            source.listenerId = undefined;
            console.log('send candidate from ' + body.destination + ' to ' + body.source + ' candidate ' + addresses[body.destination].address);
        }
    }, 100);
});

server.post('/disconnect', jsonParser, function (request, response) {
    var body = request.body;
    var source = addresses[body.source];

    if(source.listenerId)
        clearInterval(source.listenerId);

    console.log('disconnect source ' + body.source);
    addresses[body.source] = undefined;

    response.send();
});


var servers = {};


server.post('/connect_to', jsonParser, function (request, response) {
    var body = request.body;
    var destination = servers[body.destination];
    
    console.log('connect ' + body.source + ' to ' + body.destination)
    
    if(!destination){
        response.send('error: server not found');
        return;
    }

    if(!destination.clients){
        response.send('error: server offline');
        return;
    }

    destination.clients.push(body.source);

    response.send();
});

server.post('/listen', jsonParser, function (request, response) {
    var body = request.body;
    
    servers[body.id] = {
        clients: []
    };

    var server = servers[body.id];

    server.clients = [];
    server.serverListener = setInterval(function(){
        //console.log('serve ' + body.id)

        if(server.clients.length){
            response.send(server.clients.shift());
            clearInterval(server.serverListener);
        }
    }, 10)
});

server.post('/stop_listen', jsonParser, function (request, response) {
    var body = request.body;
    var server = servers[body.id];

    clearInterval(server.serverListener);
    servers[body.id] = undefined;
    response.send();
});


server.listen(8080);