# `dune-plot-style` - DUNE official plot styling tools

This repository contains coding tools to help analyzers easily make plots adhering to the DUNE Plot Style documented at https://wiki.dunescience.org/wiki/DUNE_Plot_Styles.
(Those guidelines are also duplicated in this repository as [`guidelines.md`](https://github.com/DUNE/dune-plot-style/blob/main/guidelines.md).)

It also contains tools to help embed metadata in plot images that contain "provenance" info so they can be back-traced if necessary.

To illustrate how to use those tools and what the results look like, some examples are also offered.

There are tools designed for use with the most common plotting technologies currently in use:

* ROOT (C++ backend)
* PyROOT
* matplotlib

We welcome bug reports, suggestions, and (especially) code contributions!
Please see [Contributing](#4-Contributing) below.



## 1. Installation

There are a few ways you can use `dune-plot-style`.

#### Fermilab UPS package

If you're working on a DUNE GPVM on Fermilab computing resources (`dunegpvmXX.fnal.gov`),
`dune-plot-style` is available as a UPS package
which automatically sets the relevant environment variables for you.

If you're not otherwise familiar with UPS, the relevant procedure goes something like:

```bash
# sets up the UPS system
$ source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

# list available versions
$ ups list -aK+ dune-plot-style

# set up a specific version
setup dune-plot-style v00_02
```

At this point you should be able to `#include "DUNEStyle.h"` or `from dunestyle import ...` as described in the following sections.

#### Standalone Python setup

`dune-plot-style` supports being set up as a standalone Python package.
You'll need to install a handful of common Python libraries for the examples to work.
The recommended way to install these packages is to set up a virtual environment.
This avoids potential package version conflicts and allows you to download the necessary packages on a remote server where you don't have root privileges, such as the GPVMs.

```
cd path/to/dune-plot-style/ # Or wherever you like to store virtual environments
python3 -m venv my_env
source /my_env/bin/activate
```

Next, download the desired version of `dune-plot-style` from [the GitHub releases page](https://github.com/DUNE/dune-plot-style/releases).
You can then install `dune-plot-style` and whatever other packages you need:

```bash
# dependencies first
python3 -m pip install matplotlib numpy scipy

# now dune-plot-style
cd /path/to/unpacked/tarball
python3 -m pip install .
```


At this point you should be able to `from dunestyle import ...` as described below.

#### Standalone C++ ROOT setup

A single header file provides the entire C++ ROOT interface: `src/root/cpp/include/DUNEStyle.h`.
You may download this file independently from the repository, or (recommended), download [a tagged source distribution](https://github.com/DUNE/dune-plot-style/releases).
Then, simply copy it to wherever you would like it to live.

If you are using it exclusively with ROOT macros, you'll need to ensure that the directory where `DUNEStyle.h` is located
is included in the environment variable `$ROOT_INCLUDE_PATH`.

If you prefer to build a standalone C++ application/executable, you'll need to ensure the directory where `DUNEStyle.h` is located
is visible to your build system.  For example, with `gcc` or `g++` you'll want to include that directory with `-I`.


## 2. How to use the stylistic coding tools

### ROOT (C++)

##### Regular use
There's a C++ header in this package called `DUNEStyle.h`.
Simply `#include` it in your ROOT script(s) to have all subsequent plots take on the basics of DUNE plot style.
(See the [installation](#1-installation) section above for more information on how to make it available).

There are a few stylistic items you'll have to enforce by hand, however.
These have dedicated functions you can invoke:

* "DUNE Simulation", "DUNE Preliminary", "DUNE Work In Progress" watermarks
* Centering axis titles
* Choosing appropriate palettes for "colz" plots

Check out the source of [`DUNEStyle.h`](https://github.com/DUNE/dune-plot-style/blob/main/src/root/cpp/include/DUNEStyle.h)
  for one-stop functions you can call to get this behavior.  They have in-line Doxygen style comments explaining how to use them.
The [examples](#3-examples) noted below also show how to use them.

##### Not applying by default

There may be situations in which you prefer not to enforce the DUNE style on every plot that is made in a particular macro.
In that case, you may set a compile-time define to disable automatic application:

```c++
#define DUNESTYLE_ENABLE_AUTOMATICALLY 0
```

Then, when you're ready, you can apply the DUNE style to any subsequent plots made by calling

```c++
dunestyle::SetDuneStyle();
```

### PyROOT

PyROOT comes pre-installed with most ROOT builds these days, but you should still check that your PyROOT and Python versions are the same:

```bash
root-config --has-pyroot # Gives "yes"

# The output of these next two lines should match
root-config --python3-version 
python3 --version 
```

The PyROOT style tools are simply a wrapper around the C++ ones, and behave the same way once invoked, so see the documentation above for more information about them.

To apply the DUNE style, once you've completed the [installation](#1-installation), all you need is to import the `dunestyle` module:

```python
import dunestyle.root as dunestyle
```

Again the [examples](#3-examples) illustrate more of what you can do.

If you wish to delay the application of the DUNE style, similarly to what's described in the [C++ section](#root-c++), you can set a global flag before importing the `dunestyle` module:

```python
import builtins  #  this only works with Python3...
builtins.__dict__["DUNESTYLE_ENABLE_AUTOMATICALLY"] = False
import dunestyle.root as dunestyle
```

after which you can

```python
dunestyle.enable()
```

to turn it on.

### matplotlib

The matplotlib style tools consist of two parts:
* a ["style sheet"](https://matplotlib.org/stable/tutorials/introductory/customizing.html#using-style-sheets) file which sets most of the default stylings
* an importable module which contains functions to apply watermarks, etc.  This module also applies the style sheet by default (this behavior can be disabled using the same mechanism as described in the [PyROOT section](#pyroot), above).

To enable these, you'll need to install `dune-plot-style` as a Python module.
This will setup the `$MPLCONFIGDIR` environment variable to pick up the style sheet. 

To enable `dunestyle` in your scripts, simply

```python
import dunestyle.matplotlib as dunestyle
```

If you wish to delay the application of the DUNE style, you can use the same technique laid out in the [PyROOT](#pyroot) section above.

See the [examples](#3-examples) for more ideas of what you can do.

## 3. Examples


[![Example status](https://github.com/DUNE/dune-plot-style/actions/workflows/main.yml/badge.svg)](https://github.com/DUNE/dune-plot-style/actions/workflows/main.yml)

_[**todo**: include images from `examples/` dir.  also point out how the various features were obtained with the code in `examples/` ]_

_[**todo**: add ROOT and PyROOT examples]_

### matplotlib

The matplotlib example script can be found in `dune-plot-style/examples/matplotlib`. It creates a handful of common plot types used in HEP, including stacked histograms, data-to-simulation comparisons, and 2D histograms with confidence contours drawn. To run the example script and produce some plots, simply run

```
python3 example.py
```

from the `examples/matplotlib` subdirectory. Note that you'll need to have `matplotlib`, `numpy`, and `scipy` installed for this to work (see instructions above). Some of the example plots are shown below.

<a href="url"><img src="https://github.com/DUNE/dune-plot-style/blob/feature/amogan_mplstyle/examples/images/example.matplotlib.gaus.png" align="left" height="256" ></a>
<a href="url"><img src="https://github.com/DUNE/dune-plot-style/blob/feature/amogan_mplstyle/examples/images/example.matplotlib.datamc.png" align="left" height="256" ></a>

<a href="url"><img src="https://github.com/DUNE/dune-plot-style/blob/feature/amogan_mplstyle/examples/images/example.matplotlib.hist2D.png" align="left" height="256" ></a>
<a href="url"><img src="https://github.com/DUNE/dune-plot-style/blob/feature/amogan_mplstyle/examples/images/example.matplotlib.histstacked.png" align="left" height="256" ></a>

## 4. Contributing

If you encounter problems, have a suggestion, or (especially) want to contribute an enhancement or bug-fix,
please use the GitHub tools.

* For bug reports or feature requests, please [file an Issue](https://github.com/DUNE/dune-plot-style/issues).
* To contribute code, please [open a Pull Request](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request).


