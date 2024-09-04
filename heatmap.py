#
# PTOC: PDE to ODE converter
# Created by Dirck van den Ende, Leiden University
# https://github.com/dirckvdende/PDE-hybrid-converter
#

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
    print("Usage: " + sys.argv[0] + " <filename> [sim arguments]")
    exit()

os.system("./build/main -d --ode " + " ".join(sys.argv[1:]))
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
times = []
for row in data:
    if row[0] != "__time__" and row[0] != "t":
        heatData.append(row[1:])
    elif row[0] == "__time__":
        times = row[1:]

# Remove some ticks
timesShow = np.linspace(0, len(times) - 1, 5, dtype=int)
timesLabels = ["{:.1f}".format(times[len(times) - i - 1]) for i in timesShow]

xShow = np.linspace(0, len(heatData) - 1, 5, dtype=int)

CUSTOM_X_LABELS = False
if CUSTOM_X_LABELS:
    xmin, xmax = -10, 10
    xLabels = np.linspace(xmin, xmax, 5, dtype=float)
else:
    xLabels = xShow

plt.subplots(figsize=(4 * 1.4, 3 * 1.4))
hm = sns.heatmap(np.flipud(np.transpose(heatData)))
hm.set_yticks(timesShow)
hm.set_yticklabels(timesLabels)
hm.set_xticks(xShow)
hm.set_xticklabels(xLabels)
plt.xlabel("x")
plt.ylabel("t")
plt.savefig("tmp/heatmap.png", dpi=300, bbox_inches="tight")
