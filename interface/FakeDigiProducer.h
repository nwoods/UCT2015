#ifndef FAKEDIGIPRODUCER_h
#define FAKEDIGIPRODUCER_h


// system include files
#include <memory>
#include <iostream>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/Common/interface/SortedCollection.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveSample.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
//#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "CommonTools/UtilAlgos/interface/TFileService.h" 

//#include "TTree.h"

#include "L1Trigger/UCT2015/interface/CTPCard.h"

using namespace std;
using namespace edm;

//
// class declaration
//

class FakeDigiProducer : public edm::EDProducer
{
 public:
  explicit FakeDigiProducer(const edm::ParameterSet&);
  ~FakeDigiProducer();
  
   private:
  // General CMSSW members
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);

  // General data members
  unsigned run_;
  unsigned lumi_;
  unsigned event_;
  
  // User data members
  //  TTree* tree;
  const vector<string> ecalFileNames;
  const vector<string> hcalFileNames;
  const unsigned numDigis;
  const bool doDebug;

  EcalTrigPrimDigiCollection makeEcalCollectionFromFile(string fileName);
  EcalTrigPrimDigiCollection makeEcalCollection();
  HcalTrigPrimDigiCollection makeHcalCollectionFromFile(string fileName);
  HcalTrigPrimDigiCollection makeHcalCollection();
  vector<CTPOutput> getGoodFileEntries(string fileName);
};




#endif /* Header guard ifndef FAKEDIGIPRODUCER_h */
