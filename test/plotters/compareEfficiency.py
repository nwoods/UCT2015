'''

Script to make some quick efficiency and resolution plots

Author: Evan K. Friis, UW Madison; modified by Nate Woods

Usage: python compareEfficiency.py TAVGPUFILE.root XAVGPUFILE.root label[optional]

E.G.
python compareEfficiency.py TAVGPUFILE.root XAVGPUFILE.root v3
will produce rate plots in ~/www/v3_filename.png

python compareEfficiency.py TAVGPUFILE.root XAVGPUFILE.root UCT2015/test  
will produce rate plots in ~/www/UCT2015/test_filename.png 

python compareEfficiency.py TAVGPUFILE.root XAVGPUFILE.root
will produce rate plots in ~/www/filename.png


'''

#######################
### GET EVERYTHING ####
#######################
from sys import argv, stdout, stderr

import ROOT


#######################
######## STYLE ########
#######################
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)


#######################
######## File #########
#######################
if len(argv) < 3:
   print 'Usage:python allRatePlots.py tAvgPUFile.root xAvgPUFile.root label[optional]'
   exit()

Infile_T = argv[1]
Infile_X = argv[2]
ntuple_file_t = ROOT.TFile(Infile_T)
ntuple_file_x = ROOT.TFile(Infile_X)
ntuple_file_old = ROOT.TFile(Infile_X)

####################################
####### CALIBRAITON FACTOR #########
####################################
L1_CALIB_FACTOR = 1.0
L1G_CALIB_FACTOR = 1.0

####################################
######## LABEL & SAVE WHERE #########
####################################

if len(argv)>3:
   saveWhere='~/www/'+argv[3]+'_'
else:
   saveWhere='~/www/'

### EG NTuples
eg_ntuple_old = ntuple_file_old.Get("rlxEGEfficiency/Ntuple")
iso_eg_ntuple_old = ntuple_file_old.Get("rlxUCTisoL1EGEfficiency/Ntuple")
eg_ntuple_t = ntuple_file_t.Get("rlxEGEfficiency/Ntuple")
eg_ntuple_x = ntuple_file_x.Get("rlxEGEfficiency/Ntuple")

### Tau NTuples
tau_ntuple_old = ntuple_file_old.Get("rlxTauEfficiency/Ntuple")
tau_ntuple_t = ntuple_file_t.Get("rlxTauEfficiency/Ntuple")
tau_ntuple_x = ntuple_file_x.Get("rlxTauEfficiency/Ntuple")

### Jet NTuples
jet_ntuple_old = ntuple_file_old.Get("corrjetEfficiency/Ntuple")
jet_ntuple_t = ntuple_file_t.Get("corrjetEfficiency/Ntuple")
jet_ntuple_x = ntuple_file_x.Get("corrjetEfficiency/Ntuple")

canvas = ROOT.TCanvas("asdf", "adsf", 800, 600)

def make_plot(tree, variable, selection, binning, xaxis='', title=''):
    ''' Plot a variable using draw and return the histogram '''
    draw_string = "%s * %0.2f>>htemp(%s)" % (variable, 1.0, ", ".join(str(x) for x in binning))
#    draw_string = "%s>>htemp(%s)" % (variable, ", ".join(str(x) for x in binning))
    print draw_string
    print selection
    tree.Draw(draw_string, selection, "goff")
    output_histo = ROOT.gDirectory.Get("htemp").Clone()
    output_histo.GetXaxis().SetTitle(xaxis)
    output_histo.SetTitle(title)
    return output_histo

def make_efficiency(denom, num, color, markerStyle):
    ''' Make an efficiency graph '''
    eff = ROOT.TGraphAsymmErrors(num, denom)
    eff.SetMarkerStyle(markerStyle)
    eff.SetMarkerColor(color)
    eff.SetMarkerSize(1.5)
    eff.SetLineColor(color)
    return eff

## def make_l1g_efficiency(denom, num):
##     ''' Make an efficiency graph with the "UCT" style '''
##     eff = ROOT.TGraphAsymmErrors(num, denom)
##     eff.SetMarkerStyle(24)
##     eff.SetMarkerColor(ROOT.EColor.kBlue)
##     eff.SetMarkerSize(1.5)
##     eff.SetLineColor(ROOT.EColor.kBlack)
##     return eff
## 
    

L1G_CALIB_FACTOR = 1.0

