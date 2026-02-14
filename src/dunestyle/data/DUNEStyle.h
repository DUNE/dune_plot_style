///  \file   DUNEStyle.h
///  \brief  Placeholder with plot style for files in this package until DUNE has a central style defined
///  \author J. Wolcott <jwolcott@fnal.gov>
///  \date   March 2022
///
///  \note  This style will be automatically applied to any plots made after `#include`ing it.
///         If you don't want that, set the preprocessor header
///         \code
///          #define DUNESTYLE_ENABLE_AUTOMATICALLY 0
///         \endcode
///         Then you can enable the style manually by calling `dunestyle::setDuneStyle()`.

// n.b.  much of this style ripped off of NOvA official style

#ifndef DUNE_STYLE_H
#define DUNE_STYLE_H

#include "TColor.h"
#include "TGraph.h"
#include "TH1.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TH2.h"

namespace dunestyle
{
  // n.b.: the default style is turned on by SetDuneStyle(),
  // which is called at the bottom of this file
  // (after everything else is defined)

  /// Non-user-facing part of the DUNE style tools
  namespace _internal
  {
    enum class OIColors
    {
      kOrange,
      kSkyBlue,
      kBlueGreen,
      kYellow,
      kBlue,
      kVermilion,
      kRedPurple,
    };

    // The actual TColor objects that correspond to the Okabe-Ito palette,
    // since they need to be explicitly defined.
    // If you're looking for the color *indices*,
    // look in the `colors` namespace, further down
    // The RGB values are taken from here:
    // https://mikemol.github.io/technique/colorblind/2018/02/11/color-safe-palette.html.
    //
    // They're packed into a function to avoid the Static Initialization Order Fiasco
    // (https://en.cppreference.com/w/cpp/language/siof)
    const TColor & GetOIColor(OIColors color)
    {
      switch (color)
      {
        case OIColors::kOrange:
          static const TColor __kOIOrange(TColor::GetFreeColorIndex(), 0.90, 0.60, 0);
          return __kOIOrange;

        case OIColors::kSkyBlue:
          static const TColor __kOISkyBlue(TColor::GetFreeColorIndex(), 0.35, 0.70, 0.90);
          return __kOISkyBlue;

        case OIColors::kBlueGreen:
          static const TColor __kOIBlueGreen(TColor::GetFreeColorIndex(), 0, 0.60, 0.50);
          return __kOIBlueGreen;

        case OIColors::kYellow:
          static const TColor __kOIYellow(TColor::GetFreeColorIndex(), 0.95, 0.90, 0.25);
          return __kOIYellow;

        case OIColors::kBlue:
          static const TColor __kOIBlue(TColor::GetFreeColorIndex(), 0, 0.45, 0.70);
          return __kOIBlue;

        case OIColors::kVermilion:
          static const TColor __kOIVermilion(TColor::GetFreeColorIndex(), 0.80, 0.40, 0);
          return __kOIVermilion;

        case OIColors::kRedPurple:
          static const TColor __kOIRedPurple(TColor::GetFreeColorIndex(), 0.80, 0.60, 0.70);
          return __kOIRedPurple;

        default:
          throw std::out_of_range("Unknown OIColor");
      }
    }

    const TColor & GetOffWhiteColor()
    {
      static const TColor __kOffWhite(TColor::GetFreeColorIndex(), 0.9412, 0.9412, 0.9412);
      return __kOffWhite;
    }
  }

  // ----------------------------------------------------------------------------

  /// Colo(u)rs we encourage collaborators to use.
  /// N.b.: 'colors' namespace is aliased to 'colours' below in case you prefer BrEng spelling
  namespace colors
  {
    /// Available color cycles for use with \ref NextColo(u)r() below
    enum class Cycle
    {
      OkabeIto,
      DUNELogo,
      NumCycles  // size counter
    };

    // probably this is just an int, but this way it's always right
    using Color_t = decltype(std::declval<TColor>().GetNumber());
    using Colour_t = Color_t;

