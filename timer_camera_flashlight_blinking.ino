// Define GPIO pin for FlashLight control
#define FLASHLIGHT_PIN 4

void setup() {
    // Initialize FlashLight pin
    pinMode(FLASHLIGHT_PIN, OUTPUT);
}

void loop() {
    // Blink the flashlight
    digitalWrite(FLASHLIGHT_PIN, HIGH); // Turn on FlashLight
    delay(500);                         // Wait for 500 milliseconds
    digitalWrite(FLASHLIGHT_PIN, LOW);  // Turn off FlashLight
    delay(500);                         // Wait for 500 milliseconds
}
