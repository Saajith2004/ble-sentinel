/*
 * BLE Sentinel v1.7
 * BLE Device Inventory
 *
 * Board: ESP32 Dev Module
 * Hardware: ESP32-WROOM-32U
 *
 * Features:
 * - On-demand BLE scanning
 * - Device address and advertised name
 * - Latest RSSI per address
 * - Advertisement count per address
 * - Scan summary
 */

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan* pBLEScan;

const int SCAN_TIME_SECONDS = 5;
const int MAX_DEVICES = 100;

struct DeviceRecord {
  String address;
  String name;
  int latestRSSI;
  int advertisementCount;
};

DeviceRecord devices[MAX_DEVICES];

int uniqueDeviceCount = 0;
int totalAdvertisements = 0;

class InventoryCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {

    String address =
      advertisedDevice.getAddress().toString().c_str();

    String name = "";

    if (advertisedDevice.haveName()) {
      name = advertisedDevice.getName().c_str();
    }

    int rssi = advertisedDevice.getRSSI();

    totalAdvertisements++;

    // Check whether this address is already in the inventory.
    for (int i = 0; i < uniqueDeviceCount; i++) {

      if (devices[i].address == address) {

        devices[i].latestRSSI = rssi;
        devices[i].advertisementCount++;

        if (name.length() > 0) {
          devices[i].name = name;
        }

        return;
      }
    }

    // Add a new address if inventory capacity remains.
    if (uniqueDeviceCount < MAX_DEVICES) {

      devices[uniqueDeviceCount].address = address;
      devices[uniqueDeviceCount].name = name;
      devices[uniqueDeviceCount].latestRSSI = rssi;
      devices[uniqueDeviceCount].advertisementCount = 1;

      uniqueDeviceCount++;
    }
  }
};

void resetInventory() {
  uniqueDeviceCount = 0;
  totalAdvertisements = 0;
}

void printInventory() {

  Serial.println();
  Serial.println("========================================");
  Serial.println(" BLE SENTINEL v1.7 - SCAN SUMMARY");
  Serial.println("========================================");

  for (int i = 0; i < uniqueDeviceCount; i++) {

    Serial.println();

    Serial.print("Address: ");
    Serial.println(devices[i].address);

    Serial.print("Name: ");

    if (devices[i].name.length() > 0) {
      Serial.println(devices[i].name);
    } else {
      Serial.println("Not advertised");
    }

    Serial.print("Latest RSSI: ");
    Serial.print(devices[i].latestRSSI);
    Serial.println(" dBm");

    Serial.print("Advertisements received: ");
    Serial.println(devices[i].advertisementCount);
  }

  Serial.println();
  Serial.println("----------------------------------------");

  Serial.print("Unique addresses observed: ");
  Serial.println(uniqueDeviceCount);

  Serial.print("Total advertisements received: ");
  Serial.println(totalAdvertisements);

  Serial.println("========================================");
}

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("========================================");
  Serial.println(" BLE SENTINEL v1.7");
  Serial.println(" BLE DEVICE INVENTORY");
  Serial.println("========================================");

  BLEDevice::init("BLE-SENTINEL-INVENTORY");

  pBLEScan = BLEDevice::getScan();

  pBLEScan->setAdvertisedDeviceCallbacks(
    new InventoryCallbacks()
  );

  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);

  Serial.println("Inventory scanner initialised.");
}

void loop() {

  Serial.println();
  Serial.println("Do you want to scan for BLE devices? (y/n):");

  while (Serial.available() == 0) {
    delay(50);
  }

  char choice = Serial.read();

  while (Serial.available() > 0) {
    Serial.read();
  }

  if (choice == 'y' || choice == 'Y') {

    resetInventory();

    Serial.println();
    Serial.println("Scanning for 5 seconds...");

    BLEScanResults* results =
      pBLEScan->start(SCAN_TIME_SECONDS, false);

    Serial.print("Scan finished. Advertisements reported by scan: ");
    Serial.println(results->getCount());

    printInventory();

    Serial.println();
    Serial.println("Enter y to scan again or n to wait.");

  } else if (choice == 'n' || choice == 'N') {

    Serial.println("Scan not started.");

  } else {

    Serial.println("Invalid choice. Enter y or n.");

  }
}
