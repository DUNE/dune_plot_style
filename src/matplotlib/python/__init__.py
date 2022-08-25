import os
import matplotlib

mplconfigpath = os.path.dirname(os.path.realpath(__file__))+"/style/"

if 'MPLCONFIGDIR' in os.environ:
    os.environ['MPLCONFIGDIR'] = mplconfigpath + os.pathsep + os.environ['MPLCONFIGDIR']
else:
    os.environ['MPLCONFIGDIR'] = mplconfigpath

from .dunestyle import *
