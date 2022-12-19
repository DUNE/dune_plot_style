///
/// ROOT placeholder example.  Work in progress.
///

#include "TCanvas.h"
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

#include "DUNEStyle.h"

TF1 f("f", "[&](double *x, double *p){ return TMath::Exp(-0.5*(x[0]-p[0])*(x[0]-p[0])); }", -100, 100, 1);

// save a lot of useless repetitive typing
TLegend * MakeLegend()
{
  auto leg = new TLegend(0.7, 0.5, 0.9, 0.85);
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
  TLegend * leg = MakeLegend();
  leg->AddEntry("example1d", "1D histogram", "l");
  h1D->Draw();
  leg->Draw();
  dunestyle::CenterTitles(h1D);
  dunestyle::WIP();
  dunestyle::SimulationSide();
}

//-------------------------------------------------------------------
void DataMCExample(TCanvas * c)
{
  // 1D data/mc comparison type plot
  c->cd();
  c->Clear();
  TLegend * leg = MakeLegend();

  TH1D *h1D = new TH1D("example1d", ";x label;y label", 50, -5, 5);
  h1D->FillRandom("gaus", 1000);
  TH1D* h1D_ratio = (TH1D*)h1D->Clone("h1D_ratio");
  TPad * p1;
  TPad * p2;
  p1 = p2 = nullptr;
  dunestyle::SplitCanvas(c, 0.3, p1, p2);
  c->cd(); p1->Draw(); p1->cd();
  h1D->GetXaxis()->SetLabelSize(0.);
  h1D_ratio->GetXaxis()->SetTitleOffset(1.25);
  h1D_ratio->GetYaxis()->SetTitle("(Data - Fit)/Fit");
  leg->Clear();
  h1D->Fit("gaus");
  h1D->Draw("E");
  TF1* fit = h1D->GetFunction("gaus");
  leg->AddEntry(h1D,"data","lep");
  leg->AddEntry(fit,"fit","l");
  leg->Draw();
  h1D_ratio->Sumw2();
  h1D_ratio->Add(fit, -1);
  h1D_ratio->Divide(fit);
  TF1 zero("zero","0.",-5,5);
  dunestyle::CornerLabel("MC/Data Comparison Example");
  c->cd(); p2->Draw(); p2->cd();
  h1D_ratio->GetYaxis()->SetRangeUser(-1.,1.);
  h1D_ratio->Draw("E");
  zero.Draw("same");
}

//-------------------------------------------------------------------
void TwoDExample(TCanvas * c)
{

  c->Clear();
  c->cd();
  TLegend * leg = MakeLegend();

  TH2D h2D("example2d", ";x label;y label", 100, -5, 5, 100, -5, 5);
  TF2 cust_guas_2d("cust_gaus_2d","ROOT::Math::bigaussian_pdf(x,y,0.5,1.0,-0.5,0,0)");
  h2D.FillRandom("cust_gaus_2d",1e7);
  h2D.Draw("colz");
  dunestyle::CenterTitles(&h2D);
  dunestyle::Simulation();
  dunestyle::CornerLabel("2D Histogram Example");

  // compute the contour levels.
  // this is a clumsy way of getting the cumuluative distribution function
  TH1D tmp("tmp", "tmp", h2D.GetMaximum()+1, 0, h2D.GetMaximum()+1);
  for (int i = 0; i < h2D.GetNcells()+1; i++)
    tmp.Fill(h2D.GetBinContent(i), h2D.GetBinContent(i));
  double cutoffs[3] = {0.997, 0.954, 0.682};
  std::vector<double> levels;
  int runSum = 0;
  for (int i = 0; i < tmp.GetNcells(); i++)
  {
    if (levels.size() > 2)
      break;
    if (tmp.Integral(i, tmp.GetNcells()+1) < cutoffs[levels.size()]*h2D.Integral())
      levels.push_back(i);
  }

  // now that we have them, draw them
  std::vector<int> linestyles = {kSolid, kDashed, kDotted};
  for (std::size_t sigma : {1, 2, 3})
  {
    std::vector<TGraph*> graphs = dunestyle::GetContourGraphs(&h2D, levels[3-sigma]);
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
  TLegend * leg = MakeLegend();

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
    leg->GetListOfPrimitives()->AddFirst(new TLegendEntry(hstack->GetStack()->Last(), Form("Hist #%zu", histIdx+1), "f"));
  }
  hstack->Draw();
  leg->Draw();
  dunestyle::CenterTitles(hstack->GetHistogram());
}

//-------------------------------------------------------------------
void OverlayExample(TCanvas * c, std::vector<TH1D>& hists)
{
  // stacked histogram
  c->Clear();
  c->cd();
  TLegend * leg = MakeLegend();

  if (hists.empty())
    std::vector<TH1D> hists = GaussHists();
  for (std::size_t histIdx = 0; histIdx < hists.size(); histIdx++)
  {
    TH1D& h = hists[histIdx];
    auto color = dunestyle::colors::NextColor(dunestyle::colors::Cycle::OkabeIto, histIdx==0 ? 0 : -1);
    h.SetLineColor(color);
    dunestyle::CenterTitles(&h);
    auto newh = h.DrawCopy(histIdx == 0 ? "" : "same");  // need to leak it so it doesn't disappear
    leg->GetListOfPrimitives()->AddFirst(new TLegendEntry(newh, Form("Hist #%zu", histIdx+1), "l"));
  }
  leg->Draw();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
void example()
{

  TCanvas c;

  OneDHistExample(&c);
  c.SaveAs("example.root.pdf(");

  DataMCExample(&c);
  c.SaveAs("example.root.pdf");

  TwoDExample(&c);
  c.SaveAs("example.root.pdf");

  // otherwise ROOT tries to use GSL, which the user may or may not have built
  ROOT::Math::IntegratorOneDimOptions::SetDefaultIntegrator("Gauss");
  std::vector<TH1D> hists = GaussHists();
  StackedExample(&c, hists);
  c.Print("example.root.pdf");

  OverlayExample(&c, hists);
  c.Print("example.root.pdf)");

}
