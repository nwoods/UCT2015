'''

Script to make some quick efficiency and resolution plots

Author: Evan K. Friis, UW Madison; modified by Nate Woods

Usage: python compareEfficiency.py TAVGPUFILE.root XAVGPUFILE.root NOPUFILE.root label[optional]

E.G.
python compareEfficiency.py TAVGPUFILE.root NOPUFILE.root v3_
will produce rate plots in ~/www/v3_filename.png

python compareEfficiency.py TAVGPUFILE.root NOPUFILE.root UCT2015/test_  
will produce rate plots in ~/www/UCT2015/test_filename.png 

python compareEfficiency.py TAVGPUFILE.root NOPUFILE.root
will produce rate plots in ~/www/filename.png


'''

#######################
### GET EVERYTHING ####
#######################
from sys import argv, stdout, stderr

import ROOT

from array import array

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
   print 'Usage:python allRatePlots.py tAvgPUFile.root noPUFile.root label[optional]'
   exit()

Infile_T = argv[1]
Infile_N = argv[2]
ntuple_file_t = ROOT.TFile(Infile_T)
ntuple_file_n = ROOT.TFile(Infile_N)
ntuple_file_old = ROOT.TFile(Infile_T)
ntuple_file_qj = ROOT.TFile(Infile_N)

def setn():
   global n #stupid hack to make profiles work correctly
   n=0

####################################
######## LABEL & SAVE WHERE #########
####################################

if len(argv)>3:
   saveWhere='~/www/'+argv[3]
else:
   saveWhere='~/www/'

### EG NTuples
eg_ntuple_old = ntuple_file_old.Get("rlxEGEfficiency/Ntuple")
iso_eg_ntuple_old = ntuple_file_old.Get("rlxUCTisoL1EGEfficiency/Ntuple")
eg_ntuple_t = ntuple_file_t.Get("rlxEGEfficiency/Ntuple")
eg_ntuple_n = ntuple_file_n.Get("rlxEGEfficiency/Ntuple")

### Tau NTuples
tau_ntuple_old = ntuple_file_old.Get("rlxTauEfficiency/Ntuple")
tau_ntuple_t = ntuple_file_t.Get("rlxTauEfficiency/Ntuple")
tau_ntuple_n = ntuple_file_n.Get("rlxTauEfficiency/Ntuple")

### Jet NTuples
jet_ntuple_old = ntuple_file_old.Get("corrjetEfficiency/Ntuple")
jet_ntuple_t = ntuple_file_t.Get("corrjetEfficiency/Ntuple")
jet_ntuple_n = ntuple_file_n.Get("jetEfficiency/Ntuple")
quadJet_ntuple = ntuple_file_qj.Get("quadJetEfficiency/Ntuple")

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


def make_profile(Ntuple, variables, binning, selection, color, markerStyle, min, max, file_title="profile", title='',xaxis='',yaxis=''):
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
   frame.GetYaxis().SetTitle(yaxis)
   frame.Draw()
   prof.Draw('e')
   filename = saveWhere + file_title + '.png'
   canvas.SaveAs(filename)
   

def make_many_profiles(NtupleList, variableList, xVariable, binning, selectionList, colorList, markerList, min, max, nameList, file_title="many_profiles", title='',xaxis='',yaxis=''):
   ''' Make and save an arbitrary number of profiles '''
   if not (len(NtupleList) == len(colorList) and len(NtupleList) == len(colorList) and len(NtupleList) == len(nameList) and len(NtupleList) == len(variableList) and len(NtupleList) == len(selectionList)):
      print "List arguments to make_many_profiles must be the same size"
      return

   profList=[]
   legend = ROOT.TLegend(0.3, 0.7, 0.5, 0.9, "", "brNDC")
   legend.SetFillColor(ROOT.EColor.kWhite)
   legend.SetBorderSize(1)

   for i in range(len(NtupleList)):
      plot = make_plot(NtupleList[i], variableList[i]+":"+xVariable, selectionList[i], binning, xaxis, title)
      prof = plot.ProfileX()
      prof.SetMarkerSize(1.5)
      prof.SetMarkerStyle(markerList[i])
      prof.SetMarkerColor(colorList[i])
      prof.SetLineColor(colorList[i])
      prof.SetMinimum(min)
      prof.SetMaximum(max)
      profList.append(prof)
      legend.AddEntry(profList[i], nameList[i], "pe")

   framebins = binning[0:3]
      
   frame = ROOT.TH1F("frame", "frame", *framebins)
   frame.SetMaximum(max)
   frame.SetMinimum(min)
   frame.SetTitle(title)
   frame.GetXaxis().SetTitle(xaxis)
   frame.GetYaxis().SetTitle(yaxis)
   frame.Draw()
   for prof in profList:
      prof.Draw('esame')
   legend.Draw()
   filename = saveWhere + file_title + '.png'
   canvas.SaveAs(filename)
   

