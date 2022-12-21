# DUNE plotting guidelines and recommendations

We split these guidelines into two parts, [requirements](#requirements) and [recommendations](#recommendations).
Requirements should be followed unless there is a good reason not to,
while recommendations help you prepare publication-quality plots conforming to the guidelines,
and give additional advice and points to consider.

The approval process for plots is out of scope of this document and covered elsewhere.

The `dune-plot-style` package offers sensible defaults for `ROOT` and `matplotlib`, to aid following these recommendations.
Please see its `README`, or its `examples/` directory, for more on how to use the technical tools.
Contributions to `dune-plot-style` for other plotting packages are welcome.

## Requirements

- Plots should be easily readable to their intended audience.
All text, including axis labels and titles, should be large enough to be read.
Note, that the size and other properties of the plot might need to be adapted for presentations or papers,
as ideal text sizes etc. depend on the use.
- All plots should be labelled with the appropriate water mark (typically one of the following:
DUNE, DUNE Preliminary, DUNE Work In Progress, DUNE Simulation).
- For reusability,
please save plots at least in one lossless format,
ideally vector-based (e.g. pdf, eps),
in addition to high-quality raster-based versions (.png).
It is recommended to avoid lossy compression such as .jpg.
For reproducibility and adaptability,
it is also recommended to keep the code generating the plot in version control,
and/or distributing the .C macro version of the plot alongside it.

## Recommendations

- Using plot titles (above the top of the figure) is discouraged as they are often overlooked.
Prefer embedding relevant information in plot axes or annotations inside the figure itself.
- If any numbers are displayed on a plot,
ensure they have only a reasonable number of significant figures. 
Prefer readability over extra information.
- Avoid large empty regions in plots.
(Rescale axes as necessary.)
- Strive to avoid jargon as much as possible.
For instance: use “simulation” or “sim.” instead of “MC”

### Font and font size

#### For presentations & posters:

- Use a sans serif font (e.g.: Arial)
- Use large fonts for axis labels, legend entries, etc.  (The `dune-plot-style` package’s tools do this by default.)

#### For papers:

- Check journal style guidelines carefully.
- Ensure plot text is at least as large as that in the body of the article. 

### Colo(u)r palette:
Colo(u)r choices should look appealing and be accessible to those with colour-vision deficiency (CVD).
- When discrete curves are shown, the Okabe-Ito color cycle is recommended.
  (`dune-plot-style` sets the default matplotlib cycler to Okabe-Ito; 
   the `dunestyle::colors::NextColor()` function can be used to obtain this cycle in ROOT.)
- For continuous color ranges (e.g. z-axis of 2D histograms), the cividis color palette is recommended.
  Cividis avoids common pitfalls,  is designed with CVD in mind,
  and is available by default in `ROOT` and `matplotlib`.
  `dune-plot-style` sets the default palette to cividis.
- For special situations, monochrome (e.g.: white-to-red) or bichrome (e.g.: blue-to-white-to-red)
  color scales may be appropriate.  (E.g.: bichrome is suitable for covariance or correlation matrices.)
  `dune-plot-style` again has helper functions to facilitate this.

Please see the `examples/` subdirectory of `dune-plot-style` for technical guidance
on how to employ the helper functions.

### Axis labels & titles:

- Use consistent capitalization. 
Whether “Title Case” or “Sentence case” are used,
it should be the same on all axes of a plot.
- Always include units in labels unless a quantity is unitless.
- Titles should be centered along the axes.

### Lines and markers:

- Data should generally be indicated by black solid points with error bars unless there is a reason to do otherwise.
- Models/simulation should be lines or step-histograms to differentiate from data (unless statistical uncertainty of MC sample is significant).
- Refrain from connecting points/markers unless there’s a good reason to do so.
- Lines should be thick enough (2pt or thicker) to be easily read from a distance.
- When markers are used instead of lines,
markers for different distributions or curves should be distinct shapes (do not rely on colour alone to separate them).
- Markers with different shapes should be similar sizes unless some other consideration prevails.
- When error bars are not used,
solid histograms or lines are recommended to improve readability.

### Fills:

- Choose hatching or fill patterns carefully.
Very often they can make plots more difficult to read rather than easier.
- If showing multiple histograms in an un-stacked fashion,
avoid fills or use transparency.

### Legends:

- Use capitalization consistent with axis labels (see above).
- Font size should generally be at least as large as axis labels. (Ensure readability)
- When error bars are used,
the type of errors (statistical vs. systematic) should be indicated in the legend entry.
- Items should appear in the same order as they are drawn in a histogram stack, if applicable.
