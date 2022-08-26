try:
    # this handles the arrangement that comes out
    #  when dune-plot-style is set up standalone by using `pip install`.
    # (the UPS product sets $MPLCONFIGDIR directly in its setup process.)
    import os, os.path, pathlib
    import dunestyle.stylelib as module
    os.environ["MPLCONFIGDIR"] = str(pathlib.Path(module.__path__[0]).parent.absolute()) + (":" + os.environ["MPLCONFIGDIR"] if "MPLCONFIGDIR" in os.environ else "")
except:
    pass

from .dunestyle import *
