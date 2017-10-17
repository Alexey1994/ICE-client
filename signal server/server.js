var express = require('express')
var server  = express()

var address1 = {host: "127.0.0.1", port: "80"}
var address2 = {host: "127.0.0.1", port: "80"}

server.get('/reg1', function(request, response){
	address1 = {
		host: request.query['host'],
		port: request.query['port']
	}

	console.log('registrate1')
	console.log(address1)
	console.log("")

	response.send()
})

server.get('/reg2', function(request, response){
	address2 = {
		host: request.query['host'],
		port: request.query['port']
	}

	console.log('registrate1')
	console.log(address2)
	console.log("")

	response.send()
})


server.get('/host1', function(request, response){
	response.send(address2.host)
})

server.get('/port1', function(request, response){
	response.send("" + address2.port)
})


server.get('/host2', function(request, response){
	response.send(address1.host)
})

server.get('/port2', function(request, response){
	response.send("" + address1.port)
})

server.listen(8080)