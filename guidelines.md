# DUNE plotting guidelines and recommendations

_Last revision_: April 2023  by Plot Style Guidelines Task Force

## Introduction
The following set of guidelines have been devised by a Plot Style Guidelines Task Force, 
in conjunction with the Authorship and Publications Board (APB), and revised with feedback from the collaboration.
The main goals of the guidelines are to encourage:
* DUNE plots being accessible to the widest possible audience
* DUNE plots having a common visual theme and being quickly recognizable as DUNE plots 

These guidelines apply specifically to plots used in a public-facing context (presentations or papers shared outside the collaboration).
While we encourage the use of the tools described below by collaborators
even in collaboration internal settings, so as to have more accessible plots produced by default, it is not mandatory
(and in informal settings the recommendations' usefulness should be weighed against the technical workload they impose).

We split these guidelines into two parts, [requirements](#requirements) and [recommendations](#recommendations).
Requirements are mandatory unless there is a strong reason for not complying with them.
Recommendations are highly encouraged, but not mandatory; abiding by them will help collaborators prepare
public-quality plots conforming to the Requirements.  They also indicate important points to consider in plot preparation.

The approval process for plots is out of scope of this document and covered elsewhere.

Included with the guidelines is a collection of software tools, gathered together as the 
[`dune_plot_style`](https://github.com/DUNE/dune_plot_style) package, 
which offers sensible defaults and helper functions for `ROOT` and `matplotlib`, to aid in following these recommendations.
Please see its [`README`](https://github.com/DUNE/dune_plot_style/blob/main/README.md), 
or its [`examples/`](https://github.com/DUNE/dune_plot_style/tree/main/examples) directory, 
for more on how to use the technical tools.
Contributions to `dune_plot_style` for other plotting packages are welcome---please
[create a branch](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-branches) 
with your proposed changes and [open a pull request](https://github.com/DUNE/dune_plot_style/compare)!

## Requirements
All plots **must**:

- **Be easily readable to their intended audience**.  This affects: text size, font choice, colors, line styles, etc.   
  See [recommendations](#recommendations) below for further advice on what this might mean in practice.
- **Be labelled with the appropriate watermark**:     
  _DUNE_, _DUNE Preliminary_, _DUNE Work In Progress_, or _DUNE Simulation_.  (See the [official APB policy](https://docs.dunescience.org/cgi-bin/private/ShowDocument?docid=1115) for when each is appropriate.)  
  Use the `Official()`, `Preliminary()`, `WIP()`, or `Simulation()` functions from `dune_plot_style` to obtain them.
- **Be saved in at least in one vector-based format** (e.g., pdf or eps),  
  and **at least one high-quality raster-based format** (e.g., png).
- For publications, **be stylistically internally consistent within documents**.

## Recommendations

### General advice

- Use both color and style to differentiate parts of plots.
- For reproducibility and adaptability, keep the code generating the plot in version control.
  (If this is not possible, distributing the .C macro version of the plot alongside it can suffice in a pinch.)
- Ensure all numbers shown have only a reasonable number of significant figures.   
  Prefer readability over extra information.
- Avoid large empty regions in plots. (Rescale axes as necessary.)
- Strive to avoid jargon as much as possible. For instance: use “simulation” or “sim.” instead of “MC”.
- When comparing to external datasets, ensure that DUNE is highlighted.
- Avoid ROOT's default statistics box. Instead, pull out any pertinent information and add it, properly formatted, to the plot.
- Use an axis length ratio of y/x between 0.7 and 0.75 unless other considerations prevail.  (This is default behavior in both ROOT and matplotlib.)

### Annotations
- Prefer embedding relevant information in plot axes or annotations inside the figure itself,
  rather than using plot titles (above the top of the figure). The latter can be accidentally cropped out of a figure and are often overlooked.  (However, in sufficiently complex figures this guidance may be relaxed.)
- Where appropriate, add explicit identifying information of the plot (eg Year, POT).
- Font sizes should be comparable to that of the axis labels to ensure readability.

### Font and font size

- Use a sans serif font.  (`dune_plot_style` uses Helvetica by default.)
- Use consistent fonts across a single plot.
- Limit the number of fonts and text styles (bold, italic, underlined, etc.)

#### For presentations & posters:

- Use large fonts for axis labels, legend entries, etc.  (The `dune_plot_style` package’s tools do this by default.)

#### For papers:

- Check journal style guidelines carefully.
- Ensure plot text size is similar to that in the body of the article as possible. 

### Colo(u)r considerations:
Color choices should look appealing and be accessible to those with color-vision deficiency (CVD).

#### Plot backgrounds:

The high contrast resulting from fully saturated colors against a white background can be difficult for individuals with dyslexia.
Consider using an "off-white" background instead.
The functions `OffWhiteBackground()` in both ROOT and matplotlib toolkits in this package will set figure backgrounds 
to match the off-white background in the [official DUNE slide template](https://docs.dunescience.org/cgi-bin/ShowDocument?docid=78).

#### Markers/lines:

- When discrete curves are shown, the [Okabe-Ito color cycle](https://jfly.uni-koeln.de/color/) is recommended.
  (`dune_plot_style` sets the default matplotlib cycler to Okabe-Ito; 
   the `dunestyle::colors::NextColor()` function can be used to obtain this cycle in ROOT.)
  The chosen ordering for Okabe-Ito is intended to separate colors that appear visually similar to individuals
  with various types of CVD, but if other considerations prevail, the order may be rearranged. 
  If only a subset of colors is needed, we encourage using the second, third, and fourth members of the cycle, 
  which correspond to the DUNE logo colors.  (Use `NextColor(Cycle::DUNELogo)` in the ROOT version 
  or `SetDUNELogoColors()` in the matplotlib version of `dune_plot_style` to make this happen.)
- For continuous color ranges (e.g. z-axis of 2D histograms), the [cividis color palette](https://journals.plos.org/plosone/article?id=10.1371/journal.pone.0199239)
  is recommended.  Cividis avoids common pitfalls,  is designed with CVD in mind,
  and is available by default in `ROOT` and `matplotlib`.
  `dune_plot_style` sets the default palette to cividis.
- For special situations, monochrome (e.g.: white-to-red) or bichrome (e.g.: blue-to-white-to-red)
  color scales may be appropriate.  (E.g.: bichrome is suitable for covariance or correlation matrices.)
  `dune_plot_style` again has helper functions to facilitate this.
- If you choose other colors than those recommended above, be sure to check their reproduction in a number of situations
  (screen, projector, monochrome print) as they may not appear the same way everywhere.  We also recommend passing them through
  a CVD simulator or two on the web to get a feel for how your color choices may appear to individuals with various CVDs.

Please see the `README.md` and the `examples/` subdirectory of `dune_plot_style` for technical guidance
on how to employ the helper functions.

### Axis labels & titles:

- Use consistent capitalization. 
Whether “Title Case” or “Sentence case” are used,
it should be the same on all axes of a plot.
- Always include units in labels unless a quantity is unitless.
- Titles should be centered along the axes.
- When histograms have variable binning or a bin width not divisible by the axis tick divisions, the "counts" axis
  (y (z)-axis for a 1 (2)-D histogram) should include the bin width: e.g., "Events / 10 MeV". 

### Lines and markers:

- Data should generally be indicated by black solid points with error bars unless there is a reason to do otherwise.
- Refrain from connecting points/markers unless there’s a good reason to do so.
- Models/simulation should be lines or step-histograms to differentiate from data (unless statistical uncertainty of MC sample is significant).
- When multiple lines or markers are shown, use distinct line or marker styles to distinguish them in addition to color choices. This aids in accessibility. 
- Lines should be thick enough (2pt or thicker) to be easily read from a distance.
- Markers with different styles should be similar sizes unless some other consideration prevails.
- When error bars are not used, solid histograms or lines are recommended to improve readability.
- Consider whether using separate statistical and systematic error bars make sense in the context of your plot.
- Prefer using error bands for simulation and error bars for data.


### Fills:

- Choose hatching or fill patterns carefully.
Very often they can make plots more difficult to read rather than easier.
- If showing multiple histograms in an un-stacked fashion,
avoid fills or use transparency.

### Legends:

- Use capitalization consistent with axis labels (see above).
- Font size should be comparable to that of the axis labels to ensure readability.
- When error bars are used,
the type of errors (statistical vs. systematic) should be indicated in the legend entry.
- Items should appear in the same top-to-bottom order as they are drawn in a histogram stack, if applicable.

### Special guidance for Event Displays
- Add Run/Subrun/Event and date text overlaying event display.
- Overlay DUNE logo.
- Use standardized rainbow colour scheme (blue-green-yellow-red).


## Useful Links
- [Colorcet](https://colorcet.holoviz.org) python package. Has a number of color maps to use, though care should be taken to make sure these maps fit with the spirit of the rest of these guidelines.
