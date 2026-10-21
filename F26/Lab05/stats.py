import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from lab5 import precond_perf


tperiter = {}
for k, v in precond_perf.items():
    tperiter[k] = v["time [s]"] / v["iters"]

for k, v in tperiter.items():
    print(k, tperiter[k], np.mean(tperiter[k]))

fig, axs = plt.subplots(1, 2)

axs[0].set_xlabel("Grid Size")
axs[1].set_xlabel("Grid Size")
axs[0].set_ylabel("Time [s]")
axs[1].set_ylabel("Iterations")

for k, v, in precond_perf.items():
    axs[0].plot(v["grid_size"], v["time [s]"], marker="o", label=k)
    axs[1].plot(v["grid_size"], v["iters"], marker="o", label=k)

axs[0].legend()
axs[1].legend()
fig.tight_layout()
plt.savefig("ex3.png")
