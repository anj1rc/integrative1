#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 23
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "realme7";
const char* password = "ricorico";

void setup() {
    Serial.begin(115200);
    dht.begin();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void loop() {
 
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    if ((WiFi.status() == WL_CONNECTED)) {
        HTTPClient http;

        if (isnan(temp) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        String payload_temp = "{\"name\": \"temperature\", \"value\": \"" + String(temp) + "\"}";
        http.begin("http://192.168.19.120/dht11sensor/api.php"); 
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode_temp = http.POST(payload_temp);
        if (httpResponseCode_temp > 0) {
            String response_temp = http.getString();
            Serial.println(httpResponseCode_temp);
            Serial.println(response_temp);
        } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode_temp);
        }
        http.end();

        String payload_humidity = "{\"name\": \"humidity\", \"value\": \"" + String(humidity) + "\"}";
        http.begin("http://192.168.19.120/dht11sensor/api.php"); 
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode_humidity = http.POST(payload_humidity);
        if (httpResponseCode_humidity > 0) {
            String response_humidity = http.getString();
            Serial.println(httpResponseCode_humidity);
            Serial.println(response_humidity);
        } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode_humidity);
        }
        http.end();
    }

    delay(60000); 
}