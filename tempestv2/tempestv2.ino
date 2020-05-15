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
  server.on("/", [](){
    page = "<h1>Tempest Web Server</h1><h3>Temperature:</h3> <h4>"+String(temp)+"</h4><h3>Humidity:</h3> <h4>"+String(hum)+"</h4>";
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