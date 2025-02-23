#include <WiFi.h>
#include <WebServer.h> 

const char* ssid = "realme7";
const char* password = "ricorico";

const int led1 = 23; 
const int led2 = 21; 
const int led3 = 19; 
const int led4 = 16; 
const int led5 = 5; 

bool handleAlt = false;
bool handleCtb = false;

WebServer server(80); 

void handleRoot() {
  // HTML content with a button
  String html = "<!DOCTYPE html><html><head><title>ESP32 LED Control</title></head><body>";
  html += "<h1>ESP32 LED Control</h1>";
  html += "<button onclick=\"fetch('/on').then(response => alert('LED ON!'))\">Turn LED ON</button>";
  html += "<button onclick =\"fetch('/off').then(response => alert('LED OFF!'))\">Turn LED OFF</button>";
  html += "<button onclick=\"fetch('/alt').then(response => alert('ALTERNATE LED!'))\">ALTERNATE LED</button>";
  html += "<button onclick=\"fetch('/ctb').then(response => alert('CENTER TO BOTTOM LED!'))\">CENTER TO BOTTOM LED</button>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);  // Send the HTML content to the client
}

void handleOn() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  digitalWrite(led5, HIGH);  // Turn the LED on
  handleAlt = false;
handleCtb = false;
  server.send(200, "text/plain", "LED is ON");
}

void handleOff() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);  // Turn the LED off
  handleAlt = false;
handleCtb = false;
  server.send(200, "text/plain", "LED is OFF");
}



void alternate() {
handleAlt = true;
handleCtb = false;
server.send(200, "text/plain", "LED ALT");
  }


void centertobottom() {
handleCtb = true;
handleAlt = false;
server.send(200, "text/plain", "LED Center to Bottom");
  }


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
pinMode(led4, OUTPUT);
pinMode(led5, OUTPUT);

digitalWrite(led1, LOW);
digitalWrite(led2, LOW);
digitalWrite(led3, LOW);
digitalWrite(led4, LOW);
digitalWrite(led5, LOW);  

WiFi.begin(ssid, password);
while (WiFi.status()!= WL_CONNECTED) {
  delay(1000);
  Serial.println("Connecting to Wifi...");
}
Serial.println("Connected to Wifi.");

Serial.println(WiFi.localIP());

server.on("/", handleRoot); 
server.on("/on", handleOn);
server.on("/off", handleOff);
server.on("/alt", alternate); 
server.on("/ctb", centertobottom); 

server.begin(); 
Serial.println("HTTP server started"); 
}


void loop() {
  // put your main code here, to run repeatedly:
server.handleClient(); 

if (handleAlt) {
handleAlt = true;
digitalWrite(led1, HIGH);
delay(500);
digitalWrite(led1, LOW);

digitalWrite(led3, HIGH);
delay(500);
digitalWrite(led3, LOW);

digitalWrite(led5, HIGH);
delay(500);
digitalWrite(led5, LOW);

digitalWrite(led2, HIGH);
delay(500);
digitalWrite(led2, LOW);

digitalWrite(led4, HIGH);
delay(500);
digitalWrite(led4, LOW);
}

if (handleCtb) {
 handleCtb = true;
  digitalWrite(led3, HIGH);
delay(500);
digitalWrite(led3, LOW);

digitalWrite(led2, HIGH);
digitalWrite(led4, HIGH);
delay(500);
digitalWrite(led2, LOW);
digitalWrite(led4, LOW);

digitalWrite(led1, HIGH);
digitalWrite(led5, HIGH);
delay(500); 
digitalWrite(led1, LOW);
digitalWrite(led5, LOW);

delay(500);
//back

digitalWrite(led1, HIGH);
digitalWrite(led5, HIGH);
delay(500);
digitalWrite(led1, LOW);
digitalWrite(led5, LOW);

digitalWrite(led2, HIGH);
digitalWrite(led4, HIGH);
delay(500);
digitalWrite(led2, LOW);
digitalWrite(led4, LOW);

digitalWrite(led3, HIGH);
delay(500);
digitalWrite(led3, LOW);

delay(500);
}
}
