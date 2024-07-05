#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "Sagor";
const char* password = "xyzpqr1990";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// GPIO where the LED is connected
const int ledPin = 2;

// Variable to store the state of the LED
bool ledState = false;

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 LED Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html { font-family: Arial; display: inline-block; text-align: center; }
    h1 { font-size: 2.0rem; }
    .switch { position: relative; display: inline-block; width: 120px; height: 68px }
    .switch input { display: none; }
    .slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; transition: .4s; border-radius: 34px; }
    .slider:before { position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: white; transition: .4s; border-radius: 50%; }
    input:checked + .slider { background-color: #2196F3; }
    input:checked + .slider:before { transform: translateX(52px); }
  </style>
</head>
<body>
  <h1>ESP32 LED Control</h1>
  <label class="switch">
    <input type="checkbox" onchange="toggleCheckbox(this)" id="ledSwitch">
    <span class="slider"></span>
  </label>
  <script>
    function toggleCheckbox(element) {
      var xhr = new XMLHttpRequest();
      if (element.checked) {
        xhr.open("GET", "/led?state=on", true);
      } else {
        xhr.open("GET", "/led?state=off", true);
      }
      xhr.send();
    }
    window.onload = function() {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/ledState", true);
      xhr.onload = function() {
        var ledState = xhr.responseText;
        document.getElementById("ledSwitch").checked = (ledState === "on");
      }
      xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";

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
  digitalWrite(ledPin, LOW);


  initWiFi();

  // Route to load the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Route to control LED
  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request){
    String inputMessage;
    if (request->hasParam("state")) {
      inputMessage = request->getParam("state")->value();
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

  // Route to get the current LED state
  server.on("/ledState", HTTP_GET, [](AsyncWebServerRequest *request){
    if (ledState) {
      request->send(200, "text/plain", "on");
    } else {
      request->send(200, "text/plain", "off");
    }
  });

  // Start server
  server.begin();
}

void loop() {
  // Nothing to do here - everything is handled asynchronously
}

