'''

Script to make rate plots for both PU methods and compare them

Author: N. Woods, M.Cepeda, S. Dasu, L. Dodd, E. Friis  	  UW Madison


Usage: python comparePURates.py TAVGPUFILE.root XAVGPUFILE.root label[optional] 

 
E.G.
python allRatePlots.py TAVGPUFILE.root XAVGPUFILE.root v3
will produce rate plots in ~/www/v3_filename.png

python alRatesPlots.py TAVGPUFILE.root XAVGPUFILE.root UCT2015/test  
will produce rate plots in ~/www/UCT2015/test_filename.png 

python allRatePlots.py TAVGPUFILE.root XAVGPUFILE.root
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

TInfile = argv[1]
XInfile = argv[2]
t_ntuple_file = ROOT.TFile(TInfile)
x_ntuple_file = ROOT.TFile(XInfile)

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
 


####################################
####### GET NTUPLES #########
####################################
#Jet Candidates
jet_old_ntuple = x_ntuple_file.Get("jetL1Rate/Ntuple")
t_jet_uct_ntuple = t_ntuple_file.Get("corrjetUCTRate/Ntuple")
x_jet_uct_ntuple = x_ntuple_file.Get("corrjetUCTRate/Ntuple")
#x_jet_uct_ntuple = x_ntuple_file.Get("corrjetUCTRate/Ntuple")

#EG Candidates
iso_old_eg_ntuple = x_ntuple_file.Get("isoEGL1Rate/Ntuple")
rlx_old_eg_ntuple = x_ntuple_file.Get("rlxEGL1Rate/Ntuple")
t_rlx_uct_eg_ntuple = t_ntuple_file.Get("rlxEGUCTRate/Ntuple")
x_rlx_uct_eg_ntuple = x_ntuple_file.Get("rlxEGUCTRate/Ntuple")

#Tau Candidates
rlx_old_tau_ntuple = x_ntuple_file.Get("tauL1Rate/Ntuple")
t_rlx_uct_tau_ntuple = t_ntuple_file.Get("rlxTauUCTRate/Ntuple")
x_rlx_uct_tau_ntuple = x_ntuple_file.Get("rlxTauUCTRate/Ntuple")


################################################################################
#######################        DRAWING BELOW         ###########################
################################################################################

canvas = ROOT.TCanvas("asdf", "adsf", 800, 600)


################################################################################
#  make_plot uses draw() method to draw pt to be used in make_ _rate method    #
################################################################################

def make_plot(tree, variable, selection, binning, xaxis='', title='', calFactor=1):
    ''' Plot a variable using draw and return the histogram '''
    draw_string = "%s * %0.2f>>htemp(%s)" % (variable, calFactor, ", ".join(str(x) for x in binning))
    print draw_string
    tree.Draw(draw_string, selection, "goff")
    output_histo = ROOT.gDirectory.Get("htemp").Clone()
    output_histo.GetXaxis().SetTitle(xaxis)
    output_histo.SetTitle(title)
    return output_histo

######################################################################################
#  make_ _rate method calculates rate given a pt histogram and returns histogram     #
######################################################################################

def make_rate(pt, color, markerStyle):
    ''' Make a rate plot with speficied color and marker style '''
    numBins = pt.GetXaxis().GetNbins()
    rate = pt.Clone()

    for i in range(1, numBins+1):
        rate.SetBinContent(i, pt.Integral(i, numBins))

    rate.SetLineColor(color)
    rate.SetMarkerStyle(markerStyle)
    rate.SetMarkerColor(color)

    return rate    

 
def make_old_rate(pt):
    ''' Make a rate plot out of L1Extra Pts '''
    numBins = pt.GetXaxis().GetNbins()
    rate = pt.Clone()

    for i in range(1, numBins+1):
        rate.SetBinContent(i, pt.Integral(i, numBins))

    rate.SetLineColor(ROOT.EColor.kRed)
    rate.SetMarkerStyle(20)
    rate.SetMarkerColor(ROOT.EColor.kRed)

    return rate


def make_uct_rate_t(pt):
    ''' Return a rate plot out of UCT Pts (time average PU)'''
    numBins = pt.GetXaxis().GetNbins()
    rate = pt.Clone()

    for i in range(1, numBins+1):
        rate.SetBinContent(i, pt.Integral(i, numBins))

    rate.SetLineColor(ROOT.EColor.kBlue)
    rate.SetMarkerStyle(22)
    rate.SetMarkerColor(ROOT.EColor.kBlue)

    return rate


def make_uct_rate_x(pt):
    ''' Return a rate plot out of UCT Pts (space averagePU)'''
    numBins = pt.GetXaxis().GetNbins()
    rate = pt.Clone()

    for i in range(1, numBins+1):
        rate.SetBinContent(i, pt.Integral(i, numBins))

    rate.SetLineColor(ROOT.EColor.kGreen)
    rate.SetMarkerStyle(21)
    rate.SetMarkerColor(ROOT.EColor.kGreen)

    return rate



############################################################################
#     plotRates method will be used to Draw rates. Calls make_plot         #
#
#        l1_pt[pt plot used to calculate rate] = make_plot( [function above]    #
#        l1ntuple [l1 or uct], 'pt'[variable name],    				# 
#        "", [selection such as eta cut goes here]  				#
#        binning, [used later]  						#
#        '','', [axis titles]							#
#        L1_CALIB_FACTOR [Calibration factor, default is now zero]          	#
#        )									#
#################################################################################

def plotRates(oldntuple, tuctntuple, xuctntuple, binning, filename, title='', xaxis='', uctSelection=""):
    ''' Save a rate Plot '''

    old_pt = make_plot(
        oldntuple, 'pt',
        "", # No Selection
        binning,
        '','', # No Titles
        L1_CALIB_FACTOR
        )
    t_uct_pt = make_plot(
        tuctntuple, 'pt',
        uctSelection,
        binning,
        '','', # No Titles
        L1G_CALIB_FACTOR
        )
    x_uct_pt = make_plot(
        xuctntuple, 'pt',
        uctSelection,
        binning,
        '','',
        L1G_CALIB_FACTOR
        )

    oldRate = make_rate(old_pt, ROOT.EColor.kRed, 20)
    tUCTRate = make_rate(t_uct_pt, ROOT.EColor.kBlue, 22)
    xUCTRate = make_rate(x_uct_pt, ROOT.EColor.kGreen, 21)

    canvas.SetLogy()
    oldRate.SetTitle(title)
    oldRate.GetXaxis().SetTitle(xaxis)
    oldRate.Draw('ph')
    xUCTRate.Draw('phsame')
    tUCTRate.Draw('phsame')
    legend = ROOT.TLegend(0.6, 0.5, 0.89, 0.7, "", "brNDC")
    legend.SetFillColor(ROOT.EColor.kWhite)
    legend.SetBorderSize(1)
    legend.AddEntry(tUCTRate, "UCT (time avg PU)", "p")
    legend.AddEntry(xUCTRate, "UCT (space avg PU)", "p")
    legend.AddEntry(oldRate, "Current", "p")
    legend.Draw("same")
    saveas = saveWhere+filename+'.png' 
    print 'will be saved as %s'%saveas
    canvas.SaveAs(saveas)

################################################################################
#############       Final Drawng of Rates                        ###############
#############       plotRates( L1 Eff Ntuple, UCT Eff Ntuple     ###############
#############                  [#bins, min, max],                ###############
#############                  filename saved as png,            ###############
#############                  Hist Title, X asix label)         ###############
################################################################################

#Rlx EG
plotRates(rlx_old_eg_ntuple, t_rlx_uct_eg_ntuple, x_rlx_uct_eg_ntuple,
          [40, 0, 150],
          'rlx_eg_rate_noTauVeto',
          "Relaxed EG Rate", "L1 p_{T} (GeV)",
          "!mipBit&&((!tauVeto&&pt<63)||pt>63)")

#Rlx tau
plotRates(rlx_old_tau_ntuple, t_rlx_uct_tau_ntuple, x_rlx_uct_tau_ntuple,
          [40, 0, 150],
          'rlx_tau_rate',
          "Relaxed Tau Rate", "L1 p_{T} (GeV)")

# #Iso EG
# plotRates(iso_old_eg_ntuple, t_rlx_uct_eg_ntuple, x_rlx_uct_eg_ntuple,
#           [40, 0, 150],
#           'iso_eg_rate',
#           "Isolated EG Rate", "L1 p_{T} (GeV)",
#           "jetPt>0&&(jetPt-pt)/pt<=.1&&!mipBit&&!tauVeto")
# 

#Iso EG No TauVeto
plotRates(iso_old_eg_ntuple, t_rlx_uct_eg_ntuple, x_rlx_uct_eg_ntuple,
          [40, 0, 150],
          'iso_eg_rate',
          "Isolated EG Rate (No tauVeto above 63 GeV)", "L1 p_{T} (GeV)",
          "((jetPt>0&&(jetPt-pt)/pt<=.1)||pt>63)&&!mipBit&&((!tauVeto&&pt<63)||pt>63)")

#Iso Tau
plotRates(rlx_old_tau_ntuple, t_rlx_uct_tau_ntuple, x_rlx_uct_tau_ntuple,
          [40, 0, 150],
          'iso_tau_rate',
          "Isolated Tau Rate", "L1 p_{T} (GeV)",
          "(jetPt>0&&(jetPt-pt)/pt<=.1)||pt>63")

#Singe Jet
plotRates(jet_old_ntuple,t_jet_uct_ntuple,x_jet_uct_ntuple,
          [40, 0, 200],
          'jet_rate',
          "Jet Rate", "L1 p_{T} (GeV)")





