#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE Sentinel configuration
#define DEVICE_NAME "BLE-SENTINEL"

#define SERVICE_UUID \
  "7b7e0001-7a5d-4c4a-9b8a-112233445566"

#define STATUS_UUID \
  "7b7e0002-7a5d-4c4a-9b8a-112233445566"

BLEServer* bleServer = nullptr;
BLECharacteristic* statusCharacteristic = nullptr;

bool deviceConnected = false;

class SentinelServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* server) override {
    deviceConnected = true;
    Serial.println("Client connected.");
  }

  void onDisconnect(BLEServer* server) override {
    deviceConnected = false;
    Serial.println("Client disconnected.");

    BLEDevice::startAdvertising();
    Serial.println("Advertising restarted.");
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("       BLE SENTINEL");
  Serial.println("==============================");

  // Initialize BLE
  BLEDevice::init(DEVICE_NAME);

  // Create BLE server
  bleServer = BLEDevice::createServer();

  bleServer->setCallbacks(
    new SentinelServerCallbacks()
  );

  // Create a custom service
  BLEService* service =
    bleServer->createService(SERVICE_UUID);

  // Create a readable status characteristic
  statusCharacteristic =
    service->createCharacteristic(
      STATUS_UUID,
      BLECharacteristic::PROPERTY_READ
    );

  statusCharacteristic->setValue(
    "BLE Sentinel v1.0 | Status: ONLINE"
  );

  // Start the service
  service->start();

  // Configure advertising
  BLEAdvertising* advertising =
    BLEDevice::getAdvertising();

  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);

  // Start broadcasting
  BLEDevice::startAdvertising();

  Serial.println("BLE initialized successfully.");
  Serial.print("Device name: ");
  Serial.println(DEVICE_NAME);

  Serial.println("Service UUID:");
  Serial.println(SERVICE_UUID);

  Serial.println("Advertising started.");
}

void loop() {
  delay(1000);
}
