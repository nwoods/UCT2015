'''

Emulate the 2015 upgrade layer 1 calorimeter trigger

Usage:

      ./RunLayer1EmuOnFakeDigis_cfg.py

Optional arguments:

      inputFiles=myFile.root outputFile=outputFile.root maxEvents=-1

Authors: N. Woods, E. Friis, S. Dasu, T. Sarangi, M. Cepeda (UW Madison)

'''

import FWCore.ParameterSet.Config as cms
#from Configuration.StandardSequences.RawToDigi_Data_cff import *

# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# Set defaults:
options.inputFiles = '/store/user/tapas/ETauSkim/skim_12_1_erV.root'
options.outputFile = "fakeDataTestOut.root"
options.parseArguments()

process = cms.Process("Layer1Emulator")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

## process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
## process.GlobalTag.globaltag = 'GR_H_V28::All'

## process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
## process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("EmptySource")

## process.source = cms.Source("PoolSource",
##     # replace 'myfile.root' with the source file you want to use
##     fileNames = cms.untracked.vstring(options.inputFiles)
##     )

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('myOutputFile.root'),
    outputCommands = cms.untracked.vstring("keep *")
    )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.outputFile)
    )

process.FakeDigiProducer = cms.EDProducer(
    "FakeDigiProducer",
    ecalFileNames = cms.vstring("./fakeDigis/fakeEcals1.txt",
                                "./fakeDigis/fakeEcals2.txt",
                                "./fakeDigis/fakeEcals3.txt",
                                "./fakeDigis/fakeEcals4.txt"),
    hcalFileNames = cms.vstring("./fakeDigis/fakeHcals1.txt",
                                "./fakeDigis/fakeHcals2.txt",
                                "./fakeDigis/fakeHcals3.txt",
                                "./fakeDigis/fakeHcals4.txt"),
    )

process.Layer1UCTProducer = cms.EDProducer(
    "Layer1UCTProducer",
    hcalDigiSrc = cms.InputTag("fakeHcalDigis"),
    ecalDigis = cms.InputTag("fakeEcalDigis"),
    RCTCableParams = cms.PSet(
    iEtaBounds = cms.vint32(-25,-17,-9,-1,8,16,24,32),
    iPhiBounds = cms.vuint32(8,16,24,32,40,48,56,64,72),
    ),
    )

#process.load("L1Trigger.UCT2015.emulation_cfi")

process.path = cms.Path(process.FakeDigiProducer
#                        * process.Layer1UCTProducer
                        )

process.end = cms.EndPath(process.out)
