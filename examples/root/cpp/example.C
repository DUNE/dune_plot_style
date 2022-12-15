///
/// ROOT placeholder example.  Work in progress.
///

#include "TCanvas.h"
#include "TF1.h"
#include "TF2.h"
#include "TH1D.h"
#include "TH2D.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaletteAxis.h"

#include "DUNEStyle.h"

void example()
{

  TCanvas c;

  // 1D histogram example
  TH1D *h1D = new TH1D("example1d", ";x label;y label", 50, -5, 5);
  h1D->FillRandom("gaus",1000);
  TLegend *leg = new TLegend(0.6,0.65,0.8,0.8);
  leg->AddEntry("example1d","1D histogram","l");
  h1D->Draw();
  leg->Draw();
  dunestyle::CenterTitles(h1D);
  dunestyle::WIP();
  dunestyle::SimulationSide();
  c.SaveAs("example.root.pdf(");

  // 1D data/mc comparison type plot
  c.Clear();
  TH1D* h1D_ratio = (TH1D*)h1D->Clone("h1D_ratio");
  TPad * p1;
  TPad * p2;
  p1 = p2 = nullptr;
  dunestyle::SplitCanvas(&c, 0.3, p1, p2);
  c.cd(); p1->Draw(); p1->cd();
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
  c.cd(); p2->Draw(); p2->cd();
  h1D_ratio->GetYaxis()->SetRangeUser(-1.,1.);
  h1D_ratio->Draw("E");
  zero.Draw("same");
  c.Print("example.root.pdf");

  // 2D histogram example
  c.Clear();
  leg->Clear();
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

  c.Print("example.root.pdf");

  // stacked histogram
  c.Clear();
  leg->Clear();
  THStack hstack("examplestack", ";x label; y label");
  TH1D hs1("hs1", ";x label;y label", 100, -5, 5);
  TH1D hs2("hs2", ";x label;y label", 100, -5, 5);
  TH1D hs3("hs3", ";x label;y label", 100, -5, 5);
  hs1.FillRandom("gaus",10000);
  hs2.FillRandom("gaus",5000);
  hs3.FillRandom("gaus",1000);
  hstack.Add(&hs1);
  hstack.Add(&hs2);
  hstack.Add(&hs3);
  hstack.Draw("pfc");
  leg->SetHeader("Stacked Histograms");
  leg->AddEntry("hs1","one hist","f");
  leg->AddEntry("hs2","two hist","f");
  leg->AddEntry("hs3","three hist","f");
  leg->Draw();
  dunestyle::CornerLabel("Stacked Histograms Example");
  c.Print("example.root.pdf)");

}
