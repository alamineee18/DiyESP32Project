#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


BLECharacteristic *pCharacteristic;  // This line declares a pointer to a BLECharacteristic object.
bool deviceConnected = false;

// Server callbacks
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    Serial.println("Clint Connected");
    deviceConnected = true;
    // pCharacteristic->setValue("From Server");
    // pCharacteristic->notify();
  }

  void onDisconnect(BLEServer *pServer) {
    Serial.print("Clint Disonnected");
    deviceConnected = false;
    pServer->getAdvertising()->start();
  }
};


class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue().c_str();
    if (value.length() > 0) {
      Serial.print("Recieved Data from Client: ");
      for (int i = 0; i < value.length(); i++)  //write data from app here readed
        Serial.print(value[i]);

      Serial.println();
    }
  }

  void onRead(BLECharacteristic *pCharacteristic) {
    Serial.println("Characteristic was read");
  }
};


void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("BLE Clock");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | 
                                                                        BLECharacteristic::PROPERTY_WRITE | 
                                                                        BLECharacteristic::PROPERTY_NOTIFY);

  // Set characteristic callbacks
  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify...");

}

void loop() {
  if (deviceConnected) {
    pCharacteristic->setValue(("Server 2: " + String(millis())).c_str());
    pCharacteristic->notify();

    Serial.println("Data send to client");
  }
  delay(2000); 
}
