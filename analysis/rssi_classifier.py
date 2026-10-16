import pandas as pd


def classify_rssi(rssi):
    """Classify BLE signal strength using experimental thresholds."""

    if rssi >= -70:
        return "Strong"
    elif rssi >= -85:
        return "Moderate"
    else:
        return "Weak"


# BLE Sentinel v1.4 - RSSI Signal Classification

data = {
    "Distance_m": [0.5, 1, 2, 3, 4, 5],
    "Average_RSSI_dBm": [-68, -83, -86, -90, -96, -99],
}

df = pd.DataFrame(data)

df["Signal_Classification"] = df["Average_RSSI_dBm"].apply(
    classify_rssi
)

print("BLE Sentinel v1.4 - RSSI Signal Classification")
print("=" * 55)

print(df.to_string(index=False))

print("\nClassification Summary")
print("=" * 55)

summary = df["Signal_Classification"].value_counts()

for category in ["Strong", "Moderate", "Weak"]:
    count = summary.get(category, 0)
    print(f"{category}: {count} measurement(s)")

print("\nNote:")
print("These thresholds are experimental, not universal BLE standards.")
print("RSSI alone cannot reliably determine the distance to a device.")