    ///@{
    /// Colors from the Okabe-Ito palette, which is deisgned to be friendly
    /// for those with Color Vision Deficiencies (CVD).
    inline Color_t kOkabeItoOrange = _internal::GetOIColor(_internal::OIColors::kOrange).GetNumber();
    inline Color_t kOkabeItoSkyBlue = _internal::GetOIColor(_internal::OIColors::kSkyBlue).GetNumber();
    inline Color_t kOkabeItoBlueGreen = _internal::GetOIColor(_internal::OIColors::kBlueGreen).GetNumber();
    inline Color_t kOkabeItoBlue = _internal::GetOIColor(_internal::OIColors::kBlue).GetNumber();
    inline Color_t kOkabeItoYellow = _internal::GetOIColor(_internal::OIColors::kYellow).GetNumber();
    inline Color_t kOkabeItoVermilion = _internal::GetOIColor(_internal::OIColors::kVermilion).GetNumber();
    inline Color_t kOkabeItoRedPurple = _internal::GetOIColor(_internal::OIColors::kRedPurple).GetNumber();
    ///@}

    /// Off-white color, used to improve access for those with dyslexia
    inline Color_t kOffWhite = _internal::GetOffWhiteColor().GetNumber();

    /// If you would like all the colors in one package
    const std::map<Cycle, std::vector<Color_t>> kColorCycles
    {
        // this ordering differs from the original Okabe-Ito ordering,
        // which uses yellow (difficult to see on projectors) fairly early in the list.
        // here we also get the DUNE logo colors in the first 4, which is nice.
        { Cycle::OkabeIto, { kBlack,
                             kOkabeItoVermilion,
                             kOkabeItoSkyBlue,
                             kOkabeItoOrange,
                             kOkabeItoBlueGreen,
                             kOkabeItoRedPurple,
                             kOkabeItoBlue,
                             kOkabeItoYellow }},

        { Cycle::DUNELogo, { kOkabeItoVermilion,
                             kOkabeItoSkyBlue,
                             kOkabeItoOrange }},
    };
    const auto kColourCycles = kColorCycles;   ///< Alias for \ref kColorCycles with BrEng spelling

    /// A color cycler that runs through colors in order
    ///
    /// \param cycle  The dunestyle::colors::Cycle you want to run through
    /// \param start  Start cycling from a particular color index.  (-1 continues from previous cycle.)
    /// \return       A color index known to TColor
    Color_t NextColor(Cycle cycle = Cycle::OkabeIto, int start=-1)
    {
      static std::vector<std::size_t> counter(static_cast<std::size_t>(Cycle::NumCycles));

      const std::vector<Color_t> & colorVec = kColorCycles.at(cycle);
      auto cycleIdx = static_cast<std::size_t>(cycle);
      if (start >= 0)
        counter[cycleIdx] = start % colorVec.size();
      Color_t colorVal = colorVec[counter[cycleIdx]];
      counter[cycleIdx] = (counter[cycleIdx] + 1) % colorVec.size();

      return colorVal;
    }

    /// An alias for \ref NextColor() with BrEng spelling
    constexpr auto NextColour = NextColor;

  } // namespace color
  namespace colours = dunestyle::colors;

  // ----------------------------------------------------------------------------
  // ----------------------------------------------------------------------------

  /// Apply a text label at arbitrary location, color, alignment.
  /// Mostly used as an internal utility for the more specific label functions,
  /// but end-users can feel free to use as well.
  ///
  /// \param text    The string to write
  /// \param xLoc    Where to write, along x (in NDC, i.e., fraction-of-pad, coordinates)
  /// \param yLoc    Where to write, along y (in NDC)
  /// \param color   ROOT color to use
  /// \param hAlign  Horizontal text alignment relative to (xLoc, yLoc).  See ETextAlign in ROOT's TAttText
  /// \param vAlign  Vertical text alignment relative to (xLoc, yLoc).  See ETextAlign in ROOT's TAttText
  /// \return        The TLatex instance for the text
  TLatex * TextLabel(const std::string & text, double xLoc, double yLoc, short color=kBlue,
                     ETextAlign hAlign=kHAlignLeft, ETextAlign vAlign=kVAlignTop)
  {
    auto txtObj = new TLatex(xLoc, yLoc, text.c_str());
    txtObj->SetTextColor(color);
    txtObj->SetNDC();
    txtObj->SetTextSize(2 / 30.);
    txtObj->SetTextAlign(hAlign + vAlign);
    txtObj->Draw();

    return txtObj;
  }

