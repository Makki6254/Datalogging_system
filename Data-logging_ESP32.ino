#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "TECH";
const char* password = "12345678";

// Google script Web_App_URL.
String Web_App_URL = "https://script.google.com/macros/s/AKfycbwkNWJus9qdYH2Dojxh897PCvjg_upyqcBbMvowdxiXQnrOHeekaePfXvToZkfxWpOY/exec";

// Variables to store sensor data
String Status_Read_Sensor = "Success"; 
int analog1, analog2, analog3, analog4;
int digital1, digital2, digital3, digital4;

// Define analog and digital pins
#define ANALOG_PIN_1 34
#define ANALOG_PIN_2 35
#define ANALOG_PIN_3 32
#define ANALOG_PIN_4 33

#define LED_PIN 23

#define DIGITAL_PIN_1 13
#define DIGITAL_PIN_2 12
#define DIGITAL_PIN_3 14
#define DIGITAL_PIN_4 27

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize sensor pins
  pinMode(ANALOG_PIN_1, INPUT);
  pinMode(ANALOG_PIN_2, INPUT);
  pinMode(ANALOG_PIN_3, INPUT);
  pinMode(ANALOG_PIN_4, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  pinMode(DIGITAL_PIN_1, INPUT);
  pinMode(DIGITAL_PIN_2, INPUT);
  pinMode(DIGITAL_PIN_3, INPUT);
  pinMode(DIGITAL_PIN_4, INPUT);

  // Initialize WiFi
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting to WiFi...");
    Serial.println(ssid);
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  // Read analog sensor data
  analog1 = analogRead(ANALOG_PIN_1);
  analog2 = analogRead(ANALOG_PIN_2);
  analog3 = analogRead(ANALOG_PIN_3);
  analog4 = analogRead(ANALOG_PIN_4);

  // Read digital sensor data
  digital1 = digitalRead(DIGITAL_PIN_1);
  digital2 = digitalRead(DIGITAL_PIN_2);
  digital3 = digitalRead(DIGITAL_PIN_3);
  digital4 = digitalRead(DIGITAL_PIN_4);

  if (WiFi.status() == WL_CONNECTED) {
    // Prepare the URL with the sensor data
    String Send_Data_URL = Web_App_URL + "?sts=" + Status_Read_Sensor;
    Send_Data_URL += "&a1=" + String(analog1);
    Send_Data_URL += "&a2=" + String(analog2);
    Send_Data_URL += "&a3=" + String(analog3);
    Send_Data_URL += "&a4=" + String(analog4);
    Send_Data_URL += "&d1=" + String(digital1);
    Send_Data_URL += "&d2=" + String(digital2);
    Send_Data_URL += "&d3=" + String(digital3);
    Send_Data_URL += "&d4=" + String(digital4);

    Serial.println("Sending data to Google Sheets...");

    // Send the request to Google Sheets
    HTTPClient http;
    http.begin(Send_Data_URL.c_str());
    int httpCode = http.GET(); 

    // Check the response code
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Response: " + payload);
      digitalWrite(LED_PIN,HIGH);
      delay(200);
      digitalWrite(LED_PIN,LOW);
    } else {
      Serial.println("Error in HTTP request");
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  // Wait for 10 seconds before sending the next set of data
  delay(5000);// Data Send Time
  
}
