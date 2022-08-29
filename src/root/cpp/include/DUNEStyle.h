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

#ifndef DUNE_ND_LAR_RECO_STYLE
#define DUNE_ND_LAR_RECO_STYLE

#include "TColor.h"
#include "TH1.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TStyle.h"

namespace dunestyle
{
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

  void ColorBlindPalette()
  {
    gStyle->SetPalette(kCividis);
  }

  /// A nice monochrome palette (white -> red)
  void CherryInvertedPalette()
  {
    gStyle->SetPalette(kCherry);
    TColor::InvertPalette();
  }

}

#endif
