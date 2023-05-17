
import matplotlib.pyplot as plt
import numpy as np
import os
import sys

if len(sys.argv) != 2:
    print("Usage: " + sys.argv[0] + " <filename>")
    exit()
os.system("./build/main --ode " + sys.argv[1])
f = open("./tmp/ode.txt")

for line in f.readlines():
    data = line.strip().split(" ")
    plt.plot([float(x) for x in data[1:]], label=data[0])
plt.legend()
plt.savefig("tmp/ode.png")
