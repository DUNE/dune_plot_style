try:
    # This conceit handles the arrangement that comes out
    #  when dune_plot_style is set up standalone by using `pip install -e`.
    # You may notice that this file is very similar to the one in the python/ subdirectory;
    # that's because when installed with a regular `pip install` (without the `-e`),
    # this file is not installed anywhere, and is not needed.
    # It's only needed because the `install -e` invocation, instead of doing an install,
    # simply points back to the package, so the directory structure is not rearranged as it is in installation.
    import os, os.path, pathlib
    from . import stylelib as module
    os.environ["MPLCONFIGDIR"] = os.path.join(str(pathlib.Path(module.__path__[0]).parent.absolute()), "stylelib") + (os.pathsep + os.environ["MPLCONFIGDIR"] if "MPLCONFIGDIR" in os.environ else "")
except:
    pass

from .python import *