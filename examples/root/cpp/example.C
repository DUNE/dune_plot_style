///
/// Demonstration of DUNE plot style using C++ ROOT.
///
/// Original authors:  Young DUNE plot style task force
/// Comments to:  Authorship & publications board (dune-apb@fnal.gov)


#include "TCanvas.h"
#include "TFrame.h"
#include "TF1.h"
#include "TF2.h"
#include "TH1D.h"
#include "TH2D.h"
#include "THStack.h"
#include "TLegendEntry.h"
#include "TMath.h"
#include "Math/IntegratorOptions.h"
#include "TLegend.h"
#include "TPaletteAxis.h"
#include "TPaveText.h"

#include "DUNEStyle.h"

TF1 f("f", "[&](double *x, double *p){ return TMath::Exp(-0.5*(x[0]-p[0])*(x[0]-p[0])); }", -100, 100, 1);

// save a lot of useless repetitive typing
TLegend * MakeLegend(float left=0.7, float bottom=0.5, float right=0.9, float top=0.85)
{
  auto leg = new TLegend(left, bottom, right, top);
  leg->SetFillStyle(0);  // unfortunately can't set this in TStyle :(

  return leg;
}

//-------------------------------------------------------------------
// enables us to reuse the histograms rather than regenerating every time
std::vector<TH1D> GaussHists(std::size_t nHists=dunestyle::colors::kColorCycles.at(dunestyle::colors::Cycle::OkabeIto).size())
{
  std::vector<TH1D> hists;
  for (std::size_t histIdx = 0; histIdx < nHists; histIdx++)
  {
    f.SetParameter(0, 2*int(histIdx) - (int(nHists)-1));
    hists.emplace_back(Form("hs%zu", histIdx+1), ";x label;y label", 100, -2*(nHists/2.+2), 2*nHists);
    hists.back().FillRandom("f", 10000);
  }
  return hists;
}

//-------------------------------------------------------------------
void OneDHistExample(TCanvas * c)
{
  TH1D *h1D = new TH1D("example1d", ";x label;y label", 50, -5, 5);
  h1D->FillRandom("gaus", 1000);
  h1D->Draw();
  h1D->GetYaxis()->SetRangeUser(h1D->GetYaxis()->GetXmin(), h1D->GetMaximum()*1.25);  // make room for watermark
  dunestyle::CenterTitles(h1D);
  dunestyle::Simulation();
}

//-------------------------------------------------------------------
void DataMCExample(TCanvas * c)
{
  // 1D data/mc comparison type plot
  c->cd();
  c->Clear();
  TLegend * leg = MakeLegend(0.65, 0.7, 0.9, 0.85);

  TH1D *h1D = new TH1D("example1d", ";x label;y label", 50, -5, 5);
  h1D->FillRandom("gaus", 1000);
  dunestyle::CenterTitles(h1D);

  TH1D* h1D_ratio = (TH1D*)h1D->Clone("h1D_ratio");
  TPad * p1;
  TPad * p2;
  p1 = p2 = nullptr;
  dunestyle::SplitCanvas(c, 0.3, p1, p2);
  c->cd(); p1->Draw(); p1->cd();
  h1D_ratio->GetXaxis()->SetTitleOffset(1.25);
  h1D_ratio->GetYaxis()->SetTitle("(Data - Fit)/Fit");
  leg->Clear();
  h1D->Fit("gaus", "Q");
  h1D->Draw("E");
  TF1* fit = h1D->GetFunction("gaus");
  leg->AddEntry(h1D,"Data","lep");
  leg->AddEntry(fit,"Fit","l");
  leg->Draw();
  h1D->GetYaxis()->SetRangeUser(h1D->GetMinimum(), h1D->GetMaximum()*1.35);  // make room for watermark
  dunestyle::Preliminary();

  h1D_ratio->Sumw2();
  h1D_ratio->Add(fit, -1);
  h1D_ratio->Divide(fit);
  auto zero = new TF1("zero","0.",-5,5);
  dunestyle::CenterTitles(h1D_ratio);
  c->cd(); p2->Draw(); p2->cd();
  h1D_ratio->GetYaxis()->SetRangeUser(-0.99,0.99);
  h1D_ratio->Draw("E");
  zero->Draw("same");

  p1->cd();
  auto pave = new TPaveText(0.6, 0.45, 0.85, 0.65, "NDC NB");
  pave->SetBorderSize(0);
  pave->SetFillStyle(0);
  pave->SetTextSizePixels(35);
  TText* head = pave->AddText("Fit Parameters:");
  head->SetTextFont(62);
  pave->AddText(Form("A = %.2f #pm %.2f", fit->GetParameter(0), fit->GetParError(0)));
  pave->AddText(Form("#mu = %.2f #pm %.2f", fit->GetParameter(1), fit->GetParError(1)));
  pave->AddText(Form("#sigma = %.2f #pm %.2f", fit->GetParameter(2), fit->GetParError(2)));
  pave->AddText(Form("#chi^{2}/ndof = %.2f/%d", fit->GetChisquare(), fit->GetNDF()));
  pave->Draw();

}

