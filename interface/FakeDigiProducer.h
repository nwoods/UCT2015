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

struct CTPOutput
{
  int ieta;
  unsigned iphi;
  unsigned et;
};

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

  // User function members
/*   template<typename Digi, typename DigiSample, typename TowerDetId> */
/*     SortedCollection<Digi>& makeCollectionFromFile(fstream& f) */
/*     { */
/*       if(!f.is_open()) */
/* 	throw cms::Exception("BadFile") << "File failed to open" << endl; */

/*       SortedCollection<Digi> output = SortedCollection<Digi>(numDigis); */

/*       unsigned nDigisPlaced = 0; */

/*       while(true) */
/* 	{ */
/* 	  int iEta; */
/* 	  unsigned iPhi; */
/* 	  unsigned et; */
/* 	  f >> iEta; */
/* 	  if(f.eof()) */
/* 	    throw cms::Exception("BadFile")  */
/* 	      << "Input file must have 3 columns" << endl; */
/* 	  f >> iPhi; */
/* 	  if(f.eof()) */
/* 	    throw cms::Exception("BadFile")  */
/* 	      << "Input file must have 3 columns" << endl; */
/* 	  f >> et; */
/* 	  TowerDetId tow = TowerDetId(iEta,iPhi); */
/* 	  Digi digi = Digi(tow); */
/* 	  digi.setSample(0,DigiSample(et%0x100)); */
/* 	  output.push_back(digi); */

/* 	  if(f.eof() || ++nDigisPlaced == numDigis) */
/* 	    break; */
/* 	} */
/*       if(nDigisPlaced != numDigis) */
/* 	cout << "WARNING The number of digis placed in the collection" */
/* 	     << " is wrong." << endl; */

/*       return output; */
/*     } */

/*   template<typename Digi, typename DigiSample, typename TowerDetId> */
/*     SortedCollection<Digi>& makeCollectionFromFile()  */
/*     { */
/*       SortedCollection<Digi> output = SortedCollection<Digi>(numDigis); */

/*       for(int iEta = -32; iEta <= 32; ++iEta) */
/* 	{ */
/* 	  if(iEta == 0) continue; */
/* 	  for(unsigned iPhi = 1; iPhi <= 72; ++iPhi) */
/* 	    { */
/* 	      TowerDetId tow = TowerDetId(iEta,iPhi); */
/* 	      Digi digi = Digi(tow); */
/* 	      digi.setSample(0,DigiSample(0)); */
/* 	      output.push_back(digi); */
/* 	    } */
/* 	} */
    
/*       return output; */
/*     } */

  EcalTrigPrimDigiCollection makeEcalCollectionFromFile(string fileName);
  EcalTrigPrimDigiCollection makeEcalCollection();
  HcalTrigPrimDigiCollection makeHcalCollectionFromFile(string fileName);
  HcalTrigPrimDigiCollection makeHcalCollection();
  vector<CTPOutput> getGoodFileEntries(string fileName);
};




#endif /* Header guard ifndef FAKEDIGIPRODUCER_h */
