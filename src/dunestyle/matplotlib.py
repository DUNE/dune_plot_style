""" dunestyle.py (matplotlib edition): DUNE plot style tools for use with matplotlib.

This module simply imports the style defined in the C++ header version of the style.
By default it will be enabled immediately on import.

If you do not want it enabled on import, set the global:
```
import builtins  #  this only works with Python3...
builtins.__dict__["DUNESTYLE_ENABLE_AUTOMATICALLY"] = False
import dunestyle
```
Then you can call dunestyle.enable() to turn it on.

:author: J. Wolcott <jwolcott@fnal.gov>
:date:   March 2022
"""

import os
from matplotlib import pyplot as plt

def enable():
    import os

    path = os.path.join(os.environ['MPLCONFIGDIR'].split(os.pathsep)[0], "dune.mplstyle")
    assert os.path.exists(path), "Can't locate DUNE matplotlib style sheet file!  I tried path: " + path
    plt.style.use(path)
    print("DUNE plot style enabled")


import builtins
_IMPORT_FLAG_NAME = "DUNESTYLE_ENABLE_AUTOMATICALLY"
if _IMPORT_FLAG_NAME not in builtins.__dict__ or builtins.__dict__[_IMPORT_FLAG_NAME]:
    enable()

##########   Utility functions below  ################

def _GetTransform(transform=None, ax=None):
    """ Used in the text functions below.  Not intended for end-users  """
    if transform is not None:
        return transform
    if ax is not None and hasattr(ax, "transAxes"):
        return ax.transAxes
    return plt.gca().transAxes

def TextLabel(text, x, y, transform=None, ax=None, **kwargs):
    """
    Add a text label at an arbitray place on the plot.
    Mostly used as an internal utility for the more specific label functions,
    but end-users can feel free to use as well.

    :param text:  Text to write
    :param x:     Intended x-coordinate
    :param y:     Intended y-coordinate
    :param transform:  If you want to use a transformation other than the default transAxes, supply here.
    :param ax:    If you prefer to pass an Axes directly (perhaps you have multiple in a split canvas), do so here
    :param kwargs: Any other arguments will be passed to pyplot.text()
    :return:      None
    """
    plotter = plt if ax is None else ax
    kwargs.setdefault("fontdict", {})
    kwargs["fontdict"]["fontsize"] = 18
    if "color" in kwargs:
        kwargs["fontdict"]["color"] = kwargs.pop("color")
    if "fontsize" in kwargs:
        kwargs["fontdict"]["fontsize"] = kwargs.pop("fontsize")
    if "align" in kwargs:
        kwargs["horizontalalignment"] = kwargs.pop("align")
    plotter.text(x, y, text,
                 transform=_GetTransform(transform, plotter),
                 **kwargs)

def DUNEWatermarkString():
    """
    Produces the "DUNE" part of the string used in watermarks so it can be styled independently if necessary
    :return: The appropriately styled "DUNE" string
    """

    return r"$\mathdefault{\bf{DUNE}}$"

def Preliminary(x=0.05, y=0.90, align='left', transform=None, ax=None, **kwargs):
    """
    Apply a "DUNE Preliminary" label.
    :param x:          x-location for the label.  Default is just inside the frame on the left.
    :param y:          y-location for the label.  Default is just inside the frame on the top.
    :param align:      Text alignment (note: not placement!) for the label.  Default is left-align.
    :param transform:  If you want to use a transformation other than the default transAxes, supply here.
    :param ax:         If you prefer to pass an Axes directly (perhaps you have multiple in a split canvas), do so here
    :param kwargs:     Any other arguments will be passed to pyplot.text()
    :return:           None
    """
    TextLabel(DUNEWatermarkString() + " Preliminary", x, y, ax=ax, transform=transform, align=align, color="black", **kwargs)

def WIP(x=0.05, y=0.90, align='left', transform=None, ax=None, **kwargs):
    """
    Apply a "DUNE Work In Progress" label.

    See help on TextLabel() for the optional parameters.
    """
    TextLabel(DUNEWatermarkString() + " Work In Progress", x, y, ax=ax, transform=transform, align=align, color="black", **kwargs)

def Simulation(x=0.05, y=0.90, align='left', ax=None, transform=None, **kwargs):
    """
    Apply a "DUNE Simulation" label.

    See help on TextLabel() for the optional parameters.
    """
    TextLabel(DUNEWatermarkString() + " Simulation", x, y, ax=ax, transform=transform, align=align, color="black", **kwargs)

def SimulationSide(x=1.05, y=0.5, align='right', ax=None, transform=None, **kwargs):
    """
    Apply a "DUNE Simulation" label on the right outside of the frame.
    NOTE: the Simulation() version is heavily preferred over this one;
    this "outside-the-canvas" version exists for special cases when
    there are already too many other labels inside it.

    See on TextLabel() for the optional parameters.
    """
    TextLabel(DUNEWatermarkString() + " Simulation", x, y, ax=ax, transform=transform, align=align, rotation=270, color="black", **kwargs)

def Official(x=0.05, y=0.90, align='left', ax=None, transform=None, **kwargs):
    """
    Apply a "DUNE" label (for officially approved results only).

    See help on TextLable() for the optional parameters.
    """
    TextLabel(DUNEWatermarkString(), x, y, ax=ax, transform=transform, align=align, color="black", **kwargs)

def CornerLabel(label, ax=None, transform=None, **kwargs):
    """
    Apply a gray label with user-specified text on the upper-left corner (outside the plot frame).

    See help on TextLabel() for the optional parameters.
    """
    TextLabel(label, 0, 1.05, ax=ax, transform=transform, color="gray", **kwargs)

def SetDUNELogoColors():
    """ Set the color cycler to use the subset of Okabe-Ito colors that overlap with the DUNE logo colors. """

    from cycler import cycler
    cyc = cycler(color=['#D55E00', '#56B4E9', '#E69F00'])
    plt.rc("axes", prop_cycle=cyc)

def SetOkabeItoColors():
    """ Set the color cycler to use Okabe-Ito colors.. """

    from cycler import cycler
    cyc = cycler(color=['#000000', '#D55E00', '#56B4E9', '#E69F00', '#009E73', '#CC79A7', '#0072B2', '#F0E442',])
    plt.rc("axes", prop_cycle=cyc)

def OffWhiteBackground():
    """ Set the background color of the figure to match the off-white background of the updated DUNE slide template. """
    
    plt.rcParams['axes.facecolor'] = '#f0f0f0'
    plt.rcParams['figure.facecolor'] = '#f0f0f0'
    plt.rcParams['savefig.facecolor'] = '#f0f0f0'
