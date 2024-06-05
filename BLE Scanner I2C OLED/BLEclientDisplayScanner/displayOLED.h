#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// Menu variables
int currentMenuIndex = 0;
int menuItemCount;
//static const unsigned char PROGMEM image_arrow_right_bits[] = {0x08,0x04,0xfe,0x04,0x08};


void showMenu() {
  menuItemCount = deviceCount - 1;  // it menuitem

  display.clearDisplay();
  for (int i = 0; i < menuItemCount; i++) {
    if (i == currentMenuIndex) {
      display.setCursor(10, i * 15);
      display.print(">");
      //display.drawBitmap(10, i*15, image_arrow_right_bits, 7, 5, 0xFFFF);
      //display.setTextSize();
      //display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Highlighted text
    } else {
      //display.setTextColor(SSD1306_WHITE);
      //display.setTextSize(1);
    }

    display.setCursor(25, i * 15);
    display.print(i + 1);
    display.print(": ");
    display.println(deviceNames[i]);
    //Serial.println(deviceNames[i]);
  }
  display.display();
}

void executeMenuAction() {



  int selectedDevice = currentMenuIndex;  // user input is stored here.

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

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Selected: ");
  display.println(deviceNames[currentMenuIndex]);
  display.display();
  delay(4000);  // Wait for a second before returning to the menu
  showMenu();
}