  /// Apply a text label in one of 6 prespecified locations: top left, top right, ... bottom center, bottom right
  ///
  /// \param text     The string to write
  /// \param align    Alignment position.  Specify using ETextAlign values from ROOT's TAttText
  /// \param inFrame  When using "top" vertical alignment, specifies whether the label is inside the plot frame or outside it
  /// \param color    ROOT color to use
  /// \return         The TLatex instance for the text
  TLatex * TextLabel(const std::string & text, ETextAlign align, bool inFrame=true, short color=kBlue)
  {
    auto hAlign = static_cast<ETextAlign>(align - (align % 10));
    auto vAlign = static_cast<ETextAlign>(align % 10);
    float xloc = (hAlign == kHAlignRight) ? 0.85 : ((hAlign == kHAlignLeft) ? 0.18 : 0.525);
    float yloc = (vAlign == kVAlignTop) ? (inFrame ? 0.87 : 0.96) : ((vAlign == kVAlignBottom) ? 0.13 : 0.5);
    return TextLabel(text, xloc, yloc, color, hAlign, vAlign);
  }

  /// Return the "DUNE" part of the watermark string, which may have its own styling
  std::string DUNEWatermarkString()
  {
      return "#font[62]{DUNE}";
  }


  /// Write a "DUNE Work In Progress" tag
  ///
  /// \param loc   Location to write (upper left is default).   Specify using ETextAlign values from ROOT's TAttText
  /// \param inFrame  When using "top" vertical alignment, specifies whether the label is inside the plot frame or outside it
  /// \return      The TLatex instance for the text
  TLatex* WIP(ETextAlign loc=static_cast<ETextAlign>(kHAlignLeft + kVAlignTop), bool inFrame=true)
  {
    return TextLabel(DUNEWatermarkString() + " Work In Progress", loc, inFrame, kBlack);
  }

  // ----------------------------------------------------------------------------

  /// Write a "DUNE Preliminary" tag
  ///
  /// \param loc   Location to write (upper left is default).   Specify using ETextAlign values from ROOT's TAttText
  /// \param inFrame  When using "top" vertical alignment, specifies whether the label is inside the plot frame or outside it
  /// \return      The TLatex instance for the text
  TLatex* Preliminary(ETextAlign loc=static_cast<ETextAlign>(kHAlignLeft + kVAlignTop), bool inFrame=true)
  {
    return TextLabel(DUNEWatermarkString() + " Preliminary", loc, inFrame, kBlack);
  }

  // ----------------------------------------------------------------------------

  /// Write a "DUNE Simulation" tag
  ///
  /// \param loc   Location to write (upper left is default).   Specify using ETextAlign values from ROOT's TAttText
  /// \param inFrame  When using "top" vertical alignment, specifies whether the label is inside the plot frame or outside it
  /// \return      The TLatex instance for the text
  TLatex* Simulation(ETextAlign loc=static_cast<ETextAlign>(kHAlignLeft + kVAlignTop), bool inFrame=true)
  {
    return TextLabel(DUNEWatermarkString() + " Simulation", loc, inFrame, kBlack);
  }

  // ----------------------------------------------------------------------------

  /// Write a "DUNE Simulation" tag on the right side, outside the canvas (fixed location)
  ///
  /// \return      The TLatex instance for the text
  TLatex* SimulationSide()
  {
    TLatex * label = TextLabel(DUNEWatermarkString() + " Simulation", .93, .9, kBlack);
    label->SetTextAngle(270);
    label->SetTextAlign(12);
    return label;
  }

  // ----------------------------------------------------------------------------

  /// Write a "DUNE" tag (use for officially approved results only)
  ///
  /// \param loc   Location to write (upper left is default).   Specify using ETextAlign values from ROOT's TAttText
  /// \param inFrame  When using "top" vertical alignment, specifies whether the label is inside the plot frame or outside it
  /// \return      The TLatex instance for the text
  TLatex* Official(ETextAlign loc=static_cast<ETextAlign>(kHAlignLeft + kVAlignTop), bool inFrame=true)
  {
    return TextLabel(DUNEWatermarkString(), loc, inFrame, kBlack);
  }

  // ----------------------------------------------------------------------------

  /// Center the axis titles for a histogram
  ///
  /// \param histo  Histogram in question
  void CenterTitles(TH1 *histo)
  {
    histo->GetXaxis()->CenterTitle();
    histo->GetYaxis()->CenterTitle();
    histo->GetZaxis()->CenterTitle();
  }

  // ----------------------------------------------------------------------------

