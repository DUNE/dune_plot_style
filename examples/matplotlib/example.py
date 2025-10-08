"""
Demonstration of DUNE plot style using matplotlib.

Original authors:  Young DUNE plot style task force
     Comments to:  Authorship & publications board (dune-apb@fnal.gov)
"""

import numpy as np
import scipy.stats
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt
from matplotlib.patches import Ellipse
import matplotlib.gridspec as gridspec
from matplotlib.backends.backend_pdf import PdfPages

from cycler import cycler

import dunestyle.matplotlib as dunestyle

from plotting_helpers import Gauss, CovEllipse

# how many histograms to draw in multi-hist plots
N_HISTS = 8   # exhibits all the colors in the Okabe-Ito cycler

dunestyle.OffWhiteBackground()

### 1D histogram example ###
def Hist1D(pdf):
    x = np.random.normal(0, 1, 1000)

    plt.figure()
    ax = plt.axes()
    plt.hist(x, histtype='step', label="Hist", range=(-5, 5), bins=50)
    plt.xlabel('x label')
    plt.ylabel('y label')
    plt.xlim(-5,5)
    ax.set_ylim(0, 1.2*ax.get_ylim()[1])
    dunestyle.Simulation()
    plt.savefig("example.matplotlib.hist1D.png")
    pdf.savefig()

### Data/MC example ###
# Gaus fits are not as straightforward in matplotlib as they are
# in ROOT. See the second example at
# https://physics.nyu.edu/pine/pymanual/html/chap8/chap8_fitting.html

# This example saves a Gaussian as a numpy histogram, but this isn't 
# strictly necessary. It just makes data manipulation easier and 
# allows us to manipulate the histogram data without drawing it
def DataMC(pdf):
    mu, sigma = 0, 1
    np.random.seed(89)
    x_gaus = np.random.normal(mu, sigma, 1000)
    counts, bin_edges = np.histogram(x_gaus, bins=50, range=(-5, 5))
    bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
#    std_dev = np.std(x_gaus)
    abs_errors = np.sqrt(counts)
    frac_errors = 100*np.ones_like(counts, dtype=float)  # default errors will be 10000% so that we ignore empty bins
    frac_errors[counts > 0] = 1./np.sqrt(counts[counts > 0])

    # Use SciPy's curve_fit function to return optimal fit
    # parameters (popt) and the covariance matrix (pconv)
    # Note that you need to give curve_fit a good initial guess
    # for fit parameters in order for it to converge (see p0 below)
    mean = sum(bin_centers * counts) / sum(counts)
    sigma = np.sqrt(sum(counts * (bin_centers - mean) ** 2) / sum(counts))
    popt, pcov = curve_fit(Gauss, bin_centers, counts, 
                           p0=[max(bin_centers), mean, sigma],
                           sigma=frac_errors)

    # Unpack optimal fit parameters and uncertainties from 
    # diagonal of covariance matrix 
    A, x0, sig = popt
    dA, dx0, dsig = [np.sqrt(pcov[j,j]) for j in range(popt.size)]

    # Create fitting function
    x_fit = np.linspace(-4, 4, 100)
    y_fit = Gauss(x_fit, A, x0, sig)
    fit_at_bin_ctrs = Gauss(bin_centers, A, x0, sig)

    ratio = counts / fit_at_bin_ctrs
    diff =  counts - fit_at_bin_ctrs
    residuals = diff / fit_at_bin_ctrs
    chi2 = (diff**2/fit_at_bin_ctrs).sum()

    # this way (count only nonzero bins) is the way ROOT counts deg of freedom,
    # so we mimic it for consistency (not nec. correctness)
    from inspect import signature
    ndf = np.count_nonzero(counts > 0) - len(signature(Gauss).parameters)

    fig = plt.figure(figsize=(8,6))
    gs = fig.add_gridspec(nrows=2, ncols=1, height_ratios=[3, 1], hspace=0)
    axs = gs.subplots(sharex=True)

    # only show non-empty bins below
    mask = np.nonzero(counts)

    # Top plot
    axs[0].set_ylabel("y label", fontsize="xx-large")
    axs[0].plot(x_fit, y_fit, color='r', label="Fit")
    axs[0].errorbar(x=bin_centers[mask], y=counts[mask], xerr=0, yerr=abs_errors[mask],
                 color='black', fmt='o', capsize=1, label="Data")
    axs[0].text(0.68, 0.60, 'Fit Parameters:',
             fontdict={'size': 14, 'weight': 'bold'},
             transform=axs[0].transAxes)

    fontdict = {"size": 14}
    axs[0].text(0.68, 0.52, r'A = {0:0.2f}$\pm${1:0.2f}'.format(A, dA),
                fontdict=fontdict,
                transform=axs[0].transAxes)
    axs[0].text(0.68, 0.46, r'$\mu$ = {0:0.2f}$\pm${1:0.2f}'.format(x0, dx0),
                fontdict=fontdict,
                transform=axs[0].transAxes)
    axs[0].text(0.68, 0.40, r'$\sigma$ = {0:0.2f}$\pm${1:0.2f}'.format(sig, dsig),
                fontdict=fontdict,
                transform=axs[0].transAxes)
    axs[0].text(0.68, 0.34, r'$\chi^2$/ndof = {0:0.2f}/{1:d}'.format(chi2, ndf),
                fontdict=fontdict,
                transform=axs[0].transAxes)
    axs[0].legend(fontsize="xx-large")  # since the upper panel is only 70% of the whole canvas, the legend is (by default) too small
    axs[0].set_xlim(-5,5)
    axs[0].set_ylim(bottom=0)

    axs[0].tick_params(labelsize="x-large")

    dunestyle.Preliminary(x=0.02, ax=axs[0], fontsize="xx-large")

    # Bottom plot
    axs[1].errorbar(x=bin_centers[mask], y=residuals[mask], yerr=frac_errors[mask],
                    color='black', fmt='o', capsize=1, label="Ratio")
    axs[1].axhline(y=0, color="r", zorder=-1)
    axs[1].set_xlabel("x label", fontsize="xx-large")
    axs[1].set_ylabel("(Data - Fit)/Fit", fontsize="xx-large")
    axs[1].set_ylim(-0.99,0.99)

    axs[1].tick_params(labelsize="x-large")

    for ax in axs:
        ax.label_outer()

    plt.savefig("example.matplotlib.datamc.png")
    pdf.savefig()

