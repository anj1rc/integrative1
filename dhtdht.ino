#include <WiFi.h>
#include <HTTPClient.h>

#include <DHT.h>
#define DHTPIN 23
#define DHTTYPE DHT11

DHT dht11 (DHTPIN, DHTTYPE);


String URL = "http://localhost/dht11_sensor/db.php";

const char* ssid = "realme7";
const char* password ="ricorico";

float temperature = 0;
float humidity = 0;

void setup() {
  Serial.begin(115200);
  dht11.begin();


    WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000); 
    Serial.println("Connecting to WiFi..."); 
  } 
  Serial.println("Connected to WiFi"); 
  Serial.println(WiFi.localIP()); 
}

void loop() {
getSensorValue();
String postData = "temperature " + String(temperature) + "& humidity = " + String(humidity);

HTTPClient http;
http.begin(URL);

http.addHeader("Content-Type", "application/json");
int httpCode = http.POST(postData);
String payLoad = http.getString();

Serial.print("URL : "); 
Serial.println(URL);
Serial.print("DATA : "); 
Serial.println(postData);
Serial.print("httpCode : "); 
Serial.println(httpCode);
Serial.print("payload : "); 
Serial.println(payLoad);
}

void getSensorValue() {
  temperature = dht11.readTemperature();
  humidity = dht11.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    temperature = 0;
    humidity = 0;
  }
  Serial.printf("Temperature: %d °C\n", temperature);
  Serial.printf("Humidity: %d %%\n", humidity);
}
