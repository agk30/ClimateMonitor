#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include "SECRET.h"
#include <MQTT.h>
#include "mqttHelper.h"
#include <WiFiS3.h>
//#include <WiFiNINA.h>

#define DHT22_PIN 2
#define GREEN_LED_PIN 4
#define RED_LED_PIN 3

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// MQTT credentials from SECRET.h
const char mqtt_user[] = SECRET_MQTT_USER;
const char mqtt_password[] = SECRET_MQTT_PASS;

WiFiClient wifiClient;
MQTTClient mqttDude;


int count = 0;
//set interval for sending messages (milliseconds)
const long interval = 30000;  // Send every 30 seconds (recommended for HA)
bool discovery_sent = false;

unsigned long previousMillis = 0;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht22(DHT22_PIN, DHT22);



char displayChar = '-';

// MQTT message callback (required by MQTT.h library)
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

// put function declarations here:
int myFunction(int, int);

void toggleLED(int pin) {
  if (digitalRead(pin) == LOW) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}

void toggleChar(char &c) {
  if (c == '-') {
    c = '|';
  } else {
    c = '-';
  }
}



void setup() {

  Serial.begin(9600);

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  
  //dht22.begin(); // initialize the DHT22 sensor

  //pinMode(DHT22_PIN, INPUT);
  //pinMode(GREEN_LED_PIN, OUTPUT);
  //pinMode(RED_LED_PIN, OUTPUT);

  //lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  //lcd.backlight();

 

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {

    // failed, retry

    Serial.print(".");

    //delay(5000);

  }

  Serial.println("You're connected to the network");

  Serial.println();


  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(MQTT_BROKER);
  Serial.print("Using username: ");
  Serial.println(mqtt_user);
  Serial.print("Password length: ");
  Serial.println(strlen(mqtt_password));

  // Initialize MQTT client with callback
  mqttDude.begin(MQTT_BROKER, wifiClient);
  mqttDude.onMessage(messageReceived);

  Serial.print("\nconnecting...");
  while (!mqttDude.connect("arduino", mqtt_user, mqtt_password)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // Send Home Assistant discovery messages
  sendHomeAssistantDiscovery(mqttDude);
  discovery_sent = true;

}


void loop() {
  // put your main code here, to run repeatedly:

  // wait a few seconds between measurements.
  delay(500);

  //toggleLED(GREEN_LED_PIN);
  //toggleLED(RED_LED_PIN);
  // read humidity
  //float humi  = dht22.readHumidity();
  // read temperature as Celsius
  //float tempC = dht22.readTemperature();
  // read temperature as Fahrenheit
  //float tempF = dht22.readTemperature(true);

  float humi = 55.0;
  float tempC = 22.0;
  float tempF = tempC * 9.0 / 5.0 + 32.0;

  // Check if readings are valid
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT sensor!");
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
  } else {
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    
    //Serial.print("Humidity: ");
    //Serial.print(humi);
    //Serial.print("%\n");  

    //lcd.setCursor(0,0);
    //lcd.print("Humidity: ");
    //lcd.print(String(humi, 2));
   // lcd.print("%");
   // lcd.setCursor(0,1);
    //lcd.print("Temp (C): ");
   // lcd.print(String(tempC, 2));
   // lcd.print("C");
   // lcd.setCursor(0,2);
   // lcd.print("Temp (F): ");
   // lcd.print(String(tempF, 2));
    //lcd.print("F");
   // lcd.setCursor(0,3);
   // lcd.print("Status: ");
   // lcd.print(displayChar);
   // toggleChar(displayChar);
  }

  // Keep MQTT connection alive
  mqttDude.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    // Only send data if sensor readings are valid
    if (!isnan(humi) && !isnan(tempC)) {
      Serial.println("=== Sending Data to Home Assistant ===");
      Serial.println("Temperature: " + String(tempC) + "°C");
      Serial.println("Humidity: " + String(humi) + "%");
      
      // Send temperature
      Serial.println("Publishing temperature to: " + String(HA_STATE_TEMP));
      if (mqttDude.publish(HA_STATE_TEMP, String(tempC, 2).c_str())) {
        Serial.println("✓ Temperature sent successfully");
      } else {
        Serial.println("✗ Failed to send temperature");
      }

      delay(100);
      
      // Send humidity
      Serial.println("Publishing humidity to: " + String(HA_STATE_HUM));
      if (mqttDude.publish(HA_STATE_HUM, String(humi, 2).c_str())) {
        Serial.println("✓ Humidity sent successfully");
      } else {
        Serial.println("✗ Failed to send humidity");
      }

      Serial.println("=== Data sent successfully ===");
    } else {
      Serial.println("ERROR: Invalid sensor data - skipping MQTT send");
    }

    Serial.println();
  }

}