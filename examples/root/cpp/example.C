///
/// ROOT placeholder example.  we can do way better than this.
///

#include "TCanvas.h"
#include "TH1D.h"

#include "DUNEStyle.h"

void example()
{
  TCanvas c;
  TH1D h("example", ";x label;y label", 500, -5, 5);
  h.FillRandom("gaus");
  h.Draw();
  dunestyle::CenterTitles(&h);
  dunestyle::WIP();
  c.SaveAs("example.root.png");
}
