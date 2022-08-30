"""
matplotlib placeholder example.  we can do way better than this.
"""

import numpy as np
import scipy.stats
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt
import matplotlib.gridspec as gridspec

import dunestyle.matplotlib as dunestyle

# The first two example plots will use the same Gaussian distribution.
# Here, we save the Gaussian as a numpy histogram, but this isn't 
# strictly necessary. It just makes data manipulation easier and 
# allows us to manipulate the histogram data without drawing it
# (relevant in the data/MC plot, where we want to convert the
# histogram to points).
mu, sigma = 0, 1
x_gaus = np.random.normal(mu, sigma, 1000)

hist, bin_edges = np.histogram(x_gaus, bins=50, range=(-5,5))
bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
# Dummy error values; just for illlustration
y_error = np.std(x_gaus)/np.sqrt(x_gaus.size)*hist

### 1D Gaussian example ###
# The "stairs" plot is the easiest way to plot a histogram
# from a numpy histogram. If you're not using a numpy histogram,
# then use plt.hist().
plt.stairs(hist, bin_edges, fill=False, label="Hist", linewidth=1.5)
plt.xlabel('x label')
plt.ylabel('y label')
plt.legend()
dunestyle.WIP()
dunestyle.SimulationSide()
plt.savefig("example.1Dhist.matplotlib.png")

### Data/MC example ###
# For this example, we take our "data" from the above 1D Gaussian histogram

# Gaus fits are not as straightforward in matplotlib as they are
# in ROOT. See the second example at
# https://physics.nyu.edu/pine/pymanual/html/chap8/chap8_fitting.html

def Gauss(x, H, A, x0, sigma):
  return H + A * np.exp(-(x - x0) ** 2 / (2 * sigma ** 2))

# Use SciPy's curve_fit function to return optimal fit
# parameters (popt) and the covariance matrix (pconv)
# Note that you need to give curve_fit a good initial guess
# in order for it to converge (see p0 below)
def Gauss_fit(x, y):
  mean = sum(x * y) / sum(y)
  sigma = np.sqrt(sum(y * (x - mean) ** 2) / sum(y))
  popt, pcov = curve_fit(Gauss, x, y, p0=[min(y), max(y), mean, sigma])
  return popt, pcov

fig = plt.figure(figsize=(10,6))
gs = gridspec.GridSpec(nrows=2, ncols=1, height_ratios=[3, 1])

popt, pcov = Gauss_fit(bin_centers, hist)

# Unpack fit parameters from popt and uncertainties from 
# diagonal of covariance matrix
H, A, x0, sig = popt
dH, dA, dx0, dsig = [np.sqrt(pcov[j,j]) for j in range(popt.size)]

# Create fitting function
x_fit = bin_centers
y_fit = Gauss(x_fit, H, A, x0, sig)

residuals = hist/Gauss(bin_centers, H, A, x0, sigma)

# Top plot
ax0 = fig.add_subplot(gs[0, 0])
ax0.set_ylabel("y label")
ax0.plot(x_fit, y_fit, color='r', label="Fit")
ax0.errorbar(x=bin_centers, y=hist,
             yerr=y_error, fmt='o', capsize=0.5, label="Data")
ax0.legend()
dunestyle.CornerLabel("Data/MC")

# Bottom plot
ax1 = fig.add_subplot(gs[1, 0], sharex=ax0)
ax1.errorbar(x=bin_centers, y=residuals,
             yerr=y_error, fmt='o', capsize=1, label="Ratio")
ax1.axhline(color="r", zorder=-1)
ax1.set_xlabel("x label")
ax1.set_ylabel("Residuals")
plt.savefig("example.datamc.matplotlib.png")

### 2D Histogram Example ###
mean = (0, 0)
cov = [[0.5,-0.5],[-0.5,1]]
throws = np.random.multivariate_normal(mean, cov, 10000000)
xbins = np.arange(100)
ybins = np.arange(100)
xrange = [-5,5]
yrange = [-5,5]
fig, ax = plt.subplots()
hist2d = ax.hist2d(throws[:,0],throws[:,1], bins=100, range=[[-5,5],[-5,5]])
ax.set_xlabel("x label")
ax.set_ylabel("y label")
# Add z-axis colorbar. When creating hist2d, it returns
# (counts, xedges, yedges, image), in that order. We need
# the image to be called by fig.colorbar(). See
# https://stackoverflow.com/questions/42387471/how-to-add-a-colorbar-for-a-hist2d-plot
fig.colorbar(hist2d[3])
dunestyle.CornerLabel("2D Histogram Example")
dunestyle.Simulation()
plt.savefig("example.2Dhist.matplotlib.png")

x = np.linspace(-5, 5, 500)
y = scipy.stats.norm.pdf(x)

plt.plot(x, y, label="Baby's first Gaussian")
plt.xlabel("x label")
plt.ylabel("y label")
plt.legend()
dunestyle.WIP()
plt.savefig("example.matplotlib.png")
