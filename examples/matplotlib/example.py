"""
matplotlib placeholder example.  we can do way better than this.
"""

import numpy as np
import scipy.stats
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt
from matplotlib.patches import Ellipse
import matplotlib.gridspec as gridspec
import matplotlib.mlab as mlab

import dunestyle.matplotlib as dunestyle

### Simple 1D Gaussian example ###
x = np.linspace(-5, 5, 500)
y = scipy.stats.norm.pdf(x)

plt.plot(x, y, label="Gaussian")
plt.xlabel("x label")
plt.ylabel("y label")
plt.legend()
dunestyle.WIP()
dunestyle.SimulationSide()
plt.savefig("example.gaussian.matplotlib.png")

# The next two example plots will use the same Gaussian distribution.
# Here, we save the Gaussian as a numpy histogram, but this isn't 
# strictly necessary. It just makes data manipulation easier and 
# allows us to manipulate the histogram data without drawing it
# (relevant in the data/MC plot, where we want to convert the
# histogram to points).
mu, sigma = 0, 1
x_gaus = np.random.normal(mu, sigma, 1000)
counts, bin_edges = np.histogram(x_gaus, bins=30)
bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2

# Bin errors go as sigma/sqrt(N), where N is the bin count
# TODO What about 0 bins?
std_dev = np.std(x_gaus)
y_errors = np.array([std_dev/np.sqrt(bin_count) if bin_count != 0 else 1e-3 for bin_count in counts])
#y_errors = [std_dev/np.sqrt(bin_count) if bin_count != 0 else 0 for bin_count in counts]

### 1D histogram example ###
# Can plot histograms from numpy hists using either stairs or hist, but
# the syntax is slightly different between the two
#plt.stairs(counts, bin_edges, fill=False, label="Hist", linewidth=1.5)
#plt.hist(bin_edges[:-1], bin_edges, weights=counts, fill=False, label="Hist", linewidth=1.5)
plt.hist(x_gaus, histtype='step', label="Hist", linewidth=1.5)
plt.xlabel('x label')
plt.ylabel('y label')
# Don't need this next line if plotting using numpy histogram
plt.xlim(-5,5)
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
    #popt, pcov = curve_fit(Gauss, x, y, p0=[min(y), max(y), mean, sigma])
    popt, pcov = curve_fit(Gauss, x, y, p0=[1, mean, sigma])
    return popt, pcov

mean = sum(bin_centers * counts) / sum(counts)
sigma = np.sqrt(sum(counts * (bin_centers - mean) ** 2) / sum(counts))
#popt, pcov = curve_fit(Gauss, bin_centers, counts, p0=[1,mean,sigma], sigma=y_errors)
popt, pcov = curve_fit(Gauss, bin_centers, counts, 
                       p0=[min(bin_centers), max(bin_centers), mean, sigma],
                       sigma=y_errors)
#popt, pcov = curve_fit(Gauss, bin_centers, counts, 
#                       p0=[min(bin_centers), max(bin_centers), mean, sigma])

fig = plt.figure(figsize=(10,6))
gs = gridspec.GridSpec(nrows=2, ncols=1, height_ratios=[3, 1])

#popt, pcov = Gauss_fit(bin_centers, counts)

# Unpack fit parameters from popt and uncertainties from 
# diagonal of covariance matrix
H, A, x0, sig = popt
dH, dA, dx0, dsig = [np.sqrt(pcov[j,j]) for j in range(popt.size)]

# Create fitting function
# TODO: This fit drawing is too choppy. Smooth it out
x_fit = bin_centers
y_fit = Gauss(x_fit, H, A, x0, sig)

# TODO Check residuals. (Count-fit)/fit? Something else?
residuals = counts/Gauss(bin_centers, H, A, x0, sigma)

# Top plot
ax0 = fig.add_subplot(gs[0, 0])
ax0.set_ylabel("y label")
ax0.plot(x_fit, y_fit, color='r', label="Fit")
ax0.errorbar(x=bin_centers, y=counts,
             yerr=y_errors, fmt='o', capsize=0.5, label="Data")
ax0.legend()
ax0.set_xlim(-5,5)
dunestyle.CornerLabel("Data/MC")

# Bottom plot
ax1 = fig.add_subplot(gs[1, 0], sharex=ax0)
ax1.errorbar(x=bin_centers, y=residuals,
             yerr=y_errors, fmt='o', capsize=1, label="Ratio")
ax1.axhline(y=1, color="r", zorder=-1)
ax1.set_xlabel("x label")
ax1.set_ylabel("Residuals")
ax1.set_ylim(0,2)
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

### 2D Contour Example ###
def plot_point_cov(points, nstd=2, ax=None, **kwargs):
    """
    Plots an `nstd` sigma ellipse based on the mean and covariance of a point
    "cloud" (points, an Nx2 array).

    Parameters
    ----------
        points : An Nx2 array of the data points.
        nstd : The radius of the ellipse in numbers of standard deviations.
            Defaults to 2 standard deviations.
        ax : The axis that the ellipse will be plotted on. Defaults to the 
            current axis.
        Additional keyword arguments are pass on to the ellipse patch.

    Returns
    -------
        A matplotlib ellipse artist
    """
    pos = points.mean(axis=0)
    cov = np.cov(points, rowvar=False)
    return plot_cov_ellipse(cov, pos, nstd, ax, **kwargs)

