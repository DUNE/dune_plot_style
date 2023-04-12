# `dune_plot_style` - DUNE official plot styling tools

This repository contains coding tools to help analyzers easily make plots adhering to the DUNE Plot Style documented at https://wiki.dunescience.org/wiki/DUNE_Plot_Styles.

It also contains tools to help embed metadata in plot images that contain "provenance" info so they can be back-traced if necessary.

To illustrate how to use those tools and what the results look like, some examples are also offered.

There are tools designed for use with the most common plotting technologies currently in use:

* ROOT (C++ backend)
* PyROOT
* matplotlib

We welcome bug reports, suggestions, and (especially) code contributions!
Please see [Contributing](#4-Contributing) below.



## 1. Installation

There are a few ways you can use `dune_plot_style`.

#### Fermilab UPS package

If you're working on a DUNE GPVM on Fermilab computing resources (`dunegpvmXX.fnal.gov`),
`dune_plot_style` is available as a UPS package
which automatically sets the relevant environment variables for you.

If you're not otherwise familiar with UPS, the relevant procedure goes something like:

```bash
# sets up the UPS system
$ source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

# list available versions
$ ups list -aK+ dune_plot_style

# set up a specific version
setup dune_plot_style v00_02
```

At this point you should be able to `#include "DUNEStyle.h"` or `from dunestyle import ...` as described in the following sections.

#### Standalone Python setup

`dune_plot_style` supports being set up as a standalone Python package.
You'll need to install a handful of common Python libraries for the examples to work.
The recommended way to install these packages is to set up a virtual environment.
This avoids potential package version conflicts and allows you to download the necessary packages on a remote server where you don't have root privileges, such as the GPVMs.

```
cd path/to/dune_plot_style/ # Or wherever you like to store virtual environments
python3 -m venv my_env
source /my_env/bin/activate
```

Next, download the desired version of `dune_plot_style` from [the GitHub releases page](https://github.com/DUNE/dune_plot_style/releases).
You can then install `dune_plot_style` and whatever other packages you need:

```bash
# dependencies first
python3 -m pip install matplotlib numpy scipy

# now dune_plot_style
cd /path/to/unpacked/tarball
python3 -m pip install .
```


At this point you should be able to `from dunestyle import ...` as described below.

#### Standalone C++ ROOT setup

A single header file provides the entire C++ ROOT interface: `src/root/cpp/include/DUNEStyle.h`.
You may download this file independently from the repository, or (recommended), download [a tagged source distribution](https://github.com/DUNE/dune_plot_style/releases).
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

Check out the source of [`DUNEStyle.h`](https://github.com/DUNE/dune_plot_style/blob/main/src/root/cpp/include/DUNEStyle.h)
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

To enable these, you'll need to install `dune_plot_style` as a Python module.
This will setup the `$MPLCONFIGDIR` environment variable to pick up the style sheet. 

To enable `dunestyle` in your scripts, simply

```python
import dunestyle.matplotlib as dunestyle
```

If you wish to delay the application of the DUNE style, you can use the same technique laid out in the [PyROOT](#pyroot) section above.

See the [examples](#3-examples) for more ideas of what you can do.

## 3. Examples

There are example scripts for ROOT C++, PyROOT, and Matplotlib in the `examples/` directory.
They are built automatically using GitHub's Continuous Integration suite (see [Continuous integration](#5-continuous-integration), below).
You can also generate them yourself to see how they work.
Similary

### ROOT

There are C++ and PyROOT examples in `dune_plot_style/examples/root/cpp/example.C` and `dune_plot_style/examples/root/cpp/example.py`, respectively. To run the C++ version, ensure your `$ROOT_INCLUDE_PATH` is set to include the appropriate directory (see [Installation](#1-installation) above).
Then, you can simply
```c++
root -l -b -q example.C
```

Running the PyROOT version is similar.  First ensure your `$PYTHONPATH` is set correctly (again see [Installation](#1-installation) above), then:
```python
python3 example.py
```

The output (which is identical between them) is illustrated below.

<div style="text-align: center">
<a href="examples/images/example.root.datamc.png"><img src="examples/images/example.root.datamc.png" width="30%" ></a>
<a href="examples/images/example.root.datamc.png"><img src="examples/images/example.root.hist1D.png" width="30%" ></a>
<a href="examples/images/example.root.datamc.png"><img src="examples/images/example.root.hist2D.png" width="30%" ></a>

<a href="examples/images/example.root.histoverlay.png"><img src="examples/images/example.root.histoverlay.png" width="30%" ></a>
<a href="examples/images/example.root.histstacked.png"><img src="examples/images/example.root.histstacked.png" width="30%" ></a>
</div>


### matplotlib

The matplotlib example script can be found in `dune_plot_style/examples/matplotlib`. It creates a handful of common plot types used in HEP, including stacked histograms, data-to-simulation comparisons, and 2D histograms with confidence contours drawn. To run the example script and produce some plots, simply run

```
python3 example.py
```

from the `examples/matplotlib` subdirectory. Note that you'll need to have `matplotlib`, `numpy`, and `scipy` installed for this to work (see instructions above).  The `matplotlib` versions of the example plots are shown below.

<div style="text-align: center">
<a href="examples/images/example.matplotlib.datamc.png"><img src="examples/images/example.matplotlib.datamc.png" width="30%" ></a>
_<a href="examples/images/example.matplotlib.hist1D.png"><img src="examples/images/example.matplotlib.hist1D.png" width="30%"></a>_
<a href="examples/images/example.matplotlib.hist2D.png"><img src="examples/images/example.matplotlib.hist2D.png" width="30%"></a>

<a href="examples/images/example.matplotlib.histoverlay.png"><img src="examples/images/example.matplotlib.histoverlay.png" width="30%"></a>
<a href="examples/images/example.matplotlib.histstacked.png"><img src="examples/images/example.matplotlib.histstacked.png" width="30%"></a>
</div>

## 4. Contributing

If you encounter problems, have a suggestion, or (especially) want to contribute an enhancement or bug-fix,
please use the GitHub tools.

* For bug reports or feature requests, please [file an Issue](https://github.com/DUNE/dune_plot_style/issues).
* To contribute code, please [open a Pull Request](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request).

# 5. Continuous integration

[![matplotlib example status](https://github.com/DUNE/dune_plot_style/actions/workflows/matplotlib.yml/badge.svg)](https://github.com/DUNE/dune_plot_style/actions/workflows/main.yml)
[![ROOT example status](https://github.com/DUNE/dune_plot_style/actions/workflows/root.yml/badge.svg)](https://github.com/DUNE/dune_plot_style/actions/workflows/main.yml)

To ensure that the examples can be run with the current style,
they are automatically rerun using Github Actions,
in the `ROOT CI` and `Matplotlib CI` workflows,
which are defined in `.github/workflows/*.yml`.
Both workflows cache the dependencies and run automatically for every update to main and for all pull requests,
if files that might affect them were modified.
A whitelist of paths to watch is defined in each workflow.
(The output are stored in the Workflow output directory---see the links above---but the branch is not automatically updated.  Contributions that would automate the branch update are welcome!)

For ROOT, the dependencies are defined in `conda.yml` while the matplotlib dependencies are defined directly in the workflow.

As the repository is public, the CI does not count against the DUNE quota, and both workflows run on Github's public Ubuntu runners.
