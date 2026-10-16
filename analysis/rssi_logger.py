# BLE Sentinel v1.5
# RSSI Data Logger
# Records manually entered BLE signal measurements in a CSV file.

import csv
import os
from datetime import datetime

CSV_FILE = "rssi_measurements.csv"


def classify_rssi(rssi):
    """Classify RSSI using experimental thresholds."""

    if rssi >= -70:
        return "STRONG"
    elif rssi >= -85:
        return "MODERATE"
    else:
        return "WEAK"


def save_measurement(rssi):
    """Save an RSSI measurement to a CSV file."""

    file_exists = os.path.isfile(CSV_FILE)

    with open(CSV_FILE, "a", newline="", encoding="utf-8") as file:
        writer = csv.writer(file)

        if not file_exists or os.path.getsize(CSV_FILE) == 0:
            writer.writerow([
                "Timestamp",
                "RSSI_dBm",
                "Classification"
            ])

        writer.writerow([
            datetime.now().astimezone().isoformat(timespec="seconds"),
            rssi,
            classify_rssi(rssi)
        ])


def main():
    print("=" * 45)
    print(" BLE SENTINEL v1.5")
    print(" RSSI Data Logger")
    print("=" * 45)

    print("\nEnter RSSI readings measured using your BLE scanner.")
    print("Example: -72")
    print("Enter 'q' to quit.")

    while True:
        user_input = input("\nRSSI (dBm): ").strip()

        if user_input.lower() == "q":
            print("\nData logging finished.")
            print(f"Measurements saved in: {CSV_FILE}")
            break

        try:
            rssi = float(user_input)

            if not -150 <= rssi <= 0:
                print("Invalid RSSI. Enter a value from -150 to 0 dBm.")
                continue

            save_measurement(rssi)

            print(f"Saved: {rssi:g} dBm")
            print(f"Classification: {classify_rssi(rssi)}")

        except ValueError:
            print("Invalid input. Enter a number or 'q'.")


if __name__ == "__main__":
    main()
