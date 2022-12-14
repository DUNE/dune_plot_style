"""
matplotlib placeholder example.  we can do way better than this.
"""

import numpy as np
import scipy.stats
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt
from matplotlib.patches import Ellipse
import matplotlib.gridspec as gridspec
from matplotlib.backends.backend_pdf import PdfPages

import dunestyle.matplotlib as dunestyle

from plotting_helpers import Gauss, CovEllipse

### Simple 1D Gaussian example ###
def Gauss1D(pdf):
    x = np.linspace(-5, 5, 500)
    y = scipy.stats.norm.pdf(x)

    # Set axex color. For specific axes, you can use e.g.
    # ax.spines['left'].set_color()
    # Also, note this needs to come before plt.plot() or else
    # matplotlib freaks out
    ax = plt.axes()
    ax.spines[:].set_color('black')

    plt.plot(x, y, label="Gaussian")
    plt.xlabel("x label")
    plt.ylabel("y label")
    plt.legend()

    # Scale y-axis so "Work in Progress" watermark fits in frame
    ax.set_ylim(0, 1.2*ax.get_ylim()[1])
    dunestyle.WIP()
    dunestyle.SimulationSide()
    plt.savefig("example.matplotlib.gaus.png")
    pdf.savefig()

### 1D histogram example ###
def Hist1D(pdf):
    x = np.random.normal(0, 1, 1000)


    plt.figure()
    plt.style.use('tableau-colorblind10')
    ax = plt.axes()
    ax.spines[:].set_color('black')
    plt.hist(x, histtype='step', label="Hist", linewidth=2)
    plt.xlabel('x label')
    plt.ylabel('y label')
    plt.xlim(-5,5)
    plt.legend()
    ax.set_ylim(0, 1.2*ax.get_ylim()[1])
    dunestyle.WIP()
    dunestyle.SimulationSide()
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
    x_gaus = np.random.normal(mu, sigma, 10000)
    counts, bin_edges = np.histogram(x_gaus, bins=20)
    bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
    std_dev = np.std(x_gaus)
    y_errors = np.array([std_dev/np.sqrt(bin_count) if bin_count != 0 else 1e-3 for bin_count in counts])

    # Use SciPy's curve_fit function to return optimal fit
    # parameters (popt) and the covariance matrix (pconv)
    # Note that you need to give curve_fit a good initial guess
    # for fit parameters in order for it to converge (see p0 below)
    mean = sum(bin_centers * counts) / sum(counts)
    sigma = np.sqrt(sum(counts * (bin_centers - mean) ** 2) / sum(counts))
    popt, pcov = curve_fit(Gauss, bin_centers, counts, 
                           p0=[min(bin_centers), max(bin_centers), mean, sigma],
                           sigma=y_errors)

    # Unpack optimal fit parameters and uncertainties from 
    # diagonal of covariance matrix 
    H, A, x0, sig = popt
    dH, dA, dx0, dsig = [np.sqrt(pcov[j,j]) for j in range(popt.size)]

    # Create fitting function
    x_fit = np.linspace(-4, 4, 100)
    y_fit = Gauss(x_fit, H, A, x0, sig)

    ratio = counts/Gauss(bin_centers, H, A, x0, sigma)
    residuals = (counts - Gauss(bin_centers, H, A, x0, sigma)) / Gauss(bin_centers, H, A, x0, sigma)
    chi2 = ((residuals**2)).sum() / float(bin_centers.size-2)

    fig = plt.figure(figsize=(8,6))
    gs = gridspec.GridSpec(nrows=2, ncols=1, height_ratios=[3, 1])

    # Top plot
    ax0 = fig.add_subplot(gs[0, 0])
    ax0.set_ylabel("y label")
    ax0.plot(x_fit, y_fit, color='r', label="Fit")
    ax0.errorbar(x=bin_centers, y=counts, yerr=y_errors, 
                 color='black', fmt='o', capsize=1, label="Data")
    ax0.text(0.70, 0.70, 'Gauss Fit Parameters:', 
             fontdict={'color': 'darkred', 'size': 10, 'weight': 'bold'},
             transform=ax0.transAxes)
    ax0.text(0.70, 0.65, 'H = {0:0.1f}$\pm${1:0.1f}'
             .format(H, dH), transform=ax0.transAxes)
    ax0.text(0.70, 0.60, 'A = {0:0.2f}$\pm${1:0.2f}'
             .format(A, dA), transform=ax0.transAxes)
    ax0.text(0.70, 0.55, r'$\mu$ = {0:0.2f}$\pm${1:0.2f}'
             .format(x0, dx0), transform=ax0.transAxes)
    ax0.text(0.70, 0.50, r'$\sigma$ = {0:0.1f}$\pm${1:0.1f}'
             .format(sig, dsig), transform=ax0.transAxes)
    ax0.text(0.70, 0.40, '$\chi^2/ndof$ = {0:0.2f}'
             .format(chi2),transform=ax0.transAxes)
    ax0.spines[:].set_color('black')
    ax0.legend()
    ax0.set_xlim(-5,5)
    dunestyle.CornerLabel("Data/MC")

    # Bottom plot
    ax1 = fig.add_subplot(gs[1, 0], sharex=ax0)
    ax1.errorbar(x=bin_centers, y=residuals, yerr=y_errors, 
                 color='black', fmt='o', capsize=1, label="Ratio")
    ax1.axhline(y=0, color="r", zorder=-1)
    ax1.set_xlabel("x label")
    ax1.set_ylabel("(Data - Fit)/Fit")
    ax1.set_ylim(-1,1)
    ax1.spines[:].set_color('black')
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

    # If you need to calculate the covariance yourself, use numpy's method
    #npcov = np.cov([throws[:,0],throws[:,1]], rowvar=True)

    ellip_1sig = CovEllipse(throws[:,0], throws[:,1], cov, nsig=1, 
                             edgecolor='firebrick', label=r"1$\sigma$",
                             linewidth=2)
    ellip_2sig = CovEllipse(throws[:,0], throws[:,1], cov, nsig=2, 
                             edgecolor='fuchsia', label=r"2$\sigma$", 
                             linewidth=2, linestyle='--')
    ellip_3sig = CovEllipse(throws[:,0], throws[:,1], cov, nsig=3, 
                             edgecolor='cyan', label=r"3$\sigma$", 
                             linewidth=2, linestyle=':')
    ax.add_patch(ellip_1sig)
    ax.add_patch(ellip_2sig)
    ax.add_patch(ellip_3sig)

    ax.set_xlabel("x label")
    ax.set_ylabel("y label")
    ax.spines[:].set_color('black')
    dunestyle.CornerLabel("2D Histogram Example")
    dunestyle.Simulation(x=1.15) # Shift slightly right 
    plt.legend()
    plt.savefig("example.matplotlib.hist2D.png")
    pdf.savefig()

