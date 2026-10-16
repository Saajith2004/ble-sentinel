#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE Sentinel v1.1

#define DEVICE_NAME "BLE-SENTINEL"

#define SERVICE_UUID \
  "7b7e0001-7a5d-4c4a-9b8a-112233445566"

#define STATUS_UUID \
  "7b7e0002-7a5d-4c4a-9b8a-112233445566"

BLEServer* bleServer = nullptr;
BLECharacteristic* statusCharacteristic = nullptr;

bool deviceConnected = false;

unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 2000;

// BLE connection callbacks
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
  Serial.println("    BLE SENTINEL v1.1");
  Serial.println("==============================");

  // Initialize BLE
  BLEDevice::init(DEVICE_NAME);

  // Create BLE server
  bleServer = BLEDevice::createServer();

  bleServer->setCallbacks(
    new SentinelServerCallbacks()
  );

  // Create custom GATT service
  BLEService* service =
    bleServer->createService(SERVICE_UUID);

  // Create readable and notifiable characteristic
  statusCharacteristic =
    service->createCharacteristic(
      STATUS_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_NOTIFY
    );

  // Add notification descriptor
  statusCharacteristic->addDescriptor(
    new BLE2902()
  );

  // Set initial status
  statusCharacteristic->setValue(
    "BLE Sentinel v1.1 | Status: ONLINE"
  );

  // Start service
  service->start();

  // Configure advertising
  BLEAdvertising* advertising =
    BLEDevice::getAdvertising();

  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);

  BLEDevice::startAdvertising();

  Serial.println("BLE initialized successfully.");
  Serial.println("Device name: BLE-SENTINEL");
  Serial.println("Notifications enabled in firmware.");
  Serial.println("Advertising started.");
}

void loop() {

  unsigned long currentTime = millis();

  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {

    lastUpdate = currentTime;

    // Generate updated device information
    String status =
      "BLE Sentinel v1.1 | Uptime: " +
      String(currentTime / 1000) +
      "s | Free Heap: " +
      String(ESP.getFreeHeap()) +
      " bytes";

    // Update the characteristic
    statusCharacteristic->setValue(status.c_str());

    Serial.println(status);

    // Send update when a client is connected
    if (deviceConnected) {

      statusCharacteristic->notify();

      Serial.println("Notification sent.");
    }
  }

  delay(10);
}
