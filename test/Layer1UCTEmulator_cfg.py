'''

Emulate the 2015 upgrade layer 1 calorimeter trigger

Usage:

      ./Layer1UCTEmulator_cfg.py

Optional arguments:

      inputFiles=myFile.root outputFile=outputFile.root maxEvents=-1

Authors: N. Woods, E. Friis, S. Dasu, T. Sarangi (UW Madison)

'''

import FWCore.ParameterSet.Config as cms
#from Configuration.StandardSequences.RawToDigi_Data_cff import *

# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# Set defaults:
options.inputFiles = '/store/user/tapas/ETauSkim/skim_12_1_erV.root'
options.outputFile = "caloTriggerEmulatorOut.root"
options.maxEvents = 200
options.parseArguments()

process = cms.Process("Layer1Emulator")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_H_V28::All'

process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents),
    )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(options.inputFiles)
    )

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string(options.outputFile),
    outputCommands = cms.untracked.vstring("drop *",
                                           "keep CTPOutput*_*_*_*"),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('path'),
        ),
    )

## process.TFileService = cms.Service(
##     "TFileService",
##     fileName = cms.string('testingOutput.root'),#options.outputFile),
##     )

process.Layer1UCTProducer = cms.EDProducer(
    "Layer1UCTProducer",
    hcalDigiSrc = cms.InputTag("hcalDigis"),
    ecalDigis = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
    RCTCableParams = cms.PSet(
    iEtaBounds = cms.vint32(-27,-21,-15,-9,-3,2,8,14,20,26,32),
    iPhiBounds = cms.vuint32(4,10,16,22,28,34,40,46,52,58,64,70),
    ecalLSB = cms.double(0.5),
    ),
    doDebug = cms.bool(False)
    )

## process.uctDigiStep = cms.Sequence(
##     # Only do the digitization of objects that we care about
##     #RawToDigi
##     process.gctDigis
##     * process.gtDigis
##     * process.ecalDigis
##     * process.hcalDigis
## )

process.load("L1Trigger.UCT2015.emulation_cfi")

process.path = cms.Path(process.uctDigiStep
                        * process.Layer1UCTProducer
                        )

process.end = cms.EndPath(process.out)
