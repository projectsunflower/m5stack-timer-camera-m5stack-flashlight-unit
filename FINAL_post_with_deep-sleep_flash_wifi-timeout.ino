#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include "M5TimerCAM.h"

// WiFi credentials
const char* ssid = "your wifi ssid";
const char* password = "your wifi password";

// Server details
const char* SERVER = "test.org";
const int SERVER_PORT = 3000; // change to your server port

// Flash pin
#define FLASH_PIN 4

WiFiClient wifi;
HttpClient client = HttpClient(wifi, SERVER, SERVER_PORT);

// Deep sleep duration in seconds
#define TEN_SECONDS (1 * 10)
#define ONE_MINUTE (1 * 60)
#define ONE_HOUR (60 * 60)
#define TWENTY_FOUR_HOURS (24 * 60 * 60)
unsigned long sleepDuration = ONE_HOUR;

const unsigned long wifiTimeout = 30000; // WiFi connection timeout in milliseconds (30 seconds)

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.setSleep(false); // Disable WiFi sleep mode
    Serial.print("Connecting to ");
    Serial.println(ssid);

    unsigned long startAttemptTime = millis();
    
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nFailed to connect to WiFi within the timeout period");
        prepareForDeepSleep();
        return;
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Set up flash pin
    pinMode(FLASH_PIN, OUTPUT);
    digitalWrite(FLASH_PIN, LOW); // Ensure flash is off initially

    // Activate flash before capturing image
    digitalWrite(FLASH_PIN, HIGH);

    // Initialize TimerCAM
    TimerCAM.begin();
    if (!TimerCAM.Camera.begin()) {
        Serial.println("Camera Init Fail");
        return;
    }
    Serial.println("Camera Init Success");

    TimerCAM.Camera.sensor->set_pixformat(TimerCAM.Camera.sensor, PIXFORMAT_JPEG);
    TimerCAM.Camera.sensor->set_framesize(TimerCAM.Camera.sensor, FRAMESIZE_QVGA);
    TimerCAM.Camera.sensor->set_vflip(TimerCAM.Camera.sensor, 1);
    TimerCAM.Camera.sensor->set_hmirror(TimerCAM.Camera.sensor, 0);

    // Capture image
    if (TimerCAM.Camera.get()) {
        Serial.println("Image captured");

        // Print image information
        Serial.println("Image data size: " + String(TimerCAM.Camera.fb->len));
        Serial.println("Image width: " + String(TimerCAM.Camera.fb->width));
        Serial.println("Image height: " + String(TimerCAM.Camera.fb->height));

        // Deactivate flash after capturing image
        digitalWrite(FLASH_PIN, LOW);

        // Prepare HTTP POST request
        String endpoint = "/upload";
        String contentType = "multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW";

        // Construct HTTP POST body (multipart/form-data)
        String postData = "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
        postData += "Content-Disposition: form-data; name=\"image\"; filename=\"image.jpg\"\r\n";
        postData += "Content-Type: image/jpeg\r\n\r\n";
        postData += String((char*)TimerCAM.Camera.fb->buf, TimerCAM.Camera.fb->len);
        postData += "\r\n";
        postData += "------WebKitFormBoundary7MA4YWxkTrZu0gW--\r\n";

        // Make HTTP POST request
        client.post(endpoint.c_str(), contentType.c_str(), postData.length(), (const byte*)postData.c_str());

        // Check response
        int statusCode = client.responseStatusCode();
        String response = client.responseBody();

        Serial.print("HTTP status code: ");
        Serial.println(statusCode);
        Serial.print("Server response: ");
        Serial.println(response);

        // Free camera memory
        TimerCAM.Camera.free();
    } else {
        Serial.println("Failed to capture image");
        digitalWrite(FLASH_PIN, LOW); // Ensure flash is off if capture fails
    }

    // Optionally, you can add code here to change the sleepDuration based on certain conditions
    // Example: if a certain button is pressed, set sleepDuration to TWENTY_FOUR_HOURS

    // Prepare for deep sleep
    Serial.println("Preparing to enter deep sleep...");
    prepareForDeepSleep();
}

void prepareForDeepSleep() {
    esp_sleep_enable_timer_wakeup(sleepDuration * 1000000); // Convert to microseconds
    esp_deep_sleep_start();
}

void loop() {
    // Empty loop as execution ends in deep sleep
}
