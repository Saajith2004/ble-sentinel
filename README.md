# BLE Sentinel

### ESP32-Based Bluetooth Low Energy Proximity Analysis & Security Lab

BLE Sentinel is an embedded-systems project exploring Bluetooth Low Energy
(BLE), wireless device discovery, GATT communication, and signal-strength
analysis using an ESP32 and an iPhone.

## Project Goals

- Implement a custom BLE beacon using an ESP32.
- Create a custom GATT service and characteristic.
- Discover and connect to the device using an iPhone.
- Investigate RSSI-based signal-strength measurements.
- Analyse experimental data using Python.
- Explore BLE security and access-control mechanisms.

## Hardware

- ESP32-WROOM-32U development board
- MacBook Air running macOS
- iPhone
- USB data cable

## Technologies

- Arduino C++
- ESP32 Arduino Core
- Bluetooth Low Energy (BLE)
- Generic Attribute Profile (GATT)
- nRF Connect for Mobile
- Git and GitHub
- Python (planned)

## Current Implementation

### BLE Beacon

Device name: `BLE-SENTINEL`

### Custom GATT Service

Service UUID:

`7b7e0001-7a5d-4c4a-9b8a-112233445566`

Characteristic UUID:

`7b7e0002-7a5d-4c4a-9b8a-112233445566`

Characteristic value:

`BLE Sentinel v1.0 | Status: ONLINE`
## Hardware Test Evidence

The ESP32 successfully advertised its BLE service and established
a connection with an iPhone using nRF Connect for Mobile.

The custom GATT characteristic was read successfully, returning
the expected status message.

![BLE Sentinel connected to iPhone](images/iphone-ble-test.png)

## v1.1 — Live BLE Notifications

BLE Sentinel v1.1 adds live notifications to the custom GATT
characteristic.

The ESP32 periodically reports:
- Device uptime
- Available heap memory

Notifications were enabled and tested using nRF Connect
for Mobile on an iPhone.

### Notification Test Evidence

![BLE Sentinel v1.1 Notifications](images/ble-sentinel-v1.1-notifications.png)

## Verified Results

- [x] ESP32 BLE initialization
- [x] BLE advertising
- [x] iPhone device discovery
- [x] GATT service discovery
- [x] Successful GATT connection
- [x] Reading the custom characteristic

## Development Roadmap

- [x] Initial BLE beacon
- [x] Custom GATT service
- [x] Initial iPhone communication test
- [ ] Live BLE status notifications
- [ ] RSSI experiments
- [ ] Python-based data analysis
- [ ] BLE security testing
- [ ] Technical documentation

## Security and Privacy

This project is intended for controlled BLE experimentation and
authorized security testing.

RSSI alone is not a reliable way to verify a device's identity or
physical proximity.

## License

This project is licensed under the MIT License.
See the `LICENSE` file for details.
