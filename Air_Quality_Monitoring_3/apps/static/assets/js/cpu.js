"use strict";
//Notify
$.notify({
	icon: 'flaticon-interface-1',
	title: 'Info',
	message: 'This page contains information related to the microcontroller used, Wi-Fi configuration characteristics and memory usage.',
},{
	type: 'info',
	placement: {
		from: "bottom",
		align: "right"
	},
	time: 1000,
});
const options = {
	connectTimeout: 60000,
	// Authentication
	clientId: 'mqttjs_' + Math.random().toString(16),
	username: "web_client",
	password: "123456",
	keepalive: 60,
	clean: true,
};
var connected = false;
const WebSocket_URL = "ws://ec2-54-91-139-105.compute-1.amazonaws.com:8083/mqtt";
const client = mqtt.connect(WebSocket_URL, options);
client.on("connect", () => {
  console.log("Mqtt conectado por WS!");
  client.subscribe("values", { qos: 0 }, (error) => {
    if (!error) {
      console.log("Suscripción exitosa!");
    } else {
      console.log("Suscripción fallida!");
    }
  });
  // publish(topic, payload, options/callback)
  client.publish("fabrica", "esto es un verdadero éxito", (error) => {
    console.log(error || "Mensaje enviado!!!");
  });
});
client.on("message", (topic, message) => {
	console.log(
	  "Mensaje recibido bajo tópico: ",
	  topic,
	  " -> ",
	  message.toString()
	);
    const obj = JSON.parse(message.toString());
    var elemento=document.getElementById("mqtt-cpu-clock");
    elemento.innerHTML=`${obj.clkcpu}[MHz]`;
    elemento=document.getElementById("mqtt-app-cpu");
    elemento.innerHTML=`${obj.usecpu0}%`;
    elemento=document.getElementById("mqtt-pro-cpu");
    elemento.innerHTML=`${obj.usecpu1}%`;
    elemento=document.getElementById("mqtt-model");
    elemento.innerHTML=`${obj.model}`;
    elemento=document.getElementById("mqtt-cores");
    elemento.innerHTML=`${obj.cores}`;
    elemento=document.getElementById("mqtt-features");
    elemento.innerHTML=`${obj.features}`;
    elemento=document.getElementById("mqtt-revision");
    elemento.innerHTML=`${obj.revision}`;
    elemento=document.getElementById("mqtt-xtal-clock");
    elemento.innerHTML=`${obj.xtalclk}[MHz]`;
    elemento=document.getElementById("mqtt-ssid");
    elemento.innerHTML=`${obj.ssid}`;
    elemento=document.getElementById("mqtt-scan-method");
    elemento.innerHTML=`${obj.scanmethod}`;
    elemento=document.getElementById("mqtt-channel");
    elemento.innerHTML=`${obj.channel}`;
    elemento=document.getElementById("mqtt-mac");
    elemento.innerHTML=`${obj.mac}`;
    elemento=document.getElementById("mqtt-bssid");
    elemento.innerHTML=`${obj.bssid}`;
    elemento=document.getElementById("mqtt-sort-method");
    elemento.innerHTML=`${obj.sortmethod}`;
    elemento=document.getElementById("mqtt-threshold-rssi");
    elemento.innerHTML=`${obj.rssithr}[dBm]`;
    elemento=document.getElementById("mqtt-auth-mode");
    elemento.innerHTML=`${obj.authmode}`;
    elemento=document.getElementById("mqtt-protocol");
    elemento.innerHTML=`${obj.protocol}`;
    elemento=document.getElementById("mqtt-bandwidth");
    elemento.innerHTML=`${obj.bandwidth}[Mhz]`;
    elemento=document.getElementById("mqtt-rssi");
    elemento.innerHTML=`${obj.rssireal}[dBm]`;
});
  
client.on("reconnect", (error) => {
	console.log("Error al reconectar", error);
});
  
client.on("error", (error) => {
	console.log("Error de conexión:", error);
});