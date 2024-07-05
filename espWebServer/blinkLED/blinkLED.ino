#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "Sagor";
const char* password = "xyzpqr1990";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


const int ledPin = 2; // GPIO where the LED is connected
bool ledState = false; // Variable to store the state of the LED

void initWiFi(){
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

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  initWiFi();

  

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello, world");
  });

  // Route to control LED
  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request){
    
    String inputMessage;

    if (request->hasParam("state")) {
      inputMessage = request->getParam("state")->value();

      if (inputMessage == "on") {
        digitalWrite(ledPin, HIGH);
        Serial.println("Turn on LED");
        ledState = true;
      } else if (inputMessage == "off") {
        digitalWrite(ledPin, LOW);
        ledState = false;
      }
      request->send(200, "text/plain", "LED state updated");
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
