#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <BLEUUID.h>

static BLEUUID ServiceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID pCharacteristicUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

//Flags stating if should begin connecting and if the connection is up
static boolean doConnect = false;
static boolean connected = false;
//Characteristicd that we want to read
static BLERemoteCharacteristic* pCharacteristic;


BLEScan* pBLEScan;
BLEAdvertisedDevice* devices[10];  // Store up to 10 devices

const char* deviceNames[10];
int deviceCount = 0;
int totalDevice;

#define NAME_MAX_LENGTH 100  // Maximum length of a device name


//Callback function that gets called, when another device's advertisement has been received
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (deviceCount < 5) {
      devices[deviceCount] = new BLEAdvertisedDevice(advertisedDevice);
      
      if (advertisedDevice.haveName()) {
        char* deviceName = new char[NAME_MAX_LENGTH];
        // Copy the name to the allocated memory
        strncpy(deviceName, advertisedDevice.getName().c_str(), NAME_MAX_LENGTH);
        deviceName[NAME_MAX_LENGTH - 1] = '\0';  // Ensure null termination
        deviceNames[deviceCount] = deviceName;

        Serial.print(deviceCount);
        Serial.print(": ");
        Serial.println(deviceNames[deviceCount]);  // Print the device name as it is discovere
      } else {
        BLEDevice::getScan()->stop();
        Serial.println("Scanning end.");
      }
      deviceCount++;
      Serial.print("The device count value:");
      Serial.println(deviceCount);
    }
  }
};

// //When the BLE Server sends a new character or data reading with the notify property
static void pNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  Serial.print("Recieved data from server: ");
  std::string data((char*)pData, length);
  Serial.println(data.c_str());
}

bool readData(BLEClient* pClient) {
  BLERemoteService* pRemoteService = pClient->getService(ServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(ServiceUUID.toString().c_str());
    return (false);
  } else {
    Serial.println("Service found");
  }

  //To find Characteristic
  pCharacteristic = pRemoteService->getCharacteristic(pCharacteristicUUID);

  if (pCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID");
    return false;
  }
  Serial.println("Found our characteristics");

  if (pCharacteristic->canNotify())
    pCharacteristic->registerForNotify(pNotifyCallback);

  pCharacteristic->writeValue("Connected Sucessufully");

  return true;
}
