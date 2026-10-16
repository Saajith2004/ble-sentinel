/*
 * BLE Sentinel v1.6
 * BLE Device Scanner
 *
 * Platform: ESP32-WROOM-32U
 * Board: ESP32 Dev Module
 *
 * Scans for nearby Bluetooth Low Energy advertisements
 * and reports device names, addresses, and RSSI values
 * when available.
 */

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan* pBLEScan;

const int SCAN_TIME_SECONDS = 5;

class SentinelAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {

    Serial.println("----------------------------------------");

    Serial.print("Device Address: ");
    Serial.println(advertisedDevice.getAddress().toString().c_str());

    Serial.print("RSSI: ");
    Serial.print(advertisedDevice.getRSSI());
    Serial.println(" dBm");

    if (advertisedDevice.haveName()) {
      Serial.print("Device Name: ");
      Serial.println(advertisedDevice.getName().c_str());
    } else {
      Serial.println("Device Name: Not advertised");
    }

    if (advertisedDevice.haveServiceUUID()) {
      Serial.print("Service UUID: ");
      Serial.println(advertisedDevice.getServiceUUID().toString().c_str());
    }

    Serial.println("----------------------------------------");
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("========================================");
  Serial.println(" BLE SENTINEL v1.6");
  Serial.println(" BLE Device Scanner");
  Serial.println("========================================");

  BLEDevice::init("BLE-SENTINEL-SCANNER");

  pBLEScan = BLEDevice::getScan();

  pBLEScan->setAdvertisedDeviceCallbacks(
    new SentinelAdvertisedDeviceCallbacks()
  );

  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);

  Serial.println("Scanner initialised successfully.");
}

void loop() {
  Serial.println();
  Serial.println("Do you want to scan for BLE devices? (y/n):");

  // Wait for the user's choice
  while (Serial.available() == 0) {
    delay(50);
  }

  char choice = Serial.read();

  // Clear remaining characters from the input line
  while (Serial.available() > 0) {
    Serial.read();
  }

  if (choice == 'y' || choice == 'Y') {

    Serial.println();
    Serial.println("Starting BLE scan...");
    Serial.println();

    BLEScanResults* foundDevices =
      pBLEScan->start(SCAN_TIME_SECONDS, false);

    Serial.println();
    Serial.println("Scan complete.");

    Serial.print("Devices detected: ");
    Serial.println(foundDevices->getCount());

    Serial.println();
    Serial.println("Scan finished. No automatic rescanning.");

  } else if (choice == 'n' || choice == 'N') {

    Serial.println("Scan not started.");

  } else {

    Serial.println("Invalid choice. Enter y or n.");

  }
}
