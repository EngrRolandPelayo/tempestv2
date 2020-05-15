#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT11


DHT dht(DHTPIN, DHTTYPE);

// Replace with your network credentials
const char* ssid = "PLDTHOMEFIBRjVZpa";
const char* password = "stacey0921";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "";
String text = "";
float temp, hum; 
void setup(void){

  dht.begin();
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/data.txt", [](){
   text = (String)temp + "&" + (String)hum;
   server.send(200, "text/html", text);
 });
  server.on("/", [](){
    page = "<h1>Tempest V2</h1><div id=\"data\">""</div>\r\n";
   page += "<script>\r\n";
   page += "var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 1000);\r\n";
   page += "function loadData(url, callback){\r\n";
   page += "var xhttp = new XMLHttpRequest();\r\n";
   page += "xhttp.onreadystatechange = function(){\r\n";
   page += " if(this.readyState == 4 && this.status == 200){\r\n";
   page += " callback.apply(xhttp);\r\n";
   page += " }\r\n";
   page += "};\r\n";
   page += "xhttp.open(\"GET\", url, true);\r\n";
   page += "xhttp.send();\r\n";
   page += "}\r\n";
   page += "function updateData(){\r\n";
   page += "var data = this.responseText;\r\n";
   page += "var s = data.split('&')\r\n;";
   page += "var temp = s[0];\r\n";
   page += "var hum = s[1];\r\n";
   page += " document.getElementById(\"data\").innerHTML = '<h3>Temperature: '+temp+' &#176;C</h3><h3>Humidity: '+hum+' %</h3>';\r\n";
   page += "}\r\n";
   page += "</script>\r\n";
    server.send(200, "text/html", page);
  });
  
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  delay(1000);
  server.handleClient();
}