#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <time.h>

const char* firestoreUrl = "https://firestore.googleapis.com/v1/projects/mbc-integ-f2c87/databases/(default)/documents/sensorData?key=AIzaSyCuSvVCiX0rw4x_PgNcIcz5NyF6EP3-C4E";

const char* ntpServer = "time.google.com";
const long gmtOffset_sec = 8 * 3600; // GMT+8 for Philippines
const int daylightOffset_sec = 0;

#define DHTPIN 23      // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT 11

#define LIGHT_SENSOR_PIN 32 // ESP32 pin GIOP33 (ADC0)

DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
const char* ssid = "realme7";
const char* password = "ricorico";

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
    int analogValue = analogRead(LIGHT_SENSOR_PIN);

  
    Serial.print("Daylight = ");
    Serial.print(analogValue);  

    String daylightValue;
    if (analogValue < 800) {  // Adjust threshold based on actual readings
        daylightValue = "1";  // Dark
        Serial.println(" => 1 (Dark)");
    } else {
        daylightValue = "0";  // Bright
        Serial.println(" => 0 (Bright)");
    }

    delay(5000);

    // Reading temperature and humidity values
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Print temperature and humidity readings to the Serial Monitor
    Serial.print(" Temperature: ");
    Serial.print(temp);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    if (WiFi.status() == WL_CONNECTED) {
        // Get current time
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Failed to obtain time");
            return;
        }
        
        char timestamp[30];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", &timeinfo);

        // Prepare HTTP client
        HTTPClient http;
        if (isnan(temp) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        // Manually construct the JSON string
        String jsonData = "{";
        jsonData += "\"fields\": {";
        jsonData += "\"daylight\": {\"stringValue\": \"" + String(daylightValue) + "\"},";
        jsonData += "\"temperature\": {\"stringValue\": \"" + String(temp) + "\"},";
        jsonData += "\"humidity\": {\"stringValue\": \"" + String(humidity) + "\"},";
        jsonData += "\"timestamp\": {\"stringValue\": \"" + String(timestamp) + "\"}";
        jsonData += "}}";

        http.begin(firestoreUrl);
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(jsonData);
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println(response);
        } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    }

    // Wait before next measurement
    delay(5000); // 5 seconds
}