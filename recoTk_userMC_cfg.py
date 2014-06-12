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
                                fileNames = cms.untracked.vstring('file:TTbar_8TeV_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT.root')
                            )

process.options = cms.untracked.PSet(

    )

# Production Info
process.configurationMetadata = cms.untracked.PSet(
        version = cms.untracked.string('$Revision: 1.19 $'),
            annotation = cms.untracked.string('step3 nevts:10'),
            name = cms.untracked.string('Applications')
        )

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
                                             splitLevel = cms.untracked.int32(0),
                                             eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
                                             outputCommands = process.RECOSIMEventContent.outputCommands,
                                             fileName = cms.untracked.string('recoTrack_TTbar8TeV.root'),
                                             dataset = cms.untracked.PSet(
            filterName = cms.untracked.string(''),
                    dataTier = cms.untracked.string('GEN-SIM-RECO')
                )
                                         )

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)




#Extra stuff from Matti
process.myreconstruction = cms.Sequence(process.trackerlocalreco+
                                                                                process.muonlocalreco+process.calolocalreco+process.offlineBeamSpot+
                                                                                process.MeasurementTrackerEvent+
                                                                                process.siPixelClusterShapeCache+
                                                                                process.standalonemuontracking+process.recopixelvertexing+process.iterTracking+
                                                                                process.electronSeedsSeq+process.doAlldEdXEstimators+process.trackExtrapolator+
                                                                                process.vertexreco+process.logErrorHarvester)
process.reconstruction_step = cms.Path(process.myreconstruction)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)
