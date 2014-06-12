# Auto generated configuration file to be used for running tracking reco from user produced MC
import FWCore.ParameterSet.Config as cms

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
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
    'root://xrootd.unl.edu//store/relval/CMSSW_5_3_13_patch3/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START53_LV3_Feb24-v1/00000/2A1180FB-5C9D-E311-8CD5-003048D375A8.root') #use xrootd to remotely access data at different sites
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('step3 nevts:10'),
    name = cms.untracked.string('Applications')
)
 
#Output definition
process.RECOSIMoutput = cms.OutputModule("PoolOutputModule", 
                                         splitLevel = cms.untracked.int32(0),
                                         eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
                                         outputCommands = process.RECOSIMEventContent.outputCommands,
                                         fileName = cms.untracked.string('ttbar_timing_5_3_13.root'),
                                         dataset = cms.untracked.PSet(filterName = cms.untracked.string(''),
                                                                      dataTier = cms.untracked.string('GEN-SIM-RECO')
                                                                      )                                       
                                         )

# Global Tags
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Keep track of timing
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

#Choose steps of iterative tracking
import FWCore.ParameterSet.Config as cms

from RecoTracker.IterativeTracking.InitialStep_cff import *
from RecoTracker.IterativeTracking.LowPtTripletStep_cff import *
from RecoTracker.IterativeTracking.PixelPairStep_cff import *
from RecoTracker.IterativeTracking.DetachedTripletStep_cff import *
from RecoTracker.IterativeTracking.MixedTripletStep_cff import *
from RecoTracker.IterativeTracking.PixelLessStep_cff import *
from RecoTracker.IterativeTracking.TobTecStep_cff import *
from RecoTracker.FinalTrackSelectors.MergeTrackCollections_cff import *
from RecoTracker.ConversionSeedGenerators.ConversionStep_cff import *

#Taken from RecoTracker.Configuration.iterativeTracking_cff
process.myiterTracking = cms.Sequence(InitialStep*
                            LowPtTripletStep*
                            PixelPairStep*
                            DetachedTripletStep*
                            MixedTripletStep*
                            PixelLessStep*
                            TobTecStep*
                            generalTracks*
                            ConvStep*
                            conversionStepTracks
                            )



#Perform the Reconstruction
process.myreconstruction = cms.Sequence(process.trackerlocalreco+
                                        process.muonlocalreco+
                                        process.calolocalreco+
                                        process.offlineBeamSpot+
#                                        process.MeasurementTrackerEvent+
#                                        process.siPixelClusterShapeCache+
#                                        process.standalonemuontracking+
                                        process.recopixelvertexing+
                                        process.myiterTracking+
                                        process.electronSeedsSeq+
                                        process.doAlldEdXEstimators+
                                        process.trackExtrapolator+
                                        process.vertexreco+
                                        process.logErrorHarvester)
process.reconstruction_step = cms.Path(process.myreconstruction)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.RECOSIMoutput_step,process.endjob_step)

