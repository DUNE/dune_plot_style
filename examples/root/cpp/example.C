///
/// ROOT placeholder example.  Work in progress.
///

#include "TCanvas.h"
#include "TH1D.h"

#include "DUNEStyle.h"

void example()
{

  TCanvas c;

  // 1D histogram example
  TH1D h1D("example1d", ";x label;y label", 100, -5, 5);
  h1D.FillRandom("gaus",10000);
  TLegend leg(0.6,0.7,0.8,0.85);
  leg.AddEntry("example1d","1D histogram","l");
  h1D.Draw();
  leg.Draw();
  dunestyle::CenterTitles(&h1D);
  dunestyle::WIP();
  dunestyle::SimulationSide();
  c.Print("example.root.pdf(");

  // 2D histogram example
  c.Clear();
  TH2D h2D("example2d", ";x label;y label", 100, -5, 5, 100, -5, 5);
  TF2 cust_guas_2d("cust_gaus_2d","ROOT::Math::bigaussian_pdf(x,y,0.5,1.0,-0.5,0,0)");
  h2D.FillRandom("cust_gaus_2d",1e7);
  h2D.Draw("colz");
  dunestyle::CenterTitles(&h2D);
  dunestyle::Simulation();
  dunestyle::CornerLabel("Neutrino beam");
  c.Print("example.root.pdf");

  // 2D contour example
  c.Clear();
  double levels[3] = {500,5000,25000};
  h2D.SetContour(3,levels);
  h2D.Draw("cont1");
  dunestyle::CenterTitles(&h2D);
  dunestyle::Simulation();
  dunestyle::SimulationSide();
  dunestyle::CornerLabel("Neutrino beam");
  c.Print("example.root.pdf");

  // stacked histogram
  c.Clear();
  leg.Clear();
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
  leg.SetHeader("Stacked Histograms");
  leg.AddEntry("hs1","one hist","f");
  leg.AddEntry("hs2","two hist","f");
  leg.AddEntry("hs3","three hist","f");
  leg.Draw();
  dunestyle::CornerLabel("Stacked histograms");
  c.Print("example.root.pdf)");

}