def compare_efficiencies(Ntuple_t, Ntuple_n, oldNtuple, variable, ptCut,
                         binning, selection_reco="", selection_uct="",
                         selection_old="", file_title="plot",
                         title='', xaxis=''):
    ''' Returns a (L1, L1G) tuple of TGraphAsymmErrors '''
    denom_t = make_plot(
        Ntuple_t, variable,
        selection_reco, 
        binning
        )
    denom_n = make_plot(
        Ntuple_n, variable,
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
    num_n = make_plot(
        Ntuple_n, variable,
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
    eff_n = make_efficiency(denom_n, num_n, ROOT.EColor.kGreen, 21)
    eff_old = make_efficiency(denom_n, num_old, ROOT.EColor.kRed, 20)
    frame.SetMaximum(1.2)
    frame.SetTitle(title)
    frame.GetXaxis().SetTitle(xaxis)
    frame.Draw()
    eff_n.Draw('pe')
    eff_old.Draw('pe')
    eff_t.Draw('pe')
    legend = ROOT.TLegend(0.7, 0.2, 0.89, 0.4, "", "brNDC")
    legend.SetFillColor(ROOT.EColor.kWhite)
    legend.SetBorderSize(1)
    legend.AddEntry(eff_t, "UCT (time avg PU)", "pe")
    legend.AddEntry(eff_n, "UCT (space avg PU)", "pe")
    legend.AddEntry(eff_old, "Current", "pe")
    legend.Draw()
    filename = saveWhere + file_title + '.png'
    canvas.SaveAs(filename)

################################################################################
# Relaxed tau efficiency for a 20 GeV cut on L1
################################################################################

setn()


L1PtCut = 20

#rlx EG no tauVeto
compare_efficiencies(eg_ntuple_t, eg_ntuple_n, eg_ntuple_old,
                     "recoPt", L1PtCut, [40, 0, 200],
                     "", # No reco selection (rlx EG)
                     "!l1gMIP&&(!l1gTauVeto||l1gPt>63)", # UCT EG
                     "", # No old L1 selection
                     "rlx_eg_eff_%i_noTauVetoCut"%(L1PtCut),
                     "Relaxed EG efficiency (20GeV, no tau veto above 63 GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
                     "RECO p_{T} (GeV)")

#iso EG No tauVeto
compare_efficiencies(eg_ntuple_t, eg_ntuple_n, iso_eg_ntuple_old,
                     "recoPt", L1PtCut, [40, 0, 200],
                     "dr03CombinedEt/recoPt < 0.1", # Isolated reco
                     "!l1gMIP&&(!l1gTauVeto||l1gPt>63)&&l1gJetPt>0&&((l1gJetPt-l1gPt)/l1gPt<=.2||l1gPt>63.)", # UCT EG iso
                     "", # No selection for iso L1
                     "iso_eg_eff_%i_noTauVeto"%(L1PtCut),
                     "Isolated EG efficiency (20GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
                     "RECO p_{T} (GeV)")


#rlx tau
compare_efficiencies(tau_ntuple_t, tau_ntuple_n, tau_ntuple_old,
                     "recoPt", L1PtCut, [40, 0, 200],
                     "", # No reco selection (rlx tau)
                     "", # UCT tau
                     "", # No old L1 selection
                     "rlx_tau_eff_%i"%(L1PtCut),
                     "Relaxed Tau Efficiency (20 GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
                     "RECO p_{T} (GeV)")


#iso tau
compare_efficiencies(tau_ntuple_t, tau_ntuple_n, tau_ntuple_old,
                     "recoPt", L1PtCut, [40, 0, 200],
                     "", # reco taus automatically isolated
                     "l1gJetPt>0&&((l1gJetPt-l1gPt)/l1gPt<=.2||l1gPt>63)", # UCT Iso Tau
                     "", # No old L1 selection lol
                     "iso_tau_eff_%i"%(L1PtCut),
                     "Isolated Tau fficiency (20 GeV)",# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
                     "RECO p_{T} (GeV)")



#Quad Jet

bins = [0., 30.,34.,38.,42.,46.,50.,54.,58.,62.,66.,70.,78.,86.,102.,118.,140.]

qjNum = ROOT.TH1F("qjNum","Quad Jet Efficiency Numerator",len(bins)-1,array('d',bins))
qjDenom = ROOT.TH1F("qjDenom", "Quad Jet Efficiency Denominator",len(bins)-1,array('d',bins))
for evt in recoPT:
   for recoCand in range(len(recoPT[evt])):
      fourthPt = -2
      if recoIndex[evt][recoCand] == 3:
         fourthPt = recoPT[evt][recoCand]
         qjDenom.Fill(fourthPt)
         for l1Cand in range(len(l1PT[evt])):
            if match[evt][l1Cand] == 1 and l1Index[evt][l1Cand] == 4 and l1PT[evt][l1Cand] >= ptCut:
               qjNum.Fill(fourthPt)

qjEff = make_efficiency(qjDenom, qjNum, ROOT.EColor.kRed, 20)
frame = ROOT.TH1F("frame", "frame", len(bins)-1,array('d',bins))
frame.SetMaximum(1.2)
frame.SetTitle("4-Jet Efficiency (40GeV)")
frame.GetXaxis().SetTitle("4th Jet Reco P_{T}")
frame.Draw()
qjEff.Draw('pe')
filename = saveWhere + 'quadJetEfficiency.png'
canvas.SaveAs(filename)


#Jets


for JetL1PtCut in [30., 50., 70., 150., 200.]:
 
   compare_efficiencies(jet_ntuple_t, jet_ntuple_n, jet_ntuple_old,
                        "recoPt", JetL1PtCut, [40, 0, 200],
                        "l1gPt > recoPt * .3 && l1gPt < recoPt * 1.7", # reco selection 
                        "", # no UCT selection
                        "", # no old L1 selection
                        "jet_eff_%i"%(JetL1PtCut),
                        "Jet efficiency (%uGeV)"%(JetL1PtCut),# jetRelIso<%0.2f regRelIso<%0.2f" % (jetCut, regCut),
                        "RECO p_{T} (GeV)")

jet_ntuple_list = [jet_ntuple_t,jet_ntuple_n,jet_ntuple_old]
jet_drawstring_list = ["(recoPt-l1gPt)/recoPt","(recoPt-l1gPt)/recoPt",
                    "(recoPt-l1Pt)/recoPt"]
jet_selection_list = ["l1gMatch && recoPt > 20 && l1gPt > recoPt * .3 && l1gPt < recoPt * 1.7",
                    "l1gMatch && recoPt > 20 && l1gPt > recoPt * .3 && l1gPt < recoPt * 1.7", 
                    "l1Match && recoPt > 20 && l1gPt > recoPt * .3 && l1gPt < recoPt * 1.7"] 
color_list = [ROOT.EColor.kBlue,ROOT.EColor.kGreen,ROOT.EColor.kRed]
legend_name_list = ["Time avg PU","Space avg PU", "Current"]



#Jet Resolution plot w.r.t. #PVs
make_many_profiles(jet_ntuple_list,
                   jet_drawstring_list,
                   "nPVs",
                   [35,0.,35.,100,-10.,10.],
                   jet_selection_list,
                   color_list,
                   [22,21,20],
                   -1.,1.,
                   legend_name_list,
                   "jetResVsNPVs",
                   "Jet Resolution vs # Primary Vertices",
                   "# Primary Vertices",
                   "(RecoPt - L1Pt)/RecoPt"
                   )

                        
#Jet Resolution plot w.r.t. #Pt
make_many_profiles(jet_ntuple_list,
                   jet_drawstring_list,
                   "recoPt",
                   [25,0.,200.,100,-10.,10.],
                   jet_selection_list,
                   color_list,
                   [22,21,20],
                   -1.,1.,
                   legend_name_list,
                   "jetResVsPt",
                   "Jet Resolution vs Pt",
                   "Reco Pt",
                   "(RecoPt - L1Pt)/RecoPt"
                   )

ptComp2d_t = make_plot(jet_ntuple_t, "l1gPt:recoPt", "l1gPt>2.&&recoPt>2.",
                       [80,0.,80.,80,0.,80.],
                       "Reco Pt (GeV)",
                       "L1 Jet Pt (time average) Vs Reco Jet Pt")
frame = ROOT.TH2F("frame", "frame", 80,0.,80.,80,0.,80.)
frame.SetTitle("L1 Jet Pt Vs Reco Jet Pt")
frame.GetXaxis().SetTitle("Reco Pt")
frame.GetYaxis().SetTitle("L1 Jet Pt (time average PU)")
frame.Draw()
ptComp2d_t.Draw('colzsame')
filename=saveWhere + "tAvgPUJetPtVsRecoPt_80.png"
canvas.SaveAs(filename)

ptComp2d_n = make_plot(jet_ntuple_n, "l1gPt:recoPt", "l1gPt>2.&&recoPt>2.",
                       [80,0.,80.,80,0.,80.],
                       "Reco Pt (GeV)",
                       "L1 Jet Pt (time average) Vs Reco Jet Pt")
frame = ROOT.TH2F("frame", "frame", 80,0.,80.,80,0.,80.)
frame.SetTitle("L1 Jet Pt Vs Reco Jet Pt")
frame.GetXaxis().SetTitle("Reco Pt")
frame.GetYaxis().SetTitle("L1 Jet Pt (time average PU)")
frame.Draw()
ptComp2d_n.Draw('colzsame')
filename=saveWhere + "uncalJetPtVsRecoPt_80.png"
canvas.SaveAs(filename)



mlo = []
blo = []
mhi = []
bhi = []
boundmd = [40.,40.,45.,40.,
           65.,45.,45.,55.,
           60.,65.,65.,]#65.,
#            65.,65.,50.,50.,
#            50.,65.,50.,30.,
#            45.,25.,]
boundhi = [70.,140.,160.,200.,
           200.,200.,200.,200.,
           200.,190.,200.,]#195.,
#            200.,200.,170.,185.,
#            180.,185.,165.,165.,
#            130.,70.,]
for eta in range (11):
   print "Eta = %i & %i" % (eta,21-eta)
   hist2 = make_plot(jet_ntuple_t, "recoPt:l1gPt",
                     "l1gPt>25.&&recoPt>25.&&l1gPt>.3*recoPt&&l1gPt<1.7*recoPt&&l1gMatch&&(l1gEtaCode==%i||l1gEtaCode==%i)"%(eta,21-eta),
                     [300,0.,300.,300,0.,300.],
                     "Reco Pt (GeV)",
                     "L1 Jet Pt (time average) Vs Reco Jet Pt")
   prof = hist2.ProfileX()
   fnlo = ROOT.TF1("fnlo","pol1",5.,boundmd[eta])
   fnhi = ROOT.TF1("fnhi","pol1",boundmd[eta],boundhi[eta])
   prof.Fit("fnlo","QR")
   blo.append(fnlo.GetParameter(0))
   print "BLO = %0.5f" % blo[eta]
   mlo.append(fnlo.GetParameter(1))
   print "MLO = %0.5f" % mlo[eta]
   print "X^2 LO = %0.2f" % fnlo.GetChisquare()
   prof.Fit("fnhi","QR")
   bhi.append(fnhi.GetParameter(0))
   print "BHI = %0.5f" % bhi[eta]
   mhi.append(fnhi.GetParameter(1))
   print "MHI = %0.5f" % mhi[eta]
   print "X^2 HI = %0.2f" % fnhi.GetChisquare()
   prof.SetLineColor(ROOT.EColor.kBlue)
   prof.SetMarkerColor(ROOT.EColor.kBlue)
   prof.SetMarkerSize(1.2)
   prof.SetMarkerStyle(23)
   prof.SetMaximum(230.)
   prof.SetMinimum(0.)
   frame = ROOT.TH1F("frame", "frame", 200,0.,200.)
   frame.SetMaximum(230.)
   frame.SetMinimum(0.)
   frame.SetTitle("L1 Jet Pt Vs Reco Jet Pt, iEta=%i&%i"%(eta,21-eta))
   frame.GetXaxis().SetTitle("L1 Jet Pt (time average PU)")
   frame.GetYaxis().SetTitle("Reco Jet Pt")
   frame.Draw()
   prof.Draw('ESAME')
   filename=saveWhere + ("fitPlots/JetCalPt%i.png"%eta)
   canvas.SaveAs(filename)

print "MLO = "
for i in range(11):
   print mlo[i]

print "\nBLO = "
for i in range(11):
   print blo[i]

print "\nMHI = "
for i in range(11):
   print mhi[i]

print "\nBHI = "
for i in range(11):
   print bhi[i]
