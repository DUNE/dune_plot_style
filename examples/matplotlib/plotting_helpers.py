# Helper functions for example.py. Mostly copied from StackOverflow

import numpy as np
import scipy.stats 
from matplotlib.patches import Ellipse

def Gauss(x, A, x0, sigma):
    return A * np.exp(-(x - x0) ** 2 / (2 * sigma ** 2))

def CovEllipse(xdata, ydata, cov, q=None, nsig=None, facecolor='none', **kwargs):
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
