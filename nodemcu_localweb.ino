#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
// Replace with your network credentials
const char* ssid = "iot";
const char* password = "12345687";
 
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
 
String page = "";
int resistor = A0;
int data;
void setup(void){
  //the HTML of the web page
 
  //make the LED pin output and initially turned off
  pinMode(resistor,INPUT);
  
   
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
     page = "<!DOCTYPE html><html><body><h1>poetiometer data</h1><h2>"+String(data)+"</h2>";
    server.send(200, "text/html", page);
  });
  
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  data=analogRead(resistor);
  delay(1000);
  server.handleClient();
}
