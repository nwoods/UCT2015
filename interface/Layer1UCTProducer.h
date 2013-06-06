#ifndef LAYER1UCTPRODUCER_h
#define LAYER1UCTPRODUCER_h


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/Common/interface/SortedCollection.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h" 

/* #include "TTree.h" */
/* #include "TH1D.h" */

#include "CTPCard.h"
#include "RCTCables.h"

//
// class declaration
//

class Layer1UCTProducer : public edm::EDProducer
{
 public:
  explicit Layer1UCTProducer(const edm::ParameterSet&);
  ~Layer1UCTProducer();
  
  
 private:
  // General CMSSW members
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  
  // User members
  const edm::InputTag hcalDigiSrc;
  const edm::InputTag ecalDigis;
  const edm::ParameterSet cableParams;
  const bool doDebug;

/*   TTree* tree; */
/*   TH1D* etHisto; */
  unsigned run_;
  unsigned lumi_;
  unsigned event_;
};




#endif /* Header guard ifndef LAYER1UCTPRODUCER_h */
