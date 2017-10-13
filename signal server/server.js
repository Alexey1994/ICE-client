var express = require('express')
var server  = express()

var addres1
var address2

server.get('/reg1', function(request, response){
	addres1 = request.query['address']
	console.log('registrate1 ' + addres1)
	response.send()
})

server.get('/reg2', function(request, response){
	addres2 = request.query['address']
	console.log('registrate2 ' + addres2)
	response.send()
})

server.get('/conn1', function(request, response){
	response.send(address2)
})

server.get('/conn2', function(request, response){
	response.send(addres1)
})

server.listen(8080)