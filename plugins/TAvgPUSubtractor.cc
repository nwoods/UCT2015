//
// Package:    TAvgPUSubtractor.cc
//
// Original Author:  Nate Woods (Wisconsin)
//         Created:  Aug. 9 2013
//


// system include files
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

using namespace std;
using namespace edm;


class TAvgPUSubtractor : public edm::EDProducer {
public:

  explicit TAvgPUSubtractor(const edm::ParameterSet&);

private:

  virtual void produce(edm::Event&, const edm::EventSetup&);

  // Calculates PU based on previous 256, adds current rank to pipeline
  // If rank>tAvgPUCut, leaves PU unchanged from previous event
  void updatePU(); 

  // Replaces region rank with (region rank - region PU) as found in updatePU()
  void subtractPU();

  inline unsigned getRegInd(unsigned eta, unsigned phi) const
  {
    return eta * 18 + phi;
  }

  auto_ptr<L1CaloRegionCollection> newRegions;
  auto_ptr<vector<float>> regPULevel;

  vector<unsigned> regionPUTotal;
  vector<list<unsigned>> prevRegRanks;
  const unsigned tAvgPUCut;

  const InputTag regionInputTag;
};

//
// constructors and destructor
//
TAvgPUSubtractor::TAvgPUSubtractor(const ParameterSet& iConfig) :
  tAvgPUCut(iConfig.getParameter<unsigned>("tAvgPUCut")),
  regionInputTag(iConfig.getParameter<InputTag>("regionSrc"))
{
  regionPUTotal = vector<unsigned>(396,0);
  for(unsigned i = 0; i < regionPUTotal.size(); ++i)
    prevRegRanks.push_back(list<unsigned>());

  produces<L1CaloRegionCollection>();
  produces<vector<float>>();
}


// ------------ method called for each event  ------------
void
TAvgPUSubtractor::produce(Event& iEvent, const EventSetup& iSetup)
{

  Handle<L1CaloRegionCollection> regionHandle;

  iEvent.getByLabel(regionInputTag, regionHandle);

  if(regionHandle->size() != 18*22)
    throw cms::Exception("BadRegions") << "incoming regions had "
				     << regionHandle->size()
				     << " regions!" << endl;

  newRegions = auto_ptr<L1CaloRegionCollection>(new L1CaloRegionCollection);
  newRegions->resize(22*18);

  regPULevel = auto_ptr<vector<float>>(new vector<float>());

  for(unsigned q = 0; q < regionHandle->size(); ++q)
    {
      unsigned eta = regionHandle->at(q).gctEta();
      unsigned phi = regionHandle->at(q).gctPhi();

      unsigned ind = getRegInd(eta,phi);

      newRegions->at(ind) = regionHandle->at(q);
    }

  updatePU();
  
  subtractPU();

  iEvent.put(newRegions);
  iEvent.put(regPULevel);
}



void TAvgPUSubtractor::updatePU()
{
  for(unsigned eta = 0; eta < 22; ++eta)
    {
      for(unsigned phi = 0; phi < 18; ++phi)
	{
	  unsigned ind = getRegInd(eta,phi);
	  
	  unsigned newRank = newRegions->at(ind).et();
	  
	  if(prevRegRanks.at(ind).size() == 0) // if fist item, PU=0
	    {
	      regionPUTotal.at(ind) = 0;
	    }

	  if(newRank < tAvgPUCut)
	    {
	      if(prevRegRanks.at(ind).size() == 257)
		{
		  unsigned totalSum = regionPUTotal.at(ind);// << 8;
		  totalSum -= prevRegRanks.at(ind).back();
		  prevRegRanks.at(ind).pop_back();
		  totalSum += prevRegRanks.at(ind).front();
		  regionPUTotal.at(ind) = totalSum;// >> 8;
		}
	      else if(prevRegRanks.at(ind).size() != 0)
		{
		  unsigned totalSum = regionPUTotal.at(ind);// * 
		  //(prevRegRanks.at(ind).size() - 1);
		  totalSum += prevRegRanks.at(ind).front();
		  regionPUTotal.at(ind) = totalSum;// / 
		  //prevRegRanks.at(ind).size();
		}
	  
	      prevRegRanks.at(ind).push_front(newRegions->at(ind).et());
	    }
	}
    }
}



void TAvgPUSubtractor::subtractPU()
{
  for(unsigned eta = 0; eta < 22; ++eta)
    {
      for(unsigned phi = 0; phi < 18; ++phi)
	{
	  unsigned ind = getRegInd(eta,phi);

	  unsigned newEt = newRegions->at(ind).et();
	  unsigned puLevel = regionPUTotal.at(ind);
	  if(prevRegRanks.at(ind).size() == 257)
	    puLevel /= 256;
	  else if(prevRegRanks.at(ind).size() != 0)
	    puLevel /= prevRegRanks.at(ind).size();

	  regPULevel->push_back((float) puLevel);

	  if(newEt > puLevel)
	    newEt -= puLevel;
	  else
	    newEt = 0;

	  uint16_t regRawData = newRegions->at(ind).raw();
	  // Last 10(Barrel/Endcap) or 8(HF) bits are Et. 
	  // Remove + replace with PU subtracted Et.
	  if(newRegions->at(ind).isHf())
	    regRawData = (regRawData & 0xFF00) + newEt;
	  else
	    regRawData = (regRawData & 0xFC00) + newEt;
	  newRegions->at(ind).setRawData((uint32_t)regRawData);
	}
    }
}

//define this as a plug-in
DEFINE_FWK_MODULE(TAvgPUSubtractor);
