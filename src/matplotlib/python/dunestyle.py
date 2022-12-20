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

	path = os.environ['MPLCONFIGDIR'].split(':')[0]+"/dune.mplstyle"
	assert os.path.exists(path), "Can't locate DUNE matplotlib style sheet file!  I tried path: " + path
	plt.style.use(path)
	print("DUNE plot style enabled")


import builtins
_IMPORT_FLAG_NAME = "DUNESTYLE_ENABLE_AUTOMATICALLY"
if _IMPORT_FLAG_NAME not in builtins.__dict__ or builtins.__dict__[_IMPORT_FLAG_NAME]:
    enable()


##########   Utility functions below  ################

""" Used in the text functions below.  don't  """
def _GetTransform(transform=None, ax=None):
    if transform is not None:
        return transform
    if ax is not None and hasattr(ax, "transAxes"):
        return ax.transAxes
    return plt.gca().transAxes

def TextLabel(text, x, y, transform=None, ax=None, **kwargs):
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

def Preliminary(x=0.05, y=0.90, align='left', transform=None, ax=None, **kwargs):
    TextLabel("DUNE Preliminary", x, y, ax=ax, transform=transform, align=align, color="blue", **kwargs)

def WIP(x=0.05, y=0.90, align='left', transform=None, ax=None, **kwargs):
    TextLabel("DUNE Work In Progress", x, y, ax=ax, transform=transform, align=align, color="blue", **kwargs)

def Simulation(x=0.05, y=0.90, align='left', ax=None, transform=None, **kwargs):
    TextLabel("DUNE Simulation", x, y, ax=ax, transform=transform, align=align, color="gray", **kwargs)

def SimulationSide(x=1.05, y=0.5, align='right', ax=None, transform=None, **kwargs):
    TextLabel("DUNE Simulation", x, y, ax=ax, transform=transform, align=align, rotation=270, color="gray", **kwargs)

def CornerLabel(label, ax=None, transform=None, **kwargs):
    TextLabel(label, 0, 1.05, ax=ax, transform=transform, color="gray", **kwargs)
