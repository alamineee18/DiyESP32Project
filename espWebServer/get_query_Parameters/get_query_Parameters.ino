#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "GizanTech_network";
const char* password = "#Analysis23";


AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  // Example: /get?message=Hello
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
    String message;
    if (request->hasParam("message")) {
      message = request->getParam("message")->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Message: " + message);
  });

  server.begin();
}

void loop() {
}
