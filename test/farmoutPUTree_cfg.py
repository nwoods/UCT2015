#flake8: noqa
'''

Make ntuples with time- and space-averaged pileup data from zero/minbias

Usage:

    ./makePUTree_cfg.py

Optional arguments:

    inputFiles=myFile.root outputFile=outputFile.root maxEvents=-1

Authors: L. Dodd, N. Woods, I. Ojalvo, S. Dasu, M. Cepeda, E. Friis (UW Madison)

'''

import FWCore.ParameterSet.Config as cms
import os

# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# Set useful defaults
options.inputFiles = $inputFileNames
options.outputFile = "$outputFileName"
options.maxEvents = -1

options.parseArguments()

process = cms.Process("puMethodComparison")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_H_V28::All'

process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.outputFile),
    outputCommands = cms.untracked.vstring("keep *",),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('path'),
        ),
    )

process.load("L1Trigger.UCT2015.emulation_cfi")

# Determine which calibration to use
from L1Trigger.UCT2015.emulation_cfi import *

# Read inst. lumi. info from the scalers
process.load("EventFilter.ScalersRawToDigi.ScalersRawToDigi_cfi")
process.scalersRawToDigi.scalersInputTag = 'rawDataCollector'

process.EstimatedPUSubtractor = cms.EDProducer(
    "EstimatedPUSubtractor",
    regionSrc = cms.InputTag("uctDigis"),
#    PUParamSrc = cms.string("./TAvgPUParams.root"),
#    PUProfileName = cms.string("tAvgPUVsNVtx"),
)

process.makePUTree = cms.EDAnalyzer(
    "PUTree",
    regionLSB = cms.double(0.5),
    tAvgPUCut = cms.double(7.),
    xAvgPUCut = cms.double(7.),
    isMC = cms.bool(False),
)

process.p1 = cms.Path(
    process.uctDigiStep *
    process.hackHCALMIPs *
    process.uctDigis *
    process.scalersRawToDigi *
    process.EstimatedPUSubtractor *
    process.makePUTree
)

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
