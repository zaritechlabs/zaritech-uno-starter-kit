/**
 * @file smart_distance_radar.ino
 * @brief Official example project for the Zaritech Uno Starter Kit.
 * @details This project integrates the HC-SR04 Ultrasonic Sensor, I2C OLED Display, 
 *          and a Piezo Buzzer to create an intelligent obstacle detection radar system.
 * 
 * Developed by Zaritech Labs (A division of Zaritech Dynamics).
 * May Allah grant us success on this path.
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // Share reset pin with Arduino
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Hardware Pin Configuration
const int TRIG_PIN   = 9;  // HC-SR04 Trigger Pin
const int ECHO_PIN   = 10; // HC-SR04 Echo Pin
const int BUZZER_PIN = 11; // Piezo Buzzer Pin

// Radar System Thresholds (in centimeters)
const int CRITICAL_DISTANCE = 15; 

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Configure Hardware Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize I2C OLED Display (Default Address: 0x3C)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed. Check wiring!"));
    for(;;); // Stop execution if display is not found
  }

  // Display Welcome Screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 20);
  display.println("ZARITECH LABS");
  display.setCursor(15, 35);
  display.println("Uno Starter Kit");
  display.display();
  delay(2000); 
}

void loop() {
  long duration;
  int distance;

  // Clear trigger pin and send a 10-microsecond pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin pulse duration (in microseconds)
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in centimeters (Speed of sound is ~343 m/s)
  distance = duration * 0.034 / 2;

  // Error handling for out-of-range sensor readings
  if (distance > 400 || distance <= 0) {
    distance = -1; 
  }

  // Debug output to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Update OLED Display UI
  display.clearDisplay();
  
  // Draw Top Bar
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Zaritech Radar Sys.");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // Draw Distance Information
  display.setCursor(0, 25);
  if (distance == -1) {
    display.setTextSize(2);
    display.println("OUT OF RANGE");
  } else {
    display.setTextSize(2);
    display.print(distance);
    display.setTextSize(1);
    display.println(" cm");
    
    // Status Indicator Text
    display.setCursor(0, 50);
    if (distance <= CRITICAL_DISTANCE) {
      display.println("STATUS: WARNING!");
      // Activate alert sound (Buzzer) if obstacle is too close
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      display.println("STATUS: CLEAR");
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
  
  display.display();
  delay(100); // 10Hz Refresh Rate for smooth radar tracking
}
