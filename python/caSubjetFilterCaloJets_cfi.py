import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.CaloJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *

caSubjetFilterCaloJets = cms.EDProducer(
    "SubjetFilterJetProducer",
    CaloJetParameters,
    AnomalousCellParameters,
    jetAlgorithm = cms.string("CambridgeAachen"),
    rParam       = cms.double(1.2),
    massDropCut  = cms.double(0.667),
    asymmCut     = cms.double(0.3)
    )
