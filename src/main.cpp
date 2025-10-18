#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHT22_PIN 2
#define GREEN_LED_PIN 4
#define RED_LED_PIN 3

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht22(DHT22_PIN, DHT22);

char displayChar = '-';

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
  dht22.begin(); // initialize the DHT22 sensor

  pinMode(DHT22_PIN, INPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  //lcd.setCursor(3,0);
  //lcd.print("Hello, world!");
  //lcd.setCursor(2,1);
  //lcd.print("Ywrobot Arduino!");
  //lcd.setCursor(0,2);
  //lcd.print("Arduino LCM IIC 2004");
  //lcd.setCursor(2,3);
  //lcd.print("Power By Ec-yuan!");
}

void loop() {
  // put your main code here, to run repeatedly:

  // wait a few seconds between measurements.
  delay(500);

  //toggleLED(GREEN_LED_PIN);
  //toggleLED(RED_LED_PIN);
  // read humidity
  float humi  = dht22.readHumidity();
  // read temperature as Celsius
  float tempC = dht22.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht22.readTemperature(true);

  //Serial.print("Humidity: ");
  //Serial.print(humi);
  //Serial.print("%\n");  

  lcd.setCursor(0,0);
  lcd.print("Humidity: ");
  lcd.print(String(humi, 2));
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Temp (C): ");
  lcd.print(String(tempC, 2));
  lcd.print("C");
  lcd.setCursor(0,2);
  lcd.print("Temp (F): ");
  lcd.print(String(tempF, 2));
  lcd.print("F");
  lcd.setCursor(0,3);
  lcd.print("Status: ");
  lcd.print(displayChar);
  toggleChar(displayChar);

}