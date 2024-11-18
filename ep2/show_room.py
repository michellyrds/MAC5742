import numpy as np
import matplotlib.pyplot as plt

# Load heat data from file
# heat_data = np.loadtxt("room.txt")
heat_data = np.loadtxt("cuda_room.txt")

extent = [0, 10, 0, 10]

# Plot heatmap
plt.imshow(
    heat_data, cmap="hot", interpolation="nearest", extent=extent, origin="lower"
)
plt.colorbar(label="Temperature")
plt.title("Heat Distribution")
plt.xlabel("X Coordinate")
plt.ylabel("Y Coordinate")
plt.show()