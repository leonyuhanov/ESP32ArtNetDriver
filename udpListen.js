//A simple NODEJS Artnet MOnitor

var PORT = 6454;
var dgram = require('dgram');
var client = dgram.createSocket('udp4');

  client.on('listening', function ()
  {
      var address = client.address();
      console.log('UDP Client listening on ' + address.address + ":" + address.port);
      client.setBroadcast(true)
  });
  
  client.on('message', function (message, remote)
  {   
      console.log("Incoming Packet length:"+message.length+"\r\n--------------------------------");
	  var index=0, subIndex=0;
	  var logSTring="";

	  for(index=0; index<message.length; index++)
	  {
		logSTring+="["+index+"]["+message[index]+"]";
	  }
	  console.log(logSTring+"\r\n------------------\r\n");
  });
  
  client.bind(PORT);