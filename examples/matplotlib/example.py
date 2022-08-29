"""
matplotlib placeholder example.  we can do way better than this.
"""

import numpy as np
import scipy.stats
from matplotlib import pyplot as plt

import dunestyle as dunestyle

# 1D Gaussian example
mu, sigma = 0, 1
nbins = 50
x_gaus = np.random.normal(mu, sigma, 1000)

#plt.hist(x_gaus, nbins, label="Hist", histtype='step')
plt.hist(x_gaus, nbins, label="Hist", histtype='step')
plt.xlabel('x label')
plt.ylabel('y label')
plt.xlim([-5,5])
plt.legend()
dunestyle.WIP()
dunestyle.SimulationSide()
plt.savefig("example.1Dhist.matplotlib.png")

x = np.linspace(-5, 5, 500)
y = scipy.stats.norm.pdf(x)

plt.plot(x, y, label="Baby's first Gaussian")
plt.xlabel("x label")
plt.ylabel("y label")
plt.legend()
dunestyle.WIP()
plt.savefig("example.matplotlib.png")
