// Temperature Control
// Written by Romualdo Dasig for Elex Project
// Released under an MIT license.

// Depends on the following Arduino libraries:
// - LiquidCrystal Library: https://www.arduino.cc/en/Reference/LiquidCrystal
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

#include <LiquidCrystal.h>
#include <DHT.h>

// Define constants.
#define DEBUG 0                   // Enable or disable debugging
#define DHTPIN  A0                // Temperature Sense pin
#define DHTTYPE DHT11             // DHT 11

// Configure Temperature sensor.
DHT dht(DHTPIN, DHTTYPE);
float hum;                        // Humidity
float temp;                       // Temperature
unsigned long previousMillis = 0; // Variable to store current time.
const long interval = 2000;       // Set interval to read temperature.

// Configure LCD display.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Configure aircon and fan motors.
const int airconPin = 9;
const int fanPin = 10;

// Configure speed controller.
const int speedControlPin = 8;
const int lowSpeed = 125;
const int highSpeed = 255;
int speedVal = lowSpeed;

// Set temperature settings.
int airconState = 0;
const float highTempSetting = 26.00;
const float lowTempSetting = 23.00;

void setup() {
  if (DEBUG) {
    // Initialize Serial monitor.
    Serial.begin(9600);
  }

  // Initialize Temperature sensor.
  dht.begin();

  // Initialize a 16x2 LCD display.
  lcd.begin(16, 2); 
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");

  // Initialize aircon and fan motors.
  pinMode(airconPin, OUTPUT);
  pinMode(fanPin, OUTPUT);

  // Initialize speed controller.
  pinMode(speedControlPin, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // Save the last time we've read the temperature.
    previousMillis = currentMillis;

    // Read data and store it to variables hum and temp.
    hum = dht.readHumidity();
    temp= dht.readTemperature();

    if (DEBUG) {
      // Print temp and humidity values to serial monitor.
      Serial.print("Humidity: ");
      Serial.print(hum);
      Serial.print(" %, Temp: ");
      Serial.print(temp);
      Serial.print(" *C\n");
    }
  }

  // Display temperature and humidity.
  lcd.setCursor(6, 0);
  lcd.print(temp);
  lcd.setCursor(6, 1);
  lcd.print(hum);

  // Set speed of the motor and fan.
  int speedState = digitalRead(speedControlPin);
  if (speedState == HIGH) {
    speedVal = highSpeed;
  }
  else {
    speedVal = lowSpeed;
  }

  // Run fan motor.
  analogWrite(fanPin, speedVal);

  // Turn on the aircon when temperature is high.
  if (airconState == 1) {
    if (temp <= lowTempSetting) {
      // Turn off the aircon.
      analogWrite(airconPin, 0);

      // Set aircon state to idle.
      airconState = 0;
    }
  }
  else {
    if (temp >= highTempSetting) {
      // Turn on the aircon.
      analogWrite(airconPin, lowSpeed);

      // Set aircon state to running.
      airconState = 1;
    }
  }
}

