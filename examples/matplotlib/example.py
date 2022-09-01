"""
matplotlib placeholder example.  we can do way better than this.
"""

import numpy as np
import scipy.stats
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt
from matplotlib.patches import Ellipse
import matplotlib.gridspec as gridspec

import dunestyle.matplotlib as dunestyle

from plotting_helpers import Gauss, CovEllipse

### Simple 1D Gaussian example ###
def Gauss1D():
    x = np.linspace(-5, 5, 500)
    y = scipy.stats.norm.pdf(x)

    plt.plot(x, y, label="Gaussian")
    plt.xlabel("x label")
    plt.ylabel("y label")
    plt.legend()
    dunestyle.WIP()
    dunestyle.SimulationSide()
    plt.savefig("example.matplotlib.gaus.png")

# This example saves a Gaussian as a numpy histogram, but this isn't 
# strictly necessary. It just makes data manipulation easier and 
# allows us to manipulate the histogram data without drawing it
# (relevant in the data/MC plot, where we want to convert the
# histogram to points).

### 1D histogram example ###
def Hist1D():
    mu, sigma = 0, 1
    x_gaus = np.random.normal(mu, sigma, 1000)

    plt.hist(x_gaus, histtype='step', label="Hist", linewidth=1.5)
    plt.xlabel('x label')
    plt.ylabel('y label')
    plt.xlim(-5,5)
    plt.legend()
    dunestyle.WIP()
    dunestyle.SimulationSide()
    plt.savefig("example.matplotlib.hist1D.png")

### Data/MC example ###
# For this example, we take our "data" from the above 1D Gaussian histogram

# Gaus fits are not as straightforward in matplotlib as they are
# in ROOT. See the second example at
# https://physics.nyu.edu/pine/pymanual/html/chap8/chap8_fitting.html

#def Gauss(x, H, A, x0, sigma):
#    return H + A * np.exp(-(x - x0) ** 2 / (2 * sigma ** 2))

def DataMC():
    # Use SciPy's curve_fit function to return optimal fit
    # parameters (popt) and the covariance matrix (pconv)
    mu, sigma = 0, 1
    x_gaus = np.random.normal(mu, sigma, 1000)
    counts, bin_edges = np.histogram(x_gaus, bins=30)
    bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
    std_dev = np.std(x_gaus)
    y_errors = np.array([std_dev/np.sqrt(bin_count) if bin_count != 0 else 1e-3 for bin_count in counts])

    # Note that you need to give curve_fit a good initial guess
    # in order for it to converge (see p0 below)
    mean = sum(bin_centers * counts) / sum(counts)
    sigma = np.sqrt(sum(counts * (bin_centers - mean) ** 2) / sum(counts))
    popt, pcov = curve_fit(Gauss, bin_centers, counts, 
                           p0=[min(bin_centers), max(bin_centers), mean, sigma],
                           sigma=y_errors)

    # Unpack fit parameters from popt and uncertainties from 
    # diagonal of covariance matrix
    H, A, x0, sig = popt
    dH, dA, dx0, dsig = [np.sqrt(pcov[j,j]) for j in range(popt.size)]

    ratio = counts/Gauss(bin_centers, H, A, x0, sigma)

    fig = plt.figure(figsize=(10,6))
    gs = gridspec.GridSpec(nrows=2, ncols=1, height_ratios=[3, 1])

    # Create fitting function
    x_fit = np.linspace(-5, 5, 500)
    y_fit = Gauss(x_fit, H, A, x0, sig)

    # Top plot
    ax0 = fig.add_subplot(gs[0, 0])
    ax0.set_ylabel("y label")
    ax0.plot(x_fit, y_fit, color='r', label="Fit")
    ax0.errorbar(x=bin_centers, y=counts,
                 yerr=y_errors, fmt='o', capsize=1, label="Data")
    ax0.legend()
    ax0.set_xlim(-5,5)
    dunestyle.CornerLabel("Data/MC")

    # Bottom plot
    ax1 = fig.add_subplot(gs[1, 0], sharex=ax0)
    ax1.errorbar(x=bin_centers, y=ratio,
                 yerr=y_errors, fmt='o', capsize=1, label="Ratio")
    ax1.axhline(y=1, color="r", zorder=-1)
    ax1.set_xlabel("x label")
    ax1.set_ylabel("Ratio to Fit")
    ax1.set_ylim(0,2)
    plt.savefig("example.matplotlib.datamc.png")

### 2D Histogram Example ###
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

    return ellipse

def Hist2DContour():
    mean = (0, 0)
    cov = [[0.5,-0.5],[-0.5,1]]
    throws = np.random.multivariate_normal(mean, cov, 10000000)
    xbins = np.arange(100)
    ybins = np.arange(100)
    xrange = [-5,5]
    yrange = [-5,5]

    fig, ax = plt.subplots()
    hist2d = ax.hist2d(throws[:,0],throws[:,1], bins=100, cmin=1,
                       range=[xrange,yrange])
    # Add z-axis colorbar. When creating hist2d, it returns
    # (counts, xedges, yedges, image), in that order. We need
    # the image to be called by fig.colorbar(). See
    # https://stackoverflow.com/questions/42387471/how-to-add-a-colorbar-for-a-hist2d-plot
    fig.colorbar(hist2d[3])
    npcov = np.cov([throws[:,0],throws[:,1]], rowvar=True)

    ellip_1sig = CovEllipse(throws[:,0], throws[:,1], npcov, nsig=1, 
                             edgecolor='firebrick', label=r"1$\sigma$",
                             linewidth=2)
    ellip_2sig = CovEllipse(throws[:,0], throws[:,1], npcov, nsig=2, 
                             edgecolor='fuchsia', label=r"2$\sigma$", 
                             linewidth=2, linestyle='--')
    ellip_3sig = CovEllipse(throws[:,0], throws[:,1], npcov, nsig=3, 
                             edgecolor='cyan', label=r"3$\sigma$", 
                             linewidth=2, linestyle=':')
    ax.add_patch(ellip_1sig)
    ax.add_patch(ellip_2sig)
    ax.add_patch(ellip_3sig)

    ax.set_xlabel("x label")
    ax.set_ylabel("y label")
    dunestyle.CornerLabel("2D Histogram Example")
    dunestyle.Simulation()
    plt.legend()
    plt.savefig("example.matplotlib.hist2D.png")

### Stacked histogram example ###
def HistStacked():
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
    plt.savefig("example.matplotlib.histstacked.png")

### Overlayed histogram example ###
def HistOverlay():
    x1 = np.random.normal( 0, 1, 1000)
    x2 = np.random.normal( 1, 1, 1000)
    x3 = np.random.normal(-1, 1, 1000)
    nbins = 50
    plt.figure()
    plt.style.use('tableau-colorblind10')
    plt.hist(x1, nbins, histtype='step', linewidth=2, label="Hist 1")
    plt.hist(x2, nbins, histtype='step', linewidth=2, label="Hist 2")
    plt.hist(x3, nbins, histtype='step', linewidth=2, label="Hist 3")
    plt.xlabel('x label')
    plt.ylabel('y label')
    dunestyle.WIP()
    dunestyle.SimulationSide()
    plt.legend()
    plt.savefig("example.matplotlib.histoverlay.png")

if __name__ == '__main__':
    Gauss1D()
    Hist1D()
    DataMC()
    Hist2DContour()
    HistStacked()
    HistOverlay()
