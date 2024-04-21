#include <WiFiNINA.h>
#include <Wire.h>

// Wi-Fi credentials
const char ssid[] = "NetComm 9527";       
const char pass[] = "geveffkeha";         

// IFTTT webhook configuration
const char IFTTT_WEBHOOK_KEY[] = "bzvTyNrphhGkwSbEHbNQBh";  
const char EVENT_NAME[] = "sunlight_exposure_detected";     

// Light sensor pin
const int LIGHT_SENSOR_PIN = A0;          
const int THRESHOLD_VALUE = 100;        

WiFiSSLClient client;

void setup() {
  Serial.begin(9600);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  
  // Connect to Wi-Fi
  connectToWiFi();
}

void loop() {
  int lightValue = analogRead(LIGHT_SENSOR_PIN);
  
  if (lightValue > THRESHOLD_VALUE) {
    Serial.println("Sunlight exposure detected!");

    // Trigger IFTTT event
    sendIFTTTEvent();
    
    // Wait for 2 hours before checking again
    delay(2 * 60 * 60 * 1000); // 2 hours in milliseconds
  } else {
    Serial.println("No sufficient sunlight exposure.");
    delay(1000); // Check again in 1 second
  }
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nWiFi Connected");
}

void sendIFTTTEvent() {
  if (client.connect("maker.ifttt.com", 443)) {
    Serial.println("Sending data to IFTTT");

    // Construct the HTTP request
    String postData = "value1=Sunlight%20Exposure";
    String url = "/trigger/" + String(EVENT_NAME) + "/with/key/" + String(IFTTT_WEBHOOK_KEY);
    client.print("POST " + url + " HTTP/1.1\r\n");
    client.print("Host: maker.ifttt.com\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: ");
    client.print(postData.length());
    client.print("\r\n\r\n");
    client.print(postData);
    client.print("\r\n");

    delay(1000); // Wait for IFTTT response
    Serial.println("IFTTT Event Sent");
  } else {
    Serial.println("Failed to connect to IFTTT server");
  }

  client.stop(); // Close the connection
}
