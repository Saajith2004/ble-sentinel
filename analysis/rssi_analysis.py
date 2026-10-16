import matplotlib.pyplot as plt
import pandas as pd

# BLE Sentinel v1.3
# RSSI distance experiment

data = {
    "Distance_m": [0.5, 1, 2, 3, 4, 5],
    "Average_RSSI_dBm": [-68, -83, -86, -90, -96, -99],
}

df = pd.DataFrame(data)

print("BLE Sentinel v1.3 - RSSI Analysis")
print("=" * 40)
print(df.to_string(index=False))

print("\nSummary Statistics")
print("=" * 40)
print(df["Average_RSSI_dBm"].describe())

# Calculate the signal-strength change
rssi_change = (
    df["Average_RSSI_dBm"].iloc[-1]
    - df["Average_RSSI_dBm"].iloc[0]
)

print(f"\nRSSI change from 0.5 m to 5 m: {rssi_change} dB")

# Generate the RSSI graph
plt.figure(figsize=(9, 5))

plt.plot(
    df["Distance_m"],
    df["Average_RSSI_dBm"],
    marker="o",
    label="Average RSSI",
)

plt.title("BLE Sentinel: RSSI vs Distance")
plt.xlabel("Distance (m)")
plt.ylabel("Average RSSI (dBm)")
plt.grid(True)
plt.legend()
plt.tight_layout()

plt.savefig("rssi_distance.png", dpi=300)
plt.show()
