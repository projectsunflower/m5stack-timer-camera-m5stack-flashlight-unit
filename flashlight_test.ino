#include <M5Unified.h>
#include <Wire.h>

// Define GPIO pins for FlashLight control
#define FLASHLIGHT_PIN 26  // Example GPIO pin, adjust as needed

void setup() {
  // Initialize M5Stack
  M5.begin();
  Wire.begin();

  // Initialize FlashLight pin
  pinMode(FLASHLIGHT_PIN, OUTPUT);
  digitalWrite(FLASHLIGHT_PIN, LOW); // Turn off FlashLight initially

  // Initialize Timer Camera X
  // Assuming Timer Camera X is already set up and connected
  Serial.begin(115200);
  Serial.println("Timer Camera X and FlashLight Test");
}

void loop() {
  // Example: Turn on FlashLight when a photo is taken
  // This is a simple example, you can add more complex logic as needed

  // Simulate taking a photo
  Serial.println("Taking a photo...");
  delay(1000); // Simulate delay for taking a photo

  // Turn on FlashLight
  Serial.println("Turning on FlashLight...");
  digitalWrite(FLASHLIGHT_PIN, HIGH);
  delay(500); // Keep FlashLight on for 500ms

  // Turn off FlashLight
  Serial.println("Turning off FlashLight...");
  digitalWrite(FLASHLIGHT_PIN, LOW);

  // Wait before taking the next photo
  delay(5000); // Adjust delay as needed
}
