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
#options.inputFiles = '/store/user/tapas/ETauSkim/skim_12_1_erV.root'
options.outputFile = "fakeDataTestOut.root"
#options.debug = True ## causing crash?
options.parseArguments()

process = cms.Process("Layer1EmulatorOnFakeData")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

## process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
## process.GlobalTag.globaltag = 'GR_H_V28::All'

## process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
## process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(7) )

process.source = cms.Source("EmptySource")

## process.source = cms.Source("PoolSource",
##     # replace 'myfile.root' with the source file you want to use
##     fileNames = cms.untracked.vstring(options.inputFiles)
##     )

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(options.outputFile),
    outputCommands = cms.untracked.vstring("keep *"),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('path')
        )
    )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.outputFile)
    )

process.FakeDigiProducer = cms.EDProducer(
    "FakeDigiProducer",
    ecalFileNames = cms.vstring("./fakeDigis/eCluster0.txt",
                                "./fakeDigis/eCluster1.txt",
                                "./fakeDigis/eCluster2.txt",
                                "./fakeDigis/eCluster3.txt",
                                "./fakeDigis/eCluster4.txt",
                                "./fakeDigis/eCluster5.txt",
                                "./fakeDigis/eCluster6.txt",),
    hcalFileNames = cms.vstring("./fakeDigis/hcalTest0.txt",
                                "./fakeDigis/hcalTest1.txt",
                                "./fakeDigis/hcalTest2.txt",
                                "./fakeDigis/hcalTest3.txt",
                                "./fakeDigis/hcalTest4.txt",
                                "./fakeDigis/hcalTest5.txt",
                                "./fakeDigis/hcalTest6.txt",),
    doDebug = cms.bool(True),#options.debug),
    )

process.FakeRegionProducer = cms.EDProducer(
    "FakeRegionProducer",
    regFileNames = cms.vstring("./fakeRegions/stage1Regions0.txt",
                               "./fakeRegions/stage1Regions1.txt",
                               "./fakeRegions/stage1Regions2.txt",
                               "./fakeRegions/stage1Regions3.txt",
                               "./fakeRegions/stage1Regions4.txt",
                               "./fakeRegions/stage1Regions5.txt",
                               "./fakeRegions/stage1Regions6.txt",),
    candFileNames = cms.vstring("./fakeCands/stage1EmCands0.txt",
                                "./fakeCands/stage1EmCands1.txt",
                                "./fakeCands/stage1EmCands2.txt",
                                "./fakeCands/stage1EmCands3.txt",
                                "./fakeCands/stage1EmCands4.txt",
                                "./fakeCands/stage1EmCands5.txt",
                                "./fakeCands/stage1EmCands6.txt",),
    doDebug = cms.bool(True),
    )

process.Layer1UCTProducer = cms.EDProducer(
    "Layer1UCTProducer",
    hcalDigiSrc = cms.InputTag("FakeDigiProducer","fakeHcalDigis"),
    ecalDigis = cms.InputTag("FakeDigiProducer","fakeEcalDigis"),
    RCTCableParams = cms.PSet(
    iEtaBounds = cms.vint32(-27,-21,-15,-9,-3,2,8,14,20,26,32),
    #(-25,-17,-9,-1,8,16,24,32),
    iPhiBounds = cms.vuint32(4,10,16,22,28,34,40,46,52,58,64,70),
    #(8,16,24,32,40,48,56,64,72),
    ecalLSB = cms.double(0.5),
    regEtaBounds = cms.vint32(21,),
    regPhiBounds = cms.vint32(17,),
    ),
    rctRegions = cms.InputTag("FakeRegionProducer","fakeRegions"),
    emCands = cms.InputTag("FakeRegionProducer","fakeCands"),
    doDebug = cms.bool(True),
    )

#process.load("L1Trigger.UCT2015.emulation_cfi")

process.path = cms.Path(process.FakeDigiProducer
                        * process.Layer1UCTProducer
                        )

process.end = cms.EndPath(process.out)
