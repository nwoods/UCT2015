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

def setn():
   global n #stupid hack to make profiles work correctly
   n=0

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
    global n
    draw_string = "%s>>htemp%i(%s)" % (variable, n, ", ".join(str(x) for x in binning))

    print draw_string
    print selection
    tree.Draw(draw_string, selection, "goff")
    output_histo = ROOT.gDirectory.Get("htemp%i"%(n)).Clone()
    output_histo.GetXaxis().SetTitle(xaxis)
    output_histo.SetTitle(title)
    n=n+1
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

def make_profile(Ntuple, variables, binning, selection, color, markerStyle, min, max, file_title="profile", title='',xaxis=''):
   ''' Make and save a legit profile '''
   plot = make_plot(Ntuple, variables, selection, binning, xaxis, title)
   prof = plot.ProfileX()
   prof.SetMarkerSize(1.5)
   prof.SetMarkerStyle(markerStyle)
   prof.SetMarkerColor(color)
   prof.SetLineColor(color)
   prof.SetMinimum(min)
   prof.SetMaximum(max)

   framebins = binning[0:3]
   
   frame = ROOT.TH1F("frame", "frame", *framebins)
   frame.SetMaximum(max)
   frame.SetMinimum(min)
   frame.SetTitle(title)
   frame.GetXaxis().SetTitle(xaxis)
   frame.Draw()
   prof.Draw('e')
   filename = saveWhere + file_title + '.png'
   canvas.SaveAs(filename)
   

def compare_efficiencies(Ntuple_t, Ntuple_x, oldNtuple, variable, ptCut,
                         binning, selection_reco="", selection_uct="",
                         selection_old="", file_title="plot",
                         title='', xaxis=''):
    ''' Returns a (L1, L1G) tuple of TGraphAsymmErrors '''
    denom_t = make_plot(
        Ntuple_t, variable,
        selection_reco, 
        binning
        )
    denom_x = make_plot(
        Ntuple_x, variable,
        selection_reco, 
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
        selection_string_uct,
        binning
        )
    num_x = make_plot(
        Ntuple_x, variable,
        selection_string_uct,
        binning
        )
    num_old = make_plot(
        oldNtuple, variable,
        selection_string_old,
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

setn()


L1PtCut = 20

#rlx EG
compare_efficiencies(eg_ntuple_t, eg_ntuple_x, eg_ntuple_old,
                     "recoPt", L1PtCut, [40, 0, 200],
                     "", # No reco selection (rlx EG)
                     "!l1gMIP&&!l1gTauVeto", # UCT EG
                     "", # No old L1 selection
                     "rlx_eg_eff_%i"%(L1PtCut),
                     "Relaxed EG efficiency (20GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
                     "RECO p_{T} (GeV)")

#rlx EG no MIP
compare_efficiencies(eg_ntuple_t, eg_ntuple_x, eg_ntuple_old,
                     "recoPt", L1PtCut, [40, 0, 200],
                     "", # No reco selection (rlx EG)
                     "!l1gTauVeto", # UCT EG
                     "", # No old L1 selection
                     "rlx_eg_eff_%i_noMIPCut"%(L1PtCut),
                     "Relaxed EG efficiency (20GeV, tauVeto cut only)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
                     "RECO p_{T} (GeV)")

#rlx EG no tauVeto
compare_efficiencies(eg_ntuple_t, eg_ntuple_x, eg_ntuple_old,
                     "recoPt", L1PtCut, [40, 0, 200],
                     "", # No reco selection (rlx EG)
                     "!l1gMIP", # UCT EG
                     "", # No old L1 selection
                     "rlx_eg_eff_%i_noTauVetoCut"%(L1PtCut),
                     "Relaxed EG efficiency (20GeV, MIP cut only)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
                     "RECO p_{T} (GeV)")

# #iso EG
# compare_efficiencies(eg_ntuple_t, eg_ntuple_x, iso_eg_ntuple_old,
#                      "recoPt", L1PtCut, [40, 0, 200],
#                      "dr03CombinedEt/recoPt < 0.1", # Isolated reco
#                      "!l1gMIP&&!l1gTauVeto&&l1gJetPt>0&&((l1gJetPt-l1gPt)/l1gPt<=.2||l1gPt>63.)", # UCT EG iso
#                      "", # No selection for iso L1
#                      "iso_eg_eff_%i"%(L1PtCut),
#                      "Isolated EG efficiency (20GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
#                      "RECO p_{T} (GeV)")
# 
# #rlx tau
# compare_efficiencies(tau_ntuple_t, tau_ntuple_x, tau_ntuple_old,
#                      "recoPt", L1PtCut, [40, 0, 200],
#                      "", # No reco selection (rlx tau)
#                      "", # UCT tau
#                      "", # No old L1 selection
#                      "rlx_tau_eff_%i"%(L1PtCut),
#                      "Relaxed Tau Efficiency (20 GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
#                      "RECO p_{T} (GeV)")
# 
# 
# #iso tau
# compare_efficiencies(tau_ntuple_t, tau_ntuple_x, tau_ntuple_old,
#                      "recoPt", L1PtCut, [40, 0, 200],
#                      "", # reco taus automatically isolated
#                      "l1gJetPt>0&&((l1gJetPt-l1gPt)/l1gPt<=.2||l1gPt>63)", # UCT Iso Tau
#                      "", # No old L1 selection lol
#                      "iso_tau_eff_%i"%(L1PtCut),
#                      "Isolated Tau fficiency (20 GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
#                      "RECO p_{T} (GeV)")
# 
# #Jets
# 
# 
# for JetL1PtCut in [30., 50., 70., 150., 200.]:
#  
#    compare_efficiencies(jet_ntuple_t, jet_ntuple_x, jet_ntuple_old,
#                         "recoPt", JetL1PtCut, [40, 0, 200],
#                         "", # No reco selection 
#                         "", # No UCT selection
#                         "", # No old L1 selection
#                         "jet_eff_%i"%(JetL1PtCut),
#                         "Jet efficiency (%uGeV)"%(JetL1PtCut),# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
#                         "RECO p_{T} (GeV)")
# 
# 
# #Jet Resolution plot w.r.t. #PVs
# make_profile(jet_ntuple_t, "(recoPt-l1gPt)/recoPt:nPVs",
#              [35,0.,35.,100,-10.,10.],
#              "l1gMatch && (recoPt-l1gPt)/recoPt < 10 && (recoPt-l1gPt)/recoPt > -10",
#              ROOT.EColor.kRed,
#              1,
#              -3.,
#              3.,
#              "jetRes_vs_nPVs",
#              "Jet Pt Resolution Vs. # Primary Vertices",
#              "# Primary Vertices"
#              )
#              
# #Jet Resolution plot w.r.t. Pt
# make_profile(jet_ntuple_t, "(recoPt-l1gPt)/recoPt:recoPt",
#              [200,0.,200.,100,-10.,10.],
#              "l1gMatch && (recoPt-l1gPt)/recoPt < 10 && (recoPt-l1gPt)/recoPt > -10",
#              ROOT.EColor.kRed,
#              1,
#              -3.,
#              3.,
#              "jetRes_vs_pt",
#              "Jet Pt Resolution Vs. Reco Pt",
#              "Reco Pt"
#              )
#              
