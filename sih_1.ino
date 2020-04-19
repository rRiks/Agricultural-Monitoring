#include <SoftwareSerial.h>   //Header file to create RX & Tx 
#include <stdlib.h>
#include <DHT.h> //  DHT.h library
#define DHTPIN A0 
#define DHTTYPE  DHT11 
#define SOIL A1
DHT dht(DHTPIN, DHTTYPE); //Intialization of DHT11
// replace with your channel's thingspeak API key
String apiKey = "70PQYLTAOD3AFDQK";//application program interface
SoftwareSerial ser(2, 3); // RX, TX
//SoftwareSerial ser(8, 9); // RX, TX   For GSM

int i=1;
int motor1=5;
int motor2=6;
int soildi=7;
// this runs once
void setup() {                
  // initialize the sensor at 7th pin as an input.
  // enable debug serial
  Serial.begin(115200);//Baud rate for serial communication
  // enable software serial
   pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
     pinMode(8, OUTPUT);
    pinMode(soildi,INPUT);
  ser.begin(115200);    // Baud rate for ESP8266
  dht.begin(); //To start the DHT11 Sensor 
  ser.println("AT");
  delay(1000);
  ser.println("AT+RST");    //RESET
  delay(1000);
  ser.println("AT+CWMODE=1");   //Station
  delay(1000);
  ser.println("AT+CWJAP=\"diks\",\"12345687\"");
  delay(10000);
}

 
void loop() 
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float moisture=analogRead(A1);
  float WATER=analogRead(A2);
   float moisture1=digitalRead(soildi);
   Serial.print("moisture1=");
   Serial.println(moisture);
    Serial.print("moisture1=");
   Serial.println(WATER);
   delay(1000);
   if(moisture>1000||moisture<300)
   {
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    Serial.print("OK");
   }
   else
   {
   digitalWrite(5,LOW);
   digitalWrite(6, LOW);
    Serial.print("NOT OK");
   }
   if(WATER>100)
   {
    Serial.println("WATER MORE USE");
    digitalWrite(8, HIGH);
    Serial.println("AT+CMGF=1");
delay(1000);
Serial.println("AT+CMGS=\"+91xxxxxxxxxx\"\r");
delay(1000);
Serial.print(" TYPE MSG");
delay(200);
Serial.println((char)26);
delay(1000);
   }
   else
   {
    digitalWrite(8,LOW);
   }
   
   
  String field1=String(humidity);   //Float value of Humidity converted to string
  String field2=String(temperature); 
  String field3=String(moisture);
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\""; //transmission control protocol
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80"; //instantiate server at port 80 (http port)hyper text transfer protocol4
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  // prepare GET string
  //GET https://api.thingspeak.com/update?api_key=TMWR47CL9G5PTZHM&field1=0&field2=0
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(field1);
  getStr +="&field2=";
  getStr += String(field2);
  getStr +="&field3=";
  getStr += String(field3);
  
  
  getStr += "\r\n\r\n\r\n";
  // send data length
  cmd = "AT+CIPSEND=";        // Sending data
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");  // Closing TCP Connection
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  // thingspeak needs 15 sec delay between updates
  delay(1500);  
}

