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
#include "TH1.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TStyle.h"

namespace dunestyle
{
  // n.b.: the default style is turned on by SetDuneStyle(),
  // which is called at the bottom of this file
  // (after everything else is defined)

  /// Non-user-facing part of the DUNE style tools
  namespace _internal
  {
    // The actual TColor objects that correspond to the Okabe-Ito palette,
    // since they need to be explicitly defined.
    // If you're looking for the color *indices*,
    // look in the `colors` namespace, further down
    // The RGB values are taken from here:
    // https://mikemol.github.io/technique/colorblind/2018/02/11/color-safe-palette.html
    const TColor __kOIOrange(TColor::GetFreeColorIndex(), 0.90, 0.60, 0);
    const TColor __kOISkyBlue(TColor::GetFreeColorIndex(), 0.35, 0.70, 0.90);
    const TColor __kOIBlueGreen(TColor::GetFreeColorIndex(), 0, 0.60, 0.50);
    const TColor __kOIYellow(TColor::GetFreeColorIndex(), 0.95, 0.90, 0.25);
    const TColor __kOIBlue(TColor::GetFreeColorIndex(), 0, 0.45, 0.70);
    const TColor __kOIVermilion(TColor::GetFreeColorIndex(), 0.80, 0.40, 0);
    const TColor __kOIRedPurple(TColor::GetFreeColorIndex(), 0.80, 0.60, 0.70);
  }

  /// Colo(u)rs we encourage collaborators to use.
  /// N.b.: 'colors' namespace is aliased to 'colours' below in case you prefer BrEng spelling
  namespace colors
  {
    /// Available color cycles for use with \ref NextColo(u)r() below
    enum class Cycle
    {
      OkabeIto,
      NumCycles  // size counter
    };

    // probably this is just an int, but this way it's always right
    using Color_t = decltype(std::declval<TColor>().GetNumber());
    using Colour_t = Color_t;

    ///@{
    /// Colors from the Okabe-Ito palette, which is deisgned to be friendly
    /// for those with Color Vision Deficiencies (CVD).
    Color_t kOkabeItoOrange = _internal::__kOIOrange.GetNumber();
    Color_t kOkabeItoSkyBlue = _internal::__kOISkyBlue.GetNumber();
    Color_t kOkabeItoBlueGreen = _internal::__kOIBlueGreen.GetNumber();
    Color_t kOkabeItoBlue = _internal::__kOIBlue.GetNumber();
    Color_t kOkabeItoYellow = _internal::__kOIYellow.GetNumber();
    Color_t kOkabeItoVermilion = _internal::__kOIVermilion.GetNumber();
    Color_t kOkabeItoRedPurple = _internal::__kOIRedPurple.GetNumber();
    ///@}

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

  // Put a "DUNE Work In Progress" tag in the corner
  TLatex* WIP(ETextAlign labelLoc=kHAlignRight)
  {
    short halign = labelLoc - (labelLoc % 10);
    float loc = (halign == kHAlignRight) ? 0.85 : ((halign == kHAlignLeft) ? 0.15 : 0.525);
    TLatex *prelim = new TLatex(loc, 0.92, "DUNE Work In Progress");
    prelim->SetTextColor(kBlue);
    prelim->SetNDC();
    prelim->SetTextSize(2 / 30.);
    prelim->SetTextAlign(halign + kVAlignBottom);
    prelim->Draw();

    return prelim;
  }


  // Put a "DUNE Simulation" tag in the corner
  void Simulation()
  {
    TLatex *prelim = new TLatex(.9, .95, "DUNE Simulation");
    prelim->SetTextColor(kGray + 1);
    prelim->SetNDC();
    prelim->SetTextSize(2 / 30.);
    prelim->SetTextAlign(32);
    prelim->Draw();
  }

  // Put a "DUNE Simulation" tag on the right
  void SimulationSide()
  {
    TLatex *prelim = new TLatex(.93, .9, "DUNE Simulation");
    prelim->SetTextColor(kGray + 1);
    prelim->SetNDC();
    prelim->SetTextSize(2 / 30.);
    prelim->SetTextAngle(270);
    prelim->SetTextAlign(12);
    prelim->Draw();
  }

// Add a label in top left corner
// Especially useful for "Neutrino Beam" and "Antineutrino Beam" labels
  void CornerLabel(std::string Str)
  {
    TLatex *CornLab = new TLatex(.1, .93, Str.c_str());
    CornLab->SetTextColor(kGray + 1);
    CornLab->SetNDC();
    CornLab->SetTextSize(2 / 30.);
    CornLab->SetTextAlign(11);
    CornLab->Draw();
  }

  void CenterTitles(TH1 *histo)
  {
    histo->GetXaxis()->CenterTitle();
    histo->GetYaxis()->CenterTitle();
    histo->GetZaxis()->CenterTitle();
  }

  /// Palette friendly to those with Colo(u)r Vision Deficiencies (CVD)
  void CVDPalette()
  {
    gStyle->SetPalette(kCividis);
  }

  /// A nice monochrome palette (white -> red)
  void CherryInvertedPalette()
  {
    gStyle->SetPalette(kCherry);
    TColor::InvertPalette();
  }

  /// A nice bichrome palette (blue -> white -> red):
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

    //set the background color to white
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

    // Set the number of tick marks to show
    duneStyle->SetNdivisions(506, "xyz");

    // Set the tick mark style
    duneStyle->SetPadTickX(1);
    duneStyle->SetPadTickY(1);

    // Extend the left and bottom margins so axis titles don't run off the pad
    duneStyle->SetPadBottomMargin(0.15);
    duneStyle->SetPadLeftMargin(0.15);
    duneStyle->SetPadRightMargin(0.15);

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
