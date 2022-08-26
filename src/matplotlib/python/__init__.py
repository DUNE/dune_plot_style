try:
    import os, os.path, pathlib
    import dunestyle.stylelib as module
    os.environ["MPLCONFIGDIR"] = str(pathlib.Path(module.__path__[0]).parent.absolute()) + (":" + os.environ["MPLCONFIGDIR"] if "MPLCONFIGDIR" in os.environ else "")
except:
    pass

from .dunestyle import *
