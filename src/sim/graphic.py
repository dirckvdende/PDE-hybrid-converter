#
# This program generates a visual output for the ODE system simulator. It runs
# the simulator automatically
#

import matplotlib.pyplot as plt
import numpy as np
import os
import sys

if len(sys.argv) < 2:
    print("Usage: " + sys.argv[0] + " <filename> [display vars]")
    exit()

varNames = set()
for name in sys.argv[2:]:
    varNames.add(name)

os.system("./build/main --ode " + sys.argv[1])
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

for row in data:
    plt.plot([float(x) for x in row[1:]], label=row[0])

plt.legend()
plt.savefig("tmp/ode.png")