def plot_cov_ellipse(cov, pos, nstd=2, ax=None, **kwargs):
    """
    Plots an `nstd` sigma error ellipse based on the specified covariance
    matrix (`cov`). Additional keyword arguments are passed on to the 
    ellipse patch artist.

    Parameters
    ----------
        cov : The 2x2 covariance matrix to base the ellipse on
        pos : The location of the center of the ellipse. Expects a 2-element
            sequence of [x0, y0].
        nstd : The radius of the ellipse in numbers of standard deviations.
            Defaults to 2 standard deviations.
        ax : The axis that the ellipse will be plotted on. Defaults to the 
            current axis.
        Additional keyword arguments are pass on to the ellipse patch.

    Returns
    -------
        A matplotlib ellipse artist
    """
    def eigsorted(cov):
        vals, vecs = np.linalg.eigh(cov)
        order = vals.argsort()[::-1]
        return vals[order], vecs[:,order]

    if ax is None:
        ax = plt.gca()

    vals, vecs = eigsorted(cov)
    theta = np.degrees(np.arctan2(*vecs[:,0][::-1]))

    # Width and height are "full" widths, not radius
    width, height = 2 * nstd * np.sqrt(vals)
    ellip = Ellipse(xy=pos, width=width, height=height, angle=theta, **kwargs)

    ax.add_artist(ellip)
    return ellip

def cov_ellipse(xdata, ydata, cov, q=None, nsig=None, facecolor='none', **kwargs):
    """
    Parameters
    ----------
    cov : (2, 2) array
        Covariance matrix.
    q : float, optional
        Confidence level, should be in (0, 1)
    nsig : int, optional
        Confidence level in unit of standard deviations. 
        E.g. 1 stands for 68.3% and 2 stands for 95.4%.

    Returns
    -------
    width, height, angle :
         The lengths of two axises and the angle angle in degree
    for the ellipse.
    """

    if q is not None:
        q = np.asarray(q)
    elif nsig is not None:
        q = 2 * scipy.stats.norm.cdf(nsig) - 1
    else:
        raise ValueError('One of `q` and `nsig` should be specified.')
    r2 = scipy.stats.chi2.ppf(q, 2)

    val, vec = np.linalg.eigh(cov)
    width, height = 2 * np.sqrt(val[:, None] * r2)
    angle = np.degrees(np.arctan2(*vec[::-1, 0]))

    xmean = np.mean(xdata)
    ymean = np.mean(ydata)

    ellipse = Ellipse(xy=(xmean,ymean), width=width, height=height, angle=angle,
                          facecolor=facecolor, **kwargs)

    #return width, height, angle
    return ellipse



cov = np.cov([throws[:,0],throws[:,1]], rowvar=True)
#ellip_1sig = cov_ellipse(throws[:,0], throws[:,1], cov, nsig=1)

fig, ax = plt.subplots(figsize=(8,6))
#ellip_1sig = Ellipse(xy=(xpos,ypos), width=width, height=height, angle=angle,
#                     facecolor='none', edgecolor='firebrick', label=r"1$\sigma$")
ellip_1sig = cov_ellipse(throws[:,0], throws[:,1], cov, nsig=1, 
                         edgecolor='firebrick', label=r"1$\sigma")
ellip_2sig = cov_ellipse(throws[:,0], throws[:,1], cov, nsig=2, 
                         edgecolor='fuchsia', label=r"2$\sigma", linestyle='--')
ellip_3sig = cov_ellipse(throws[:,0], throws[:,1], cov, nsig=3, 
                         edgecolor='blue', label=r"1$\sigma", linestyle=':')
ax.add_patch(ellip_1sig)
ax.add_patch(ellip_2sig)
ax.add_patch(ellip_3sig)
ax.set_xlim(-5,5)
ax.set_ylim(-5,5)
plt.xlabel("x label")
plt.ylabel("y label")
dunestyle.CornerLabel("2D Contour Example")
dunestyle.Simulation()
plt.legend()
plt.savefig("example.2Dcontour.matplotlib.png")

### Stacked histogram example ###
x1 = np.random.normal( 0, 1, 1000)
x2 = np.random.normal( 1, 1, 1000)
x3 = np.random.normal(-1, 1, 1000)
nbins = 50
plt.figure()
hist_labels = ['Hist 1', 'Hist 2', 'Hist 3']
plt.hist([x1,x2,x3], nbins, histtype='step', stacked=True, linewidth=2, label=hist_labels)
plt.xlabel('x label')
plt.ylabel('y label')
dunestyle.WIP()
dunestyle.SimulationSide()
plt.legend()
plt.savefig("example.stackedhist.matplotlib.png")


### Overlayed histogram example ###
plt.figure()
plt.hist(x1, nbins, histtype='step', linewidth=2, label="Hist 1")
plt.hist(x2, nbins, histtype='step', linewidth=2, label="Hist 2")
plt.hist(x3, nbins, histtype='step', linewidth=2, label="Hist 3")
plt.xlabel('x label')
plt.ylabel('y label')
dunestyle.WIP()
dunestyle.SimulationSide()
plt.legend()
plt.savefig("example.overlayhist.matplotlib.png")

