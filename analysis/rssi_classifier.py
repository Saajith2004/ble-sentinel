# BLE Sentinel v1.4
# Interactive BLE RSSI Signal Classifier

def classify_rssi(rssi):
    """Classify BLE signal strength using experimental thresholds."""

    if rssi >= -70:
        return "STRONG"
    elif rssi >= -85:
        return "MODERATE"
    else:
        return "WEAK"


def main():
    print("=" * 45)
    print(" BLE SENTINEL v1.4")
    print(" Interactive RSSI Signal Classifier")
    print("=" * 45)

    print("\nSignal thresholds:")
    print("Strong:   -70 dBm or higher")
    print("Moderate: -85 to below -70 dBm")
    print("Weak:     below -85 dBm")

    while True:
        user_input = input(
            "\nEnter RSSI in dBm (or 'q' to quit): "
        ).strip()

        if user_input.lower() == "q":
            print("\nBLE Sentinel classifier closed.")
            break

        try:
            rssi = float(user_input)

            if not -150 <= rssi <= 0:
                print("Invalid RSSI. Enter a value from -150 to 0 dBm.")
                continue

            classification = classify_rssi(rssi)

            print("\n--- Classification Result ---")
            print(f"RSSI: {rssi:g} dBm")
            print(f"Signal Strength: {classification}")

            if classification == "STRONG":
                print("Interpretation: Strong received signal.")
            elif classification == "MODERATE":
                print("Interpretation: Moderate received signal.")
            else:
                print("Interpretation: Weak received signal.")

            print(
                "\nNote: RSSI is affected by distance, "
                "obstacles, interference, and antenna orientation."
            )

        except ValueError:
            print("Invalid input. Enter a number, such as -72, or 'q'.")

if __name__ == "__main__":
    main()
