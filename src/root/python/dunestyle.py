""" DUNE plot style tools for use with (Py)ROOT.

This module simply imports the style defined in the C++ header version of the style.
By default it will be enabled immediately on import.

If you do not want it enabled on import, set the global:
```
import builtins  #  this only works with Python3...
builtins.__dict__["DUNESTYLE_ENABLE_AUTOMATICALLY"] = False
import dunestyle.root as dunestyle
```
Then you can call dunestyle.enable() to turn it on.

:author: J. Wolcott <jwolcott@fnal.gov>
:date:   March 2022
"""

import builtins

_CPP_HEADER = "DUNEStyle.h"
_UPS_VAR = "DUNE_PLOT_STYLE_INC"

# unfortunately child namespaces seem not to be loaded by default
_CHILD_NAMESPACES = [
	"colors",
]


def enable():
	import os.path
	import sys
	import ROOT


	search_paths = [os.path.curdir, os.path.join(os.path.dirname(__file__), "../cpp/include")]
	try:
		from dunestyle import data as data_module
		search_paths.insert(0, data_module.__path__[0])
	except:
		pass

	if _UPS_VAR in os.environ:
		search_paths.insert(0, os.environ[_UPS_VAR])

	found_path = None
	for search_paths in search_paths:
		fullpath = os.path.join(search_paths, _CPP_HEADER)
		if os.path.isfile(fullpath):
			found_path =  fullpath
			break

	if found_path:
		ROOT.gInterpreter.LoadFile(found_path)
	else:
		raise FileNotFoundError("Cannot find DUNE style header '%s'" % _CPP_HEADER)

	# grab all the functions out of the cpp file from ROOT's namespace
	for obj in dir(ROOT.dunestyle):
		if obj.startswith("_"):
			continue
		setattr(sys.modules[__name__], obj, getattr(ROOT.dunestyle, obj))

	for ns in _CHILD_NAMESPACES:
		try:
			setattr(sys.modules[__name__], ns, getattr(ROOT.dunestyle, ns))
		except NameError:
			pass

	print("DUNE plot style enabled")


_IMPORT_FLAG_NAME = "DUNESTYLE_ENABLE_AUTOMATICALLY"
if _IMPORT_FLAG_NAME not in builtins.__dict__ or builtins.__dict__[_IMPORT_FLAG_NAME]:
	enable()
