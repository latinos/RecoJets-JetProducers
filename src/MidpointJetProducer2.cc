// File: MidpointJetProducer2.cc
// Description:  see MidpointJetProducer2.h
// Author:  M. Paterno
// Creation Date:  MFP Apr. 6 2005 Initial version.
// Revision:  R. Harris,  Oct. 19, 2005 Modified to use real CaloTowers from Jeremy Mans
//
//--------------------------------------------
#include <memory>

#include "RecoJets/JetProducers/interface/MidpointJetProducer2.h"
#include "DataFormats/JetObjects/interface/CaloJetCollection.h"
#include "RecoJets/JetAlgorithms/interface/CaloJetMaker.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "FWCore/Framework/interface/Handle.h"

using namespace std;
using namespace reco;

namespace cms
{

  // Constructor takes input parameters now: to be replaced with parameter set.

  MidpointJetProducer2::MidpointJetProducer2(edm::ParameterSet const& conf):
    alg_(conf.getParameter<double>("seedThreshold"),
	 conf.getParameter<double>("towerThreshold"),
	 conf.getParameter<double>("coneRadius"),
	 conf.getParameter<double>("coneAreaFraction"),
	 conf.getParameter<int>("maxPairSize"),
	 conf.getParameter<int>("maxIterations"),
	 conf.getParameter<double>("overlapThreshold"),
	 conf.getUntrackedParameter<int>("debugLevel",0)),
    src_(conf.getParameter<string>( "src" ))
  {
    produces<CaloJetCollection>();
  }

  // Virtual destructor needed.
  MidpointJetProducer2::~MidpointJetProducer2() { }  

  // Functions that gets called by framework every event
  void MidpointJetProducer2::produce(edm::Event& e, const edm::EventSetup&)
  {
    // get input
    edm::Handle<CandidateCollection> towers;
    e.getByLabel( src_, towers );                    
    vector <const Candidate*> input;
    vector <ProtoJet2> output;
    // fill input
    input.reserve (towers->size ());
    CandidateCollection::const_iterator tower = towers->begin ();
    for (; tower != towers->end (); tower++) {
      input.push_back (&*tower); 
    }
    // run algorithm
    alg_.run (input, &output);
    // produce output collection
    auto_ptr<CaloJetCollection> result(new CaloJetCollection);  //Empty Jet Coll
    vector <ProtoJet2>::const_iterator protojet = output.begin ();
    CaloJetMaker jetMaker;
    for (; protojet != output.end (); protojet++) {
      result->push_back (jetMaker.makeCaloJet (*protojet));
    }
    // store output
    e.put(result);  //Puts Jet Collection into event
  }

}
