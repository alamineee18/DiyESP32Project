#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

// Replace with your network credentials
const char* ssid = "Sagor";
const char* password = "xyzpqr1990";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// GPIO where the LED is connected
const int ledPin = 2;

// Variable to store the state of the LED
bool ledState = false;

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

  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  initWiFi();
  initLittleFS();

  // Route to load the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  // Route to control LED with POST
  server.on("/led", HTTP_POST, [](AsyncWebServerRequest* request) {
    String inputMessage;
    if (request->hasParam("state", true)) {
      inputMessage = request->getParam("state", true)->value();
      if (inputMessage == "on") {
        digitalWrite(ledPin, HIGH);
        ledState = true;
      } else if (inputMessage == "off") {
        digitalWrite(ledPin, LOW);
        ledState = false;
      }
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Bad Request");
    }
  });

  // Start server
  server.begin();
}

void loop() {
  // Nothing to do here - everything is handled asynchronously
}
