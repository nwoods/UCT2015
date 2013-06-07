#ifndef RCTCABLES_h
#define RCTCABLES_h

/*
 * ============================================================     
 *    
 *       Filename:     RCTCables.h
 *    
 *    Description:     An emulator for the cabling system for the
 *                     upgrade calorimeter trigger cabling. Sends
 *                     digis to the correct card and selects the
 *                     digis for a given card from a collection.
 *    
 *         Author:     Nate Woods, nwoods@hep.wisc.edu
 *    Institution:     UW Madison
 *    
 * ============================================================    
 */    

// System include files
#include <iostream>
#include <vector>
#include <algorithm>
using std::vector;

// User include files
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveSample.h"
#include "DataFormats/Common/interface/SortedCollection.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace std;
using namespace edm;

struct detRegions
{
  unsigned etaReg;
  unsigned phiReg;
};

class RCTCables
{
 public:
  explicit RCTCables(const ParameterSet& config);
  ~RCTCables();
  unsigned getCTPIndex(int iEta, unsigned iPhi) const;

  template<typename digi>
    SortedCollection<digi> 
    selectDigis(unsigned CTPIndex,
		const SortedCollection<digi>& 
		digisIn) const
    {
      SortedCollection<digi> output = SortedCollection<digi>();
      detRegions regions = RCTCables::recoverRegions(CTPIndex);
      
      for(unsigned i = 0; i < digisIn.size(); ++i)
	{
	  // If the digi is in this region, return it
	  if((digisIn[i].id().ieta() <= iEtaBounds.at(regions.etaReg)
	      && (regions.etaReg == 0 ? 
		  true : 
		  digisIn[i].id().ieta() > iEtaBounds.at(regions.etaReg-1)))
	     && (unsigned(digisIn[i].id().iphi()) <= 
		 iPhiBounds.at(regions.phiReg)
		 && (regions.phiReg == 0 ? 
		     true : 
		     (unsigned(digisIn[i].id().iphi()) 
		      > iPhiBounds.at(regions.phiReg-1)))))
	    output.push_back(digisIn[i]);
	}
      return output;
    }

  inline unsigned getNCards() const {return nEtaRegions * nPhiRegions;}

 private:
  // Bounds are the MAXIMUM index handled by a given card:
  // if iEtaBound[0] == 4, then tower 4 is handled by card 0, 5 is not.
  // Does not check for towers that span multiple indices.
  vector<int> iEtaBounds;
  vector<unsigned> iPhiBounds;
  const unsigned nEtaRegions;
  const unsigned nPhiRegions;

  // Returns doublet [etaRegion, phiRegion]
  inline detRegions recoverRegions(unsigned ind) const
    {
      detRegions output;
      output.phiReg = ind % nPhiRegions; // Phi region
      output.etaReg = (ind - output.phiReg) / nPhiRegions; // Eta region
      return output;
    }

};
#endif /*end of include guard: RCTCables */
