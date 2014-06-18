# Heavily modified from an auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --datatier GEN-SIM-RECO,AODSIM --conditions auto:startup -s RAW2DIGI,L1Reco,RECO,EI --eventcontent RECOSIM,AODSIM -n 1000 --no_exec --filein file:step2.root --fileout file:step3.root
import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('CommonTools.ParticleFlow.EITopPAG_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

options = VarParsing ('analysis')
options.register ('nSteps',
				  0,
				  VarParsing.multiplicity.singleton,
				  VarParsing.varType.int,
				  "Number of steps to execute")
options.parseArguments()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:step2.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('step3 nevts:100'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:step3.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    )
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

myglobalreco = cms.Sequence(process.offlineBeamSpot*process.MeasurementTrackerEvent*process.siPixelClusterShapeCache*process.standalonemuontracking*process.recopixelvertexing)

iterTrackingSteps = [ 
    process.InitialStep,
    process.DetachedTripletStep,
    process.LowPtTripletStep,
    process.PixelPairStep,
    process.MixedTripletStep,
    process.PixelLessStep,
    process.TobTecStep,
    process.earlyGeneralTracks,
    process.muonSeededStep,
    process.preDuplicateMergingGeneralTracks,
    process.generalTracksSequence,
    process.ConvStep,
    process.conversionStepTracks
]

nSteps = options.nSteps;

if nSteps > 0 :
    mySteps = process.InitialStep;
    for n in range(1, nSteps) :
        mySteps *= iterTrackingSteps[n]

    # my sequences
    myiterTracking = cms.Sequence(mySteps)
    myreconstruction = cms.Sequence(process.localreco*myglobalreco*myiterTracking)
else :
    myreconstruction = cms.Sequence(process.localreco*myglobalreco)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(myreconstruction)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.RECOSIMoutput_step)
