#
# PTOC: PDE to ODE converter
# Created by Dirck van den Ende, Leiden University
# https://github.com/dirckvdende/PDE-hybrid-converter
#

#
# Visualize a 2D grid where grid cells are in groups
# Each group will have some color
#

import numpy as np
import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 2:
    print("Usage: " + sys.argv[0] + " <input file>")
    exit()

data = []
with open(sys.argv[1]) as f:
    for line in f:
        data.append([])
        for entry in line.split(" ")[:-1]:
            if entry == "":
                continue
            data[-1].append(int(entry))
colorMap = np.random.random(len(data) * len(data[0]))
isBorder = "_border" in sys.argv[1]
if not isBorder:
    for x in range(len(data)):
        for y in range(len(data[0])):
            data[x][y] = colorMap[data[x][y]]

if isBorder:
    plt.imshow(data, interpolation="nearest", cmap="Greys")
else:
    plt.imshow(data, interpolation="nearest")
plt.savefig(sys.argv[1].replace(".txt", ".png"), dpi=200, bbox_inches="tight")