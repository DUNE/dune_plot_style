'''
PyROOT examples.
'''
import ROOT
import numpy as np

import dunestyle.root as dunestyle

ROOT.gROOT.SetBatch(True)

c = ROOT.TCanvas()

# 1D histogram example
h1D = ROOT.TH1D('example1d', ';x label;y label', 50, -5, 5)
h1D.FillRandom('gaus',1000)
leg = ROOT.TLegend(0.6,0.65,0.8,0.8)
leg.AddEntry('example1d','1D histogram','l')
h1D.Draw()
leg.Draw()
dunestyle.CenterTitles(h1D)
dunestyle.WIP()
dunestyle.SimulationSide()
c.Print('example.pyroot.pdf(')

# 1D data/mc comparison type plot
c.Clear()
h1D_ratio = h1D.Clone('h1D_ratio')
p1 = ROOT.TPad('p1','p1',0.,0.35,1.,1.)
p2 = ROOT.TPad('p2','p2',0.,0.,1.,0.35)
p1.SetBottomMargin(0.04)
p1.SetTopMargin(0.15)
p2.SetBottomMargin(0.3)
p2.SetTopMargin(0.04)
c.cd(); p1.Draw(); p1.cd()
h1D.GetXaxis().SetLabelSize(0.)
h1D_ratio.GetXaxis().SetTitleOffset(1.25)
h1D_ratio.GetYaxis().SetTitle('ratio to fit')
leg.Clear()
h1D.Fit('gaus')
h1D.Draw('E')
fit = h1D.GetFunction('gaus')
leg.AddEntry(h1D,'data','lep')
leg.AddEntry(fit,'fit','l')
leg.Draw()
h1D_ratio.Sumw2()
h1D_ratio.Divide(fit)
one = ROOT.TF1('one','1.',-5,5)
dunestyle.CornerLabel('MC/Data Comparison Example')
c.cd(); p2.Draw(); p2.cd()
h1D_ratio.GetYaxis().SetRangeUser(0.,2.)
h1D_ratio.Draw('E')
one.Draw('same')
c.Print('example.pyroot.pdf')

# 2D histogram example
c.Clear()
h2D = ROOT.TH2D('example2d', ';x label;y label', 100, -5, 5, 100, -5, 5)
mean = (0,0)
cov = [[0.5,-0.5],[-0.5,1]]
throws = np.random.multivariate_normal(mean, cov, 10000000)
for throw in throws: h2D.Fill(throw[0],throw[1])
h2D.Draw('colz')
dunestyle.CenterTitles(h2D)
dunestyle.Simulation()
dunestyle.CornerLabel('2D Histogram Example')
c.Print('example.pyroot.pdf')

# 2D contour example
c.Clear()
leg.Clear()
level1 = 500.
level2 = 5000.
level3 = 25000.
levels = np.array([level1,level2,level3])
h2D.SetContour(3,levels)
palette = h2D.GetListOfFunctions().FindObject('palette')
l1_h = ROOT.TH1I('l1_h','l1_h',1,0,1)
l1_h.SetFillColor(palette.GetValueColor(h2D.GetContourLevel(0)))
# doesn't work?
#l1_h = ROOT.TH1I('l1_h','l1_h',1,0,1)
#l1_h.SetFillColor(palette.GetValueColor(h2D.GetContourLevel(1)))
l2_h = ROOT.TH1I('l2_h','l2_h',1,0,1)
l2_h.SetFillColor(palette.GetValueColor((h2D.GetContourLevel(2)-h2D.GetContourLevel(0))/2.))
l3_h = ROOT.TH1I('l3_h','l3_h',1,0,1)
l3_h.SetFillColor(palette.GetValueColor(h2D.GetContourLevel(2)))
leg.AddEntry(l1_h,'level 1 contour','f')
leg.AddEntry(l2_h,'level 2 contour','f')
leg.AddEntry(l3_h,'level 3 contour','f')
h2D.Draw('cont1')
leg.Draw()
dunestyle.CenterTitles(h2D)
dunestyle.Simulation()
dunestyle.SimulationSide()
dunestyle.CornerLabel('2D Contour Example')
c.Print('example.pyroot.pdf')

# stacked histogram
c.Clear()
leg.Clear()
hstack = ROOT.THStack('examplestack', ';x label;y label')
hs1 = ROOT.TH1D('hs1', ';x label;y label', 100, -5, 5)
hs2 = ROOT.TH1D('hs2', ';x label;y label', 100, -5, 5)
hs3 = ROOT.TH1D('hs3', ';x label;y label', 100, -5, 5)
hs1.FillRandom('gaus',10000)
hs2.FillRandom('gaus',5000)
hs3.FillRandom('gaus',1000)
hstack.Add(hs1)
hstack.Add(hs2)
hstack.Add(hs3)
hstack.Draw('pfc')
leg.SetHeader('Stacked Histograms')
leg.AddEntry('hs1','one hist','f')
leg.AddEntry('hs2','two hist','f')
leg.AddEntry('hs3','three hist','f')
leg.Draw()
dunestyle.CornerLabel('Stacked Histograms Example')
c.Print('example.pyroot.pdf)')

