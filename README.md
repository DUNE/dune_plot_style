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

### Fermilab UPS package

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

### Standalone Python setup



`dune_plot_style` supports being set up as a standalone Python package.
You'll need to install a handful of common Python libraries for the examples to work.
The recommended way to install these packages is to set up a virtual environment.
This avoids potential package version conflicts and allows you to download the necessary packages on a remote server where you don't have root privileges, such as the GPVMs.

##### Check Python version prerequisites

**`dune_plot_style` requires Python >= 3.9.**
(If you attempt to use an older version, you may encounter issues setting up the dependency chain below.)
You can check what version is currently set up using
```bash
python --version
```

If you'll be using `dune_plot_style` on a machine you control, use your operating system package manager or other suitable means to obtain an appropriate version of Python.

<details><summary>If you are installing on a DUNE GPVM, follow these steps instead</summary>
You will need to set up a more recent version of Python than the base system version.
The easiest way to do this is to use the UPS system.  First, set that up:

```bash
# sets up the UPS system
$ source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
```

* If you intend to use `dune_plot_style` in conjunction with ROOT, simply set up ROOT, as that comes bundled with a Python version dependency.
```bash
# this is the most recent as of the writing of these instructions.
# list all possibilities using `ups list -aK+ root`
$ setup root v6_22_08d -q e20:p392:prof 
```
* If instead you have no interest in setting up ROOT, you can simply set up the Python version you want directly:
```bash
# again, list all possibilities with `ups list -aK+ python`.
# note that a minimum of 3.9 is required
$ setup python v3_9_2
```

**note that these steps will need to be executed for every use**, prior to entering any virtual environments created in the following steps. 
</details>

##### Set up virtual environment and install

```
cd /path/to/venvs # wherever you like to store virtual environments
python3 -m venv my_env
source my_env/bin/activate
```

Next, download the desired version of `dune_plot_style` from [the GitHub releases page](https://github.com/DUNE/dune_plot_style/releases).
You can then install `dune_plot_style` and whatever other packages you need:

```bash
# dependencies first
python3 -m pip install matplotlib numpy scipy

# this is one way to obtain the tarball, but use any way you like
cd /path/to/install/area
export DUNE_PLOT_STYLE_LATEST_TAG=`curl --silent "https://api.github.com/repos/DUNE/dune_plot_style/releases" | jq -r 'map(select(.prerelease == false)) | first | .tag_name'`
wget --no-check-certificate https://github.com/DUNE/dune_plot_style/archive/refs/tags/${DUNE_PLOT_STYLE_LATEST_TAG}.tar.gz -O dune_plot_style.tar.gz
tar -xvzf dune_plot_style.tar.gz

# obviously adjust the directory name for whatever came out of the tarball
cd /path/to/install/area/dune_plot_style
python3 -m pip install .
```

At this point you should be able to `from dunestyle import ...` as described below.

##### Subsequent use

You'll need to set up your virtual environment (and, if on a GPVM, you'll need the UPS setup for Python or Root before that) as noted in the previous steps.
You won't need to run the installation instructions more than once, however.

### Standalone C++ ROOT setup

A single header file provides the entire C++ ROOT interface: `src/root/cpp/include/DUNEStyle.h`.
You may download this file independently from the repository, or (recommended), download [a tagged source distribution](https://github.com/DUNE/dune_plot_style/releases).
Then, simply copy it to wherever you would like it to live.

If you are using it exclusively with ROOT macros, you'll need to ensure that the directory where `DUNEStyle.h` is located
is included in the environment variable `$ROOT_INCLUDE_PATH`.
(If you're using the UPS package as described in the previous section, this is done for you automatically.)

If you prefer to build a standalone C++ application/executable, you'll need to ensure the directory where `DUNEStyle.h` is located
is visible to your build system.  For example, with `gcc` or `g++` you'll want to include that directory with `-I`.
This might look like:
```bash
# if you're using the UPS package
g++ -o mytest -I $DUNE_PLOT_STYLE_INC mytest.C

# if you installed by hand
g++ -o mytest -I /path/to/dune_plot_style/src/root/cpp/include mytest.C
```


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


[![Example status](https://github.com/DUNE/dune_plot_style/actions/workflows/main.yml/badge.svg)](https://github.com/DUNE/dune_plot_style/actions/workflows/main.yml)

_[**todo**: include images from `examples/` dir.  also point out how the various features were obtained with the code in `examples/` ]_

_[**todo**: add ROOT and PyROOT examples]_

### matplotlib

The matplotlib example script can be found in `dune_plot_style/examples/matplotlib`. It creates a handful of common plot types used in HEP, including stacked histograms, data-to-simulation comparisons, and 2D histograms with confidence contours drawn. To run the example script and produce some plots, simply run

```
python3 example.py
```

from the `examples/matplotlib` subdirectory. Note that you'll need to have `matplotlib`, `numpy`, and `scipy` installed for this to work (see instructions above).  The `matplotlib` versions of the example plots are shown below.

<a href="url"><img src="https://github.com/DUNE/dune_plot_style/blob/main/examples/images/example.matplotlib.datamc.png" align="left" height="256" ></a>
<a href="url"><img src="https://github.com/DUNE/dune_plot_style/blob/main/examples/images/example.matplotlib.hist1D.png" align="left" height="256" ></a>
<a href="url"><img src="https://github.com/DUNE/dune_plot_style/blob/main/examples/images/example.matplotlib.hist2D.png" align="left" height="256" ></a>

<a href="url"><img src="https://github.com/DUNE/dune_plot_style/blob/main/examples/images/example.matplotlib.histoverlay.png" align="left" height="256" ></a>
<a href="url"><img src="https://github.com/DUNE/dune_plot_style/blob/main/examples/images/example.matplotlib.histstacked.png" align="left" height="256" ></a>

## 4. Contributing

If you encounter problems, have a suggestion, or (especially) want to contribute an enhancement or bug-fix,
please use the GitHub tools.

* For bug reports or feature requests, please [file an Issue](https://github.com/DUNE/dune_plot_style/issues).
* To contribute code, please [open a Pull Request](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request).

---

Copyright © 2023 FERMI NATIONAL ACCELERATOR LABORATORY for the benefit of the DUNE Collaboration.

This repository, and all software contained within, is licensed under the Apache
License, Version 2.0 (the "License"); you may not use this file except in compliance
with the License. You may obtain a copy of the License at



`http://www.apache.org/licenses/LICENSE-2.0`



Copyright is granted to FERMI NATIONAL ACCELERATOR LABORATORY on behalf of the Deep
Underground Neutrino Experiment (DUNE). Unless required by applicable law or agreed to
in writing, software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
License for the specific language governing permissions and limitations under the
License.