  /// Switch to a palette friendly to those with Colo(u)r Vision Deficiencies (CVD)
  void CVDPalette()
  {
    gStyle->SetPalette(kCividis);
  }

  // ----------------------------------------------------------------------------

  /// Switch to a nice monochrome palette (white -> red)
  void CherryInvertedPalette()
  {
    gStyle->SetPalette(kCherry);
    TColor::InvertPalette();
  }

  // ----------------------------------------------------------------------------

  /// Switch to a nice bichrome palette (blue -> white -> red):
  /// Recommended for use only when range is symmetric around zero or unity
  void BlueWhiteRedPalette()
  {
    const int NRGBs = 3;
    const int n_color_contours = 999;
    static bool initialized=false;
    static int* colors=new int[n_color_contours];

    if(!initialized){
      gStyle->SetNumberContours(n_color_contours);
      double stops[NRGBs] = { 0.00, 0.50, 1.00};
      double red[NRGBs]   = { 0.00, 1.00, 1.00};
      double green[NRGBs] = { 0.00, 1.00, 0.00};
      double blue[NRGBs]  = { 1.00, 1.00, 0.00};
      int colmin=TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, n_color_contours);
      for(int i=0; i<n_color_contours; ++i) colors[i]=colmin+i;

      initialized=true;
    }
    gStyle->SetNumberContours(n_color_contours);
    gStyle->SetPalette(n_color_contours, colors);
  }

  // ----------------------------------------------------------------------------
  void OffWhiteBackground()
  {
    gStyle->SetFillColor(colors::kOffWhite);
    gStyle->SetFrameFillColor(colors::kOffWhite);
    gStyle->SetCanvasColor(colors::kOffWhite);
    gStyle->SetTitleFillColor(colors::kOffWhite);
    gStyle->SetPadColor(colors::kOffWhite);
    gStyle->SetStatColor(colors::kOffWhite);
  }

  // ----------------------------------------------------------------------------

  /// Divide a TCanvas into two pads.
  ///
  /// \param c       The canvas to divide
  /// \param ysplit  Fraction (from the bottom of the canvas) to split at
  /// \param p1      Upper pad.  This pointer is filled with the pad created.
  /// \param p2      Lower pad.  This pointer is filled with the pad created.
  void SplitCanvas(TCanvas * c, double ysplit, TPad*& p1, TPad*& p2)
  {
    c->cd();
    if (!p1)
      p1 = new TPad("", "", 0, 0, 1, 1);
    if (!p2)
      p2 = new TPad("", "", 0, 0, 1, 1);

    p1->SetBottomMargin(ysplit);
    p2->SetTopMargin(1-ysplit);

    // Draw p1 second since it's often the more important one, that the user
    // would prefer to be able to interact with.
    for(TPad* p: {p2, p1}){
      p->SetFillStyle(0);
      p->Draw();
    }
  }

  // ----------------------------------------------------------------------------

  /// Obtain the TGraph(s) corresponding to a particular contour level for a TH2
  ///
  /// \param h2     The TH2 to examine
  /// \param level  Fractional level in [0, 1]
  /// \return       Vector of TGraphs that represent the whole contour (multiple if contour has disjoint pieces)
  std::vector<TGraph*> GetContourGraphs(TH2* h2, double level)
  {
    std::vector<TGraph*> ret;

    std::unique_ptr<TH2> surf(dynamic_cast<TH2*>(h2->Clone("tmp_h2_for_drawing_graphs")));

    TVirtualPad* bak = gPad;

    const bool wasbatch = gROOT->IsBatch();
    gROOT->SetBatch(); // User doesn't want to see our temporary canvas
    TCanvas tmp;

    gStyle->SetOptStat(0);

    surf->SetContour(1, &level);
    surf->Draw("cont list");

    tmp.Update();
    tmp.Paint();

    gROOT->SetBatch(wasbatch);
    gPad = bak;

    // The graphs we need (contained inside TLists, contained inside
    // TObjArrays) are in the list of specials. But we need to be careful about
    // types, because other stuff can get in here too (TDatabasePDG for
    // example).
    TCollection* specs = gROOT->GetListOfSpecials();

    TIter nextSpec(specs);
    while(TObject* spec = nextSpec()){
      if(!spec->InheritsFrom(TObjArray::Class())) continue;
      auto conts = dynamic_cast<TObjArray*>(spec);

      if(conts->IsEmpty()) continue;

      if(!conts->At(0)->InheritsFrom(TList::Class())) continue;
      auto cont = dynamic_cast<TList*>(conts->At(0));

      TIter nextObj(cont);
      // Contour could be split into multiple pieces
      std::size_t piece = 0;
      while(TObject* obj = nextObj()){
        if(!obj->InheritsFrom(TGraph::Class())) continue;

        ret.push_back(dynamic_cast<TGraph*>(obj->Clone(Form("%s_contour%f_piece%zu", obj->GetName(), level, piece))));
        piece++;
      } // end for obj
    } // end for spec

    return ret;
  }

  // ----------------------------------------------------------------------------
  // ----------------------------------------------------------------------------

  /// Enable the DUNE style.
  bool SetDuneStyle()
  {

    // Defaults to classic style, but that's OK, we can fix it
    TStyle* duneStyle = new TStyle("duneStyle", "DUNE Style");

    duneStyle->SetPalette(kViridis);

    // Center title
    duneStyle->SetTitleAlign(22);
    duneStyle->SetTitleX(.5);
    duneStyle->SetTitleY(.95);
    duneStyle->SetTitleBorderSize(0);

    // No info box
    duneStyle->SetOptStat(0);

    //set the background color to white.
    // see OffWhiteBackground() above for an alternative...
    duneStyle->SetFillColor(10);
    duneStyle->SetFrameFillColor(10);
    duneStyle->SetCanvasColor(10);
    duneStyle->SetPadColor(10);
    duneStyle->SetTitleFillColor(0);
    duneStyle->SetStatColor(10);

    // Don't put a colored frame around the plots
    duneStyle->SetFrameBorderMode(0);
    duneStyle->SetCanvasBorderMode(0);
    duneStyle->SetPadBorderMode(0);

    // Set the default line color for a fit function to be red
    duneStyle->SetFuncColor(kRed);

    // No border on legends
    duneStyle->SetLegendBorderSize(0);

    // Axis titles
    duneStyle->SetTitleSize(.055, "xyz");
    duneStyle->SetTitleOffset(0.92, "xy");
    duneStyle->SetTitleOffset(0.7, "z");

    // This applies the same settings to the overall plot title
    duneStyle->SetTitleSize(.055, "");
    duneStyle->SetTitleOffset(.8, "");

    // Axis labels (numbering)
    duneStyle->SetLabelSize(.04, "xyz");
    duneStyle->SetLabelOffset(.005, "xyz");

    // Prevent ROOT from occasionally automatically zero-suppressing
    duneStyle->SetHistMinimumZero();

    // Thicker lines
    duneStyle->SetHistLineWidth(2);
    duneStyle->SetFrameLineWidth(2);
    duneStyle->SetFuncWidth(2);

    // Markers
    duneStyle->SetMarkerStyle(kFullDotLarge);
    duneStyle->SetErrorX(0);

    // Set the number of tick marks to show
    duneStyle->SetNdivisions(506, "xyz");

    // Set the tick mark style
    duneStyle->SetPadTickX(1);
    duneStyle->SetPadTickY(1);

    // Extend the left and bottom margins so axis titles don't run off the pad
    duneStyle->SetPadBottomMargin(0.15);
    duneStyle->SetPadLeftMargin(0.15);
    duneStyle->SetPadRightMargin(0.15);  // this is a bit wide in general but means most colorbars won't run off the edge

    // Fonts
    const int kDuneFont = 42;
    duneStyle->SetStatFont(kDuneFont);
    duneStyle->SetLabelFont(kDuneFont, "xyz");
    duneStyle->SetTitleFont(kDuneFont, "xyz");
    duneStyle->SetTitleFont(kDuneFont, ""); // Apply same setting to plot titles
    duneStyle->SetTextFont(kDuneFont);
    duneStyle->SetLegendFont(kDuneFont);

    // use the CVD-friendly palette by default
    dunestyle::CVDPalette();

    gROOT->SetStyle("duneStyle");

    return true;
  }

#if !defined(DUNESTYLE_ENABLE_AUTOMATICALLY) || DUNESTYLE_ENABLE_AUTOMATICALLY
  namespace _internal
  {
    /// Throwaway global variable used to call the function that defines the style.
    /// Buried in this _internal namespace to make it clear you shouldn't use it.
    const bool __discarded = SetDuneStyle();
  }
#endif

} // namespace dunestyle

#endif  // DUNE_STYLE_H
