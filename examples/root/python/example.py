"""
PyROOT placeholder example.  we can do way better than this.
"""

import ROOT

import dunestyle.root as dunestyle

c = ROOT.TCanvas()
h = ROOT.TH1D("example", ";x label;y label", 500, -5, 5)
h.FillRandom("gaus")
h.Draw()
dunestyle.CenterTitles(h)
dunestyle.WIP()
c.SaveAs("example.root.png")
