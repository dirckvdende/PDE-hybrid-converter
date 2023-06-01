#
# This program generates a visual output for the ODE system simulator. It runs
# the simulator automatically. It generates a heatmap with time vertically and
# the variables (space) horizontally
#

import matplotlib.pyplot as plt
import numpy as np
import os
import seaborn as sns
import sys

if len(sys.argv) < 2:
    print("Usage: " + sys.argv[0] + " <filename> [display vars]")
    exit()

varNames = set()
for name in sys.argv[2:]:
    varNames.add(name)

os.system("./build/main -d --ode " + sys.argv[1])
f = open("./tmp/ode.csv")

data = []
first = True
for line in f.readlines():
    if first:
        names = line.strip().split(",")
        for name in names:
            data.append([name])
        first = False
    else:
        entries = [float(x) for x in line.strip().split(",")]
        for i in range(len(entries)):
            data[i].append(entries[i])

data.sort(key=(lambda row : row[0]))

heatData = []
for row in data:
    if ((row[0] in varNames or len(varNames) == 0) and row[0] != "__time__" and
    row[0] != "t"):
        heatData.append(row[1:])

sns.heatmap(np.transpose(heatData))
plt.savefig("tmp/heatmap.png", dpi=300)