//-------------------------------------------------------------------
void TwoDExample(TCanvas * c)
{

  c->Clear();
  c->cd();
  TLegend * leg = MakeLegend(0.7, 0.65, 0.9, 0.85);

  auto h2d = new TH2D("example2d", ";x label;y label", 100, -5, 5, 120, -5, 7);
  auto cust_gaus_2d = new TF2("cust_gaus_2d","ROOT::Math::bigaussian_pdf(x,y,0.5,1.0,-0.5,0,0)");
  h2d->FillRandom("cust_gaus_2d",1e7);
  dunestyle::CenterTitles(h2d);
  h2d->Draw("colz");
  dunestyle::Simulation();

  // compute the contour levels.
  // this is a clumsy way of getting the cumulative distribution function
  TH1D tmp("tmp", "tmp", h2d->GetMaximum()+1, 0, h2d->GetMaximum()+1);
  for (int i = 0; i < h2d->GetNcells()+1; i++)
    tmp.Fill(h2d->GetBinContent(i), h2d->GetBinContent(i));
  double cutoffs[3] = {0.997, 0.954, 0.682};
  std::vector<double> levels;
  int runSum = 0;
  for (int i = 0; i < tmp.GetNcells(); i++)
  {
    if (levels.size() > 2)
      break;
    if (tmp.Integral(i, tmp.GetNcells()+1) < cutoffs[levels.size()]*h2d->Integral())
      levels.push_back(i);
  }

  // now that we have them, draw them
  std::vector<int> linestyles = {kSolid, kDotted, kDashed};
  for (std::size_t sigma : {1, 2, 3})
  {
    std::vector<TGraph*> graphs = dunestyle::GetContourGraphs(h2d, levels[3-sigma]);
    auto color = dunestyle::colors::NextColor();
    for (TGraph * g : graphs)
    {
      g->SetLineColor(color);
      g->SetLineStyle(linestyles[sigma-1]);
      g->Draw("same");
    }
    leg->AddEntry(graphs.back(), Form("%zu#sigma", sigma), "l");
  }
  leg->Draw();
}

//-------------------------------------------------------------------
void StackedExample(TCanvas * c, std::vector<TH1D>& hists)
{
  // stacked histogram
  c->Clear();
  c->cd();
  TLegend * leg = MakeLegend(0.68, 0.45, 0.9, 0.87);

  auto hstack = new THStack("examplestack", ";x label; y label");
  if (hists.empty())
    std::vector<TH1D> hists = GaussHists();
  for (std::size_t histIdx = 0; histIdx < hists.size(); histIdx++)
  {
    TH1D& h = hists[histIdx];
    auto color = dunestyle::colors::NextColor(dunestyle::colors::Cycle::OkabeIto, histIdx==0 ? 0 : -1);
    h.SetLineColor(color);
    h.SetFillColor(color);
    hstack->Add(dynamic_cast<TH1D*>(h.Clone(h.GetName())));

    // we do this the hard way so the legend has the top-most histogram in the stack first
    leg->GetListOfPrimitives()->AddFirst(new TLegendEntry(&h, Form("Hist #%zu", histIdx+1), "f"));
  }
  hstack->Draw();
  leg->Draw();

  hstack->SetMaximum(hstack->GetMaximum()*1.25);   // make some room for the watermark
  dunestyle::CenterTitles(hstack->GetHistogram());
  dunestyle::WIP();
}

//-------------------------------------------------------------------
void OverlayExample(TCanvas * c, std::vector<TH1D>& hists)
{
  // stacked histogram
  c->Clear();
  c->cd();
  TLegend * leg = MakeLegend(0.68, 0.45, 0.9, 0.87);

  if (hists.empty())
    std::vector<TH1D> hists = GaussHists();
  TH1 * hFirst = nullptr;
  for (std::size_t histIdx = 0; histIdx < hists.size(); histIdx++)
  {
    TH1D& h = hists[histIdx];
    auto color = dunestyle::colors::NextColor(dunestyle::colors::Cycle::OkabeIto, histIdx==0 ? 0 : -1);
    h.SetLineColor(color);
    h.SetFillStyle(0);
    dunestyle::CenterTitles(&h);
    auto newh = h.DrawCopy(histIdx == 0 ? "" : "same");  // need to leak it so it doesn't disappear
    if (!hFirst)
      hFirst = newh;

    // we do this the hard way so the legend has the top-most histogram in the stack first
    leg->GetListOfPrimitives()->AddFirst(new TLegendEntry(newh, Form("Hist #%zu", histIdx+1), "l"));
  }
  c->RedrawAxis();  // otherwise the last histogram drawn overlaps with the frame
  leg->Draw();
  hFirst->SetMaximum(hFirst->GetMaximum()*1.25); // make some space for the watermark
  dunestyle::WIP();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
void example()
{
  dunestyle::OffWhiteBackground();

  TCanvas c;

  OneDHistExample(&c);
  c.SaveAs("example.root.pdf(");
  c.SaveAs("example.root.hist1D.png");

  DataMCExample(&c);
  c.SaveAs("example.root.pdf");
  c.SaveAs("example.root.datamc.png");

  TwoDExample(&c);
  c.SaveAs("example.root.pdf");
  c.SaveAs("example.root.hist2D.png");

  // otherwise ROOT tries to use GSL, which the user may or may not have built
  ROOT::Math::IntegratorOneDimOptions::SetDefaultIntegrator("Gauss");
  std::vector<TH1D> hists = GaussHists();
  StackedExample(&c, hists);
  c.Print("example.root.pdf");
  c.SaveAs("example.root.histstacked.png");

  OverlayExample(&c, hists);
  c.Print("example.root.pdf)");
  c.SaveAs("example.root.histoverlay.png");

}

#ifndef __ROOTCLING__
int main()
{
  example();
  return 0;
}
#endif