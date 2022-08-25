"""
matplotlib placeholder example.  we can do way better than this.
"""

import numpy as np
import scipy.stats
from matplotlib import pyplot as plt

import dunestyle.matplotlib as dunestyle
plt.style.use('../matplotlib/stylelib/dune.mplstyle')

x = np.linspace(-5, 5, 500)
y = scipy.stats.norm.pdf(x)

plt.plot(x, y, label="Baby's first Gaussian")
plt.xlabel("x label")
plt.ylabel("y label")
plt.legend()
dunestyle.WIP()
plt.savefig("example.matplotlib.png")