### Stacked histogram example ###
def HistStacked(pdf):
    x1 = np.random.normal( 0, 1, 1000)
    x2 = np.random.normal( 1, 1, 1000)
    x3 = np.random.normal(-1, 1, 1000)
    nbins = 50
    plt.figure()
    ax = plt.axes()
    ax.spines[:].set_color('black')
    # Can choose one of matplotlib's built-in color patlettes if you prefer
    plt.style.use('tableau-colorblind10')
    hist_labels = ['One Hist', 'Two Hist', 'Three Hist']
    plt.hist([x1,x2,x3], nbins, histtype='stepfilled', stacked=True, linewidth=2, label=hist_labels)
    plt.xlabel('x label')
    plt.ylabel('y label')
    ax.set_ylim(0, 1.2*ax.get_ylim()[1])
    dunestyle.WIP()
    dunestyle.SimulationSide()
    plt.legend()
    plt.savefig("example.matplotlib.histstacked.png")
    pdf.savefig()

### Overlayed histogram example ###
def HistOverlay(pdf):
    x1 = np.random.normal( 0, 1, 1000)
    x2 = np.random.normal( 2, 1, 1000)
    x3 = np.random.normal(-2, 1, 1000)
    nbins = 25
    plt.figure()
    ax = plt.axes()
    ax.spines[:].set_color('black')
    plt.style.use('tableau-colorblind10')
    plt.hist(x1, nbins, histtype='step', linewidth=2, label="One Hist")
    plt.hist(x2, nbins, histtype='step', linewidth=2, label="Two Hist")
    plt.hist(x3, nbins, histtype='step', linewidth=2, label="Three Hist")
    plt.xlabel('x label')
    plt.ylabel('y label')
    ax.set_ylim(0, 1.2*ax.get_ylim()[1])
    dunestyle.WIP()
    dunestyle.SimulationSide()
    plt.legend()
    plt.savefig("example.matplotlib.histoverlay.png")
    pdf.savefig()

if __name__ == '__main__':
    pdf = PdfPages("example.matplotlib.pdf")

    Gauss1D(pdf)
    Hist1D(pdf)
    DataMC(pdf)
    Hist2DContour(pdf)
    HistStacked(pdf)
    HistOverlay(pdf)

    pdf.close()