def Hist2DContour(pdf):
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

    ## Add z-axis colorbar. 
    # When creating hist2d, it returns (counts, xedges, yedges, image), 
    # in that order. We need the image to be called by fig.colorbar(). See
    # https://stackoverflow.com/questions/42387471/how-to-add-a-colorbar-for-a-hist2d-plot
    fig.colorbar(hist2d[3])

    # we want to cycle both the colors AND the line-styles,
    # so we combine cyclers.
    # unfortunately since they have different numbers of items,
    # we need to force the line-style one to repeat a few times,
    # then chop off the excess
    cyc = cycler(edgecolor=plt.rcParams["axes.prop_cycle"].by_key()["color"]) + cycler(linestyle=["-", ":", "--"]*10)[:len(plt.rcParams["axes.prop_cycle"])]
    cyc = cyc()
    for nsig in range(1,4):
        ellipse = CovEllipse(throws[:,0], throws[:,1], cov, nsig=nsig,
                             label=r"{0}$\sigma$".format(nsig), **next(cyc))
        ax.add_patch(ellipse)

    ax.set_ylim(top=1.3*ax.get_ylim()[1])  # make a little space for the "DUNE simulation" label
    ax.set_xlabel("x label")
    ax.set_ylabel("y label")
    dunestyle.Simulation()
    plt.legend()
    plt.savefig("example.matplotlib.hist2D.png")
    pdf.savefig()

### Stacked histogram example ###
def HistStacked(pdf):
    hist_extent = (N_HISTS-1)
    x = [np.random.normal(i, 1, 10000) for i in range(-hist_extent, hist_extent+2, 2)]
    nbins = 100
    plt.figure()
    ax = plt.axes()
    hist_labels = ["Hist #{0}".format(i+1) for i in range(len(x))]
    plt.hist(x, nbins, histtype='stepfilled', stacked=True, label=hist_labels)
    plt.xlabel('x label')
    plt.ylabel('y label')
    ax.set_xlim(-2*(N_HISTS/2+2), 2*N_HISTS)
    ax.set_ylim(0, 1.2*ax.get_ylim()[1])
    dunestyle.WIP()
    plt.legend()
    plt.savefig("example.matplotlib.histstacked.png")
    pdf.savefig()

### Overlayed histogram example ###
def HistOverlay(pdf):
    hist_extent = (N_HISTS-1)
    x = [np.random.normal(i, 1, 10000) for i in range(-hist_extent, hist_extent+2, 2)]
    hist_labels = ["Hist #{0}".format(i+1) for i in range(len(x))]
    nbins = 100
    plt.figure()
    ax = plt.axes()
    plt.hist(x, nbins, histtype='step', label=hist_labels)
    plt.xlabel('x label')
    plt.ylabel('y label')
    ax.set_xlim(-2*(N_HISTS/2+2), 2*N_HISTS)
    ax.set_ylim(0, 1.2*ax.get_ylim()[1])
    dunestyle.WIP()
    plt.legend()
    plt.savefig("example.matplotlib.histoverlay.png")
    pdf.savefig()

if __name__ == '__main__':
    pdf = PdfPages("example.matplotlib.pdf")

    Hist1D(pdf)
    DataMC(pdf)
    Hist2DContour(pdf)
    HistStacked(pdf)
    HistOverlay(pdf)

    pdf.close()
