#include "BLEmanagement.h"
#include "displayOLED.h"
#include "ButtonFunction.h"


void setup() {
  //Start serial communication
  Serial.begin(115200);

  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  Serial.println("Display Begin");


  //Initialize the button to input
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_ENTER, INPUT_PULLUP);

  //Call interrupt when button will press
  attachInterrupt(digitalPinToInterrupt(BUTTON_UP), handleUpButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_DOWN), handleDownButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_ENTER), handleEnterButtonPress, FALLING);

  Serial.println("Searching BLE server...");
  BLEDevice::init("");  //Init BLE device
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10);

  Serial.print("Devices found: ");

  Serial.println("Enter the number of the device you want to connect to:");
  
  showMenu();
}

void loop() {
  if (Serial.available()) {
    int selectedDevice = Serial.parseInt();  // user input is stored here.

    if (selectedDevice >= 0 && selectedDevice < deviceCount) {
      Serial.print("Connecting to ");
      Serial.println(devices[selectedDevice]->getName().c_str());

      // Connect to the selected device
      BLEClient* pClient = BLEDevice::createClient();
      pClient->connect(devices[selectedDevice]);

      if (pClient->isConnected()) {
        Serial.println("Successfully connected to the device!");

        // Go to read data function to read the
        readData(pClient);

      } else {
        Serial.println("Failed to connect to the device.");
      }
    } else {
      Serial.println("Invalid device number. Please enter a valid number.");
    }
  }
  if (BUTTON_UP_Pressed) {
    BUTTON_UP_Pressed = false;             // Reset the flag
    Serial.println("Up Button pressed!");  // Debug message
    // Add additional logic to handle the button press
    currentMenuIndex--;
    if (currentMenuIndex < 0) {
      currentMenuIndex = menuItemCount - 1;
    }

    showMenu();
  }


  if (BUTTON_DOWN_Pressed) {
    BUTTON_DOWN_Pressed = false;             // Reset the flag
    Serial.println("Down Button pressed!");  // Debug message
    // Add additional logic to handle the button press
    currentMenuIndex++; 
    if (currentMenuIndex >= menuItemCount) {
      currentMenuIndex = 0;
    }
    showMenu();
  }


  if (BUTTON_ENTER_Pressed) {
    BUTTON_ENTER_Pressed = false;             // Reset the flag
    Serial.println("Enter Button pressed!");  // Debug message
    // Add additional logic to handle the button press

    executeMenuAction();
  }
}
