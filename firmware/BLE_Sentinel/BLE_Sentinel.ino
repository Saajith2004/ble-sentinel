#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE Sentinel v1.2
// ESP32 BLE connection-duration monitoring

#define DEVICE_NAME "BLE-SENTINEL"

#define SERVICE_UUID \
  "7b7e0001-7a5d-4c4a-9b8a-112233445566"

#define STATUS_UUID \
  "7b7e0002-7a5d-4c4a-9b8a-112233445566"

BLEServer* bleServer = nullptr;
BLECharacteristic* statusCharacteristic = nullptr;

bool deviceConnected = false;

unsigned long connectionStartTime = 0;
unsigned long lastUpdate = 0;

const unsigned long UPDATE_INTERVAL = 2000;

// BLE connection callbacks
class SentinelServerCallbacks : public BLEServerCallbacks {

  void onConnect(BLEServer* server) override {
    deviceConnected = true;

    // Record the start of this connection session
    connectionStartTime = millis();

    Serial.println("Client connected.");
    Serial.println("Connection timer started.");
  }

  void onDisconnect(BLEServer* server) override {
    deviceConnected = false;

    Serial.println("Client disconnected.");
    Serial.println("Connection session ended.");

    BLEDevice::startAdvertising();

    Serial.println("Advertising restarted.");
  }
};

// Format elapsed seconds as HH:MM:SS
String formatDuration(unsigned long totalSeconds) {
  unsigned long hours = totalSeconds / 3600;
  unsigned long minutes = (totalSeconds % 3600) / 60;
  unsigned long seconds = totalSeconds % 60;

  char buffer[16];

  snprintf(
    buffer,
    sizeof(buffer),
    "%02lu:%02lu:%02lu",
    hours,
    minutes,
    seconds
  );

  return String(buffer);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("    BLE SENTINEL v1.2");
  Serial.println("==============================");

  // Initialize BLE
  BLEDevice::init(DEVICE_NAME);

  // Create BLE server
  bleServer = BLEDevice::createServer();
  bleServer->setCallbacks(new SentinelServerCallbacks());

  // Create GATT service
  BLEService* service =
    bleServer->createService(SERVICE_UUID);

  // Create readable and notifiable characteristic
  statusCharacteristic = service->createCharacteristic(
    STATUS_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  // Enable notification subscription support
  statusCharacteristic->addDescriptor(new BLE2902());

  statusCharacteristic->setValue(
    "BLE Sentinel v1.2 | Status: DISCONNECTED"
  );

  service->start();

  // Configure BLE advertising
  BLEAdvertising* advertising = BLEDevice::getAdvertising();

  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);

  BLEDevice::startAdvertising();

  Serial.println("BLE initialized successfully.");
  Serial.println("Device name: BLE-SENTINEL");
  Serial.println("Advertising started.");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = currentTime;

    unsigned long uptimeSeconds = currentTime / 1000;
    unsigned long connectionSeconds = 0;

    if (deviceConnected) {
      connectionSeconds =
        (currentTime - connectionStartTime) / 1000;
    }

    String status;

    if (deviceConnected) {
      status =
        "BLE Sentinel v1.2 | Status: CONNECTED"
        " | Connection: " +
        formatDuration(connectionSeconds);
    } else {
      status =
        "BLE Sentinel v1.2 | Status: DISCONNECTED";
    }

    status +=
      " | Uptime: " + String(uptimeSeconds) +
      "s | Free Heap: " +
      String(ESP.getFreeHeap()) + " bytes";

    // Update the GATT characteristic
    statusCharacteristic->setValue(status.c_str());

    // Print the current status to Serial Monitor
    Serial.println(status);

    // Send a notification if the client is connected
    if (deviceConnected) {
      statusCharacteristic->notify();
      Serial.println("Notification sent.");
    }
  }

  delay(10);
}
