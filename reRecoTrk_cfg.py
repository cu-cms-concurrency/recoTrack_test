import FWCore.ParameterSet.Config as cms

#Works for CMSSW_7_1_0_pre8.  Used ttbar GEN-SIM-RECO samples.  

process = cms.Process("RERECO")

### standard MessageLoggerConfiguration
process.load("FWCore.MessageService.MessageLogger_cfi")

### source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_7_1_0_pre8/RelValTTbar_13/GEN-SIM-RECO/PU25ns_PRE_LS171_V9-v1/00000/04EE8F23-9CE7-E311-AAC1-002590596498.root'
    )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

### conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

### standard includes (can't live without)
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

### define tracking sequences to run
# need to recreate hits from clusters (which are stored in RECO)
process.clustToHits = cms.Sequence(
    process.siPixelRecHits*process.siStripMatchedRecHits
)
# this is the official tracking sequence
process.tracking = cms.Sequence(
    process.MeasurementTrackerEvent* #Matti K. noted that these modules are needed for the reconstruction, but for whatever reason are not included in the trackingGlobalReco.  This may change with a given prerelease.
    process.siPixelClusterShapeCache*
    process.trackingGlobalReco
)

### define output EDMFile
MyReRecoEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_siPixelClusters_*_*',
    'keep *_siStripClusters_*_*',
    'keep *_offlineBeamSpot_*_*',
    'keep *_pixelVertices_*_*',
    'keep *_*_*_RERECO'),
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize=cms.untracked.int32(5*1024*1024)
)
process.out = cms.OutputModule("PoolOutputModule",
    MyReRecoEventContent,
    fileName = cms.untracked.string('reReco_trk.root')
)

### paths
process.p = cms.Path(process.clustToHits * process.tracking)
process.o = cms.EndPath(process.out)

### sequence of paths to run
process.schedule = cms.Schedule(process.p,process.o)

### debug time and memory information (more at Validation/Performance/python//TimeMemory*.py)
#process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
#process.SimpleMemoryCheck=cms.Service("SimpleMemoryCheck",
#                                      ignoreTotal=cms.untracked.int32(1),
#                                      oncePerEventMode=cms.untracked.bool(False))
