#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include <AccelStepper.h>

// Replace with your network credentials
const char* ssid = "Sagor";
const char* password = "xyzpqr1990";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Define the stepper motor and the pins it uses
#define STEP_PIN 26
#define DIR_PIN 25
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Motor state
bool motorOn = false;



void initWiFi() {

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}


void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize the stepper motor
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(50);

  initWiFi();
  initLittleFS();

  // Route to load the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });

  // Route to control the motor
  server.on("/motor", HTTP_POST, [](AsyncWebServerRequest *request){
    String state;
    if (request->hasParam("state", true)) {
      state = request->getParam("state", true)->value();
      if (state == "on") {
        motorOn = true;
        stepper.moveTo(1000000);  // Move indefinitely
      } else if (state == "off") {
        motorOn = false;
        stepper.stop();
      }
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Bad Request");
    }
  });

  // Route to set motor speed
  server.on("/speed", HTTP_POST, [](AsyncWebServerRequest *request){
    String speed;
    if (request->hasParam("speed", true)) {
      speed = request->getParam("speed", true)->value();
      stepper.setMaxSpeed(speed.toInt());
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Bad Request");
    }
  });

  // Route to set motor acceleration
  server.on("/acceleration", HTTP_POST, [](AsyncWebServerRequest *request){
    String acceleration;
    if (request->hasParam("acceleration", true)) {
      acceleration = request->getParam("acceleration", true)->value();
      stepper.setAcceleration(acceleration.toInt());
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Bad Request");
    }
  });

  // Start server
  server.begin();
}

void loop() {
  if (motorOn) {
    stepper.run();
  }
}