def compare_efficiencies(Ntuple_t, Ntuple_x, oldNtuple, variable, ptCut,
                         binning, selection_reco="", selection_uct="",
                         selection_old="", file_title="plot",
                         title='', xaxis=''):
    ''' Returns a (L1, L1G) tuple of TGraphAsymmErrors '''
    denom_t = make_plot(
        Ntuple_t, variable,
        selection_reco, #"dr03CombinedEt < 0.2", # Select isolated candidates
        binning
        )
    denom_x = make_plot(
        Ntuple_x, variable,
        selection_reco, #"dr03CombinedEt < 0.2", # Select isolated candidates
        binning
        )
    selection_string_uct = ("l1gMatch&&l1gPt>%0.2f"%(ptCut))
    selection_string_old = ("l1Match&&l1Pt>%0.2f"%(ptCut))
    if selection_reco != "":
        selection_string_uct += "&&" + selection_reco
        selection_string_old += "&&" + selection_reco
    if selection_uct != "":
        selection_string_uct += ("&&" + selection_uct)
    if selection_old != "":
        selection_string_old += ("&&" + selection_old)
    num_t = make_plot(
        Ntuple_t, variable,
        selection_string_uct,#string_uct, #"l1gMatch && %0.2f * l1gPt > %0.2f && dr03CombinedEt < 0.2 && (l1gJetPt - l1gPt) / l1gPt < %0.2f && (l1gRegionEt - l1gPt) / l1gPt < %0.2f" % (L1G_CALIB_FACTOR, l1PtCut, jetRelIsoCut, regionRelIsoCut),
        binning
        )
    num_x = make_plot(
        Ntuple_x, variable,
        selection_string_uct, #"l1Match && l1Pt > %0.2f && dr03CombinedEt < 0.2" % (l1PtCut),
        binning
        )
    num_old = make_plot(
        oldNtuple, variable,
        selection_string_old, #"l1Match && l1Pt > %0.2f && dr03CombinedEt < 0.2" % (l1PtCut),
        binning
        )

    frame = ROOT.TH1F("frame", "frame", *binning)
    eff_t = make_efficiency(denom_t, num_t, ROOT.EColor.kBlue, 22)
    eff_x = make_efficiency(denom_x, num_x, ROOT.EColor.kGreen, 21)
    eff_old = make_efficiency(denom_x, num_old, ROOT.EColor.kRed, 20)
    frame.SetMaximum(1.2)
    frame.SetTitle(title)
    frame.GetXaxis().SetTitle(xaxis)
    frame.Draw()
    eff_x.Draw('pe')
    eff_old.Draw('pe')
    eff_t.Draw('pe')
    legend = ROOT.TLegend(0.7, 0.2, 0.89, 0.4, "", "brNDC")
    legend.SetFillColor(ROOT.EColor.kWhite)
    legend.SetBorderSize(1)
    legend.AddEntry(eff_t, "UCT (time avg PU)", "pe")
    legend.AddEntry(eff_x, "UCT (space avg PU", "pe")
    legend.AddEntry(eff_old, "Current", "pe")
    legend.Draw()
    filename = saveWhere + file_title + '.png'
    canvas.SaveAs(filename)

################################################################################
# Relaxed tau efficiency for a 20 GeV cut on L1
################################################################################

jetRange = [10000]#[0.90, 0.95, 1.00, 1.05, 1.10]#[0.10, 0.15, 0.20, 0.25]
regRange = [10000]#[0.90, 0.95, 1.00, 1.05, 1.10]#[0.05, 0.10, 0.15, 0.20]

for jetCut in jetRange:
    for regCut in regRange:
1PtCut = 20

# rlx EG
# compare_efficiencies(eg_ntuple_t, eg_ntuple_x, eg_ntuple_old,
#                      "recoPt", L1PtCut, [40, 0, 200],
#                      "", # No reco selection (rlx EG)
#                      "!l1gMIP&&!l1gTauVeto", # UCT EG
#                      "", # No old L1 selection
#                      "rlx_eg_eff_%0.2f"%(L1PtCut),
#                      "Relaxed EG efficiency (20GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
#                      "RECO p_{T} (GeV)")
# 
iso EG
compare_efficiencies(eg_ntuple_t, eg_ntuple_x, iso_eg_ntuple_old,
                     "recoPt", L1PtCut, [40, 0, 200],
                     "dr03CombinedEt/recoPt < 0.1", # Isolated reco
                     "!l1gMIP&&!l1gTauVeto&&l1gJetPt>0&&((l1gJetPt-l1gPt)/l1gPt<=.2||l1gPt>63.)", # UCT EG iso
                     "", # No selection for iso L1
                     "iso_eg_eff_%0.2f"%(L1PtCut),
                     "Isolated EG efficiency (20GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
                     "RECO p_{T} (GeV)")

# rlx tau
# compare_efficiencies(tau_ntuple_t, tau_ntuple_x, tau_ntuple_old,
#                      "recoPt", L1PtCut, [40, 0, 200],
#                      "", # No reco selection (rlx tau)
#                      "", # UCT tau
#                      "", # No old L1 selection
#                      "rlx_tau_eff_%0.2f"%(L1PtCut),
#                      "Relaxed EG Efficiency (20 GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
#                      "RECO p_{T} (GeV)")
# 
# 
# iso tau
# compare_efficiencies(tau_ntuple_t, tau_ntuple_x, tau_ntuple_old,
#                      "recoPt", L1PtCut, [40, 0, 200],
#                      "dr03CombinedEt < 0.2", # Isolated (iso tau)
#                      "l1gJetPt>0&&(l1gJetPt-l1gPt)/l1gPt<=.1", # UCT Iso Tau
#                      "", # No old L1 selection lol
#                      "iso_tau_eff_%0.2f"%(L1PtCut),
#                      "Isolated Tau fficiency (20 GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
#                      "RECO p_{T} (GeV)")
# 
# #Jet
# 
# for JetL1PtCut in [30., 50., 70., 150., 200.]:
# 
#    compare_efficiencies(jet_ntuple_t, jet_ntuple_x, jet_ntuple_old,
#                         "recoPt", JetL1PtCut, [40, 0, 200],
#                         "", # No reco selection 
#                         "", # No UCT selection
#                         "", # No old L1 selection
#                         "jet_eff_%0.2f"%(JetL1PtCut),
#                         "Jet efficiency (%uGeV)"%(JetL1PtCut),# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
#                         "RECO p_{T} (GeV)")
